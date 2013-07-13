/* wl0003.c   */
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


int             wl0003_CatSendStr(struct global_struct *, char *, wl0003_st_send *);
int             wl0003_ParceRecvStr(wl0003_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             wl0003_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int wl0003_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wl0003_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wl0003_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wl0003_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wlsendbuf0003 = (wl0003_st_send *)malloc(sizeof(wl0003_st_send));
        gbp->wlrecvbuf0003 = (wl0003_st_recv *)malloc(sizeof(wl0003_st_recv));

        gbp->sendbufcat = malloc(wl0003_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(wl0003_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', wl0003_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wl0003_LAN_RECV_BUF_LEN + 1);

    memset(gbp->wlsendbuf0003,  '\0',  sizeof(wl0003_st_send));

    sprintf(gbp->confirmbuf, "     ");

    get_tag_data("TRANSACTION_ID", gbp->wlsendbuf0003->record_id,gbp,stdout_buffer);
    get_tag_data("COMPANY", gbp->wlsendbuf0003->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->wlsendbuf0003->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->wlsendbuf0003->userid,gbp,stdout_buffer);
    get_tag_data("WL_COMPANY", gbp->wlsendbuf0003->wl_company,gbp,stdout_buffer);
    get_tag_data("WL_DIVISION", gbp->wlsendbuf0003->wl_division,gbp,stdout_buffer);
    get_tag_data("WL_ID", gbp->wlsendbuf0003->wl_id,gbp,stdout_buffer);
    get_tag_data("ITEM_NUM", gbp->wlsendbuf0003->item_no,gbp,stdout_buffer);
    get_tag_data("LINE_NUM", gbp->wlsendbuf0003->line_no,gbp,stdout_buffer);
    get_tag_data("CUSTOM_FLAG", gbp->wlsendbuf0003->custom_flag,gbp,stdout_buffer);
    get_tag_data("KIT_FLAG", gbp->wlsendbuf0003->kit_flag,gbp,stdout_buffer);

        if(wl0003_CatSendStr(gbp, gbp->sendbufcat, gbp->wlsendbuf0003) == wl0003_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->wlport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0003);
                        free (gbp->wlrecvbuf0003);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, wl0003_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0003);
                        free (gbp->wlrecvbuf0003);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wl0003_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0003);
                        free (gbp->wlrecvbuf0003);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0003);
                        free (gbp->wlrecvbuf0003);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                wl0003_ParceRecvStr(gbp->wlrecvbuf0003, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0003);
                free (gbp->wlrecvbuf0003);

        }
        else
        {
                XML_Error("wl0003_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0003);
                free (gbp->wlrecvbuf0003);
                return(-1);
        }

    return(0);
}


int wl0003_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, wl0003_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-2.2s%-2.2s%-16.16s%-2.2s%-2.2s%-16.16s%-10.10s"
			"%-20.20s%04d%-1.1s%-1.1s",

            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->wl_company,
            ptr_sendbuf->wl_division,
            ptr_sendbuf->wl_id,
            ptr_sendbuf->wl_filler,
            ptr_sendbuf->item_no,
            atoi(ptr_sendbuf->line_no),
            ptr_sendbuf->custom_flag,
            ptr_sendbuf->kit_flag);

    return(gbp->j);
}


