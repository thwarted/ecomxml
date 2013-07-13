/* tt0105.c  Get Giftwrap */

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


int		tt0105_CatSendStr(struct global_struct *,char *, tt0105_st_send *);
int		tt0105_ParceRecvStr(tt0105_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0105_lt_process(request_rec *, struct global_struct *,char *);


int tt0105_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)			/* Get the current working path */
		return(-1);

    GetInf(gbp);							/* Read info from INF file */

	return(0);
}

int tt0105_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0105_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}

/******************************************************************/
/*                                                                */
/*  Process the request                                           */
/*                                                                */
/******************************************************************/


int tt0105_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0105 = (tt0105_st_send *)malloc(sizeof(tt0105_st_send));
	gbp->recvbuf0105 = (tt0105_st_recv *)malloc(sizeof(tt0105_st_recv));

	gbp->sendbufcat = malloc(tt0105_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0105_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0105_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0105_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0105,  '\0',  sizeof(tt0105_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0105->request_id, "XML");
    strcpy(gbp->sendbuf0105->record_id, "0105");

	strcpy(gbp->sendbuf0105->ip_address, r->connection->remote_ip);

	get_tag_data("COMPANY", gbp->sendbuf0105->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0105->division,gbp,stdout_buffer);
	get_tag_data("UID",  gbp->sendbuf0105->userid,gbp,stdout_buffer);
	get_tag_data("GWRAP_FLAG",  gbp->sendbuf0105->gwrap_flag,gbp,stdout_buffer);
	get_tag_data("PROD_NUM",  gbp->sendbuf0105->prod_num,gbp,stdout_buffer);
	get_tag_data("SHOP_CART_PAGE_NUM",  gbp->sendbuf0105->shop_cart_page_num,gbp,stdout_buffer);
	get_tag_data("SHOP_CART_LINE_NUM",  gbp->tagname,gbp,stdout_buffer);

    gbp->i = atoi(gbp->tagname);
    sprintf(gbp->sendbuf0105->shop_cart_line_num, "%03d", gbp->i);

    
	if((tt0105_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0105)) == tt0105_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0105);
			free (gbp->recvbuf0105);
			return(-1);
		}

	
		if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0105_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0105);
			free (gbp->recvbuf0105);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0105_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0105);
			free (gbp->recvbuf0105);
			return(-1);
		}


		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0105);
			free (gbp->recvbuf0105);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0105_ParceRecvStr(gbp->recvbuf0105, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0105);
		free (gbp->recvbuf0105);
	}
	else
	{
		XML_Error("tt0105_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0105);
		free (gbp->recvbuf0105);
		return(-1);
	}

    return(0);
}


int tt0105_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0105_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-20.20s%-2.2s%-3.3s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
            ptr_sendbuf->gwrap_flag,
            ptr_sendbuf->prod_num,
            ptr_sendbuf->shop_cart_page_num,
			ptr_sendbuf->shop_cart_line_num);

    return(gbp->j);
}


