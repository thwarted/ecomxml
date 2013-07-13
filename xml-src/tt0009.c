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


int		tt0009_CatSendStr(struct global_struct *, char *, tt0009_st_send *);
int		tt0009_ParceRecvStr(tt0009_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0009_lt_process(request_rec *, struct global_struct *, char *);



int tt0009_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)			/* Get the current working path */
		return(-1);

    GetInf(gbp);							/* Read info from INF file */


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


	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0010>\n", gbp->tt_betag);

    return 0;
}















