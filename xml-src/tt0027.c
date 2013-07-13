/* tt0027.c  Linkshare */
/* modified for phase 2 - JL */
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


int             tt0027_CatSendStr(struct global_struct *, char *, tt0027_st_send *);
int             tt0027_ParceRecvStr(tt0027_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0027_lt_process(request_rec *, struct global_struct *, char *);


int tt0027_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0027_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0027_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0027_lt_process(request_rec *r, struct global_struct *gbp, char * stdout_buffer)
{
        gbp->sendbuf0027 = (tt0027_st_send *)malloc(sizeof(tt0027_st_send));
        gbp->recvbuf0027 = (tt0027_st_recv *)malloc(sizeof(tt0027_st_recv));

        gbp->sendbufcat = malloc(tt0027_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0027_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0027_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0027_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0027,  '\0',  sizeof(tt0027_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0027->request_id, "XML");
    strcpy(gbp->sendbuf0027->record_id, "0027");

        strcpy(gbp->sendbuf0027->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0027->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0027->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0027->userid,gbp,stdout_buffer);
        get_tag_data("LINKSHARE_ID", gbp->sendbuf0027->linkshare_id,gbp,stdout_buffer);
        get_tag_data("LINKSHARE_TIME_IN", gbp->sendbuf0027->linkshare_time_in,gbp,stdout_buffer);

        if(tt0027_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0027) == tt0027_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0027);
                        free (gbp->recvbuf0027);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0027_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0027);
                        free (gbp->recvbuf0027);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0027_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0027);
                        free (gbp->recvbuf0027);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0027);
                        free (gbp->recvbuf0027);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0027_ParceRecvStr(gbp->recvbuf0027, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0027);
                free (gbp->recvbuf0027);
        }
        else
        {
                XML_Error("tt0027_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0027);
                free (gbp->recvbuf0027);
                return(-1);
        }

    return(0);
}


int tt0027_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0027_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-64.64s%-14.14s",
                        ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->linkshare_id,
            ptr_sendbuf->linkshare_time_in);
            
    return(gbp->j);
}


int tt0027_ParceRecvStr(tt0027_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0028.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0028 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0028.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0028 %s\"tt0028\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0027_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0027_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0027_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0027_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0027_REC_ID_LEN );
    gbp->count += tt0027_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0027_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0027_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0027_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0027_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0027_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0027_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0027_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0027_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0027_ERR_LEN;

        gbp->count += tt0027_SEND_FILLER_LEN;


        memset(ptr_recvbuf->linkshare_ok,'\0', tt0027_LINK_OK_LEN+1);
    memcpy(ptr_recvbuf->linkshare_ok, sz_recvbufcat + gbp->count, tt0027_LINK_OK_LEN);
        ap_rprintf(r,"		<LINKSHARE_OK>%s</LINKSHARE_OK>\n", handle_special_chars(gbp,ptr_recvbuf->linkshare_ok));
    gbp->count += tt0027_LINK_OK_LEN;


    ap_rprintf(r,"    	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0028>\n", gbp->tt_betag);

    return(0);
}

