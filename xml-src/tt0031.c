/* tt0031.c Delete Item */
/* modified for phase 2 - JL */
#ifdef WIN0032
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


int		tt0031_CatSendStr(struct global_struct *, char *, tt0031_st_send *);
int		tt0031_ParceRecvStr(tt0031_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0031_lt_process(request_rec *, struct global_struct *, char *);


int tt0031_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0031_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0031_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0031_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0031 = (tt0031_st_send *)malloc(sizeof(tt0031_st_send));
	gbp->recvbuf0031 = (tt0031_st_recv *)malloc(sizeof(tt0031_st_recv));

	gbp->sendbufcat = malloc(tt0031_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0031_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}  

    memset(gbp->sendbufcat, '\0', tt0031_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0031_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0031,  '\0',  sizeof(tt0031_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0031->request_id, "XML");
    strcpy(gbp->sendbuf0031->record_id, "0031");

	strcpy(gbp->sendbuf0031->ip_address, r->connection->remote_ip);
	get_tag_data("COMPANY", gbp->sendbuf0031->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0031->division,gbp,stdout_buffer);
	get_tag_data("UID", gbp->sendbuf0031->userid,gbp,stdout_buffer);
	get_tag_data("CART_LINE_NUM", gbp->sendbuf0031->cart_line_num,gbp,stdout_buffer);
	gbp->sendbuf0031->cart_flag[0]='D';
	
	if(tt0031_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0031) == tt0031_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0031);
			free (gbp->recvbuf0031);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0031_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0031);
			free (gbp->recvbuf0031);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0031_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0031);
			free (gbp->recvbuf0031);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0031);
			free (gbp->recvbuf0031);
			return(-1);
		}
		
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0031_ParceRecvStr(gbp->recvbuf0031, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0031);
		free (gbp->recvbuf0031);

	}
	else
	{
		XML_Error("tt0031_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0031);
		free (gbp->recvbuf0031);
		return(-1);
	}

    return(0);
}


int tt0031_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0031_st_send *ptr_sendbuf)
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


