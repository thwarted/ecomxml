/* tt0129.c Order Review */
/*modified for phase 2 - JL */
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



int             tt0129_CatSendStr(struct global_struct *,char *, tt0129_st_send *);
int             tt0129_ParceRecvStr(tt0129_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0129_lt_process(request_rec *, struct global_struct *, char *);



int tt0129_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */
    

        return(0);
}

int tt0129_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0129_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0129_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0129 = (tt0129_st_send *)malloc(sizeof(tt0129_st_send));
        gbp->recvbuf0129 = (tt0129_st_recv *)malloc(sizeof(tt0129_st_recv));

        gbp->sendbufcat = malloc(tt0129_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0129_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0129_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0129_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0129,  '\0',  sizeof(tt0129_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0129->request_id, "XML");
    strcpy(gbp->sendbuf0129->record_id, "0129");

        strcpy(gbp->sendbuf0129->ip_address, r->connection->remote_ip);
    // Fill the buffers

        get_tag_data("COMPANY", gbp->sendbuf0129->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0129->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0129->userid,gbp,stdout_buffer);
        get_tag_data("ENCRYPT_CUST_EDP", gbp->sendbuf0129->encrypt_cust_edp,gbp,stdout_buffer);
        get_tag_data("ORD_NO",gbp->sendbuf0129->ord_num,gbp,stdout_buffer);
        get_tag_data("ORD_SUFFIX",gbp->sendbuf0129->ord_suffix,gbp,stdout_buffer);

        if((tt0129_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0129)) == tt0129_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0129);
                        free (gbp->recvbuf0129);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0129_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0129);
                        free (gbp->recvbuf0129);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0129_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0129);
                        free (gbp->recvbuf0129);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0129);
                        free (gbp->recvbuf0129);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0129_ParceRecvStr(gbp->recvbuf0129, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0129);
                free (gbp->recvbuf0129);
        }
    else
    {
                XML_Error("tt0129_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0129);
                free (gbp->recvbuf0129);
                return(-1);
    }

    return(0);
}

int tt0129_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0129_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-17.17s%-8.8s%-4.4s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->encrypt_cust_edp,
                        ptr_sendbuf->ord_num,
                        ptr_sendbuf->ord_suffix);

    return(gbp->j);
}


