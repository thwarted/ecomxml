/* tt0009.c Customer Inquiry */
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



int             tt0009_CatSendStr(struct global_struct *, char *, tt0009_st_send *);
int             tt0009_ParceRecvStr(tt0009_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0009_lt_process(request_rec *, struct global_struct *, char *);



int tt0009_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */


        return(0);
}


int tt0009_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0009_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0009_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0009 = (tt0009_st_send *)malloc(sizeof(tt0009_st_send));
        gbp->recvbuf0009 = (tt0009_st_recv *)malloc(sizeof(tt0009_st_recv));

        gbp->sendbufcat = malloc(tt0009_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0009_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0009_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0009_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0009,  '\0',  sizeof(tt0009_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0009->request_id, "XML");
    strcpy(gbp->sendbuf0009->record_id, "0009");

        strcpy(gbp->sendbuf0009->ip_address, r->connection->remote_ip);
    strcpy(gbp->sendbuf0009->cart_update, "Y");
        
        get_tag_data("COMPANY", gbp->sendbuf0009->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0009->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0009->userid,gbp,stdout_buffer);
        get_tag_data("PASSWORD", gbp->sendbuf0009->password,gbp,stdout_buffer);
        get_tag_data("ORD_NUM", gbp->sendbuf0009->ord_num,gbp,stdout_buffer);
        get_tag_data("CUST_EDP", gbp->sendbuf0009->cust_edp,gbp,stdout_buffer);
        get_tag_data("FNAME", gbp->sendbuf0009->fname,gbp,stdout_buffer);
        get_tag_data("LNAME", gbp->sendbuf0009->lname,gbp,stdout_buffer);
        get_tag_data("COMPANY_NAME", gbp->sendbuf0009->comp_name,gbp,stdout_buffer);
        get_tag_data("STREET", gbp->sendbuf0009->street,gbp,stdout_buffer);
        get_tag_data("CITY", gbp->sendbuf0009->city,gbp,stdout_buffer);
        get_tag_data("STATE", gbp->sendbuf0009->state,gbp,stdout_buffer);
        get_tag_data("ZIPCODE", gbp->sendbuf0009->zipcode,gbp,stdout_buffer);
        get_tag_data("COUNTRY", gbp->sendbuf0009->country,gbp,stdout_buffer);
        get_tag_data("DAY_PHONE", gbp->sendbuf0009->day_phone,gbp,stdout_buffer);
        get_tag_data("NGT_PHONE", gbp->sendbuf0009->night_phone,gbp,stdout_buffer);
        get_tag_data("EMAIL_ADDR", gbp->sendbuf0009->email_addr,gbp,stdout_buffer);
        get_tag_data("CUST_NUM", gbp->sendbuf0009->cust_num,gbp,stdout_buffer);
        get_tag_data("CUSTOMER_LOGON", gbp->sendbuf0009->customer_logon,gbp,stdout_buffer);

    if((tt0009_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0009)) == tt0009_LAN_SEND_BUF_LEN)
    {

                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0009);
                        free (gbp->recvbuf0009);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0009_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0009);
                        free (gbp->recvbuf0009);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0009_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0009);
                        free (gbp->recvbuf0009);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0009);
                        free (gbp->recvbuf0009);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0009_ParceRecvStr(gbp->recvbuf0009, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0009);
                free (gbp->recvbuf0009);
    }
    else
    {
                XML_Error("tt0009_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0009);
                free (gbp->recvbuf0009);
                return(-1);
    }

    return 0;
}







int tt0009_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0009_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-16.16s%-8.8s%09d%-1.1s%-16.16s%-16.16s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-4.4s%-16.16s%-16.16s%-50.50s%-10.10s%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->password,
            ptr_sendbuf->ord_num,
            atoi(ptr_sendbuf->cust_edp),
            ptr_sendbuf->cart_update,
            ptr_sendbuf->fname,
            ptr_sendbuf->lname,
            ptr_sendbuf->comp_name,
            ptr_sendbuf->street,
            ptr_sendbuf->city,
            ptr_sendbuf->state,
            ptr_sendbuf->zipcode,
            ptr_sendbuf->country,
            ptr_sendbuf->day_phone,
            ptr_sendbuf->night_phone,
            ptr_sendbuf->email_addr,
                        ptr_sendbuf->cust_num,
                        ptr_sendbuf->customer_logon);

    return(gbp->j);
}


