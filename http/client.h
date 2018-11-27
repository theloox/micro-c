

#ifndef XHTTP_CLIENT
#define XHTTP_CLIENT

#include <lxStr.h>

#include <common.h>


int xhttp_client(xhttp_response *response, xhttp_request *req, int port);

int xhttp_send_request(xhttp_request *req, int soc);

int xhttp_get_response(xhttp_response *response, int soc);

#endif
