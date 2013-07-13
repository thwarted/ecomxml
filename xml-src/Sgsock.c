/*********************************************************************/
/* Filename : sgsock.c   Sprocket communications source file         */
/*                                                                   */
/* Updated 07/06/99 - RBS   Unix and NT in same file                 */
/*********************************************************************/

#include "sgsock.h"
#include "httpd.h"

//#define PRINTDEBUG 
#ifdef PRINTDEBUG
void printline(char *);
#endif
extern int XML_Error(char*, char*, char*, char *, request_rec *, struct global_struct*);
extern void PrintHeader(void);

//char *wsecode = NULL;
#ifdef WIN32
char *get_wsastartup_error_code(int error, struct global_struct *);
#endif

/********************************************************************/
/*                                                                  */
/*  Socket Code                                                     */
/*                                                                  */
/********************************************************************/

/**********************/
/*  Connect           */
/**********************/
#ifdef WIN32
SOCKET FAR PASCAL sga_connect(const char FAR *hostname, const char FAR *service, 
			const char FAR *progname, int *error_num, request_rec *r, 
				struct global_struct*gbp)
#else
SOCKET sga_connect(const char *hostname, const char *service, const char *progname,
			int *error_num, request_rec *r, struct global_struct* gbp)
#endif
{
    int status = -1;
	u_long addr;
    int sockrecvlen = 0;
    int socksendlen = 0;
#ifdef PRINTDEBUG
	char *tempbuf;
#endif
    struct sockaddr_in sockaddr;

#ifdef WIN32
    char ecode[WSADESCRIPTION_LEN + WSASYS_STATUS_LEN + 80 +1];
//    PHOSTENT phe;
//    PSERVENT pse;
    WSADATA WSAData;
#else
    char ecode[80 +1];
//    struct hostent *phe;
//    struct servent *pse;
#endif

    sockrecvlen = sizeof(int);
    socksendlen = sizeof(int);
#ifdef PRINTDEBUG
    printline("----------------------------------");
    printline("> sga_connect                     ");
#endif

#ifdef WIN32
    memset(ecode, '\0', WSADESCRIPTION_LEN + WSASYS_STATUS_LEN + 80 +1);
    if((status = WSAStartup(MAKEWORD(1,1), &WSAData)) != 0)
    {
        sprintf(ecode, "%s", get_wsastartup_error_code(status,gbp));
        strcat(ecode, WSAData.szSystemStatus);
        strcat(ecode, WSAData.szDescription);
        XML_Error("sga_connect -> WSAStartup", "failed", "communications", ecode, r,gbp);
        error_num = (int *)status;

        return(INVALID_SOCKET);
    }
#else
    memset(ecode, '\0', 80 +1);
//	setservent(0);
#endif

#ifdef PRINTDEBUG
    printline("> Get host by name                ");
	if((tempbuf=malloc(80))==NULL)
	{
		return(-1);
	}
	sprintf(tempbuf,"Hostname = %s",hostname);
	printline(tempbuf);
#endif
	
    if((gbp->phe = gethostbyname(hostname)) == NULL)
    {
		if((int)(addr=inet_addr(hostname))!=-1)
		{
			gbp->phe=gethostbyaddr((char *)&addr,sizeof(addr),AF_INET);
			if(gbp->phe==NULL)
			{
			 XML_Error("sga_connect -> gethostbyname/addr", "failed", 
			"communications", get_sock_error_code(gbp),r,gbp);

			error_num = (int *)-1;
#ifdef WIN32
			WSACleanup();
#endif
#ifdef PRINTDEBUG
			free(tempbuf);
#endif
			return(INVALID_SOCKET);
			}
		}
	}

#ifdef PRINTDEBUG
    printline("> Get service by name             ");
	sprintf(tempbuf,"Service = %s %d",service,strlen(service));
	printline(tempbuf);

#endif
    if((gbp->pse = getservbyname(service, "tcp")) == NULL)
    {
        XML_Error("sga_connect -> getservbyname", "failed", "communications",
			get_sock_error_code(gbp),r,gbp);

        error_num = (int *)-1;
#ifdef WIN32
        WSACleanup();
#endif
#ifdef PRINTDEBUG
	free(tempbuf);
#endif
        return(INVALID_SOCKET);
    }
#ifdef PRINTDEBUG
	free(tempbuf);
#endif
#ifdef WIN32
    memcpy ((char FAR *)&(sockaddr.sin_addr.s_addr), gbp->phe->h_addr, gbp->phe->h_length);
#else
    memcpy ((char *)&(sockaddr.sin_addr.s_addr), gbp->phe->h_addr, gbp->phe->h_length);
#endif

    sockaddr.sin_family = (short)AF_INET;
    sockaddr.sin_port = gbp->pse->s_port;
                                                                                                        /* ?? IPPROTO_TCP); ?? */
                                                                                                        /* ?? IPPROTO_IP); ?? */
#ifdef PRINTDEBUG
    printline("> Create Socket                   ");
#endif
    if((gbp->sock = socket((int)AF_INET, (int)SOCK_STREAM, (int)IPPROTO_TCP)) == INVALID_SOCKET)
    {
        XML_Error("sga_connect -> socket", "failed", "communications", 
			get_sock_error_code(gbp),r,gbp);
        error_num = (int *)-1;
#ifdef WIN32
        WSACleanup();
#endif
        return(INVALID_SOCKET);
    }


#ifdef PRINTDEBUG
    printline("> Connect to Socket               ");
#endif
    if((status = connect(gbp->sock, (struct sockaddr *) &sockaddr, sizeof( sockaddr))) == SOCKET_ERROR)
    {
        XML_Error("sga_connect -> connect", "failed", "communications", 
			get_sock_error_code(gbp),r,gbp);
	    error_num = (int *)status;
    //  sga_close2(sock, (int)DONT_USE_SHUTDOWN);
        return(INVALID_SOCKET);
    }
#ifdef PRINTDEBUG
    printline("> Done in sga_connect             ");
#endif
    return(gbp->sock);
}


