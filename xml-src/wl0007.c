/* wl0007.c   */
/*modified for phase 2 - JL */
#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <malloc.h>


#include "sgsock.h"
#include "funcs.h"
#include "externs.h"
#include "http_protocol.h"


int             wl0007_CatSendStr(struct global_struct *, char *, wl0007_st_send *);
int             wl0007_ParceRecvStr(wl0007_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             wl0007_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int wl0007_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wl0007_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wl0007_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wl0007_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wlsendbuf0007 = (wl0007_st_send *)malloc(sizeof(wl0007_st_send));
        gbp->wlrecvbuf0007 = (wl0007_st_recv *)malloc(sizeof(wl0007_st_recv));

        gbp->sendbufcat = malloc(wl0007_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(wl0007_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', wl0007_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wl0007_LAN_RECV_BUF_LEN + 1);

    memset(gbp->wlsendbuf0007,  '\0',  sizeof(wl0007_st_send));

    sprintf(gbp->confirmbuf, "     ");

    get_tag_data("TRANSACTION_ID", gbp->wlsendbuf0007->record_id,gbp,stdout_buffer);
    get_tag_data("COMPANY", gbp->wlsendbuf0007->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->wlsendbuf0007->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->wlsendbuf0007->userid,gbp,stdout_buffer);
    get_tag_data("CUST_EDP", gbp->wlsendbuf0007->cust_edp,gbp,stdout_buffer);
    get_tag_data("PASSWORD", gbp->wlsendbuf0007->password,gbp,stdout_buffer);
    get_tag_data("OVRD_FLAG", gbp->wlsendbuf0007->ovrd_flag,gbp,stdout_buffer);


        if(wl0007_CatSendStr(gbp, gbp->sendbufcat, gbp->wlsendbuf0007) == wl0007_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->wlport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0007);
                        free (gbp->wlrecvbuf0007);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, wl0007_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0007);
                        free (gbp->wlrecvbuf0007);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wl0007_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0007);
                        free (gbp->wlrecvbuf0007);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0007);
                        free (gbp->wlrecvbuf0007);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                wl0007_ParceRecvStr(gbp->wlrecvbuf0007, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0007);
                free (gbp->wlrecvbuf0007);

        }
        else
        {
                XML_Error("wl0007_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0007);
                free (gbp->wlrecvbuf0007);
                return(-1);
        }

    return(0);
}


int wl0007_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, wl0007_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-2.2s%-2.2s%-16.16s%-31.31s%09d%-16.16s%-1.1s%-5.5s",

            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->filler1,
            atoi(ptr_sendbuf->cust_edp),
            ptr_sendbuf->password,
            ptr_sendbuf->ovrd_flag,
            ptr_sendbuf->filler2);

    return(gbp->j);
}


int wl0007_ParceRecvStr(wl0007_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wl0007.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, wl_tag);
*/
        ap_rprintf(r,"%s0007 %s\"wl0007\">\n", gbp->wl_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->record_id,'\0', wl0007_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, wl0007_REC_ID_LEN );
    gbp->count += wl0007_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', wl0007_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, wl0007_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += wl0007_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', wl0007_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, wl0007_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += wl0007_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', wl0007_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, wl0007_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += wl0007_ERR_LEN;

    gbp->count += wl0007_RECV_FILLER_LEN;

 
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0007>\n", gbp->wl_betag);


    return(0);
}

