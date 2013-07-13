/* tt0025.c Modify Quantity */
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


int             tt0025_CatSendStr(struct global_struct *,char *, tt0025_st_send *);
int             tt0025_ParceRecvStr(tt0025_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0025_lt_process(request_rec *, struct global_struct *, char *);


int tt0025_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0025_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0025_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0025_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0025 = (tt0025_st_send *)malloc(sizeof(tt0025_st_send));
        gbp->recvbuf0025 = (tt0025_st_recv *)malloc(sizeof(tt0025_st_recv));

        gbp->sendbufcat = malloc(tt0025_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0025_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0025_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0025_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0025,  '\0',  sizeof(tt0025_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0025->request_id, "XML");
    strcpy(gbp->sendbuf0025->record_id, "0025");

        strcpy(gbp->sendbuf0025->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0025->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0025->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0025->userid,gbp,stdout_buffer);
        get_tag_data("PAGE_NO", gbp->sendbuf0025->page_no,gbp,stdout_buffer);


        for(gbp->i = 0; gbp->i < tt0025_MOD_QTY_COUNT; gbp->i++)
        {
        sprintf(gbp->tagname, "MOD_LINE_NUM_%d", gbp->i+1);
                get_tag_data(gbp->tagname , gbp->sendbuf0025->modify_line_num[gbp->i],gbp,stdout_buffer);

                sprintf(gbp->tagname, "MOD_NEW_QTY_%d", gbp->i+1);
                get_tag_data(gbp->tagname , gbp->sendbuf0025->modify_new_qty[gbp->i],gbp,stdout_buffer);
        }


    if((tt0025_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0025)) == tt0025_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0025);
                        free (gbp->recvbuf0025);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0025_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0025);
                        free (gbp->recvbuf0025);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0025_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0025);
                        free (gbp->recvbuf0025);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0025);
                        free (gbp->recvbuf0025);
                        return(-1);
                }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0025_ParceRecvStr(gbp->recvbuf0025, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0025);
                free (gbp->recvbuf0025);
        }
        else
        {
                XML_Error("tt0025_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0025);
                free (gbp->recvbuf0025);
                return(-1);
        }

    return 0;
}


