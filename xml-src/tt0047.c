/* tt0047.c Get Order Info */
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


int		tt0047_CatSendStr(struct global_struct *, char *, tt0047_st_send *);
int		tt0047_ParceRecvStr(tt0047_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0047_lt_process(request_rec *, struct global_struct *, char *);


int tt0047_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0047_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0047_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0047_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0047 = (tt0047_st_send *)malloc(sizeof(tt0047_st_send));
	gbp->recvbuf0047 = (tt0047_st_recv *)malloc(sizeof(tt0047_st_recv));

	gbp->sendbufcat = malloc(tt0047_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0047_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0047_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0047_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0047,  '\0',  sizeof(tt0047_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0047->request_id, "XML");
    strcpy(gbp->sendbuf0047->record_id, "0047");

	strcpy(gbp->sendbuf0047->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0047->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0047->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0047->userid,gbp,stdout_buffer);
	

	if(tt0047_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0047) == tt0047_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0047);
			free (gbp->recvbuf0047);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0047_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0047);
			free (gbp->recvbuf0047);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0047_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0047);
			free (gbp->recvbuf0047);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0047);
			free (gbp->recvbuf0047);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0047_ParceRecvStr(gbp->recvbuf0047, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0047);
		free (gbp->recvbuf0047);
	}
	else
	{
		XML_Error("tt0047_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0047);
		free (gbp->recvbuf0047);
		return(-1);
	}

    return(0);
}


int tt0047_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0047_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler);
            
    return(gbp->j);
}


