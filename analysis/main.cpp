
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

void abnormal(xhttp_request *req, int soc)
{
    int r;
    int cli;
    invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    lxStr url;
    std::vector<invoice> records;
    invoice tgt;
    std::vector<invoice>::reverse_iterator it;


    url = req->url.substr(1);

    if (url == "abnormal")
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

    tgt.id = 0;

    int nrecords = 0;
    double sum = 0;

    for (it = records.rbegin(); it != records.rend(); ++it)
    {
        if (cli != it->client)
            continue;

        if (tgt.id == 0)
        {
            tgt = *it;
            continue;
        }

        ++nrecords;
        sum += it->amount;
    }

    if (tgt.id == 0)
        ret = "result: error\nmessage: client not found\n";
    else if (nrecords == 0)
        ret = "result: error\nmessage: no previous data\n";
    else if (nrecords < 2)
        ret = "result: error\nmessage: not enough data (2 or more)\n";
    else if (tgt.amount < ((sum / nrecords) * 0.50))
    {
        snprintf(buf, sizeof(buf), "result: ok\nabnormal: below\nid: %d\ncurrentamount: %.02f\naversge: %.02f\n", tgt.id, tgt.amount, (sum / nrecords));
        ret = buf;
    }
    else if (tgt.amount > ((sum / nrecords) * 1.50))
    {
        snprintf(buf, sizeof(buf), "result: ok\nabnormal: above\nid: %d\ncurrentamount: %.02f\naversge: %0.2f\n", tgt.id, tgt.amount, (sum / nrecords));
        ret = buf;
    }


    xhttp_send_response(ret, soc);

}

void duplicates(xhttp_request *req, int soc)
{
    int r;
    //invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    std::vector<invoice> records;
    std::vector<invoice>::reverse_iterator it, ti;
    std::vector<int> used;
    std::vector<int>::iterator di;
    std::map<int, std::vector<int> > dups;


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


    time_t now = time(NULL);

    for (it = records.rbegin(); it != records.rend(); ++it)
    {
        //printf("it->id %d\n",it->id);

        for (di = used.begin(); di != used.end(); ++di)
        {
            if (*di == it->id)
                break;
        }

        if (di != used.end())
        {
            //printf("used\n");
            continue;
        }


        for (ti = it + 1; ti != records.rend(); ++ti)
        {
            //printf("testing ti->id %d\n", ti->id);

            if (now - 3600 > ti->utime)
                continue;

            if (it->client != ti->client)
                continue;

            if (it->amount != ti->amount)
                continue;

            for (di = dups[it->id].begin(); di != dups[it->id].end(); ++di)
            {
                if (*di == ti->id)
                    break;
            }

            if (di != dups[it->id].end())
                continue;

            //printf("found dupe\n");

            used.push_back(ti->id);
            dups[it->id].push_back(ti->id);

        }
    }


    if (dups.empty())
    {
        ret = "result: ok\nmessage: no records found\n";
        xhttp_send_response(ret, soc);
        return;
    }

    ret = "result: ok\n";
    std::map<int, std::vector<int> >::iterator mi;

    for (mi = dups.begin(); mi != dups.end(); ++mi)
    {

        snprintf(buf, sizeof (buf), "---\n id: %d\n duplicates:\n", mi->first);
        ret.strcat(buf);

        for (di = mi->second.begin(); di != mi->second.end(); ++di)
        {
            snprintf(buf, sizeof (buf), "  - %d\n", *di);
            ret.strcat(buf);
        }

    }


    xhttp_send_response(ret, soc);

}

void same(xhttp_request *req, int soc)
{
    int r;
    //invoice inv;
    char buf[1024];
    xhttp_request req_inv;
    xhttp_response res;
    lxStr ret;
    lxStr result;
    std::vector<invoice> records;
    std::vector<invoice>::reverse_iterator it, ti;
    std::vector<int> used;
    std::vector<int>::iterator di;
    std::map<int, std::vector<int> > dups;


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


    time_t now = time(NULL);

    for (it = records.rbegin(); it != records.rend(); ++it)
    {
        //printf("it->id %d\n",it->id);

        for (di = used.begin(); di != used.end(); ++di)
        {
            if (*di == it->id)
                break;
        }

        if (di != used.end())
        {
            //printf("used\n");
            continue;
        }


        for (ti = it + 1; ti != records.rend(); ++ti)
        {
            //printf("testing ti->id %d\n", ti->id);
            //printf("uids %lu %lu\n", it->uid, ti->uid);

            if (it->uid != ti->uid)
                continue;

            for (di = dups[it->id].begin(); di != dups[it->id].end(); ++di)
            {
                if (*di == ti->id)
                    break;
            }

            if (di != dups[it->id].end())
                continue;

            //printf("found dupe\n");

            used.push_back(ti->id);
            dups[it->id].push_back(ti->id);

        }
    }


    if (dups.empty())
    {
        ret = "result: ok\nmessage: no records found\n";
        xhttp_send_response(ret, soc);
        return;
    }

    ret = "result: ok\n";
    std::map<int, std::vector<int> >::iterator mi;

    for (mi = dups.begin(); mi != dups.end(); ++mi)
    {

        snprintf(buf, sizeof (buf), "---\n id: %d\n sameuid:\n", mi->first);
        ret.strcat(buf);

        for (di = mi->second.begin(); di != mi->second.end(); ++di)
        {
            snprintf(buf, sizeof (buf), "  - %d\n", *di);
            ret.strcat(buf);
        }

    }


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

    else if (req.url == "/abnormal" || req.url.StartsWith("/abnormal/"))
        abnormal(&req, soc);

    else if (req.url == "/duplicates")
        duplicates(&req, soc);

    else if (req.url == "/same")
        same(&req, soc);

    else
        xhttp_send_error(404, soc);


    return (NULL);
}


int main(int argc, char **argv)
{

    xhttp_server(A_PORT, handler);

    return 0;
}