/**********************/
/*  Send              */
/**********************/
#ifdef WIN32
int FAR PASCAL sga_send(SOCKET s, const char FAR *buf, int len, request_rec *r,
					 struct global_struct*gbp)
#else
int sga_send(SOCKET s, const char *buf, int len, request_rec *r, 
			 struct global_struct*gbp)
#endif
{
    gbp->success = -1;
	gbp->totleft = len;

	do {
		gbp->thissend = gbp->totleft;
		if(gbp->thissend > 25000) {
			gbp->thissend = 25000;
		}

#ifdef PRINTDEBUG
    printline("> In sga_send                     ");
    printline("> Sending Data                    ");
#endif
    if((gbp->success = send(s, buf+gbp->bytes_sent, gbp->thissend, NO_FLAGS_SET)) == SOCKET_ERROR)
    {
        XML_Error("sga_send -> send", "failed", "communications",
			get_sock_error_code(gbp),r,gbp);

        sga_close2(s, (int)USE_SHUTDOWN,r,gbp);
  //  if(wsecode != NULL)
  //      free(wsecode);
    //  if(strlen(wsecode) != 0)
    //      free(wsecode);
        return(gbp->success);
    }
	gbp->bytes_sent+=gbp->success;
	gbp->totleft-=gbp->thissend;

	}while (gbp->totleft>0);

#ifdef PRINTDEBUG
    printline("> Sending EOS                     ");
#endif
    if((gbp->success = send(s, "**ENDOFSTREAM**", 15, NO_FLAGS_SET)) == SOCKET_ERROR)
    {
        XML_Error("sga_send -> send", "failed", "communications",
			get_sock_error_code(gbp),r,gbp);

        sga_close2(s, (int)USE_SHUTDOWN,r,gbp);
  //  if(wsecode != NULL)
  //      free(wsecode);
    //  if(strlen(wsecode) != 0)
    //      free(wsecode);
        return(gbp->success);
    }
#ifdef PRINTDEBUG
    printline("> Done in sga_send                ");
#endif
    return(gbp->bytes_sent);
}

/**********************/
/*  Receive           */
/**********************/
#ifdef WIN32
int FAR PASCAL sga_recv(SOCKET s, char FAR *buf, int len, request_rec *r,
						struct global_struct*gbp)
#else
int sga_recv(SOCKET s, char *buf, int len, request_rec *r,
			 struct global_struct*gbp)