int tt0047_ParceRecvStr(tt0047_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
	gbp->count = 0;
 	
	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0048.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0048 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0048.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0048 %s\"tt0048\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0047_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0047_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0047_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0047_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0047_REC_ID_LEN );
    gbp->count += tt0047_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0047_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0047_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0047_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0047_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0047_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0047_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0047_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0047_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0047_ERR_LEN;

	gbp->count += tt0047_SEND_FILLER_LEN;


	memset(ptr_recvbuf->pay_method,'\0', tt0047_PAY_METH_LEN+1);
    memcpy(ptr_recvbuf->pay_method, sz_recvbufcat + gbp->count, tt0047_PAY_METH_LEN);
	ap_rprintf(r,"		<PAY_METHOD>%s</PAY_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->pay_method));
    gbp->count += tt0047_PAY_METH_LEN;

	memset(ptr_recvbuf->fname,'\0', tt0047_FN_LEN+1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0047_FN_LEN);
	ap_rprintf(r,"		<FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += tt0047_FN_LEN;

	memset(ptr_recvbuf->minit,'\0', tt0047_MI_LEN+1);
    memcpy(ptr_recvbuf->minit, sz_recvbufcat + gbp->count, tt0047_MI_LEN);
	ap_rprintf(r,"		<MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->minit));
    gbp->count += tt0047_MI_LEN;

	memset(ptr_recvbuf->lname,'\0', tt0047_LN_LEN+1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0047_LN_LEN);
	ap_rprintf(r,"		<LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += tt0047_LN_LEN;

	memset(ptr_recvbuf->title_code,'\0', tt0047_TITLE_LEN+1);
    memcpy(ptr_recvbuf->title_code, sz_recvbufcat + gbp->count, tt0047_TITLE_LEN);
	ap_rprintf(r,"		<TITLE_CODE>%s</TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->title_code));
    gbp->count += tt0047_TITLE_LEN;

	memset(ptr_recvbuf->company_name,'\0', tt0047_COMP_LEN+1);
    memcpy(ptr_recvbuf->company_name, sz_recvbufcat + gbp->count, tt0047_COMP_LEN);
	ap_rprintf(r,"		<COMPANY_NAME>%s</COMPANY_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->company_name));
    gbp->count += tt0047_COMP_LEN;

	memset(ptr_recvbuf->jap_flag,'\0', tt0047_JAP_LEN+1);
    memcpy(ptr_recvbuf->jap_flag, sz_recvbufcat + gbp->count, tt0047_JAP_LEN);
	ap_rprintf(r,"		<JAP_FLAG>%s</JAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->jap_flag));
    gbp->count += tt0047_JAP_LEN;

	memset(ptr_recvbuf->addr_ref1,'\0', tt0047_REF_LEN+1);
    memcpy(ptr_recvbuf->addr_ref1, sz_recvbufcat + gbp->count, tt0047_REF_LEN);
	ap_rprintf(r,"		<ADDR_REF1>%s</ADDR_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->addr_ref1));
    gbp->count += tt0047_REF_LEN;

	memset(ptr_recvbuf->addr_ref2,'\0', tt0047_REF_LEN+1);
    memcpy(ptr_recvbuf->addr_ref2, sz_recvbufcat + gbp->count, tt0047_REF_LEN);
	ap_rprintf(r,"		<ADDR_REF2>%s</ADDR_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->addr_ref2));
    gbp->count += tt0047_REF_LEN;

	memset(ptr_recvbuf->street,'\0', tt0047_STREET_LEN+1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, tt0047_STREET_LEN);
	ap_rprintf(r,"		<STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));
    gbp->count += tt0047_STREET_LEN;

	memset(ptr_recvbuf->city,'\0', tt0047_CITY_LEN+1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, tt0047_CITY_LEN);
	ap_rprintf(r,"		<CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));
    gbp->count += tt0047_CITY_LEN;

	memset(ptr_recvbuf->state,'\0', tt0047_STATE_LEN+1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, tt0047_STATE_LEN);
	ap_rprintf(r,"		<STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));
    gbp->count += tt0047_STATE_LEN;

	memset(ptr_recvbuf->zipcode,'\0', tt0047_ZIP_LEN+1);
    memcpy(ptr_recvbuf->zipcode, sz_recvbufcat + gbp->count, tt0047_ZIP_LEN);
	ap_rprintf(r,"		<ZIPCODE>%s</ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->zipcode));
    gbp->count += tt0047_ZIP_LEN;

	memset(ptr_recvbuf->country,'\0', tt0047_COUNTRY_LEN+1);
    memcpy(ptr_recvbuf->country, sz_recvbufcat + gbp->count, tt0047_COUNTRY_LEN);
	ap_rprintf(r,"		<COUNTRY>%s</COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->country));
    gbp->count += tt0047_COUNTRY_LEN;

	memset(ptr_recvbuf->day_phone,'\0', tt0047_PHONE_LEN+1);
    memcpy(ptr_recvbuf->day_phone, sz_recvbufcat + gbp->count, tt0047_PHONE_LEN);
	ap_rprintf(r,"		<DAY_PHONE>%s</DAY_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->day_phone));
    gbp->count += tt0047_PHONE_LEN;

	memset(ptr_recvbuf->ngt_phone,'\0', tt0047_PHONE_LEN+1);
    memcpy(ptr_recvbuf->ngt_phone, sz_recvbufcat + gbp->count, tt0047_PHONE_LEN);
	ap_rprintf(r,"		<NGT_PHONE>%s</NGT_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->ngt_phone));
    gbp->count += tt0047_PHONE_LEN;

	memset(ptr_recvbuf->no_promo,'\0', tt0047_PROMO_LEN+1);
    memcpy(ptr_recvbuf->no_promo, sz_recvbufcat + gbp->count, tt0047_PROMO_LEN);
	ap_rprintf(r,"		<NO_PROMO>%s</NO_PROMO>\n", handle_special_chars(gbp,ptr_recvbuf->no_promo));
    gbp->count += tt0047_PROMO_LEN;

	memset(ptr_recvbuf->no_rent,'\0', tt0047_RENT_LEN+1);
    memcpy(ptr_recvbuf->no_rent, sz_recvbufcat + gbp->count, tt0047_RENT_LEN);
	ap_rprintf(r,"		<NO_RENT>%s</NO_RENT>\n", handle_special_chars(gbp,ptr_recvbuf->no_rent));
    gbp->count += tt0047_RENT_LEN;

	memset(ptr_recvbuf->remit_amt,'\0', tt0047_REMIT_LEN+1);
    memcpy(ptr_recvbuf->remit_amt, sz_recvbufcat + gbp->count, tt0047_REMIT_LEN);
	ap_rprintf(r,"		<REMIT_AMT>%s</REMIT_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->remit_amt));
    gbp->count += tt0047_REMIT_LEN;

	memset(ptr_recvbuf->future_date,'\0', tt0047_FO_LEN+1);
    memcpy(ptr_recvbuf->future_date, sz_recvbufcat + gbp->count, tt0047_FO_LEN);
	ap_rprintf(r,"		<FUTURE_DATE>%s</FUTURE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->future_date));
    gbp->count += tt0047_FO_LEN;

	memset(ptr_recvbuf->po_num,'\0', tt0047_PO_LEN+1);
    memcpy(ptr_recvbuf->po_num, sz_recvbufcat + gbp->count, tt0047_PO_LEN);
	ap_rprintf(r,"		<PO_NUM>%s</PO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->po_num));
    gbp->count += tt0047_PO_LEN;

	memset(ptr_recvbuf->hold_complete,'\0', tt0047_HOLD_COMP_LEN+1);
    memcpy(ptr_recvbuf->hold_complete, sz_recvbufcat + gbp->count, tt0047_HOLD_COMP_LEN);
	ap_rprintf(r,"		<HOLD_COMPLETE>%s</HOLD_COMPLETE>\n", handle_special_chars(gbp,ptr_recvbuf->hold_complete));
    gbp->count += tt0047_HOLD_COMP_LEN;

	memset(ptr_recvbuf->expedite_ord,'\0', tt0047_EXP_ORD_LEN+1);
    memcpy(ptr_recvbuf->expedite_ord, sz_recvbufcat + gbp->count, tt0047_EXP_ORD_LEN);
	ap_rprintf(r,"		<EXPEDITE_ORD>%s</EXPEDITE_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->expedite_ord));
    gbp->count += tt0047_EXP_ORD_LEN;

	memset(ptr_recvbuf->no_backord,'\0', tt0047_BACKORD_LEN+1);
    memcpy(ptr_recvbuf->no_backord, sz_recvbufcat + gbp->count, tt0047_BACKORD_LEN);
	ap_rprintf(r,"		<NO_BACKORD>%s</NO_BACKORD>\n", handle_special_chars(gbp,ptr_recvbuf->no_backord));
    gbp->count += tt0047_BACKORD_LEN;

	memset(ptr_recvbuf->postpone_ord,'\0', tt0047_POST_ORD_LEN+1);
    memcpy(ptr_recvbuf->postpone_ord, sz_recvbufcat + gbp->count, tt0047_POST_ORD_LEN);
	ap_rprintf(r,"		<POSTPONE_ORD>%s</POSTPONE_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->postpone_ord));
    gbp->count += tt0047_POST_ORD_LEN;

	memset(ptr_recvbuf->no_insurance,'\0', tt0047_INSUR_LEN+1);
    memcpy(ptr_recvbuf->no_insurance, sz_recvbufcat + gbp->count, tt0047_INSUR_LEN);
	ap_rprintf(r,"		<NO_INSURANCE>%s</NO_INSURANCE>\n", handle_special_chars(gbp,ptr_recvbuf->no_insurance));
    gbp->count += tt0047_INSUR_LEN;

	memset(ptr_recvbuf->cc_up_front,'\0', tt0047_UP_FRT_LEN+1);
    memcpy(ptr_recvbuf->cc_up_front, sz_recvbufcat + gbp->count, tt0047_UP_FRT_LEN);
	ap_rprintf(r,"		<CC_UP_FRONT>%s</CC_UP_FRONT>\n", handle_special_chars(gbp,ptr_recvbuf->cc_up_front));
    gbp->count += tt0047_UP_FRT_LEN;

	memset(ptr_recvbuf->overpayment,'\0', tt0047_OVERPAYMENT_LEN+1);
    memcpy(ptr_recvbuf->overpayment, sz_recvbufcat + gbp->count, tt0047_OVERPAYMENT_LEN);
	ap_rprintf(r,"		<OVERPAYMENT>%s</OVERPAYMENT>\n", handle_special_chars(gbp,ptr_recvbuf->overpayment));
    gbp->count += tt0047_OVERPAYMENT_LEN;

	memset(ptr_recvbuf->standing_ord,'\0', tt0047_STAND_ORD_LEN+1);
    memcpy(ptr_recvbuf->standing_ord, sz_recvbufcat + gbp->count, tt0047_STAND_ORD_LEN);
	ap_rprintf(r,"		<STANDING_ORD>%s</STANDING_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->standing_ord));
    gbp->count += tt0047_STAND_ORD_LEN;

	memset(ptr_recvbuf->gender,'\0', tt0047_GENDER_LEN+1);
    memcpy(ptr_recvbuf->gender, sz_recvbufcat + gbp->count, tt0047_GENDER_LEN);
	ap_rprintf(r,"		<GENDER>%s</GENDER>\n", handle_special_chars(gbp,ptr_recvbuf->gender));
    gbp->count += tt0047_GENDER_LEN;

	memset(ptr_recvbuf->ship_method,'\0', tt0047_SHIP_METH_LEN+1);
    memcpy(ptr_recvbuf->ship_method, sz_recvbufcat + gbp->count, tt0047_SHIP_METH_LEN);
	ap_rprintf(r,"		<SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method));
    gbp->count += tt0047_SHIP_METH_LEN;

	memset(ptr_recvbuf->demograph1,'\0', tt0047_DEMOGRAPH_LEN+1);
    memcpy(ptr_recvbuf->demograph1, sz_recvbufcat + gbp->count, tt0047_DEMOGRAPH_LEN);
	ap_rprintf(r,"		<DEMOGRAPH1>%s</DEMOGRAPH1>\n", handle_special_chars(gbp,ptr_recvbuf->demograph1));
    gbp->count += tt0047_DEMOGRAPH_LEN;

	memset(ptr_recvbuf->demograph2,'\0', tt0047_DEMOGRAPH_LEN+1);
    memcpy(ptr_recvbuf->demograph2, sz_recvbufcat + gbp->count, tt0047_DEMOGRAPH_LEN);
	ap_rprintf(r,"		<DEMOGRAPH2>%s</DEMOGRAPH2>\n", handle_special_chars(gbp,ptr_recvbuf->demograph2));
    gbp->count += tt0047_DEMOGRAPH_LEN;

	memset(ptr_recvbuf->fax_phone,'\0', tt0047_FAX_LEN+1);
    memcpy(ptr_recvbuf->fax_phone, sz_recvbufcat + gbp->count, tt0047_FAX_LEN);
	ap_rprintf(r,"		<FAX_PHONE>%s</FAX_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->fax_phone));
    gbp->count += tt0047_FAX_LEN;

	memset(ptr_recvbuf->order_xref,'\0', tt0047_ORD_XREF_LEN+1);
    memcpy(ptr_recvbuf->order_xref, sz_recvbufcat + gbp->count, tt0047_ORD_XREF_LEN);
	ap_rprintf(r,"		<ORDER_XREF>%s</ORDER_XREF>\n", handle_special_chars(gbp,ptr_recvbuf->order_xref));
    gbp->count += tt0047_ORD_XREF_LEN;

	memset(ptr_recvbuf->cust_type,'\0', tt0047_CUST_TYPE_LEN+1);
    memcpy(ptr_recvbuf->cust_type, sz_recvbufcat + gbp->count, tt0047_CUST_TYPE_LEN);
	ap_rprintf(r,"		<CUST_TYPE>%s</CUST_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cust_type));
    gbp->count += tt0047_CUST_TYPE_LEN;

	memset(ptr_recvbuf->installments,'\0', tt0047_INSTAL_LEN+1);
    memcpy(ptr_recvbuf->installments, sz_recvbufcat + gbp->count, tt0047_INSTAL_LEN);
	ap_rprintf(r,"		<INSTALLMENTS>%s</INSTALLMENTS>\n", handle_special_chars(gbp,ptr_recvbuf->installments));
    gbp->count += tt0047_INSTAL_LEN;

	memset(ptr_recvbuf->discount_code1,'\0', tt0047_DISC_LEN+1);
    memcpy(ptr_recvbuf->discount_code1, sz_recvbufcat + gbp->count, tt0047_DISC_LEN);
	ap_rprintf(r,"		<DISCOUNT_CODE1>%s</DISCOUNT_CODE1>\n", handle_special_chars(gbp,ptr_recvbuf->discount_code1));
    gbp->count += tt0047_DISC_LEN;

	memset(ptr_recvbuf->discount_code2,'\0', tt0047_DISC_LEN+1);
    memcpy(ptr_recvbuf->discount_code2, sz_recvbufcat + gbp->count, tt0047_DISC_LEN);
	ap_rprintf(r,"		<DISCOUNT_CODE2>%s</DISCOUNT_CODE2>\n", handle_special_chars(gbp,ptr_recvbuf->discount_code2));
    gbp->count += tt0047_DISC_LEN;

	memset(ptr_recvbuf->birthday,'\0', tt0047_BIRTHDAY_LEN+1);
    memcpy(ptr_recvbuf->birthday, sz_recvbufcat + gbp->count, tt0047_BIRTHDAY_LEN);
	ap_rprintf(r,"		<BIRTHDAY>%s</BIRTHDAY>\n", handle_special_chars(gbp,ptr_recvbuf->birthday));
    gbp->count += tt0047_BIRTHDAY_LEN;

	memset(ptr_recvbuf->password,'\0', tt0047_PASSWORD_LEN+1);
    memcpy(ptr_recvbuf->password, sz_recvbufcat + gbp->count, tt0047_PASSWORD_LEN);
	ap_rprintf(r,"		<PASSWORD>%s</PASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->password));
    gbp->count += tt0047_PASSWORD_LEN;

	memset(ptr_recvbuf->email_address,'\0', tt0047_EMAIL_LEN+1);
    memcpy(ptr_recvbuf->email_address, sz_recvbufcat + gbp->count, tt0047_EMAIL_LEN);
	ap_rprintf(r,"		<EMAIL_ADDR>%s</EMAIL_ADDR>\n", handle_special_chars(gbp,ptr_recvbuf->email_address));
    gbp->count += tt0047_EMAIL_LEN;

	memset(ptr_recvbuf->cc_type,'\0', tt0047_CC_TYPE_LEN+1);
    memcpy(ptr_recvbuf->cc_type, sz_recvbufcat + gbp->count, tt0047_CC_TYPE_LEN);
	ap_rprintf(r,"		<CC_TYPE>%s</CC_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cc_type));
    gbp->count += tt0047_CC_TYPE_LEN;

	memset(ptr_recvbuf->cc_num,'\0', tt0047_CC_NUM_LEN+1);
    memcpy(ptr_recvbuf->cc_num, sz_recvbufcat + gbp->count, tt0047_CC_NUM_LEN);
	ap_rprintf(r,"		<CC_NUM>%s</CC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->cc_num));
    gbp->count += tt0047_CC_NUM_LEN;

	memset(ptr_recvbuf->cc_exp_month,'\0', tt0047_CC_EXP_LEN+1);
    memcpy(ptr_recvbuf->cc_exp_month, sz_recvbufcat + gbp->count, tt0047_CC_EXP_LEN);
	ap_rprintf(r,"		<CC_EXP_MONTH>%s</CC_EXP_MONTH>\n", handle_special_chars(gbp,ptr_recvbuf->cc_exp_month));
    gbp->count += tt0047_CC_EXP_LEN;

	memset(ptr_recvbuf->cc_exp_year,'\0', tt0047_CC_EXP_LEN+1);
    memcpy(ptr_recvbuf->cc_exp_year, sz_recvbufcat + gbp->count, tt0047_CC_EXP_LEN);
	ap_rprintf(r,"		<CC_EXP_YEAR>%s</CC_EXP_YEAR>\n", handle_special_chars(gbp,ptr_recvbuf->cc_exp_year));
    gbp->count += tt0047_CC_EXP_LEN;

	memset(ptr_recvbuf->shipto_num,'\0', tt0047_SHIPTO_NO_LEN+1);
    memcpy(ptr_recvbuf->shipto_num, sz_recvbufcat + gbp->count, tt0047_SHIPTO_NO_LEN);
	ap_rprintf(r,"		<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_num));
    gbp->count += tt0047_SHIPTO_NO_LEN;

	memset(ptr_recvbuf->shipto_fname,'\0', tt0047_SHIPTO_FN_LEN+1);
    memcpy(ptr_recvbuf->shipto_fname, sz_recvbufcat + gbp->count, tt0047_SHIPTO_FN_LEN);
	ap_rprintf(r,"		<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_fname));
    gbp->count += tt0047_SHIPTO_FN_LEN;

	memset(ptr_recvbuf->shipto_minit,'\0', tt0047_SHIPTO_MI_LEN+1);
    memcpy(ptr_recvbuf->shipto_minit, sz_recvbufcat + gbp->count, tt0047_SHIPTO_MI_LEN);
	ap_rprintf(r,"		<SHIPTO_MINITIAL>%s</SHIPTO_MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_minit));
    gbp->count += tt0047_SHIPTO_MI_LEN;

	memset(ptr_recvbuf->shipto_lname,'\0', tt0047_SHIPTO_LN_LEN+1);
    memcpy(ptr_recvbuf->shipto_lname, sz_recvbufcat + gbp->count, tt0047_SHIPTO_LN_LEN);
	ap_rprintf(r,"		<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_lname));
    gbp->count += tt0047_SHIPTO_LN_LEN;

	memset(ptr_recvbuf->shipto_title_code,'\0', tt0047_SHIPTO_TITLE_LEN+1);
    memcpy(ptr_recvbuf->shipto_title_code, sz_recvbufcat + gbp->count, tt0047_SHIPTO_TITLE_LEN);
	ap_rprintf(r,"		<SHIPTO_TITLE_CODE>%s</SHIPTO_TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_title_code));
    gbp->count += tt0047_SHIPTO_TITLE_LEN;

	memset(ptr_recvbuf->shipto_company_name,'\0', tt0047_SHIPTO_COMP_LEN+1);
    memcpy(ptr_recvbuf->shipto_company_name, sz_recvbufcat + gbp->count, tt0047_SHIPTO_COMP_LEN);
	ap_rprintf(r,"		<SHIPTO_COMPANY_NAME>%s</SHIPTO_COMPANY_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_company_name));
    gbp->count += tt0047_SHIPTO_COMP_LEN;

	memset(ptr_recvbuf->shipto_jap_flag,'\0', tt0047_SHIPTO_JAP_LEN+1);
    memcpy(ptr_recvbuf->shipto_jap_flag, sz_recvbufcat + gbp->count, tt0047_SHIPTO_JAP_LEN);
	ap_rprintf(r,"		<SHIPTO_JAP_FLAG>%s</SHIPTO_JAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_jap_flag));
    gbp->count += tt0047_JAP_LEN;

	memset(ptr_recvbuf->shipto_addr_ref1,'\0', tt0047_SHIPTO_REF_LEN+1);
    memcpy(ptr_recvbuf->shipto_addr_ref1, sz_recvbufcat + gbp->count, tt0047_SHIPTO_REF_LEN);
	ap_rprintf(r,"		<SHIPTO_ADDR_REF1>%s</SHIPTO_ADDR_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_addr_ref1));
    gbp->count += tt0047_SHIPTO_REF_LEN;

	memset(ptr_recvbuf->shipto_addr_ref2,'\0', tt0047_SHIPTO_REF_LEN+1);
    memcpy(ptr_recvbuf->shipto_addr_ref2, sz_recvbufcat + gbp->count, tt0047_SHIPTO_REF_LEN);
	ap_rprintf(r,"		<SHIPTO_ADDR_REF2>%s</SHIPTO_ADDR_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_addr_ref2));
    gbp->count += tt0047_SHIPTO_REF_LEN;

	memset(ptr_recvbuf->shipto_street,'\0', tt0047_SHIPTO_STREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_street, sz_recvbufcat + gbp->count, tt0047_SHIPTO_STREET_LEN);
	ap_rprintf(r,"		<SHIPTO_STREET>%s</SHIPTO_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_street));
    gbp->count += tt0047_SHIPTO_STREET_LEN;

	memset(ptr_recvbuf->shipto_city,'\0', tt0047_SHIPTO_CITY_LEN+1);
    memcpy(ptr_recvbuf->shipto_city, sz_recvbufcat + gbp->count, tt0047_SHIPTO_CITY_LEN);
	ap_rprintf(r,"		<SHIPTO_CITY>%s</SHIPTO_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_city));
    gbp->count += tt0047_SHIPTO_CITY_LEN;

	memset(ptr_recvbuf->shipto_state,'\0', tt0047_SHIPTO_STATE_LEN+1);
    memcpy(ptr_recvbuf->shipto_state, sz_recvbufcat + gbp->count, tt0047_SHIPTO_STATE_LEN);
	ap_rprintf(r,"		<SHIPTO_STATE>%s</SHIPTO_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_state));
    gbp->count += tt0047_SHIPTO_STATE_LEN;

	memset(ptr_recvbuf->shipto_zipcode,'\0', tt0047_SHIPTO_ZIP_LEN+1);
    memcpy(ptr_recvbuf->shipto_zipcode, sz_recvbufcat + gbp->count, tt0047_SHIPTO_ZIP_LEN);
	ap_rprintf(r,"		<SHIPTO_ZIPCODE>%s</SHIPTO_ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_zipcode));
    gbp->count += tt0047_SHIPTO_ZIP_LEN;

	memset(ptr_recvbuf->shipto_country,'\0', tt0047_SHIPTO_COUNTRY_LEN+1);
    memcpy(ptr_recvbuf->shipto_country, sz_recvbufcat + gbp->count, tt0047_SHIPTO_COUNTRY_LEN);
	ap_rprintf(r,"		<SHIPTO_COUNTRY>%s</SHIPTO_COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_country));
    gbp->count += tt0047_SHIPTO_COUNTRY_LEN;

	memset(ptr_recvbuf->shipto_phone,'\0', tt0047_SHIPTO_PHONE_LEN+1);
    memcpy(ptr_recvbuf->shipto_phone, sz_recvbufcat + gbp->count, tt0047_SHIPTO_PHONE_LEN);
	ap_rprintf(r,"		<SHIPTO_PHONE>%s</SHIPTO_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_phone));
    gbp->count += tt0047_SHIPTO_PHONE_LEN;

	memset(ptr_recvbuf->ship_method_desc,'\0', tt0047_SHIP_METHOD_DESC_LEN+1);
    memcpy(ptr_recvbuf->ship_method_desc, sz_recvbufcat + gbp->count, tt0047_SHIP_METHOD_DESC_LEN);
	ap_rprintf(r,"		<SHIP_METHOD_DESC>%s</SHIP_METHOD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method_desc));
    gbp->count += tt0047_SHIP_METHOD_DESC_LEN;


	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0048>\n", gbp->tt_betag);

    return(0);
}