int tt0009_ParceRecvStr(tt0009_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0010.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0010 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0010.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0010 %s\"tt0010\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

    
    memset(ptr_recvbuf->request_id, '\0', tt0009_REQ_ID_LEN + 1);
        memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0009_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0009_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0009_REC_ID_LEN + 1);
        memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0009_REC_ID_LEN );
    gbp->count += tt0009_REC_ID_LEN;

        memset(ptr_recvbuf->userid, '\0', tt0009_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0009_USER_ID_LEN );
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0009_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0009_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0009_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0009_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0009_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0009_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0009_ERR_LEN;

        gbp->count += tt0009_SEND_FILLER_LEN;


        memset(ptr_recvbuf->password, '\0', tt0009_PASSWORD_LEN + 1);
    memcpy(ptr_recvbuf->password, sz_recvbufcat + gbp->count, tt0009_PASSWORD_LEN );
        ap_rprintf(r,"		<PASSWORD>%s</PASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->password));
    gbp->count += tt0009_PASSWORD_LEN;

        memset(ptr_recvbuf->more_flag, '\0', tt0009_MORE_LEN + 1);
    memcpy(ptr_recvbuf->more_flag, sz_recvbufcat + gbp->count, tt0009_MORE_LEN );
        ap_rprintf(r,"		<MORE_FLAG>%s</MORE_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->more_flag));
    gbp->count += tt0009_MORE_LEN;


        memset(ptr_recvbuf->cust_edp, '\0', tt0009_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0009_EDP_LEN );
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0009_EDP_LEN;

	    memset(ptr_recvbuf->title, '\0', tt0009_TITLE_LEN + 1);
    memcpy(ptr_recvbuf->title, sz_recvbufcat + gbp->count, tt0009_TITLE_LEN );
        ap_rprintf(r,"		<TITLE>%s</TITLE>\n", handle_special_chars(gbp,ptr_recvbuf->title));
    gbp->count += tt0009_TITLE_LEN;

        memset(ptr_recvbuf->fname, '\0', tt0009_FN_LEN + 1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0009_FN_LEN );
        ap_rprintf(r,"		<FIRST_NAME>%s</FIRST_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += tt0009_FN_LEN;

        memset(ptr_recvbuf->lname, '\0', tt0009_LN_LEN + 1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0009_LN_LEN );
        ap_rprintf(r,"		<LAST_NAME>%s</LAST_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += tt0009_LN_LEN;

        memset(ptr_recvbuf->comp_name, '\0', tt0009_COMP_NAME_LEN + 1);
    memcpy(ptr_recvbuf->comp_name, sz_recvbufcat + gbp->count, tt0009_COMP_NAME_LEN );
        ap_rprintf(r,"		<COMPANY_NAME>%s</COMPANY_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->comp_name));
    gbp->count += tt0009_COMP_NAME_LEN;

        memset(ptr_recvbuf->street, '\0', tt0009_STREET_LEN + 1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, tt0009_STREET_LEN );
        ap_rprintf(r,"		<STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));
    gbp->count += tt0009_STREET_LEN;

        memset(ptr_recvbuf->city, '\0', tt0009_CITY_LEN + 1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, tt0009_CITY_LEN );
        ap_rprintf(r,"		<CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));
    gbp->count += tt0009_CITY_LEN;

        memset(ptr_recvbuf->state, '\0', tt0009_STATE_LEN + 1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, tt0009_STATE_LEN );
        ap_rprintf(r,"		<STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));
    gbp->count += tt0009_STATE_LEN;

        memset(ptr_recvbuf->zipcode, '\0', tt0009_ZIP_LEN + 1);
    memcpy(ptr_recvbuf->zipcode, sz_recvbufcat + gbp->count, tt0009_ZIP_LEN );
        ap_rprintf(r,"		<ZIPCODE>%s</ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->zipcode));
    gbp->count += tt0009_ZIP_LEN;

        memset(ptr_recvbuf->country, '\0', tt0009_RET_COUNTRY_LEN + 1);
    memcpy(ptr_recvbuf->country, sz_recvbufcat + gbp->count, tt0009_RET_COUNTRY_LEN );
        ap_rprintf(r,"		<COUNTRY>%s</COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->country));
    gbp->count += tt0009_RET_COUNTRY_LEN;

        memset(ptr_recvbuf->day_phone, '\0', tt0009_PHONE_LEN + 1);
    memcpy(ptr_recvbuf->day_phone, sz_recvbufcat + gbp->count, tt0009_PHONE_LEN );
        ap_rprintf(r,"		<DAY_PHONE>%s</DAY_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->day_phone));
    gbp->count += tt0009_PHONE_LEN;

        memset(ptr_recvbuf->night_phone, '\0', tt0009_PHONE_LEN + 1);
    memcpy(ptr_recvbuf->night_phone, sz_recvbufcat + gbp->count, tt0009_PHONE_LEN );
        ap_rprintf(r,"		<NGT_PHONE>%s</NGT_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->night_phone));
    gbp->count += tt0009_PHONE_LEN;

	    memset(ptr_recvbuf->fax_phone, '\0', tt0009_PHONE_LEN + 1);
    memcpy(ptr_recvbuf->fax_phone, sz_recvbufcat + gbp->count, tt0009_PHONE_LEN );
        ap_rprintf(r,"		<FAX_PHONE>%s</FAX_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->fax_phone));
    gbp->count += tt0009_PHONE_LEN;

        memset(ptr_recvbuf->email_addr, '\0', tt0009_EMAIL_LEN + 1);
    memcpy(ptr_recvbuf->email_addr, sz_recvbufcat + gbp->count, tt0009_EMAIL_LEN );
        ap_rprintf(r,"		<EMAIL_ADDR>%s</EMAIL_ADDR>\n", handle_special_chars(gbp,ptr_recvbuf->email_addr));
    gbp->count += tt0009_EMAIL_LEN;

        memset(ptr_recvbuf->ref1, '\0', tt0009_REF_LEN + 1);
    memcpy(ptr_recvbuf->ref1, sz_recvbufcat + gbp->count, tt0009_REF_LEN );
        ap_rprintf(r,"		<REF1>%s</REF1>\n", handle_special_chars(gbp,ptr_recvbuf->ref1));
    gbp->count += tt0009_REF_LEN;

        memset(ptr_recvbuf->ref2, '\0', tt0009_REF_LEN + 1);
    memcpy(ptr_recvbuf->ref2, sz_recvbufcat + gbp->count, tt0009_REF_LEN );
        ap_rprintf(r,"		<REF2>%s</REF2>\n", handle_special_chars(gbp,ptr_recvbuf->ref2));
    gbp->count += tt0009_REF_LEN;

        memset(ptr_recvbuf->cust_num, '\0', tt0009_CUST_NO_LEN + 1);
    memcpy(ptr_recvbuf->cust_num, sz_recvbufcat + gbp->count, tt0009_CUST_NO_LEN );
        ap_rprintf(r,"		<CUST_NUM>%s</CUST_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->cust_num));
    gbp->count += tt0009_CUST_NO_LEN;

        memset(ptr_recvbuf->returning_cust, '\0', tt0009_RETURNING_CUST_LEN + 1);
    memcpy(ptr_recvbuf->returning_cust, sz_recvbufcat + gbp->count, tt0009_RETURNING_CUST_LEN );
        ap_rprintf(r,"		<RETURNING_CUST>%s</RETURNING_CUST>\n", handle_special_chars(gbp,ptr_recvbuf->returning_cust));
    gbp->count += tt0009_RETURNING_CUST_LEN;

        memset(ptr_recvbuf->stand_ord_cust, '\0', tt0009_STAND_ORD_CUST_LEN + 1);
    memcpy(ptr_recvbuf->stand_ord_cust, sz_recvbufcat + gbp->count, tt0009_STAND_ORD_CUST_LEN );
        ap_rprintf(r,"		<STAND_ORD_CUST>%s</STAND_ORD_CUST>\n", handle_special_chars(gbp,ptr_recvbuf->stand_ord_cust));
    gbp->count += tt0009_STAND_ORD_CUST_LEN;

        memset(ptr_recvbuf->customer_logon, '\0', tt0009_CUSTOMER_LOGON_LEN + 1);
    memcpy(ptr_recvbuf->customer_logon, sz_recvbufcat + gbp->count, tt0009_CUSTOMER_LOGON_LEN );
        ap_rprintf(r,"		<CUSTOMER_LOGON>%s</CUSTOMER_LOGON>\n", handle_special_chars(gbp,ptr_recvbuf->customer_logon));
    gbp->count += tt0009_CUSTOMER_LOGON_LEN;

    memset(ptr_recvbuf->encrypt_cust_edp, '\0', tt0009_ENCRYPT_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->encrypt_cust_edp, sz_recvbufcat + gbp->count, tt0009_ENCRYPT_CUST_EDP_LEN );
        ap_rprintf(r,"		<ENCRYPT_CUST_EDP>%s</ENCRYPT_CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->encrypt_cust_edp));
    gbp->count += tt0009_ENCRYPT_CUST_EDP_LEN;

	memset(ptr_recvbuf->birthdate, '\0', tt0009_BIRTHDATE_LEN + 1);
    memcpy(ptr_recvbuf->birthdate, sz_recvbufcat + gbp->count, tt0009_BIRTHDATE_LEN );
        ap_rprintf(r,"		<BIRTHDATE>%s</BIRTHDATE>\n", handle_special_chars(gbp,ptr_recvbuf->birthdate));
    gbp->count += tt0009_BIRTHDATE_LEN;

	memset(ptr_recvbuf->ref_flag, '\0', tt0009_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->ref_flag, sz_recvbufcat + gbp->count, tt0009_FLAG_LEN );
        ap_rprintf(r,"		<REF_FLAG>%s</REF_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->ref_flag));
    gbp->count += tt0009_FLAG_LEN;

	memset(ptr_recvbuf->ups_type, '\0', tt0009_UPS_TYPE_LEN + 1);
    memcpy(ptr_recvbuf->ups_type, sz_recvbufcat + gbp->count, tt0009_UPS_TYPE_LEN );
        ap_rprintf(r,"		<UPS_TYPE>%s</UPS_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->ups_type));
    gbp->count += tt0009_UPS_TYPE_LEN;

	memset(ptr_recvbuf->gender, '\0', tt0009_GENDER_LEN + 1);
    memcpy(ptr_recvbuf->gender, sz_recvbufcat + gbp->count, tt0009_GENDER_LEN );
        ap_rprintf(r,"		<GENDER>%s</GENDER>\n", handle_special_chars(gbp,ptr_recvbuf->gender));
    gbp->count += tt0009_GENDER_LEN;

	memset(ptr_recvbuf->language, '\0', tt0009_LANGUAGE_LEN + 1);
    memcpy(ptr_recvbuf->language, sz_recvbufcat + gbp->count, tt0009_LANGUAGE_LEN );
        ap_rprintf(r,"		<LANGUAGE>%s</LANGUAGE>\n", handle_special_chars(gbp,ptr_recvbuf->language));
    gbp->count += tt0009_LANGUAGE_LEN;

	memset(ptr_recvbuf->post_code, '\0', tt0009_POST_CODE_LEN + 1);
    memcpy(ptr_recvbuf->post_code, sz_recvbufcat + gbp->count, tt0009_POST_CODE_LEN );
        ap_rprintf(r,"		<POST_CODE>%s</POST_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->post_code));
    gbp->count += tt0009_POST_CODE_LEN;

	memset(ptr_recvbuf->cust_type, '\0', tt0009_CUST_TYPE_LEN + 1);
    memcpy(ptr_recvbuf->cust_type, sz_recvbufcat + gbp->count, tt0009_CUST_TYPE_LEN );
        ap_rprintf(r,"		<CUST_TYPE>%s</CUST_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_type));
    gbp->count += tt0009_CUST_TYPE_LEN;

	memset(ptr_recvbuf->cust_stat, '\0', tt0009_CUST_STATUS_LEN + 1);
    memcpy(ptr_recvbuf->cust_stat, sz_recvbufcat + gbp->count, tt0009_CUST_STATUS_LEN );
        ap_rprintf(r,"		<CUST_STATUS>%s</CUST_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->cust_stat));
    gbp->count += tt0009_CUST_STATUS_LEN;

	memset(ptr_recvbuf->promote, '\0', tt0009_PROMOTE_LEN + 1);
    memcpy(ptr_recvbuf->promote, sz_recvbufcat + gbp->count, tt0009_PROMOTE_LEN );
        ap_rprintf(r,"		<PROMOTE>%s</PROMOTE>\n", handle_special_chars(gbp,ptr_recvbuf->promote));
    gbp->count += tt0009_PROMOTE_LEN;

	memset(ptr_recvbuf->rent, '\0', tt0009_RENT_LEN + 1);
    memcpy(ptr_recvbuf->rent, sz_recvbufcat + gbp->count, tt0009_RENT_LEN );
        ap_rprintf(r,"		<RENT>%s</RENT>\n", handle_special_chars(gbp,ptr_recvbuf->rent));
    gbp->count += tt0009_RENT_LEN;

	memset(ptr_recvbuf->member_id, '\0', tt0009_MEMBER_ID_LEN + 1);
    memcpy(ptr_recvbuf->member_id, sz_recvbufcat + gbp->count, tt0009_MEMBER_ID_LEN );
        ap_rprintf(r,"		<MEMBER_ID>%s</MEMBER_ID>\n", handle_special_chars(gbp,ptr_recvbuf->member_id));
    gbp->count += tt0009_MEMBER_ID_LEN;

	memset(ptr_recvbuf->contract_no, '\0', tt0009_CONTRACT_NO_LEN + 1);
    memcpy(ptr_recvbuf->contract_no, sz_recvbufcat + gbp->count, tt0009_CONTRACT_NO_LEN );
        ap_rprintf(r,"		<CONTRACT_NO>%s</CONTRACT_NO>\n", handle_special_chars(gbp,ptr_recvbuf->contract_no));
    gbp->count += tt0009_CONTRACT_NO_LEN;

	memset(ptr_recvbuf->credit_limit, '\0', tt0009_CREDIT_LIMIT_LEN + 1);
    memcpy(ptr_recvbuf->credit_limit, sz_recvbufcat + gbp->count, tt0009_CREDIT_LIMIT_LEN );
        ap_rprintf(r,"		<CREDIT_LIMIT>%s</CREDIT_LIMIT>\n", handle_special_chars(gbp,ptr_recvbuf->credit_limit));
    gbp->count += tt0009_CREDIT_LIMIT_LEN;

	memset(ptr_recvbuf->terms_days, '\0', tt0009_TERMS_DAYS_LEN + 1);
    memcpy(ptr_recvbuf->terms_days, sz_recvbufcat + gbp->count, tt0009_TERMS_DAYS_LEN );
        ap_rprintf(r,"		<TERMS_DAYS>%s</TERMS_DAYS>\n", handle_special_chars(gbp,ptr_recvbuf->terms_days));
    gbp->count += tt0009_TERMS_DAYS_LEN;

	memset(ptr_recvbuf->terms_pct, '\0', tt0009_TERMS_PCT_LEN + 1);
    memcpy(ptr_recvbuf->terms_pct, sz_recvbufcat + gbp->count, tt0009_TERMS_PCT_LEN );
        ap_rprintf(r,"		<TERMS_PCT>%s</TERMS_PCT>\n", handle_special_chars(gbp,ptr_recvbuf->terms_pct));
    gbp->count += tt0009_TERMS_PCT_LEN;

	memset(ptr_recvbuf->norm_days, '\0', tt0009_NORM_DAYS_LEN + 1);
    memcpy(ptr_recvbuf->norm_days, sz_recvbufcat + gbp->count, tt0009_NORM_DAYS_LEN );
        ap_rprintf(r,"		<NORM_DAYS>%s</NORM_DAYS>\n", handle_special_chars(gbp,ptr_recvbuf->norm_days));
    gbp->count += tt0009_NORM_DAYS_LEN;

	memset(ptr_recvbuf->total_open, '\0', tt0009_TOTAL_OPEN_LEN + 1);
    memcpy(ptr_recvbuf->total_open, sz_recvbufcat + gbp->count, tt0009_TOTAL_OPEN_LEN );
        ap_rprintf(r,"		<TOTAL_OPEN>%s</TOTAL_OPEN>\n", handle_special_chars(gbp,ptr_recvbuf->total_open));
    gbp->count += tt0009_TOTAL_OPEN_LEN;

	memset(ptr_recvbuf->total_potential, '\0', tt0009_TOTAL_POTENTIAL_LEN + 1);
    memcpy(ptr_recvbuf->total_potential, sz_recvbufcat + gbp->count, tt0009_TOTAL_POTENTIAL_LEN );
        ap_rprintf(r,"		<TOTAL_POTENTIAL>%s</TOTAL_POTENTIAL>\n", handle_special_chars(gbp,ptr_recvbuf->total_potential));
    gbp->count += tt0009_TOTAL_POTENTIAL_LEN;

	memset(ptr_recvbuf->disc_pct, '\0', tt0009_DISC_PCT_LEN + 1);
    memcpy(ptr_recvbuf->disc_pct, sz_recvbufcat + gbp->count, tt0009_DISC_PCT_LEN );
        ap_rprintf(r,"		<DISC_PCT>%s</DISC_PCT>\n", handle_special_chars(gbp,ptr_recvbuf->disc_pct));
    gbp->count += tt0009_DISC_PCT_LEN;

	memset(ptr_recvbuf->cod_limit, '\0', tt0009_COD_LIMIT_LEN + 1);
    memcpy(ptr_recvbuf->cod_limit, sz_recvbufcat + gbp->count, tt0009_COD_LIMIT_LEN );
        ap_rprintf(r,"		<COD_LIMIT>%s</COD_LIMIT>\n", handle_special_chars(gbp,ptr_recvbuf->cod_limit));
    gbp->count += tt0009_COD_LIMIT_LEN;

	memset(ptr_recvbuf->cod_comment_cd, '\0', tt0009_COD_COMMENT_CD_LEN + 1);
    memcpy(ptr_recvbuf->cod_comment_cd, sz_recvbufcat + gbp->count, tt0009_COD_COMMENT_CD_LEN );
        ap_rprintf(r,"		<COD_COMMENT_CD>%s</COD_COMMENT_CD>\n", handle_special_chars(gbp,ptr_recvbuf->cod_comment_cd));
    gbp->count += tt0009_COD_COMMENT_CD_LEN;

	memset(ptr_recvbuf->sales_territory, '\0', tt0009_SALES_TERRITORY_LEN + 1);
    memcpy(ptr_recvbuf->sales_territory, sz_recvbufcat + gbp->count, tt0009_SALES_TERRITORY_LEN );
        ap_rprintf(r,"		<SALES_TERRITORY>%s</SALES_TERRITORY>\n", handle_special_chars(gbp,ptr_recvbuf->sales_territory));
    gbp->count += tt0009_SALES_TERRITORY_LEN;

	memset(ptr_recvbuf->sales_rep, '\0', tt0009_SALES_REP_LEN + 1);
    memcpy(ptr_recvbuf->sales_rep, sz_recvbufcat + gbp->count, tt0009_SALES_REP_LEN );
        ap_rprintf(r,"		<SALES_REP>%s</SALES_REP>\n", handle_special_chars(gbp,ptr_recvbuf->sales_rep));
    gbp->count += tt0009_SALES_REP_LEN;

	memset(ptr_recvbuf->inv_req, '\0', tt0009_INV_REQ_LEN + 1);
    memcpy(ptr_recvbuf->inv_req, sz_recvbufcat + gbp->count, tt0009_INV_REQ_LEN );
        ap_rprintf(r,"		<INV_REQ>%s</INV_REQ>\n", handle_special_chars(gbp,ptr_recvbuf->inv_req));
    gbp->count += tt0009_INV_REQ_LEN;

	memset(ptr_recvbuf->inv_hand, '\0', tt0009_INV_HAND_LEN + 1);
    memcpy(ptr_recvbuf->inv_hand, sz_recvbufcat + gbp->count, tt0009_INV_HAND_LEN );
        ap_rprintf(r,"		<INV_HAND>%s</INV_HAND>\n", handle_special_chars(gbp,ptr_recvbuf->inv_hand));
    gbp->count += tt0009_INV_HAND_LEN;

	memset(ptr_recvbuf->tax_exempt, '\0', tt0009_TAX_EXEMPT_LEN + 1);
    memcpy(ptr_recvbuf->tax_exempt, sz_recvbufcat + gbp->count, tt0009_TAX_EXEMPT_LEN );
        ap_rprintf(r,"		<TAX_EXEMPT>%s</TAX_EXEMPT>\n", handle_special_chars(gbp,ptr_recvbuf->tax_exempt));
    gbp->count += tt0009_TAX_EXEMPT_LEN;

	memset(ptr_recvbuf->std_disc, '\0', tt0009_STD_DISC_LEN + 1);
    memcpy(ptr_recvbuf->std_disc, sz_recvbufcat + gbp->count, tt0009_STD_DISC_LEN );
        ap_rprintf(r,"		<STD_DISC>%s</STD_DISC>\n", handle_special_chars(gbp,ptr_recvbuf->std_disc));
    gbp->count += tt0009_STD_DISC_LEN;

	memset(ptr_recvbuf->tax_id, '\0', tt0009_TAX_ID_LEN + 1);
    memcpy(ptr_recvbuf->tax_id, sz_recvbufcat + gbp->count, tt0009_TAX_ID_LEN );
        ap_rprintf(r,"		<TAX_ID>%s</TAX_ID>\n", handle_special_chars(gbp,ptr_recvbuf->tax_id));
    gbp->count += tt0009_TAX_ID_LEN;

	memset(ptr_recvbuf->mbr_duration, '\0', tt0009_MBR_DURATION_LEN + 1);
    memcpy(ptr_recvbuf->mbr_duration, sz_recvbufcat + gbp->count, tt0009_MBR_DURATION_LEN );
        ap_rprintf(r,"		<MBR_DURATION>%s</MBR_DURATION>\n", handle_special_chars(gbp,ptr_recvbuf->mbr_duration));
    gbp->count += tt0009_MBR_DURATION_LEN;

	memset(ptr_recvbuf->mbr_act_chain, '\0', tt0009_MBR_ACT_CHAIN_LEN + 1);
    memcpy(ptr_recvbuf->mbr_act_chain, sz_recvbufcat + gbp->count, tt0009_MBR_ACT_CHAIN_LEN );
        ap_rprintf(r,"		<MBR_ACT_CHAIN>%s</MBR_ACT_CHAIN>\n", handle_special_chars(gbp,ptr_recvbuf->mbr_act_chain));
    gbp->count += tt0009_MBR_ACT_CHAIN_LEN;

	memset(ptr_recvbuf->mbr_exp_code, '\0', tt0009_MBR_EXP_CODE_LEN + 1);
    memcpy(ptr_recvbuf->mbr_exp_code, sz_recvbufcat + gbp->count, tt0009_MBR_EXP_CODE_LEN );
        ap_rprintf(r,"		<MBR_EXP_CODE>%s</MBR_EXP_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->mbr_exp_code));
    gbp->count += tt0009_MBR_EXP_CODE_LEN;

	memset(ptr_recvbuf->postg_exmpt, '\0', tt0009_POSTG_EXMPT_LEN + 1);
    memcpy(ptr_recvbuf->postg_exmpt, sz_recvbufcat + gbp->count, tt0009_POSTG_EXMPT_LEN );
        ap_rprintf(r,"		<POSTG_EXMPT>%s</POSTG_EXMPT>\n", handle_special_chars(gbp,ptr_recvbuf->postg_exmpt));
    gbp->count += tt0009_POSTG_EXMPT_LEN;

	memset(ptr_recvbuf->insur_exmpt, '\0', tt0009_INSUR_EXMPT_LEN + 1);
    memcpy(ptr_recvbuf->insur_exmpt, sz_recvbufcat + gbp->count, tt0009_INSUR_EXMPT_LEN );
        ap_rprintf(r,"		<INSUR_EXMPT>%s</INSUR_EXMPT>\n", handle_special_chars(gbp,ptr_recvbuf->insur_exmpt));
    gbp->count += tt0009_INSUR_EXMPT_LEN;

	memset(ptr_recvbuf->mbr_init_date, '\0', tt0009_MBR_INIT_DATE_LEN + 1);
    memcpy(ptr_recvbuf->mbr_init_date, sz_recvbufcat + gbp->count, tt0009_MBR_INIT_DATE_LEN );
        ap_rprintf(r,"		<MBR_INIT_DATE>%s</MBR_INIT_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->mbr_init_date));
    gbp->count += tt0009_MBR_INIT_DATE_LEN;

	memset(ptr_recvbuf->supp_stmt, '\0', tt0009_SUPP_STMT_LEN + 1);
    memcpy(ptr_recvbuf->supp_stmt, sz_recvbufcat + gbp->count, tt0009_SUPP_STMT_LEN );
        ap_rprintf(r,"		<SUPP_STMT>%s</SUPP_STMT>\n", handle_special_chars(gbp,ptr_recvbuf->supp_stmt));
    gbp->count += tt0009_SUPP_STMT_LEN;

	memset(ptr_recvbuf->bill_complete, '\0', tt0009_BILL_COMPLETE_LEN + 1);
    memcpy(ptr_recvbuf->bill_complete, sz_recvbufcat + gbp->count, tt0009_BILL_COMPLETE_LEN );
        ap_rprintf(r,"		<BILL_COMPLETE>%s</BILL_COMPLETE>\n", handle_special_chars(gbp,ptr_recvbuf->bill_complete));
    gbp->count += tt0009_BILL_COMPLETE_LEN;

	memset(ptr_recvbuf->ca_status, '\0', tt0009_CA_STATUS_LEN + 1);
    memcpy(ptr_recvbuf->ca_status, sz_recvbufcat + gbp->count, tt0009_CA_STATUS_LEN );
        ap_rprintf(r,"		<CA_STATUS>%s</CA_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->ca_status));
    gbp->count += tt0009_CA_STATUS_LEN;

	memset(ptr_recvbuf->mbr_exp_date, '\0', tt0009_MBR_EXP_DATE_LEN + 1);
    memcpy(ptr_recvbuf->mbr_exp_date, sz_recvbufcat + gbp->count, tt0009_MBR_EXP_DATE_LEN );
        ap_rprintf(r,"		<MBR_EXP_DATE>%s</MBR_EXP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->mbr_exp_date));
    gbp->count += tt0009_MBR_EXP_DATE_LEN;

	memset(ptr_recvbuf->mbr_status, '\0', tt0009_MBR_STATUS_LEN + 1);
    memcpy(ptr_recvbuf->mbr_status, sz_recvbufcat + gbp->count, tt0009_MBR_STATUS_LEN );
        ap_rprintf(r,"		<MBR_STATUS>%s</MBR_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->mbr_status));
    gbp->count += tt0009_MBR_STATUS_LEN;

	memset(ptr_recvbuf->orig_date, '\0', tt0009_ORIG_DATE_LEN + 1);
    memcpy(ptr_recvbuf->orig_date, sz_recvbufcat + gbp->count, tt0009_ORIG_DATE_LEN );
        ap_rprintf(r,"		<ORIG_DATE>%s</ORIG_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->orig_date));
    gbp->count += tt0009_ORIG_DATE_LEN;

	memset(ptr_recvbuf->middle_ini, '\0', tt0009_MI_LEN + 1);
    memcpy(ptr_recvbuf->middle_ini, sz_recvbufcat + gbp->count, tt0009_MI_LEN );
        ap_rprintf(r,"		<MID_INI>%s</MID_INI>\n", handle_special_chars(gbp,ptr_recvbuf->middle_ini));
    gbp->count += tt0009_MI_LEN;
	  
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0010>\n", gbp->tt_betag);

    return 0;
}















