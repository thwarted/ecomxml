/* tt0091.c Standing Order Review */
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


int             tt0091_CatSendStr(struct global_struct *, char *, tt0091_st_send *);
int             tt0091_ParceRecvStr(tt0091_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0091_lt_process(request_rec *, struct global_struct *, char *);


int tt0091_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0091_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0091_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0091_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0091 = (tt0091_st_send *)malloc(sizeof(tt0091_st_send));
        gbp->recvbuf0091 = (tt0091_st_recv *)malloc(sizeof(tt0091_st_recv));

        gbp->sendbufcat = malloc(tt0091_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0091_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0091_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0091_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0091,  '\0',  sizeof(tt0091_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0091->request_id, "XML");
    strcpy(gbp->sendbuf0091->record_id, "0091");

        strcpy(gbp->sendbuf0091->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0091->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0091->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0091->userid,gbp,stdout_buffer);
        get_tag_data("CUST_EDP", gbp->sendbuf0091->cust_edp,gbp,stdout_buffer);
        get_tag_data("UPDATE_MESSAGE", gbp->sendbuf0091->update_message,gbp,stdout_buffer);



    if((tt0091_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0091)) == tt0091_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0091);
                        free (gbp->recvbuf0091);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0091_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0091);
                        free (gbp->recvbuf0091);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0091_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0091);
                        free (gbp->recvbuf0091);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0091);
                        free (gbp->recvbuf0091);
                        return(-1);
                }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0091_ParceRecvStr(gbp->recvbuf0091, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0091);
                free (gbp->recvbuf0091);
        }
        else
        {
                XML_Error("tt0091_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0091);
                free (gbp->recvbuf0091);
                return(-1);
        }

    return 0;
}


int tt0091_CatSendStr(struct global_struct *gbp, char *sendbufcat, tt0091_st_send *sendbuf)
{
    gbp->j = sprintf(sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-9.9s%-80.80s",
            sendbuf->request_id,
            sendbuf->record_id,
            sendbuf->company,
            sendbuf->division,
            sendbuf->userid,
            sendbuf->ip_address,
                        sendbuf->filler,
                        sendbuf->cust_edp,
                        sendbuf->update_message
                        );
                        
    return(gbp->j);
}
    

