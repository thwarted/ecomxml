/* tt0045.c Query ShipTo */
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


int		tt0045_CatSendStr(struct global_struct *, char *, tt0045_st_send *);
int		tt0045_ParceRecvStr(tt0045_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0045_lt_process(request_rec *, struct global_struct *, char *);


int tt0045_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0045_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0045_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0045_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0045 = (tt0045_st_send *)malloc(sizeof(tt0045_st_send));
	gbp->recvbuf0045 = (tt0045_st_recv *)malloc(sizeof(tt0045_st_recv));

	gbp->sendbufcat = malloc(tt0045_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0045_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0045_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0045_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0045,  '\0',  sizeof(tt0045_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0045->request_id, "XML");
    strcpy(gbp->sendbuf0045->record_id, "0045");

	strcpy(gbp->sendbuf0045->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0045->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0045->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0045->userid,gbp,stdout_buffer);
	get_tag_data("SHIPTO_NUM", gbp->sendbuf0045->shipto_num,gbp,stdout_buffer);
	

	if(tt0045_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0045) == tt0045_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0045);
			free (gbp->recvbuf0045);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0045_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0045);
			free (gbp->recvbuf0045);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0045_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0045);
			free (gbp->recvbuf0045);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0045);
			free (gbp->recvbuf0045);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0045_ParceRecvStr(gbp->recvbuf0045, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0045);
		free (gbp->recvbuf0045);
	}
	else
	{
		XML_Error("tt0045_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0045);
		free (gbp->recvbuf0045);
		return(-1);
	}

    return(0);
}


int tt0045_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0045_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-2.2s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
			ptr_sendbuf->shipto_num);
            
    return(gbp->j);
}


int tt0045_ParceRecvStr(tt0045_st_recv *ptr_recvbuf, char *sz_recvbufcat,request_rec *r,
					  struct global_struct *gbp)
{
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0046.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0046 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0046.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0046 %s\"tt0046\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0045_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0045_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0045_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0045_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0045_REC_ID_LEN );
    gbp->count += tt0045_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0045_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0045_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0045_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0045_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0045_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0045_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0045_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0045_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0045_ERR_LEN;

	gbp->count += tt0045_SEND_FILLER_LEN;


