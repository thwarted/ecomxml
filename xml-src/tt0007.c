/* tt0007.c Order Review */
/*modified for phase 2 - JL */
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



int             tt0007_CatSendStr(struct global_struct *,char *, tt0007_st_send *);
int             tt0007_ParceRecvStr(tt0007_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0007_lt_process(request_rec *, struct global_struct *, char *);



int tt0007_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */
    

        return(0);
}

int tt0007_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0007_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0007_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0007 = (tt0007_st_send *)malloc(sizeof(tt0007_st_send));
        gbp->recvbuf0007 = (tt0007_st_recv *)malloc(sizeof(tt0007_st_recv));

        gbp->sendbufcat = malloc(tt0007_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0007_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0007_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0007_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0007,  '\0',  sizeof(tt0007_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0007->request_id, "XML");
    strcpy(gbp->sendbuf0007->record_id, "0007");

        strcpy(gbp->sendbuf0007->ip_address, r->connection->remote_ip);
    // Fill the buffers

        get_tag_data("COMPANY", gbp->sendbuf0007->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0007->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0007->userid,gbp,stdout_buffer);
        get_tag_data("ORD_NUM", gbp->sendbuf0007->ord_num,gbp,stdout_buffer);
        get_tag_data("PAGE_NO", gbp->sendbuf0007->page_no,gbp,stdout_buffer);
        get_tag_data("ENCRYPT_CUST_EDP", gbp->sendbuf0007->encrypt_cust_edp,gbp,stdout_buffer);
        get_tag_data("PURCH_ORD_NO",gbp->sendbuf0007->po_number,gbp,stdout_buffer);
        get_tag_data("SEARCH_TYPE",gbp->sendbuf0007->search_type,gbp,stdout_buffer);

        if((tt0007_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0007)) == tt0007_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0007);
                        free (gbp->recvbuf0007);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0007_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0007);
                        free (gbp->recvbuf0007);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0007_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0007);
                        free (gbp->recvbuf0007);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0007);
                        free (gbp->recvbuf0007);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0007_ParceRecvStr(gbp->recvbuf0007, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0007);
                free (gbp->recvbuf0007);
        }
    else
    {
                XML_Error("tt0007_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0007);
                free (gbp->recvbuf0007);
                return(-1);
    }

    return(0);
}

int tt0007_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0007_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-8.8s%-3.3s%-17.17s%-20.20s%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->ord_num,
            ptr_sendbuf->page_no,
            ptr_sendbuf->encrypt_cust_edp,
                        ptr_sendbuf->po_number,
                        ptr_sendbuf->search_type);

    return(gbp->j);
}