int tt0031_ParceRecvStr(tt0031_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{

/*  char temp1;
    char temp2; */
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt02.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0032 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0032.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0032 %s\"tt0032\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0031_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0031_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0031_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0031_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0031_REC_ID_LEN );
    gbp->count += tt0031_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0031_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0031_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0031_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0031_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0031_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0031_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0031_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0031_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0031_ERR_LEN;

	gbp->count += tt0031_SEND_FILLER_LEN;


    memset(ptr_recvbuf->item_edp,'\0', tt0031_EDP_LEN+1);
    memcpy(ptr_recvbuf->item_edp, sz_recvbufcat + gbp->count, tt0031_EDP_LEN);
    gbp->count += tt0031_EDP_LEN;

    memset(ptr_recvbuf->item_num,'\0', tt0031_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0031_ITM_NO_LEN);
    gbp->count += tt0031_ITM_NO_LEN;

    memset(ptr_recvbuf->item_desc,'\0', tt0031_DESC_LEN+1);
    memcpy(ptr_recvbuf->item_desc, sz_recvbufcat + gbp->count, tt0031_DESC_LEN);
    gbp->count += tt0031_DESC_LEN;

    memset(ptr_recvbuf->item_price,'\0', tt0031_PRICE_LEN+1);
    memcpy(ptr_recvbuf->item_price, sz_recvbufcat + gbp->count, tt0031_PRICE_LEN);
    gbp->count += tt0031_PRICE_LEN;

    memset(ptr_recvbuf->item_avail_flag,'\0', tt0031_AVAIL_FLAG_LEN+1);
    memcpy(ptr_recvbuf->item_avail_flag, sz_recvbufcat + gbp->count, tt0031_AVAIL_FLAG_LEN);
    gbp->count += tt0031_AVAIL_FLAG_LEN;

    memset(ptr_recvbuf->item_avail_inv,'\0', tt0031_AVAIL_INV_LEN+1);
    memcpy(ptr_recvbuf->item_avail_inv, sz_recvbufcat + gbp->count, tt0031_AVAIL_INV_LEN);
    gbp->count += tt0031_AVAIL_INV_LEN;

    memset(ptr_recvbuf->item_po_date,'\0', tt0031_PO_LEN+1);
    memcpy(ptr_recvbuf->item_po_date, sz_recvbufcat + gbp->count, tt0031_PO_LEN);
    gbp->count += tt0031_PO_LEN;

    memset(ptr_recvbuf->upsell_info,'\0', tt0031_UPS_INFO_LEN+1);
    memcpy(ptr_recvbuf->upsell_info, sz_recvbufcat + gbp->count, tt0031_UPS_INFO_LEN);
    gbp->count += tt0031_UPS_INFO_LEN;

    memset(ptr_recvbuf->mpb_method,'\0', tt0031_MPB_METH_LEN+1);
    memcpy(ptr_recvbuf->mpb_method, sz_recvbufcat + gbp->count, tt0031_MPB_METH_LEN);
    gbp->count += tt0031_MPB_METH_LEN;

    memset(ptr_recvbuf->mpb_upsell,'\0', tt0031_MPB_UPS_LEN+1);
    memcpy(ptr_recvbuf->mpb_upsell, sz_recvbufcat + gbp->count, tt0031_MPB_UPS_LEN);
    gbp->count += tt0031_MPB_UPS_LEN;

    for(gbp->i = 0; gbp->i < tt0031_MPB_COUNT; gbp->i++) 
	{
        memset(ptr_recvbuf->mpb_qty[gbp->i].mpb_qty,'\0', tt0031_MPB_QTY_LEN+1);
        memcpy(ptr_recvbuf->mpb_qty[gbp->i].mpb_qty, sz_recvbufcat + gbp->count, tt0031_MPB_QTY_LEN);
        gbp->count += tt0031_MPB_QTY_LEN;
    }

    for(gbp->i = 0; gbp->i < tt0031_MPB_COUNT; gbp->i++) 
	{
        memset(ptr_recvbuf->mpb_price[gbp->i].mpb_price,'\0', tt0031_MPB_PRICE_LEN+1);
        memcpy(ptr_recvbuf->mpb_price[gbp->i].mpb_price, sz_recvbufcat + gbp->count, tt0031_MPB_PRICE_LEN);
		gbp->count += tt0031_MPB_PRICE_LEN;
    }


    memset(ptr_recvbuf->customization_flag,'\0', tt0031_CUST_FLAG_LEN+1);
    memcpy(ptr_recvbuf->customization_flag, sz_recvbufcat + gbp->count, tt0031_CUST_FLAG_LEN);
    gbp->count += tt0031_CUST_FLAG_LEN;

    memset(ptr_recvbuf->upsell_a,'\0', tt0031_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_a, sz_recvbufcat + gbp->count, tt0031_UPS_REQ_LEN);
    gbp->count += tt0031_UPS_REQ_LEN;

    memset(ptr_recvbuf->upsell_b,'\0', tt0031_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_b, sz_recvbufcat + gbp->count, tt0031_UPS_REQ_LEN);
    gbp->count += tt0031_UPS_REQ_LEN;

    memset(ptr_recvbuf->upsell_c,'\0', tt0031_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_c, sz_recvbufcat + gbp->count, tt0031_UPS_REQ_LEN);
    gbp->count += tt0031_UPS_REQ_LEN;

    memset(ptr_recvbuf->upsell_d,'\0', tt0031_UPS_REQ_LEN+1);
    memcpy(ptr_recvbuf->upsell_d, sz_recvbufcat + gbp->count, tt0031_UPS_REQ_LEN);
    gbp->count += tt0031_UPS_REQ_LEN;

    memset(ptr_recvbuf->image_ovrd,'\0', tt0031_IMG_OVRD_LEN+1);
    memcpy(ptr_recvbuf->image_ovrd, sz_recvbufcat + gbp->count, tt0031_IMG_OVRD_LEN);
    gbp->count += tt0031_IMG_OVRD_LEN;


    memset(ptr_recvbuf->style_id,'\0', tt0031_STYLE_ID_LEN+1);
    memcpy(ptr_recvbuf->style_id, sz_recvbufcat + gbp->count, tt0031_STYLE_ID_LEN);
    gbp->count += tt0031_STYLE_ID_LEN;

    memset(ptr_recvbuf->l1_type,'\0', tt0031_L1_TYPE_LEN+1);
    memcpy(ptr_recvbuf->l1_type, sz_recvbufcat + gbp->count, tt0031_L1_TYPE_LEN);
    gbp->count += tt0031_L1_TYPE_LEN;

    memset(ptr_recvbuf->l1_name,'\0', tt0031_L1_NAME_LEN+1);
    memcpy(ptr_recvbuf->l1_name, sz_recvbufcat + gbp->count, tt0031_L1_NAME_LEN);
    gbp->count += tt0031_L1_NAME_LEN;

    memset(ptr_recvbuf->l2_type,'\0', tt0031_L2_TYPE_LEN+1);
    memcpy(ptr_recvbuf->l2_type, sz_recvbufcat + gbp->count, tt0031_L2_TYPE_LEN);
    gbp->count += tt0031_L2_TYPE_LEN;

    memset(ptr_recvbuf->l2_name,'\0', tt0031_L2_NAME_LEN+1);
    memcpy(ptr_recvbuf->l2_name, sz_recvbufcat + gbp->count, tt0031_L2_NAME_LEN);
    gbp->count += tt0031_L2_NAME_LEN;

    memset(ptr_recvbuf->l3_type,'\0', tt0031_L3_TYPE_LEN+1);
    memcpy(ptr_recvbuf->l3_type, sz_recvbufcat + gbp->count, tt0031_L3_TYPE_LEN);
    gbp->count += tt0031_L3_TYPE_LEN;

    memset(ptr_recvbuf->l3_name,'\0', tt0031_L3_NAME_LEN+1);
    memcpy(ptr_recvbuf->l3_name, sz_recvbufcat + gbp->count, tt0031_L3_NAME_LEN);
    gbp->count += tt0031_L3_NAME_LEN;

    // Build data from array

    for(gbp->i = 0; gbp->i < tt0031_STYLE_COUNT; gbp->i++) 
	{
        memset(ptr_recvbuf->style[gbp->i].l1_entry,'\0', tt0031_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->style[gbp->i].l1_entry, sz_recvbufcat + gbp->count, tt0031_STYLE_DESC_LEN);
        gbp->count += tt0031_STYLE_DESC_LEN;

		memset(ptr_recvbuf->style[gbp->i].l2_entry,'\0', tt0031_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->style[gbp->i].l2_entry, sz_recvbufcat + gbp->count, tt0031_STYLE_DESC_LEN);
        gbp->count += tt0031_STYLE_DESC_LEN;

        memset(ptr_recvbuf->style[gbp->i].l3_entry,'\0', tt0031_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->style[gbp->i].l3_entry, sz_recvbufcat + gbp->count, tt0031_STYLE_DESC_LEN);
        gbp->count += tt0031_STYLE_DESC_LEN;
    }


	memset(ptr_recvbuf->tvs_source,'\0', tt0031_TVS_SRC_LEN + 1);
    memcpy(ptr_recvbuf->tvs_source, sz_recvbufcat + gbp->count, tt0031_TVS_SRC_LEN);
    gbp->count += tt0031_TVS_SRC_LEN;

	memset(ptr_recvbuf->item_master_price,'\0', tt0031_MASTER_PRICE_LEN + 1);
    memcpy(ptr_recvbuf->item_master_price, sz_recvbufcat + gbp->count, tt0031_MASTER_PRICE_LEN);
    gbp->count += tt0031_MASTER_PRICE_LEN;

	for(gbp->i = 0; gbp->i < tt0031_PROMPT_COUNT; gbp->i++) 
	{
        memset(ptr_recvbuf->prompt[gbp->i].prmt_item_edp,'\0', tt0031_PROM_ITM_EDP_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_edp, sz_recvbufcat + gbp->count, tt0031_PROM_ITM_EDP_LEN);
        gbp->count += tt0031_PROM_ITM_EDP_LEN;
		
		memset(ptr_recvbuf->prompt[gbp->i].prmt_item_num,'\0', tt0031_PROM_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_num, sz_recvbufcat + gbp->count, tt0031_PROM_ITM_NO_LEN);
        gbp->count += tt0031_PROM_ITM_NO_LEN;
	
		memset(ptr_recvbuf->prompt[gbp->i].prmt_item_desc,'\0', tt0031_PROM_ITM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_desc, sz_recvbufcat + gbp->count, tt0031_PROM_ITM_DESC_LEN);
        gbp->count += tt0031_PROM_ITM_DESC_LEN;

		memset(ptr_recvbuf->prompt[gbp->i].prmt_item_price,'\0', tt0031_PROM_ITM_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_price, sz_recvbufcat + gbp->count, tt0031_PROM_ITM_PRICE_LEN);
        gbp->count += tt0031_PROM_ITM_PRICE_LEN;

		memset(ptr_recvbuf->prompt[gbp->i].prmt_item_avail_inv,'\0', tt0031_PROM_ITM_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->prompt[gbp->i].prmt_item_avail_inv, sz_recvbufcat + gbp->count, tt0031_PROM_ITM_AVAIL_INV_LEN);
        gbp->count += tt0031_PROM_ITM_AVAIL_INV_LEN;
	}

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0032>\n", gbp->tt_betag);


    return(0);
}

