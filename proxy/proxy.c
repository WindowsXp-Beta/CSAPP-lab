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
        return;
    }
}

/*
 * main - Main routine for the proxy program
 */
int main(int argc, char **argv)
{
    /* 作为server端 */
    int listenfd, connfd;
    /* 作为client端 */
    int clientfd;

    char hostname[MAXLINE], port[MAXLINE], path[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char buf_server[MAXLINE], buf_client[MAXLINE];
    char rep_from_server[MAXLINE], rep_head_to_client[MAXLINE], rep_body_to_client[MAXLINE];
    char logString[MAXLINE];

    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    rio_t rio_client, rio_server;
    size_t size;//size return from end server

    /* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }
    
    listenfd = Open_listenfd(argv[1]);
    while(1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);

        /* 处理http请求 */
        Rio_readinitb(&rio_client, connfd);
        Rio_readlineb_w(&rio_client, buf_client, MAXLINE);
        sscanf(buf_client, "%s %s %s", method, uri, version);
        
        /* close it from telnet, for debug */
        if(strcmp(method, "STOP") == 0) {
            Close(connfd);
            break;
        }

        if (parse_uri(uri, hostname, path, port) == -1) {
            fprintf(stderr, "Wrong uri\n");
            Close(connfd);
            continue;
        }

        /* 作为client向url发送请求 */
        clientfd = Open_clientfd(hostname, port);
        /* build HTTP request */
        sprintf(buf_server, "%s /%s %s\r\n", method, path, "HTTP/1.1");

        do {
            Rio_readlineb_w(&rio_client, buf_client, MAXLINE);
            sprintf(buf_server, "%s%s", buf_server, buf_client);
        } while(strcmp(buf_client, "\r\n"));
        // printf("request header is\n%s", buf_server);
        /* send Http request */
        Rio_writen_w(clientfd, buf_server, strlen(buf_server));

        /* receive reponse header from server and set reponse header to client */
        Rio_readinitb(&rio_server, clientfd);
        do {
            Rio_readlineb_w(&rio_server, rep_from_server, MAXLINE);
            if(strncasecmp(rep_from_server, "Content-Length", 14) == 0) {
                size = strtol(rep_from_server + 16, NULL, 10);
                // printf("%lu\n", size);
            }
            sprintf(rep_head_to_client, "%s%s", rep_head_to_client, rep_from_server);
        } while(strcmp(rep_from_server, "\r\n"));
        format_log_entry(logString, &clientaddr, uri, size);
        printf("%s\n", logString);
        /* sent response header to client */
        Rio_writen_w(connfd, rep_head_to_client, strlen(rep_head_to_client));

        /* sent response body to client */
        while(size > 0) {
            size -= Rio_readlineb_w(&rio_server, rep_from_server, MAXLINE);
            Rio_writen_w(connfd, rep_from_server, strlen(rep_from_server));
        }


        Close(clientfd);
        Close(connfd);
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