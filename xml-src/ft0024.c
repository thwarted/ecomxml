/* ft0024.c  Product Shop */
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


int             ft0024_CatSendStr(struct global_struct *, char *, ft0024_st_send *);
int             ft0024_ParceRecvStr(ft0024_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0024_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0024_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0024_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0024_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0024_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0024 = (ft0024_st_send *)malloc(sizeof(ft0024_st_send));
        gbp->ftrecvbuf0024 = (ft0024_st_recv *)malloc(sizeof(ft0024_st_recv));

        gbp->sendbufcat = malloc(ft0024_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0024_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0024_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0024_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0024,  '\0',  sizeof(ft0024_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0024->request_id, "XML");
    strcpy(gbp->ftsendbuf0024->record_id, "0024");
    strcpy(gbp->ftsendbuf0024->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0024->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0024->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0024->userid,gbp,stdout_buffer);
    get_tag_data("PAGE_NUM", gbp->ftsendbuf0024->page_no,gbp,stdout_buffer);
    get_tag_data("PAGE_COUNT", gbp->ftsendbuf0024->page_cnt,gbp,stdout_buffer);
    get_tag_data("ITEM_NUM", gbp->ftsendbuf0024->item_no,gbp,stdout_buffer);
    get_tag_data("ITEM_QTY", gbp->ftsendbuf0024->item_qty,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_NUM", gbp->ftsendbuf0024->cust_no,gbp,stdout_buffer);
    get_tag_data("OFFER_NUM", gbp->ftsendbuf0024->offer_no,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_TYPE", gbp->ftsendbuf0024->cust_type,gbp,stdout_buffer);
    get_tag_data("CONTRACT", gbp->ftsendbuf0024->contract,gbp,stdout_buffer);

    for (gbp->i = 0; gbp->i < ft0024_VAR_COMP_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "VAR_ITEM_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0024->var_item_no[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "VAR_QTY_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0024->var_item_qty[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "VAR_PRICE_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0024->var_item_price[gbp->i],gbp,stdout_buffer);
	}
    get_tag_data("SHOP_TYPE", gbp->ftsendbuf0024->shop_type,gbp,stdout_buffer);
    get_tag_data("SHIPTO_COUNT", gbp->ftsendbuf0024->shipto_cnt,gbp,stdout_buffer);

        if(ft0024_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0024) == ft0024_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0024);
                        free (gbp->ftrecvbuf0024);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0024_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0024);
                        free (gbp->ftrecvbuf0024);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0024_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0024);
                        free (gbp->ftrecvbuf0024);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0024);
                        free (gbp->ftrecvbuf0024);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0024_ParceRecvStr(gbp->ftrecvbuf0024, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0024);
                free (gbp->ftrecvbuf0024);

        }
        else
        {
                XML_Error("ft0024_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0024);
                free (gbp->ftrecvbuf0024);
                return(-1);
        }

    return(0);
}