int tt0105_ParceRecvStr(tt0105_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
    gbp->count = 0;
    // Place the individual variables into target fields

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0106.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0106 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0106.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0106 %s\"tt0106\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);

	memset(ptr_recvbuf->request_id, '\0', tt0105_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0105_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0105_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0105_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0105_REC_ID_LEN );
    gbp->count += tt0105_REC_ID_LEN;

	memset(ptr_recvbuf->userid, '\0', tt0105_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0105_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0105_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0105_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0105_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0105_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0105_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0105_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0105_ERR_LEN;

	gbp->count += tt0105_SEND_FILLER_LEN;


    memset(ptr_recvbuf->gwrap_flag, '\0', tt0105_GWRAP_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_flag, sz_recvbufcat + gbp->count, tt0105_GWRAP_FLAG_LEN);
	ap_rprintf(r,"		<GWRAP_FLAG>%s</GWRAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_flag));
    gbp->count += tt0105_GWRAP_FLAG_LEN;

    memset(ptr_recvbuf->prod_num, '\0', tt0105_PROD_NUM_LEN + 1);
    memcpy(ptr_recvbuf->prod_num, sz_recvbufcat + gbp->count, tt0105_PROD_NUM_LEN);
	ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->prod_num));
    gbp->count += tt0105_PROD_NUM_LEN;

    memset(ptr_recvbuf->prod_desc, '\0', tt0105_PROD_DESC_LEN + 1);
    memcpy(ptr_recvbuf->prod_desc, sz_recvbufcat + gbp->count, tt0105_PROD_DESC_LEN);
	ap_rprintf(r,"		<PROD_DESC>%s</PROD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->prod_desc));
    gbp->count += tt0105_PROD_DESC_LEN;

    memset(ptr_recvbuf->shop_cart_page_num, '\0', tt0105_SHOP_CART_PAGE_NUM_LEN + 1);
    memcpy(ptr_recvbuf->shop_cart_page_num, sz_recvbufcat + gbp->count, tt0105_SHOP_CART_PAGE_NUM_LEN);
	ap_rprintf(r,"		<SHOP_CART_PAGE_NUM>%s</SHOP_CART_PAGE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shop_cart_page_num));
    gbp->count += tt0105_SHOP_CART_PAGE_NUM_LEN;

    memset(ptr_recvbuf->shop_cart_line_num, '\0', tt0105_SHOP_CART_LINE_NUM_LEN + 1);
    memcpy(ptr_recvbuf->shop_cart_line_num, sz_recvbufcat + gbp->count, tt0105_SHOP_CART_LINE_NUM_LEN);
	ap_rprintf(r,"		<SHOP_CART_LINE_NUM>%s</SHOP_CART_LINE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_flag));
    gbp->count += tt0105_SHOP_CART_LINE_NUM_LEN;

    memset(ptr_recvbuf->prod_qty, '\0', tt0105_PROD_QTY_LEN + 1);
    memcpy(ptr_recvbuf->prod_qty, sz_recvbufcat + gbp->count, tt0105_PROD_QTY_LEN);
	ap_rprintf(r,"		<PROD_QTY>%s</PROD_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->prod_qty));
    gbp->count += tt0105_PROD_QTY_LEN;

    memset(ptr_recvbuf->gwrap_edp_no, '\0', tt0105_GWRAP_EDP_NO_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_edp_no, sz_recvbufcat + gbp->count, tt0105_GWRAP_FLAG_LEN);
	ap_rprintf(r,"		<GWRAP_EDP_NO>%s</GWRAP_EDP_NO>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_edp_no));
    gbp->count += tt0105_GWRAP_EDP_NO_LEN;

    memset(ptr_recvbuf->gwrap_item_no, '\0', tt0105_GWRAP_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_item_no, sz_recvbufcat + gbp->count, tt0105_GWRAP_ITEM_NO_LEN);
	ap_rprintf(r,"		<GWRAP_ITEM_NO>%s</GWRAP_ITEM_NO>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_item_no));
    gbp->count += tt0105_GWRAP_ITEM_NO_LEN;

    memset(ptr_recvbuf->gwrap_desc, '\0', tt0105_GWRAP_DESC_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_desc, sz_recvbufcat + gbp->count, tt0105_GWRAP_DESC_LEN);
	ap_rprintf(r,"		<GWRAP_DESC>%s</GWRAP_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_desc));
    gbp->count += tt0105_GWRAP_DESC_LEN;

    memset(ptr_recvbuf->gwrap_price, '\0', tt0105_GWRAP_PRICE_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_price, sz_recvbufcat + gbp->count, tt0105_GWRAP_PRICE_LEN);
	ap_rprintf(r,"		<GWRAP_PRICE>%s</GWRAP_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_price));
    gbp->count += tt0105_GWRAP_PRICE_LEN;

    memset(ptr_recvbuf->gwrap_avail_inv, '\0', tt0105_GWRAP_AVAIL_INV_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_avail_inv, sz_recvbufcat + gbp->count, tt0105_GWRAP_AVAIL_INV_LEN);
	ap_rprintf(r,"		<GWRAP_AVAIL_INV>%s</GWRAP_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_avail_inv));
    gbp->count += tt0105_GWRAP_AVAIL_INV_LEN;

    memset(ptr_recvbuf->gwrap_custom_flag, '\0', tt0105_GWRAP_CUSTOM_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_custom_flag, sz_recvbufcat + gbp->count, tt0105_GWRAP_CUSTOM_FLAG_LEN);
	ap_rprintf(r,"		<GWRAP_CUSTOM_FLAG>%s</GWRAP_CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_custom_flag));
    gbp->count += tt0105_GWRAP_CUSTOM_FLAG_LEN;

    memset(ptr_recvbuf->gwrap_image_override, '\0', tt0105_GWRAP_IMAGE_OVERRIDE_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_image_override, sz_recvbufcat + gbp->count, tt0105_GWRAP_IMAGE_OVERRIDE_LEN);
	ap_rprintf(r,"		<GWRAP_IMAGE_OVERRIDE>%s</GWRAP_IMAGE_OVERRIDE>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_image_override));
    gbp->count += tt0105_GWRAP_IMAGE_OVERRIDE_LEN;

    memset(ptr_recvbuf->gwrap_custom_last_no, '\0', tt0105_GWRAP_CUSTOM_LAST_NO_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_custom_last_no, sz_recvbufcat + gbp->count, tt0105_GWRAP_CUSTOM_LAST_NO_LEN);
	ap_rprintf(r,"		<GWRAP_CUSTOM_LAST_NO>%s</GWRAP_CUSTOM_LAST_NO>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_custom_last_no));
    gbp->count += tt0105_GWRAP_CUSTOM_LAST_NO_LEN;


    // Build data from array

    for(gbp->i = 0; gbp->i < tt0105_CUSTOM_DATA_COUNT; gbp->i++) 
	{
    memset(ptr_recvbuf->c_array[gbp->i].gwrap_customiz_lines, '\0', tt0105_GWRAP_CUSTOMIZ_LINES_LEN + 1);
    memcpy(ptr_recvbuf->c_array[gbp->i].gwrap_customiz_lines, sz_recvbufcat + gbp->count, tt0105_GWRAP_CUSTOMIZ_LINES_LEN);
	if (ptr_recvbuf->c_array[gbp->i].gwrap_customiz_lines[0] != ' '){

		ap_rprintf(r,"		<CUSTOMIZATION_DATA>\n");
		ap_rprintf(r,"		    <GWRAP_CUSTOMIZ_LINES>%s</GWRAP_CUSTOMIZ_LINES>\n", handle_special_chars(gbp,ptr_recvbuf->c_array[gbp->i].gwrap_customiz_lines));
		ap_rprintf(r,"		</CUSTOMIZATION_DATA>\n");

	}
    gbp->count += tt0105_GWRAP_CUSTOMIZ_LINES_LEN;

    }

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0106>\n", gbp->tt_betag);

    return 0;
}
