/* tt0079.c Coupon Review */
/* modified for phase 2 - JL */
#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>


#include "sgsock.h"
#include "funcs.h"
#include "externs.h"
#include "http_protocol.h"


int             tt0079_CatSendStr(struct global_struct *, char *, tt0079_st_send *);
int             tt0079_ParceRecvStr(tt0079_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0079_lt_process(request_rec *, struct global_struct *, char *);


int tt0079_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0079_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0079_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0079_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0079 = (tt0079_st_send *)malloc(sizeof(tt0079_st_send));
        gbp->recvbuf0079 = (tt0079_st_recv *)malloc(sizeof(tt0079_st_recv));

        gbp->sendbufcat = malloc(tt0079_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0079_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0079_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0079_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0079,  '\0',  sizeof(tt0079_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0079->request_id, "XML");
    strcpy(gbp->sendbuf0079->record_id, "0079");

        strcpy(gbp->sendbuf0079->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0079->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0079->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0079->userid,gbp,stdout_buffer);
        get_tag_data("COUP_RVW_FLAG", gbp->sendbuf0079->coup_rvw_flag,gbp,stdout_buffer);

    if((tt0079_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0079)) == tt0079_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0079);
                        free (gbp->recvbuf0079);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0079_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0079);
                        free (gbp->recvbuf0079);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0079_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0079);
                        free (gbp->recvbuf0079);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0079);
                        free (gbp->recvbuf0079);
                        return(-1);
                }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0079_ParceRecvStr(gbp->recvbuf0079, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0079);
                free (gbp->recvbuf0079);
        }
        else
        {
                XML_Error("tt0079_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0079);
                free (gbp->recvbuf0079);
                return(-1);
        }

    return 0;
}


int tt0079_CatSendStr(struct global_struct *gbp, char *sendbufcat, tt0079_st_send *sendbuf)
{
    gbp->j = sprintf(sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-2.2s",

            sendbuf->request_id,
            sendbuf->record_id,
            sendbuf->company,
            sendbuf->division,
            sendbuf->userid,
            sendbuf->ip_address,
                        sendbuf->filler,
                        sendbuf->coup_rvw_flag
                        );
                        
    return(gbp->j);
}
    

int tt0079_ParceRecvStr(tt0079_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0080.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0080 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0080.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0080 %s\"tt0080\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

        memset(ptr_recvbuf->request_id,'\0', tt0079_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0079_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0079_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0079_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0079_REC_ID_LEN );
    gbp->count += tt0079_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0079_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0079_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0079_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0079_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0079_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0079_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0079_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0079_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0079_ERR_LEN;

        gbp->count += tt0079_SEND_FILLER_LEN;


    memset(ptr_recvbuf->coup_rvw_flag, '\0', tt0079_COUP_RVW_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->coup_rvw_flag, sz_recvbufcat + gbp->count, tt0079_COUP_RVW_FLAG_LEN );
        ap_rprintf(r,"		<COUP_RVW_FLAG>%s</COUP_RVW_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->coup_rvw_flag));
    gbp->count += tt0079_COUP_RVW_FLAG_LEN;


    // Build data from array

    for(gbp->i = 0 ; gbp->i < tt0079_COUPON_DATA_COUNT ; gbp->i++) 
    {
        memset(ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_num, '\0', tt0079_COUP_RVW_CO_NUM_LEN + 1);
        memcpy(ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_num, sz_recvbufcat + gbp->count, tt0079_COUP_RVW_CO_NUM_LEN);
        gbp->count += tt0079_COUP_RVW_CO_NUM_LEN;
 
                memset(ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_amount, '\0', tt0079_COUP_RVW_CO_AMOUNT_LEN + 1);
        memcpy(ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_amount, sz_recvbufcat + gbp->count, tt0079_COUP_RVW_CO_AMOUNT_LEN);
        gbp->count += tt0079_COUP_RVW_CO_AMOUNT_LEN;

        memset(ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_type, '\0', tt0079_COUP_RVW_CO_TYPE_LEN + 1);
        memcpy(ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_type, sz_recvbufcat + gbp->count, tt0079_COUP_RVW_CO_TYPE_LEN);
        gbp->count += tt0079_COUP_RVW_CO_TYPE_LEN;
    }

        /* ... AND NOW WE OUTPUT THE ITEM DETAIL RECORDS */

    for(gbp->i = 0 ; gbp->i < tt0079_COUPON_DATA_COUNT ; gbp->i++) 
    {
                ap_rprintf(r,"		<COUPON_REVIEW_DETAIL>\n");
                ap_rprintf(r,"			<COUP_RVW_CO_NUM>%s</COUP_RVW_CO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_num));
                ap_rprintf(r,"			<COUP_RVW_CO_AMOUNT>%s</COUP_RVW_CO_AMOUNT>\n", handle_special_chars(gbp,ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_amount));
                ap_rprintf(r,"			<COUP_RVW_CO_TYPE>%s</COUP_RVW_CO_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->coupon_data[gbp->i].coup_rvw_co_type));
                ap_rprintf(r,"		</COUPON_REVIEW_DETAIL>\n");
    }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0080>\n", gbp->tt_betag);

    return(0);
}