int tt0007_ParceRecvStr(tt0007_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0008.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0008 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0008.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0008 %s\"tt0008\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"  %s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0007_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0007_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0007_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0007_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0007_REC_ID_LEN);
    gbp->count += tt0007_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0007_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0007_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0007_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0007_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0007_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0007_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0007_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0007_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0007_ERR_LEN;

        gbp->count += tt0007_SEND_FILLER_LEN;


    memset(ptr_recvbuf->ord_date, '\0', tt0007_ORD_DATE_LEN + 1);
    memcpy(ptr_recvbuf->ord_date, sz_recvbufcat + gbp->count, tt0007_ORD_DATE_LEN);
        ap_rprintf(r,"		<DATE_ORD_WAS_PLACED>%s</DATE_ORD_WAS_PLACED>\n", handle_special_chars(gbp,ptr_recvbuf->ord_date));
    gbp->count += tt0007_ORD_DATE_LEN;

    memset(ptr_recvbuf->ord_tot_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_tot_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<ORD_TOTAL_AMT>%s</ORD_TOTAL_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->ord_tot_dol));
    gbp->count += tt0007_DOL_LEN;

    memset(ptr_recvbuf->ord_prod_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_prod_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<TOTAL_PROD_AMT>%s</TOTAL_PROD_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->ord_prod_dol));
    gbp->count += tt0007_DOL_LEN;

    memset(ptr_recvbuf->ord_post_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_post_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<POSTAGE_HANDLING>%s</POSTAGE_HANDLING>\n", handle_special_chars(gbp,ptr_recvbuf->ord_post_dol));
    gbp->count += tt0007_DOL_LEN;

    memset(ptr_recvbuf->ord_tax_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_tax_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<TAX>%s</TAX>\n", handle_special_chars(gbp,ptr_recvbuf->ord_tax_dol));
    gbp->count += tt0007_DOL_LEN;

    memset(ptr_recvbuf->ord_disc_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_disc_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<DISCOUNT>%s</DISCOUNT>\n", handle_special_chars(gbp,ptr_recvbuf->ord_disc_dol));
    gbp->count += tt0007_DOL_LEN;

        memset(ptr_recvbuf->ord_credit_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_credit_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<CREDIT>%s</CREDIT>\n", handle_special_chars(gbp,ptr_recvbuf->ord_credit_dol));
    gbp->count += tt0007_DOL_LEN;

        memset(ptr_recvbuf->ord_remit_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_remit_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<REMIT>%s</REMIT>\n", handle_special_chars(gbp,ptr_recvbuf->ord_remit_dol));
    gbp->count += tt0007_DOL_LEN;

        memset(ptr_recvbuf->ord_grand_tot_dol, '\0', tt0007_DOL_LEN + 1);
    memcpy(ptr_recvbuf->ord_grand_tot_dol, sz_recvbufcat + gbp->count, tt0007_DOL_LEN);
        ap_rprintf(r,"		<GRAND_TOTAL>%s</GRAND_TOTAL>\n", handle_special_chars(gbp,ptr_recvbuf->ord_grand_tot_dol));
    gbp->count += tt0007_DOL_LEN;

    memset(ptr_recvbuf->pay_method, '\0', tt0007_PAY_METH_LEN + 1);
    memcpy(ptr_recvbuf->pay_method, sz_recvbufcat + gbp->count, tt0007_PAY_METH_LEN);
        ap_rprintf(r,"		<PAY_METHOD>%s</PAY_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->pay_method));
    gbp->count += tt0007_PAY_METH_LEN;

    memset(ptr_recvbuf->num_levels, '\0', tt0007_LEVEL_LEN + 1);
    memcpy(ptr_recvbuf->num_levels, sz_recvbufcat + gbp->count, tt0007_LEVEL_LEN);
        ap_rprintf(r,"		<NUM_OF_LEVELS>%s</NUM_OF_LEVELS>\n", handle_special_chars(gbp,ptr_recvbuf->num_levels));
    gbp->count += tt0007_LEVEL_LEN;

    memset(ptr_recvbuf->page_no, '\0', tt0007_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, tt0007_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NO>%s</PAGE_NO>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += tt0007_PAGE_NO_LEN;

    memset(ptr_recvbuf->tot_levels, '\0', tt0007_TOT_LEVEL_LEN + 1);
    memcpy(ptr_recvbuf->tot_levels, sz_recvbufcat + gbp->count, tt0007_TOT_LEVEL_LEN);
        ap_rprintf(r,"		<TOT_LEVELS>%s</TOT_LEVELS>\n", handle_special_chars(gbp,ptr_recvbuf->tot_levels));
    gbp->count += tt0007_TOT_LEVEL_LEN;

        memset(ptr_recvbuf->order_no, '\0', tt0007_ORD_NO_LEN + 1);
    memcpy(ptr_recvbuf->order_no, sz_recvbufcat + gbp->count, tt0007_ORD_NO_LEN);
        ap_rprintf(r,"		<ORD_NUM>%s</ORD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->order_no));
    gbp->count += tt0007_ORD_NO_LEN;

        memset(ptr_recvbuf->purchase_ord_num, '\0', tt0007_PO_NUM_LEN + 1);
    memcpy(ptr_recvbuf->purchase_ord_num, sz_recvbufcat + gbp->count, tt0007_PO_NUM_LEN);
        ap_rprintf(r,"		<PURCH_ORD_NO>%s</PURCH_ORD_NO>\n", handle_special_chars(gbp,ptr_recvbuf->purchase_ord_num));
    gbp->count += tt0007_PO_NUM_LEN;

memset(ptr_recvbuf->status, '\0', tt0007_STATUS_LEN + 1);
    memcpy(ptr_recvbuf->status, sz_recvbufcat + gbp->count, tt0007_STATUS_LEN);
        ap_rprintf(r,"		<ORDER_STATUS>%s</ORDER_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->status));
    gbp->count += tt0007_STATUS_LEN;

    memset(ptr_recvbuf->filler1, '\0', tt0007_FILLER1_LEN + 1);
    memcpy(ptr_recvbuf->filler1, sz_recvbufcat + gbp->count, tt0007_FILLER1_LEN);
    gbp->count += tt0007_FILLER1_LEN;


        ap_rprintf(r,"		<ORDER_DETAILS>\n");

                memset(ptr_recvbuf->addr_1, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_1, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE1>%s</SHIP_TO_ADDR_LINE1>\n", handle_special_chars(gbp,ptr_recvbuf->addr_1));                 
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->addr_2, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_2, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE2>%s</SHIP_TO_ADDR_LINE2>\n", handle_special_chars(gbp,ptr_recvbuf->addr_2));
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->addr_3, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_3, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE3>%s</SHIP_TO_ADDR_LINE3>\n", handle_special_chars(gbp,ptr_recvbuf->addr_3));
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->addr_4, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_4, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE4>%s</SHIP_TO_ADDR_LINE4>\n", handle_special_chars(gbp,ptr_recvbuf->addr_4));
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->addr_5, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_5, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE5>%s</SHIP_TO_ADDR_LINE5>\n", handle_special_chars(gbp,ptr_recvbuf->addr_5));
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->addr_6, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_6, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE6>%s</SHIP_TO_ADDR_LINE6>\n", handle_special_chars(gbp,ptr_recvbuf->addr_6));
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->addr_7, '\0', tt0007_ADDR_LEN + 1);
                memcpy(ptr_recvbuf->addr_7, sz_recvbufcat + gbp->count, tt0007_ADDR_LEN);
                ap_rprintf(r,"			<SHIP_TO_ADDR_LINE7>%s</SHIP_TO_ADDR_LINE7>\n", handle_special_chars(gbp,ptr_recvbuf->addr_7));
                gbp->count += tt0007_ADDR_LEN;

                memset(ptr_recvbuf->line_item_num, '\0', tt0007_LINE_ITM_LEN + 1);
                memcpy(ptr_recvbuf->line_item_num, sz_recvbufcat + gbp->count, tt0007_LINE_ITM_LEN);
                ap_rprintf(r,"			<NUM_OF_LINE_ITEMS>%s</NUM_OF_LINE_ITEMS>\n", handle_special_chars(gbp,ptr_recvbuf->line_item_num));
                gbp->count += tt0007_LINE_ITM_LEN;

                memset(ptr_recvbuf->lvl_status, '\0', tt0007_LVL_STATUS_LEN + 1);
                memcpy(ptr_recvbuf->lvl_status, sz_recvbufcat + gbp->count, tt0007_LVL_STATUS_LEN);
                ap_rprintf(r,"			<STATUS_OF_ORD_LEVEL>%s</STATUS_OF_ORD_LEVEL>\n", handle_special_chars(gbp,ptr_recvbuf->lvl_status));
                gbp->count += tt0007_LVL_STATUS_LEN;

                memset(ptr_recvbuf->ship_method, '\0', tt0007_SHIP_METH_LEN + 1);
                memcpy(ptr_recvbuf->ship_method, sz_recvbufcat + gbp->count, tt0007_SHIP_METH_LEN);
                ap_rprintf(r,"			<ORD_LEVEL_SHIP_METHOD>%s</ORD_LEVEL_SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method));
                gbp->count += tt0007_SHIP_METH_LEN;

                memset(ptr_recvbuf->ship_method_type, '\0', tt0007_SM_TYPE_LEN + 1);
                memcpy(ptr_recvbuf->ship_method_type, sz_recvbufcat + gbp->count, tt0007_SM_TYPE_LEN);
                ap_rprintf(r,"			<SHIP_METHOD_TYPE>%s</SHIP_METHOD_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method_type));
                gbp->count += tt0007_SM_TYPE_LEN;
        
                memset(ptr_recvbuf->ship_date, '\0', tt0007_SHIP_DATE_LEN + 1);
                memcpy(ptr_recvbuf->ship_date, sz_recvbufcat + gbp->count, tt0007_SHIP_DATE_LEN);
                ap_rprintf(r,"			<LAST_SHIP_DATE>%s</LAST_SHIP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->ship_date));
                gbp->count += tt0007_SHIP_DATE_LEN;

                memset(ptr_recvbuf->track_num, '\0', tt0007_TRACK_LEN + 1);
                memcpy(ptr_recvbuf->track_num, sz_recvbufcat + gbp->count, tt0007_TRACK_LEN);
                ap_rprintf(r,"			<TRACKING_NUM>%s</TRACKING_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->track_num));
                gbp->count += tt0007_TRACK_LEN;

                memset(ptr_recvbuf->suffix, '\0', tt0007_SUFFIX_LEN + 1);
                memcpy(ptr_recvbuf->suffix, sz_recvbufcat + gbp->count, tt0007_SUFFIX_LEN);
                ap_rprintf(r,"			<ORD_LEVEL_SUFFIX>%s</ORD_LEVEL_SUFFIX>\n", handle_special_chars(gbp,ptr_recvbuf->suffix));
                gbp->count += tt0007_SUFFIX_LEN;

                memset(ptr_recvbuf->filler2, '\0', tt0007_FILLER2_LEN + 1);
                memcpy(ptr_recvbuf->filler2, sz_recvbufcat + gbp->count, tt0007_FILLER2_LEN);
                gbp->count += tt0007_FILLER2_LEN;

                ap_rprintf(r,"		</ORDER_DETAILS>\n");

                for(gbp->k = 0; gbp->k < tt0007_LINE_ITEM_COUNT; gbp->k++) 
                {
                memset(ptr_recvbuf->item[gbp->k].item_num, '\0', tt0007_ITM_NO_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].item_num, sz_recvbufcat + gbp->count, tt0007_ITM_NO_LEN);
                        gbp->count += tt0007_ITM_NO_LEN;

                        memset(ptr_recvbuf->item[gbp->k].item_desc, '\0', tt0007_ITM_DESC_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].item_desc, sz_recvbufcat + gbp->count, tt0007_ITM_DESC_LEN);
                        gbp->count += tt0007_ITM_DESC_LEN;

                        memset(ptr_recvbuf->item[gbp->k].item_status, '\0', tt0007_ITM_STATUS_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].item_status, sz_recvbufcat + gbp->count, tt0007_ITM_STATUS_LEN);
                        gbp->count += tt0007_ITM_STATUS_LEN;

                        memset(ptr_recvbuf->item[gbp->k].item_qty, '\0', tt0007_ITM_QTY_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].item_qty, sz_recvbufcat + gbp->count, tt0007_ITM_QTY_LEN);
                        gbp->count += tt0007_ITM_QTY_LEN;

                        memset(ptr_recvbuf->item[gbp->k].item_price, '\0', tt0007_ITM_PRICE_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].item_price, sz_recvbufcat + gbp->count, tt0007_ITM_PRICE_LEN);
                        gbp->count += tt0007_ITM_PRICE_LEN;

                        memset(ptr_recvbuf->item[gbp->k].item_gift_cert, '\0', tt0007_GFT_CERT_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].item_gift_cert, sz_recvbufcat + gbp->count, tt0007_GFT_CERT_LEN);
                        gbp->count += tt0007_GFT_CERT_LEN;

            memset(ptr_recvbuf->item[gbp->k].item_type, '\0', tt0007_ITM_TYPE_LEN + 1);
            memcpy(ptr_recvbuf->item[gbp->k].item_type, sz_recvbufcat + gbp->count, tt0007_ITM_TYPE_LEN);
            gbp->count += tt0007_ITM_TYPE_LEN;
                }

                gbp->j = atoi(ptr_recvbuf->line_item_num);

                if(gbp->j > (int)tt0007_LINE_ITEM_COUNT)
                        gbp->j  = (int)tt0007_LINE_ITEM_COUNT;

                for(gbp->k = 0; gbp->k < gbp->j; gbp->k++) 
                {
                        ap_rprintf(r,"		<LINE_ITEM_DETAILS>\n");
                        ap_rprintf(r,"			<LINE_ITEM_PROD_NUM>%s</LINE_ITEM_PROD_NUM>\n", 
                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_num));
                        ap_rprintf(r,"			<LINE_ITEM_PROD_DESCRIPTION>%s</LINE_ITEM_PROD_DESCRIPTION>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_desc));
                        ap_rprintf(r,"			<LINE_ITEM_STATUS>%s</LINE_ITEM_STATUS>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_status));
                        ap_rprintf(r,"			<LINE_ITEM_SALEABLE_QTY>%s</LINE_ITEM_SALEABLE_QTY>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_qty));
                        ap_rprintf(r,"			<LINE_ITEM_PROD_PRICE>%s</LINE_ITEM_PROD_PRICE>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_price));
                        ap_rprintf(r,"			<LINE_ITEM_GIFT_CERT>%s</LINE_ITEM_GIFT_CERT>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_gift_cert));
                        ap_rprintf(r,"			<LINE_ITEM_TYPE>%s</LINE_ITEM_TYPE>\n",
                                            handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].item_type));
                        ap_rprintf(r,"		</LINE_ITEM_DETAILS>\n");
                }


                memset(ptr_recvbuf->py_edp, '\0', tt0007_PY_EDP_LEN + 1);
                memcpy(ptr_recvbuf->py_edp, sz_recvbufcat + gbp->count, tt0007_PY_EDP_LEN);
                ap_rprintf(r,"		<PY_EDP>%s</PY_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->py_edp));
                gbp->count += tt0007_PY_EDP_LEN;

                memset(ptr_recvbuf->py_custno, '\0', tt0007_PY_CUSTNO_LEN + 1);
                memcpy(ptr_recvbuf->py_custno, sz_recvbufcat + gbp->count, tt0007_PY_CUSTNO_LEN);
                ap_rprintf(r,"		<PY_CUSTNO>%s</PY_CUSTNO>\n", handle_special_chars(gbp,ptr_recvbuf->py_custno));
                gbp->count += tt0007_PY_CUSTNO_LEN;

                memset(ptr_recvbuf->py_fname, '\0', tt0007_PY_FNAME_LEN + 1);
                memcpy(ptr_recvbuf->py_fname, sz_recvbufcat + gbp->count, tt0007_PY_FNAME_LEN);
                ap_rprintf(r,"		<PY_FNAME>%s</PY_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->py_fname));
                gbp->count += tt0007_PY_FNAME_LEN;

                memset(ptr_recvbuf->py_mi, '\0', tt0007_PY_MI_LEN + 1);
                memcpy(ptr_recvbuf->py_mi, sz_recvbufcat + gbp->count, tt0007_PY_MI_LEN);
                ap_rprintf(r,"		<PY_MI>%s</PY_MI>\n", handle_special_chars(gbp,ptr_recvbuf->py_mi));
                gbp->count += tt0007_PY_MI_LEN;

                memset(ptr_recvbuf->py_lname, '\0', tt0007_PY_LNAME_LEN + 1);
                memcpy(ptr_recvbuf->py_lname, sz_recvbufcat + gbp->count, tt0007_PY_LNAME_LEN);
                ap_rprintf(r,"		<PY_LNAME>%s</PY_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->py_lname));
                gbp->count += tt0007_PY_LNAME_LEN;

                memset(ptr_recvbuf->py_company, '\0', tt0007_PY_COMPANY_LEN + 1);
                memcpy(ptr_recvbuf->py_company, sz_recvbufcat + gbp->count, tt0007_PY_COMPANY_LEN);
                ap_rprintf(r,"		<PY_COMPANY>%s</PY_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->py_company));
                gbp->count += tt0007_PY_COMPANY_LEN;

                memset(ptr_recvbuf->py_street, '\0', tt0007_PY_STREET_LEN + 1);
                memcpy(ptr_recvbuf->py_street, sz_recvbufcat + gbp->count, tt0007_PY_STREET_LEN);
                ap_rprintf(r,"		<PY_STREET>%s</PY_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->py_street));
                gbp->count += tt0007_PY_STREET_LEN;

                memset(ptr_recvbuf->py_ref1, '\0', tt0007_PY_REF1_LEN + 1);
                memcpy(ptr_recvbuf->py_ref1, sz_recvbufcat + gbp->count, tt0007_PY_REF1_LEN);
                ap_rprintf(r,"		<PY_REF1>%s</PY_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->py_ref1));
                gbp->count += tt0007_PY_REF1_LEN;

                memset(ptr_recvbuf->py_ref2, '\0', tt0007_PY_REF2_LEN + 1);
                memcpy(ptr_recvbuf->py_ref2, sz_recvbufcat + gbp->count, tt0007_PY_REF2_LEN);
                ap_rprintf(r,"		<PY_REF2>%s</PY_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->py_ref2));
                gbp->count += tt0007_PY_REF2_LEN;

                memset(ptr_recvbuf->py_city, '\0', tt0007_PY_CITY_LEN + 1);
                memcpy(ptr_recvbuf->py_city, sz_recvbufcat + gbp->count, tt0007_PY_CITY_LEN);
                ap_rprintf(r,"		<PY_CITY>%s</PY_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->py_city));
                gbp->count += tt0007_PY_CITY_LEN;

                memset(ptr_recvbuf->py_state, '\0', tt0007_PY_STATE_LEN + 1);
                memcpy(ptr_recvbuf->py_state, sz_recvbufcat + gbp->count, tt0007_PY_STATE_LEN);
                ap_rprintf(r,"		<PY_STATE>%s</PY_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->py_state));
                gbp->count += tt0007_PY_STATE_LEN;

                memset(ptr_recvbuf->py_zip, '\0', tt0007_PY_ZIP_LEN + 1);
                memcpy(ptr_recvbuf->py_zip, sz_recvbufcat + gbp->count, tt0007_PY_ZIP_LEN);
                ap_rprintf(r,"		<PY_ZIP>%s</PY_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->py_zip));
                gbp->count += tt0007_PY_ZIP_LEN;

                memset(ptr_recvbuf->dm_edp, '\0', tt0007_DM_EDP_LEN + 1);
                memcpy(ptr_recvbuf->dm_edp, sz_recvbufcat + gbp->count, tt0007_DM_EDP_LEN);
                ap_rprintf(r,"		<DM_EDP>%s</DM_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->dm_edp));
                gbp->count += tt0007_DM_EDP_LEN;

                memset(ptr_recvbuf->dm_custno, '\0', tt0007_DM_CUSTNO_LEN + 1);
                memcpy(ptr_recvbuf->dm_custno, sz_recvbufcat + gbp->count, tt0007_DM_CUSTNO_LEN);
                ap_rprintf(r,"		<DM_CUSTNO>%s</DM_CUSTNO>\n", handle_special_chars(gbp,ptr_recvbuf->dm_custno));
                gbp->count += tt0007_DM_CUSTNO_LEN;

                memset(ptr_recvbuf->dm_fname, '\0', tt0007_DM_FNAME_LEN + 1);
                memcpy(ptr_recvbuf->dm_fname, sz_recvbufcat + gbp->count, tt0007_DM_FNAME_LEN);
                ap_rprintf(r,"		<DM_FNAME>%s</DM_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->dm_fname));
                gbp->count += tt0007_DM_FNAME_LEN;

                memset(ptr_recvbuf->dm_mi, '\0', tt0007_DM_MI_LEN + 1);
                memcpy(ptr_recvbuf->dm_mi, sz_recvbufcat + gbp->count, tt0007_DM_MI_LEN);
                ap_rprintf(r,"		<DM_MI>%s</DM_MI>\n", handle_special_chars(gbp,ptr_recvbuf->dm_mi));
                gbp->count += tt0007_DM_MI_LEN;

                memset(ptr_recvbuf->dm_lname, '\0', tt0007_DM_LNAME_LEN + 1);
                memcpy(ptr_recvbuf->dm_lname, sz_recvbufcat + gbp->count, tt0007_DM_LNAME_LEN);
                ap_rprintf(r,"		<DM_LNAME>%s</DM_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->dm_lname));
                gbp->count += tt0007_DM_LNAME_LEN;

                memset(ptr_recvbuf->dm_company, '\0', tt0007_DM_COMPANY_LEN + 1);
                memcpy(ptr_recvbuf->dm_company, sz_recvbufcat + gbp->count, tt0007_DM_COMPANY_LEN);
                ap_rprintf(r,"		<DM_COMPANY>%s</DM_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->dm_company));
                gbp->count += tt0007_DM_COMPANY_LEN;

                memset(ptr_recvbuf->dm_street, '\0', tt0007_DM_STREET_LEN + 1);
                memcpy(ptr_recvbuf->dm_street, sz_recvbufcat + gbp->count, tt0007_DM_STREET_LEN);
                ap_rprintf(r,"		<DM_STREET>%s</DM_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->dm_street));
                gbp->count += tt0007_DM_STREET_LEN;

                memset(ptr_recvbuf->dm_ref1, '\0', tt0007_DM_REF1_LEN + 1);
                memcpy(ptr_recvbuf->dm_ref1, sz_recvbufcat + gbp->count, tt0007_DM_REF1_LEN);
                ap_rprintf(r,"		<DM_REF1>%s</DM_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->dm_ref1));
                gbp->count += tt0007_DM_REF1_LEN;

                memset(ptr_recvbuf->dm_ref2, '\0', tt0007_DM_REF2_LEN + 1);
                memcpy(ptr_recvbuf->dm_ref2, sz_recvbufcat + gbp->count, tt0007_DM_REF2_LEN);
                ap_rprintf(r,"		<DM_REF2>%s</DM_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->dm_ref2));
                gbp->count += tt0007_DM_REF2_LEN;

                memset(ptr_recvbuf->dm_city, '\0', tt0007_DM_CITY_LEN + 1);
                memcpy(ptr_recvbuf->dm_city, sz_recvbufcat + gbp->count, tt0007_DM_CITY_LEN);
                ap_rprintf(r,"		<DM_CITY>%s</DM_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->dm_city));
                gbp->count += tt0007_DM_CITY_LEN;

                memset(ptr_recvbuf->dm_state, '\0', tt0007_DM_STATE_LEN + 1);
                memcpy(ptr_recvbuf->dm_state, sz_recvbufcat + gbp->count, tt0007_DM_STATE_LEN);
                ap_rprintf(r,"		<DM_STATE>%s</DM_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->dm_state));
                gbp->count += tt0007_DM_STATE_LEN;

                memset(ptr_recvbuf->dm_zip, '\0', tt0007_DM_ZIP_LEN + 1);
                memcpy(ptr_recvbuf->dm_zip, sz_recvbufcat + gbp->count, tt0007_DM_ZIP_LEN);
                ap_rprintf(r,"		<DM_ZIP>%s</DM_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->dm_zip));
                gbp->count += tt0007_DM_ZIP_LEN;

                memset(ptr_recvbuf->si_edp, '\0', tt0007_SI_EDP_LEN + 1);
                memcpy(ptr_recvbuf->si_edp, sz_recvbufcat + gbp->count, tt0007_SI_EDP_LEN);
                ap_rprintf(r,"		<SI_EDP>%s</SI_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->si_edp));
                gbp->count += tt0007_SI_EDP_LEN;

                memset(ptr_recvbuf->si_custno, '\0', tt0007_SI_CUSTNO_LEN + 1);
                memcpy(ptr_recvbuf->si_custno, sz_recvbufcat + gbp->count, tt0007_SI_CUSTNO_LEN);
                ap_rprintf(r,"		<SI_CUSTNO>%s</SI_CUSTNO>\n", handle_special_chars(gbp,ptr_recvbuf->si_custno));
                gbp->count += tt0007_SI_CUSTNO_LEN;

                memset(ptr_recvbuf->si_fname, '\0', tt0007_SI_FNAME_LEN + 1);
                memcpy(ptr_recvbuf->si_fname, sz_recvbufcat + gbp->count, tt0007_SI_FNAME_LEN);
                ap_rprintf(r,"		<SI_FNAME>%s</SI_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->si_fname));
                gbp->count += tt0007_SI_FNAME_LEN;

                memset(ptr_recvbuf->si_mi, '\0', tt0007_SI_MI_LEN + 1);
                memcpy(ptr_recvbuf->si_mi, sz_recvbufcat + gbp->count, tt0007_SI_MI_LEN);
                ap_rprintf(r,"		<SI_MI>%s</SI_MI>\n", handle_special_chars(gbp,ptr_recvbuf->si_mi));
                gbp->count += tt0007_SI_MI_LEN;

                memset(ptr_recvbuf->si_lname, '\0', tt0007_SI_LNAME_LEN + 1);
                memcpy(ptr_recvbuf->si_lname, sz_recvbufcat + gbp->count, tt0007_SI_LNAME_LEN);
                ap_rprintf(r,"		<SI_LNAME>%s</SI_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->si_lname));
                gbp->count += tt0007_SI_LNAME_LEN;

                memset(ptr_recvbuf->si_company, '\0', tt0007_SI_COMPANY_LEN + 1);
                memcpy(ptr_recvbuf->si_company, sz_recvbufcat + gbp->count, tt0007_SI_COMPANY_LEN);
                ap_rprintf(r,"		<SI_COMPANY>%s</SI_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->si_company));
                gbp->count += tt0007_SI_COMPANY_LEN;

                memset(ptr_recvbuf->si_street, '\0', tt0007_SI_STREET_LEN + 1);
                memcpy(ptr_recvbuf->si_street, sz_recvbufcat + gbp->count, tt0007_SI_STREET_LEN);
                ap_rprintf(r,"		<SI_STREET>%s</SI_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->si_street));
                gbp->count += tt0007_SI_STREET_LEN;

                memset(ptr_recvbuf->si_ref1, '\0', tt0007_SI_REF1_LEN + 1);
                memcpy(ptr_recvbuf->si_ref1, sz_recvbufcat + gbp->count, tt0007_SI_REF1_LEN);
                ap_rprintf(r,"		<SI_REF1>%s</SI_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->si_ref1));
                gbp->count += tt0007_SI_REF1_LEN;

                memset(ptr_recvbuf->si_ref2, '\0', tt0007_SI_REF2_LEN + 1);
                memcpy(ptr_recvbuf->si_ref2, sz_recvbufcat + gbp->count, tt0007_SI_REF2_LEN);
                ap_rprintf(r,"		<SI_REF2>%s</SI_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->si_ref2));
                gbp->count += tt0007_SI_REF2_LEN;

                memset(ptr_recvbuf->si_city, '\0', tt0007_SI_CITY_LEN + 1);
                memcpy(ptr_recvbuf->si_city, sz_recvbufcat + gbp->count, tt0007_SI_CITY_LEN);
                ap_rprintf(r,"		<SI_CITY>%s</SI_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->si_city));
                gbp->count += tt0007_SI_CITY_LEN;

                memset(ptr_recvbuf->si_state, '\0', tt0007_SI_STATE_LEN + 1);
                memcpy(ptr_recvbuf->si_state, sz_recvbufcat + gbp->count, tt0007_SI_STATE_LEN);
                ap_rprintf(r,"		<SI_STATE>%s</SI_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->si_state));
                gbp->count += tt0007_SI_STATE_LEN;

                memset(ptr_recvbuf->si_zip, '\0', tt0007_SI_ZIP_LEN + 1);
                memcpy(ptr_recvbuf->si_zip, sz_recvbufcat + gbp->count, tt0007_SI_ZIP_LEN);
                ap_rprintf(r,"		<SI_ZIP>%s</SI_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->si_zip));
                gbp->count += tt0007_SI_ZIP_LEN;
        
        ap_rprintf(r,"  %s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"  %s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"  %s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0008>\n", gbp->tt_betag);

    return 0;
}