	memset(ptr_recvbuf->shipto_num,'\0', tt0045_SHIPTO_NO_LEN+1);
    memcpy(ptr_recvbuf->shipto_num, sz_recvbufcat + gbp->count, tt0045_SHIPTO_NO_LEN);
	ap_rprintf(r,"		<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_num));
    gbp->count += tt0045_SHIPTO_NO_LEN;

	memset(ptr_recvbuf->shipto_fname,'\0', tt0045_SHIPTO_FN_LEN+1);
    memcpy(ptr_recvbuf->shipto_fname, sz_recvbufcat + gbp->count, tt0045_SHIPTO_FN_LEN);
	ap_rprintf(r,"		<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_fname));
    gbp->count += tt0045_SHIPTO_FN_LEN;

	memset(ptr_recvbuf->shipto_minit,'\0', tt0045_SHIPTO_MI_LEN+1);
    memcpy(ptr_recvbuf->shipto_minit, sz_recvbufcat + gbp->count, tt0045_SHIPTO_MI_LEN);
	ap_rprintf(r,"		<SHIPTO_MINITIAL>%s</SHIPTO_MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_minit));
    gbp->count += tt0045_SHIPTO_MI_LEN;

	memset(ptr_recvbuf->shipto_lname,'\0', tt0045_SHIPTO_LN_LEN+1);
    memcpy(ptr_recvbuf->shipto_lname, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LN_LEN);
	ap_rprintf(r,"		<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_lname));
    gbp->count += tt0045_SHIPTO_LN_LEN;

	memset(ptr_recvbuf->shipto_title_code,'\0', tt0045_SHIPTO_TITLE_LEN+1);
    memcpy(ptr_recvbuf->shipto_title_code, sz_recvbufcat + gbp->count, tt0045_SHIPTO_TITLE_LEN);
	ap_rprintf(r,"		<SHIPTO_TITLE_CODE>%s</SHIPTO_TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_title_code));
    gbp->count += tt0045_SHIPTO_TITLE_LEN;

	memset(ptr_recvbuf->shipto_company_name,'\0', tt0045_SHIPTO_COMP_LEN+1);
    memcpy(ptr_recvbuf->shipto_company_name, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMP_LEN);
	ap_rprintf(r,"		<SHIPTO_COMPANY_NAME>%s</SHIPTO_COMPANY_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_company_name));
    gbp->count += tt0045_SHIPTO_COMP_LEN;

	memset(ptr_recvbuf->shipto_jap_flag,'\0', tt0045_SHIPTO_JAP_LEN+1);
    memcpy(ptr_recvbuf->shipto_jap_flag, sz_recvbufcat + gbp->count, tt0045_SHIPTO_JAP_LEN);
	ap_rprintf(r,"		<SHIPTO_JAP_FLAG>%s</SHIPTO_JAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_jap_flag));
    gbp->count += tt0045_SHIPTO_JAP_LEN;

	memset(ptr_recvbuf->shipto_addr_ref1,'\0', tt0045_SHIPTO_REF_LEN+1);
    memcpy(ptr_recvbuf->shipto_addr_ref1, sz_recvbufcat + gbp->count, tt0045_SHIPTO_REF_LEN);
	ap_rprintf(r,"		<SHIPTO_ADDR_REF1>%s</SHIPTO_ADDR_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_addr_ref1));
    gbp->count += tt0045_SHIPTO_REF_LEN;

	memset(ptr_recvbuf->shipto_addr_ref2,'\0', tt0045_SHIPTO_REF_LEN+1);
    memcpy(ptr_recvbuf->shipto_addr_ref2, sz_recvbufcat + gbp->count, tt0045_SHIPTO_REF_LEN);
	ap_rprintf(r,"		<SHIPTO_ADDR_REF2>%s</SHIPTO_ADDR_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_addr_ref2));
    gbp->count += tt0045_SHIPTO_REF_LEN;

	memset(ptr_recvbuf->shipto_street,'\0', tt0045_SHIPTO_STREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_street, sz_recvbufcat + gbp->count, tt0045_SHIPTO_STREET_LEN);
	ap_rprintf(r,"		<SHIPTO_STREET>%s</SHIPTO_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_street));
    gbp->count += tt0045_SHIPTO_STREET_LEN;

	memset(ptr_recvbuf->shipto_city,'\0', tt0045_SHIPTO_CITY_LEN+1);
    memcpy(ptr_recvbuf->shipto_city, sz_recvbufcat + gbp->count, tt0045_SHIPTO_CITY_LEN);
	ap_rprintf(r,"		<SHIPTO_CITY>%s</SHIPTO_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_city));
    gbp->count += tt0045_SHIPTO_CITY_LEN;

	memset(ptr_recvbuf->shipto_state,'\0', tt0045_SHIPTO_STATE_LEN+1);
    memcpy(ptr_recvbuf->shipto_state, sz_recvbufcat + gbp->count, tt0045_SHIPTO_STATE_LEN);
	ap_rprintf(r,"		<SHIPTO_STATE>%s</SHIPTO_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_state));
    gbp->count += tt0045_SHIPTO_STATE_LEN;

	memset(ptr_recvbuf->shipto_zipcode,'\0', tt0045_SHIPTO_ZIP_LEN+1);
    memcpy(ptr_recvbuf->shipto_zipcode, sz_recvbufcat + gbp->count, tt0045_SHIPTO_ZIP_LEN);
	ap_rprintf(r,"		<SHIPTO_ZIPCODE>%s</SHIPTO_ZIPCODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_zipcode));
    gbp->count += tt0045_SHIPTO_ZIP_LEN;

	memset(ptr_recvbuf->shipto_country,'\0', tt0045_SHIPTO_COUNTRY_LEN+1);
    memcpy(ptr_recvbuf->shipto_country, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COUNTRY_LEN);
	ap_rprintf(r,"		<SHIPTO_COUNTRY>%s</SHIPTO_COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_country));
    gbp->count += tt0045_SHIPTO_COUNTRY_LEN;

	memset(ptr_recvbuf->shipto_phone,'\0', tt0045_SHIPTO_PHONE_LEN+1);
    memcpy(ptr_recvbuf->shipto_phone, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PHONE_LEN);
	ap_rprintf(r,"		<SHIPTO_PHONE>%s</SHIPTO_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_phone));
    gbp->count += tt0045_SHIPTO_PHONE_LEN;

	memset(ptr_recvbuf->shipto_fo_date,'\0', tt0045_SHIPTO_FO_LEN+1);
    memcpy(ptr_recvbuf->shipto_fo_date, sz_recvbufcat + gbp->count, tt0045_SHIPTO_FO_LEN);
	ap_rprintf(r,"		<SHIPTO_FO_DATE>%s</SHIPTO_FO_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_fo_date));
    gbp->count += tt0045_SHIPTO_FO_LEN;

	memset(ptr_recvbuf->shipto_ship_method,'\0', tt0045_SHIPTO_SHIP_METH_LEN+1);
    memcpy(ptr_recvbuf->shipto_ship_method, sz_recvbufcat + gbp->count, tt0045_SHIPTO_SHIP_METH_LEN);
	ap_rprintf(r,"		<SHIPTO_SHIP_METHOD>%s</SHIPTO_SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_ship_method));
    gbp->count += tt0045_SHIPTO_SHIP_METH_LEN;

	memset(ptr_recvbuf->shipto_desc,'\0', tt0045_SHIPTO_DESC_LEN+1);
    memcpy(ptr_recvbuf->shipto_desc, sz_recvbufcat + gbp->count, tt0045_SHIPTO_DESC_LEN);
	ap_rprintf(r,"		<SHIPTO_SHIP_METHOD_DESC>%s</SHIPTO_SHIP_METHOD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_desc));
    gbp->count += tt0045_SHIPTO_DESC_LEN;


	memset(ptr_recvbuf->shipto_comment1,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment1, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT1>%s</SHIPTO_COMMENT1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment1));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_comment2,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment2, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT2>%s</SHIPTO_COMMENT2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment2));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_pkslp1,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp1, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP1>%s</SHIPTO_PKSLP1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp1));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label1,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label1, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL1>%s</SHIPTO_LABEL1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label1));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet1,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet1, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET1>%s</SHIPTO_GREET1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet1));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_pkslp2,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp2, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP2>%s</SHIPTO_PKSLP2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp2));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label2,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label2, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL2>%s</SHIPTO_LABEL2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label2));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet2,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet2, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET2>%s</SHIPTO_GREET2>\n", ptr_recvbuf->shipto_greet2);
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_comment3,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment3, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT3>%s</SHIPTO_COMMENT3>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment3));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_comment4,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment4, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT4>%s</SHIPTO_COMMENT4>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment4));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_pkslp3,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp3, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP3>%s</SHIPTO_PKSLP3>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp3));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label3,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label3, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL3>%s</SHIPTO_LABEL3>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label3));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet3,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet3, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET3>%s</SHIPTO_GREET3>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet3));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_pkslp4,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp4, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP4>%s</SHIPTO_PKSLP4>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp4));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label4,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label4, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL4>%s</SHIPTO_LABEL4>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label4));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet4,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet4, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET4>%s</SHIPTO_GREET4>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet4));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_comment5,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment5, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT5>%s</SHIPTO_COMMENT5>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment5));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_comment6,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment6, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT6>%s</SHIPTO_COMMENT6>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment6));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_pkslp5,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp5, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP5>%s</SHIPTO_PKSLP5>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp5));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label5,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label5, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL5>%s</SHIPTO_LABEL5>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label5));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet5,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet5, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET5>%s</SHIPTO_GREET5>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet5));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_pkslp6,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp6, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP6>%s</SHIPTO_PKSLP6>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp6));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label6,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label6, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL6>%s</SHIPTO_LABEL6>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label6));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet6,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet6, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET6>%s</SHIPTO_GREET6>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet6));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_comment7,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment7, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT7>%s</SHIPTO_COMMENT7>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment7));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_comment8,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment8, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT8>%s</SHIPTO_COMMENT8>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment8));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_pkslp7,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp7, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP7>%s</SHIPTO_PKSLP7>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp7));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label7,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label7, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL7>%s</SHIPTO_LABEL7>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label7));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet7,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet7, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET7>%s</SHIPTO_GREET7>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet7));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_pkslp8,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp8, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP8>%s</SHIPTO_PKSLP8>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp8));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label8,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label8, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL8>%s</SHIPTO_LABEL8>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label8));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet8,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet8, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET8>%s</SHIPTO_GREET8>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet8));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_comment9,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment9, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT9>%s</SHIPTO_COMMENT9>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment9));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_comment10,'\0', tt0045_SHIPTO_COMM_LEN+1);
    memcpy(ptr_recvbuf->shipto_comment10, sz_recvbufcat + gbp->count, tt0045_SHIPTO_COMM_LEN);
	ap_rprintf(r,"		<SHIPTO_COMMENT10>%s</SHIPTO_COMMENT10>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_comment10));
    gbp->count += tt0045_SHIPTO_COMM_LEN;

	memset(ptr_recvbuf->shipto_pkslp9,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp9, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP9>%s</SHIPTO_PKSLP9>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp9));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label9,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label9, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL9>%s</SHIPTO_LABEL9>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label9));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet9,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet9, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET9>%s</SHIPTO_GREET9>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet9));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_pkslp10,'\0', tt0045_SHIPTO_PKSLP_LEN+1);
    memcpy(ptr_recvbuf->shipto_pkslp10, sz_recvbufcat + gbp->count, tt0045_SHIPTO_PKSLP_LEN);
	ap_rprintf(r,"		<SHIPTO_PKSLP10>%s</SHIPTO_PKSLP10>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_pkslp10));
    gbp->count += tt0045_SHIPTO_PKSLP_LEN;

	memset(ptr_recvbuf->shipto_label10,'\0', tt0045_SHIPTO_LABEL_LEN+1);
    memcpy(ptr_recvbuf->shipto_label10, sz_recvbufcat + gbp->count, tt0045_SHIPTO_LABEL_LEN);
	ap_rprintf(r,"		<SHIPTO_LABEL10>%s</SHIPTO_LABEL10>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_label10));
    gbp->count += tt0045_SHIPTO_LABEL_LEN;

	memset(ptr_recvbuf->shipto_greet10,'\0', tt0045_SHIPTO_GREET_LEN+1);
    memcpy(ptr_recvbuf->shipto_greet10, sz_recvbufcat + gbp->count, tt0045_SHIPTO_GREET_LEN);
	ap_rprintf(r,"		<SHIPTO_GREET10>%s</SHIPTO_GREET10>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_greet10));
    gbp->count += tt0045_SHIPTO_GREET_LEN;

	memset(ptr_recvbuf->shipto_edp,'\0', tt0045_SHIPTO_EDP_LEN+1);
    memcpy(ptr_recvbuf->shipto_edp, sz_recvbufcat + gbp->count, tt0045_SHIPTO_EDP_LEN);
	ap_rprintf(r,"		<SHIPTO_EDP>%s</SHIPTO_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_edp));
    gbp->count += tt0045_SHIPTO_EDP_LEN;

	memset(ptr_recvbuf->ship_to_email,'\0', tt0045_SHIP_TO_EMAIL_LEN+1);
    memcpy(ptr_recvbuf->ship_to_email, sz_recvbufcat + gbp->count, tt0045_SHIP_TO_EMAIL_LEN);
	ap_rprintf(r,"		<SHIP_TO_EMAIL>%s</SHIP_TO_EMAIL>\n", handle_special_chars(gbp,ptr_recvbuf->ship_to_email));
    gbp->count += tt0045_SHIP_TO_EMAIL_LEN;


	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0046>\n", gbp->tt_betag);

    return(0);
}

