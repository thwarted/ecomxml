/* tt0085.c  Get Dynamic Upsell */
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


int             tt0085_CatSendStr(struct global_struct *, char *, tt0085_st_send *);
int             tt0085_ParceRecvStr(tt0085_st_recv *, char *, request_rec *, struct global_struct *);
int             tt0085_lt_process(request_rec *, struct global_struct *, char *);


int tt0085_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0085_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0085_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0085_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0085 = (tt0085_st_send *)malloc(sizeof(tt0085_st_send));
        gbp->recvbuf0085 = (tt0085_st_recv *)malloc(sizeof(tt0085_st_recv));

        gbp->sendbufcat = malloc(tt0085_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate gbp->sendbufcat\n");
                fflush(stdout);
                return(-1);
        }

        gbp->recvbufcat = malloc(tt0085_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate gbp->recvbufcat\n");
                fflush(stdout);
                return(-1);
        }

    memset(gbp->sendbufcat, '\0', tt0085_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0085_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0085,  '\0',  sizeof(tt0085_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0085->request_id, "XML");
    strcpy(gbp->sendbuf0085->record_id, "0085");

        strcpy(gbp->sendbuf0085->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0085->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0085->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0085->userid,gbp,stdout_buffer);
        get_tag_data("PROD_NUM", gbp->sendbuf0085->item_num,gbp,stdout_buffer);
        get_tag_data("CUST_EDP", gbp->sendbuf0085->cust_edp,gbp,stdout_buffer);
        get_tag_data("PREDICTIVE_TYPE", gbp->sendbuf0085->predictive_type,gbp,stdout_buffer);
        

        if(tt0085_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0085) == tt0085_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbufcat);
                        free (gbp->recvbuf0085);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0085_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbufcat);
                        free (gbp->recvbuf0085);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0085_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->recvbuf0085);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbufcat);
                        free (gbp->recvbuf0085);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0085_ParceRecvStr(gbp->recvbuf0085, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->recvbufcat);
                free (gbp->recvbuf0085);
        }
        else
        {
                XML_Error("tt0085_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbufcat);
                free (gbp->recvbuf0085);
                return(-1);
        }

    return(0);
}


int tt0085_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0085_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-20.20s%09d%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->item_num,
                        atoi(ptr_sendbuf->cust_edp),
                        ptr_sendbuf->predictive_type);

            
    return(gbp->j);
}


int tt0085_ParceRecvStr(tt0085_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r, struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"http://localhost/xml-dtd/tt0086.xsl\"?>\n");

