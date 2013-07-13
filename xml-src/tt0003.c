/* tt0003.c  Shopping Request */
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


int             tt0003_CatSendStr(struct global_struct *, char *, tt0003_st_send *);
int             tt0003_ParceRecvStr(tt0003_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0003_lt_process(request_rec *, struct global_struct *, char *);

int tt0003_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}


int tt0003_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0003_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0003_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0003 = (tt0003_st_send *)malloc(sizeof(tt0003_st_send));
        gbp->recvbuf0003 = (tt0003_st_recv *)malloc(sizeof(tt0003_st_recv));

        gbp->sendbufcat = malloc(tt0003_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0003_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0003_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0003_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0003,  '\0',  sizeof(tt0003_st_send));

    strcpy(gbp->sendbuf0003->request_id, "XML");
    strcpy(gbp->sendbuf0003->record_id, "0003");

        get_tag_data("COMPANY", gbp->sendbuf0003->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0003->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0003->userid,gbp,stdout_buffer);

        strcpy(gbp->sendbuf0003->ip_address, r->connection->remote_ip);

        get_tag_data("SEARCH_KEY_WORD", gbp->sendbuf0003->key_word,gbp,stdout_buffer);
        get_tag_data("CURR_PAGE_NUM", gbp->sendbuf0003->curr_page_num,gbp,stdout_buffer);
        get_tag_data("PAGE_LEN", gbp->sendbuf0003->page_len,gbp,stdout_buffer);  
        get_tag_data("CAT_PROD_NUM", gbp->sendbuf0003->cat_item_num,gbp,stdout_buffer);

        if(tt0003_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0003) == tt0003_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0003);
                        free (gbp->recvbuf0003);
                        return(-1);
                }
//ap_rprintf(r,"HOST %s PORT %s SENDBUF %s",gbp->hphost, gbp->webport,gbp->sendbufcat); 

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0003_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0003);
                        free (gbp->recvbuf0003);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0003_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0003);
                        free (gbp->recvbuf0003);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0003);
                        free (gbp->recvbuf0003);
                        return(-1);
                }

/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);
//ap_rprintf(r,"RECVBUF %s",gbp->recvbufcat); 

                tt0003_ParceRecvStr(gbp->recvbuf0003, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0003);
                free (gbp->recvbuf0003);

        }
        else
        {
                XML_Error("tt0003_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0003);
                free (gbp->recvbuf0003);
                return(-1);
        }

    return(0);
}


int tt0003_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0003_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
        "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-150.150s%04d%02d%-20.20s",
        ptr_sendbuf->request_id,
        ptr_sendbuf->record_id,
        ptr_sendbuf->company,
        ptr_sendbuf->division,
        ptr_sendbuf->userid,
        ptr_sendbuf->ip_address,
                ptr_sendbuf->filler,
        ptr_sendbuf->key_word,
        atoi(ptr_sendbuf->curr_page_num),
        atoi(ptr_sendbuf->page_len),
                ptr_sendbuf->cat_item_num);


    return(gbp->j);
}


int tt0003_ParceRecvStr(tt0003_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    /* Place the individual variables into target fields */
//ap_rprintf(r,"RECVBUF %s",sz_recvbufcat);
        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);
#ifdef XSL
        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0004.xsl\"?>\n");
