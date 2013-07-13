/*********************************************************************/
/*Filename : sgsock.h   sgsockdll header file                                            */ 
/*Created 02/10/98   by Scott Daugherty, Smith Gardner and Assoc Inc.*/
/*********************************************************************/

#ifdef WIN32

#ifdef _EXPORTING
        #define SGA_DECLSPEC      __declspec(dllexport)
#else
        #define SGA_DECLSPEC      __declspec(dllimport)
#endif

#include <windows.h>
//#include <winsock.h>
#include <time.h>
#include <direct.h>

#else
#define SOCKET int
#include <errno.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/time.h>
#ifdef SOLARIS2
#include <netinet/in_systm.h>
#endif
#include <netdb.h>
#include <netinet/ip.h>
/*
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#include <arpa/inet.h>
*/

#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "httpd.h"
#ifndef GBSTRUCT_INCL
#define  GBSTRUCT_INCL
#include "gbstruct.h"
#endif

#ifdef WIN32

#define NO_FLAGS_SET            0

#define USE_SHUTDOWN            1
#define DONT_USE_SHUTDOWN       0
 
//SGA_DECLSPEC SOCKET FAR PASCAL 
//sga_connect(const char FAR *hostname, const char FAR *service, const char FAR *progname, int *err_no);
//SGA_DECLSPEC int FAR PASCAL sga_close(SOCKET s);
//SGA_DECLSPEC int FAR PASCAL sga_send(SOCKET s, const char FAR *buf, int len);
//SGA_DECLSPEC int FAR PASCAL sga_recv(SOCKET s, char FAR *buf, int len);

SOCKET FAR PASCAL sga_connect(const char FAR *hostname, const char FAR *service, 
                const char FAR *progname, int *err_no, request_rec *, struct global_struct*);
int FAR PASCAL sga_close(SOCKET s, int shutdown_switch, request_rec *r, 
                                                                                                                struct global_struct*);

int FAR PASCAL sga_send(SOCKET s, const char FAR *buf, int len, request_rec *, struct global_struct*);
int FAR PASCAL sga_recv_old(SOCKET s, char FAR *buf, int len, request_rec *, struct global_struct*);

int FAR PASCAL sga_recv(SOCKET s, char FAR *buf, int len, request_rec *, struct global_struct*);
int FAR PASCAL sga_recv_new_1(SOCKET s, char FAR *buf, int length, request_rec *, struct global_struct*);
int sga_close2(SOCKET s, int shutdown_switch, request_rec *r, struct global_struct*gbp);


char *get_sock_error_code(struct global_struct*);

#define SYSTEM_BUFFER_LENGTH 30000
#define PACKET_SIZE 1460

#else

/*********************************************************************/

#ifdef LINUX
#include <linux/limits.h>
#else
#include <limits.h>
#endif

#include <unistd.h>

//typedef int SOCKET;

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define _MAX_PATH PATH_MAX 

#define NO_FLAGS_SET            0

#define USE_SHUTDOWN            1
#define DONT_USE_SHUTDOWN       0
 

SOCKET sga_connect(const char *hostname, const char *service, 
                const char *progname, int *err_no, request_rec *, struct global_struct*);
int sga_close(SOCKET s, int shutdown_switch, request_rec *, struct global_struct*);
int sga_send(SOCKET s, const char *buf, int len, request_rec *, struct global_struct*);
int sga_recv(SOCKET s, char *buf, int len, request_rec *, struct global_struct*);

char *get_sock_error_code(struct global_struct*);

#define SYSTEM_BUFFER_LENGTH 30000
#define PACKET_SIZE 1460

#endif
