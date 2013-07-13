/* tt0119.c  AR-Data */

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


int     tt0119_CatSendStr(struct global_struct *, char *, tt0119_st_send *);
int     tt0119_ParceRecvStr(tt0119_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int     tt0119_lt_process(request_rec *, struct global_struct *, char *);

int tt0119_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}


int tt0119_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0119_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0119_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0119 = (tt0119_st_send *)malloc(sizeof(tt0119_st_send));
        gbp->recvbuf0119 = (tt0119_st_recv *)malloc(sizeof(tt0119_st_recv));

        gbp->sendbufcat = malloc(tt0119_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0119_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0119_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0119_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0119,  '\0',  sizeof(tt0119_st_send));

    strcpy(gbp->sendbuf0119->request_id, "XML");
    strcpy(gbp->sendbuf0119->record_id, "0119");

        get_tag_data("COMPANY", gbp->sendbuf0119->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0119->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0119->userid,gbp,stdout_buffer);

        strcpy(gbp->sendbuf0119->ip_address, r->connection->remote_ip);

        get_tag_data("OA_TRANS_FLAG", gbp->sendbuf0119->oa_trans_flag,gbp,stdout_buffer);
        get_tag_data("ENCRYPT_CUST_EDP", gbp->sendbuf0119->encrypt_cust_edp,gbp,stdout_buffer);
        get_tag_data("PAGE_NUM", gbp->sendbuf0119->page_num,gbp,stdout_buffer);
        

        if(tt0119_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0119) == tt0119_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0119);
                        free (gbp->recvbuf0119);
                        return(-1);
                }
//ap_rprintf(r,"HOST %s PORT %s SENDBUF %s",gbp->hphost, gbp->webport,gbp->sendbufcat); 

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0119_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0119);
                        free (gbp->recvbuf0119);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0119_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0119);
                        free (gbp->recvbuf0119);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0119);
                        free (gbp->recvbuf0119);
                        return(-1);
                }

/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);
//ap_rprintf(r,"RECVBUF %s",gbp->recvbufcat); 

                tt0119_ParceRecvStr(gbp->recvbuf0119, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0119);
                free (gbp->recvbuf0119);

        }
        else
        {
                XML_Error("tt0119_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0119);
                free (gbp->recvbuf0119);
                return(-1);
        }

    return(0);
}


