#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
//#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/un.h>
#include <unistd.h>

#include <lxSTR.h>

#include <common.h>

#include <client.h>

int xhttp_client(xhttp_response *response, xhttp_request *req, int port)
{
    int soc;
    struct sockaddr_in sin;
    char buf[2048];
    int ret = 0;


    if ((soc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        exit(-1);
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

    sin.sin_addr.s_addr = 0x0100007F;

    if (connect(soc, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
        perror("connect error");
        exit(-1);
    }


    ret = xhttp_send_request(req, soc);

    if (ret < 0)
        return (ret);

    ret = xhttp_get_response(response, soc);

    return (ret);
}


#define XHTTP_REQUEST "\
%s %s HTTP/1.0\n\
Content-Length: %lu\n\
\n"

int xhttp_send_request(xhttp_request *req, int soc)
{
    int r;
    int ret = 0;
    char buf[2048];
    const char *bp;
    size_t rem;


    r = snprintf(buf, sizeof (buf), XHTTP_REQUEST, (const char *)req->method, (const char *)req->url, req->body.Len());

    r = write(soc, buf, 1024 > r ? r : 1024);

    if (r == -1)
    {
        //perror("read");
        ret = -1;
        goto done;
    }


   if (req->body.Len() == 0)
    {
        goto done;
    }


    rem = req->body.Len();
    bp = (const char *)req->body;

    while ((r = write(soc, bp, 1024 > rem ? rem : 1024)) > 0)
    {
        //printf("write %u rem %d\n", r, rem);

        if (r == -1)
        {
            //perror("read");
            return (-1);
        }
        else if (r == 0)
        {
            //printf("EOF\n");
            //close(soc);
            return (0);
        }

        rem -= r;
        bp += r;

        if (rem <= 0)
        {
            ret = r;
            break;
        }
    }

done:

    return (ret);
}


int xhttp_get_response(xhttp_response *res, int soc)
{
    int re;
    int ret = -1;
    int cl = 0;
    char buf[2048];

    lxStr lin;
    lxStr proto;
    int result;
    lxStr result_;
    lxStr msg;
    int length;

    res->body.Clear();


    re = xhttp_get_line(&lin, soc);
    if (re < 0)
    {
        return (-1);
    }

    int l, r;

    l = lin.strpos(" ");
    r = lin.rstrpos(" ");

    proto = lin.substr(0, l);
    msg = lin.substr(r + 1);
    result = lin.substr(l + 1, r - l - 1).Int();

    //printf("parsed: [%s] [%s] [%d]\n", (const char *)proto, (const char *)msg, result);

    if (proto.Len() == 0 || result == 0 || msg.Len() == 0)
        goto done;

    res->proto = proto;
    res->result = result;


    while (1)
    {
        re = xhttp_get_line(&lin, soc);
        if (re < 0)
        {
            return (-1);
        }
        if (lin.Len() == 0)
            break;

        lxStr k = lin.strtokl(":");
        lxStr v = lin.strtokr(":");

        v.trim();

        //printf("[%s][%s]\n", (const char *)k, (const char *)v);

        if (k == "Content-Length")
            cl = v.Int();
    }

    if (cl < 0)
        goto done;

    r = cl;

    while ((re = read(soc, buf, sizeof(buf))) > 0)
    {
        //printf("read %u bytes: %.*s\n", res, res, buf);

        if (re == -1)
        {
            //perror("read");
            return (-1);
        }
        else if (re == 0)
        {
            //printf("EOF\n");
            //close(soc);
            return (0);
        }

        r -= re;

        res->body.strcat(buf, re);

        if (r <= 0)
        {
            ret = cl;
            break;
        }
    }

done:

    close (soc);

    return (ret);
}
