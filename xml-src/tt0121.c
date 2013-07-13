/* tt0121.c  AR-Data */

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


int     tt0121_CatSendStr(struct global_struct *, char *, tt0121_st_send *);
int     tt0121_ParceRecvStr(tt0121_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int     tt0121_lt_process(request_rec *, struct global_struct *, char *);

int tt0121_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}


int tt0121_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0121_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0121_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0121 = (tt0121_st_send *)malloc(sizeof(tt0121_st_send));
        gbp->recvbuf0121 = (tt0121_st_recv *)malloc(sizeof(tt0121_st_recv));

        gbp->sendbufcat = malloc(tt0121_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0121_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0121_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0121_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0121,  '\0',  sizeof(tt0121_st_send));

    strcpy(gbp->sendbuf0121->request_id, "XML");
    strcpy(gbp->sendbuf0121->record_id, "0121");

        get_tag_data("COMPANY", gbp->sendbuf0121->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0121->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0121->userid,gbp,stdout_buffer);

        strcpy(gbp->sendbuf0121->ip_address, r->connection->remote_ip);

        get_tag_data("OA_TRANS_FLAG", gbp->sendbuf0121->oa_trans_flag,gbp,stdout_buffer);
        get_tag_data("DB_FULL_ORDER_NO", gbp->sendbuf0121->db_full_order_no,gbp,stdout_buffer);
        get_tag_data("ENCRYPT_CUST_EDP", gbp->sendbuf0121->encrypt_cust_edp,gbp,stdout_buffer);
        get_tag_data("PAGE_NUM", gbp->sendbuf0121->page_num,gbp,stdout_buffer);
        

        if(tt0121_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0121) == tt0121_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0121);
                        free (gbp->recvbuf0121);
                        return(-1);
                }
//ap_rprintf(r,"HOST %s PORT %s SENDBUF %s",gbp->hphost, gbp->webport,gbp->sendbufcat); 

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0121_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0121);
                        free (gbp->recvbuf0121);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0121_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0121);
                        free (gbp->recvbuf0121);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0121);
                        free (gbp->recvbuf0121);
                        return(-1);
                }

/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);
//ap_rprintf(r,"RECVBUF %s",gbp->recvbufcat); 

                tt0121_ParceRecvStr(gbp->recvbuf0121, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0121);
                free (gbp->recvbuf0121);

        }
        else
        {
                XML_Error("tt0121_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0121);
                free (gbp->recvbuf0121);
                return(-1);
        }

    return(0);
}


int tt0121_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0121_st_send *ptr_sendbuf)
{

//    FILE *fp;

    gbp->j = sprintf(sz_sendbufcat,
        "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-12.12s%-17.17s%-4.4s",
        ptr_sendbuf->request_id,
        ptr_sendbuf->record_id,
        ptr_sendbuf->company,
        ptr_sendbuf->division,
        ptr_sendbuf->userid,
        ptr_sendbuf->ip_address,
                ptr_sendbuf->filler,
        ptr_sendbuf->oa_trans_flag,
        ptr_sendbuf->db_full_order_no,
        ptr_sendbuf->encrypt_cust_edp,
        ptr_sendbuf->page_num);

// Begin - RAVI Debug

//    fp = fopen("/tmp/xml_debug.log", "w");

//    fprintf(fp, "request_id = %s\n", ptr_sendbuf->request_id);
//    fprintf(fp, "record_id = %s\n", ptr_sendbuf->record_id);
//    fprintf(fp, "company = %s\n", ptr_sendbuf->company);
//    fprintf(fp, "division = %s\n", ptr_sendbuf->division);
//    fprintf(fp, "userid = %s\n", ptr_sendbuf->userid);
//    fprintf(fp, "ip_address = %s\n", ptr_sendbuf->ip_address);
//    fprintf(fp, "filler = %s\n", ptr_sendbuf->filler);
//    fprintf(fp, "oa_trans_flag = %s\n", ptr_sendbuf->oa_trans_flag);
//    fprintf(fp, "cust_edp = %s\n", ptr_sendbuf->cust_edp);
//    fprintf(fp, "page_num = %s\n", ptr_sendbuf->page_num);

//    fclose(fp);

// End - RAVI Debug

    return(gbp->j);
}