int tt0025_CatSendStr(struct global_struct *gbp, char *sendbufcat, tt0025_st_send *sendbuf)
{
//    FILE *fp;

        gbp->j = -1;

    gbp->j = sprintf(sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-2.2s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s"
            "%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s%-2.2s%-4.4s",

//                      "%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d"
//                      "%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d"
//                      "%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d"
//                      "%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d"
//                      "%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d%02d%04d",

            sendbuf->request_id,
            sendbuf->record_id,
            sendbuf->company,
            sendbuf->division,
            sendbuf->userid,
            sendbuf->ip_address,
                        sendbuf->filler,
                        sendbuf->page_no,
            sendbuf->modify_line_num[0],
            sendbuf->modify_new_qty[0],
                sendbuf->modify_line_num[1],
            sendbuf->modify_new_qty[1],
                        sendbuf->modify_line_num[2],
            sendbuf->modify_new_qty[2],
                        sendbuf->modify_line_num[3],
            sendbuf->modify_new_qty[3],
                        sendbuf->modify_line_num[4],
            sendbuf->modify_new_qty[4],
                        sendbuf->modify_line_num[5],
            sendbuf->modify_new_qty[5],
                        sendbuf->modify_line_num[6],
            sendbuf->modify_new_qty[6],
                        sendbuf->modify_line_num[7],
            sendbuf->modify_new_qty[7],
                        sendbuf->modify_line_num[8],
            sendbuf->modify_new_qty[8],
                        sendbuf->modify_line_num[9],
            sendbuf->modify_new_qty[9],
                        sendbuf->modify_line_num[10],
            sendbuf->modify_new_qty[10],
                        sendbuf->modify_line_num[11],
            sendbuf->modify_new_qty[11],
                        sendbuf->modify_line_num[12],
            sendbuf->modify_new_qty[12],
                        sendbuf->modify_line_num[13],
            sendbuf->modify_new_qty[13],
                        sendbuf->modify_line_num[14],
            sendbuf->modify_new_qty[14],
                        sendbuf->modify_line_num[15],
            sendbuf->modify_new_qty[15],
                        sendbuf->modify_line_num[16],
            sendbuf->modify_new_qty[16],
                        sendbuf->modify_line_num[17],
            sendbuf->modify_new_qty[17],
                        sendbuf->modify_line_num[18],
            sendbuf->modify_new_qty[18],
                        sendbuf->modify_line_num[19],
            sendbuf->modify_new_qty[19],
            sendbuf->modify_line_num[20],
            sendbuf->modify_new_qty[20],
                        sendbuf->modify_line_num[21],
            sendbuf->modify_new_qty[21],
                        sendbuf->modify_line_num[22],
            sendbuf->modify_new_qty[22],
                        sendbuf->modify_line_num[23],
            sendbuf->modify_new_qty[23],
                        sendbuf->modify_line_num[24],
            sendbuf->modify_new_qty[24],
                        sendbuf->modify_line_num[25],
            sendbuf->modify_new_qty[25],
                        sendbuf->modify_line_num[26],
            sendbuf->modify_new_qty[26],
                        sendbuf->modify_line_num[27],
            sendbuf->modify_new_qty[27],
                        sendbuf->modify_line_num[28],
            sendbuf->modify_new_qty[28],
                        sendbuf->modify_line_num[29],
            sendbuf->modify_new_qty[29],
            sendbuf->modify_line_num[30],
            sendbuf->modify_new_qty[30],
                        sendbuf->modify_line_num[31],
            sendbuf->modify_new_qty[31],
                        sendbuf->modify_line_num[32],
            sendbuf->modify_new_qty[32],
                        sendbuf->modify_line_num[33],
            sendbuf->modify_new_qty[33],
                        sendbuf->modify_line_num[34],
            sendbuf->modify_new_qty[34],
                        sendbuf->modify_line_num[35],
            sendbuf->modify_new_qty[35],
                        sendbuf->modify_line_num[36],
            sendbuf->modify_new_qty[36],
                        sendbuf->modify_line_num[37],
            sendbuf->modify_new_qty[37],
                        sendbuf->modify_line_num[38],
            sendbuf->modify_new_qty[38],
                        sendbuf->modify_line_num[39],
            sendbuf->modify_new_qty[39],
            sendbuf->modify_line_num[40],
            sendbuf->modify_new_qty[40],
                    sendbuf->modify_line_num[41],
            sendbuf->modify_new_qty[41],
                        sendbuf->modify_line_num[42],
            sendbuf->modify_new_qty[42],
                        sendbuf->modify_line_num[43],
            sendbuf->modify_new_qty[43],
                        sendbuf->modify_line_num[44],
            sendbuf->modify_new_qty[44],
                        sendbuf->modify_line_num[45],
            sendbuf->modify_new_qty[45],
                        sendbuf->modify_line_num[46],
            sendbuf->modify_new_qty[46],
                        sendbuf->modify_line_num[47],
            sendbuf->modify_new_qty[47],
                        sendbuf->modify_line_num[48],
            sendbuf->modify_new_qty[48],
                        sendbuf->modify_line_num[49],
            sendbuf->modify_new_qty[49]
                        );

// BEGIN - RAVI Debug

//fp = fopen("/tmp/xml_debug.log", "w");
//fprintf(fp, "request_id = %s\n", sendbuf->request_id);
//fprintf(fp, "record_id = %s\n", sendbuf->record_id);
//fprintf(fp, "company = %s\n", sendbuf->company);
//fprintf(fp, "division = %s\n", sendbuf->division);
//fprintf(fp, "userid = %s\n", sendbuf->userid);
//fprintf(fp, "ip_address = %s\n", sendbuf->ip_address);
//fprintf(fp, "filler = %s\n", sendbuf->filler);
//fprintf(fp, "page_no = %s\n", sendbuf->page_no);
//fprintf(fp, "line_1 = %s\n", sendbuf->modify_line_num[0]);
//fprintf(fp, "qty_1 = %s\n", sendbuf->modify_new_qty[0]);
//fprintf(fp, "line_20 = %s\n", sendbuf->modify_line_num[19]);
//fprintf(fp, "qty_20 = %s\n", sendbuf->modify_new_qty[19]);
//fprintf(fp, "line_22 = %s\n", sendbuf->modify_line_num[21]);
//fprintf(fp, "qty_22 = %s\n", sendbuf->modify_new_qty[21]);
//fprintf(fp, "line_50 = %s\n", sendbuf->modify_line_num[49]);
//fprintf(fp, "qty_50 = %s\n", sendbuf->modify_new_qty[49]);
//fprintf(fp, "SENDBUFCAT SIZE = %d\n", strlen(sendbufcat));
//
//fclose(fp);

// End - RAVI Debug


    return(gbp->j);
}
    

