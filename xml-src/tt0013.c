/* tt0013.c Cart Content */
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


int             tt0013_CatSendStr(struct global_struct *,char *, tt0013_st_send *);
int             tt0013_ParceRecvStr(tt0013_st_recv *, char *, request_rec *, struct global_struct *);
int             tt0013_lt_process(request_rec *, struct global_struct *, char *);

int tt0013_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0013_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0013_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0013_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0013 = (tt0013_st_send *)malloc(sizeof(tt0013_st_send));
        gbp->recvbuf0013 = (tt0013_st_recv *)malloc(sizeof(tt0013_st_recv));

        gbp->sendbufcat = malloc(tt0013_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate gbp->sendbufcat\n");
                fflush(stdout);
                return(-1);
        }

        gbp->recvbufcat = malloc(tt0013_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate gbp->recvbufcat\n");
                fflush(stdout);
                return(-1);
        }

    memset(gbp->sendbufcat, '\0', tt0013_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0013_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0013,  '\0',  sizeof(tt0013_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0013->request_id, "XML");
    strcpy(gbp->sendbuf0013->record_id, "0013");

        strcpy(gbp->sendbuf0013->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0013->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0013->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0013->userid,gbp,stdout_buffer);
        get_tag_data("CART_ACTION_FLAG", gbp->sendbuf0013->empty_flag,gbp,stdout_buffer);
        get_tag_data("PAGE_NO", gbp->sendbuf0013->page_no,gbp,stdout_buffer);


    if((tt0013_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0013)) == tt0013_LAN_SEND_BUF_LEN )
    {
        if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
        {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0013);
                        free (gbp->recvbuf0013);
            return(-1);
        }

        if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0013_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0013);
                        free (gbp->recvbuf0013);
            return(-1);
        }

        if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0013_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0013);
                        free (gbp->recvbuf0013);
            return(-1);
        }


        if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
        {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0013);
                        free (gbp->recvbuf0013);
            return(-1);
        }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0013_ParceRecvStr(gbp->recvbuf0013, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0013);
                free (gbp->recvbuf0013);
    }
        else
        {
                XML_Error("tt0013_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0013);
                free (gbp->recvbuf0013);
                return(-1);
        }

    return(0);
}



int tt0013_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0013_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-2.2s",
                        ptr_sendbuf->request_id,
                        ptr_sendbuf->record_id,
                        ptr_sendbuf->company,
                        ptr_sendbuf->division,
                        ptr_sendbuf->userid,
                        ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->empty_flag,
                        ptr_sendbuf->page_no);

    return(gbp->j);
}

int tt0013_ParceRecvStr(tt0013_st_recv *ptr_recvbuf, char *sz_recvbufcat,request_rec *r, struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0014.xsl\"?>\n");