int tt0119_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0119_st_send *ptr_sendbuf)
{

//    FILE *fp;

    gbp->j = sprintf(sz_sendbufcat,
        "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-17.17s%-4.4s",
        ptr_sendbuf->request_id,
        ptr_sendbuf->record_id,
        ptr_sendbuf->company,
        ptr_sendbuf->division,
        ptr_sendbuf->userid,
        ptr_sendbuf->ip_address,
                ptr_sendbuf->filler,
        ptr_sendbuf->oa_trans_flag,
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


int tt0119_ParceRecvStr(tt0119_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    /* Place the individual variables into target fields */

// Ravi DEBUG 

// ap_rprintf(r,"RECVBUF %s",sz_recvbufcat);

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);
#ifdef XSL
        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0120.xsl\"?>\n");
#endif
/*              ap_rprintf(r,"<!DOCTYPE %s0120 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);
        ap_rprintf(r,"                  \"%s%s0120.dtd\">\n\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0120 %s\"tt0120\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n\n", gbp->mtag);

        memset(ptr_recvbuf->request_id,'\0', tt0119_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0119_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0119_REQ_ID_LEN;

        memset(ptr_recvbuf->record_id,'\0', tt0119_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0119_REC_ID_LEN );
    gbp->count += tt0119_REC_ID_LEN;

        memset(ptr_recvbuf->userid,'\0', tt0119_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0119_USER_ID_LEN );
        ap_rprintf(r,"		<UID>%s</UID>\n\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0119_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', tt0119_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, tt0119_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += tt0119_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0119_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0119_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0119_ERR_LEN;

        gbp->count += tt0119_SEND_FILLER_LEN;

        memset(ptr_recvbuf->oa_trans_flag,'\0', tt0119_OA_TRANS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->oa_trans_flag, sz_recvbufcat + gbp->count, tt0119_OA_TRANS_FLAG_LEN);
    gbp->count += tt0119_OA_TRANS_FLAG_LEN;
        ap_rprintf(r,"		<OA_TRANS_FLAG>%s</OA_TRANS_FLAG>\n\n", handle_special_chars(gbp,ptr_recvbuf->oa_trans_flag));

        memset(ptr_recvbuf->oa_curr_page,'\0', tt0119_OA_CURR_PAGE_LEN+1);
    memcpy(ptr_recvbuf->oa_curr_page, sz_recvbufcat + gbp->count, tt0119_OA_CURR_PAGE_LEN);
    gbp->count += tt0119_OA_CURR_PAGE_LEN;
        ap_rprintf(r,"		<OA_CURR_PAGE>%s</OA_CURR_PAGE>\n\n", handle_special_chars(gbp,ptr_recvbuf->oa_curr_page));

        memset(ptr_recvbuf->num_valid_openar,'\0', tt0119_NUM_VALID_OPENAR_LEN+1);
    memcpy(ptr_recvbuf->num_valid_openar, sz_recvbufcat + gbp->count, tt0119_NUM_VALID_OPENAR_LEN);
    gbp->count += tt0119_NUM_VALID_OPENAR_LEN;
        ap_rprintf(r,"		<OA_REC_COUNT>%s</OA_REC_COUNT>\n\n", handle_special_chars(gbp,ptr_recvbuf->num_valid_openar));

        memset(ptr_recvbuf->currency_symbol,'\0', tt0119_CURRENCY_SYMBOL_LEN+1);
    memcpy(ptr_recvbuf->currency_symbol, sz_recvbufcat + gbp->count, tt0119_CURRENCY_SYMBOL_LEN);
    gbp->count += tt0119_CURRENCY_SYMBOL_LEN;
        ap_rprintf(r,"		<CURRENCY_SYMBOL>%s</CURRENCY_SYMBOL>\n\n", handle_special_chars(gbp,ptr_recvbuf->currency_symbol));

        memset(ptr_recvbuf->encrypt_cust_edp,'\0', tt0119_ENCRYPT_CUST_EDP_LEN+1);
    memcpy(ptr_recvbuf->encrypt_cust_edp, sz_recvbufcat + gbp->count, tt0119_ENCRYPT_CUST_EDP_LEN);
    gbp->count += tt0119_ENCRYPT_CUST_EDP_LEN;
        ap_rprintf(r,"		<ENCRYPT_CUST_EDP>%s</ENCRYPT_CUST_EDP>\n\n", handle_special_chars(gbp,ptr_recvbuf->encrypt_cust_edp));

        memset(ptr_recvbuf->cust_num,'\0', tt0119_CUST_NUM_LEN+1);
    memcpy(ptr_recvbuf->cust_num, sz_recvbufcat + gbp->count, tt0119_CUST_NUM_LEN);
    gbp->count += tt0119_CUST_NUM_LEN;
        ap_rprintf(r,"		<CUST_NUM>%s</CUST_NUM>\n\n", handle_special_chars(gbp,ptr_recvbuf->cust_num));

    memset(ptr_recvbuf->fname, '\0', tt0119_FNAME_LEN + 1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0119_FNAME_LEN );
    gbp->count += tt0119_FNAME_LEN;
    ap_rprintf(r,"      <FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));

    memset(ptr_recvbuf->mid_init, '\0', tt0119_MID_INIT_LEN + 1);
    memcpy(ptr_recvbuf->mid_init, sz_recvbufcat + gbp->count, tt0119_MID_INIT_LEN );
    gbp->count += tt0119_MID_INIT_LEN;
    ap_rprintf(r,"      <MID_INIT>%s</MID_INIT>\n", handle_special_chars(gbp,ptr_recvbuf->mid_init));

    memset(ptr_recvbuf->lname, '\0', tt0119_LNAME_LEN + 1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0119_LNAME_LEN );
    gbp->count += tt0119_LNAME_LEN;
    ap_rprintf(r,"      <LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));

    memset(ptr_recvbuf->comp_name, '\0', tt0119_COMP_NAME_LEN + 1);
    memcpy(ptr_recvbuf->comp_name, sz_recvbufcat + gbp->count, tt0119_COMP_NAME_LEN );
    gbp->count += tt0119_COMP_NAME_LEN;
    ap_rprintf(r,"      <COMP_NAME>%s</COMP_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->comp_name));

    memset(ptr_recvbuf->street, '\0', tt0119_STREET_LEN + 1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, tt0119_STREET_LEN );
    gbp->count += tt0119_STREET_LEN;
    ap_rprintf(r,"      <STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));

    memset(ptr_recvbuf->ref1, '\0', tt0119_REF1_LEN + 1);
    memcpy(ptr_recvbuf->ref1, sz_recvbufcat + gbp->count, tt0119_REF1_LEN );
    gbp->count += tt0119_REF1_LEN;
    ap_rprintf(r,"      <REF1>%s</REF1>\n", handle_special_chars(gbp,ptr_recvbuf->ref1));
    
    memset(ptr_recvbuf->ref2, '\0', tt0119_REF2_LEN + 1);
    memcpy(ptr_recvbuf->ref2, sz_recvbufcat + gbp->count, tt0119_REF2_LEN );
    gbp->count += tt0119_REF2_LEN;
    ap_rprintf(r,"      <REF2>%s</REF2>\n", handle_special_chars(gbp,ptr_recvbuf->ref2));

    memset(ptr_recvbuf->city, '\0', tt0119_CITY_LEN + 1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, tt0119_CITY_LEN );
    gbp->count += tt0119_CITY_LEN;
    ap_rprintf(r,"      <CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));

    memset(ptr_recvbuf->state, '\0', tt0119_STATE_LEN + 1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, tt0119_STATE_LEN );
    gbp->count += tt0119_STATE_LEN;
    ap_rprintf(r,"      <STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));

    memset(ptr_recvbuf->zipcode, '\0', tt0119_ZIPCODE_LEN + 1);
    memcpy(ptr_recvbuf->zipcode, sz_recvbufcat + gbp->count, tt0119_ZIPCODE_LEN );
    gbp->count += tt0119_ZIPCODE_LEN;
    ap_rprintf(r,"      <ZIPCODE>%s</ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->zipcode));

    memset(ptr_recvbuf->tot_avail_cred, '\0', tt0119_TOT_AVAIL_CRED_LEN + 1);
    memcpy(ptr_recvbuf->tot_avail_cred, sz_recvbufcat + gbp->count, tt0119_TOT_AVAIL_CRED_LEN );
    gbp->count += tt0119_TOT_AVAIL_CRED_LEN;
    ap_rprintf(r,"      <TOT_AVAIL_CRED>%s</TOT_AVAIL_CRED>\n", handle_special_chars(gbp,ptr_recvbuf->tot_avail_cred));

    memset(ptr_recvbuf->tot_unapl_cred, '\0', tt0119_TOT_UNAPL_CRED_LEN + 1);
    memcpy(ptr_recvbuf->tot_unapl_cred, sz_recvbufcat + gbp->count, tt0119_TOT_UNAPL_CRED_LEN );
    gbp->count += tt0119_TOT_UNAPL_CRED_LEN;
    ap_rprintf(r,"      <TOT_UNAPL_CRED>%s</TOT_UNAPL_CRED>\n", handle_special_chars(gbp,ptr_recvbuf->tot_unapl_cred));

    memset(ptr_recvbuf->tot_debits, '\0', tt0119_TOT_DEBITS_LEN + 1);
    memcpy(ptr_recvbuf->tot_debits, sz_recvbufcat + gbp->count, tt0119_TOT_DEBITS_LEN );
    gbp->count += tt0119_TOT_DEBITS_LEN;
    ap_rprintf(r,"      <TOT_DEBITS>%s</TOT_DEBITS>\n", handle_special_chars(gbp,ptr_recvbuf->tot_debits));

    memset(ptr_recvbuf->tot_open_debits, '\0', tt0119_TOT_OPEN_DEBITS_LEN + 1);
    memcpy(ptr_recvbuf->tot_open_debits, sz_recvbufcat + gbp->count, tt0119_TOT_OPEN_DEBITS_LEN );
    gbp->count += tt0119_TOT_OPEN_DEBITS_LEN;
    ap_rprintf(r,"      <TOT_OPEN_DEBITS>%s</TOT_OPEN_DEBITS>\n", handle_special_chars(gbp,ptr_recvbuf->tot_open_debits));

    memset(ptr_recvbuf->num_avail_cred, '\0', tt0119_NUM_AVAIL_CRED_LEN + 1);
    memcpy(ptr_recvbuf->num_avail_cred, sz_recvbufcat + gbp->count, tt0119_NUM_AVAIL_CRED_LEN );
    gbp->count += tt0119_NUM_AVAIL_CRED_LEN;
    ap_rprintf(r,"      <NUM_AVAIL_CRED>%s</NUM_AVAIL_CRED>\n", handle_special_chars(gbp,ptr_recvbuf->num_avail_cred));

    memset(ptr_recvbuf->num_unapl_cred, '\0', tt0119_NUM_UNAPL_CRED_LEN + 1);
    memcpy(ptr_recvbuf->num_unapl_cred, sz_recvbufcat + gbp->count, tt0119_NUM_UNAPL_CRED_LEN );
    gbp->count += tt0119_NUM_UNAPL_CRED_LEN;
    ap_rprintf(r,"      <NUM_UNAPL_CRED>%s</NUM_UNAPL_CRED>\n", handle_special_chars(gbp,ptr_recvbuf->num_unapl_cred));

    memset(ptr_recvbuf->num_debits, '\0', tt0119_NUM_DEBITS_LEN + 1);
    memcpy(ptr_recvbuf->num_debits, sz_recvbufcat + gbp->count, tt0119_NUM_DEBITS_LEN );
    gbp->count += tt0119_NUM_DEBITS_LEN;
    ap_rprintf(r,"      <NUM_DEBITS>%s</NUM_DEBITS>\n", handle_special_chars(gbp,ptr_recvbuf->num_debits));


        // Build data from array 

    for(gbp->i = 0; gbp->i < tt0119_AR_DATA_COUNT; gbp->i++) 
    {
                memset(ptr_recvbuf->ar_data[gbp->i].ar_company,'\0',tt0119_AR_COMPANY_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_company, sz_recvbufcat + gbp->count, tt0119_AR_COMPANY_LEN);
        gbp->count += tt0119_AR_COMPANY_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_division,'\0',tt0119_AR_DIVISION_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_division, sz_recvbufcat + gbp->count, tt0119_AR_DIVISION_LEN);
        gbp->count += tt0119_AR_DIVISION_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_trans_date,'\0',tt0119_AR_TRANS_DATE_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_trans_date, sz_recvbufcat + gbp->count, tt0119_AR_TRANS_DATE_LEN);
        gbp->count += tt0119_AR_TRANS_DATE_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_orig_ref,'\0',tt0119_AR_ORIG_REF_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_orig_ref, sz_recvbufcat + gbp->count, tt0119_AR_ORIG_REF_LEN);
        gbp->count += tt0119_AR_ORIG_REF_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_tot_dol,'\0',tt0119_AR_TOT_DOL_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_tot_dol, sz_recvbufcat + gbp->count, tt0119_AR_TOT_DOL_LEN);
        gbp->count += tt0119_AR_TOT_DOL_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_open_dol,'\0',tt0119_AR_OPEN_DOL_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_open_dol, sz_recvbufcat + gbp->count, tt0119_AR_OPEN_DOL_LEN);
        gbp->count += tt0119_AR_OPEN_DOL_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_appl_cr_flag,'\0',tt0119_AR_APPL_CR_FLAG_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_appl_cr_flag, sz_recvbufcat + gbp->count, tt0119_AR_APPL_CR_FLAG_LEN);
        gbp->count += tt0119_AR_APPL_CR_FLAG_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_post_ref,'\0',tt0119_AR_POST_REF_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_post_ref, sz_recvbufcat + gbp->count, tt0119_AR_POST_REF_LEN);
        gbp->count += tt0119_AR_POST_REF_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_po_number,'\0',tt0119_AR_PO_NUMBER_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_po_number, sz_recvbufcat + gbp->count, tt0119_AR_PO_NUMBER_LEN);
        gbp->count += tt0119_AR_PO_NUMBER_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_due_date,'\0',tt0119_AR_DUE_DATE_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_due_date, sz_recvbufcat + gbp->count, tt0119_AR_DUE_DATE_LEN);
        gbp->count += tt0119_AR_DUE_DATE_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_type,'\0',tt0119_AR_TYPE_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_type, sz_recvbufcat + gbp->count, tt0119_AR_TYPE_LEN);
        gbp->count += tt0119_AR_TYPE_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_type_text,'\0',tt0119_AR_TYPE_TEXT_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_type_text, sz_recvbufcat + gbp->count, tt0119_AR_TYPE_TEXT_LEN);
        gbp->count += tt0119_AR_TYPE_TEXT_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_type_flag,'\0',tt0119_AR_TYPE_FLAG_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_type_flag, sz_recvbufcat + gbp->count, tt0119_AR_TYPE_FLAG_LEN);
        gbp->count += tt0119_AR_TYPE_FLAG_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_status,'\0',tt0119_AR_STATUS_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_status, sz_recvbufcat + gbp->count, tt0119_AR_STATUS_LEN);
        gbp->count += tt0119_AR_STATUS_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_comment,'\0',tt0119_AR_COMMENT_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_comment, sz_recvbufcat + gbp->count, tt0119_AR_COMMENT_LEN);
        gbp->count += tt0119_AR_COMMENT_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_comment_type,'\0',tt0119_AR_COMMENT_TYPE_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_comment_type, sz_recvbufcat + gbp->count, tt0119_AR_COMMENT_TYPE_LEN);
        gbp->count += tt0119_AR_COMMENT_TYPE_LEN;

                memset(ptr_recvbuf->ar_data[gbp->i].ar_comment_date,'\0',tt0119_AR_COMMENT_DATE_LEN+1);
        memcpy(ptr_recvbuf->ar_data[gbp->i].ar_comment_date, sz_recvbufcat + gbp->count, tt0119_AR_COMMENT_DATE_LEN);
        gbp->count += tt0119_AR_COMMENT_DATE_LEN;

        }


    for(gbp->i = 0; gbp->i < tt0119_AR_DATA_COUNT; gbp->i++) 
    {
                ap_rprintf(r,"		<AR_DATA>\n\n");
                ap_rprintf(r,"			<AR_COMPANY>%s</AR_COMPANY>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_company));
                ap_rprintf(r,"			<AR_DIVISION>%s</AR_DIVISION>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_division));
                ap_rprintf(r,"			<AR_TRANS_DATE>%s</AR_TRANS_DATE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_trans_date));
                ap_rprintf(r,"			<AR_ORIG_REF>%s</AR_ORIG_REF>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_orig_ref));
                ap_rprintf(r,"			<AR_TOT_DOL>%s</AR_TOT_DOL>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_tot_dol));
                ap_rprintf(r,"			<AR_APPL_CR_FLAG>%s</AR_APPL_CR_FLAG>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_appl_cr_flag));
                ap_rprintf(r,"			<AR_POST_REF>%s</AR_POST_REF>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_post_ref));
                ap_rprintf(r,"			<AR_PO_NUMBER>%s</AR_PO_NUMBER>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_po_number));
                ap_rprintf(r,"			<AR_DUE_DATE>%s</AR_DUE_DATE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_due_date));
                ap_rprintf(r,"			<AR_TYPE>%s</AR_TYPE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_type));
                ap_rprintf(r,"			<AR_TYPE_TEXT>%s</AR_TYPE_TEXT>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_type_text));
                ap_rprintf(r,"			<AR_TYPE_FLAG>%s</AR_TYPE_FLAG>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_type_flag));
                ap_rprintf(r,"			<AR_STATUS>%s</AR_STATUS>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_status));
                ap_rprintf(r,"			<AR_COMMENT>%s</AR_COMMENT>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_comment));
                ap_rprintf(r,"			<AR_COMMENT_TYPE>%s</AR_COMMENT_TYPE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_comment_type));
                ap_rprintf(r,"			<AR_COMMENT_DATE>%s</AR_COMMENT_DATE>\n\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->ar_data[gbp->i].ar_comment_date));
                ap_rprintf(r,"		</AR_DATA>\n\n");
    }

        ap_rprintf(r,"	%s>\n\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n\n", gbp->rsetag);

        ap_rprintf(r,"%s0120>\n\n", gbp->tt_betag);

    return(0);
}