#endif
/*
        ap_rprintf(r,"<!DOCTYPE %s0086 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                        tt_tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0086.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"<!-- GET DYNAMIC UPSELL -->\n");

        ap_rprintf(r,"%s0086 %s\"tt0086\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0085_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0085_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0085_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0085_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0085_REC_ID_LEN );
    gbp->count += tt0085_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0085_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0085_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0085_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0085_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0085_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0085_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0085_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0085_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0085_ERR_LEN;

        gbp->count += tt0085_SEND_FILLER_LEN;


        memset(ptr_recvbuf->item_num,'\0', tt0085_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0085_ITM_NO_LEN);
        ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_num));
    gbp->count += tt0085_ITM_NO_LEN;

        memset(ptr_recvbuf->item_edp,'\0', tt0085_EDP_LEN+1);
    memcpy(ptr_recvbuf->item_edp, sz_recvbufcat + gbp->count, tt0085_EDP_LEN);
        ap_rprintf(r,"		<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item_edp));
    gbp->count += tt0085_EDP_LEN;

        memset(ptr_recvbuf->item_desc,'\0', tt0085_DESC_LEN+1);
    memcpy(ptr_recvbuf->item_desc, sz_recvbufcat + gbp->count, tt0085_DESC_LEN);
        ap_rprintf(r,"		<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->item_desc));
    gbp->count += tt0085_DESC_LEN;

        memset(ptr_recvbuf->prod_count,'\0', tt0085_COUNT_LEN+1);
    memcpy(ptr_recvbuf->prod_count, sz_recvbufcat + gbp->count, tt0085_COUNT_LEN);
        ap_rprintf(r,"		<PROD_COUNT>%s</PROD_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->prod_count));
    gbp->count += tt0085_COUNT_LEN;

        memset(ptr_recvbuf->cat_count,'\0', tt0085_COUNT_LEN+1);
    memcpy(ptr_recvbuf->cat_count, sz_recvbufcat + gbp->count, tt0085_COUNT_LEN);
        ap_rprintf(r,"		<CAT_COUNT>%s</CAT_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->cat_count));
    gbp->count += tt0085_COUNT_LEN;


        for (gbp->i = 0; gbp->i < tt0085_UPS_PROD_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->ups_prod[gbp->i].ups_prod_num,'\0', tt0085_ITM_NO_LEN+1);
            memcpy(ptr_recvbuf->ups_prod[gbp->i].ups_prod_num, sz_recvbufcat + gbp->count, tt0085_ITM_NO_LEN);
                gbp->count += tt0085_ITM_NO_LEN;

                memset(ptr_recvbuf->ups_prod[gbp->i].ups_prod_edp,'\0', tt0085_EDP_LEN+1);
            memcpy(ptr_recvbuf->ups_prod[gbp->i].ups_prod_edp, sz_recvbufcat + gbp->count, tt0085_EDP_LEN);
                gbp->count += tt0085_EDP_LEN;

                memset(ptr_recvbuf->ups_prod[gbp->i].ups_prod_desc,'\0', tt0085_DESC_LEN+1);
            memcpy(ptr_recvbuf->ups_prod[gbp->i].ups_prod_desc, sz_recvbufcat + gbp->count, tt0085_DESC_LEN);
                gbp->count += tt0085_DESC_LEN;

                memset(ptr_recvbuf->ups_prod[gbp->i].ups_prod_image,'\0', tt0085_IMG_LEN+1);
            memcpy(ptr_recvbuf->ups_prod[gbp->i].ups_prod_image, sz_recvbufcat + gbp->count, tt0085_IMG_LEN);
                gbp->count += tt0085_IMG_LEN;

                memset(ptr_recvbuf->ups_prod[gbp->i].ups_prod_price,'\0', tt0085_PRICE_LEN+1);
            memcpy(ptr_recvbuf->ups_prod[gbp->i].ups_prod_price, sz_recvbufcat + gbp->count, tt0085_PRICE_LEN);
                gbp->count += tt0085_PRICE_LEN;

                memset(ptr_recvbuf->ups_prod[gbp->i].ups_prod_avail_inv,'\0', tt0085_AVAIL_LEN+1);
            memcpy(ptr_recvbuf->ups_prod[gbp->i].ups_prod_avail_inv, sz_recvbufcat + gbp->count, tt0085_AVAIL_LEN);
                gbp->count += tt0085_AVAIL_LEN;
        }

        gbp->k = atoi(ptr_recvbuf->prod_count);
        if(gbp->k > (int)tt0085_UPS_PROD_COUNT)
                gbp->k = (int)tt0085_UPS_PROD_COUNT;

        for (gbp->i = 0; gbp->i < gbp->k; gbp->i++)
        {
                ap_rprintf(r,"		<UPS_PRODUCTS_DETAIL>\n");
                ap_rprintf(r,"			<UPS_PROD_NUM>%s</UPS_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->ups_prod[gbp->i].ups_prod_num));
                ap_rprintf(r,"			<UPS_PROD_EDP>%s</UPS_PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->ups_prod[gbp->i].ups_prod_edp));
                ap_rprintf(r,"			<UPS_PROD_DESCRIPTION>%s</UPS_PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->ups_prod[gbp->i].ups_prod_desc));
                ap_rprintf(r,"			<UPS_PROD_IMAGE>%s</UPS_PROD_IMAGE>\n", handle_special_chars(gbp,ptr_recvbuf->ups_prod[gbp->i].ups_prod_image));
                ap_rprintf(r,"			<UPS_PROD_PRICE>%s</UPS_PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->ups_prod[gbp->i].ups_prod_price));
                ap_rprintf(r,"			<UPS_PROD_AVAIL_INV>%s</UPS_PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->ups_prod[gbp->i].ups_prod_avail_inv));
                ap_rprintf(r,"		</UPS_PRODUCTS_DETAIL>\n");
        }

        for (gbp->i = 0; gbp->i < tt0085_UPS_CAT_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->ups_cat[gbp->i].ups_cat_num,'\0', tt0085_ITM_NO_LEN+1);
            memcpy(ptr_recvbuf->ups_cat[gbp->i].ups_cat_num, sz_recvbufcat + gbp->count, tt0085_ITM_NO_LEN);
                gbp->count += tt0085_ITM_NO_LEN;

                memset(ptr_recvbuf->ups_cat[gbp->i].ups_cat_edp,'\0', tt0085_EDP_LEN+1);
            memcpy(ptr_recvbuf->ups_cat[gbp->i].ups_cat_edp, sz_recvbufcat + gbp->count, tt0085_EDP_LEN);
                gbp->count += tt0085_EDP_LEN;

                memset(ptr_recvbuf->ups_cat[gbp->i].ups_cat_desc,'\0', tt0085_DESC_LEN+1);
            memcpy(ptr_recvbuf->ups_cat[gbp->i].ups_cat_desc, sz_recvbufcat + gbp->count, tt0085_DESC_LEN);
                gbp->count += tt0085_DESC_LEN;

                memset(ptr_recvbuf->ups_cat[gbp->i].ups_cat_image,'\0', tt0085_IMG_LEN+1);
            memcpy(ptr_recvbuf->ups_cat[gbp->i].ups_cat_image, sz_recvbufcat + gbp->count, tt0085_IMG_LEN);
                gbp->count += tt0085_IMG_LEN;

                memset(ptr_recvbuf->ups_cat[gbp->i].ups_cat_price,'\0', tt0085_PRICE_LEN+1);
            memcpy(ptr_recvbuf->ups_cat[gbp->i].ups_cat_price, sz_recvbufcat + gbp->count, tt0085_PRICE_LEN);
                gbp->count += tt0085_PRICE_LEN;

                memset(ptr_recvbuf->ups_cat[gbp->i].ups_cat_avail_inv,'\0', tt0085_AVAIL_LEN+1);
            memcpy(ptr_recvbuf->ups_cat[gbp->i].ups_cat_avail_inv, sz_recvbufcat + gbp->count, tt0085_AVAIL_LEN);
                gbp->count += tt0085_AVAIL_LEN;
        }

        gbp->l = atoi(ptr_recvbuf->cat_count);
        if(gbp->l > (int)tt0085_UPS_CAT_COUNT)
                gbp->l = (int)tt0085_UPS_CAT_COUNT;

        for (gbp->i = 0; gbp->i < gbp->l; gbp->i++)
        {
                ap_rprintf(r,"		<UPS_CATAGORYS_DETAIL>\n");
                ap_rprintf(r,"			<UPS_CAT_NUM>%s</UPS_CAT_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->ups_cat[gbp->i].ups_cat_num));
                ap_rprintf(r,"			<UPS_CAT_EDP>%s</UPS_CAT_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->ups_cat[gbp->i].ups_cat_edp));
                ap_rprintf(r,"			<UPS_CAT_DESCRIPTION>%s</UPS_CAT_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->ups_cat[gbp->i].ups_cat_desc));
                ap_rprintf(r,"			<UPS_CAT_IMAGE>%s</UPS_CAT_IMAGE>\n", handle_special_chars(gbp,ptr_recvbuf->ups_cat[gbp->i].ups_cat_image));
                ap_rprintf(r,"			<UPS_CAT_PRICE>%s</UPS_CAT_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->ups_cat[gbp->i].ups_cat_price));
                ap_rprintf(r,"			<UPS_CAT_AVAIL_INV>%s</UPS_CAT_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->ups_cat[gbp->i].ups_cat_avail_inv));
                ap_rprintf(r,"		</UPS_CATAGORYS_DETAIL>\n");
        }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0086>\n", gbp->tt_betag);

    return(0);
}

