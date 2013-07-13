/* tt0001.c  Product Quote */
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


int             tt0001_CatSendStr(struct global_struct *, char *, tt0001_st_send *);
int             tt0001_ParceRecvStr(tt0001_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0001_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int tt0001_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0001_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0001_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0001_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0001 = (tt0001_st_send *)malloc(sizeof(tt0001_st_send));
        gbp->recvbuf0001 = (tt0001_st_recv *)malloc(sizeof(tt0001_st_recv));

        gbp->sendbufcat = malloc(tt0001_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0001_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', tt0001_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0001_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0001,  '\0',  sizeof(tt0001_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0001->request_id, "XML");
    strcpy(gbp->sendbuf0001->record_id, "0001");

        strcpy(gbp->sendbuf0001->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0001->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0001->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0001->userid,gbp,stdout_buffer);
        get_tag_data("PROD_NUM", gbp->sendbuf0001->item_num,gbp,stdout_buffer);
        get_tag_data("PROD_QTY", gbp->sendbuf0001->item_qty,gbp,stdout_buffer);

        get_tag_data("URQA", gbp->sendbuf0001->upsell_req_a,gbp,stdout_buffer);
        if((strlen(gbp->sendbuf0001->upsell_req_a)) == 0)
        strcpy(gbp->sendbuf0001->upsell_req_a, "-");

        get_tag_data("URQB", gbp->sendbuf0001->upsell_req_b,gbp,stdout_buffer);
        if((strlen(gbp->sendbuf0001->upsell_req_b)) == 0)
        strcpy(gbp->sendbuf0001->upsell_req_b, "-");

        get_tag_data("URQC", gbp->sendbuf0001->upsell_req_c,gbp,stdout_buffer);
        if((strlen(gbp->sendbuf0001->upsell_req_c)) == 0)
        strcpy(gbp->sendbuf0001->upsell_req_c, "-");

        get_tag_data("URQD", gbp->sendbuf0001->upsell_req_d,gbp,stdout_buffer);
        if((strlen(gbp->sendbuf0001->upsell_req_d)) == 0)
        strcpy(gbp->sendbuf0001->upsell_req_d, "-");

        get_tag_data("STYLE_FLAG", gbp->sendbuf0001->is_style_flag,gbp,stdout_buffer);

        if(tt0001_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0001) == tt0001_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0001);
                        free (gbp->recvbuf0001);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0001_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0001);
                        free (gbp->recvbuf0001);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0001_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0001);
                        free (gbp->recvbuf0001);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0001);
                        free (gbp->recvbuf0001);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0001_ParceRecvStr(gbp->recvbuf0001, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0001);
                free (gbp->recvbuf0001);

        }
        else
        {
                XML_Error("tt0001_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0001);
                free (gbp->recvbuf0001);
                return(-1);
        }

    return(0);
}


int tt0001_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0001_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-20.20s%04d%-1.1s%-3.3s%-1.1s%-1.1s%-1.1s"
                        "%-1.1s%-1.1s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s"
                        "%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s"
                        "%-1.1s%-1.1s%-16.16s%-2.2s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->item_num,
            atoi(ptr_sendbuf->item_qty),
            ptr_sendbuf->cart_flag,
            ptr_sendbuf->cart_line_num,
            ptr_sendbuf->upsell_req_a,
            ptr_sendbuf->upsell_req_b,
            ptr_sendbuf->upsell_req_c,
            ptr_sendbuf->upsell_req_d,
            ptr_sendbuf->is_style_flag,
                        ptr_sendbuf->customize[0].customization,
                        ptr_sendbuf->customize[1].customization,
                        ptr_sendbuf->customize[2].customization,
                        ptr_sendbuf->customize[3].customization,
                        ptr_sendbuf->customize[4].customization,
                        ptr_sendbuf->customize[5].customization,
                        ptr_sendbuf->customize[6].customization,
                        ptr_sendbuf->customize[7].customization,
                        ptr_sendbuf->customize[8].customization,
                        ptr_sendbuf->customize[9].customization,        
                        ptr_sendbuf->customize[10].customization,
                        ptr_sendbuf->customize[11].customization,
                        ptr_sendbuf->customize[12].customization,
                        ptr_sendbuf->customize[13].customization,
                        ptr_sendbuf->customize[14].customization,
                        ptr_sendbuf->customize[15].customization,
                        ptr_sendbuf->customize[16].customization,
                        ptr_sendbuf->customize[17].customization,
                        ptr_sendbuf->customize[18].customization,
                        ptr_sendbuf->customize[19].customization,
                        ptr_sendbuf->inventory_reqd,
                        ptr_sendbuf->tvs_item,
                        ptr_sendbuf->tvs_source,
                        ptr_sendbuf->shipto_num);

    return(gbp->j);
}


