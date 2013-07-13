/* tt0057.c Variable Kit Put In Cart */
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

int             tt0057_CatSendStr(struct global_struct *,char *, tt0057_st_send *);
int             tt0057_ParceRecvStr(tt0057_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0057_lt_process(request_rec *, struct global_struct *,char *);


int tt0057_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0057_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0057_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0057_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0057 = (tt0057_st_send *)malloc(sizeof(tt0057_st_send));
        gbp->recvbuf0057 = (tt0057_st_recv *)malloc(sizeof(tt0057_st_recv));

        gbp->sendbufcat = malloc(tt0057_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0057_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0057_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0057_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0057,  '\0',  sizeof(tt0057_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0057->request_id, "XML");
    strcpy(gbp->sendbuf0057->record_id, "0057");

        strcpy(gbp->sendbuf0057->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0057->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0057->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0057->userid,gbp,stdout_buffer);
        get_tag_data("KIT_PROD_NUM", gbp->sendbuf0057->kit_item_num,gbp,stdout_buffer);
        get_tag_data("KIT_PROD_QTY", gbp->sendbuf0057->kit_item_qty,gbp,stdout_buffer);
        get_tag_data("INVENTORY_REQD", gbp->sendbuf0057->inv_reqd,gbp,stdout_buffer);
        get_tag_data("SHIPTO_NUM", gbp->sendbuf0057->shipto_num,gbp,stdout_buffer);
        
        for (gbp->i = 0; gbp->i < tt0057_R1_ITEM_COUNT; gbp->i++)
        {
                sprintf(gbp->tagname, "R1_PROD_NUM_%d", gbp->i+1);
                get_tag_data(gbp->tagname , gbp->sendbuf0057->r1_item_num[gbp->i],gbp,stdout_buffer);

                sprintf(gbp->tagname, "R1_PROD_QTY_%d", gbp->i+1);
                get_tag_data(gbp->tagname , gbp->sendbuf0057->r1_item_qty[gbp->i],gbp,stdout_buffer);
        }


        if(tt0057_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0057) == tt0057_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0057);
                        free (gbp->recvbuf0057);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0057_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0057);
                        free (gbp->recvbuf0057);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0057_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0057);
                        free (gbp->recvbuf0057);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0057);
                        free (gbp->recvbuf0057);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0057_ParceRecvStr(gbp->recvbuf0057, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0057);
                free (gbp->recvbuf0057);
        }
        else
        {
                XML_Error("tt0057_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0057);
                free (gbp->recvbuf0057);
                return(-1);
        }

    return(0);
}


int tt0057_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0057_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-20.20s%04d%-1.1s%-2.2s"
                        "%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
                        "%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
                        "%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
                        "%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
                        "%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->kit_item_num,
                        atoi(ptr_sendbuf->kit_item_qty),
                        ptr_sendbuf->inv_reqd,
                        ptr_sendbuf->shipto_num,
                        ptr_sendbuf->r1_item_num[0],
                        atoi(ptr_sendbuf->r1_item_qty[0]),
                        ptr_sendbuf->r1_item_num[1],
                        atoi(ptr_sendbuf->r1_item_qty[1]),
                        ptr_sendbuf->r1_item_num[2],
                        atoi(ptr_sendbuf->r1_item_qty[2]),
                        ptr_sendbuf->r1_item_num[3],
                        atoi(ptr_sendbuf->r1_item_qty[3]),
                        ptr_sendbuf->r1_item_num[4],
                        atoi(ptr_sendbuf->r1_item_qty[4]),
                        ptr_sendbuf->r1_item_num[5],
                        atoi(ptr_sendbuf->r1_item_qty[5]),
                        ptr_sendbuf->r1_item_num[6],
                        atoi(ptr_sendbuf->r1_item_qty[6]),
                        ptr_sendbuf->r1_item_num[7],
                        atoi(ptr_sendbuf->r1_item_qty[7]),
                        ptr_sendbuf->r1_item_num[8],
                        atoi(ptr_sendbuf->r1_item_qty[8]),
                        ptr_sendbuf->r1_item_num[9],
                        atoi(ptr_sendbuf->r1_item_qty[9]),
                        ptr_sendbuf->r1_item_num[10],
                        atoi(ptr_sendbuf->r1_item_qty[10]),
                        ptr_sendbuf->r1_item_num[11],
                        atoi(ptr_sendbuf->r1_item_qty[11]),
                        ptr_sendbuf->r1_item_num[12],
                        atoi(ptr_sendbuf->r1_item_qty[12]),
                        ptr_sendbuf->r1_item_num[13],
                        atoi(ptr_sendbuf->r1_item_qty[13]),
                        ptr_sendbuf->r1_item_num[14],
                        atoi(ptr_sendbuf->r1_item_qty[14]),
                        ptr_sendbuf->r1_item_num[15],
                        atoi(ptr_sendbuf->r1_item_qty[15]),
                        ptr_sendbuf->r1_item_num[16],
                        atoi(ptr_sendbuf->r1_item_qty[16]),
                        ptr_sendbuf->r1_item_num[17],
                        atoi(ptr_sendbuf->r1_item_qty[17]),
                        ptr_sendbuf->r1_item_num[18],
                        atoi(ptr_sendbuf->r1_item_qty[18]),
                        ptr_sendbuf->r1_item_num[19],
                        atoi(ptr_sendbuf->r1_item_qty[19]),
                        ptr_sendbuf->r1_item_num[20],
                        atoi(ptr_sendbuf->r1_item_qty[20]),
                        ptr_sendbuf->r1_item_num[21],
                        atoi(ptr_sendbuf->r1_item_qty[21]),
                        ptr_sendbuf->r1_item_num[22],
                        atoi(ptr_sendbuf->r1_item_qty[22]),
                        ptr_sendbuf->r1_item_num[23],
                        atoi(ptr_sendbuf->r1_item_qty[23]),
                        ptr_sendbuf->r1_item_num[24],
                        atoi(ptr_sendbuf->r1_item_qty[24]));
                        
    return(gbp->j);
}