#endif
/*
        ap_rprintf(r,"<!DOCTYPE %s0014 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                tt_tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0014.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"<!-- CART CONTENTS -->\n");

        ap_rprintf(r,"%s0014 %s\"tt0014\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

        memset(ptr_recvbuf->request_id,'\0', tt0013_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0013_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0013_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0013_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0013_REC_ID_LEN );
    gbp->count += tt0013_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0013_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0013_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0013_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0013_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0013_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0013_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0013_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0013_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0013_ERR_LEN;

        gbp->count += tt0013_SEND_FILLER_LEN;


    memset(ptr_recvbuf->cust_edp, '\0', tt0013_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0013_EDP_LEN );
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0013_EDP_LEN;

    memset(ptr_recvbuf->source_code, '\0', tt0013_SRC_LEN + 1);
    memcpy(ptr_recvbuf->source_code, sz_recvbufcat + gbp->count, tt0013_SRC_LEN );
        ap_rprintf(r,"		<CART_SOURCE_CODE>%s</CART_SOURCE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->source_code));
    gbp->count += tt0013_SRC_LEN;

    memset(ptr_recvbuf->item_count, '\0', tt0013_ITM_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->item_count, sz_recvbufcat + gbp->count, tt0013_ITM_COUNT_LEN);
        ap_rprintf(r,"		<PROD_COUNT>%s</PROD_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->item_count));
    gbp->count += tt0013_ITM_COUNT_LEN;

    memset(ptr_recvbuf->page_no, '\0', tt0013_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, tt0013_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NO>%s</PAGE_NO>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += tt0013_PAGE_NO_LEN;

    memset(ptr_recvbuf->page_count, '\0', tt0013_PAGE_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->page_count, sz_recvbufcat + gbp->count, tt0013_PAGE_COUNT_LEN);
        ap_rprintf(r,"		<PAGE_COUNT>%s</PAGE_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->page_count));
    gbp->count += tt0013_PAGE_COUNT_LEN;

    memset(ptr_recvbuf->cart_total, '\0', tt0013_CART_TOTAL_LEN + 1);
    memcpy(ptr_recvbuf->cart_total, sz_recvbufcat + gbp->count, tt0013_CART_TOTAL_LEN);
        ap_rprintf(r,"		<CART_TOTAL>%s</CART_TOTAL>\n", handle_special_chars(gbp,ptr_recvbuf->cart_total));
    gbp->count += tt0013_CART_TOTAL_LEN;
        
        gbp->k = atoi(ptr_recvbuf->item_count);
        if(gbp->k > (int)tt0013_ITEM_COUNT)
                gbp->k = (int)tt0013_ITEM_COUNT;

    // Build data from array

    for(gbp->i = 0 ; gbp->i < tt0013_ITEM_COUNT ; gbp->i++) 
    {
        memset(ptr_recvbuf->item[gbp->i].item_edp, '\0', tt0013_ITM_EDP_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_edp, sz_recvbufcat + gbp->count, tt0013_ITM_EDP_LEN);
        gbp->count += tt0013_ITM_EDP_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_num, '\0', tt0013_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_num, sz_recvbufcat+gbp->count, tt0013_ITM_NO_LEN);
        gbp->count += tt0013_ITM_NO_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_qty, '\0', tt0013_ITM_QTY_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_qty, sz_recvbufcat+gbp->count, tt0013_ITM_QTY_LEN);
        gbp->count += tt0013_ITM_QTY_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_desc, '\0', tt0013_ITM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_desc, sz_recvbufcat+gbp->count, tt0013_ITM_DESC_LEN);
                gbp->count += tt0013_ITM_DESC_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_customization_flag, '\0', tt0013_ITM_CUSTOM_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_customization_flag, sz_recvbufcat+gbp->count, tt0013_ITM_CUSTOM_FLAG_LEN);
                gbp->count += tt0013_ITM_CUSTOM_FLAG_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_price, '\0', tt0013_ITM_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_price, sz_recvbufcat+gbp->count, tt0013_ITM_PRICE_LEN);
        gbp->count += tt0013_ITM_PRICE_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_avail_inv , '\0', tt0013_ITM_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_avail_inv, sz_recvbufcat + gbp->count, tt0013_ITM_AVAIL_INV_LEN);
        gbp->count += tt0013_ITM_AVAIL_INV_LEN;

        memset(ptr_recvbuf->item[gbp->i].shipto_num, '\0', tt0013_SHIPTO_NO_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].shipto_num, sz_recvbufcat + gbp->count, tt0013_SHIPTO_NO_LEN);
        gbp->count += tt0013_SHIPTO_NO_LEN;

                memset(ptr_recvbuf->item[gbp->i].shipto_fname, '\0', tt0013_SHIPTO_FN_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].shipto_fname, sz_recvbufcat + gbp->count, tt0013_SHIPTO_FN_LEN);
        gbp->count += tt0013_SHIPTO_FN_LEN;

                memset(ptr_recvbuf->item[gbp->i].shipto_lname, '\0', tt0013_SHIPTO_LN_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].shipto_lname, sz_recvbufcat + gbp->count, tt0013_SHIPTO_LN_LEN);
        gbp->count += tt0013_SHIPTO_LN_LEN;

                memset(ptr_recvbuf->item[gbp->i].stand_ord_item, '\0', tt0013_STAND_ORD_ITEM_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].stand_ord_item, sz_recvbufcat + gbp->count, tt0013_STAND_ORD_ITEM_LEN);
        gbp->count += tt0013_STAND_ORD_ITEM_LEN;

                memset(ptr_recvbuf->item[gbp->i].stand_ord_date, '\0', tt0013_STAND_ORD_DATE_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].stand_ord_date, sz_recvbufcat + gbp->count, tt0013_STAND_ORD_DATE_LEN);
        gbp->count += tt0013_STAND_ORD_DATE_LEN;

                memset(ptr_recvbuf->item[gbp->i].gwrap_flag, '\0', tt0013_GWRAP_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].gwrap_flag, sz_recvbufcat + gbp->count, tt0013_GWRAP_FLAG_LEN);
        gbp->count += tt0013_GWRAP_FLAG_LEN;

                memset(ptr_recvbuf->item[gbp->i].gwrap_avail_inv, '\0', tt0013_GWRAP_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].gwrap_avail_inv, sz_recvbufcat + gbp->count, tt0013_GWRAP_AVAIL_INV_LEN);
        gbp->count += tt0013_GWRAP_AVAIL_INV_LEN;

                memset(ptr_recvbuf->item[gbp->i].frame_qty, '\0', tt0013_FRAME_QTY_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].frame_qty, sz_recvbufcat + gbp->count, tt0013_FRAME_QTY_LEN);
        gbp->count += tt0013_FRAME_QTY_LEN;

                memset(ptr_recvbuf->item[gbp->i].price_ovrd_flag, '\0', tt0013_PRICE_OVRD_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].price_ovrd_flag, sz_recvbufcat + gbp->count, tt0013_PRICE_OVRD_FLAG_LEN);
        gbp->count += tt0013_PRICE_OVRD_FLAG_LEN;

        }

/* ... AND NOW WE OUTPUT THE ITEM DETAIL RECORDS */

    for(gbp->i = 0 ; gbp->i < gbp->k ; gbp->i++) 
    {
                ap_rprintf(r,"		<PRODUCT_DETAIL>\n");
                ap_rprintf(r,"			<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_edp));
                ap_rprintf(r,"			<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_num));
                ap_rprintf(r,"			<SALEABLE_QTY>%s</SALEABLE_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_qty));
                ap_rprintf(r,"			<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n",     handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_desc));         
                ap_rprintf(r,"			<PROD_CUSTOMIZATION_FLAG>%s</PROD_CUSTOMIZATION_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_customization_flag));
                ap_rprintf(r,"			<PROD_PRICE>%s</PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_price));
                ap_rprintf(r,"			<PROD_AVAIL_INV>%s</PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_avail_inv));
                ap_rprintf(r,"			<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shipto_num));
                ap_rprintf(r,"			<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shipto_fname));
                ap_rprintf(r,"			<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shipto_lname));
                ap_rprintf(r,"			<STAND_ORD_ITEM>%s</STAND_ORD_ITEM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].stand_ord_item));
                ap_rprintf(r,"			<STAND_ORD_DATE>%s</STAND_ORD_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].stand_ord_date));
                ap_rprintf(r,"			<GWRAP_FLAG>%s</GWRAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].gwrap_flag));
                ap_rprintf(r,"			<GWRAP_AVAIL_INV>%s</GWRAP_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].gwrap_avail_inv));
                ap_rprintf(r,"			<FRAME_QTY>%s</FRAME_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].frame_qty));
                ap_rprintf(r,"			<PRICE_OVRD_FLAG>%s</PRICE_OVRD_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].price_ovrd_flag));

                ap_rprintf(r,"		</PRODUCT_DETAIL>\n");
    }

        memset(ptr_recvbuf->gc_coup_flag, '\0', tt0013_GC_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->gc_coup_flag, sz_recvbufcat + gbp->count, tt0013_GC_FLAG_LEN);
        ap_rprintf(r,"		<GIFTCERT_COUPON_FLAG>%s</GIFTCERT_COUPON_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gc_coup_flag));
    gbp->count += tt0013_GC_FLAG_LEN;

        memset(ptr_recvbuf->stand_ord_cust, '\0', tt0013_STAND_ORD_CUST_LEN + 1);
    memcpy(ptr_recvbuf->stand_ord_cust, sz_recvbufcat + gbp->count, tt0013_STAND_ORD_CUST_LEN);
        ap_rprintf(r,"		<STAND_ORD_CUST>%s</STAND_ORD_CUST>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_cust));
    gbp->count += tt0013_STAND_ORD_CUST_LEN;

        memset(ptr_recvbuf->return_cust_flag, '\0', tt0013_RETURN_CUST_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->return_cust_flag, sz_recvbufcat + gbp->count, tt0013_RETURN_CUST_FLAG_LEN);
        ap_rprintf(r,"		<RETURN_CUST_FLAG>%s</RETURN_CUST_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->return_cust_flag));
    gbp->count += tt0013_RETURN_CUST_FLAG_LEN;
        
        
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0014>\n", gbp->tt_betag);

    return(0);
}





    



 

