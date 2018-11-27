

#ifndef XHTTP_COMMON
#define XHTTP_COMMON

#include <list>
#include <map>
#include <string>
#include <vector>

#include <lxSTR.h>

typedef struct _xhttp_response {
    lxStr proto;
    int result;
    int length;
    lxStr body;
} xhttp_response;

typedef struct _xhttp_request {
    lxStr url;
    lxStr method;
    int result;
    lxStr body;
} xhttp_request;


int xhttp_get_line(lxStr *str, int soc);
int xhttp_get_request(xhttp_request *req, int soc);

int xhttp_send_error(int err, int soc);
int xhttp_send_response(lxStr response, int soc);

int parse_request_body(std::map<std::string, std::string> *header, lxStr *str);

typedef struct _invoice {
    int id;
    long uid;
    time_t utime;
    int client;
    int items;
    double amount;
} invoice;
int parse_response_body_invoice(lxStr *result, std::vector<invoice> *records, lxStr *str);

#endif