int tt0091_ParceRecvStr(tt0091_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0092.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0092 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0092.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0092 %s\"tt0092\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

        memset(ptr_recvbuf->request_id,'\0', tt0091_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0091_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0091_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0091_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0091_REC_ID_LEN );
    gbp->count += tt0091_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0091_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0091_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0091_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0091_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0091_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0091_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0091_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0091_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0091_ERR_LEN;

        gbp->count += tt0091_SEND_FILLER_LEN;


    memset(ptr_recvbuf->hdr_cust_edp, '\0', tt0091_HDR_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->hdr_cust_edp, sz_recvbufcat + gbp->count, tt0091_HDR_CUST_EDP_LEN );
        ap_rprintf(r,"		<HDR_CUST_EDP>%s</HDR_CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_cust_edp));
    gbp->count += tt0091_HDR_CUST_EDP_LEN;

    memset(ptr_recvbuf->hdr_fname, '\0', tt0091_HDR_FNAME_LEN + 1);
    memcpy(ptr_recvbuf->hdr_fname, sz_recvbufcat + gbp->count, tt0091_HDR_FNAME_LEN );
        ap_rprintf(r,"		<HDR_FNAME>%s</HDR_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_fname));
    gbp->count += tt0091_HDR_FNAME_LEN;

    memset(ptr_recvbuf->hdr_lname, '\0', tt0091_HDR_LNAME_LEN + 1);
    memcpy(ptr_recvbuf->hdr_lname, sz_recvbufcat + gbp->count, tt0091_HDR_LNAME_LEN );
        ap_rprintf(r,"		<HDR_LNAME>%s</HDR_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_lname));
    gbp->count += tt0091_HDR_LNAME_LEN;

    memset(ptr_recvbuf->hdr_pm, '\0', tt0091_HDR_PM_LEN + 1);
    memcpy(ptr_recvbuf->hdr_pm, sz_recvbufcat + gbp->count, tt0091_HDR_PM_LEN );
        ap_rprintf(r,"		<HDR_PM>%s</HDR_PM>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_pm));
    gbp->count += tt0091_HDR_PM_LEN;

    memset(ptr_recvbuf->hdr_pm_type, '\0', tt0091_HDR_PM_TYPE_LEN + 1);
    memcpy(ptr_recvbuf->hdr_pm_type, sz_recvbufcat + gbp->count, tt0091_HDR_PM_TYPE_LEN );
        ap_rprintf(r,"		<HDR_PM_TYPE>%s</HDR_PM_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_pm_type));
    gbp->count += tt0091_HDR_PM_TYPE_LEN;

    memset(ptr_recvbuf->hdr_cc_no, '\0', tt0091_HDR_CC_NO_LEN + 1);
    memcpy(ptr_recvbuf->hdr_cc_no, sz_recvbufcat + gbp->count, tt0091_HDR_CC_NO_LEN );
        ap_rprintf(r,"		<HDR_CC_NO>%s</HDR_CC_NO>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_cc_no));
    gbp->count += tt0091_HDR_CC_NO_LEN;

    memset(ptr_recvbuf->hdr_cc_exp, '\0', tt0091_HDR_CC_EXP_LEN + 1);
    memcpy(ptr_recvbuf->hdr_cc_exp, sz_recvbufcat + gbp->count, tt0091_HDR_CC_EXP_LEN );
        ap_rprintf(r,"		<HDR_CC_EXP>%s</HDR_CC_EXP>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_cc_exp));
    gbp->count += tt0091_HDR_CC_EXP_LEN;

    memset(ptr_recvbuf->hdr_co, '\0', tt0091_HDR_CO_LEN + 1);
    memcpy(ptr_recvbuf->hdr_co, sz_recvbufcat + gbp->count, tt0091_HDR_CO_LEN );
        ap_rprintf(r,"		<HDR_CO>%s</HDR_CO>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_co));
    gbp->count += tt0091_HDR_CO_LEN;

    memset(ptr_recvbuf->hdr_div, '\0', tt0091_HDR_DIV_LEN + 1);
    memcpy(ptr_recvbuf->hdr_div, sz_recvbufcat + gbp->count, tt0091_HDR_DIV_LEN );
        ap_rprintf(r,"		<HDR_DIV>%s</HDR_DIV>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_div));
    gbp->count += tt0091_HDR_DIV_LEN;

    memset(ptr_recvbuf->count, '\0', tt0091_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->count, sz_recvbufcat + gbp->count, tt0091_COUNT_LEN );
        ap_rprintf(r,"		<COUNT>%s</COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->count));
    gbp->count += tt0091_COUNT_LEN;

	memset(ptr_recvbuf->hdr_full_cc_no, '\0', tt0091_HDR_FULL_CC_NO_LEN + 1);
    memcpy(ptr_recvbuf->hdr_full_cc_no, sz_recvbufcat + gbp->count, tt0091_HDR_FULL_CC_NO_LEN );
        ap_rprintf(r,"		<FULL_CC_NO>%s</FULL_CC_NO>\n", handle_special_chars(gbp,ptr_recvbuf->hdr_full_cc_no));
    gbp->count += tt0091_HDR_FULL_CC_NO_LEN;

	memset(ptr_recvbuf->hdr_filler_len, '\0', tt0091_HDR_FILLER_LEN + 1);
    memcpy(ptr_recvbuf->hdr_filler_len, sz_recvbufcat + gbp->count, tt0091_HDR_FILLER_LEN );
    gbp->count += tt0091_HDR_FILLER_LEN;

    // Build data from array

    for(gbp->i = 0 ; gbp->i < tt0091_STAND_ORD_DETAIL ; gbp->i++) 
    {

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_edp, '\0', tt0091_DTL_SHIPTO_EDP_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_edp, sz_recvbufcat + gbp->count, tt0091_DTL_SHIPTO_EDP_LEN);
        gbp->count += tt0091_DTL_SHIPTO_EDP_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_fname, '\0', tt0091_DTL_SHIPTO_FNAME_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_fname, sz_recvbufcat + gbp->count, tt0091_DTL_SHIPTO_FNAME_LEN);
        gbp->count += tt0091_DTL_SHIPTO_FNAME_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_lname, '\0', tt0091_DTL_SHIPTO_LNAME_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_lname, sz_recvbufcat + gbp->count, tt0091_DTL_SHIPTO_LNAME_LEN);
        gbp->count += tt0091_DTL_SHIPTO_LNAME_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_sm, '\0', tt0091_DTL_SHIPTO_SM_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_sm, sz_recvbufcat + gbp->count, tt0091_DTL_SHIPTO_SM_LEN);
        gbp->count += tt0091_DTL_SHIPTO_SM_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_sm_type, '\0', tt0091_DTL_SHIPTO_SM_TYPE_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_sm_type, sz_recvbufcat + gbp->count, tt0091_DTL_SHIPTO_SM_TYPE_LEN);
        gbp->count += tt0091_DTL_SHIPTO_SM_TYPE_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_next_date, '\0', tt0091_DTL_NEXT_DATE_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_next_date, sz_recvbufcat + gbp->count, tt0091_DTL_NEXT_DATE_LEN);
        gbp->count += tt0091_DTL_NEXT_DATE_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_temp_date, '\0', tt0091_DTL_TEMP_DATE_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_temp_date, sz_recvbufcat + gbp->count, tt0091_DTL_TEMP_DATE_LEN);
        gbp->count += tt0091_DTL_TEMP_DATE_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_stop_date, '\0', tt0091_DTL_STOP_DATE_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_stop_date, sz_recvbufcat + gbp->count, tt0091_DTL_STOP_DATE_LEN);
        gbp->count += tt0091_DTL_STOP_DATE_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_post_start, '\0', tt0091_DTL_POST_START_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_post_start, sz_recvbufcat + gbp->count, tt0091_DTL_POST_START_LEN);
        gbp->count += tt0091_DTL_POST_START_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_post_stop, '\0', tt0091_DTL_POST_STOP_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_post_stop, sz_recvbufcat + gbp->count, tt0091_DTL_POST_STOP_LEN);
        gbp->count += tt0091_DTL_POST_STOP_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_edp, '\0', tt0091_DTL_ITEM_EDP_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_edp, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_EDP_LEN);
        gbp->count += tt0091_DTL_ITEM_EDP_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_no, '\0', tt0091_DTL_ITEM_NO_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_no, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_NO_LEN);
        gbp->count += tt0091_DTL_ITEM_NO_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_desc, '\0', tt0091_DTL_ITEM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_desc, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_DESC_LEN);
        gbp->count += tt0091_DTL_ITEM_DESC_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_qty, '\0', tt0091_DTL_ITEM_QTY_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_qty, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_QTY_LEN);
        gbp->count += tt0091_DTL_ITEM_QTY_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_price, '\0', tt0091_DTL_ITEM_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_price, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_PRICE_LEN);
        gbp->count += tt0091_DTL_ITEM_PRICE_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_ships, '\0', tt0091_DTL_ITEM_SHIPS_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_ships, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_SHIPS_LEN);
        gbp->count += tt0091_DTL_ITEM_SHIPS_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_int_n, '\0', tt0091_DTL_ITEM_INT_N_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_int_n, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_INT_N_LEN);
        gbp->count += tt0091_DTL_ITEM_INT_N_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_int_x, '\0', tt0091_DTL_ITEM_INT_X_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_int_x, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_INT_X_LEN);
        gbp->count += tt0091_DTL_ITEM_INT_X_LEN;

        memset(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_status, '\0', tt0091_DTL_ITEM_STATUS_LEN + 1);
        memcpy(ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_status, sz_recvbufcat + gbp->count, tt0091_DTL_ITEM_STATUS_LEN);
        gbp->count += tt0091_DTL_ITEM_STATUS_LEN;


    }

        /* ... AND NOW WE OUTPUT THE ITEM DETAIL RECORDS */

    for(gbp->i = 0 ; gbp->i < tt0091_STAND_ORD_DETAIL ; gbp->i++) 
    {
                if (gbp->i < atoi(ptr_recvbuf->count) ){
                ap_rprintf(r,"		<STAND_ORD_DETAIL>\n");
                ap_rprintf(r,"			<DTL_SHIPTO_EDP>%s</DTL_SHIPTO_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_edp));
                ap_rprintf(r,"			<DTL_SHIPTO_FNAME>%s</DTL_SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_fname));
                ap_rprintf(r,"			<DTL_SHIPTO_LNAME>%s</DTL_SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_lname));
                ap_rprintf(r,"			<DTL_SHIPTO_SM>%s</DTL_SHIPTO_SM>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_sm));
                ap_rprintf(r,"			<DTL_SHIPTO_SM_TYPE>%s</DTL_SHIPTO_SM_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_shipto_sm_type));
                ap_rprintf(r,"			<DTL_NEXT_DATE>%s</DTL_NEXT_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_next_date));
                ap_rprintf(r,"			<DTL_TEMP_DATE>%s</DTL_TEMP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_temp_date));
                ap_rprintf(r,"			<DTL_STOP_DATE>%s</DTL_STOP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_stop_date));
                ap_rprintf(r,"			<DTL_POST_START>%s</DTL_POST_START>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_post_start));
                ap_rprintf(r,"			<DTL_POST_STOP>%s</DTL_POST_STOP>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_post_stop));
                ap_rprintf(r,"			<DTL_ITEM_EDP>%s</DTL_ITEM_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_edp));
                ap_rprintf(r,"			<DTL_ITEM_NO>%s</DTL_ITEM_NO>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_no));
                ap_rprintf(r,"			<DTL_ITEM_DESC>%s</DTL_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_desc));
                ap_rprintf(r,"			<DTL_ITEM_QTY>%s</DTL_ITEM_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_qty));
                ap_rprintf(r,"			<DTL_ITEM_PRICE>%s</DTL_ITEM_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_price));
                ap_rprintf(r,"			<DTL_ITEM_SHIPS>%s</DTL_ITEM_SHIPS>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_ships));
                ap_rprintf(r,"			<DTL_ITEM_INT_N>%s</DTL_ITEM_INT_N>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_int_n));
                ap_rprintf(r,"			<DTL_ITEM_INT_X>%s</DTL_ITEM_INT_X>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_int_x));
                ap_rprintf(r,"			<DTL_ITEM_STATUS>%s</DTL_ITEM_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_detail[gbp->i].dtl_item_status));
                ap_rprintf(r,"		</STAND_ORD_DETAIL>\n");
                }
    }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0092>\n", gbp->tt_betag);

    return(0);
}
