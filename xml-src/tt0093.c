/* tt0093.c Standing Order Maintenance */
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


int             tt0093_CatSendStr(struct global_struct *, char *, tt0093_st_send *);
int             tt0093_ParceRecvStr(tt0093_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0093_lt_process(request_rec *, struct global_struct *, char *);


int tt0093_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0093_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0093_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0093_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0093 = (tt0093_st_send *)malloc(sizeof(tt0093_st_send));
        gbp->recvbuf0093 = (tt0093_st_recv *)malloc(sizeof(tt0093_st_recv));

        gbp->sendbufcat = malloc(tt0093_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0093_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0093_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0093_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0093,  '\0',  sizeof(tt0093_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0093->request_id, "XML");
    strcpy(gbp->sendbuf0093->record_id, "0093");

        strcpy(gbp->sendbuf0093->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0093->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0093->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0093->userid,gbp,stdout_buffer);
        get_tag_data("UPDATE_FLAG", gbp->sendbuf0093->update_flag,gbp,stdout_buffer);
        get_tag_data("HDR_CUST_EDP", gbp->sendbuf0093->hdr_cust_edp,gbp,stdout_buffer);
        get_tag_data("HDR_PM", gbp->sendbuf0093->hdr_pm,gbp,stdout_buffer);
        get_tag_data("HDR_PM_TYPE", gbp->sendbuf0093->hdr_pm_type,gbp,stdout_buffer);
        get_tag_data("HDR_CC_NO", gbp->sendbuf0093->hdr_cc_no,gbp,stdout_buffer);
        get_tag_data("HDR_CC_EXP", gbp->sendbuf0093->hdr_cc_exp,gbp,stdout_buffer);
        get_tag_data("HDR_CO", gbp->sendbuf0093->hdr_co,gbp,stdout_buffer);
        get_tag_data("HDR_DIV", gbp->sendbuf0093->hdr_div,gbp,stdout_buffer);
		get_tag_data("HDR_CC_SEC_CODE",gbp->sendbuf0093->hdr_cc_sec_code,gbp,stdout_buffer);

        for(gbp->i = 0; gbp->i < tt0093_STAND_ORD_DETAIL; gbp->i++)
        {

                set_tag_name(gbp->tagname, "DTL_DELETE_ITEM_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_delete_item,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_SHIPTO_EDP_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_shipto_edp,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_SHIPTO_SM_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_shipto_sm,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_SHIPTO_SM_TYPE_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_shipto_sm_type,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_NEXT_DATE_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_next_date,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_TEMP_DATE_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_temp_date,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_STOP_DATE_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_stop_date,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_POST_START_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_post_start,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_POST_STOP_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_post_stop,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_ITEM_EDP_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_item_edp,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_ITEM_QTY_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_item_no,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_ITEM_SHIPS_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_item_ships,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_ITEM_INT_N_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_item_int_n,gbp,stdout_buffer);

                set_tag_name(gbp->tagname, "DTL_ITEM_INT_X_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0093->stand_ord_detail[gbp->i].dtl_item_int_x,gbp,stdout_buffer);

        }


    if((tt0093_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0093)) == tt0093_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0093);
                        free (gbp->recvbuf0093);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0093_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0093);
                        free (gbp->recvbuf0093);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0093_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0093);
                        free (gbp->recvbuf0093);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0093);
                        free (gbp->recvbuf0093);
                        return(-1);
                }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0093_ParceRecvStr(gbp->recvbuf0093, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0093);
                free (gbp->recvbuf0093);
        }
        else
        {
                XML_Error("tt0093_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0093);
                free (gbp->recvbuf0093);
                return(-1);
        }

    return 0;
}


int tt0093_CatSendStr(struct global_struct *gbp, char *sendbufcat, tt0093_st_send *sendbuf)
{
    gbp->j = sprintf(sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-9.9s%-20.20s%-2.2s%-20.20s%-4.4s%-2.2s%-2.2s%-4.4s%-96.96s",
            sendbuf->request_id,
            sendbuf->record_id,
            sendbuf->company,
            sendbuf->division,
            sendbuf->userid,
            sendbuf->ip_address,
                        sendbuf->filler,
                        sendbuf->update_flag,
                        sendbuf->hdr_cust_edp,
                        sendbuf->hdr_pm,
                        sendbuf->hdr_pm_type,
                        sendbuf->hdr_cc_no,
                        sendbuf->hdr_cc_exp,
                        sendbuf->hdr_co,
                        sendbuf->hdr_div,
						sendbuf->hdr_cc_sec_code,
						sendbuf->hdr_filler
                        );

        for(gbp->i=0; gbp->i < tt0093_STAND_ORD_DETAIL; gbp->i++)
        {
                gbp->j += sprintf((sendbufcat+gbp->j),
                        "%-1.1s%-9.9s%-80.80s%-2.2s%-8.8s%-8.8s%-8.8s%-4.4s%-4.4s%-9.9s%-9.9s%-4.4s%-3.3s%-25.25s",
                        sendbuf->stand_ord_detail[gbp->i].dtl_delete_item,
                        sendbuf->stand_ord_detail[gbp->i].dtl_shipto_edp,
                        sendbuf->stand_ord_detail[gbp->i].dtl_shipto_sm,
                        sendbuf->stand_ord_detail[gbp->i].dtl_shipto_sm_type,
                        sendbuf->stand_ord_detail[gbp->i].dtl_next_date,
                        sendbuf->stand_ord_detail[gbp->i].dtl_temp_date,
                        sendbuf->stand_ord_detail[gbp->i].dtl_stop_date,
                        sendbuf->stand_ord_detail[gbp->i].dtl_post_start,
                        sendbuf->stand_ord_detail[gbp->i].dtl_post_stop,
                        sendbuf->stand_ord_detail[gbp->i].dtl_item_edp,
                        sendbuf->stand_ord_detail[gbp->i].dtl_item_no,
                        sendbuf->stand_ord_detail[gbp->i].dtl_item_ships,
                        sendbuf->stand_ord_detail[gbp->i].dtl_item_int_n,
                        sendbuf->stand_ord_detail[gbp->i].dtl_item_int_x);


        }


    return(gbp->j);
}
    

int tt0093_ParceRecvStr(tt0093_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0094.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0094 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0094.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0094 %s\"tt0094\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

        memset(ptr_recvbuf->request_id,'\0', tt0093_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0093_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0093_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0093_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0093_REC_ID_LEN );
    gbp->count += tt0093_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0093_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0093_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0093_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0093_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0093_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0093_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0093_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0093_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0093_ERR_LEN;

        gbp->count += tt0093_SEND_FILLER_LEN;


    memset(ptr_recvbuf->cust_edp, '\0', tt0093_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0093_CUST_EDP_LEN );
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0093_CUST_EDP_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0094>\n", gbp->tt_betag);

    return(0);
}