int ft0024_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0024_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%04ds%04d%-20.20s%04d%-10.10s%-8.8s%-2.2s%-8.8s"
            "%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d"
            "%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d"
            "%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d"
            "%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d"
            "%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d%-20.20s%04d%09d"
            "%-1.1s%-2.2s",
            
			ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            atoi(ptr_sendbuf->page_no),
            atoi(ptr_sendbuf->page_cnt),
            ptr_sendbuf->item_no,
            atoi(ptr_sendbuf->item_qty),
            ptr_sendbuf->cust_no,
            ptr_sendbuf->offer_no,
            ptr_sendbuf->cust_type,
            ptr_sendbuf->contract,
            ptr_sendbuf->var_item_no[0],
            atoi(ptr_sendbuf->var_item_qty[0]),
			(int)((atof(ptr_sendbuf->var_item_price[0]))*100),
            ptr_sendbuf->var_item_no[1],
            atoi(ptr_sendbuf->var_item_qty[1]),
			(int)((atof(ptr_sendbuf->var_item_price[1]))*100),
            ptr_sendbuf->var_item_no[2],
            atoi(ptr_sendbuf->var_item_qty[2]),
			(int)((atof(ptr_sendbuf->var_item_price[2]))*100),
            ptr_sendbuf->var_item_no[3],
            atoi(ptr_sendbuf->var_item_qty[3]),
			(int)((atof(ptr_sendbuf->var_item_price[3]))*100),
            ptr_sendbuf->var_item_no[4],
            atoi(ptr_sendbuf->var_item_qty[4]),
			(int)((atof(ptr_sendbuf->var_item_price[4]))*100),
            ptr_sendbuf->var_item_no[5],
            atoi(ptr_sendbuf->var_item_qty[5]),
			(int)((atof(ptr_sendbuf->var_item_price[5]))*100),
            ptr_sendbuf->var_item_no[6],
            atoi(ptr_sendbuf->var_item_qty[6]),
			(int)((atof(ptr_sendbuf->var_item_price[6]))*100),
            ptr_sendbuf->var_item_no[7],
            atoi(ptr_sendbuf->var_item_qty[7]),
			(int)((atof(ptr_sendbuf->var_item_price[7]))*100),
            ptr_sendbuf->var_item_no[8],
            atoi(ptr_sendbuf->var_item_qty[8]),
			(int)((atof(ptr_sendbuf->var_item_price[8]))*100),
            ptr_sendbuf->var_item_no[9],
            atoi(ptr_sendbuf->var_item_qty[9]),
			(int)((atof(ptr_sendbuf->var_item_price[9]))*100),
            ptr_sendbuf->var_item_no[10],
            atoi(ptr_sendbuf->var_item_qty[10]),
			(int)((atof(ptr_sendbuf->var_item_price[10]))*100),
            ptr_sendbuf->var_item_no[11],
            atoi(ptr_sendbuf->var_item_qty[11]),
			(int)((atof(ptr_sendbuf->var_item_price[11]))*100),
            ptr_sendbuf->var_item_no[12],
            atoi(ptr_sendbuf->var_item_qty[12]),
			(int)((atof(ptr_sendbuf->var_item_price[12]))*100),
            ptr_sendbuf->var_item_no[13],
            atoi(ptr_sendbuf->var_item_qty[13]),
			(int)((atof(ptr_sendbuf->var_item_price[13]))*100),
            ptr_sendbuf->var_item_no[14],
            atoi(ptr_sendbuf->var_item_qty[14]),
			(int)((atof(ptr_sendbuf->var_item_price[14]))*100),
            ptr_sendbuf->var_item_no[15],
            atoi(ptr_sendbuf->var_item_qty[15]),
			(int)((atof(ptr_sendbuf->var_item_price[15]))*100),
            ptr_sendbuf->var_item_no[16],
            atoi(ptr_sendbuf->var_item_qty[16]),
			(int)((atof(ptr_sendbuf->var_item_price[16]))*100),
            ptr_sendbuf->var_item_no[17],
            atoi(ptr_sendbuf->var_item_qty[17]),
			(int)((atof(ptr_sendbuf->var_item_price[17]))*100),
            ptr_sendbuf->var_item_no[18],
            atoi(ptr_sendbuf->var_item_qty[18]),
			(int)((atof(ptr_sendbuf->var_item_price[18]))*100),
            ptr_sendbuf->var_item_no[19],
            atoi(ptr_sendbuf->var_item_qty[19]),
			(int)((atof(ptr_sendbuf->var_item_price[19]))*100),
            ptr_sendbuf->var_item_no[20],
            atoi(ptr_sendbuf->var_item_qty[20]),
			(int)((atof(ptr_sendbuf->var_item_price[20]))*100),
            ptr_sendbuf->var_item_no[21],
            atoi(ptr_sendbuf->var_item_qty[21]),
			(int)((atof(ptr_sendbuf->var_item_price[21]))*100),
            ptr_sendbuf->var_item_no[22],
            atoi(ptr_sendbuf->var_item_qty[22]),
			(int)((atof(ptr_sendbuf->var_item_price[22]))*100),
            ptr_sendbuf->var_item_no[23],
            atoi(ptr_sendbuf->var_item_qty[23]),
			(int)((atof(ptr_sendbuf->var_item_price[23]))*100),
            ptr_sendbuf->var_item_no[24],
            atoi(ptr_sendbuf->var_item_qty[24]),
			(int)((atof(ptr_sendbuf->var_item_price[24]))*100),
            ptr_sendbuf->shop_type,
            ptr_sendbuf->shipto_cnt);


	return(gbp->j);
}


