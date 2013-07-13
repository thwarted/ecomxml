/* tt0041.c  Add Order Info */
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


int             tt0041_CatSendStr(struct global_struct *, char *, tt0041_st_send *);
int             tt0041_ParceRecvStr(tt0041_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0041_lt_process(request_rec *, struct global_struct *, char *);


int tt0041_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0041_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0041_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0041_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0041 = (tt0041_st_send *)malloc(sizeof(tt0041_st_send));
        gbp->recvbuf0041 = (tt0041_st_recv *)malloc(sizeof(tt0041_st_recv));

        gbp->sendbufcat = malloc(tt0041_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0041_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0041_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0041_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0041,  '\0',  sizeof(tt0041_st_send));

        
    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0041->request_id, "XML");
    strcpy(gbp->sendbuf0041->record_id, "0041");

        strcpy(gbp->sendbuf0041->ip_address, r->connection->remote_ip);


    get_tag_data("COMPANY", gbp->sendbuf0041->company, gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0041->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0041->userid,gbp,stdout_buffer);
        get_tag_data("PAY_METHOD", gbp->sendbuf0041->pay_method,gbp,stdout_buffer);
        get_tag_data("FNAME", gbp->sendbuf0041->fname,gbp,stdout_buffer);
        get_tag_data("MINITIAL", gbp->sendbuf0041->minit,gbp,stdout_buffer);
        get_tag_data("LNAME", gbp->sendbuf0041->lname,gbp,stdout_buffer);
        get_tag_data("TITLE_CODE", gbp->sendbuf0041->title_code,gbp,stdout_buffer);
        get_tag_data("COMPANY_NAME", gbp->sendbuf0041->company_name,gbp,stdout_buffer);
        get_tag_data("JAP_FLAG", gbp->sendbuf0041->jap_flag,gbp,stdout_buffer);
        get_tag_data("ADDR_REF1", gbp->sendbuf0041->addr_ref1,gbp,stdout_buffer);
        get_tag_data("ADDR_REF2", gbp->sendbuf0041->addr_ref2,gbp,stdout_buffer);
        get_tag_data("STREET", gbp->sendbuf0041->street,gbp,stdout_buffer);
        get_tag_data("CITY", gbp->sendbuf0041->city,gbp,stdout_buffer);
        get_tag_data("STATE", gbp->sendbuf0041->state,gbp,stdout_buffer);
        get_tag_data("ZIPCODE", gbp->sendbuf0041->zipcode,gbp,stdout_buffer);
        get_tag_data("COUNTRY", gbp->sendbuf0041->country,gbp,stdout_buffer);
        get_tag_data("DAY_PHONE", gbp->sendbuf0041->day_phone,gbp,stdout_buffer);
        get_tag_data("NGT_PHONE", gbp->sendbuf0041->ngt_phone,gbp,stdout_buffer);
        get_tag_data("NO_PROMO", gbp->sendbuf0041->no_promo,gbp,stdout_buffer);
        get_tag_data("NO_RENT", gbp->sendbuf0041->no_rent,gbp,stdout_buffer);
        get_tag_data("REMIT_AMT", gbp->sendbuf0041->remit_amt,gbp,stdout_buffer);
        get_tag_data("FUTURE_DATE", gbp->sendbuf0041->future_date,gbp,stdout_buffer);
        get_tag_data("PO_NUM", gbp->sendbuf0041->po_num,gbp,stdout_buffer);
        get_tag_data("HOLD_COMPLETE", gbp->sendbuf0041->hold_complete,gbp,stdout_buffer);
        get_tag_data("EXPEDITE_ORD", gbp->sendbuf0041->expedite_ord,gbp,stdout_buffer);
        get_tag_data("NO_BACKORD", gbp->sendbuf0041->no_backord,gbp,stdout_buffer);
        get_tag_data("POSTPONE_ORD", gbp->sendbuf0041->postpone_ord,gbp,stdout_buffer);
        get_tag_data("NO_INSURANCE", gbp->sendbuf0041->no_insurance,gbp,stdout_buffer);
        get_tag_data("CC_UP_FRONT", gbp->sendbuf0041->cc_up_front,gbp,stdout_buffer);
        get_tag_data("OVERPAYMENT", gbp->sendbuf0041->overpayment,gbp,stdout_buffer);
        get_tag_data("STANDING_ORD", gbp->sendbuf0041->standing_ord,gbp,stdout_buffer);
        get_tag_data("GENDER", gbp->sendbuf0041->gender,gbp,stdout_buffer);
        get_tag_data("SHIP_METHOD", gbp->sendbuf0041->ship_method,gbp,stdout_buffer);
        get_tag_data("DEMOGRAPH1", gbp->sendbuf0041->demograph1,gbp,stdout_buffer);
        get_tag_data("DEMOGRAPH2", gbp->sendbuf0041->demograph2,gbp,stdout_buffer);
        get_tag_data("FAX_PHONE", gbp->sendbuf0041->fax_phone,gbp,stdout_buffer);
        get_tag_data("ORDER_XREF", gbp->sendbuf0041->order_xref,gbp,stdout_buffer);
        get_tag_data("CUST_TYPE", gbp->sendbuf0041->cust_type,gbp,stdout_buffer);
        get_tag_data("INSTALLMENTS", gbp->sendbuf0041->installments,gbp,stdout_buffer);
        get_tag_data("DISCOUNT_CODE1", gbp->sendbuf0041->discount_code1,gbp,stdout_buffer);
        get_tag_data("DISCOUNT_CODE2", gbp->sendbuf0041->discount_code2,gbp,stdout_buffer);
        get_tag_data("BIRTHDAY", gbp->sendbuf0041->birthday,gbp,stdout_buffer);
        get_tag_data("PASSWORD", gbp->sendbuf0041->password,gbp,stdout_buffer);
        get_tag_data("EMAIL_ADDR", gbp->sendbuf0041->email_address,gbp,stdout_buffer);
        get_tag_data("CC_NUM", gbp->sendbuf0041->cc_num,gbp,stdout_buffer);
        get_tag_data("CC_EXP_MONTH", gbp->sendbuf0041->cc_exp_month,gbp,stdout_buffer);
        get_tag_data("CC_EXP_YEAR", gbp->sendbuf0041->cc_exp_year,gbp,stdout_buffer);
        get_tag_data("SHIPTO_FNAME", gbp->sendbuf0041->shipto_fname,gbp,stdout_buffer);
        get_tag_data("SHIPTO_MINITIAL", gbp->sendbuf0041->shipto_minit,gbp,stdout_buffer);
        get_tag_data("SHIPTO_LNAME", gbp->sendbuf0041->shipto_lname,gbp,stdout_buffer);
        get_tag_data("SHIPTO_TITLE_CODE", gbp->sendbuf0041->shipto_title_code,gbp,stdout_buffer);
        get_tag_data("SHIPTO_COMPANY_NAME", gbp->sendbuf0041->shipto_company_name,gbp,stdout_buffer);
        get_tag_data("SHIPTO_JAP_FLAG", gbp->sendbuf0041->shipto_jap_flag,gbp,stdout_buffer);
        get_tag_data("SHIPTO_ADDR_REF1", gbp->sendbuf0041->shipto_addr_ref1,gbp,stdout_buffer);
        get_tag_data("SHIPTO_ADDR_REF2", gbp->sendbuf0041->shipto_addr_ref2,gbp,stdout_buffer);
        get_tag_data("SHIPTO_STREET", gbp->sendbuf0041->shipto_street,gbp,stdout_buffer);
        get_tag_data("SHIPTO_CITY", gbp->sendbuf0041->shipto_city,gbp,stdout_buffer);
        get_tag_data("SHIPTO_STATE", gbp->sendbuf0041->shipto_state,gbp,stdout_buffer);
        get_tag_data("SHIPTO_ZIPCODE", gbp->sendbuf0041->shipto_zipcode,gbp,stdout_buffer);
        get_tag_data("SHIPTO_COUNTRY", gbp->sendbuf0041->shipto_country,gbp,stdout_buffer);
        get_tag_data("SHIPTO_PHONE", gbp->sendbuf0041->shipto_phone,gbp,stdout_buffer);
        
        
        if(tt0041_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0041) == tt0041_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0041);
                        free (gbp->recvbuf0041);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0041_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0041);
                        free (gbp->recvbuf0041);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0041_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0041);
                        free (gbp->recvbuf0041);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0041);
                        free (gbp->recvbuf0041);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0041_ParceRecvStr(gbp->recvbuf0041, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0041);
                free (gbp->recvbuf0041);
        }
        else
        {
                XML_Error("tt0041_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0041);
                free (gbp->recvbuf0041);
                return(-1);
        }

    return(0);
}