#endif
/*              ap_rprintf(r,"<!DOCTYPE %s0004 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);
        ap_rprintf(r,"                  \"%s%s0004.dtd\">\n\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0004 %s\"tt0004\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n\n", gbp->mtag);

        memset(ptr_recvbuf->request_id,'\0', tt0003_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0003_REQ_ID_LEN);
    gbp->count += tt0003_REQ_ID_LEN;//ap_rprintf(r,"reqid=%s",ptr_recvbuf->request_id);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n\n", handle_special_chars(gbp,ptr_recvbuf->request_id));

        memset(ptr_recvbuf->record_id,'\0', tt0003_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0003_REC_ID_LEN );
    gbp->count += tt0003_REC_ID_LEN;

        memset(ptr_recvbuf->userid,'\0', tt0003_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0003_USER_ID_LEN );
    gbp->count += tt0003_USER_ID_LEN;
        ap_rprintf(r,"		<UID>%s</UID>\n\n", handle_special_chars(gbp,ptr_recvbuf->userid));

    memset(ptr_recvbuf->success_flag,'\0', tt0003_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, tt0003_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += tt0003_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0003_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0003_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0003_ERR_LEN;

        gbp->count += tt0003_SEND_FILLER_LEN;

        memset(ptr_recvbuf->item_count,'\0', tt0003_ITM_COUNT_LEN+1);
    memcpy(ptr_recvbuf->item_count, sz_recvbufcat + gbp->count, tt0003_ITM_COUNT_LEN);
    gbp->count += tt0003_ITM_COUNT_LEN;
        ap_rprintf(r,"		<PROD_COUNT>%s</PROD_COUNT>\n\n", handle_special_chars(gbp,ptr_recvbuf->item_count));

        memset(ptr_recvbuf->current_page,'\0', tt0003_PAGE_NO_LEN+1);
    memcpy(ptr_recvbuf->current_page, sz_recvbufcat + gbp->count, tt0003_PAGE_NO_LEN);
    gbp->count += tt0003_PAGE_NO_LEN;
        ap_rprintf(r,"		<CURRENT_PAGE>%s</CURRENT_PAGE>\n\n", handle_special_chars(gbp,ptr_recvbuf->current_page));

        // Build data from array 

        gbp->k = (int)tt0003_ITEM_COUNT;
        
    for(gbp->i = 0; gbp->i < gbp->k; gbp->i++) 
    {
                memset(ptr_recvbuf->item[gbp->i].item_edp,'\0',tt0003_ITM_EDP_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_edp, sz_recvbufcat + gbp->count, tt0003_ITM_EDP_LEN);
        gbp->count += tt0003_ITM_EDP_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_num,'\0',tt0003_ITM_NO_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_num, sz_recvbufcat+gbp->count, tt0003_ITM_NO_LEN);
        gbp->count += tt0003_ITM_NO_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_desc,'\0',tt0003_ITM_DESC_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_desc, sz_recvbufcat+gbp->count, tt0003_ITM_DESC_LEN);
        gbp->count += tt0003_ITM_DESC_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_price,'\0',tt0003_ITM_PRICE_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_price, sz_recvbufcat+gbp->count, tt0003_ITM_PRICE_LEN);
        gbp->count += tt0003_ITM_PRICE_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_avail_inv,'\0',tt0003_ITM_AVAIL_INV_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_avail_inv, sz_recvbufcat+gbp->count, tt0003_ITM_AVAIL_INV_LEN);
        gbp->count += tt0003_ITM_AVAIL_INV_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_po_date,'\0',tt0003_ITM_PO_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_po_date, sz_recvbufcat+gbp->count, tt0003_ITM_PO_LEN);
        gbp->count += tt0003_ITM_PO_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_image_ovrd,'\0',tt0003_ITM_IMAGE_OVRD_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].item_image_ovrd, sz_recvbufcat+gbp->count, tt0003_ITM_IMAGE_OVRD_LEN);
        gbp->count += tt0003_ITM_IMAGE_OVRD_LEN;

                memset(ptr_recvbuf->item[gbp->i].shopping_item_type,'\0',tt0003_ITM_TYPE_LEN+1);
        memcpy(ptr_recvbuf->item[gbp->i].shopping_item_type, sz_recvbufcat + gbp->count, tt0003_ITM_TYPE_LEN);
        gbp->count += tt0003_ITM_TYPE_LEN;
        }


        gbp->k = (int)tt0003_ITEM_COUNT;
        

    for(gbp->i = 0; gbp->i < gbp->k; gbp->i++) 
    {
                if(strcmp(ptr_recvbuf->item[gbp->i].item_edp, "         ") != 0)
                {
                   ap_rprintf(r,"		<PRODUCT_DETAIL>\n\n");
                   ap_rprintf(r,"			<PROD_EDP>%s</PROD_EDP>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_edp));
                   ap_rprintf(r,"			<PROD_NUM>%s</PROD_NUM>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_num)); 
                   ap_rprintf(r,"			<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_desc));
                   ap_rprintf(r,"			<PROD_PRICE>%s</PROD_PRICE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_price));
                   ap_rprintf(r,"			<PROD_AVAIL_INV>%s</PROD_AVAIL_INV>\n\n", 
                                                                                                                       handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_avail_inv));
                   ap_rprintf(r,"			<PROD_PO_DATE>%s</PROD_PO_DATE>\n\n", 
                                                                                                                       handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_po_date));
                   ap_rprintf(r,"			<IMAGE_OVERRIDE>%s</IMAGE_OVERRIDE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_image_ovrd));
                   ap_rprintf(r,"			<SHOPPING_PROD_TYPE>%s</SHOPPING_PROD_TYPE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shopping_item_type));
                   ap_rprintf(r,"		</PRODUCT_DETAIL>\n\n");
                }
    }

        ap_rprintf(r,"	%s>\n\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n\n", gbp->rsetag);

        ap_rprintf(r,"%s0004>\n\n", gbp->tt_betag);

    return(0);
}