int tt0129_ParceRecvStr(tt0129_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0130.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0130 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0130.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0130 %s\"tt0130\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0129_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0129_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0129_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0129_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0129_REC_ID_LEN);
    gbp->count += tt0129_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0129_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0129_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0129_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0129_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0129_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0129_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0129_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0129_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0129_ERR_LEN;

        gbp->count += tt0129_SEND_FILLER_LEN;


    memset(ptr_recvbuf->encrypt_cust_edp, '\0', tt0129_ENCRYPT_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->encrypt_cust_edp, sz_recvbufcat + gbp->count, tt0129_ENCRYPT_CUST_EDP_LEN);
        ap_rprintf(r,"		<ENCRYPT_CUST_EDP>%s</ENCRYPT_CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->encrypt_cust_edp));
    gbp->count += tt0129_ENCRYPT_CUST_EDP_LEN;

    memset(ptr_recvbuf->ord_num, '\0', tt0129_ORD_NO_LEN + 1);
    memcpy(ptr_recvbuf->ord_num, sz_recvbufcat + gbp->count, tt0129_ORD_NO_LEN);
        ap_rprintf(r,"		<ORD_NUM>%s</ORD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->ord_num));
    gbp->count += tt0129_ORD_NO_LEN;

    memset(ptr_recvbuf->ord_suffix, '\0', tt0129_ORD_SUFFIX_LEN + 1);
    memcpy(ptr_recvbuf->ord_suffix, sz_recvbufcat + gbp->count, tt0129_ORD_SUFFIX_LEN);
        ap_rprintf(r,"		<ORD_SUFFIX>%s</ORD_SUFFIX>\n", handle_special_chars(gbp,ptr_recvbuf->ord_suffix));
    gbp->count += tt0129_ORD_SUFFIX_LEN;

    memset(ptr_recvbuf->prg_name, '\0', tt0129_PRG_NAME_LEN + 1);
    memcpy(ptr_recvbuf->prg_name, sz_recvbufcat + gbp->count, tt0129_PRG_NAME_LEN);
        ap_rprintf(r,"		<PRG_NAME>%s</PRG_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->prg_name));
    gbp->count += tt0129_PRG_NAME_LEN;

    memset(ptr_recvbuf->prg_desc, '\0', tt0129_PRG_DESC_LEN + 1);
    memcpy(ptr_recvbuf->prg_desc, sz_recvbufcat + gbp->count, tt0129_PRG_DESC_LEN);
        ap_rprintf(r,"		<PRG_DESC>%s</PRG_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->prg_desc));
    gbp->count += tt0129_PRG_DESC_LEN;

    memset(ptr_recvbuf->prg_start_date, '\0', tt0129_DATE_LEN + 1);
    memcpy(ptr_recvbuf->prg_start_date, sz_recvbufcat + gbp->count, tt0129_DATE_LEN);
        ap_rprintf(r,"		<PRG_START_DATE>%s</PRG_START_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->prg_start_date));
    gbp->count += tt0129_DATE_LEN;

    memset(ptr_recvbuf->prg_ships, '\0', tt0129_PRG_SHIPS_LEN + 1);
    memcpy(ptr_recvbuf->prg_ships, sz_recvbufcat + gbp->count, tt0129_PRG_SHIPS_LEN);
        ap_rprintf(r,"		<PRG_SHIPS>%s</PRG_SHIPS>\n", handle_special_chars(gbp,ptr_recvbuf->prg_ships));
    gbp->count += tt0129_PRG_SHIPS_LEN;

    memset(ptr_recvbuf->prg_status_code, '\0', tt0129_PRG_STATUS_CODE_LEN + 1);
    memcpy(ptr_recvbuf->prg_status_code, sz_recvbufcat + gbp->count, tt0129_PRG_STATUS_CODE_LEN);
        ap_rprintf(r,"		<PRG_STSTUS_CODE>%s</PRG_STSTUS_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->prg_status_code));
    gbp->count += tt0129_PRG_STATUS_CODE_LEN;

    memset(ptr_recvbuf->prg_status, '\0', tt0129_PRG_STATUS_LEN + 1);
    memcpy(ptr_recvbuf->prg_status, sz_recvbufcat + gbp->count, tt0129_PRG_STATUS_LEN);
        ap_rprintf(r,"		<PRG_STATUS>%s</PRG_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->prg_status));
    gbp->count += tt0129_PRG_STATUS_LEN;

    memset(ptr_recvbuf->prg_bu_fname, '\0', tt0129_FNAME_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_fname, sz_recvbufcat + gbp->count, tt0129_FNAME_LEN);
        ap_rprintf(r,"		<PRG_BUYER_FNAME>%s</PRG_BUYER_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_fname));
        gbp->count += tt0129_FNAME_LEN;

        memset(ptr_recvbuf->prg_bu_lname, '\0', tt0129_LNAME_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_lname, sz_recvbufcat + gbp->count, tt0129_LNAME_LEN);
        ap_rprintf(r,"		<PRG_BUYER_LNAME>%s</PRG_BUYER_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_lname));
        gbp->count += tt0129_LNAME_LEN;

        memset(ptr_recvbuf->prg_bu_company, '\0', tt0129_BU_COMPANY_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_company, sz_recvbufcat + gbp->count, tt0129_BU_COMPANY_LEN);
        ap_rprintf(r,"		<PRG_BUYER_COMPANY>%s</PRG_BUYER_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_company));
        gbp->count += tt0129_BU_COMPANY_LEN;

        memset(ptr_recvbuf->prg_bu_street, '\0', tt0129_STREET_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_street, sz_recvbufcat + gbp->count, tt0129_STREET_LEN);
        ap_rprintf(r,"		<PRG_BUYER_STREET>%s</PRG_BUYER_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_street));
        gbp->count += tt0129_STREET_LEN;

        memset(ptr_recvbuf->prg_bu_city, '\0', tt0129_CITY_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_city, sz_recvbufcat + gbp->count, tt0129_CITY_LEN);
        ap_rprintf(r,"		<PRG_BUYER_CITY>%s</PRG_BUYER_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_city));
        gbp->count += tt0129_CITY_LEN;

        memset(ptr_recvbuf->prg_bu_state, '\0', tt0129_STATE_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_state, sz_recvbufcat + gbp->count, tt0129_STATE_LEN);
        ap_rprintf(r,"		<PRG_BUYER_STATE>%s</PRG_BUYER_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_state));
        gbp->count += tt0129_STATE_LEN;

        memset(ptr_recvbuf->prg_bu_country, '\0', tt0129_COUNTRY_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_country, sz_recvbufcat + gbp->count, tt0129_COUNTRY_LEN);
        ap_rprintf(r,"		<PRG_BUYER_COUNTRY>%s</PRG_BUYER_COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_country));
        gbp->count += tt0129_COUNTRY_LEN;

        memset(ptr_recvbuf->prg_bu_zip, '\0', tt0129_ZIP_LEN + 1);
        memcpy(ptr_recvbuf->prg_bu_zip, sz_recvbufcat + gbp->count, tt0129_ZIP_LEN);
        ap_rprintf(r,"		<PRG_BUYER_ZIP>%s</PRG_BUYER_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->prg_bu_zip));
        gbp->count += tt0129_ZIP_LEN;

    memset(ptr_recvbuf->prg_st_fname, '\0', tt0129_FNAME_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_fname, sz_recvbufcat + gbp->count, tt0129_FNAME_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_FNAME>%s</PRG_SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_fname));
        gbp->count += tt0129_FNAME_LEN;

        memset(ptr_recvbuf->prg_st_lname, '\0', tt0129_LNAME_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_lname, sz_recvbufcat + gbp->count, tt0129_LNAME_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_LNAME>%s</PRG_SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_lname));
        gbp->count += tt0129_LNAME_LEN;

        memset(ptr_recvbuf->prg_st_company, '\0', tt0129_BU_COMPANY_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_company, sz_recvbufcat + gbp->count, tt0129_BU_COMPANY_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_COMPANY>%s</PRG_SHIPTO_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_company));
        gbp->count += tt0129_BU_COMPANY_LEN;

        memset(ptr_recvbuf->prg_st_street, '\0', tt0129_STREET_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_street, sz_recvbufcat + gbp->count, tt0129_STREET_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_STREET>%s</PRG_SHIPTO_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_street));
        gbp->count += tt0129_STREET_LEN;

        memset(ptr_recvbuf->prg_st_city, '\0', tt0129_CITY_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_city, sz_recvbufcat + gbp->count, tt0129_CITY_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_CITY>%s</PRG_SHIPTO_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_city));
        gbp->count += tt0129_CITY_LEN;

        memset(ptr_recvbuf->prg_st_state, '\0', tt0129_STATE_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_state, sz_recvbufcat + gbp->count, tt0129_STATE_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_STATE>%s</PRG_SHIPTO_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_state));
        gbp->count += tt0129_STATE_LEN;

        memset(ptr_recvbuf->prg_st_country, '\0', tt0129_COUNTRY_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_country, sz_recvbufcat + gbp->count, tt0129_COUNTRY_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_COUNTRY>%s</PRG_SHIPTO_COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_country));
        gbp->count += tt0129_COUNTRY_LEN;

        memset(ptr_recvbuf->prg_st_zip, '\0', tt0129_ZIP_LEN + 1);
        memcpy(ptr_recvbuf->prg_st_zip, sz_recvbufcat + gbp->count, tt0129_ZIP_LEN);
        ap_rprintf(r,"		<PRG_SHIPTO_ZIP>%s</PRG_SHIPTO_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->prg_st_zip));
        gbp->count += tt0129_ZIP_LEN;

        memset(ptr_recvbuf->prg_billed, '\0', tt0129_PRG_BILLED_LEN + 1);
        memcpy(ptr_recvbuf->prg_billed, sz_recvbufcat + gbp->count, tt0129_PRG_BILLED_LEN);
        ap_rprintf(r,"		<PRG_BILLED>%s</PRG_BILLED>\n", handle_special_chars(gbp,ptr_recvbuf->prg_billed));
        gbp->count += tt0129_PRG_BILLED_LEN;

        memset(ptr_recvbuf->prg_paid, '\0', tt0129_PRG_PAID_LEN + 1);
        memcpy(ptr_recvbuf->prg_paid, sz_recvbufcat + gbp->count, tt0129_PRG_PAID_LEN);
        ap_rprintf(r,"		<PRG_PAID>%s</PRG_PAID>\n", handle_special_chars(gbp,ptr_recvbuf->prg_paid));
        gbp->count += tt0129_PRG_PAID_LEN;

        memset(ptr_recvbuf->prg_returned, '\0', tt0129_PRG_RETURNED_LEN + 1);
        memcpy(ptr_recvbuf->prg_returned, sz_recvbufcat + gbp->count, tt0129_PRG_RETURNED_LEN);
        ap_rprintf(r,"		<PRG_RETURNED>%s</PRG_RETURNED>\n", handle_special_chars(gbp,ptr_recvbuf->prg_returned));
        gbp->count += tt0129_PRG_RETURNED_LEN;

        memset(ptr_recvbuf->prg_other_paid, '\0', tt0129_PRG_OTHER_PAID_LEN + 1);
        memcpy(ptr_recvbuf->prg_other_paid, sz_recvbufcat + gbp->count, tt0129_PRG_OTHER_PAID_LEN);
        ap_rprintf(r,"		<PRG_OTHER_PAID>%s</PRG_OTHER_PAID>\n", handle_special_chars(gbp,ptr_recvbuf->prg_other_paid));
        gbp->count += tt0129_PRG_OTHER_PAID_LEN;

        memset(ptr_recvbuf->actual_lvl_count, '\0', tt0129_ACTUAL_LVL_COUNT_LEN + 1);
        memcpy(ptr_recvbuf->actual_lvl_count, sz_recvbufcat + gbp->count, tt0129_ACTUAL_LVL_COUNT_LEN);
        gbp->count += tt0129_ACTUAL_LVL_COUNT_LEN;


//      for(gbp->k = 0; gbp->k < tt0129_LEVEL_COUNT; gbp->k++) 

        gbp->j = atoi(ptr_recvbuf->actual_lvl_count);
        if(gbp->j > (int)tt0129_LEVEL_COUNT)
                gbp->j  = (int)tt0129_LEVEL_COUNT;

        for(gbp->k = 0; gbp->k < gbp->j; gbp->k++) 
        {
                memset(ptr_recvbuf->item[gbp->k].lvl_item, '\0', tt0129_LVL_ITEM_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].lvl_item, sz_recvbufcat + gbp->count, tt0129_LVL_ITEM_LEN);
                        gbp->count += tt0129_LVL_ITEM_LEN;

                        memset(ptr_recvbuf->item[gbp->k].lvl_desc, '\0', tt0129_LVL_DESC_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].lvl_desc, sz_recvbufcat + gbp->count, tt0129_LVL_DESC_LEN);
                        gbp->count += tt0129_LVL_DESC_LEN;

                        memset(ptr_recvbuf->item[gbp->k].lvl_status, '\0', tt0129_LVL_STATUS_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].lvl_status, sz_recvbufcat + gbp->count, tt0129_LVL_STATUS_LEN);
                        gbp->count += tt0129_LVL_STATUS_LEN;

                        memset(ptr_recvbuf->item[gbp->k].lvl_date, '\0', tt0129_DATE_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].lvl_date, sz_recvbufcat + gbp->count, tt0129_DATE_LEN);
                        gbp->count += tt0129_DATE_LEN;

                        memset(ptr_recvbuf->item[gbp->k].lvl_collector, '\0', tt0129_LVL_COLLECTOR_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].lvl_collector, sz_recvbufcat + gbp->count, tt0129_LVL_COLLECTOR_LEN);
                        gbp->count += tt0129_LVL_COLLECTOR_LEN;

                        memset(ptr_recvbuf->item[gbp->k].lvl_effort, '\0', tt0129_LVL_EFFORT_LEN + 1);
                        memcpy(ptr_recvbuf->item[gbp->k].lvl_effort, sz_recvbufcat + gbp->count, tt0129_LVL_EFFORT_LEN);
                        gbp->count += tt0129_LVL_EFFORT_LEN;

            memset(ptr_recvbuf->item[gbp->k].lvl_eff_date, '\0', tt0129_DATE_LEN + 1);
            memcpy(ptr_recvbuf->item[gbp->k].lvl_eff_date, sz_recvbufcat + gbp->count, tt0129_DATE_LEN);
            gbp->count += tt0129_DATE_LEN;
                }

//              for(gbp->k = 0; gbp->k < tt0129_LEVEL_COUNT; gbp->k++) 

                gbp->j = atoi(ptr_recvbuf->actual_lvl_count);
                if(gbp->j > (int)tt0129_LEVEL_COUNT)
                        gbp->j  = (int)tt0129_LEVEL_COUNT;

                for(gbp->k = 0; gbp->k < gbp->j; gbp->k++) 
                {
                        ap_rprintf(r,"		<LINE_ITEM_DETAILS>\n");
                        ap_rprintf(r,"			<LINE_ITEM_PROD_NUM>%s</LINE_ITEM_PROD_NUM>\n", 
                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_item));
                        ap_rprintf(r,"			<LINE_ITEM_PROD_DESCRIPTION>%s</LINE_ITEM_PROD_DESCRIPTION>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_desc));
                        ap_rprintf(r,"			<LINE_ITEM_STATUS>%s</LINE_ITEM_STATUS>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_status));
                        ap_rprintf(r,"			<LINE_ITEM_DATE>%s</LINE_ITEM_DATE>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_date));
                        ap_rprintf(r,"			<LINE_ITEM_COLLECTOR>%s</LINE_ITEM_COLLECTOR>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_collector));
                        ap_rprintf(r,"			<LINE_ITEM_EFFORT>%s</LINE_ITEM_EFFORT>\n", 
                                                                                        handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_effort));
                        ap_rprintf(r,"			<LINE_ITEM_EFF_DATE>%s</LINE_ITEM_EFF_DATE>\n",
                                            handle_special_chars(gbp,ptr_recvbuf->item[gbp->k].lvl_eff_date));
                        ap_rprintf(r,"		</LINE_ITEM_DETAILS>\n");
                }


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0130>\n", gbp->tt_betag);

    return 0;
}


