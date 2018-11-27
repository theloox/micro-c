
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <xhttp.h>
#include <common.h>


std::list<invoice> invoices;

pthread_mutex_t mutex;


void root(xhttp_request *req, int soc)
{
    lxStr response;

    response = "hello root\n";

    if (req->body.Len() > 5)
        xhttp_send_response(response, soc);
    else
        xhttp_send_error(400, soc);
}

void create(xhttp_request *req, int soc)
{
    int id;
    invoice inv;
    std::list<invoice>::iterator it;
    std::map<std::string, std::string> header;

    parse_request_body(&header, &req->body);

    inv.id = 0;
    inv.utime = time(NULL);
    inv.client = strtol(header["client"].c_str(), NULL, 10);
    inv.items = strtol(header["items"].c_str(), NULL,10);
    inv.amount = strtod(header["amount"].c_str(), NULL);
    if (header["uid"] == "")
        inv.uid = random();
    else
        inv.uid = strtol(header["uid"].c_str(), NULL,10);


    pthread_mutex_lock(&mutex);

    if (inv.client > 0)
    {
        if (invoices.empty())
            id = 0;
        else
            id = invoices.back().id;

        inv.id = id + 1;

        invoices.insert(invoices.end(), inv);
    }

    pthread_mutex_unlock(&mutex);

    xhttp_send_response("result: ok\n", soc);

}

void rd(xhttp_request *req, int soc)
{
    int id;
    char buf[1024];
    lxStr url;
    lxStr ret;
    std::list<invoice>::iterator it;


    url = req->url.substr(1);

    if (url == "read")
    {
        xhttp_send_response("result: error\nmessage: id not found\n", soc);
        return;
    }

    id = strtol((const char *)url.strtokr("/"), NULL, 10);

    if (id <= 0)
    {
        xhttp_send_response("result: error\nmessage: id not found\n", soc);
        return;
    }


    pthread_mutex_lock(&mutex);

    for (it = invoices.begin(); it != invoices.end(); ++it)
    {

        if (id == it->id)
        {
            snprintf(buf, sizeof (buf), "result: ok\n---\n id: %d\n uid: %lu\n utime: %li\n client: %d\n items: %d\n amount: %.02f\n", it->id, it->uid, it->utime, it->client, it->items, it->amount);
            ret = buf;

            break;
        }
    }

    if (it == invoices.end())
    {
        ret = "result: error\nmessage: id not found\n";
    }

    pthread_mutex_unlock(&mutex);

    xhttp_send_response(ret, soc);
}

void update(xhttp_request *req, int soc)
{
    int id;
    char buf[1024];
    invoice inv;
    lxStr ret;
    std::list<invoice>::iterator it, er;
    std::map<std::string, std::string> header;

    parse_request_body(&header, &req->body);

    inv.id = strtol(header["id"].c_str(), NULL, 10);
    inv.client = strtol(header["client"].c_str(), NULL, 10);
    inv.items = strtol(header["items"].c_str(), NULL,10);
    inv.amount = strtod(header["amount"].c_str(), NULL);

    id = strtol(header["id"].c_str(), NULL, 10);

    if (id <= 0)
    {
        xhttp_send_response("result: error\nmessage: id not found\n", soc);
        return;
    }


    pthread_mutex_lock(&mutex);

    for (it = invoices.begin(); it != invoices.end(); ++it)
    {

        if (id == it->id)
        {
            er = it;

            snprintf(buf, sizeof (buf), "result: ok\n");
            ret = buf;

            break;
        }
    }

    if (it == invoices.end())
    {
        ret = "result: error\nmessage: id not found\n";
    }
    else
    {
        inv.uid = er->uid;
        inv.utime = er->utime;

        *er = inv;
    }

    pthread_mutex_unlock(&mutex);

    xhttp_send_response(ret, soc);

}

void del(xhttp_request *req, int soc)
{
    int id;
    char buf[1024];
    lxStr url;
    lxStr ret;
    std::list<invoice>::iterator it, er;
    std::map<std::string, std::string> header;


    parse_request_body(&header, &req->body);

    id = strtol(header["id"].c_str(), NULL, 10);

    if (id <= 0)
    {
        xhttp_send_response("result: error\nmessage: id not found\n", soc);
        return;
    }


    pthread_mutex_lock(&mutex);

    for (it = invoices.begin(); it != invoices.end(); ++it)
    {

        if (id == it->id)
        {
            er = it;

            snprintf(buf, sizeof (buf), "result: ok\n");
            ret = buf;

            break;
        }
    }

    if (it == invoices.end())
    {
        ret = "result: error\nmessage: id not found\n";
    }
    else
    {
        invoices.erase(er);
    }

    pthread_mutex_unlock(&mutex);

    xhttp_send_response(ret, soc);

}

void all(xhttp_request *req, int soc)
{
    invoice inv;
    char buf[1024];
    lxStr ret;
    std::list<invoice>::iterator it;
    std::map<std::string, std::string> header;

    ret = "result: ok\n";

    pthread_mutex_lock(&mutex);

    for (it = invoices.begin(); it != invoices.end(); ++it)
    {
        //snprintf(buf, sizeof (buf), "---\n id: %d\n client: %d\n items: %d\n amount: %.02f\n", it->id, it->client, it->items, it->amount);
        snprintf(buf, sizeof (buf), "---\n id: %d\n uid: %lu\n utime: %lu\n client: %d\n items: %d\n amount: %.02f\n", it->id, it->uid, it->utime, it->client, it->items, it->amount);
        ret.strcat(buf);
    }

    pthread_mutex_unlock(&mutex);

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

    else if (req.url == "/create")
        create(&req, soc);

    else if (req.url == "/read" || req.url.StartsWith("/read/"))
        rd(&req, soc);

    else if (req.url == "/update")
        update(&req, soc);

    else if (req.url == "/delete")
        del(&req, soc);

    else if (req.url == "/all")
        all(&req, soc);

    else
        xhttp_send_error(400, soc);


    return (NULL);
}


int main(int argc, char **argv)
{

    pthread_mutex_init(&mutex, NULL);

    srandom(time(NULL));

    xhttp_server(I_PORT, handler);

    return 0;
}