#endif
{
    int status = 0;
    int pos = 0;
    int curpos=0;

    char *buf2;
#ifdef PRINTDEBUG
    char *tempbuf;

    printline("> In sga_recv                     ");
    if((tempbuf=malloc(80)) == NULL)
    {
        return(-1);
    }
#endif

    if((buf2 = malloc(SYSTEM_BUFFER_LENGTH + 1)) == NULL)
    {
        XML_Error("sga_recv -> malloc", "failed buffer allocation", "communications", "-1",r,gbp);
        sga_close2(s, (int)USE_SHUTDOWN,r,gbp);
#ifdef PRINTDEBUG
        free(tempbuf);
#endif
  //  if(wsecode != NULL)
  //      free(wsecode);
    //  if(strlen(wsecode) != 0)
    //      free(wsecode);
        return(-1);
    }

    do {

#ifdef PRINTDEBUG
        printline("Top of loop...");
#endif

        memset(buf2, '\0', SYSTEM_BUFFER_LENGTH + 1);

        if((status = recv(s, buf2, SYSTEM_BUFFER_LENGTH, NO_FLAGS_SET)) == SOCKET_ERROR)
        {
            XML_Error("sga_recv -> recv", "failed recv", "communications",
				get_sock_error_code(gbp),r,gbp);

            sga_close2(s, (int)USE_SHUTDOWN,r,gbp);
            free(buf2);
#ifdef PRINTDEBUG
            free(tempbuf);
#endif

         //   if(wsecode != NULL)
         //       free(wsecode);
            return(status);
        }

#ifdef PRINTDEBUG
        sprintf(tempbuf,"Got %d bytes this read...",status);
        printline(tempbuf);
#endif

        if (status==0) {
            XML_Error("sga_recv -> recv", "recieved 0 bytes", "communications", ".",r,gbp);
            return(-1);(curpos);
        }

#ifdef PRINTDEBUG
        sprintf(tempbuf,"Had before: %d",curpos);
        printline(tempbuf);
        printline(buf);
        printline("");
        sprintf(tempbuf,"Plus new  : %d",status);
        printline(tempbuf);
        printline(buf2);
        printline("");
#endif

        for(pos = 0; pos+curpos < len; pos++)
                    buf[curpos+pos] = buf2[pos];

        status+=curpos;
        if (status>len) {
            status=len;
        }
        curpos=status;

#ifdef PRINTDEBUG
        sprintf(tempbuf,"Total     : %d",status);
        printline(tempbuf);
        printline(buf);
        printline("");
        printline("Check EOS");
#endif

        /* Check for EOS after assembling the buffer */
        if (status > 15) {
            if (buf[status-15]=='*' &&
                buf[status-14]=='*' &&
                buf[status-13]=='E' &&
                buf[status-12]=='N' &&
                buf[status-11]=='D' &&
                buf[status-10]=='O' &&
                buf[status-9]== 'F' &&
                buf[status-8]== 'S' &&
                buf[status-7]== 'T' &&
                buf[status-6]== 'R' &&
                buf[status-5]== 'E' &&
                buf[status-4]== 'A' &&
                buf[status-3]== 'M' &&
                buf[status-2]== '*' &&
                buf[status-1]== '*') {
                status-=15;
                curpos=len;
                memset(buf+status,'\0',15);
#ifdef PRINTDEBUG
                printline("Got it");
#endif
            }
        }

#ifdef PRINTDEBUG
        sprintf(tempbuf,"Current pos: %d     Len: %d",curpos,len);
        printline(tempbuf);
#endif

    } while (curpos<len);

    buf[status] = '\0';
#ifdef PRINTDEBUG
    sprintf(tempbuf,"Done in sga_recv. Total is %d",status);
    printline(tempbuf);
    free(tempbuf);
#endif
    free(buf2);

    return (status);
}

/**********************/
/*  Close w/Shutdown  */
/**********************/
#ifdef WIN32
int FAR PASCAL sga_close(SOCKET s, int shutdown_switch, request_rec *r,
						 struct global_struct*gbp)
#else
int sga_close(SOCKET s, int shutdown_switch, request_rec *r,
			  struct global_struct*gbp)