int tt0121_ParceRecvStr(tt0121_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    /* Place the individual variables into target fields */
//ap_rprintf(r,"RECVBUF %s",sz_recvbufcat);
        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);
#ifdef XSL
        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0122.xsl\"?>\n");
#endif
/*              ap_rprintf(r,"<!DOCTYPE %s0122 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);
        ap_rprintf(r,"                  \"%s%s0122.dtd\">\n\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0122 %s\"tt0122\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n\n", gbp->mtag);

        memset(ptr_recvbuf->request_id,'\0', tt0121_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0121_REQ_ID_LEN);
    gbp->count += tt0121_REQ_ID_LEN;//ap_rprintf(r,"reqid=%s",ptr_recvbuf->request_id);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n\n", handle_special_chars(gbp,ptr_recvbuf->request_id));

        memset(ptr_recvbuf->record_id,'\0', tt0121_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0121_REC_ID_LEN );
    gbp->count += tt0121_REC_ID_LEN;

        memset(ptr_recvbuf->userid,'\0', tt0121_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0121_USER_ID_LEN );
    gbp->count += tt0121_USER_ID_LEN;
        ap_rprintf(r,"		<UID>%s</UID>\n\n", handle_special_chars(gbp,ptr_recvbuf->userid));

    memset(ptr_recvbuf->success_flag,'\0', tt0121_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, tt0121_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += tt0121_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0121_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0121_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0121_ERR_LEN;

        gbp->count += tt0121_SEND_FILLER_LEN;

        memset(ptr_recvbuf->oa_trans_flag,'\0', tt0121_OA_TRANS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->oa_trans_flag, sz_recvbufcat + gbp->count, tt0121_OA_TRANS_FLAG_LEN);
    gbp->count += tt0121_OA_TRANS_FLAG_LEN;
        ap_rprintf(r,"		<OA_TRANS_FLAG>%s</OA_TRANS_FLAG>\n\n", handle_special_chars(gbp,ptr_recvbuf->oa_trans_flag));

        memset(ptr_recvbuf->oa_curr_page,'\0', tt0121_OA_CURR_PAGE_LEN+1);
    memcpy(ptr_recvbuf->oa_curr_page, sz_recvbufcat + gbp->count, tt0121_OA_CURR_PAGE_LEN);
    gbp->count += tt0121_OA_CURR_PAGE_LEN;
        ap_rprintf(r,"		<OA_CURR_PAGE>%s</OA_CURR_PAGE>\n\n", handle_special_chars(gbp,ptr_recvbuf->oa_curr_page));

    memset(ptr_recvbuf->rec_count, '\0', tt0121_REC_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->rec_count, sz_recvbufcat + gbp->count, tt0121_REC_COUNT_LEN );
    ap_rprintf(r,"      <REC_COUNT>%s</REC_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->rec_count));
    gbp->count += tt0121_REC_COUNT_LEN;

    memset(ptr_recvbuf->num_appl_creds, '\0', tt0121_NUM_APPL_CREDS_LEN + 1);
    memcpy(ptr_recvbuf->num_appl_creds, sz_recvbufcat + gbp->count, tt0121_NUM_APPL_CREDS_LEN );
    ap_rprintf(r,"      <NUM_APPL_CREDS>%s</NUM_APPL_CREDS>\n", handle_special_chars(gbp,ptr_recvbuf->num_appl_creds));
    gbp->count += tt0121_NUM_APPL_CREDS_LEN;

    memset(ptr_recvbuf->tot_appl_creds, '\0', tt0121_TOT_APPL_CREDS_LEN + 1);
    memcpy(ptr_recvbuf->tot_appl_creds, sz_recvbufcat + gbp->count, tt0121_TOT_APPL_CREDS_LEN );
    ap_rprintf(r,"      <TOT_APPL_CREDS>%s</TOT_APPL_CREDS>\n", handle_special_chars(gbp,ptr_recvbuf->tot_appl_creds));
    gbp->count += tt0121_TOT_APPL_CREDS_LEN;

    memset(ptr_recvbuf->currency_symbol, '\0', tt0121_CURRENCY_SYMBOL_LEN + 1);
    memcpy(ptr_recvbuf->currency_symbol, sz_recvbufcat + gbp->count, tt0121_CURRENCY_SYMBOL_LEN );
    ap_rprintf(r,"      <CURRENCY_SYMBOL>%s</CURRENCY_SYMBOL>\n", handle_special_chars(gbp,ptr_recvbuf->currency_symbol));
    gbp->count += tt0121_CURRENCY_SYMBOL_LEN;

    memset(ptr_recvbuf->encrypt_cust_edp, '\0', tt0121_ENCRYPT_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->encrypt_cust_edp, sz_recvbufcat + gbp->count, tt0121_ENCRYPT_CUST_EDP_LEN );
    ap_rprintf(r,"      <ENCRYPT_CUST_EDP>%s</ENCRYPT_CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->encrypt_cust_edp));
    gbp->count += tt0121_ENCRYPT_CUST_EDP_LEN;

    memset(ptr_recvbuf->cust_num, '\0', tt0121_CUST_NUM_LEN + 1);
    memcpy(ptr_recvbuf->cust_num, sz_recvbufcat + gbp->count, tt0121_CUST_NUM_LEN );
    ap_rprintf(r,"      <CUST_NUM>%s</CUST_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->cust_num));
    gbp->count += tt0121_CUST_NUM_LEN;

    memset(ptr_recvbuf->fname, '\0', tt0121_FNAME_LEN + 1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0121_FNAME_LEN );
    ap_rprintf(r,"      <FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += tt0121_FNAME_LEN;

    memset(ptr_recvbuf->mid_init, '\0', tt0121_MID_INIT_LEN + 1);
    memcpy(ptr_recvbuf->mid_init, sz_recvbufcat + gbp->count, tt0121_MID_INIT_LEN );
    ap_rprintf(r,"      <MID_INIT>%s</MID_INIT>\n", handle_special_chars(gbp,ptr_recvbuf->mid_init));
    gbp->count += tt0121_MID_INIT_LEN;

    memset(ptr_recvbuf->lname, '\0', tt0121_LNAME_LEN + 1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0121_LNAME_LEN );
    ap_rprintf(r,"      <LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += tt0121_LNAME_LEN;

    memset(ptr_recvbuf->comp_name, '\0', tt0121_COMP_NAME_LEN + 1);
    memcpy(ptr_recvbuf->comp_name, sz_recvbufcat + gbp->count, tt0121_COMP_NAME_LEN );
    ap_rprintf(r,"      <COMP_NAME>%s</COMP_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->comp_name));
    gbp->count += tt0121_COMP_NAME_LEN;

    memset(ptr_recvbuf->street, '\0', tt0121_STREET_LEN + 1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, tt0121_STREET_LEN );
    ap_rprintf(r,"      <STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));
    gbp->count += tt0121_STREET_LEN;

    memset(ptr_recvbuf->ref1, '\0', tt0121_REF1_LEN + 1);
    memcpy(ptr_recvbuf->ref1, sz_recvbufcat + gbp->count, tt0121_REF1_LEN );
    ap_rprintf(r,"      <REF1>%s</REF1>\n", handle_special_chars(gbp,ptr_recvbuf->ref1));
    gbp->count += tt0121_REF1_LEN;

    memset(ptr_recvbuf->ref2, '\0', tt0121_REF2_LEN + 1);
    memcpy(ptr_recvbuf->ref2, sz_recvbufcat + gbp->count, tt0121_REF2_LEN );
    ap_rprintf(r,"      <REF2>%s</REF2>\n", handle_special_chars(gbp,ptr_recvbuf->ref2));
    gbp->count += tt0121_REF2_LEN;

    memset(ptr_recvbuf->city, '\0', tt0121_CITY_LEN + 1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, tt0121_CITY_LEN );
    ap_rprintf(r,"      <CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));
    gbp->count += tt0121_CITY_LEN;

    memset(ptr_recvbuf->state, '\0', tt0121_STATE_LEN + 1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, tt0121_STATE_LEN );
    ap_rprintf(r,"      <STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));
        gbp->count += tt0121_STATE_LEN;

    memset(ptr_recvbuf->zipcode, '\0', tt0121_ZIPCODE_LEN + 1);
    memcpy(ptr_recvbuf->zipcode, sz_recvbufcat + gbp->count, tt0121_ZIPCODE_LEN);
    ap_rprintf(r,"      <ZIPCODE>%s</ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->zipcode));
    gbp->count += tt0121_ZIPCODE_LEN;

    memset(ptr_recvbuf->db_full_order_no, '\0', tt0121_DB_FULL_ORDER_NO_LEN + 1);
    memcpy(ptr_recvbuf->db_full_order_no, sz_recvbufcat + gbp->count, tt0121_DB_FULL_ORDER_NO_LEN );
    ap_rprintf(r,"      <tt0121_DB_FULL_ORDER_NO>%s</tt0121_DB_FULL_ORDER_NO>\n", handle_special_chars(gbp,ptr_recvbuf->db_full_order_no));
    gbp->count += tt0121_DB_FULL_ORDER_NO_LEN;

    memset(ptr_recvbuf->db_company, '\0', tt0121_DB_COMPANY_LEN + 1);
    memcpy(ptr_recvbuf->db_company, sz_recvbufcat + gbp->count, tt0121_DB_COMPANY_LEN );
    ap_rprintf(r,"      <COMP_NAME>%s</COMP_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->db_company));
    gbp->count += tt0121_DB_COMPANY_LEN;

    memset(ptr_recvbuf->db_division, '\0', tt0121_DB_DIVISION_LEN + 1);
    memcpy(ptr_recvbuf->db_division, sz_recvbufcat + gbp->count, tt0121_DB_DIVISION_LEN );
    ap_rprintf(r,"      <DB_DIVISION>%s</DB_DIVISION>\n", handle_special_chars(gbp,ptr_recvbuf->db_division));
    gbp->count += tt0121_DB_DIVISION_LEN;

    memset(ptr_recvbuf->db_tot_dol, '\0', tt0121_DB_TOT_DOL_LEN + 1);
    memcpy(ptr_recvbuf->db_tot_dol, sz_recvbufcat + gbp->count, tt0121_DB_TOT_DOL_LEN );
    ap_rprintf(r,"      <DB_TOT_DOL>%s</DB_TOT_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->db_tot_dol));
    gbp->count += tt0121_DB_TOT_DOL_LEN;

    memset(ptr_recvbuf->db_open_dol, '\0', tt0121_DB_OPEN_DOL_LEN + 1);
    memcpy(ptr_recvbuf->db_open_dol, sz_recvbufcat + gbp->count, tt0121_DB_OPEN_DOL_LEN );
    ap_rprintf(r,"      <DB_OPEN_DOL>%s</DB_OPEN_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->db_open_dol));
    gbp->count += tt0121_DB_OPEN_DOL_LEN;

    memset(ptr_recvbuf->db_trans_date, '\0', tt0121_DB_TRANS_DATE_LEN + 1);
    memcpy(ptr_recvbuf->db_trans_date, sz_recvbufcat + gbp->count, tt0121_DB_TRANS_DATE_LEN );
    ap_rprintf(r,"      <DB_TRANS_DATE>%s</DB_TRANS_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->db_trans_date));
    gbp->count += tt0121_DB_TRANS_DATE_LEN;

    memset(ptr_recvbuf->db_po_number, '\0', tt0121_DB_PO_NUMBER_LEN + 1);
    memcpy(ptr_recvbuf->db_po_number, sz_recvbufcat + gbp->count, tt0121_DB_PO_NUMBER_LEN );
    ap_rprintf(r,"      <DB_PO_NUMBER>%s</DB_PO_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->db_po_number));
    gbp->count += tt0121_DB_PO_NUMBER_LEN;

    memset(ptr_recvbuf->db_due_date, '\0', tt0121_DB_DUE_DATE_LEN + 1);
    memcpy(ptr_recvbuf->db_due_date, sz_recvbufcat + gbp->count, tt0121_DB_DUE_DATE_LEN );
    ap_rprintf(r,"      <DB_DUE_DATE>%s</DB_DUE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->db_due_date));
    gbp->count += tt0121_DB_DUE_DATE_LEN;

    memset(ptr_recvbuf->db_type, '\0', tt0121_DB_TYPE_LEN + 1);
    memcpy(ptr_recvbuf->db_type, sz_recvbufcat + gbp->count, tt0121_DB_TYPE_LEN );
    ap_rprintf(r,"      <DB_TYPE>%s</DB_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->db_type));
    gbp->count += tt0121_DB_TYPE_LEN;

    memset(ptr_recvbuf->db_type_text, '\0', tt0121_DB_TYPE_TEXT_LEN + 1);
    memcpy(ptr_recvbuf->db_type_text, sz_recvbufcat + gbp->count, tt0121_DB_TYPE_TEXT_LEN );
    ap_rprintf(r,"      <DB_TYPE_TEXT>%s</DB_TYPE_TEXT>\n", handle_special_chars(gbp,ptr_recvbuf->db_type_text));
    gbp->count += tt0121_DB_TYPE_TEXT_LEN;

    memset(ptr_recvbuf->db_status, '\0', tt0121_DB_STATUS_LEN + 1);
    memcpy(ptr_recvbuf->db_status, sz_recvbufcat + gbp->count, tt0121_DB_STATUS_LEN );
    ap_rprintf(r,"      <DB_STATUS>%s</DB_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->db_status));
    gbp->count += tt0121_DB_STATUS_LEN;

    memset(ptr_recvbuf->db_comment, '\0', tt0121_DB_COMMENT_LEN + 1);
    memcpy(ptr_recvbuf->db_comment, sz_recvbufcat + gbp->count, tt0121_DB_COMMENT_LEN );
    ap_rprintf(r,"      <DB_COMMENT>%s</DB_COMMENT>\n", handle_special_chars(gbp,ptr_recvbuf->db_comment));
    gbp->count += tt0121_DB_COMMENT_LEN;

    memset(ptr_recvbuf->db_comment_type, '\0', tt0121_DB_COMMENT_TYPE_LEN + 1);
    memcpy(ptr_recvbuf->db_comment_type, sz_recvbufcat + gbp->count, tt0121_DB_COMMENT_TYPE_LEN );
    ap_rprintf(r,"      <DB_COMMENT_TYPE>%s</DB_COMMENT_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->db_comment_type));
    gbp->count += tt0121_DB_COMMENT_TYPE_LEN;

    memset(ptr_recvbuf->db_comment_date, '\0', tt0121_DB_COMMENT_DATE_LEN + 1);
    memcpy(ptr_recvbuf->db_comment_date, sz_recvbufcat + gbp->count, tt0121_DB_COMMENT_DATE_LEN );
    ap_rprintf(r,"      <DB_COMMENT_DATE>%s</DB_COMMENT_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->db_comment_date));
    gbp->count += tt0121_DB_COMMENT_DATE_LEN;


        // Build data from array 

    for(gbp->i = 0; gbp->i < atoi(ptr_recvbuf->rec_count); gbp->i++) 
    {
                memset(ptr_recvbuf->cr_data[gbp->i].cr_company,'\0',tt0121_CR_COMPANY_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_company, sz_recvbufcat + gbp->count, tt0121_CR_COMPANY_LEN);
        gbp->count += tt0121_CR_COMPANY_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_division,'\0',tt0121_CR_DIVISION_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_division, sz_recvbufcat + gbp->count, tt0121_CR_DIVISION_LEN);
        gbp->count += tt0121_CR_DIVISION_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_trans_date,'\0',tt0121_CR_TRANS_DATE_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_trans_date, sz_recvbufcat + gbp->count, tt0121_CR_TRANS_DATE_LEN);
        gbp->count += tt0121_CR_TRANS_DATE_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_orig_ref,'\0',tt0121_CR_ORIG_REF_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_orig_ref, sz_recvbufcat + gbp->count, tt0121_CR_ORIG_REF_LEN);
        gbp->count += tt0121_CR_ORIG_REF_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_tot_dol,'\0',tt0121_CR_TOT_DOL_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_tot_dol, sz_recvbufcat + gbp->count, tt0121_CR_TOT_DOL_LEN);
        gbp->count += tt0121_CR_TOT_DOL_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_post_ref,'\0',tt0121_CR_POST_REF_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_post_ref, sz_recvbufcat + gbp->count, tt0121_CR_POST_REF_LEN);
        gbp->count += tt0121_CR_POST_REF_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_due_date,'\0',tt0121_CR_DUE_DATE_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_due_date, sz_recvbufcat + gbp->count, tt0121_CR_DUE_DATE_LEN);
        gbp->count += tt0121_CR_DUE_DATE_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_type,'\0',tt0121_CR_TYPE_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_type, sz_recvbufcat + gbp->count, tt0121_CR_TYPE_LEN);
        gbp->count += tt0121_CR_TYPE_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_type_text,'\0',tt0121_CR_TYPE_TEXT_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_type_text, sz_recvbufcat + gbp->count, tt0121_CR_TYPE_TEXT_LEN);
        gbp->count += tt0121_CR_TYPE_TEXT_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_status,'\0',tt0121_CR_STATUS_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_status, sz_recvbufcat + gbp->count, tt0121_CR_STATUS_LEN);
        gbp->count += tt0121_CR_STATUS_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_comment,'\0',tt0121_CR_COMMENT_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_comment, sz_recvbufcat + gbp->count, tt0121_CR_COMMENT_LEN);
        gbp->count += tt0121_CR_COMMENT_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_comment_type,'\0',tt0121_CR_COMMENT_TYPE_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_comment_type, sz_recvbufcat + gbp->count, tt0121_CR_COMMENT_TYPE_LEN);
        gbp->count += tt0121_CR_COMMENT_TYPE_LEN;

                memset(ptr_recvbuf->cr_data[gbp->i].cr_comment_date,'\0',tt0121_CR_COMMENT_DATE_LEN+1);
        memcpy(ptr_recvbuf->cr_data[gbp->i].cr_comment_date, sz_recvbufcat + gbp->count, tt0121_CR_COMMENT_DATE_LEN);
        gbp->count += tt0121_CR_COMMENT_DATE_LEN;

        }


    for(gbp->i = 0; gbp->i < atoi(ptr_recvbuf->rec_count); gbp->i++) 
    {
                ap_rprintf(r,"		<CR_DATA>\n\n");
                ap_rprintf(r,"			<CR_COMPANY>%s</CR_COMPANY>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_company));
                ap_rprintf(r,"			<CR_DIVISION>%s</CR_DIVISION>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_division));
                ap_rprintf(r,"			<CR_TRANS_DATE>%s</CR_TRANS_DATE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_trans_date));
                ap_rprintf(r,"			<CR_ORIG_REF>%s</CR_ORIG_REF>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_orig_ref));
                ap_rprintf(r,"			<CR_TOT_DOL>%s</CR_TOT_DOL>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_tot_dol));
                ap_rprintf(r,"			<CR_POST_REF>%s</CR_POST_REF>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_post_ref));
                ap_rprintf(r,"			<CR_DUE_DATE>%s</CR_DUE_DATE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_due_date));
                ap_rprintf(r,"			<CR_TYPE>%s</CR_TYPE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_type));
                ap_rprintf(r,"			<CR_TYPE_TEXT>%s</CR_TYPE_TEXT>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_type_text));
                ap_rprintf(r,"			<CR_STATUS>%s</CR_STATUS>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_status));
                ap_rprintf(r,"			<CR_COMMENT>%s</CR_COMMENT>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_comment));
                ap_rprintf(r,"			<CR_COMMENT_TYPE>%s</CR_COMMENT_TYPE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_comment_type));
                ap_rprintf(r,"			<CR_COMMENT_DATE>%s</CR_COMMENT_DATE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->cr_data[gbp->i].cr_comment_date));
                ap_rprintf(r,"		</CR_DATA>\n\n");
    }

        ap_rprintf(r,"	%s>\n\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n\n", gbp->rsetag);

        ap_rprintf(r,"%s0122>\n\n", gbp->tt_betag);

    return(0);
}


