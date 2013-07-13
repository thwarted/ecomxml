/* tt0021.c Product Pricing */
/* modified for phase 2 - JL */
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


int             tt0021_CatSendStr(struct global_struct *, char *, tt0021_st_send *);
int             tt0021_ParceRecvStr(tt0021_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0021_lt_process(request_rec *, struct global_struct *, char *);


int tt0021_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0021_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0021_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0021_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0021 = (tt0021_st_send *)malloc(sizeof(tt0021_st_send));
        gbp->recvbuf0021 = (tt0021_st_recv *)malloc(sizeof(tt0021_st_recv));

        gbp->sendbufcat = malloc(tt0021_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0021_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0021_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0021_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0021,  '\0',  sizeof(tt0021_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0021->request_id, "XML");
    strcpy(gbp->sendbuf0021->record_id, "0021");

        strcpy(gbp->sendbuf0021->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0021->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0021->division,gbp,stdout_buffer);
        get_tag_data("UID",  gbp->sendbuf0021->userid,gbp,stdout_buffer);

    // Get all of the item numbers.

    for (gbp->i = 0; gbp->i < tt0021_ITEM_COUNT; gbp->i++) 
        {
        sprintf(gbp->tagname, "PROD_NUM_%d", gbp->i+1);
                get_tag_data(gbp->tagname,  gbp->sendbuf0021->item_num[gbp->i],gbp,stdout_buffer);
                
                sprintf(gbp->tagname, "PROD_QTY_%d", gbp->i+1);
                get_tag_data(gbp->tagname,  gbp->sendbuf0021->item_qty[gbp->i],gbp,stdout_buffer);
    }

    if((tt0021_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0021)) == tt0021_LAN_SEND_BUF_LEN)
    {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0021);
                        free (gbp->recvbuf0021);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0021_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0021);
                        free (gbp->recvbuf0021);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0021_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0021);
                        free (gbp->recvbuf0021);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0021);
                        free (gbp->recvbuf0021);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0021_ParceRecvStr(gbp->recvbuf0021, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0021);
                free (gbp->recvbuf0021);
    }
        else
        {
                XML_Error("tt0021_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0021);
                free (gbp->recvbuf0021);
                return(-1);
        }

    return 0;
}


int tt0021_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0021_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
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
            ptr_sendbuf->item_num[0],
                        atoi(ptr_sendbuf->item_qty[0]),
            ptr_sendbuf->item_num[1],
                        atoi(ptr_sendbuf->item_qty[1]),
            ptr_sendbuf->item_num[2],
                        atoi(ptr_sendbuf->item_qty[2]),
            ptr_sendbuf->item_num[3],
                        atoi(ptr_sendbuf->item_qty[3]),
            ptr_sendbuf->item_num[4],
                        atoi(ptr_sendbuf->item_qty[4]),
            ptr_sendbuf->item_num[5],
                        atoi(ptr_sendbuf->item_qty[5]),
            ptr_sendbuf->item_num[6],
                        atoi(ptr_sendbuf->item_qty[6]),
            ptr_sendbuf->item_num[7],
                        atoi(ptr_sendbuf->item_qty[7]),
            ptr_sendbuf->item_num[8],
                        atoi(ptr_sendbuf->item_qty[8]),
            ptr_sendbuf->item_num[9],
                        atoi(ptr_sendbuf->item_qty[9]),
            ptr_sendbuf->item_num[10],
                        atoi(ptr_sendbuf->item_qty[10]),
            ptr_sendbuf->item_num[11],
                        atoi(ptr_sendbuf->item_qty[11]),
            ptr_sendbuf->item_num[12],
                        atoi(ptr_sendbuf->item_qty[12]),
            ptr_sendbuf->item_num[13],
                        atoi(ptr_sendbuf->item_qty[13]),
            ptr_sendbuf->item_num[14],
                        atoi(ptr_sendbuf->item_qty[14]),
            ptr_sendbuf->item_num[15],
                        atoi(ptr_sendbuf->item_qty[15]),
            ptr_sendbuf->item_num[16],
                        atoi(ptr_sendbuf->item_qty[16]),
            ptr_sendbuf->item_num[17],
                        atoi(ptr_sendbuf->item_qty[17]),
            ptr_sendbuf->item_num[18],
                        atoi(ptr_sendbuf->item_qty[18]),
            ptr_sendbuf->item_num[19],
                        atoi(ptr_sendbuf->item_qty[19]));

    return(gbp->j);
}


int tt0021_ParceRecvStr(tt0021_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0022.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0022 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0022.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0022 %s\"tt0022\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


        memset(ptr_recvbuf->request_id ,'\0', tt0021_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0021_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0021_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id ,'\0',tt0021_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0021_REC_ID_LEN );
    gbp->count += tt0021_REC_ID_LEN;

    memset(ptr_recvbuf->userid ,'\0', tt0021_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0021_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0021_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0021_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0021_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0021_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0021_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0021_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0021_ERR_LEN;

        gbp->count += tt0021_SEND_FILLER_LEN;


    for( gbp->i = 0; gbp->i < tt0021_ITEM_COUNT; gbp->i++) 
    {
                
        memset(ptr_recvbuf->item[gbp->i].item_edp ,'\0', tt0021_EDP_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_edp, sz_recvbufcat + gbp->count, tt0021_EDP_LEN);
        gbp->count += tt0021_EDP_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_num ,'\0', tt0021_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_num, sz_recvbufcat + gbp->count, tt0021_ITM_NO_LEN);
        gbp->count += tt0021_ITM_NO_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_desc ,'\0', tt0021_DESC_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_desc, sz_recvbufcat + gbp->count, tt0021_DESC_LEN);
        gbp->count += tt0021_DESC_LEN;
  
        memset(ptr_recvbuf->item[gbp->i].item_price ,'\0', tt0021_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_price, sz_recvbufcat + gbp->count, tt0021_PRICE_LEN);
        gbp->count += tt0021_PRICE_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_avail_flag ,'\0', tt0021_AVAIL_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_avail_flag, sz_recvbufcat + gbp->count, tt0021_AVAIL_FLAG_LEN);
        gbp->count += tt0021_AVAIL_FLAG_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_avail_inv ,'\0', tt0021_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_avail_inv, sz_recvbufcat + gbp->count, tt0021_AVAIL_INV_LEN);
        gbp->count += tt0021_AVAIL_INV_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_qty ,'\0', tt0021_ITM_QTY_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_qty, sz_recvbufcat + gbp->count, tt0021_ITM_QTY_LEN);
        gbp->count += tt0021_ITM_QTY_LEN;
        
                
           if((atoi(ptr_recvbuf->item[gbp->i].item_edp)) > 0)
           {
                        ap_rprintf(r,"		<PRODUCT_DETAIL>\n");
                        ap_rprintf(r,"			<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_edp));
                        ap_rprintf(r,"			<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_num));
                        ap_rprintf(r,"			<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n", 
                                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_desc));
                        ap_rprintf(r,"			<PROD_PRICE>%s</PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_price));
                        ap_rprintf(r,"			<PROD_AVAIL_FLAG>%s</PROD_AVAIL_FLAG>\n", 
                                                                                                                handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_avail_flag));
                        ap_rprintf(r,"			<PROD_AVAIL_INV>%s</PROD_AVAIL_INV>\n", 
                                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_avail_inv));
                        ap_rprintf(r,"			<PROD_QTY>%s</PROD_QTY>\n", 
                                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_qty));
                        ap_rprintf(r,"		</PRODUCT_DETAIL>\n");
                }
    
        }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0022>\n", gbp->tt_betag);

    return 0;
}