#endif
{
    int how = 2;
    int status = -1;

#ifdef PRINTDEBUG
    printline("> In sga_close                    ");
    printline("> Shutdown Socket                 ");
#endif
    if((status = shutdown(s, how)) == SOCKET_ERROR)
    {
        XML_Error("sga_close -> shutdown", "failed", "communications",
			get_sock_error_code(gbp),r,gbp);
    }

#ifdef PRINTDEBUG
    printline("> Close Socket connection         ");
#endif
    if((status = closesocket(gbp->sock)) == SOCKET_ERROR)
    {
        XML_Error("sga_close -> closesocket", "failed", "communications",
			get_sock_error_code(gbp),r,gbp);
    }

#ifdef WIN32
#ifdef PRINTDEBUG
    printline("> Cleanup Socket                  ");
#endif
    WSACleanup();
#endif
#ifdef PRINTDEBUG
    printline("> Release wsecode                 ");
#endif
  //  if(wsecode != NULL)
  //      free(wsecode);
#ifdef PRINTDEBUG
    printline("> Done in sga_close               ");
#endif
    return(status);
}

/**********************/
/*  Close w/option    */
/**********************/

int sga_close2(SOCKET s, int shutdown_switch, request_rec *r,
			   struct global_struct*gbp)
{
    int how = 2;
    int status = -1;

#ifdef PRINTDEBUG
    printline("> In sga_close                    ");
#endif
    if(shutdown_switch == USE_SHUTDOWN)
#ifdef PRINTDEBUG
        printline("> Shutdown Socket                 ");
#endif
        if((status = shutdown(s, how)) == SOCKET_ERROR)
        {
            XML_Error("sga_close -> shutdown", "failed", "communications", 
				get_sock_error_code(gbp),r,gbp);
        }

#ifdef PRINTDEBUG
    printline("> Close Socket connection         ");
#endif
    if((status = closesocket(gbp->sock)) == SOCKET_ERROR)
    {
        XML_Error("sga_close -> closesocket", "failed", "communications", 
			get_sock_error_code(gbp),r,gbp);
    }

#ifdef WIN32
    WSACleanup();
#endif
 //   if(wsecode != NULL)
 //       free(wsecode);
    // if(strlen(wsecode) != 0)
    //     free(wsecode);
#ifdef PRINTDEBUG
    printline("> Done in sga_close2              ");
#endif
    return(status);
}

/**********************************/
/*  Get Error Code (WIN32 only)   */
/**********************************/
#ifdef WIN32
char *get_wsastartup_error_code(int error, struct global_struct *gbp)
{
#ifdef PRINTDEBUG
    printline("> In getwsastartup_error_code     ");
#endif
  //  if(wsecode != NULL)
  //      free(wsecode);

    if(error == WSASYSNOTREADY)
//        wsecode = strdup("WSASYSNOTREADY ");
		strcpy(gbp->wsecode,"WSASYSNOTREADY ");
    else if(error == WSAVERNOTSUPPORTED)
//        wsecode = strdup("WSAVERNOTSUPPORTED ");
		strcpy(gbp->wsecode,"WSAVERNOTSUPPORTED ");
    else if(error == WSAEINVAL)
//        wsecode = strdup("WSAEINVAL ");
		strcpy(gbp->wsecode,"WSAEINVAL ");

#ifdef PRINTDEBUG
    printline("> Done in wsastartup_error_code   ");
#endif
    return(gbp->wsecode);
}
#endif