int tt0025_ParceRecvStr(tt0025_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0026.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0026 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0026.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0026 %s\"tt0026\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

        memset(ptr_recvbuf->request_id,'\0', tt0025_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0025_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0025_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0025_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0025_REC_ID_LEN );
    gbp->count += tt0025_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0025_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0025_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0025_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0025_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0025_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0025_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0025_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0025_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0025_ERR_LEN;

        gbp->count += tt0025_SEND_FILLER_LEN;


    memset(ptr_recvbuf->cust_edp, '\0', tt0025_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0025_EDP_LEN );
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0025_EDP_LEN;

    memset(ptr_recvbuf->source_code, '\0', tt0025_SRC_LEN + 1);
    memcpy(ptr_recvbuf->source_code, sz_recvbufcat + gbp->count, tt0025_SRC_LEN );
        ap_rprintf(r,"		<CART_SOURCE_CODE>%s</CART_SOURCE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->source_code));
    gbp->count += tt0025_SRC_LEN;

    memset(ptr_recvbuf->item_count, '\0', tt0025_ITM_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->item_count, sz_recvbufcat + gbp->count, tt0025_ITM_COUNT_LEN);
        ap_rprintf(r,"		<PROD_COUNT>%s</PROD_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->item_count));
    gbp->count += tt0025_ITM_COUNT_LEN;
        
    memset(ptr_recvbuf->page_no, '\0', tt0025_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, tt0025_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NO>%s</PAGE_NO>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += tt0025_PAGE_NO_LEN;
        
    memset(ptr_recvbuf->page_count, '\0', tt0025_PAGE_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->page_count, sz_recvbufcat + gbp->count, tt0025_PAGE_COUNT_LEN);
        ap_rprintf(r,"		<PAGE_COUNT>%s</PAGE_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->page_count));
    gbp->count += tt0025_PAGE_COUNT_LEN;
        
    memset(ptr_recvbuf->cart_total, '\0', tt0025_CART_TOTAL_LEN + 1);
    memcpy(ptr_recvbuf->cart_total, sz_recvbufcat + gbp->count, tt0025_CART_TOTAL_LEN);
        ap_rprintf(r,"		<CART_TOTAL>%s</CART_TOTAL>\n", handle_special_chars(gbp,ptr_recvbuf->cart_total));
    gbp->count += tt0025_CART_TOTAL_LEN;
        
        gbp->j = atoi(ptr_recvbuf->item_count);
        if(gbp->j > (int)tt0025_ITEM_COUNT)
                gbp->j = (int)tt0025_ITEM_COUNT;

    for(gbp->i = 0 ; gbp->i < tt0025_ITEM_COUNT ; gbp->i++) 
    {
        memset(ptr_recvbuf->item[gbp->i].item_edp, '\0', tt0025_ITM_EDP_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_edp, sz_recvbufcat + gbp->count, tt0025_ITM_EDP_LEN);
        gbp->count += tt0025_ITM_EDP_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_num, '\0', tt0025_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_num, sz_recvbufcat+gbp->count, tt0025_ITM_NO_LEN);
        gbp->count += tt0025_ITM_NO_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_qty, '\0', tt0025_ITM_QTY_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_qty, sz_recvbufcat+gbp->count, tt0025_ITM_QTY_LEN);
        gbp->count += tt0025_ITM_QTY_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_desc, '\0', tt0025_ITM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_desc, sz_recvbufcat+gbp->count, tt0025_ITM_DESC_LEN);
                gbp->count += tt0025_ITM_DESC_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_customization_flag, '\0', tt0025_ITM_CUSTOM_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_customization_flag, sz_recvbufcat+gbp->count, tt0025_ITM_CUSTOM_FLAG_LEN);
                gbp->count += tt0025_ITM_CUSTOM_FLAG_LEN;

        memset(ptr_recvbuf->item[gbp->i].item_price, '\0', tt0025_ITM_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_price, sz_recvbufcat+gbp->count, tt0025_ITM_PRICE_LEN);
        gbp->count += tt0025_ITM_PRICE_LEN;

                memset(ptr_recvbuf->item[gbp->i].item_avail_inv , '\0', tt0025_ITM_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].item_avail_inv, sz_recvbufcat + gbp->count, tt0025_ITM_AVAIL_INV_LEN);
        gbp->count += tt0025_ITM_AVAIL_INV_LEN;

        memset(ptr_recvbuf->item[gbp->i].shipto_num, '\0', tt0025_SHIPTO_NO_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].shipto_num, sz_recvbufcat + gbp->count, tt0025_SHIPTO_NO_LEN);
        gbp->count += tt0025_SHIPTO_NO_LEN;

                memset(ptr_recvbuf->item[gbp->i].shipto_fname, '\0', tt0025_SHIPTO_FN_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].shipto_fname, sz_recvbufcat + gbp->count, tt0025_SHIPTO_FN_LEN);
        gbp->count += tt0025_SHIPTO_FN_LEN;

                memset(ptr_recvbuf->item[gbp->i].shipto_lname, '\0', tt0025_SHIPTO_LN_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].shipto_lname, sz_recvbufcat + gbp->count, tt0025_SHIPTO_LN_LEN);
        gbp->count += tt0025_SHIPTO_LN_LEN;

                memset(ptr_recvbuf->item[gbp->i].stand_ord_item, '\0', tt0025_STAND_ORD_ITEM_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].stand_ord_item, sz_recvbufcat + gbp->count, tt0025_STAND_ORD_ITEM_LEN);
        gbp->count += tt0025_STAND_ORD_ITEM_LEN;

                memset(ptr_recvbuf->item[gbp->i].stand_ord_date, '\0', tt0025_STAND_ORD_DATE_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].stand_ord_date, sz_recvbufcat + gbp->count, tt0025_STAND_ORD_DATE_LEN);
        gbp->count += tt0025_STAND_ORD_DATE_LEN;

                memset(ptr_recvbuf->item[gbp->i].gwrap_flag, '\0', tt0013_GWRAP_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].gwrap_flag, sz_recvbufcat + gbp->count, tt0025_GWRAP_FLAG_LEN);
        gbp->count += tt0025_GWRAP_FLAG_LEN;

                memset(ptr_recvbuf->item[gbp->i].gwrap_avail_inv, '\0', tt0013_GWRAP_AVAIL_INV_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].gwrap_avail_inv, sz_recvbufcat + gbp->count, tt0025_GWRAP_AVAIL_INV_LEN);
        gbp->count += tt0025_GWRAP_AVAIL_INV_LEN;

                memset(ptr_recvbuf->item[gbp->i].frame_qty, '\0', tt0025_FRM_QTY_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].frame_qty, sz_recvbufcat + gbp->count, tt0025_FRM_QTY_LEN);
        gbp->count += tt0025_FRM_QTY_LEN;
    
                memset(ptr_recvbuf->item[gbp->i].price_ovrd_flag, '\0', tt0025_PRICE_OVRD_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->item[gbp->i].price_ovrd_flag, sz_recvbufcat + gbp->count, tt0025_PRICE_OVRD_FLAG_LEN);
        gbp->count += tt0025_PRICE_OVRD_FLAG_LEN;
        }

