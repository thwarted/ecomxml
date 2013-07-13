/* tt0075.c Get RFM Data*/
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


int             tt0075_CatSendStr(struct global_struct *, char *, tt0075_st_send *);
int             tt0075_ParceRecvStr(tt0075_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0075_lt_process(request_rec *, struct global_struct *, char *);


int tt0075_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0075_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0075_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0075_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0075 = (tt0075_st_send *)malloc(sizeof(tt0075_st_send));
        gbp->recvbuf0075 = (tt0075_st_recv *)malloc(sizeof(tt0075_st_recv));

        gbp->sendbufcat = malloc(tt0075_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0075_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0075_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0075_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0075,  '\0',  sizeof(tt0075_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0075->request_id, "XML");
    strcpy(gbp->sendbuf0075->record_id, "0075");

        strcpy(gbp->sendbuf0075->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0075->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0075->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0075->userid,gbp,stdout_buffer);
        get_tag_data("CUST_EDP", gbp->sendbuf0075->cust_edp,gbp,stdout_buffer);
        

        if(tt0075_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0075) == tt0075_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0075);
                        free (gbp->recvbuf0075);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0075_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0075);
                        free (gbp->recvbuf0075);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0075_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0075);
                        free (gbp->recvbuf0075);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0075);
                        free (gbp->recvbuf0075);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0075_ParceRecvStr(gbp->recvbuf0075, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0075);
                free (gbp->recvbuf0075);
        }
        else
        {
                XML_Error("tt0075_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0075);
                free (gbp->recvbuf0075);
                return(-1);
        }

    return(0);
}



int tt0075_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0075_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%09d",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        atoi(ptr_sendbuf->cust_edp));
            
    return(gbp->j);
}


