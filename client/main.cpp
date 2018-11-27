
#include <stdio.h>
#include <string.h>

#include <client.h>
#include <server.h>

#include <lxSTR.h>


void analysis()
{
    int r;
    xhttp_request req;
    xhttp_response res;
    lxStr result;
    std::vector<invoice> records;
    std::vector<invoice>::iterator it;


    printf("================\n");

    printf("Testing root\n");
    req.url = "/";
    req.method = "GET";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing root POST\n");
    req.url = "/";
    req.method = "POST";
    req.body = "qwerty";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
    req.body.Clear();


    printf("Testing abnormal client #1\n");
    req.url = "/abnormal/1";
    req.method = "GET";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing create\n");
    req.url = "/create";
    req.method = "POST";
    req.body = " client: 1\n items:42\n amount: 4200.00\n";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);

    printf("Testing abnormal client #1\n");
    req.url = "/abnormal/1";
    req.method = "GET";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing duplicates\n");
    req.url = "/duplicates";
    req.method = "GET";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing same uid\n");
    req.url = "/same";
    req.method = "GET";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing create\n");
    req.url = "/create";
    req.method = "POST";
    req.body = " client: 3\n items:33\n amount: 330.00\n uid: 33333333\n";
    r = xhttp_client(&res, &req, I_PORT);
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);


    printf("Testing same uid\n");
    req.url = "/same";
    req.method = "GET";
    r = xhttp_client(&res, &req, A_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

}

void invoicing()
{
    int r;
    xhttp_request req;
    xhttp_response res;

    printf("================\n");

    printf("Testing root\n");
    req.url = "/";
    req.method = "GET";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing root POST\n");
    req.url = "/";
    req.method = "POST";
    req.body = "qwerty";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
    req.body.Clear();


    printf("Testing dummy create\n");
    req.url = "/create";
    req.method = "POST";
    req.body = " client: 0\n items:42\n amount: 0.00";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing create\n");
    req.url = "/create";
    req.method = "POST";
    req.body = " client: 1\n items:42\n amount: 420.00";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);

    req.body = " client: 2\n items:13\n amount: 130.00";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);

    req.body = " client: 3\n items:1\n amount: 10.00";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
    req.body.Clear();


    printf("Testing all\n");
    req.url = "/all";
    req.method = "GET";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing read #1\n");
    req.url = "/read/1";
    req.method = "GET";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing read #728 (not found)\n");
    req.url = "/read/728";
    req.method = "GET";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing update #3 items 1->3\n");
    req.url = "/update";
    req.method = "PUT";
    req.body = " id: 3\n client: 3\n items:3\n amount: 30.00";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing update #728 (not found)\n");
    req.url = "/update";
    req.method = "PUT";
    req.body = " client: 3\n items:3\n amount: 30.00";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
    req.body.Clear();

    printf("Testing all\n");
    req.url = "/all";
    req.method = "GET";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing delete #2\n");
    req.url = "/delete";
    req.method = "DELETE";
    req.body = " id: 2\n";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing delete #728 (not found)\n");
    req.url = "/delete";
    req.method = "DELETE";
    req.body = " id: 728\n";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
    req.body.Clear();

    printf("Testing all\n");
    req.url = "/all";
    req.method = "GET";
    r = xhttp_client(&res, &req, I_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

}

void reporting()
{
    int r;
    xhttp_request req;
    xhttp_response res;
    lxStr result;
    std::vector<invoice> records;
    std::vector<invoice>::iterator it;


    printf("================\n");

    printf("Testing root\n");
    req.url = "/";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing root POST\n");
    req.url = "/";
    req.method = "POST";
    req.body = "qwerty";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
    req.body.Clear();


    printf("Testing all\n");
    req.url = "/all";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing client(field) #1\n");
    req.url = "/client/1";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing client(field) #728 (not found)\n");
    req.url = "/client/728";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing last 60 (seconds)\n");
    req.url = "/last/1";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing last 3600 (seconds)\n");
    req.url = "/last/3600";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


    printf("Testing today\n");
    req.url = "/today";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");


#if 0
    printf("Testing read #1\n");
    req.url = "/read/1";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");

    printf("Testing read #728 (not found)\n");
    req.url = "/read/728";
    req.method = "GET";
    r = xhttp_client(&res, &req, R_PORT);
    printf("%s\n", (const char *)res.body);
    printf("================\n");
#endif // 0

}


int main(int argc, char **argv)
{

    if (argc < 2)
    {
        printf("Usage: xclient analysis|invoicing|reporting\n");
        return (1);
    }

    if (strncmp(argv[1], "analysis", 32) == 0)
        analysis();
    else if (strncmp(argv[1], "invoicing", 32) == 0)
        invoicing();
    else if (strncmp(argv[1], "reporting", 32) == 0)
        reporting();
    else
        printf("Usage: xclient analysis|invoicing|reporting\n");


    return (0);
}
