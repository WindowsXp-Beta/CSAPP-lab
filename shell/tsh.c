/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and ID here>
 * Author: Wei xinpeng
 * student ID: 519021910888
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
int Fork(void);
void Kill(int, int);
void Sigfillset(sigset_t *);
void Sigemptyset(sigset_t *);
void Sigaddset(sigset_t *, int);
void Sigprocmask(int ,sigset_t *, sigset_t *);
/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);

    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    /* Install the signal handlers */

    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 

    /* Initialize the job list */
    initjobs(jobs);

    /* Execute the shell's read/eval loop */
    while (1) {

	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}

	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    }

    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) //about 70 lines
{
    char *argv[MAXARGS];
    int bg;//should the job run in bg or fg
    pid_t pid;
    sigset_t mask_all, mask_one, prev_one;

    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one, SIGCHLD);

    bg = parseline(cmdline, argv);
    if (argv[0] == NULL) return;
    Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
    if (!builtin_cmd(argv)) {
        if ((pid = Fork()) == 0) { //run in child process
            Sigprocmask(SIG_SETMASK, &prev_one, NULL);
            setpgid(0, 0);//The reason of this line is in the last page of lab7.pdf.

            if (execve(argv[0], argv, environ) < 0) {
                printf("%s: Command not found\n", argv[0]);
                exit(0);
            }
        }
        
        if (!bg) {
            Sigprocmask(SIG_BLOCK, &mask_all, NULL);
            addjob(jobs, pid, FG, cmdline);
            waitfg(pid);
            Sigprocmask(SIG_SETMASK, &prev_one, NULL);
        }
        else {
            Sigprocmask(SIG_BLOCK, &mask_all, NULL);
            addjob(jobs, pid, BG, cmdline);
            Sigprocmask(SIG_SETMASK, &prev_one, NULL);
            struct job_t * bg_job = getjobpid(jobs, pid);
            printf("[%d] (%d) %s",bg_job -> jid ,bg_job -> pid, cmdline);
        }
    }
    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv)
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') { //back slash means escape
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;//第一个参数（一个字符串）的首地址
	*delim = '\0';//让这个参数字符串有结尾，将space或quote替换成\0
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 * tsh should support following built-in commands:
 * 1. The quit command terminates the shell.
 * 2. The jobs command lists all background jobs.
 * 3. The bg <job> command restarts <job> by sending it a SIGCONT signal, and then runs it in the background. The <job> argument can be either a PID or a JID.
 * 4. The fg <job> command restarts <job> by sending it a SIGCONT signal, and then runs it in the foreground. The <job> argument can be either a PID or a JID.
 */
int builtin_cmd(char **argv) //about 25 lines
{
    if (!strcmp(argv[0], "quit")) {
        exit(0);
    }
    else if (!strcmp(argv[0], "jobs")) {
        sigset_t mask_all, prev_all;
        Sigfillset(&mask_all);
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        listjobs(jobs);
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
        return 1;
    }
    else if (!strcmp(argv[0], "bg")) {
        do_bgfg(argv);
        return 1;
    }
    else if (!strcmp(argv[0], "fg")) {
        do_bgfg(argv);
        return 1;
    }
    return 0;     /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) //about 50 lines
{
    int bg = strcmp(argv[0], "bg");
    sigset_t mask_all, mask_one, prev_one;
    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one, SIGCHLD);    

    struct job_t *job_stp;
    if (!argv[1]) {
        printf("%s command requires PID or %%jobid argument\n", (!bg) ? "bg":"fg");
        return;
    }
    else if ((argv[1][0] < '0' || argv[1][0] > '9') && argv[1][0] != '%') {
        printf("%s: argument must be a PID or %%jobid\n", (!bg) ? "bg":"fg");
        return;
    }
    else if (argv[1][0] == '%') {
        int jid = 0;
        for (int i = 1; argv[1][i]; i++) {
            jid = jid * 10 + (argv[1][i] - '0');
        }
        job_stp = getjobjid(jobs, jid);
        if (!job_stp) {
            printf("%%%d: No such job\n",jid);
            return;
        }
    }
    else {
        pid_t pid = 0;
        for (int i = 0; argv[1][i]; i++) {
            pid = pid * 10 + (argv[1][i] - '0');
        }
        job_stp = getjobpid(jobs, pid);
        if (!job_stp) {
            printf("(%d): No such process\n",pid);
            return; 
        }
    }
    
    Sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
    Kill( -job_stp -> pid, SIGCONT);

    if (bg) {
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);
        job_stp -> state = FG;
        waitfg(job_stp -> pid);
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);        
    }
    else {
        Sigprocmask(SIG_BLOCK, &mask_all, NULL);
        job_stp -> state = BG;
        Sigprocmask(SIG_SETMASK, &prev_one, NULL);
        printf("[%d] (%d) %s",job_stp -> jid ,job_stp -> pid, job_stp -> cmdline);
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) //about 20 lines
{
    sigset_t mask;
    Sigemptyset(&mask);

    while (fgpid(jobs)) {
       sigsuspend(&mask);
    }
    return;
}

/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) 
{
    int olderrno = errno;
    struct job_t *job;
    sigset_t mask_all, prev_all;
    pid_t pid;
    int status;

    Sigfillset(&mask_all);
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        job = getjobpid(jobs, pid);
        int jid = job -> jid;
        int pid = job -> pid;
        if (!WIFSTOPPED(status)) deletejob(jobs, pid);
        Sigprocmask(SIG_SETMASK, &prev_all, NULL);
        if (WIFSIGNALED(status)) {
            printf("Job [%d] (%d) terminated by signal %d\n",jid ,pid, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status)) {
            job -> state = ST;
            printf("Job [%d] (%d) stopped by signal %d\n",jid ,pid, WSTOPSIG(status));
        }
    }
    // if (errno != ECHILD) {
    //     unix_error("waitpid error");
    // }
    errno = olderrno;
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig) 
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;

    Sigfillset(&mask_all);
    Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    int fg_pid = fgpid(jobs);
    Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    if (fg_pid) {//if there is no foreboard jobs
        Kill(-fg_pid, sig);
    }

    errno = olderrno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    int olderrno = errno;
    sigset_t mask_all, prev_all;

    Sigfillset(&mask_all);
    Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    int fg_pid = fgpid(jobs);
    Sigprocmask(SIG_SETMASK, &prev_all, NULL);
    if (fg_pid) {
        Kill(-fg_pid, sig);
    }

    errno = olderrno;
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;//如果被删除的是当前最大的JID，那么下一个插入便会占用这个JID
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}

/* Wrapper of Unix system functions */
int Fork(void) {
    pid_t pid;

    if ((pid = fork()) < 0) {
        unix_error("Fork error");
    }
    return pid;
}

void Kill(int pid, int sig) {
    if (kill(pid, sig) < 0) {
        unix_error("Kill error");
    }
}

void Sigfillset(sigset_t *mask) {
    if (sigfillset(mask) < 0) {
        unix_error("Sigfillset error");
    }
}

void Sigemptyset(sigset_t *mask) {
    if (sigemptyset(mask) < 0) {
        unix_error("Sigemptyset error");
    }
}

void Sigaddset(sigset_t *mask, int sig) {
    if (sigaddset(mask, sig) < 0) {
        unix_error("Sigaddset error");
    }
}

void Sigprocmask(int sig, sigset_t *mask, sigset_t *mask_prev) {
    if (sigprocmask(sig, mask, mask_prev) < 0) {
        unix_error("Sigprocmask error");
    }
}