int tt0001_ParceRecvStr(tt0001_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0002.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0002 %s\"tt0002\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0001_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0001_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0001_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0001_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0001_REC_ID_LEN );
    gbp->count += tt0001_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0001_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0001_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0001_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', tt0001_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, tt0001_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += tt0001_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->tvs_err_message,'\0', tt0001_TVS_ERR_LEN + 1);
    memcpy(ptr_recvbuf->tvs_err_message, sz_recvbufcat + gbp->count, tt0001_TVS_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->tvs_err_message));
    gbp->count += tt0001_TVS_ERR_LEN;

        gbp->count += tt0001_SEND_FILLER_LEN;

    memset(ptr_recvbuf->item_edp,'\0', tt0001_EDP_LEN+1);
    memcpy(ptr_recvbuf->item_edp, sz_recvbufcat + gbp->count, tt0001_EDP_LEN);
        ap_rprintf(r,"		<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item_edp));
    gbp->count += tt0001_EDP_LEN;

    memset(ptr_recvbuf->item_num,'\0', tt0001_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0001_ITM_NO_LEN);
        ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_num));
    gbp->count += tt0001_ITM_NO_LEN;

    memset(ptr_recvbuf->item_desc,'\0', tt0001_DESC_LEN+1);
    memcpy(ptr_recvbuf->item_desc, sz_recvbufcat + gbp->count, tt0001_DESC_LEN);
        ap_rprintf(r,"		<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->item_desc));
    gbp->count += tt0001_DESC_LEN;

    memset(ptr_recvbuf->item_price,'\0', tt0001_PRICE_LEN+1);
    memcpy(ptr_recvbuf->item_price, sz_recvbufcat + gbp->count, tt0001_PRICE_LEN);
        ap_rprintf(r,"		<PROD_PRICE>%s</PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->item_price));
    gbp->count += tt0001_PRICE_LEN;

    memset(ptr_recvbuf->item_avail_flag,'\0', tt0001_AVAIL_FLAG_LEN+1);
    memcpy(ptr_recvbuf->item_avail_flag, sz_recvbufcat + gbp->count, tt0001_AVAIL_FLAG_LEN);
        ap_rprintf(r,"		<AVAIL_INV_FLAG>%s</AVAIL_INV_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item_avail_flag));
    gbp->count += tt0001_AVAIL_FLAG_LEN;

    memset(ptr_recvbuf->item_avail_inv,'\0', tt0001_AVAIL_INV_LEN+1);
    memcpy(ptr_recvbuf->item_avail_inv, sz_recvbufcat + gbp->count, tt0001_AVAIL_INV_LEN);
        ap_rprintf(r,"		<PROD_AVAIL_INV>%s</PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->item_avail_inv));
    gbp->count += tt0001_AVAIL_INV_LEN;

    memset(ptr_recvbuf->item_po_date,'\0', tt0001_PO_LEN+1);
    memcpy(ptr_recvbuf->item_po_date, sz_recvbufcat + gbp->count, tt0001_PO_LEN);
        ap_rprintf(r,"		<PROD_PO_DATE>%s</PROD_PO_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->item_po_date));
    gbp->count += tt0001_PO_LEN;

    memset(ptr_recvbuf->upsell_info,'\0', tt0001_UPS_INFO_LEN+1);
    memcpy(ptr_recvbuf->upsell_info, sz_recvbufcat + gbp->count, tt0001_UPS_INFO_LEN);
        ap_rprintf(r,"		<PROD_UPSELL>%s</PROD_UPSELL>\n", handle_special_chars(gbp,ptr_recvbuf->upsell_info));
    gbp->count += tt0001_UPS_INFO_LEN;

    memset(ptr_recvbuf->mpb_method,'\0', tt0001_MPB_METH_LEN+1);
    memcpy(ptr_recvbuf->mpb_method, sz_recvbufcat + gbp->count, tt0001_MPB_METH_LEN);
        ap_rprintf(r,"		<MULTIPRICING_METHOD>%s</MULTIPRICING_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->mpb_method));
    gbp->count += tt0001_MPB_METH_LEN;

    memset(ptr_recvbuf->mpb_upsell,'\0', tt0001_MPB_UPS_LEN+1);
    memcpy(ptr_recvbuf->mpb_upsell, sz_recvbufcat + gbp->count, tt0001_MPB_UPS_LEN);
        ap_rprintf(r,"		<MULTIPRICE_PROD_UPSELL>%s</MULTIPRICE_PROD_UPSELL>\n", handle_special_chars(gbp,ptr_recvbuf->mpb_upsell));
    gbp->count += tt0001_MPB_UPS_LEN;

    for(gbp->i = 0; gbp->i < tt0001_MPB_COUNT; gbp->i++) 
        {
        memset(ptr_recvbuf->mpb_qty[gbp->i].mpb_qty,'\0', tt0001_MPB_QTY_LEN+1);
        memcpy(ptr_recvbuf->mpb_qty[gbp->i].mpb_qty, sz_recvbufcat + gbp->count, tt0001_MPB_QTY_LEN);
        gbp->count += tt0001_MPB_QTY_LEN;

        // Get rid of the comma, if it exists
/*      temp1 = ptr_recvbuf->mpb_qty[i].mpb_qty[0];
        temp2 = ' ';
        ptr_recvbuf->mpb_qty[i].mpb_qty[1] = temp1;
        ptr_recvbuf->mpb_qty[i].mpb_qty[0] = ' ';   */

                if((atoi(ptr_recvbuf->mpb_qty[gbp->i].mpb_qty)) > 0)
                {
                        ap_rprintf(r,"		<QTY_BREAKS>\n");
                        ap_rprintf(r,"			<QTY>%s</QTY>\n", handle_special_chars(gbp,ptr_recvbuf->mpb_qty[gbp->i].mpb_qty));
                        ap_rprintf(r,"		</QTY_BREAKS>\n");
                }
    }

    for(gbp->i = 0; gbp->i < tt0001_MPB_COUNT; gbp->i++) 
        {
        memset(ptr_recvbuf->mpb_price[gbp->i].mpb_price,'\0', tt0001_MPB_PRICE_LEN+1);
        memcpy(ptr_recvbuf->mpb_price[gbp->i].mpb_price, sz_recvbufcat + gbp->count, tt0001_MPB_PRICE_LEN);
                gbp->count += tt0001_MPB_PRICE_LEN;

                if((atof(ptr_recvbuf->mpb_price[gbp->i].mpb_price)) > 0)
                {
                        ap_rprintf(r,"		<PRICE_FOR_QTY_BREAKS>\n");
                        ap_rprintf(r,"			<MULTIPRICE_PROD_PRICE>%s</MULTIPRICE_PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->mpb_price[gbp->i].mpb_price));
                        ap_rprintf(r,"		</PRICE_FOR_QTY_BREAKS>\n");
                }
    }


    memset(ptr_recvbuf->customization_flag,'\0', tt0001_CUST_FLAG_LEN+1);
    memcpy(ptr_recvbuf->customization_flag, sz_recvbufcat + gbp->count, tt0001_CUST_FLAG_LEN);
        ap_rprintf(r,"		<CUSTOMIZATION_FLAG>%s</CUSTOMIZATION_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->customization_flag));
    gbp->count += tt0001_CUST_FLAG_LEN;

    memset(ptr_recvbuf->upsell_a,'\0', tt0001_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_a, sz_recvbufcat + gbp->count, tt0001_UPS_REQ_LEN);
        ap_rprintf(r,"		<ADDITIONAL_UPSELL_FIRST_FIELD>%s</ADDITIONAL_UPSELL_FIRST_FIELD>\n", handle_special_chars(gbp,ptr_recvbuf->upsell_a));
    gbp->count += tt0001_UPS_REQ_LEN;

    memset(ptr_recvbuf->upsell_b,'\0', tt0001_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_b, sz_recvbufcat + gbp->count, tt0001_UPS_REQ_LEN);
        ap_rprintf(r,"		<ADDITIONAL_UPSELL_SECOND_FIELD>%s</ADDITIONAL_UPSELL_SECOND_FIELD>\n", handle_special_chars(gbp,ptr_recvbuf->upsell_b));
    gbp->count += tt0001_UPS_REQ_LEN;

    memset(ptr_recvbuf->upsell_c,'\0', tt0001_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_c, sz_recvbufcat + gbp->count, tt0001_UPS_REQ_LEN);
        ap_rprintf(r,"		<ADDITIONAL_UPSELL_THIRD_FIELD>%s</ADDITIONAL_UPSELL_THIRD_FIELD>\n", handle_special_chars(gbp,ptr_recvbuf->upsell_c));
    gbp->count += tt0001_UPS_REQ_LEN;

    memset(ptr_recvbuf->upsell_d,'\0', tt0001_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_d, sz_recvbufcat + gbp->count, tt0001_UPS_REQ_LEN);
        ap_rprintf(r,"		<ADDITIONAL_UPSELL_FOURTH_FIELD>%s</ADDITIONAL_UPSELL_FOURTH_FIELD>\n", handle_special_chars(gbp,ptr_recvbuf->upsell_d));
    gbp->count += tt0001_UPS_REQ_LEN;

    memset(ptr_recvbuf->image_ovrd,'\0', tt0001_IMG_OVRD_LEN+1);
    memcpy(ptr_recvbuf->image_ovrd, sz_recvbufcat + gbp->count, tt0001_IMG_OVRD_LEN);
        ap_rprintf(r,"		<IMAGE_OVERRIDE>%s</IMAGE_OVERRIDE>\n", handle_special_chars(gbp,ptr_recvbuf->image_ovrd));
    gbp->count += tt0001_IMG_OVRD_LEN;


    memset(ptr_recvbuf->style_id,'\0', tt0001_STYLE_ID_LEN+1);
    memcpy(ptr_recvbuf->style_id, sz_recvbufcat + gbp->count, tt0001_STYLE_ID_LEN);
        ap_rprintf(r,"		<STYLE_HEADER>%s</STYLE_HEADER>\n", handle_special_chars(gbp,ptr_recvbuf->style_id));
    gbp->count += tt0001_STYLE_ID_LEN;

    memset(ptr_recvbuf->l1_type,'\0', tt0001_L1_TYPE_LEN+1);
    memcpy(ptr_recvbuf->l1_type, sz_recvbufcat + gbp->count, tt0001_L1_TYPE_LEN);
        ap_rprintf(r,"		<FIRST_LEVEL_TYPE>%s</FIRST_LEVEL_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->l1_type));
    gbp->count += tt0001_L1_TYPE_LEN;

    memset(ptr_recvbuf->l1_name,'\0', tt0001_L1_NAME_LEN+1);
    memcpy(ptr_recvbuf->l1_name, sz_recvbufcat + gbp->count, tt0001_L1_NAME_LEN);
        ap_rprintf(r,"		<FIRST_LEVEL_STYLE>%s</FIRST_LEVEL_STYLE>\n", handle_special_chars(gbp,ptr_recvbuf->l1_name));
    gbp->count += tt0001_L1_NAME_LEN;

    memset(ptr_recvbuf->l2_type,'\0', tt0001_L2_TYPE_LEN+1);
    memcpy(ptr_recvbuf->l2_type, sz_recvbufcat + gbp->count, tt0001_L2_TYPE_LEN);
        ap_rprintf(r,"		<SECOND_LEVEL_TYPE>%s</SECOND_LEVEL_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->l2_type));
    gbp->count += tt0001_L2_TYPE_LEN;

    memset(ptr_recvbuf->l2_name,'\0', tt0001_L2_NAME_LEN+1);
    memcpy(ptr_recvbuf->l2_name, sz_recvbufcat + gbp->count, tt0001_L2_NAME_LEN);
        ap_rprintf(r,"		<SECOND_LEVEL_STYLE>%s</SECOND_LEVEL_STYLE>\n", handle_special_chars(gbp,ptr_recvbuf->l2_name));
    gbp->count += tt0001_L2_NAME_LEN;

    memset(ptr_recvbuf->l3_type,'\0', tt0001_L3_TYPE_LEN+1);
    memcpy(ptr_recvbuf->l3_type, sz_recvbufcat + gbp->count, tt0001_L3_TYPE_LEN);
        ap_rprintf(r,"		<THIRD_LEVEL_TYPE>%s</THIRD_LEVEL_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->l3_type));
    gbp->count += tt0001_L3_TYPE_LEN;

    memset(ptr_recvbuf->l3_name,'\0', tt0001_L3_NAME_LEN+1);
    memcpy(ptr_recvbuf->l3_name, sz_recvbufcat + gbp->count, tt0001_L3_NAME_LEN);
        ap_rprintf(r,"		<THIRD_LEVEL_STYLE>%s</THIRD_LEVEL_STYLE>\n", handle_special_chars(gbp,ptr_recvbuf->l3_name));
    gbp->count += tt0001_L3_NAME_LEN;

    // Build data from array

    for(gbp->i = 0; gbp->i < tt0001_STYLE_COUNT; gbp->i++) 
        {
        memset(ptr_recvbuf->style[gbp->i].l1_entry,'\0', tt0001_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->style[gbp->i].l1_entry, sz_recvbufcat + gbp->count, tt0001_STYLE_DESC_LEN);
        gbp->count += tt0001_STYLE_DESC_LEN;
                sprintf(ptr_recvbuf->style[gbp->i].l1_entry,"%s",rtrim(ptr_recvbuf->style[gbp->i].l1_entry, tt0001_STYLE_DESC_LEN+1,gbp));

                memset(ptr_recvbuf->style[gbp->i].l2_entry,'\0', tt0001_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->style[gbp->i].l2_entry, sz_recvbufcat + gbp->count, tt0001_STYLE_DESC_LEN);
        gbp->count += tt0001_STYLE_DESC_LEN;
                sprintf(ptr_recvbuf->style[gbp->i].l2_entry,"%s",rtrim(ptr_recvbuf->style[gbp->i].l2_entry, tt0001_STYLE_DESC_LEN+1,gbp));

        memset(ptr_recvbuf->style[gbp->i].l3_entry,'\0', tt0001_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->style[gbp->i].l3_entry, sz_recvbufcat + gbp->count, tt0001_STYLE_DESC_LEN);
        gbp->count += tt0001_STYLE_DESC_LEN;
                sprintf(ptr_recvbuf->style[gbp->i].l3_entry,"%s",rtrim(ptr_recvbuf->style[gbp->i].l3_entry, tt0001_STYLE_DESC_LEN+1,gbp));

                if(((strlen(ptr_recvbuf->style[gbp->i].l1_entry)) > 0) ||
                   ((strlen(ptr_recvbuf->style[gbp->i].l2_entry)) > 0) ||
           ((strlen(ptr_recvbuf->style[gbp->i].l3_entry)) > 0))  
                
                {
                        ap_rprintf(r,"		<STYLE_DETAIL_DATA>\n");
                        ap_rprintf(r,"			<STYLE_DESCRIPTIONS_FIRST_LEVEL>%s</STYLE_DESCRIPTIONS_FIRST_LEVEL>\n", handle_special_chars(gbp,ptr_recvbuf->style[gbp->i].l1_entry));
                        ap_rprintf(r,"			<STYLE_DESCRIPTIONS_SECOND_LEVEL>%s</STYLE_DESCRIPTIONS_SECOND_LEVEL>\n", handle_special_chars(gbp,ptr_recvbuf->style[gbp->i].l2_entry));
                        ap_rprintf(r,"			<STYLE_DESCRIPTIONS_THIRD_LEVEL>%s</STYLE_DESCRIPTIONS_THIRD_LEVEL>\n", handle_special_chars(gbp,ptr_recvbuf->style[gbp->i].l3_entry));
                        ap_rprintf(r,"		</STYLE_DETAIL_DATA>\n");
                }
    }


        memset(ptr_recvbuf->tvs_source,'\0', tt0001_TVS_SRC_LEN + 1);
    memcpy(ptr_recvbuf->tvs_source, sz_recvbufcat + gbp->count, tt0001_TVS_SRC_LEN);
        ap_rprintf(r,"		<TVS_SOURCE>%s</TVS_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->tvs_source));
    gbp->count += tt0001_TVS_SRC_LEN;

        memset(ptr_recvbuf->item_master_price,'\0', tt0001_MASTER_PRICE_LEN + 1);
    memcpy(ptr_recvbuf->item_master_price, sz_recvbufcat + gbp->count, tt0001_MASTER_PRICE_LEN);
        ap_rprintf(r,"		<PROD_MASTER_PRICE>%s</PROD_MASTER_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->item_master_price));
    gbp->count += tt0001_MASTER_PRICE_LEN;

        for(gbp->i = 0; gbp->i < tt0001_PROMPT_COUNT; gbp->i++) 
        {
        memset(ptr_recvbuf->prompt[gbp->i].prmt_item_edp,'\0', tt0001_PROM_ITM_EDP_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_edp, sz_recvbufcat + gbp->count, tt0001_PROM_ITM_EDP_LEN);
        gbp->count += tt0001_PROM_ITM_EDP_LEN;
                
                memset(ptr_recvbuf->prompt[gbp->i].prmt_item_num,'\0', tt0001_PROM_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_num, sz_recvbufcat + gbp->count, tt0001_PROM_ITM_NO_LEN);
        gbp->count += tt0001_PROM_ITM_NO_LEN;
        
                memset(ptr_recvbuf->prompt[gbp->i].prmt_item_desc,'\0', tt0001_PROM_ITM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_desc, sz_recvbufcat + gbp->count, tt0001_PROM_ITM_DESC_LEN);
        gbp->count += tt0001_PROM_ITM_DESC_LEN;

                memset(ptr_recvbuf->prompt[gbp->i].prmt_item_price,'\0', tt0001_PROM_ITM_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_price, sz_recvbufcat + gbp->count, tt0001_PROM_ITM_PRICE_LEN);
        gbp->count += tt0001_PROM_ITM_PRICE_LEN;

                memset(ptr_recvbuf->prompt[gbp->i].prmt_item_avail_inv,'\0', tt0001_PROM_ITM_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_avail_inv, sz_recvbufcat + gbp->count, tt0001_PROM_ITM_AVAIL_INV_LEN);
        gbp->count += tt0001_PROM_ITM_AVAIL_INV_LEN;

                if((atoi(ptr_recvbuf->prompt[gbp->i].prmt_item_edp)) > 0) 
                {
                        ap_rprintf(r,"		<PROMPT_DETAIL_DATA>\n");
                        ap_rprintf(r,"			<PROMPT_PROD_EDP>%s</PROMPT_PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->prompt[gbp->i].prmt_item_edp));
                        ap_rprintf(r,"			<PROMPT_PROD_NUM>%s</PROMPT_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->prompt[gbp->i].prmt_item_num));
                        ap_rprintf(r,"			<PROMPT_PROD_DESCRIPTION>%s</PROMPT_PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->prompt[gbp->i].prmt_item_desc));
                        ap_rprintf(r,"			<PROMPT_PROD_PRICE>%s</PROMPT_PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->prompt[gbp->i].prmt_item_price));
                        ap_rprintf(r,"			<PROMPT_PROD_AVAIL_INV>%s</PROMPT_PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->prompt[gbp->i].prmt_item_avail_inv));
                        ap_rprintf(r,"		</PROMPT_DETAIL_DATA>\n");
                }
        }
    memset(ptr_recvbuf->gwrap_flag,'\0', tt0001_GWRAP_FLAG_LEN+1);
    memcpy(ptr_recvbuf->gwrap_flag, sz_recvbufcat + gbp->count, tt0001_GWRAP_FLAG_LEN);
        ap_rprintf(r,"		<GWRAP_FLAG>%s</GWRAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_flag));
    gbp->count += tt0001_GWRAP_FLAG_LEN;

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0002>\n", gbp->tt_betag);


    return(0);
}

