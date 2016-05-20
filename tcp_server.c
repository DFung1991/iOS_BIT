#include "tcp_server.h"

int tcp_new_server(const int port ,struct tcp_server **ts){
    struct sockaddr_in sa;
    //int retval;
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0){
        perror("socket");
        goto err;
    }
    bzero(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
        perror("bind");
        goto err;
    }
    (*ts) = (struct tcp_server *) malloc(sizeof(struct tcp_server));
    if ((*ts) == NULL) {
        fprintf(stderr, "Alloc Failed\n");
        goto err;
    }
    (*ts)->fd = fd;
    return 0;
err:
    if (fd > 0) close(fd);
    return -1;
}

int tcp_new_client(int fd, struct tcp_client **tc){
    (*tc) = (struct tcp_client *)malloc(sizeof(struct tcp_client));
    if ((*tc) == NULL) {
        fprintf(stderr, "Alloc Failed\n");
        goto err;
    }
    (*tc)->fd = fd;
    return 0;
err:
    return -1;
}

int tcp_listen(struct tcp_server *ts, const int max){
    if (ts == NULL || ts->fd <= 0) goto err;

    if (listen(ts->fd, max) < 0){
        perror("listen");
        goto err;
    }
    return 0;

err:
    return -1;
}

int tcp_accept(struct tcp_server *ts, struct sockaddr *addr, socklen_t *len){
    if (ts == NULL || ts->fd <= 0) goto err;

    int fd = accept(ts->fd, addr, len);

    return fd;
err:
    return -1;
}

int tcp_server_close(struct tcp_server *ts){
    if (ts == NULL || ts->fd <= 0) goto err;
    int retval = close(ts->fd);
    if (retval < 0){
        perror("close");
        goto err;
    }
    free(ts);
    return retval;
err:
    return -1;
}

#if 0
int main(int argc, char **argv){

    struct tcp_server *ts = NULL;

    printf("new tcp server ...");
    if (tcp_new_server(atoi(argv[1]), &ts) < 0){
        goto err;
    }
    printf("ok\n");

    printf("tcp_listening ...");
    if(tcp_listen(ts, 10) < 0){
        goto err;
    }
    printf("ok\n");

    printf("tcp_accepting ...");
    int fd = tcp_accept(ts, NULL, NULL);
    if (fd < 0){
        goto err;
    }
    printf("ok\n");

    struct tcp_client *tc = NULL;

    printf("new tcp_client ...");
    if (tcp_new_client(fd, &tc) < 0){
        goto err;
    }
    printf("ok\n");

    printf("input msg:");
    char msg[1024];
    memset(msg, 0, 1024);
    char *get = fgets(msg, 1024, stdin);
    printf("writing ...");
    if (tcp_write(tc, msg, strlen(msg)) < 0){
        fprintf(stderr, "Tcp Write Failed\n");
        goto err;
    }
    printf("ok\n");

    printf("reading ...");
    char buf[1500];
    memset(buf, 0, 1500);
    if (tcp_read(tc, buf, 1500) < 0){
        fprintf(stderr, "Tcp Read Failed\n");
        goto err;
    }
    printf("ok\n");
    fprintf(stdout, "%s\n", buf);
    tcp_close(tc);


err:
    tcp_server_close(ts);
    return -1;
}
#endif
