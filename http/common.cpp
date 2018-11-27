
#include <unistd.h>

#include <lxSTR.h>

#include <common.h>

int xhttp_get_line(lxStr *str, int soc)
{
    int res;
    int ret = -1;
    char buf[128];

    str->Clear();

    while (1)
    {
        res = read(soc, buf, 1);
        if (res == -1)
        {
            //perror("read");
            return (-1);
        }
        else if (res == 0)
        {
            //printf("EOF\n");
            return (-1);
        }

        if (buf[0] == '\n')
            break;

        str->strcat(buf[0]);
    }

    if (str->Len() > 0 && (*str)[str->Len() - 1] == '\r')
        *str = str->substr(0, str->Len() - 1);

    if (str->Len() == 1 && (*str)[0] == '\r')
        *str = "";

    //printf("line: [%s]\n", (const char *)*str);
    return (str->Len());
}

int xhttp_get_request(xhttp_request *req, int soc)
{
    int res;
    int ret = -1;
    int cl = 0;
    char buf[2048];

    lxStr lin;
    lxStr proto;
    lxStr method;
    lxStr url;


    res = xhttp_get_line(&lin, soc);
    if (res < 0)
    {
        return (-1);
    }

    int l, r;

    l = lin.strpos(" ");
    r = lin.rstrpos(" ");

    method = lin.substr(0, l);
    proto = lin.substr(r + 1);
    url = lin.substr(l + 1, r - l - 1);
    //printf("parsed: [%s] [%s] [%s]\n", (const char *)method, (const char *)proto, (const char *)url);

    if (method.Len() == 0 || proto.Len() == 0 || url.Len() == 0)
        goto done;

    req->url = url;
    req->method = method;

    while (1)
    {
        res = xhttp_get_line(&lin, soc);
        if (res < 0)
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

    if (cl <= 0)
        goto done;


    r = cl;

    while ((res = read(soc, buf, sizeof(buf))) > 0)
    {
        //printf("read %u bytes: %.*s\n", res, res, buf);

        if (res == -1)
        {
            //perror("read");
            return (-1);
        }
        else if (res == 0)
        {
            //printf("EOF\n");
            //close(soc);
            return (0);
        }

        r -= res;

        req->body.strcat(buf, res);

        if (r <= 0)
        {
            ret = cl;
            break;
        }
    }

done:

    return (ret);
}


#define XHTTP_ERROR "\
HTTP/1.0 %d error\n\
Content-Length: %d\n\
\n\
error: %d\n"

int xhttp_send_error(int err, int soc)
{
    int res;
    int ret = 0;
    char buf[1024];

    res = snprintf(buf, sizeof (buf), XHTTP_ERROR, err, 11 + 1, err);

    res = write(soc, buf, 1024 > res ? res : 1024);

    if (res == -1)
    {
        //perror("read");
        ret = -1;
    }

    close(soc);

    return (0);
}


#define XHTTP_HEADER "\
HTTP/1.0 200 ok\n\
Content-Length: %lu\n\
\n"

int xhttp_send_response(lxStr response, int soc)
{
    int res;
    int ret = -1;
    char buf[2048];
    const char *bp;
    size_t rem;


    if (response.Len() == 0)
    {
        xhttp_send_error(400, soc);
        goto done;
    }

    res = snprintf(buf, sizeof (buf), XHTTP_HEADER, response.Len());

    res = write(soc, buf, 1024 > res ? res : 1024);

    if (res == -1)
    {
        //perror("read");
        ret = -1;
        goto done;
    }


    rem = response.Len();
    bp = (const char *)response;

    while ((res = write(soc, bp, 1024 > rem ? rem : 1024)) > 0)
    {
        //printf("write %u rem %d\n", res, rem);

        if (res == -1)
        {
            //perror("read");
            return (-1);
        }
        else if (res == 0)
        {
            //printf("EOF\n");
            //close(soc);
            return (0);
        }

        rem -= res;
        bp += res;

        if (rem <= 0)
        {
            ret = res;
            break;
        }
    }

done:

    close(soc);

    return (ret);
}


int parse_request_body(std::map<std::string, std::string> *header, lxStr *str)
{
    lxStr head, tail;
    lxStr k, v;


    tail = *str;

    while (2)
    {
        head = tail.strtokl("\n");
        tail = tail.strtokr("\n");
        head.ltrim();

        if (head.Len() == 0)
            break;

        k = head.strtokl(":");
        v = head.strtokr(":");

        v.ltrim();

        //printf("head[%s][%s]\n", (const char *)k, (const char *)v);
        header->insert(std::pair<std::string, std::string>((const char *)k, (const char *)v));
    }

    return (0);
}

int parse_response_body_invoice(lxStr *result, std::vector<invoice> *records, lxStr *str)
{
    lxStr head, tail;
    lxStr h, t;
    lxStr k, v;
    invoice inv;
    std::map<std::string, std::string> ele;


    tail = *str;

    head = tail.strtokl("---\n");
    tail = tail.strtokr("---\n");

    *result = head;

    if (tail.Len() == 0)
        return (0);

    while (1)
    {
        head = tail.strtokl("---\n");
        tail = tail.strtokr("---\n");

        //printf("head[%s]\n", (const char *)head);

        if (head.Len() == 0)
            break;

        t = head;
        ele.clear();

        while (2)
        {
            h = t.strtokl("\n");
            t = t.strtokr("\n");
            h.ltrim();

            if (h.Len() == 0)
                break;

            k = h.strtokl(":");
            v = h.strtokr(":");

            v.ltrim();

            //printf("h[%s][%s]\n", (const char *)k, (const char *)v);
            ele.insert(std::pair<std::string, std::string>((const char *)k, (const char *)v));
        }

        inv.id = strtol(ele["id"].c_str(), NULL, 10);
        inv.uid = strtol(ele["uid"].c_str(), NULL, 10);
        inv.utime = strtol(ele["utime"].c_str(), NULL, 10);
        inv.client = strtol(ele["client"].c_str(), NULL, 10);
        inv.items = strtol(ele["items"].c_str(), NULL, 10);
        inv.amount = strtod(ele["amount"].c_str(), NULL);

        //printf("h[%s][%s]\n", (const char *)k, (const char *)v);
        records->push_back(inv);
    }

    return (0);
}
