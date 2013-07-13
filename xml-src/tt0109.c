/* tt0109.c  Giftwrap Options*/

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


int             tt0109_CatSendStr(struct global_struct *,char *, tt0109_st_send *);
int             tt0109_ParceRecvStr(tt0109_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0109_lt_process(request_rec *, struct global_struct *,char *);


int tt0109_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0109_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0109_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

/******************************************************************/
/*                                                                */
/*  Process the request                                           */
/*                                                                */
/******************************************************************/


int tt0109_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0109 = (tt0109_st_send *)malloc(sizeof(tt0109_st_send));
        gbp->recvbuf0109 = (tt0109_st_recv *)malloc(sizeof(tt0109_st_recv));

        gbp->sendbufcat = malloc(tt0109_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0109_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0109_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0109_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0109,  '\0',  sizeof(tt0109_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0109->request_id, "XML");
    strcpy(gbp->sendbuf0109->record_id, "0109");

        strcpy(gbp->sendbuf0109->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0109->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0109->division,gbp,stdout_buffer);
        get_tag_data("UID",  gbp->sendbuf0109->userid,gbp,stdout_buffer);
        get_tag_data("GWRAP_FLAG",  gbp->sendbuf0109->gwrap_flag,gbp,stdout_buffer);
        get_tag_data("PROD_NUM",  gbp->sendbuf0109->prod_num,gbp,stdout_buffer);
        get_tag_data("SHOP_CART_PAGE_NUM",  gbp->sendbuf0109->shop_cart_page_num,gbp,stdout_buffer);
        get_tag_data("SHOP_CART_LINE_NUM",  gbp->tagname,gbp,stdout_buffer);
        get_tag_data("GWRAP_ITEM_NO",  gbp->sendbuf0109->gwrap_item_no,gbp,stdout_buffer);

    gbp->i = atoi(gbp->tagname);
    sprintf(gbp->sendbuf0109->shop_cart_line_num, "%03d", gbp->i);

    
        if((tt0109_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0109)) == tt0109_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0109);
                        free (gbp->recvbuf0109);
                        return(-1);
                }

        
                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0109_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0109);
                        free (gbp->recvbuf0109);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0109_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0109);
                        free (gbp->recvbuf0109);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0109);
                        free (gbp->recvbuf0109);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0109_ParceRecvStr(gbp->recvbuf0109, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0109);
                free (gbp->recvbuf0109);
        }
        else
        {
                XML_Error("tt0109_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0109);
                free (gbp->recvbuf0109);
                return(-1);
        }

    return(0);
}


int tt0109_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0109_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-20.20s%-2.2s%-3.3s%-20.20s",
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
                        ptr_sendbuf->shop_cart_line_num,
                        ptr_sendbuf->gwrap_item_no);

    return(gbp->j);
}


int tt0109_ParceRecvStr(tt0109_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;
    // Place the individual variables into target fields

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0110.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0110 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0110.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0110 %s\"tt0110\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

        memset(ptr_recvbuf->request_id, '\0', tt0109_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0109_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0109_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0109_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0109_REC_ID_LEN );
    gbp->count += tt0109_REC_ID_LEN;

        memset(ptr_recvbuf->userid, '\0', tt0109_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0109_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0109_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0109_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0109_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0109_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0109_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0109_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0109_ERR_LEN;

        gbp->count += tt0109_SEND_FILLER_LEN;


    memset(ptr_recvbuf->gwrap_flag, '\0', tt0109_GWRAP_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_flag, sz_recvbufcat + gbp->count, tt0109_GWRAP_FLAG_LEN);
        ap_rprintf(r,"		<GWRAP_FLAG>%s</GWRAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_flag));
    gbp->count += tt0109_GWRAP_FLAG_LEN;

    memset(ptr_recvbuf->prod_num, '\0', tt0109_PROD_NUM_LEN + 1);
    memcpy(ptr_recvbuf->prod_num, sz_recvbufcat + gbp->count, tt0109_PROD_NUM_LEN);
        ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->prod_num));
    gbp->count += tt0109_PROD_NUM_LEN;

    memset(ptr_recvbuf->prod_desc, '\0', tt0109_PROD_DESC_LEN + 1);
    memcpy(ptr_recvbuf->prod_desc, sz_recvbufcat + gbp->count, tt0109_PROD_DESC_LEN);
        ap_rprintf(r,"		<PROD_DESC>%s</PROD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->prod_desc));
    gbp->count += tt0109_PROD_DESC_LEN;

    memset(ptr_recvbuf->shop_cart_page_num, '\0', tt0109_SHOP_CART_PAGE_NUM_LEN + 1);
    memcpy(ptr_recvbuf->shop_cart_page_num, sz_recvbufcat + gbp->count, tt0109_SHOP_CART_PAGE_NUM_LEN);
        ap_rprintf(r,"		<SHOP_CART_PAGE_NUM>%s</SHOP_CART_PAGE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shop_cart_page_num));
    gbp->count += tt0109_SHOP_CART_PAGE_NUM_LEN;

    memset(ptr_recvbuf->shop_cart_line_num, '\0', tt0109_SHOP_CART_LINE_NUM_LEN + 1);
    memcpy(ptr_recvbuf->shop_cart_line_num, sz_recvbufcat + gbp->count, tt0109_SHOP_CART_LINE_NUM_LEN);
        ap_rprintf(r,"		<SHOP_CART_LINE_NUM>%s</SHOP_CART_LINE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_flag));
    gbp->count += tt0109_SHOP_CART_LINE_NUM_LEN;

    memset(ptr_recvbuf->prod_qty, '\0', tt0109_PROD_QTY_LEN + 1);
    memcpy(ptr_recvbuf->prod_qty, sz_recvbufcat + gbp->count, tt0109_PROD_QTY_LEN);
        ap_rprintf(r,"		<PROD_QTY>%s</PROD_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->prod_qty));
    gbp->count += tt0109_PROD_QTY_LEN;

    memset(ptr_recvbuf->gwrap_item_no, '\0', tt0109_GWRAP_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_item_no, sz_recvbufcat + gbp->count, tt0109_GWRAP_ITEM_NO_LEN);
        ap_rprintf(r,"		<GWRAP_ITEM_NO>%s</GWRAP_ITEM_NO>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_item_no));
    gbp->count += tt0109_GWRAP_ITEM_NO_LEN;

    memset(ptr_recvbuf->gwrap_desc, '\0', tt0109_GWRAP_DESC_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_desc, sz_recvbufcat + gbp->count, tt0109_GWRAP_DESC_LEN);
        ap_rprintf(r,"		<GWRAP_DESC>%s</GWRAP_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_desc));
    gbp->count += tt0109_GWRAP_DESC_LEN;

    memset(ptr_recvbuf->gwrap_custom_flag, '\0', tt0109_GWRAP_CUSTOM_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->gwrap_custom_flag, sz_recvbufcat + gbp->count, tt0109_GWRAP_CUSTOM_FLAG_LEN);
        ap_rprintf(r,"		<GWRAP_CUSTOM_FLAG>%s</GWRAP_CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gwrap_custom_flag));
    gbp->count += tt0109_GWRAP_CUSTOM_FLAG_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0110>\n", gbp->tt_betag);

    return 0;
}