/**********************/
/*  Sock Error Code   */
/**********************/
char *get_sock_error_code(struct global_struct* gbp)
{

    int error = -1;

#ifdef PRINTDEBUG
    printline("> In get_sock_error_code          ");
#endif
 //   if(wsecode != NULL)
 //       free(wsecode);

#ifdef WIN32

#ifdef PRINTDEBUG
    printline("> Get last error                  ");
#endif
    error = WSAGetLastError();

	switch (error)
	{
		case WSASYSNOTREADY: 
			return "WSASYSNOTREADY " ;
			break;
		case WSAVERNOTSUPPORTED: 
			return "WSAVERNOTSUPPORTED " ;
			break;
	
		case WSAEINVAL: 
			return "WSAEINVAL " ;
			break;
		case WSAHOST_NOT_FOUND: 
			return "WSAHOST_NOT_FOUND " ;
			break;
		case WSATRY_AGAIN: 
			return "WSATRY_AGAIN " ;
			break;
		case WSANO_RECOVERY: 
			return "WSANO_RECOVERY " ;
			break;
		case WSANOTINITIALISED: 
			return "WSANOTINITIALISED " ;
			break;
		case WSAENETDOWN: 
			return "WSAENETDOWN " ;
			break;
		case WSANO_DATA: 
			return "WSANO_DATA - check the \"SERVICES\" file " ;
			break;
		case WSAEINPROGRESS: 
			return "WSAEINPROGRESS " ;
			break;
		case WSAEINTR: 
			return "WSAEINTR " ;
			break;

		case WSAEAFNOSUPPORT: 
			return "WSAEAFNOSUPPORT " ;
			break;
		case WSAEMFILE: 
			return "WSAEMFILE " ;
			break;
		case WSAENOBUFS: 
			return "WSAENOBUFS " ;
			break;
		case WSAEPROTONOSUPPORT: 
			return "WSAEPROTONOSUPPORT " ;
			break;
		case WSAEPROTOTYPE: 
			return "WSAEPROTOTYPE " ;
			break;
		case WSAESOCKTNOSUPPORT: 
			return "WSAESOCKTNOSUPPORT " ;
			break;

		case WSAEFAULT: 
			return "WSAEFAULT " ;
			break;
		case WSAENOPROTOOPT: 
			return "WSAENOPROTOOPT " ;
			break;
		case WSAENOTSOCK: 
			return "WSAENOTSOCK " ;
			break;

		case WSAEADDRINUSE: 
			return "WSAEADDRINUSE " ;
			break;
		case WSAEADDRNOTAVAIL: 
			return "WSAEADDRNOTAVAIL " ;
			break;
		case WSAECONNREFUSED: 
			return "WSAECONNREFUSED - check the \"LISTENERS\" " ;
			break;

		case WSAEDESTADDRREQ: 
			return "WSAEDESTADDRREQ " ;
			break;
		case WSAEISCONN: 
			return "WSAEISCONN " ;
			break;
		case WSAENETUNREACH: 
			return "WSAENETUNREACH " ;
			break;
		case WSAETIMEDOUT: 
			return "WSAETIMEDOUT " ;
			break;
		case WSAEWOULDBLOCK: 
			return "WSAEWOULDBLOCK " ;
			break;
		case WSAEACCES: 
			return "WSAEACCES " ;
			break;
		case WSAENETRESET: 
			return "WSAENETRESET " ;
			break;
		case WSAENOTCONN: 
			return "WSAENOTCONN " ;
			break;
		case WSAEOPNOTSUPP: 
			return "WSAEOPNOTSUPP " ;
			break;
		case WSAESHUTDOWN: 
			return "WSAESHUTDOWN " ;
			break;
		case WSAEMSGSIZE: 
			return "WSAEMSGSIZE " ;
			break;
		case WSAECONNABORTED: 
			return "WSAECONNABORTED " ;
			break;
		case WSAECONNRESET: 
			return "WSAECONNRESET " ;
			break;
		default :
			return "UNKNOWN SOCKET ERROR";
			break;
	};



#else
    error=0;
//    wsecode = strdup("UNIX TESTING");
	strcpy(gbp->wsecode, "UNIX TESTING");
#endif

#ifdef PRINTDEBUG
    printline("> Done in get_sock_error_code     ");
#endif
    return(gbp->wsecode);
}

void printline(char *msg)
{

    FILE *fd;
#ifndef WIN32
    struct tm *today;
    time_t    ltime;
#endif
    char dbuffer[9];
    char tbuffer[9];

    memset(dbuffer,'\0',9);
    memset(tbuffer,'\0',9);

#ifdef WIN32
    _strdate( dbuffer );
    _strtime( tbuffer );
#else
    ltime=time('\0');
    today = localtime( &ltime );
    strftime( dbuffer, 9, "%x", today );
    strftime( tbuffer, 9, "%X", today );
#endif
#ifdef WIN32
    if ((fd = fopen("WEBAPI","a+"))!=NULL) {
        fprintf(fd,"%s %s %s\n",dbuffer,tbuffer,msg);
        fclose(fd);
    }
#else
    if ((fd = fopen("/tmp/XML_Debug.log","a+"))!=NULL) {
        fprintf(fd,"%s %s %s\n",dbuffer,tbuffer,msg);
        fclose(fd);
    }

#endif
    return;

}

#ifdef WIN32
int HTML_Error(char *parm1, char *parm2,char *parm3,char *parm4, 
			   request_rec *r, struct global_struct *gbp)
{
    /*
        Do nothing
    */
        return 0;
}
#endif