int tt0041_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0041_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-2.2s%-16.16s%-1.1s%-16.16s%-2.2s"
                        "%-30.30s%-1.1s%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-4.4s"
                        "%-14.14s%-14.14s%-1.1s%-1.1s%08d%-8.8s%-20.20s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s"
                        "%-1.1s%-1.1s%-1.1s%-1.1s%-2.2s%-50.50s%-50.50s%-14.14s%-24.24s%-2.2s%-1.1s"
                        "%-1.1s%-1.1s%-8.8s%-16.16s%-50.50s%-20.20s%-2.2s%-2.2s%-16.16s%-1.1s%-16.16s"
                        "%-2.2s%-30.30s%-1.1s%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-4.4s%-14.14s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->pay_method,
                        ptr_sendbuf->fname,
                        ptr_sendbuf->minit,
                        ptr_sendbuf->lname,
                        ptr_sendbuf->title_code,
                        ptr_sendbuf->company_name,
                        ptr_sendbuf->jap_flag,
                        ptr_sendbuf->addr_ref1,
                        ptr_sendbuf->addr_ref2,
                        ptr_sendbuf->street,
                        ptr_sendbuf->city,
                        ptr_sendbuf->state,
                        ptr_sendbuf->zipcode,
                        ptr_sendbuf->country,
                        ptr_sendbuf->day_phone,
                        ptr_sendbuf->ngt_phone,
                        ptr_sendbuf->no_promo,
                        ptr_sendbuf->no_rent,
                        (int)((atof(ptr_sendbuf->remit_amt))*100),
                        ptr_sendbuf->future_date,
                        ptr_sendbuf->po_num,
                        ptr_sendbuf->hold_complete,
                        ptr_sendbuf->expedite_ord,
                        ptr_sendbuf->no_backord,
                        ptr_sendbuf->postpone_ord,
                        ptr_sendbuf->no_insurance,
                        ptr_sendbuf->cc_up_front,
                        ptr_sendbuf->overpayment,
                        ptr_sendbuf->standing_ord,
                        ptr_sendbuf->gender,
                        ptr_sendbuf->ship_method,
                        ptr_sendbuf->demograph1,
                        ptr_sendbuf->demograph2,
                        ptr_sendbuf->fax_phone,
                        ptr_sendbuf->order_xref,
                        ptr_sendbuf->cust_type,
                        ptr_sendbuf->installments,
                        ptr_sendbuf->discount_code1,
                        ptr_sendbuf->discount_code2,
                        ptr_sendbuf->birthday,
                        ptr_sendbuf->password,
                        ptr_sendbuf->email_address,
                        ptr_sendbuf->cc_num,
                        ptr_sendbuf->cc_exp_month,
                        ptr_sendbuf->cc_exp_year,
                        ptr_sendbuf->shipto_fname,
                        ptr_sendbuf->shipto_minit,
                        ptr_sendbuf->shipto_lname,
                        ptr_sendbuf->shipto_title_code,
                        ptr_sendbuf->shipto_company_name,
                        ptr_sendbuf->shipto_jap_flag,
                        ptr_sendbuf->shipto_addr_ref1,
                        ptr_sendbuf->shipto_addr_ref2,
                        ptr_sendbuf->shipto_street,
                        ptr_sendbuf->shipto_city,
                        ptr_sendbuf->shipto_state,
                        ptr_sendbuf->shipto_zipcode,
                        ptr_sendbuf->shipto_country,
                        ptr_sendbuf->shipto_phone);

            
    return(gbp->j);
}