int ft0024_ParceRecvStr(ft0024_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0024.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0024 %s\"ft0024\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0024_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0024_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0024_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0024_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0024_REC_ID_LEN );
    gbp->count += ft0024_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0024_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0024_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0024_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0024_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0024_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0024_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0024_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0024_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0024_ERR_LEN;

    gbp->count += ft0024_SEND_FILLER_LEN;

    memset(ptr_recvbuf->shop_cnt,'\0', ft0024_SHOP_CNT_LEN + 1);
    memcpy(ptr_recvbuf->shop_cnt, sz_recvbufcat + gbp->count, ft0024_SHOP_CNT_LEN);
        ap_rprintf(r,"		<SHOP_COUNT>%s</SHOP_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->shop_cnt));
    gbp->count += ft0024_SHOP_CNT_LEN;

    memset(ptr_recvbuf->page_no,'\0', ft0024_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, ft0024_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NUM>%s</PAGE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += ft0024_PAGE_NO_LEN;

    memset(ptr_recvbuf->shop_flag,'\0', ft0024_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->shop_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        ap_rprintf(r,"		<SHOP_FLAG>%s</SHOP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->shop_flag));
    gbp->count += ft0024_FLAG_LEN;

    memset(ptr_recvbuf->style_found_flag,'\0', ft0024_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->style_found_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        ap_rprintf(r,"		<STYLE_FOUND_FLAG>%s</STYLE_FOUND_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->style_found_flag));
    gbp->count += ft0024_FLAG_LEN;

    for( gbp->i = 0; gbp->i < ft0024_PRODUCT_LOOP; gbp->i++)
    {

        memset(ptr_recvbuf->product_detail[gbp->i].item_no ,'\0', ft0024_ITEM_NO_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].item_no, sz_recvbufcat + gbp->count, ft0024_ITEM_NO_LEN);
        gbp->count += ft0024_ITEM_NO_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].item_desc ,'\0', ft0024_DESC_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].item_desc, sz_recvbufcat + gbp->count, ft0024_DESC_LEN);
        gbp->count += ft0024_DESC_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].price ,'\0', ft0024_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].price, sz_recvbufcat + gbp->count, ft0024_PRICE_LEN);
        gbp->count += ft0024_PRICE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].inv_sign ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].inv_sign, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].avail_inv ,'\0', ft0024_INV_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].avail_inv, sz_recvbufcat + gbp->count, ft0024_INV_LEN);
        gbp->count += ft0024_INV_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].kit_qty ,'\0', ft0024_INV_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].kit_qty, sz_recvbufcat + gbp->count, ft0024_INV_LEN);
        gbp->count += ft0024_INV_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].item_status ,'\0', ft0024_STATUS_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].item_status, sz_recvbufcat + gbp->count, ft0024_STATUS_LEN);
        gbp->count += ft0024_STATUS_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].item_status_code ,'\0', ft0024_STATUS_CODE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].item_status_code, sz_recvbufcat + gbp->count, ft0024_STATUS_CODE_LEN);
        gbp->count += ft0024_STATUS_CODE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].oe_status ,'\0', ft0024_STATUS_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].oe_status, sz_recvbufcat + gbp->count, ft0024_STATUS_LEN);
        gbp->count += ft0024_STATUS_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].edp_no ,'\0', ft0024_EDP_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].edp_no, sz_recvbufcat + gbp->count, ft0024_EDP_LEN);
        gbp->count += ft0024_EDP_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].disc_code ,'\0', ft0024_DISC_CODE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].disc_code, sz_recvbufcat + gbp->count, ft0024_DISC_CODE_LEN);
        gbp->count += ft0024_DISC_CODE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].alt_ph_flag ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].alt_ph_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].warehouse ,'\0', ft0024_WAREHSE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].warehouse, sz_recvbufcat + gbp->count, ft0024_WAREHSE_LEN);
        gbp->count += ft0024_WAREHSE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].weight ,'\0', ft0024_WEIGHT_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].weight, sz_recvbufcat + gbp->count, ft0024_WEIGHT_LEN);
        gbp->count += ft0024_WEIGHT_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].assoc_flag ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].assoc_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].cost ,'\0', ft0024_COST_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].cost, sz_recvbufcat + gbp->count, ft0024_COST_LEN);
        gbp->count += ft0024_COST_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].min_price ,'\0', ft0024_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].min_price, sz_recvbufcat + gbp->count, ft0024_PRICE_LEN);
        gbp->count += ft0024_PRICE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].min_qty ,'\0', ft0024_QTY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].min_qty, sz_recvbufcat + gbp->count, ft0024_QTY_LEN);
        gbp->count += ft0024_QTY_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].max_qty ,'\0', ft0024_QTY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].max_qty, sz_recvbufcat + gbp->count, ft0024_QTY_LEN);
        gbp->count += ft0024_QTY_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].dim_weight ,'\0', ft0024_WEIGHT_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].dim_weight, sz_recvbufcat + gbp->count, ft0024_WEIGHT_LEN);
        gbp->count += ft0024_WEIGHT_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].source ,'\0', ft0024_SOURCE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].source, sz_recvbufcat + gbp->count, ft0024_SOURCE_LEN);
        gbp->count += ft0024_SOURCE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].full_price ,'\0', ft0024_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].full_price, sz_recvbufcat + gbp->count, ft0024_PRICE_LEN);
        gbp->count += ft0024_PRICE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].style ,'\0', ft0024_STYLE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].style, sz_recvbufcat + gbp->count, ft0024_STYLE_LEN);
        gbp->count += ft0024_STYLE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].length ,'\0', ft0024_DIMENSION_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].length, sz_recvbufcat + gbp->count, ft0024_DIMENSION_LEN);
        gbp->count += ft0024_DIMENSION_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].width ,'\0', ft0024_DIMENSION_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].width, sz_recvbufcat + gbp->count, ft0024_DIMENSION_LEN);
        gbp->count += ft0024_DIMENSION_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].depth ,'\0', ft0024_DIMENSION_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].depth, sz_recvbufcat + gbp->count, ft0024_DIMENSION_LEN);
        gbp->count += ft0024_DIMENSION_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ship_method ,'\0', ft0024_SHIP_METHOD_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ship_method, sz_recvbufcat + gbp->count, ft0024_SHIP_METHOD_LEN);
        gbp->count += ft0024_SHIP_METHOD_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ship_method_desc ,'\0', ft0024_SM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ship_method_desc, sz_recvbufcat + gbp->count, ft0024_SM_DESC_LEN);
        gbp->count += ft0024_SM_DESC_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].create_date ,'\0', ft0024_DATE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].create_date, sz_recvbufcat + gbp->count, ft0024_DATE_LEN);
        gbp->count += ft0024_DATE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].img_ovrd ,'\0', ft0024_IMG_OVRD_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].img_ovrd, sz_recvbufcat + gbp->count, ft0024_IMG_OVRD_LEN);
        gbp->count += ft0024_IMG_OVRD_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ins_item_no ,'\0', ft0024_ITEM_NO_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ins_item_no, sz_recvbufcat + gbp->count, ft0024_ITEM_NO_LEN);
        gbp->count += ft0024_ITEM_NO_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ins_item_qty ,'\0', ft0024_QTY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ins_item_qty, sz_recvbufcat + gbp->count, ft0024_QTY_LEN);
        gbp->count += ft0024_QTY_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ph_exclude_flag ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ph_exclude_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ph_amount ,'\0', ft0024_AMT_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ph_amount, sz_recvbufcat + gbp->count, ft0024_AMT_LEN);
        gbp->count += ft0024_AMT_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].company ,'\0', ft0024_COMPANY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].company, sz_recvbufcat + gbp->count, ft0024_COMPANY_LEN);
        gbp->count += ft0024_COMPANY_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].division ,'\0', ft0024_DIVISION_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].division, sz_recvbufcat + gbp->count, ft0024_DIVISION_LEN);
        gbp->count += ft0024_DIVISION_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].multiprice_flag ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].multiprice_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].multi_group ,'\0', ft0024_GROUP_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].multi_group, sz_recvbufcat + gbp->count, ft0024_GROUP_LEN);
        gbp->count += ft0024_GROUP_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ret_flag ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ret_flag, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].drop_ship ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].drop_ship, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].offer ,'\0', ft0024_OFFER_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].offer, sz_recvbufcat + gbp->count, ft0024_OFFER_LEN);
        gbp->count += ft0024_OFFER_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].category ,'\0', ft0024_CATEGORY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].category, sz_recvbufcat + gbp->count, ft0024_CATEGORY_LEN);
        gbp->count += ft0024_CATEGORY_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].reorder ,'\0', ft0024_REORDER_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].reorder, sz_recvbufcat + gbp->count, ft0024_REORDER_LEN);
        gbp->count += ft0024_REORDER_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].due_date ,'\0', ft0024_DATE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].due_date, sz_recvbufcat + gbp->count, ft0024_DATE_LEN);
        gbp->count += ft0024_DATE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].due_qty ,'\0', ft0024_DUE_QTY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].due_qty, sz_recvbufcat + gbp->count, ft0024_DUE_QTY_LEN);
        gbp->count += ft0024_DUE_QTY_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].restrict_ship ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].restrict_ship, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].future_date ,'\0', ft0024_DATE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].future_date, sz_recvbufcat + gbp->count, ft0024_DATE_LEN);
        gbp->count += ft0024_DATE_LEN;

        memset(ptr_recvbuf->product_detail[gbp->i].ser_no_reqd ,'\0', ft0024_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ser_no_reqd, sz_recvbufcat + gbp->count, ft0024_FLAG_LEN);
        gbp->count += ft0024_FLAG_LEN;

		if (strlen(ptr_recvbuf->product_detail[gbp->i].item_no) > 0) 
		{
			ap_rprintf(r,"          <PRODUCT_INFO>\n");
			ap_rprintf(r,"                  <ITEN_NUM>%s</ITEN_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].item_no));
			ap_rprintf(r,"                  <ITEM_DESC>%s</ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].item_desc));
			ap_rprintf(r,"                  <PRICE>%s</PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].price));
			ap_rprintf(r,"                  <INV_SIGN>%s</INV_SIGN>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].inv_sign));
			ap_rprintf(r,"                  <AVAIL_INV>%s</AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].avail_inv));
			ap_rprintf(r,"                  <KIT_QTY>%s</KIT_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].kit_qty));
			ap_rprintf(r,"                  <ITEM_STATUS>%s</ITEM_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].item_status));
			ap_rprintf(r,"                  <ITEM_STATUS_CODE>%s</ITEM_STATUS_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].item_status_code));
			ap_rprintf(r,"                  <OE_STATUS>%s</OE_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].oe_status));
			ap_rprintf(r,"                  <EDP_NO>%s</EDP_NO>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].edp_no));
			ap_rprintf(r,"                  <DISCOUNT_CODE>%s</DISCOUNT_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].disc_code));
			ap_rprintf(r,"                  <ALT_PH_FLAG>%s</ALT_PH_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].alt_ph_flag));
			ap_rprintf(r,"                  <WAREHOUSE>%s</WAREHOUSE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].warehouse));
			ap_rprintf(r,"                  <WEIGHT>%s</WEIGHT>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].weight));
			ap_rprintf(r,"                  <ASSOC_FLAG>%s</ASSOC_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].assoc_flag));
			ap_rprintf(r,"                  <COST>%s</COST>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].cost));
			ap_rprintf(r,"                  <MIN_PRICE>%s</MIN_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].min_price));
			ap_rprintf(r,"                  <MIN_QTY>%s</MIN_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].min_qty));
			ap_rprintf(r,"                  <MAX_QTY>%s</MAX_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].max_qty));
			ap_rprintf(r,"                  <DIM_WEIGHT>%s</DIM_WEIGHT>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].dim_weight));
			ap_rprintf(r,"                  <SOURCE>%s</SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].source));
			ap_rprintf(r,"                  <FULL_PRICE>%s</FULL_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].full_price));
			ap_rprintf(r,"                  <STYLE>%s</STYLE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].style));
			ap_rprintf(r,"                  <LENGTH>%s</LENGTH>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].length));
			ap_rprintf(r,"                  <WIDTH>%s</WIDTH>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].width));
			ap_rprintf(r,"                  <DEPTH>%s</DEPTH>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].depth));
			ap_rprintf(r,"                  <SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ship_method));
			ap_rprintf(r,"                  <SHIP_METHOD_DESC>%s</SHIP_METHOD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ship_method_desc));
			ap_rprintf(r,"                  <CREATE_DATE>%s</CREATE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].create_date));
			ap_rprintf(r,"                  <IMG_OVRD>%s</IMG_OVRD>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].img_ovrd));
			ap_rprintf(r,"                  <INS_ITEM_NUM>%s</INS_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ins_item_no));
			ap_rprintf(r,"                  <INS_ITEM_QTY>%s</INS_ITEM_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ins_item_qty));
			ap_rprintf(r,"                  <PH_EXCLUDE>%s</PH_EXCLUDE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ph_exclude_flag));
			ap_rprintf(r,"                  <PH_AMOUNT>%s</PH_AMOUNT>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ph_amount));
			ap_rprintf(r,"                  <COMPANY>%s</COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].company));
			ap_rprintf(r,"                  <DIVISION>%s</DIVISION>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].division));
			ap_rprintf(r,"                  <MULTI_PRICING_FLAG>%s</MULTI_PRICING_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].multiprice_flag));
			ap_rprintf(r,"                  <MULTI_GROUP>%s</MULTI_GROUP>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].multi_group));
			ap_rprintf(r,"                  <RETURN_FLAG>%s</RETURN_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ret_flag));
			ap_rprintf(r,"                  <DROP_SHIP>%s</DROP_SHIP>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].drop_ship));
			ap_rprintf(r,"                  <OFFER>%s</OFFER>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].offer));
			ap_rprintf(r,"                  <CATEGORY>%s</CATEGORY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].category));
			ap_rprintf(r,"                  <REORDER>%s</REORDER>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].reorder));
			ap_rprintf(r,"                  <DUE_DATE>%s</DUE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].due_date));
			ap_rprintf(r,"                  <DUE_QTY>%s</DUE_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].due_qty));
			ap_rprintf(r,"                  <RESTTRICT_SHIP>%s</RESTTRICT_SHIP>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].restrict_ship));
			ap_rprintf(r,"                  <FUTURE_DATE>%s</FUTURE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].future_date));
			ap_rprintf(r,"                  <SER_NO_REQD>%s</SER_NO_REQD>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ser_no_reqd));
			ap_rprintf(r,"          </PRODUCT_INFO>\n");
		}
	}

    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0024>\n", gbp->ft_betag);


    return(0);
}

