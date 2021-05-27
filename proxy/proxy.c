/*
 * proxy.c - ICS Web proxy
 *
 *
 */

#include "csapp.h"
#include <stdarg.h>
#include <sys/select.h>

/*
 * Function prototypes
 */
int parse_uri(char *uri, char *target_addr, char *path, char *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, size_t size);
sem_t mutex;
ssize_t Rio_readlineb_w(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
    {
        fprintf(stderr, "Rio_readlineb error: %s\n", strerror(errno));
        return 0;
    }
    return rc;
}

ssize_t Rio_readnb_w(rio_t *rp, void *usrbuf, size_t n)
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
    {
        fprintf(stderr, "Rio_readnb error: %s\n", strerror(errno));
        return 0;
    }
    return rc;
}

void Rio_writen_w(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n)
    {
        fprintf(stderr, "Rio_writen error: %s\n", strerror(errno));
        fflush(stderr);
        return;
    }
}

void *handle_one_client(void* *params) {

    int connfd = *((int*)params[0]);
    int clientfd;
    struct sockaddr_storage clientaddr = *((struct sockaddr_storage *)params[1]);
    Pthread_detach(pthread_self());
    Free(params[0]);
    Free(params[1]);
    Free(params);
    char hostname[MAXLINE]={'\0'}, port[MAXLINE]={'\0'}, path[MAXLINE]={'\0'}, method[MAXLINE]={'\0'}, uri[MAXLINE]={'\0'}, version[MAXLINE]={'\0'};
    char req_to_server[MAXLINE]={'\0'}, req_from_client[MAXLINE]={'\0'};
    char resp_from_server[MAXLINE]={'\0'}, resp_head_to_client[MAXLINE]={'\0'};
    char logString[MAXLINE]={'\0'};
    ssize_t get = 0, size = 0, cont_length = 0;
    rio_t rio_client, rio_server;
    FILE *logfile;

    /* 处理http请求 */
    Rio_readinitb(&rio_client, connfd);
    if(0 == Rio_readlineb_w(&rio_client, req_from_client, MAXLINE)) {
        fprintf(stderr, "Wrong request from client\n");
        Close(connfd);
        return NULL;
    }
    if(3 != sscanf(req_from_client, "%s %s %s", method, uri, version)) {
        fprintf(stderr, "Wrong request format\n");
        Close(connfd);
        return NULL;
    }
    // printf("request is \n%s", req_from_client);
    /* close it from telnet, for debug */
    if(strcmp(method, "STOP") == 0) {
        Close(connfd);
        return NULL;
    }

    if (parse_uri(uri, hostname, path, port) == -1) {
        fprintf(stderr, "Wrong uri\n");
        Close(connfd);
        return NULL;
    }

    /* 作为client向url发送请求 */
    clientfd = Open_clientfd(hostname, port);
    /* build HTTP request */
    /* build header */
    sprintf(req_to_server, "%s /%s %s\r\n", method, path, version);
    Rio_writen_w(clientfd, req_to_server, strlen(req_to_server));
    memset(req_to_server, 0, MAXLINE);

    while(strcmp(req_from_client, "\r\n") && (get = Rio_readlineb_w(&rio_client, req_from_client, MAXLINE)) != 0) {
        if(strncasecmp(req_from_client, "Content-Length", 14) == 0) {
            cont_length = strtol(req_from_client + 16, NULL, 10);
        }
        sprintf(req_to_server, "%s%s", req_to_server, req_from_client);
    }
    if (0 == get) {
        fprintf(stderr, "Wrong request header from client\n");
        Close(connfd);
        Close(clientfd);
        return NULL;
    }
    // printf("request header is\n%s", req_to_server);
    /* send Http request header */
    Rio_writen_w(clientfd, req_to_server, strlen(req_to_server));
    /* build request body if method not GET */
    if(strcmp(method, "GET")) {
        while(cont_length > 0) {
            if(Rio_readnb_w(&rio_client, req_from_client, 1)) {
                cont_length--;
                Rio_writen_w(clientfd, req_from_client, 1);
            }
            else break;
        }
        // while(cont_length > 0) {
        //     size_t rec_size = (cont_length < MAXLINE) ? cont_length : MAXLINE;
        //     ssize_t len;
        //     if ((len = Rio_readlineb_w(&rio_client, req_from_client, rec_size + 1)) > 0) {
        //         cont_length -= len;
        //         Rio_writen_w(clientfd, req_from_client, len);
        //     }
        //     else break;
        // }
        if(cont_length > 0) {
            fprintf(stderr, "Wrong request body\n");
            Close(connfd);
            Close(clientfd);
            return NULL;
        }
    }
    
    /* receive reponse from server and set reponse to client */
    Rio_readinitb(&rio_server, clientfd);
    /* receive response */
    if((get = Rio_readlineb_w(&rio_server, resp_from_server, MAXLINE)) == 0) {
        fprintf(stderr, "Wrong response from server\n");
        Close(connfd);
        Close(clientfd);
        return NULL;
    }
    /* receive response header */
    do {
        size += get;
        if(strncasecmp(resp_from_server, "Content-Length", 14) == 0) {
            cont_length = strtol(resp_from_server + 16, NULL, 10);
        }
        sprintf(resp_head_to_client, "%s%s", resp_head_to_client, resp_from_server);
    } while(strcmp(resp_from_server, "\r\n") && ((get = Rio_readlineb_w(&rio_server, resp_from_server, MAXLINE)) != 0));
        
    if (0 == get) {
        fprintf(stderr, "Wrong response header from server\n");
        Close(connfd);
        Close(clientfd);
        return NULL;
    }

    /* sent response header to client */
    Rio_writen_w(connfd, resp_head_to_client, strlen(resp_head_to_client));

    /* sent response body to client */
    // while(cont_length > 0) {
    //     size_t rec_size = (cont_length < MAXLINE) ? cont_length : MAXLINE;
    //     ssize_t len;
    //     if ((len = Rio_readlineb_w(&rio_server, resp_from_server, rec_size + 1)) > 0) {
    //         cont_length -= len;
    //         size += len;
    //         Rio_writen_w(connfd, resp_from_server, len);
    //     }
    //     else break;
    // }
    // while(cont_length > 0) {//这样就可以，等到读完了cont_length就退出
    //     printf("execute and length is %lu\n", cont_length);
    //     size_t rec_size = (cont_length < MAXLINE) ? cont_length : MAXLINE;
    //     ssize_t len = Rio_readlineb_w(&rio_server, resp_from_server, rec_size + 1);//加一的原因见下一条
    //     printf("len is %lu and strlen is %lu\n", len, strlen(resp_from_server));
    //     cont_length -= len;
    //     size += len;
    //     Rio_writen_w(connfd, resp_from_server, len);
    // }
    while(cont_length > 0) {
        if (Rio_readnb_w(&rio_server, resp_from_server, 1)) {
            cont_length--;
            size++;
            Rio_writen_w(connfd, resp_from_server, 1);
        }
        else break;
    }
    if(cont_length > 0) {
        fprintf(stderr, "Wrong response body\n");
        Close(connfd);
        Close(clientfd);
        return NULL;
    }

    format_log_entry(logString, &clientaddr, uri, size);
    P(&mutex);
    logfile = fopen("proxy.log", "a");
    printf("%s\n", logString);
    fclose(logfile);
    V(&mutex);

    Close(clientfd);
    Close(connfd);
    return NULL; 
}