/* ... AND NOW WE OUTPUT THE ITEM DETAIL RECORDS */

    for(gbp->i = 0 ; gbp->i < gbp->j ; gbp->i++) 
    {
                ap_rprintf(r,"		<PRODUCT_DETAIL>\n");
                ap_rprintf(r,"			<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_edp));
                ap_rprintf(r,"			<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_num));
                ap_rprintf(r,"			<SALEABLE_QTY>%s</SALEABLE_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_qty));
                ap_rprintf(r,"			<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n",     handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_desc));         
                ap_rprintf(r,"			<PROD_CUSTOMIZATION_FLAG>%s</PROD_CUSTOMIZATION_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_customization_flag));
                ap_rprintf(r,"			<PROD_PRICE>%s</PROD_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_price));
                ap_rprintf(r,"			<PROD_AVAIL_INV>%s</PROD_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_avail_inv));
                ap_rprintf(r,"			<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shipto_num));
                ap_rprintf(r,"			<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shipto_fname));
                ap_rprintf(r,"			<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].shipto_lname));
                ap_rprintf(r,"			<STAND_ORD_ITEM>%s</STAND_ORD_ITEM>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].stand_ord_item));
                ap_rprintf(r,"			<STAND_ORD_DATE>%s</STAND_ORD_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].stand_ord_date));
                ap_rprintf(r,"			<GWRAP_FLAG>%s</GWRAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].gwrap_flag));
                ap_rprintf(r,"			<GWRAP_AVAIL_INV>%s</GWRAP_AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].gwrap_avail_inv));
                ap_rprintf(r,"			<FRAME_QTY>%s</FRAME_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].frame_qty));
                ap_rprintf(r,"			<PRICE_OVRD_FLAG>%s</PRICE_OVRD_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].price_ovrd_flag));

                ap_rprintf(r,"		</PRODUCT_DETAIL>\n");
    }

        memset(ptr_recvbuf->gc_coup_flag, '\0', tt0025_GC_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->gc_coup_flag, sz_recvbufcat + gbp->count, tt0025_GC_FLAG_LEN);
        ap_rprintf(r,"		<GIFTCERT_COUPON_FLAG>%s</GIFTCERT_COUPON_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gc_coup_flag));
    gbp->count += tt0025_GC_FLAG_LEN;

        memset(ptr_recvbuf->stand_ord_cust, '\0', tt0025_STAND_ORD_CUST_LEN + 1);
    memcpy(ptr_recvbuf->stand_ord_cust, sz_recvbufcat + gbp->count, tt0025_STAND_ORD_CUST_LEN);
        ap_rprintf(r,"		<STAND_ORD_CUST>%s</STAND_ORD_CUST>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_cust));
    gbp->count += tt0025_STAND_ORD_CUST_LEN;

        memset(ptr_recvbuf->return_cust_flag, '\0', tt0025_RETURN_CUST_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->return_cust_flag, sz_recvbufcat + gbp->count, tt0025_RETURN_CUST_FLAG_LEN);
        ap_rprintf(r,"		<RETURN_CUST_FLAG>%s</RETURN_CUST_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->return_cust_flag));
    gbp->count += tt0025_RETURN_CUST_FLAG_LEN;

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0026>\n", gbp->tt_betag);

    return(0);
}