int tt0041_ParceRecvStr(tt0041_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0042.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0042 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0042.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0042 %s\"tt0042\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0041_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0041_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0041_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0041_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0041_REC_ID_LEN );
    gbp->count += tt0041_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0041_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0041_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0041_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0041_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0041_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0041_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0041_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0041_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0041_ERR_LEN;

        gbp->count += tt0041_SEND_FILLER_LEN;


        memset(ptr_recvbuf->pay_method,'\0', tt0041_PAY_METH_LEN+1);
    memcpy(ptr_recvbuf->pay_method, sz_recvbufcat + gbp->count, tt0041_PAY_METH_LEN);
        ap_rprintf(r,"		<PAY_METHOD>%s</PAY_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->pay_method));
    gbp->count += tt0041_PAY_METH_LEN;

        memset(ptr_recvbuf->fname,'\0', tt0041_FN_LEN+1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0041_FN_LEN);
        ap_rprintf(r,"		<FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += tt0041_FN_LEN;

        memset(ptr_recvbuf->minit,'\0', tt0041_MI_LEN+1);
    memcpy(ptr_recvbuf->minit, sz_recvbufcat + gbp->count, tt0041_MI_LEN);
        ap_rprintf(r,"		<MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->minit));
    gbp->count += tt0041_MI_LEN;

        memset(ptr_recvbuf->lname,'\0', tt0041_LN_LEN+1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0041_LN_LEN);
        ap_rprintf(r,"		<LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += tt0041_LN_LEN;

        memset(ptr_recvbuf->title_code,'\0', tt0041_TITLE_LEN+1);
    memcpy(ptr_recvbuf->title_code, sz_recvbufcat + gbp->count, tt0041_TITLE_LEN);
        ap_rprintf(r,"		<TITLE_CODE>%s</TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->title_code));
    gbp->count += tt0041_TITLE_LEN;

        memset(ptr_recvbuf->company_name,'\0', tt0041_COMP_LEN+1);
    memcpy(ptr_recvbuf->company_name, sz_recvbufcat + gbp->count, tt0041_COMP_LEN);
        ap_rprintf(r,"		<COMPANY_NAME>%s</COMPANY_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->company_name));
    gbp->count += tt0041_COMP_LEN;

        memset(ptr_recvbuf->jap_flag,'\0', tt0041_JAP_LEN+1);
    memcpy(ptr_recvbuf->jap_flag, sz_recvbufcat + gbp->count, tt0041_JAP_LEN);
        ap_rprintf(r,"		<JAP_FLAG>%s</JAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->jap_flag));
    gbp->count += tt0041_JAP_LEN;

        memset(ptr_recvbuf->addr_ref1,'\0', tt0041_REF_LEN+1);
    memcpy(ptr_recvbuf->addr_ref1, sz_recvbufcat + gbp->count, tt0041_REF_LEN);
        ap_rprintf(r,"		<ADDR_REF1>%s</ADDR_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->addr_ref1));
    gbp->count += tt0041_REF_LEN;

        memset(ptr_recvbuf->addr_ref2,'\0', tt0041_REF_LEN+1);
    memcpy(ptr_recvbuf->addr_ref2, sz_recvbufcat + gbp->count, tt0041_REF_LEN);
        ap_rprintf(r,"		<ADDR_REF2>%s</ADDR_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->addr_ref2));
    gbp->count += tt0041_REF_LEN;

        memset(ptr_recvbuf->street,'\0', tt0041_STREET_LEN+1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, tt0041_STREET_LEN);
        ap_rprintf(r,"		<STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));
    gbp->count += tt0041_STREET_LEN;

        memset(ptr_recvbuf->city,'\0', tt0041_CITY_LEN+1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, tt0041_CITY_LEN);
        ap_rprintf(r,"		<CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));
    gbp->count += tt0041_CITY_LEN;

        memset(ptr_recvbuf->state,'\0', tt0041_STATE_LEN+1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, tt0041_STATE_LEN);
        ap_rprintf(r,"		<STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));
    gbp->count += tt0041_STATE_LEN;

        memset(ptr_recvbuf->zipcode,'\0', tt0041_ZIP_LEN+1);
    memcpy(ptr_recvbuf->zipcode, sz_recvbufcat + gbp->count, tt0041_ZIP_LEN);
        ap_rprintf(r,"		<ZIPCODE>%s</ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->zipcode));
    gbp->count += tt0041_ZIP_LEN;

        memset(ptr_recvbuf->country,'\0', tt0041_COUNTRY_LEN+1);
    memcpy(ptr_recvbuf->country, sz_recvbufcat + gbp->count, tt0041_COUNTRY_LEN);
        ap_rprintf(r,"		<COUNTRY>%s</COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->country));
    gbp->count += tt0041_COUNTRY_LEN;

        memset(ptr_recvbuf->day_phone,'\0', tt0041_PHONE_LEN+1);
    memcpy(ptr_recvbuf->day_phone, sz_recvbufcat + gbp->count, tt0041_PHONE_LEN);
        ap_rprintf(r,"		<DAY_PHONE>%s</DAY_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->day_phone));
    gbp->count += tt0041_PHONE_LEN;

        memset(ptr_recvbuf->ngt_phone,'\0', tt0041_PHONE_LEN+1);
    memcpy(ptr_recvbuf->ngt_phone, sz_recvbufcat + gbp->count, tt0041_PHONE_LEN);
        ap_rprintf(r,"		<NGT_PHONE>%s</NGT_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->ngt_phone));
    gbp->count += tt0041_PHONE_LEN;

        memset(ptr_recvbuf->no_promo,'\0', tt0041_PROMO_LEN+1);
    memcpy(ptr_recvbuf->no_promo, sz_recvbufcat + gbp->count, tt0041_PROMO_LEN);
        ap_rprintf(r,"		<NO_PROMO>%s</NO_PROMO>\n", handle_special_chars(gbp,ptr_recvbuf->no_promo));
    gbp->count += tt0041_PROMO_LEN;

        memset(ptr_recvbuf->no_rent,'\0', tt0041_RENT_LEN+1);
    memcpy(ptr_recvbuf->no_rent, sz_recvbufcat + gbp->count, tt0041_RENT_LEN);
        ap_rprintf(r,"		<NO_RENT>%s</NO_RENT>\n", handle_special_chars(gbp,ptr_recvbuf->no_rent));
    gbp->count += tt0041_RENT_LEN;

        memset(ptr_recvbuf->remit_amt,'\0', tt0041_REMIT_LEN+1);
    memcpy(ptr_recvbuf->remit_amt, sz_recvbufcat + gbp->count, tt0041_REMIT_LEN);
        ap_rprintf(r,"		<REMIT_AMT>%s</REMIT_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->remit_amt));
    gbp->count += tt0041_REMIT_LEN;

        memset(ptr_recvbuf->future_date,'\0', tt0041_FO_LEN+1);
    memcpy(ptr_recvbuf->future_date, sz_recvbufcat + gbp->count, tt0041_FO_LEN);
        ap_rprintf(r,"		<FUTURE_DATE>%s</FUTURE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->future_date));
    gbp->count += tt0041_FO_LEN;

        memset(ptr_recvbuf->po_num,'\0', tt0041_PO_LEN+1);
    memcpy(ptr_recvbuf->po_num, sz_recvbufcat + gbp->count, tt0041_PO_LEN);
        ap_rprintf(r,"		<PO_NUM>%s</PO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->po_num));
    gbp->count += tt0041_PO_LEN;

        memset(ptr_recvbuf->hold_complete,'\0', tt0041_HOLD_COMP_LEN+1);
    memcpy(ptr_recvbuf->hold_complete, sz_recvbufcat + gbp->count, tt0041_HOLD_COMP_LEN);
        ap_rprintf(r,"		<HOLD_COMPLETE>%s</HOLD_COMPLETE>\n", handle_special_chars(gbp,ptr_recvbuf->hold_complete));
    gbp->count += tt0041_HOLD_COMP_LEN;

        memset(ptr_recvbuf->expedite_ord,'\0', tt0041_EXP_ORD_LEN+1);
    memcpy(ptr_recvbuf->expedite_ord, sz_recvbufcat + gbp->count, tt0041_EXP_ORD_LEN);
        ap_rprintf(r,"		<EXPEDITE_ORD>%s</EXPEDITE_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->expedite_ord));
    gbp->count += tt0041_EXP_ORD_LEN;

        memset(ptr_recvbuf->no_backord,'\0', tt0041_BACKORD_LEN+1);
    memcpy(ptr_recvbuf->no_backord, sz_recvbufcat + gbp->count, tt0041_BACKORD_LEN);
        ap_rprintf(r,"		<NO_BACKORD>%s</NO_BACKORD>\n", handle_special_chars(gbp,ptr_recvbuf->no_backord));
    gbp->count += tt0041_BACKORD_LEN;

        memset(ptr_recvbuf->postpone_ord,'\0', tt0041_POST_ORD_LEN+1);
    memcpy(ptr_recvbuf->postpone_ord, sz_recvbufcat + gbp->count, tt0041_POST_ORD_LEN);
        ap_rprintf(r,"		<POSTPONE_ORD>%s</POSTPONE_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->postpone_ord));
    gbp->count += tt0041_POST_ORD_LEN;

        memset(ptr_recvbuf->no_insurance,'\0', tt0041_INSUR_LEN+1);
    memcpy(ptr_recvbuf->no_insurance, sz_recvbufcat + gbp->count, tt0041_INSUR_LEN);
        ap_rprintf(r,"		<NO_INSURANCE>%s</NO_INSURANCE>\n", handle_special_chars(gbp,ptr_recvbuf->no_insurance));
    gbp->count += tt0041_INSUR_LEN;

        memset(ptr_recvbuf->cc_up_front,'\0', tt0041_UP_FRT_LEN+1);
    memcpy(ptr_recvbuf->cc_up_front, sz_recvbufcat + gbp->count, tt0041_UP_FRT_LEN);
        ap_rprintf(r,"		<CC_UP_FRONT>%s</CC_UP_FRONT>\n", handle_special_chars(gbp,ptr_recvbuf->cc_up_front));
    gbp->count += tt0041_UP_FRT_LEN;

        memset(ptr_recvbuf->overpayment,'\0', tt0041_OVERPAYMENT_LEN+1);
    memcpy(ptr_recvbuf->overpayment, sz_recvbufcat + gbp->count, tt0041_OVERPAYMENT_LEN);
        ap_rprintf(r,"		<OVERPAYMENT>%s</OVERPAYMENT>\n", handle_special_chars(gbp,ptr_recvbuf->overpayment));
    gbp->count += tt0041_OVERPAYMENT_LEN;

        memset(ptr_recvbuf->standing_ord,'\0', tt0041_STAND_ORD_LEN+1);
    memcpy(ptr_recvbuf->standing_ord, sz_recvbufcat + gbp->count, tt0041_STAND_ORD_LEN);
        ap_rprintf(r,"		<STANDING_ORD>%s</STANDING_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->standing_ord));
    gbp->count += tt0041_STAND_ORD_LEN;

        memset(ptr_recvbuf->gender,'\0', tt0041_GENDER_LEN+1);
    memcpy(ptr_recvbuf->gender, sz_recvbufcat + gbp->count, tt0041_GENDER_LEN);
        ap_rprintf(r,"		<GENDER>%s</GENDER>\n", handle_special_chars(gbp,ptr_recvbuf->gender));
    gbp->count += tt0041_GENDER_LEN;

        memset(ptr_recvbuf->ship_method,'\0', tt0041_SHIP_METH_LEN+1);
    memcpy(ptr_recvbuf->ship_method, sz_recvbufcat + gbp->count, tt0041_SHIP_METH_LEN);
        ap_rprintf(r,"		<SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method));
    gbp->count += tt0041_SHIP_METH_LEN;

        memset(ptr_recvbuf->demograph1,'\0', tt0041_DEMOGRAPH_LEN+1);
    memcpy(ptr_recvbuf->demograph1, sz_recvbufcat + gbp->count, tt0041_DEMOGRAPH_LEN);
        ap_rprintf(r,"		<DEMOGRAPH1>%s</DEMOGRAPH1>\n", handle_special_chars(gbp,ptr_recvbuf->demograph1));
    gbp->count += tt0041_DEMOGRAPH_LEN;

        memset(ptr_recvbuf->demograph2,'\0', tt0041_DEMOGRAPH_LEN+1);
    memcpy(ptr_recvbuf->demograph2, sz_recvbufcat + gbp->count, tt0041_DEMOGRAPH_LEN);
        ap_rprintf(r,"		<DEMOGRAPH2>%s</DEMOGRAPH2>\n", handle_special_chars(gbp,ptr_recvbuf->demograph2));
    gbp->count += tt0041_DEMOGRAPH_LEN;

        memset(ptr_recvbuf->fax_phone,'\0', tt0041_FAX_LEN+1);
    memcpy(ptr_recvbuf->fax_phone, sz_recvbufcat + gbp->count, tt0041_FAX_LEN);
        ap_rprintf(r,"		<FAX_PHONE>%s</FAX_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->fax_phone));
    gbp->count += tt0041_FAX_LEN;

        memset(ptr_recvbuf->order_xref,'\0', tt0041_ORD_XREF_LEN+1);
    memcpy(ptr_recvbuf->order_xref, sz_recvbufcat + gbp->count, tt0041_ORD_XREF_LEN);
        ap_rprintf(r,"		<ORD_XREF>%s</ORD_XREF>\n", handle_special_chars(gbp,ptr_recvbuf->order_xref));
    gbp->count += tt0041_ORD_XREF_LEN;

        memset(ptr_recvbuf->cust_type,'\0', tt0041_CUST_TYPE_LEN+1);
    memcpy(ptr_recvbuf->cust_type, sz_recvbufcat + gbp->count, tt0041_CUST_TYPE_LEN);
        ap_rprintf(r,"		<CUST_TYPE>%s</CUST_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_type));
    gbp->count += tt0041_CUST_TYPE_LEN;

        memset(ptr_recvbuf->installments,'\0', tt0041_INSTAL_LEN+1);
    memcpy(ptr_recvbuf->installments, sz_recvbufcat + gbp->count, tt0041_INSTAL_LEN);
        ap_rprintf(r,"		<INSTALLMENTS>%s</INSTALLMENTS>\n", handle_special_chars(gbp,ptr_recvbuf->installments));
    gbp->count += tt0041_INSTAL_LEN;

        memset(ptr_recvbuf->discount_code1,'\0', tt0041_DISC_LEN+1);
    memcpy(ptr_recvbuf->discount_code1, sz_recvbufcat + gbp->count, tt0041_DISC_LEN);
        ap_rprintf(r,"		<DISCOUNT_CODE1>%s</DISCOUNT_CODE1>\n", handle_special_chars(gbp,ptr_recvbuf->discount_code1));
    gbp->count += tt0041_DISC_LEN;

        memset(ptr_recvbuf->discount_code2,'\0', tt0041_DISC_LEN+1);
    memcpy(ptr_recvbuf->discount_code2, sz_recvbufcat + gbp->count, tt0041_DISC_LEN);
        ap_rprintf(r,"		<DISCOUNT_CODE2>%s</DISCOUNT_CODE2>\n", handle_special_chars(gbp,ptr_recvbuf->discount_code2));
    gbp->count += tt0041_DISC_LEN;

        memset(ptr_recvbuf->birthday,'\0', tt0041_BIRTHDAY_LEN+1);
    memcpy(ptr_recvbuf->birthday, sz_recvbufcat + gbp->count, tt0041_BIRTHDAY_LEN);
        ap_rprintf(r,"		<BIRTHDAY>%s</BIRTHDAY>\n", handle_special_chars(gbp,ptr_recvbuf->birthday));
    gbp->count += tt0041_BIRTHDAY_LEN;

        memset(ptr_recvbuf->password,'\0', tt0041_PASSWORD_LEN+1);
    memcpy(ptr_recvbuf->password, sz_recvbufcat + gbp->count, tt0041_PASSWORD_LEN);
        ap_rprintf(r,"		<PASSWORD>%s</PASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->password));
    gbp->count += tt0041_PASSWORD_LEN;

        memset(ptr_recvbuf->email_address,'\0', tt0041_EMAIL_LEN+1);
    memcpy(ptr_recvbuf->email_address, sz_recvbufcat + gbp->count, tt0041_EMAIL_LEN);
        ap_rprintf(r,"		<EMAIL_ADDR>%s</EMAIL_ADDR>\n", handle_special_chars(gbp,ptr_recvbuf->email_address));
    gbp->count += tt0041_EMAIL_LEN;

        memset(ptr_recvbuf->cc_type,'\0', tt0041_CC_TYPE_LEN+1);
    memcpy(ptr_recvbuf->cc_type, sz_recvbufcat + gbp->count, tt0041_CC_TYPE_LEN);
        ap_rprintf(r,"		<CC_TYPE>%s</CC_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cc_type));
    gbp->count += tt0041_CC_TYPE_LEN;

        memset(ptr_recvbuf->cc_num,'\0', tt0041_CC_NUM_LEN+1);
    memcpy(ptr_recvbuf->cc_num, sz_recvbufcat + gbp->count, tt0041_CC_NUM_LEN);
        ap_rprintf(r,"		<CC_NUM>%s</CC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->cc_num));
    gbp->count += tt0041_CC_NUM_LEN;

        memset(ptr_recvbuf->cc_exp_month,'\0', tt0041_CC_EXP_LEN+1);
    memcpy(ptr_recvbuf->cc_exp_month, sz_recvbufcat + gbp->count, tt0041_CC_EXP_LEN);
        ap_rprintf(r,"		<CC_EXP_MONTH>%s</CC_EXP_MONTH>\n", handle_special_chars(gbp,ptr_recvbuf->cc_exp_month));
    gbp->count += tt0041_CC_EXP_LEN;

        memset(ptr_recvbuf->cc_exp_year,'\0', tt0041_CC_EXP_LEN+1);
    memcpy(ptr_recvbuf->cc_exp_year, sz_recvbufcat + gbp->count, tt0041_CC_EXP_LEN);
        ap_rprintf(r,"		<CC_EXP_YEAR>%s</CC_EXP_YEAR>\n", handle_special_chars(gbp,ptr_recvbuf->cc_exp_year));
    gbp->count += tt0041_CC_EXP_LEN;

        memset(ptr_recvbuf->shipto_num,'\0', tt0041_SHIPTO_NO_LEN+1);
    memcpy(ptr_recvbuf->shipto_num, sz_recvbufcat + gbp->count, tt0041_SHIPTO_NO_LEN);
        ap_rprintf(r,"		<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_num));
    gbp->count += tt0041_SHIPTO_NO_LEN;

        memset(ptr_recvbuf->shipto_fname,'\0', tt0041_SHIPTO_FN_LEN+1);
    memcpy(ptr_recvbuf->shipto_fname, sz_recvbufcat + gbp->count, tt0041_SHIPTO_FN_LEN);
        ap_rprintf(r,"		<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_fname));
    gbp->count += tt0041_SHIPTO_FN_LEN;

        memset(ptr_recvbuf->shipto_minit,'\0', tt0041_SHIPTO_MI_LEN+1);
    memcpy(ptr_recvbuf->shipto_minit, sz_recvbufcat + gbp->count, tt0041_SHIPTO_MI_LEN);
        ap_rprintf(r,"		<SHIPTO_MINITIAL>%s</SHIPTO_MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_minit));
    gbp->count += tt0041_SHIPTO_MI_LEN;

        memset(ptr_recvbuf->shipto_lname,'\0', tt0041_SHIPTO_LN_LEN+1);
    memcpy(ptr_recvbuf->shipto_lname, sz_recvbufcat + gbp->count, tt0041_SHIPTO_LN_LEN);
        ap_rprintf(r,"		<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_lname));
    gbp->count += tt0041_SHIPTO_LN_LEN;

        memset(ptr_recvbuf->shipto_title_code,'\0', tt0041_SHIPTO_TITLE_LEN+1);
    memcpy(ptr_recvbuf->shipto_title_code, sz_recvbufcat + gbp->count, tt0041_SHIPTO_TITLE_LEN);
        ap_rprintf(r,"		<SHIPTO_TITLE_CODE>%s</SHIPTO_TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_title_code));
    gbp->count += tt0041_SHIPTO_TITLE_LEN;

        memset(ptr_recvbuf->shipto_company_name,'\0', tt0041_SHIPTO_COMP_LEN+1);
    memcpy(ptr_recvbuf->shipto_company_name, sz_recvbufcat + gbp->count, tt0041_SHIPTO_COMP_LEN);
        ap_rprintf(r,"		<SHIPTO_COMPANY_NAME>%s</SHIPTO_COMPANY_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_company_name));
    gbp->count += tt0041_SHIPTO_COMP_LEN;

        memset(ptr_recvbuf->shipto_jap_flag,'\0', tt0041_SHIPTO_JAP_LEN+1);
    memcpy(ptr_recvbuf->shipto_jap_flag, sz_recvbufcat + gbp->count, tt0041_SHIPTO_JAP_LEN);
        ap_rprintf(r,"		<SHIPTO_JAP_FLAG>%s</SHIPTO_JAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_jap_flag));
    gbp->count += tt0041_JAP_LEN;

        memset(ptr_recvbuf->shipto_addr_ref1,'\0', tt0041_SHIPTO_REF_LEN+1);
    memcpy(ptr_recvbuf->shipto_addr_ref1, sz_recvbufcat + gbp->count, tt0041_SHIPTO_REF_LEN);
        ap_rprintf(r,"		<SHIPTO_ADDR_REF1>%s</SHIPTO_ADDR_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_addr_ref1));
    gbp->count += tt0041_SHIPTO_REF_LEN;

        memset(ptr_recvbuf->shipto_addr_ref2,'\0', tt0041_SHIPTO_REF_LEN+1);
    memcpy(ptr_recvbuf->shipto_addr_ref2, sz_recvbufcat + gbp->count, tt0041_SHIPTO_REF_LEN);
        ap_rprintf(r,"		<SHIPTO_ADDR_REF2>%s</SHIPTO_ADDR_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_addr_ref2));
    gbp->count += tt0041_SHIPTO_REF_LEN;

        memset(ptr_recvbuf->shipto_street,'\0', tt0041_SHIPTO_STREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_street, sz_recvbufcat + gbp->count, tt0041_SHIPTO_STREET_LEN);
        ap_rprintf(r,"		<SHIPTO_STREET>%s</SHIPTO_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_street));
    gbp->count += tt0041_SHIPTO_STREET_LEN;

        memset(ptr_recvbuf->shipto_city,'\0', tt0041_SHIPTO_CITY_LEN+1);
    memcpy(ptr_recvbuf->shipto_city, sz_recvbufcat + gbp->count, tt0041_SHIPTO_CITY_LEN);
        ap_rprintf(r,"		<SHIPTO_CITY>%s</SHIPTO_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_city));
    gbp->count += tt0041_SHIPTO_CITY_LEN;

        memset(ptr_recvbuf->shipto_state,'\0', tt0041_SHIPTO_STATE_LEN+1);
    memcpy(ptr_recvbuf->shipto_state, sz_recvbufcat + gbp->count, tt0041_SHIPTO_STATE_LEN);
        ap_rprintf(r,"		<SHIPTO_STATE>%s</SHIPTO_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_state));
    gbp->count += tt0041_SHIPTO_STATE_LEN;

        memset(ptr_recvbuf->shipto_zipcode,'\0', tt0041_SHIPTO_ZIP_LEN+1);
    memcpy(ptr_recvbuf->shipto_zipcode, sz_recvbufcat + gbp->count, tt0041_SHIPTO_ZIP_LEN);
        ap_rprintf(r,"		<SHIPTO_ZIPCODE>%s</SHIPTO_ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_zipcode));
    gbp->count += tt0041_SHIPTO_ZIP_LEN;

        memset(ptr_recvbuf->shipto_country,'\0', tt0041_SHIPTO_COUNTRY_LEN+1);
    memcpy(ptr_recvbuf->shipto_country, sz_recvbufcat + gbp->count, tt0041_SHIPTO_COUNTRY_LEN);
        ap_rprintf(r,"		<SHIPTO_COUNTRY>%s</SHIPTO_COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_country));
    gbp->count += tt0041_SHIPTO_COUNTRY_LEN;

        memset(ptr_recvbuf->shipto_phone,'\0', tt0041_SHIPTO_PHONE_LEN+1);
    memcpy(ptr_recvbuf->shipto_phone, sz_recvbufcat + gbp->count, tt0041_SHIPTO_PHONE_LEN);
        ap_rprintf(r,"		<SHIPTO_PHONE>%s</SHIPTO_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_phone));
    gbp->count += tt0041_SHIPTO_PHONE_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0042>\n", gbp->tt_betag);

    return(0);
}