int tt0057_ParceRecvStr(tt0057_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;
        
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0058.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0058 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0058.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0058 %s\"tt0058\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0057_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0057_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0057_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0057_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0057_REC_ID_LEN );
    gbp->count += tt0057_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0057_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0057_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0057_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0057_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0057_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0057_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0057_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0057_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0057_ERR_LEN;

        gbp->count += tt0057_SEND_FILLER_LEN;


        memset(ptr_recvbuf->kit_item_num,'\0', tt0057_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->kit_item_num, sz_recvbufcat + gbp->count, tt0057_ITM_NO_LEN);
        ap_rprintf(r,"		<KIT_PROD_NUM>%s</KIT_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_num));
    gbp->count += tt0057_ITM_NO_LEN;

        memset(ptr_recvbuf->kit_item_edp,'\0', tt0057_EDP_LEN+1);
    memcpy(ptr_recvbuf->kit_item_edp, sz_recvbufcat + gbp->count, tt0057_EDP_LEN);
        ap_rprintf(r,"		<KIT_PROD_EDP>%s</KIT_PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_edp));
    gbp->count += tt0057_EDP_LEN;

        memset(ptr_recvbuf->kit_item_desc,'\0', tt0057_DESC_LEN+1);
    memcpy(ptr_recvbuf->kit_item_desc, sz_recvbufcat + gbp->count, tt0057_DESC_LEN);
        ap_rprintf(r,"		<KIT_PROD_DESCRIPTION>%s</KIT_PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_desc));
    gbp->count += tt0057_DESC_LEN;

        memset(ptr_recvbuf->upsl_X,'\0', tt0057_UPS_LEN+1);
    memcpy(ptr_recvbuf->upsl_X, sz_recvbufcat + gbp->count, tt0057_UPS_LEN);
        ap_rprintf(r,"		<UPSL_X>%s</UPSL_X>\n", handle_special_chars(gbp,ptr_recvbuf->upsl_X));
    gbp->count += tt0057_UPS_LEN;

        memset(ptr_recvbuf->image_ovrd,'\0', tt0057_IMG_OVRD_LEN+1);
    memcpy(ptr_recvbuf->image_ovrd, sz_recvbufcat + gbp->count, tt0057_IMG_OVRD_LEN);
        ap_rprintf(r,"		<IMAGE_OVERRIDE>%s</IMAGE_OVERRIDE>\n", handle_special_chars(gbp,ptr_recvbuf->image_ovrd));
    gbp->count += tt0057_IMG_OVRD_LEN;

        memset(ptr_recvbuf->kit_item_price,'\0', tt0057_PRICE_LEN+1);
    memcpy(ptr_recvbuf->kit_item_price, sz_recvbufcat + gbp->count, tt0057_PRICE_LEN);
        ap_rprintf(r,"		<KIT_PROD_PRICE>%s</KIT_PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_price));
    gbp->count += tt0057_PRICE_LEN;

    memset(ptr_recvbuf->kit_item_avail_inv,'\0', tt0057_AVAIL_LEN+1);
    memcpy(ptr_recvbuf->kit_item_avail_inv, sz_recvbufcat + gbp->count, tt0057_AVAIL_LEN);
        ap_rprintf(r,"		<KIT_PROD_AVAIL_INV>%s</KIT_PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_avail_inv));
    gbp->count += tt0057_AVAIL_LEN;

        memset(ptr_recvbuf->kit_item_qty,'\0', tt0057_QTY_LEN+1);
    memcpy(ptr_recvbuf->kit_item_qty, sz_recvbufcat + gbp->count, tt0057_QTY_LEN);
        ap_rprintf(r,"		<KIT_PROD_QTY>%s</KIT_PROD_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_qty));
    gbp->count += tt0057_QTY_LEN;

        memset(ptr_recvbuf->custom_flag,'\0', tt0057_CUSTOM_FLAG_LEN+1);
    memcpy(ptr_recvbuf->custom_flag, sz_recvbufcat + gbp->count, tt0057_CUSTOM_FLAG_LEN);
        ap_rprintf(r,"		<CUSTOM_FLAG>%s</CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->custom_flag));
    gbp->count += tt0057_CUSTOM_FLAG_LEN;

        memset(ptr_recvbuf->item_line_number,'\0', tt0057_ITEM_LINE_NUMBER_LEN+1);
    memcpy(ptr_recvbuf->item_line_number, sz_recvbufcat + gbp->count, tt0057_ITEM_LINE_NUMBER_LEN);
        ap_rprintf(r,"		<ITEM_LINE_NUMBER>%s</ITEM_LINE_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->item_line_number));
    gbp->count += tt0057_ITEM_LINE_NUMBER_LEN;

        memset(ptr_recvbuf->r1_count,'\0', tt0057_R1_COUNT_LEN+1);
    memcpy(ptr_recvbuf->r1_count, sz_recvbufcat + gbp->count, tt0057_R1_COUNT_LEN);
        ap_rprintf(r,"		<R1_COUNT>%s</R1_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->r1_count));
    gbp->count += tt0057_R1_COUNT_LEN;


        for(gbp->i = 0; gbp->i < tt0057_R1_COMP_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_num,'\0', tt0057_R1_ITM_NO_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_num, sz_recvbufcat + gbp->count, tt0057_R1_ITM_NO_LEN);
                gbp->count += tt0057_R1_ITM_NO_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_edp,'\0', tt0057_R1_EDP_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_edp, sz_recvbufcat + gbp->count, tt0057_R1_EDP_LEN);
                gbp->count += tt0057_R1_EDP_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_desc,'\0', tt0057_R1_DESC_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_desc, sz_recvbufcat + gbp->count, tt0057_R1_DESC_LEN);
                gbp->count += tt0057_R1_DESC_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_qty,'\0', tt0057_R1_QTY_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_qty, sz_recvbufcat + gbp->count, tt0057_R1_QTY_LEN);
                gbp->count += tt0057_R1_QTY_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_upsl_X,'\0', tt0057_R1_UPS_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_upsl_X, sz_recvbufcat + gbp->count, tt0057_R1_UPS_LEN);
                gbp->count += tt0057_R1_UPS_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_image_ovrd,'\0', tt0057_R1_IMG_OVRD_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_image_ovrd, sz_recvbufcat + gbp->count, tt0057_R1_IMG_OVRD_LEN);
                gbp->count += tt0057_R1_IMG_OVRD_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_price,'\0', tt0057_R1_PRICE_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_price, sz_recvbufcat + gbp->count, tt0057_R1_PRICE_LEN);
                gbp->count += tt0057_R1_PRICE_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_avail_inv,'\0', tt0057_R1_AVAIL_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_avail_inv, sz_recvbufcat + gbp->count, tt0057_R1_AVAIL_LEN);
                gbp->count += tt0057_R1_AVAIL_LEN;
        }

        gbp->k = atoi(ptr_recvbuf->r1_count);
        if(gbp->k > (int)tt0057_R1_COMP_COUNT)
                gbp->k = (int)tt0057_R1_COMP_COUNT;

        for(gbp->i = 0; gbp->i < gbp->k; gbp->i++)
        {
                ap_rprintf(r,"		<R1_COMPONENTS_DETAILS>\n");
                ap_rprintf(r,"			<R1_PROD_NUM>%s</R1_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_num));
                ap_rprintf(r,"			<R1_PROD_EDP>%s</R1_PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_edp));
                ap_rprintf(r,"			<R1_PROD_DESCRIPTION>%s</R1_PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_desc));
                ap_rprintf(r,"			<R1_PROD_QTY>%s</R1_PROD_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_qty));
                ap_rprintf(r,"			<R1_UPSL_X>%s</R1_UPSL_X>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_upsl_X));
                ap_rprintf(r,"			<R1_IMAGE_OVERRIDE>%s</R1_IMAGE_OVERRIDE>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_image_ovrd));
                ap_rprintf(r,"			<R1_PROD_PRICE>%s</R1_PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_price));
                ap_rprintf(r,"			<R1_PROD_AVAIL_INV>%s</R1_PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_avail_inv));
                ap_rprintf(r,"		</R1_COMPONENTS_DETAILS>\n");
        }


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0058>\n", gbp->tt_betag);

    return(0);
}

