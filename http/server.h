

#ifndef XHTTP_SERVER
#define XHTTP_SERVER

#ifdef __cplusplus
extern "C" {
#endif


#define A_PORT 12001
#define I_PORT 12002
#define R_PORT 12003

int xhttp_server(int port, void *func(void *));

#ifdef __cplusplus
}
#endif

#endif
