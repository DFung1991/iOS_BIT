#include "tcp_client.h"

int tcp_connect(const char *ip, const int port, struct tcp_client **tc){
    struct sockaddr_in sa;
    int fd;
    int retval;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        goto err;
    }

    bzero(&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    sa.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
        perror("connect");
        goto err;
    }
    *tc = (struct tcp_client *)malloc(sizeof(struct tcp_client));
    if (*tc == NULL){
        fprintf(stderr, "Alloc Failed\n");
    }
    (*tc)->fd = fd;
    return 0;
err:
    if (fd > 0) close(fd);
    return -1;
}

int tcp_read(struct tcp_client *tc, unsigned char *buf, int size){
    if (tc == NULL || tc->fd <= 0) goto err;
    return recv(tc->fd, buf, size, 0);
err:
    return -1;
}

int tcp_write(struct tcp_client *tc, unsigned char *buf, int size){
    if (tc == NULL || tc->fd <= 0) goto err;
    return send(tc->fd, buf, size, 0);
err:
    return -1;
}

int tcp_close(struct tcp_client *tc){
    if (tc == NULL || tc->fd <= 0) goto err;
    int retval = close(tc->fd);
    if (retval < 0){
        perror("close");
        goto err;
    }
    free(tc);
    return retval;
err:
    return -1;
}

#if 0
int main(int argc, char **argv){
    struct tcp_client *tc = NULL;

    if (argc < 4) {
        fprintf(stdout, "Usaged: %s <ip> <port> <msg>\n");
        return 1;
    }
    printf("connecting ...");
    if (tcp_connect(argv[1], atoi(argv[2]), &tc) < 0){
        fprintf(stderr, "tcp Connect Failed\n");
        goto err;
    }
    printf("ok\n");

    printf("writing ...");
    if (tcp_write(tc, argv[3], strlen(argv[3])) < 0){
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
    return 0;
err:
    tcp_close(tc);
    return -1;;
}
#endif
