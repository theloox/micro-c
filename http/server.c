
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/un.h>
//#include <unistd.h>


int xhttp_server(int port, void *func(void *))
{
    struct sockaddr_in sin;
    char buf[100];
    int fd, cl, rc;

    signal(SIGPIPE,SIG_IGN);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        exit(-1);
    }

    int optval = 1;
	int optlen = sizeof (int);

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);
    setsockopt(fd, SOL_SOCKET, SO_LINGER, &optval, optlen);

    memset(&sin, 0, sizeof(sin));
    sin.sin_port = htons(port);
	sin.sin_family = AF_INET;
	//sin.sin_addr.s_addr = htonl(interface);

    if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    {
        perror("bind error");
        exit(-1);
    }

    if (listen(fd, SOMAXCONN) == -1)
    {
        perror("listen error");
        exit(-1);
    }

    while (1)
    {
        if ((cl = accept(fd, NULL, NULL)) == -1)
        {
            perror("accept error");
            continue;
        }

        int *par;
        par = (int *)malloc(sizeof (int));

        *par = cl;

        pthread_t thr;
        pthread_create(&thr, NULL, func, par);

/*
        while ((rc = read(cl, buf, sizeof(buf))) > 0)
        {
            printf("read %u bytes: %.*s\n", rc, rc, buf);
        }
        if (rc == -1)
        {
            perror("read");
            exit(-1);
        }
        else if (rc == 0)
        {
            printf("EOF\n");
            close(cl);
        }
*/
    }


    return 0;
}