int tt0075_ParceRecvStr(tt0075_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"http://localhost/xml-dtd/tt0076.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0076 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0076.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0076 %s\"tt0076\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0075_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0075_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0075_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0075_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0075_REC_ID_LEN );
    gbp->count += tt0075_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0075_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0075_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0075_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0075_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0075_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0075_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0075_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0075_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0075_ERR_LEN;

        gbp->count += tt0075_SEND_FILLER_LEN;


        memset(ptr_recvbuf->cust_edp,'\0', tt0075_EDP_LEN+1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0075_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0075_EDP_LEN;

        memset(ptr_recvbuf->cust_type,'\0', tt0075_CUST_TYPE_LEN+1);
    memcpy(ptr_recvbuf->cust_type, sz_recvbufcat + gbp->count, tt0075_CUST_TYPE_LEN);
        ap_rprintf(r,"		<CUST_TYPE>%s</CUST_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_type));
    gbp->count += tt0075_CUST_TYPE_LEN;

        memset(ptr_recvbuf->cust_status,'\0', tt0075_CUST_STATUS_LEN+1);
    memcpy(ptr_recvbuf->cust_status, sz_recvbufcat + gbp->count, tt0075_CUST_STATUS_LEN);
        ap_rprintf(r,"		<CUST_STATUS>%s</CUST_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->cust_status));
    gbp->count += tt0075_CUST_STATUS_LEN;

        memset(ptr_recvbuf->recency_score,'\0', tt0075_RECENCY_LEN+1);
    memcpy(ptr_recvbuf->recency_score, sz_recvbufcat + gbp->count, tt0075_RECENCY_LEN);
        ap_rprintf(r,"		<RECENCY_SCORE>%s</RECENCY_SCORE>\n", handle_special_chars(gbp,ptr_recvbuf->recency_score));
    gbp->count += tt0075_RECENCY_LEN;

        memset(ptr_recvbuf->frequency_score,'\0', tt0075_FREQ_LEN+1);
    memcpy(ptr_recvbuf->frequency_score, sz_recvbufcat + gbp->count, tt0075_FREQ_LEN);
        ap_rprintf(r,"		<FREQUENCY_SCORE>%s</FREQUENCY_SCORE>\n", handle_special_chars(gbp,ptr_recvbuf->frequency_score));
    gbp->count += tt0075_FREQ_LEN;

        memset(ptr_recvbuf->monetary_score,'\0', tt0075_MONETARY_LEN+1);
    memcpy(ptr_recvbuf->monetary_score, sz_recvbufcat + gbp->count, tt0075_MONETARY_LEN);
        ap_rprintf(r,"		<MONETARY_SCORE>%s</MONETARY_SCORE>\n", handle_special_chars(gbp,ptr_recvbuf->monetary_score));
    gbp->count += tt0075_MONETARY_LEN;

        memset(ptr_recvbuf->cust_tot_doll,'\0', tt0075_TOT_DOLL_LEN+1);
    memcpy(ptr_recvbuf->cust_tot_doll, sz_recvbufcat + gbp->count, tt0075_TOT_DOLL_LEN);
        ap_rprintf(r,"		<CUST_TOTAL_DOLLARS>%s</CUST_TOTAL_DOLLARS>\n", handle_special_chars(gbp,ptr_recvbuf->cust_tot_doll));
    gbp->count += tt0075_TOT_DOLL_LEN;

        memset(ptr_recvbuf->cust_tot_ord,'\0', tt0075_TOT_ORD_LEN+1);
    memcpy(ptr_recvbuf->cust_tot_ord, sz_recvbufcat + gbp->count, tt0075_TOT_ORD_LEN);
        ap_rprintf(r,"		<CUST_TOTAL_ORD>%s</CUST_TOTAL_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->cust_tot_ord));
    gbp->count += tt0075_TOT_ORD_LEN;

        memset(ptr_recvbuf->cust_curr_date,'\0', tt0075_CURR_DATE_LEN+1);
    memcpy(ptr_recvbuf->cust_curr_date, sz_recvbufcat + gbp->count, tt0075_CURR_DATE_LEN);
        ap_rprintf(r,"		<CUST_CURR_DATE>%s</CUST_CURR_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_curr_date));
    gbp->count += tt0075_CURR_DATE_LEN;

        memset(ptr_recvbuf->cust_orig_date,'\0', tt0075_ORIG_DATE_LEN+1);
    memcpy(ptr_recvbuf->cust_orig_date, sz_recvbufcat + gbp->count, tt0075_ORIG_DATE_LEN);
        ap_rprintf(r,"		<CUST_ORIG_DATE>%s</CUST_ORIG_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_orig_date));
    gbp->count += tt0075_ORIG_DATE_LEN;

        memset(ptr_recvbuf->cust_orig_source,'\0', tt0075_ORIG_SRC_LEN+1);
    memcpy(ptr_recvbuf->cust_orig_source, sz_recvbufcat + gbp->count, tt0075_ORIG_SRC_LEN);
        ap_rprintf(r,"		<CUST_ORIG_SOURCE>%s</CUST_ORIG_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_orig_source));
    gbp->count += tt0075_ORIG_SRC_LEN;

        memset(ptr_recvbuf->cust_last_source,'\0', tt0075_LAST_SRC_LEN+1);
    memcpy(ptr_recvbuf->cust_last_source, sz_recvbufcat + gbp->count, tt0075_LAST_SRC_LEN);
        ap_rprintf(r,"		<CUST_LAST_SOURCE>%s</CUST_LAST_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_last_source));
    gbp->count += tt0075_LAST_SRC_LEN;

        memset(ptr_recvbuf->cust_purch_cat1,'\0', tt0075_PURCH_CAT_LEN+1);
    memcpy(ptr_recvbuf->cust_purch_cat1, sz_recvbufcat + gbp->count, tt0075_PURCH_CAT_LEN);
        ap_rprintf(r,"		<CUST_PURCH_CAT1>%s</CUST_PURCH_CAT1>\n", handle_special_chars(gbp,ptr_recvbuf->cust_purch_cat1));
    gbp->count += tt0075_PURCH_CAT_LEN;

        memset(ptr_recvbuf->cust_purch_cat2,'\0', tt0075_PURCH_CAT_LEN+1);
    memcpy(ptr_recvbuf->cust_purch_cat2, sz_recvbufcat + gbp->count, tt0075_PURCH_CAT_LEN);
        ap_rprintf(r,"		<CUST_PURCH_CAT2>%s</CUST_PURCH_CAT2>\n", handle_special_chars(gbp,ptr_recvbuf->cust_purch_cat2));
    gbp->count += tt0075_PURCH_CAT_LEN;


    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0076>\n", gbp->tt_betag);

    return(0);
}

