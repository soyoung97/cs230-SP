#include <stdio.h>
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

//pre-definations of functions:
void serve(char * buf, char * method, char * suffix, char * hostname );
void read_requesthdrs(rio_t *rp);
void work(int fd);
void parse_uri(char *uri, char * hostname, char * port, char * suffix);
void sighandler(int sig);
void sigchldhandler(int sig);
int main(int argc, char ** argv)
{
    Signal(SIGPIPE, sighandler);//managing signals.
    Signal(SIGCHLD, sigchldhandler);
    int listenfd, clientfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    listenfd=Open_listenfd(argv[1]);//argv[0]: ./proxy, argv[1]: port number.
    while (1){
        clientlen= sizeof(clientaddr);
        clientfd= Accept(listenfd, (SA *)&clientaddr, &clientlen);
        //to deal with multiple concurrent requests. Implemented fork().
        if (fork()==0){
            work(clientfd);
            Close(clientfd);
            exit(0);
        }
    Close(clientfd);
    }
    return 0;
}
/* work: read request line&headers. modify request line, get hostname, connects with proxy&server,
            and write it to the server file descriptor.
            */
void work(int fd){
    int  serverfd, byte;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], hostname[MAXLINE];
    char suffix[MAXLINE], port[MAXLINE];
    rio_t rioforserver;
    rio_t rioforclient;
    /* Read request line and headers */
    Rio_readinitb(&rioforclient,fd);
    Rio_readlineb(&rioforclient, buf, MAXLINE);
    //ex: GET http://www.cmu.edu/hub/index.html HTTP/1.1
    sscanf(buf, "%s %s %s", method, uri, version);
    parse_uri(uri, hostname, port, suffix);
    serverfd= open_clientfd(hostname, port);//connects with proxy and server
    serve(buf, method, suffix, hostname);
    Rio_readinitb(&rioforserver,serverfd);
    rio_writen(serverfd, buf, strlen(buf));
    while((byte=(rio_readlineb(&rioforserver, buf, MAX_OBJECT_SIZE)))){
        rio_writen(fd, buf, byte);
        }
}
//parse_uri : parse the uri and store right information in following variables: hostname, port and suffix.
void parse_uri(char *uri, char * hostname, char * port, char * suffix)
{//make it into GET /hub/index.html HTTP/1.0
    char *ptr;
    uri+=7;
    ptr=index(uri, '/');
    strcpy(suffix, ptr);
    *ptr='\0';//break the request line for further use.
    ptr=index(uri, ':');
    if (ptr){
    strcpy(port, ptr+1);
    *ptr='\0';
    }
    else//predefined as 80.
        strcpy(port,"80");
    strcpy(hostname,uri);
 }
//serve: store correct data into buffer in the right order.
void serve(char * buf, char * method, char * suffix, char * hostname){
    sprintf(buf, "%s %s %s\r\n",method, suffix, "HTTP/1.0");
    sprintf(buf, "%sHost: %s\r\n",buf,hostname);
    sprintf(buf, "%s%s",buf,user_agent_hdr);
    sprintf(buf, "%sConnection: close\r\n",buf);
    sprintf(buf, "%sProxy-Connection: close\r\n\r\n",buf);
}
void sighandler(int sig){//just return for SIGPIPE.
    return;
}
void sigchldhandler(int sig){
    while (waitpid(-1,NULL,WNOHANG)>0){
    }
    return;
}













