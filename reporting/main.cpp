
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <xhttp.h>
#include <common.h>


void root(xhttp_request *req, int soc)
{
    lxStr response;

    response = "hello root report\n";

    if (req->body.Len() > 5)
        xhttp_send_response(response, soc);
    else
        xhttp_send_error(400, soc);
}

void all(xhttp_request *req, int soc)
{
    int r;
    //invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    std::vector<invoice> records;
    std::vector<invoice>::iterator it;


    req_inv.method = "GET";
    req_inv.url = "/all";

    r = xhttp_client(&res, &req_inv, I_PORT);


    if (r < 0)
    {
        xhttp_send_error(400, soc);
        return;
    }

    if (!res.body.StartsWith("result: ok"))
    {
        xhttp_send_error(500, soc);
        return;
    }


    parse_response_body_invoice(&result, &records, &res.body);

    ret = "result: ok\n";

    for (it = records.begin(); it != records.end(); ++it)
    {
        snprintf(buf, sizeof (buf), "---\n id: %d\n utime: %lu\n client: %d\n items: %d\n amount: %.02f\n report: %d\n", it->id, it->utime, it->client, it->items, it->amount, it->id);
        ret.strcat(buf);
    }

    xhttp_send_response(ret, soc);

}

void client(xhttp_request *req, int soc)
{
    int r;
    int cli;
    //invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    lxStr url;
    std::vector<invoice> records;
    std::vector<invoice>::iterator it;


    url = req->url.substr(1);

    if (url == "client")
    {
        xhttp_send_response("result: error\nmessage: client not found\n", soc);
        return;
    }

    cli = strtol((const char *)url.strtokr("/"), NULL, 10);

    if (cli <= 0)
    {
        xhttp_send_response("result: error\nmessage: client not found\n", soc);
        return;
    }


    req_inv.method = "GET";
    req_inv.url = "/all";

    r = xhttp_client(&res, &req_inv, I_PORT);

    if (r < 0)
    {
        xhttp_send_error(400, soc);
        return;
    }

    if (!res.body.StartsWith("result: ok"))
    {
        xhttp_send_error(500, soc);
        return;
    }


    parse_response_body_invoice(&result, &records, &res.body);

    ret = "result: ok\n";

    int nrecords = 0;
    for (it = records.begin(); it != records.end(); ++it)
    {
        if (cli != it->client)
            continue;

        ++nrecords;

        snprintf(buf, sizeof (buf), "---\n id: %d\n utime:%lu\n client: %d\n items: %d\n amount: %.02f\n report: %d\n", it->id, it->utime, it->client, it->items, it->amount, it->id);
        ret.strcat(buf);
    }

    if (nrecords == 0)
        ret = "result: error\nmessage: client not found\n";

    xhttp_send_response(ret, soc);

}

void last(xhttp_request *req, int soc)
{
    int r;
    int lst;
    time_t ti;
    //invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    lxStr url;
    std::vector<invoice> records;
    std::vector<invoice>::iterator it;


    url = req->url.substr(1);

    if (url == "last")
    {
        xhttp_send_response("result: error\nmessage: period not valid\n", soc);
        return;
    }

    lst = strtol((const char *)url.strtokr("/"), NULL, 10);

    if (lst <= 0)
    {
        xhttp_send_response("result: error\nmessage: period not valid\n", soc);
        return;
    }


    req_inv.method = "GET";
    req_inv.url = "/all";

    r = xhttp_client(&res, &req_inv, I_PORT);

    if (r < 0)
    {
        xhttp_send_error(400, soc);
        return;
    }

    if (!res.body.StartsWith("result: ok"))
    {
        xhttp_send_error(500, soc);
        return;
    }


    parse_response_body_invoice(&result, &records, &res.body);

    ret = "result: ok\n";

    ti = time(NULL) - lst;
    int nrecords = 0;
    for (it = records.begin(); it != records.end(); ++it)
    {
        if (ti > it->utime)
            continue;

        ++nrecords;

        snprintf(buf, sizeof (buf), "---\n id: %d\n utime:%lu\n client: %d\n items: %d\n amount: %.02f\n report: %d\n", it->id, it->utime, it->client, it->items, it->amount, it->id);
        ret.strcat(buf);
    }

    if (nrecords == 0)
        ret = "result: error\nmessage: no records found\n";

    xhttp_send_response(ret, soc);

}

void today(xhttp_request *req, int soc)
{
    int r;
    time_t ti, to;
    //invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    lxStr url;
    std::vector<invoice> records;
    std::vector<invoice>::iterator it;


    req_inv.method = "GET";
    req_inv.url = "/all";

    r = xhttp_client(&res, &req_inv, I_PORT);

    if (r < 0)
    {
        xhttp_send_error(400, soc);
        return;
    }

    if (!res.body.StartsWith("result: ok"))
    {
        xhttp_send_error(500, soc);
        return;
    }


    parse_response_body_invoice(&result, &records, &res.body);

    ret = "result: ok\n";


    ti = time(NULL);
    //ti = ti - (ti % 86400);

    struct tm ts;
    localtime_r(&ti, &ts);

    ts.tm_sec = 0;
    ts.tm_min = 0;
    ts.tm_hour = 0;

    to = mktime(&ts);

    int nrecords = 0;
    for (it = records.begin(); it != records.end(); ++it)
    {
        if (to > it->utime)
            continue;

        ++nrecords;

        snprintf(buf, sizeof (buf), "---\n id: %d\n utime:%lu\n client: %d\n items: %d\n amount: %.02f\n report: %d\n", it->id, it->utime, it->client, it->items, it->amount, it->id);
        ret.strcat(buf);
    }

    if (nrecords == 0)
        ret = "result: error\nmessage: no records found\n";

    xhttp_send_response(ret, soc);

}


void *handler(void *soc_)
{
    int soc;
    int res;
    char buf[2048];

    soc = *(int *)soc_;
    free(soc_);

    //printf("soc:%d\n\n", soc);

    xhttp_request req;
    res = xhttp_get_request(&req, soc);

    printf("req:[%s][%s]\n\n", (const char *)req.url, (const char *)req.body);

    if (req.url == "/")
        root(&req,soc);

    else if (req.url == "/all")
        all(&req, soc);

    else if (req.url == "/client" || req.url.StartsWith("/client/"))
        client(&req, soc);

    else if (req.url == "/last" || req.url.StartsWith("/last/"))
        last(&req, soc);

    else if (req.url == "/today")
        today(&req, soc);

    else
        xhttp_send_error(404, soc);


    return (NULL);
}


int main(int argc, char **argv)
{

    xhttp_server(R_PORT, handler);

    return 0;
}