int wl0003_ParceRecvStr(wl0003_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wl0003.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, wl_tag);
*/
        ap_rprintf(r,"%s0003 %s\"wl0003\">\n", gbp->wl_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->record_id,'\0', wl0003_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, wl0003_REC_ID_LEN );
    gbp->count += wl0003_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', wl0003_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, wl0003_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += wl0003_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', wl0003_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, wl0003_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += wl0003_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', wl0003_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, wl0003_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += wl0003_ERR_LEN;

    gbp->count += wl0003_RECV_FILLER_LEN;

    memset(ptr_recvbuf->cust_edp,'\0', wl0003_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, wl0003_CUST_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += wl0003_CUST_EDP_LEN;

    memset(ptr_recvbuf->item_count,'\0', wl0003_ITM_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->item_count, sz_recvbufcat + gbp->count, wl0003_ITM_COUNT_LEN);
        ap_rprintf(r,"		<ITEM_COUNT>%s</ITEM_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->item_count));
    gbp->count += wl0003_ITM_COUNT_LEN;

    memset(ptr_recvbuf->item_edp,'\0', wl0003_ITM_EDP_LEN + 1);
    memcpy(ptr_recvbuf->item_edp, sz_recvbufcat + gbp->count, wl0003_ITM_EDP_LEN);
        ap_rprintf(r,"		<ITEM_EDP>%s</ITEM_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item_edp));
    gbp->count += wl0003_ITM_EDP_LEN;

    memset(ptr_recvbuf->item_no,'\0', wl0003_ITM_NO_LEN + 1);
    memcpy(ptr_recvbuf->item_no, sz_recvbufcat + gbp->count, wl0003_ITM_NO_LEN);
        ap_rprintf(r,"		<ITEM_NUM>%s</ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_no));
    gbp->count += wl0003_ITM_NO_LEN;

    memset(ptr_recvbuf->qty,'\0', wl0003_QTY_LEN + 1);
    memcpy(ptr_recvbuf->qty, sz_recvbufcat + gbp->count, wl0003_QTY_LEN);
        ap_rprintf(r,"		<QTY>%s</QTY>\n", handle_special_chars(gbp,ptr_recvbuf->qty));
    gbp->count += wl0003_QTY_LEN;

    memset(ptr_recvbuf->desc,'\0', wl0003_DESC_LEN + 1);
    memcpy(ptr_recvbuf->desc, sz_recvbufcat + gbp->count, wl0003_DESC_LEN);
        ap_rprintf(r,"		<DESC>%s</DESC>\n", handle_special_chars(gbp,ptr_recvbuf->desc));
    gbp->count += wl0003_DESC_LEN;

    memset(ptr_recvbuf->price,'\0', wl0003_PRICE_LEN + 1);
    memcpy(ptr_recvbuf->price, sz_recvbufcat + gbp->count, wl0003_PRICE_LEN);
        ap_rprintf(r,"		<PRICE>%s</PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->price));
    gbp->count += wl0003_PRICE_LEN;

    memset(ptr_recvbuf->custom_flag,'\0', wl0003_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->custom_flag, sz_recvbufcat + gbp->count, wl0003_FLAG_LEN);
        ap_rprintf(r,"		<CUSTOM_FLAG>%s</CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->custom_flag));
    gbp->count += wl0003_FLAG_LEN;

	for( gbp->i = 0; gbp->i < wl0003_CUSTOM_LOOP; gbp->i++)
	{
		memset(ptr_recvbuf->customization[gbp->i].custom ,'\0', wl0003_CUSTOM_LEN + 1);
		memcpy(ptr_recvbuf->customization[gbp->i].custom, sz_recvbufcat + gbp->count, wl0003_CUSTOM_LEN);
		gbp->count += wl0003_CUSTOM_LEN;

		if (strlen(ptr_recvbuf->customization[gbp->i].custom) > 0 )
		{
			ap_rprintf(r,"          <CUSTOMIZATION_INFO>\n");
    		ap_rprintf(r,"                  <CUSTOMIZATION>%s</CUSTOMIZATION>\n", handle_special_chars(gbp,ptr_recvbuf->customization[gbp->i].custom));
			ap_rprintf(r,"          </CUSTOMIZATION_INFO>\n");
		}
	}

	for( gbp->i = 0; gbp->i < wl0003_R1_LOOP; gbp->i++)
	{
		memset(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_no ,'\0', wl0003_ITM_NO_LEN + 1);
		memcpy(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_no, sz_recvbufcat + gbp->count, wl0003_ITM_NO_LEN);
		gbp->count += wl0003_ITM_NO_LEN;

		memset(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_desc ,'\0', wl0003_DESC_LEN + 1);
		memcpy(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_desc, sz_recvbufcat + gbp->count, wl0003_DESC_LEN);
		gbp->count += wl0003_DESC_LEN;

		memset(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_qty ,'\0', wl0003_QTY_LEN + 1);
		memcpy(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_qty, sz_recvbufcat + gbp->count, wl0003_QTY_LEN);
		gbp->count += wl0003_QTY_LEN;

		if (strlen(ptr_recvbuf->r1_product_detail[gbp->i].r1_item_no) > 0 )
		{
			ap_rprintf(r,"          <R1_COMPONENT_INFO>\n");
    		ap_rprintf(r,"                  <R1_ITEM_NUM>%s</R1_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->r1_product_detail[gbp->i].r1_item_no));
    		ap_rprintf(r,"                  <R1_ITEM_DESC>%s</R1_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->r1_product_detail[gbp->i].r1_item_desc));
    		ap_rprintf(r,"                  <R1_ITEM_QTY>%s</R1_ITEM_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->r1_product_detail[gbp->i].r1_item_qty));
			ap_rprintf(r,"          </R1_COMPONENT_INFO>\n");
		}
	}


 
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0003>\n", gbp->wl_betag);


    return(0);
}