/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
    /* ignore SIGPIPE */
    Signal(SIGPIPE, SIG_IGN);
    int listenfd;
    socklen_t clientlen;
    pthread_t tid;

    Sem_init(&mutex, 0, 1);
    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }
    
    listenfd = Open_listenfd(argv[1]);
    while(1) {
        void* *params = Malloc(2 * sizeof(void*));
        params[0] = Malloc(sizeof(int));
        params[1] = Malloc(sizeof(struct sockaddr_storage));
        clientlen = sizeof(struct sockaddr_storage);
        *((int*)params[0]) = Accept(listenfd, (SA *)params[1], &clientlen);
        Pthread_create(&tid, NULL, handle_one_client, params);
    }
    exit(0);
}


/*
 * parse_uri - URI parser
 *
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *hostname, char *pathname, char *port)
{
    char *hostbegin;
    char *hostend;
    char *pathbegin;
    int len;

    if (strncasecmp(uri, "http://", 7) != 0) {
        hostname[0] = '\0';
        return -1;
    }

    /* Extract the host name */
    hostbegin = uri + 7;
    hostend = strpbrk(hostbegin, " :/\r\n\0");
    if (hostend == NULL)
        return -1;
    len = hostend - hostbegin;
    strncpy(hostname, hostbegin, len);
    hostname[len] = '\0';

    /* Extract the port number */
    if (*hostend == ':') {
        char *p = hostend + 1;
        while (isdigit(*p))
            *port++ = *p++;
        *port = '\0';
    } else {
        strcpy(port, "80");
    }

    /* Extract the path */
    pathbegin = strchr(hostbegin, '/');
    if (pathbegin == NULL) {
        pathname[0] = '\0';
    }
    else {
        pathbegin++;
        strcpy(pathname, pathbegin);
    }

    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), the number of bytes
 * from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
                      char *uri, size_t size)
{
    time_t now;
    char time_str[MAXLINE];
    char host[INET_ADDRSTRLEN];

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    if (inet_ntop(AF_INET, &sockaddr->sin_addr, host, sizeof(host)) == NULL)
        unix_error("Convert sockaddr_in to string representation failed\n");

    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %s %s %zu", time_str, host, uri, size);
}