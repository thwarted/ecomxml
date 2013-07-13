/* tt0137.c  Shopping Request */
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


int             tt0137_CatSendStr(struct global_struct *, char *, tt0137_st_send *);
int             tt0137_ParceRecvStr(tt0137_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0137_lt_process(request_rec *, struct global_struct *, char *);

int tt0137_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}


int tt0137_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0137_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0137_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0137 = (tt0137_st_send *)malloc(sizeof(tt0137_st_send));
        gbp->recvbuf0137 = (tt0137_st_recv *)malloc(sizeof(tt0137_st_recv));

        gbp->sendbufcat = malloc(tt0137_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0137_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0137_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0137_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0137,  '\0',  sizeof(tt0137_st_send));

    strcpy(gbp->sendbuf0137->request_id, "XML");
    strcpy(gbp->sendbuf0137->record_id, "0137");

        get_tag_data("COMPANY", gbp->sendbuf0137->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0137->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0137->userid,gbp,stdout_buffer);

        strcpy(gbp->sendbuf0137->ip_address, r->connection->remote_ip);

        get_tag_data("COPY_CUSTOMIZE", gbp->sendbuf0137->custom_flag,gbp,stdout_buffer);
        get_tag_data("COPY_SHIP_TO", gbp->sendbuf0137->shipto_flag,gbp,stdout_buffer);
        get_tag_data("COPY_GIFT_WRAP", gbp->sendbuf0137->giftwrap_flag,gbp,stdout_buffer);  
        get_tag_data("COPY_SOURCE", gbp->sendbuf0137->source_flag,gbp,stdout_buffer);
        get_tag_data("COPY_COMMENTS", gbp->sendbuf0137->comment_flag,gbp,stdout_buffer);

        if(tt0137_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0137) == tt0137_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0137);
                        free (gbp->recvbuf0137);
                        return(-1);
                }
//ap_rprintf(r,"HOST %s PORT %s SENDBUF %s",gbp->hphost, gbp->webport,gbp->sendbufcat); 

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0137_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0137);
                        free (gbp->recvbuf0137);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0137_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0137);
                        free (gbp->recvbuf0137);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0137);
                        free (gbp->recvbuf0137);
                        return(-1);
                }

/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);
//ap_rprintf(r,"RECVBUF %s",gbp->recvbufcat); 

                tt0137_ParceRecvStr(gbp->recvbuf0137, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0137);
                free (gbp->recvbuf0137);

        }
        else
        {
                XML_Error("tt0137_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0137);
                free (gbp->recvbuf0137);
                return(-1);
        }

    return(0);
}


int tt0137_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0137_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
        "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s",
        ptr_sendbuf->request_id,
        ptr_sendbuf->record_id,
        ptr_sendbuf->company,
        ptr_sendbuf->division,
        ptr_sendbuf->userid,
        ptr_sendbuf->ip_address,
        ptr_sendbuf->filler,
        ptr_sendbuf->custom_flag,
        ptr_sendbuf->shipto_flag,
        ptr_sendbuf->giftwrap_flag,
        ptr_sendbuf->source_flag,
        ptr_sendbuf->comment_flag);


    return(gbp->j);
}


int tt0137_ParceRecvStr(tt0137_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    /* Place the individual variables into target fields */
//ap_rprintf(r,"RECVBUF %s",sz_recvbufcat);
        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);
#ifdef XSL
        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0138.xsl\"?>\n");
#endif
/*              ap_rprintf(r,"<!DOCTYPE %s0004 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);
        ap_rprintf(r,"                  \"%s%s0004.dtd\">\n\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0138 %s\"tt0138\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n\n", gbp->mtag);

    memset(ptr_recvbuf->request_id,'\0', tt0137_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0137_REQ_ID_LEN);
    gbp->count += tt0137_REQ_ID_LEN;//ap_rprintf(r,"reqid=%s",ptr_recvbuf->request_id);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n\n", handle_special_chars(gbp,ptr_recvbuf->request_id));

    memset(ptr_recvbuf->record_id,'\0', tt0137_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0137_REC_ID_LEN );
    gbp->count += tt0137_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0137_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0137_USER_ID_LEN );
    gbp->count += tt0137_USER_ID_LEN;
    ap_rprintf(r,"		<UID>%s</UID>\n\n", handle_special_chars(gbp,ptr_recvbuf->userid));

    memset(ptr_recvbuf->success_flag,'\0', tt0137_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, tt0137_SUCCESS_FLAG_LEN);
    ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += tt0137_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0137_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0137_ERR_LEN);
    ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0137_ERR_LEN;

    gbp->count += tt0137_SEND_FILLER_LEN;

    memset(ptr_recvbuf->dup_cartid,'\0', tt0137_DUP_CART_ID_LEN+1);
    memcpy(ptr_recvbuf->dup_cartid, sz_recvbufcat + gbp->count, tt0137_DUP_CART_ID_LEN);
    gbp->count += tt0137_DUP_CART_ID_LEN;
    ap_rprintf(r,"		<DUP_CART_ID>%s</DUP_CART_ID>\n\n", handle_special_chars(gbp,ptr_recvbuf->dup_cartid));


    ap_rprintf(r,"	%s>\n\n", gbp->metag);
    ap_rprintf(r,"%s\n", pt_message);
    ap_rprintf(r,"	%s>\n", gbp->rstag);

    reparse_customer_data(r,gbp);

    ap_rprintf(r,"	%s>\n\n", gbp->rsetag);

    ap_rprintf(r,"%s0138>\n\n", gbp->tt_betag);

    return(0);
}


