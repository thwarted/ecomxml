/* tt0039.c Update ShipTo */
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


int		tt0039_CatSendStr(struct global_struct *, char *, tt0039_st_send *);
int		tt0039_ParceRecvStr(tt0039_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0039_lt_process(request_rec *, struct global_struct *, char *);


int tt0039_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0039_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0039_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0039_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0039 = (tt0039_st_send *)malloc(sizeof(tt0039_st_send));
	gbp->recvbuf0039 = (tt0039_st_recv *)malloc(sizeof(tt0039_st_recv));

	gbp->sendbufcat = malloc(tt0039_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0039_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0039_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0039_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0039,  '\0',  sizeof(tt0039_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0039->request_id, "XML");
    strcpy(gbp->sendbuf0039->record_id, "0039");

	strcpy(gbp->sendbuf0039->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0039->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0039->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0039->userid,gbp,stdout_buffer);
	get_tag_data("SHIPTO_NUM", gbp->sendbuf0039->shipto_num,gbp,stdout_buffer);
	get_tag_data("SHIPTO_FNAME", gbp->sendbuf0039->shipto_fname,gbp,stdout_buffer);
	get_tag_data("SHIPTO_MINITIAL", gbp->sendbuf0039->shipto_minit,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LNAME", gbp->sendbuf0039->shipto_lname,gbp,stdout_buffer);
	get_tag_data("SHIPTO_TITLE_CODE", gbp->sendbuf0039->shipto_title,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMPANY_NAME", gbp->sendbuf0039->shipto_company,gbp,stdout_buffer);
	get_tag_data("SHIPTO_JAP_FLAG", gbp->sendbuf0039->shipto_jap_flag,gbp,stdout_buffer);
	get_tag_data("SHIPTO_ADDR_REF1", gbp->sendbuf0039->shipto_ref1,gbp,stdout_buffer);
	get_tag_data("SHIPTO_ADDR_REF2", gbp->sendbuf0039->shipto_ref2,gbp,stdout_buffer);
	get_tag_data("SHIPTO_STREET", gbp->sendbuf0039->shipto_street,gbp,stdout_buffer);
	get_tag_data("SHIPTO_CITY", gbp->sendbuf0039->shipto_city,gbp,stdout_buffer);
	get_tag_data("SHIPTO_STATE", gbp->sendbuf0039->shipto_state,gbp,stdout_buffer);
	get_tag_data("SHIPTO_ZIPCODE", gbp->sendbuf0039->shipto_zipcode,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COUNTRY", gbp->sendbuf0039->shipto_country,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PHONE", gbp->sendbuf0039->shipto_phone,gbp,stdout_buffer);
	get_tag_data("SHIPTO_FUT_ORD_DATE", gbp->sendbuf0039->shipto_fo_date,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT1", gbp->sendbuf0039->shipto_comment1,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT2", gbp->sendbuf0039->shipto_comment2,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP1", gbp->sendbuf0039->shipto_pkslp1,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL1", gbp->sendbuf0039->shipto_label1,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET1", gbp->sendbuf0039->shipto_greet1,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP2", gbp->sendbuf0039->shipto_pkslp2,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL2", gbp->sendbuf0039->shipto_label2,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET2", gbp->sendbuf0039->shipto_greet2,gbp,stdout_buffer);
	get_tag_data("SHIPTO_SHIP_METHOD", gbp->sendbuf0039->shipto_ship_method,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT3", gbp->sendbuf0039->shipto_comment3,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT4", gbp->sendbuf0039->shipto_comment4,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP3", gbp->sendbuf0039->shipto_pkslp3,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL3", gbp->sendbuf0039->shipto_label3,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET3", gbp->sendbuf0039->shipto_greet3,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP4", gbp->sendbuf0039->shipto_pkslp4,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL4", gbp->sendbuf0039->shipto_label4,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET4", gbp->sendbuf0039->shipto_greet4,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT5", gbp->sendbuf0039->shipto_comment5,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT6", gbp->sendbuf0039->shipto_comment6,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP5", gbp->sendbuf0039->shipto_pkslp5,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL5", gbp->sendbuf0039->shipto_label5,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET5", gbp->sendbuf0039->shipto_greet5,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP6", gbp->sendbuf0039->shipto_pkslp6,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL6", gbp->sendbuf0039->shipto_label6,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET6", gbp->sendbuf0039->shipto_greet6,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT7", gbp->sendbuf0039->shipto_comment7,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT8", gbp->sendbuf0039->shipto_comment8,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP7", gbp->sendbuf0039->shipto_pkslp7,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL7", gbp->sendbuf0039->shipto_label7,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET7", gbp->sendbuf0039->shipto_greet7,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP8", gbp->sendbuf0039->shipto_pkslp8,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL8", gbp->sendbuf0039->shipto_label8,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET8", gbp->sendbuf0039->shipto_greet8,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT9", gbp->sendbuf0039->shipto_comment9,gbp,stdout_buffer);
	get_tag_data("SHIPTO_COMMENT10", gbp->sendbuf0039->shipto_comment10,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP9", gbp->sendbuf0039->shipto_pkslp9,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL9", gbp->sendbuf0039->shipto_label9,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET9", gbp->sendbuf0039->shipto_greet9,gbp,stdout_buffer);
	get_tag_data("SHIPTO_PKSLP10", gbp->sendbuf0039->shipto_pkslp10,gbp,stdout_buffer);
	get_tag_data("SHIPTO_LABEL10", gbp->sendbuf0039->shipto_label10,gbp,stdout_buffer);
	get_tag_data("SHIPTO_GREET10", gbp->sendbuf0039->shipto_greet10,gbp,stdout_buffer);
	get_tag_data("SHIPTO_EDP", gbp->sendbuf0039->shipto_edp,gbp,stdout_buffer);
	get_tag_data("SHIP_TO_EMAIL", gbp->sendbuf0039->ship_to_email,gbp,stdout_buffer);
  

	if(tt0039_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0039) == tt0039_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0039);
			free (gbp->recvbuf0039);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0039_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0039);
			free (gbp->recvbuf0039);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0039_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0039);
			free (gbp->recvbuf0039);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0039);
			free (gbp->recvbuf0039);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0039_ParceRecvStr(gbp->recvbuf0039, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0039);
		free (gbp->recvbuf0039);
	}
	else
	{
		XML_Error("tt0039_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0039);
		free (gbp->recvbuf0039);
		return(-1);
	}

    return(0);
}


int tt0039_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0039_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-2.2s%-16.16s%-1.1s%-16.16s%-2.2s"
			"%-30.30s%-1.1s%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-4.4s%-14.14s"
			"%-8.8s%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-2.2s"
			"%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s"
            "%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s"
            "%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s"
            "%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s"
            "%-9.9s%-50.50s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
			ptr_sendbuf->shipto_num,
			ptr_sendbuf->shipto_fname,
			ptr_sendbuf->shipto_minit,
			ptr_sendbuf->shipto_lname,
			ptr_sendbuf->shipto_title,
			ptr_sendbuf->shipto_company,
			ptr_sendbuf->shipto_jap_flag,
			ptr_sendbuf->shipto_ref1,
			ptr_sendbuf->shipto_ref2,
			ptr_sendbuf->shipto_street,
			ptr_sendbuf->shipto_city,
			ptr_sendbuf->shipto_state,
			ptr_sendbuf->shipto_zipcode,
			ptr_sendbuf->shipto_country,
			ptr_sendbuf->shipto_phone,
			ptr_sendbuf->shipto_fo_date,
			ptr_sendbuf->shipto_comment1,
			ptr_sendbuf->shipto_comment2,
			ptr_sendbuf->shipto_pkslp1,
			ptr_sendbuf->shipto_label1,
			ptr_sendbuf->shipto_greet1,
			ptr_sendbuf->shipto_pkslp2,
			ptr_sendbuf->shipto_label2,
			ptr_sendbuf->shipto_greet2,
			ptr_sendbuf->shipto_ship_method,
			ptr_sendbuf->shipto_comment3,
			ptr_sendbuf->shipto_comment4,
			ptr_sendbuf->shipto_pkslp3,
			ptr_sendbuf->shipto_label3,
			ptr_sendbuf->shipto_greet3,
			ptr_sendbuf->shipto_pkslp4,
			ptr_sendbuf->shipto_label4,
			ptr_sendbuf->shipto_greet4,
			ptr_sendbuf->shipto_comment5,
			ptr_sendbuf->shipto_comment6,
			ptr_sendbuf->shipto_pkslp5,
			ptr_sendbuf->shipto_label5,
			ptr_sendbuf->shipto_greet5,
			ptr_sendbuf->shipto_pkslp6,
			ptr_sendbuf->shipto_label6,
			ptr_sendbuf->shipto_greet6,
			ptr_sendbuf->shipto_comment7,
			ptr_sendbuf->shipto_comment8,
			ptr_sendbuf->shipto_pkslp7,
			ptr_sendbuf->shipto_label7,
			ptr_sendbuf->shipto_greet7,
			ptr_sendbuf->shipto_pkslp8,
			ptr_sendbuf->shipto_label8,
			ptr_sendbuf->shipto_greet8,
			ptr_sendbuf->shipto_comment9,
			ptr_sendbuf->shipto_comment10,
			ptr_sendbuf->shipto_pkslp9,
			ptr_sendbuf->shipto_label9,
			ptr_sendbuf->shipto_greet9,
			ptr_sendbuf->shipto_pkslp10,
			ptr_sendbuf->shipto_label10,
			ptr_sendbuf->shipto_greet10,
			ptr_sendbuf->shipto_edp,
			ptr_sendbuf->ship_to_email);
           
    return(gbp->j);
}


int tt0039_ParceRecvStr(tt0039_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0040.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0040 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0040.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0040 %s\"tt0040\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0039_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0039_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0039_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0039_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0039_REC_ID_LEN );
    gbp->count += tt0039_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0039_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0039_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0039_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0039_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0039_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0039_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0039_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0039_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0039_ERR_LEN;

	gbp->count += tt0039_SEND_FILLER_LEN;

    	
    for(gbp->i = 0; gbp->i < tt0039_SHIPTO_COUNT; gbp->i++) 
	{

    
		memset(ptr_recvbuf->shipto[gbp->i].shipto_num,'\0', tt0039_SHIPTO_NO_LEN+1);
		memcpy(ptr_recvbuf->shipto[gbp->i].shipto_num, sz_recvbufcat + gbp->count, tt0039_SHIPTO_NO_LEN);
		gbp->count += tt0039_SHIPTO_NO_LEN;

		memset(ptr_recvbuf->shipto[gbp->i].shipto_fname,'\0', tt0039_SHIPTO_FN_LEN+1);
		memcpy(ptr_recvbuf->shipto[gbp->i].shipto_fname, sz_recvbufcat + gbp->count, tt0039_SHIPTO_FN_LEN);
		gbp->count += tt0039_SHIPTO_FN_LEN;
		sprintf(ptr_recvbuf->shipto[gbp->i].shipto_fname,"%s",
					rtrim(ptr_recvbuf->shipto[gbp->i].shipto_fname, tt0039_SHIPTO_FN_LEN+1,gbp));


		memset(ptr_recvbuf->shipto[gbp->i].shipto_lname,'\0', tt0039_SHIPTO_LN_LEN+1);
		memcpy(ptr_recvbuf->shipto[gbp->i].shipto_lname, sz_recvbufcat + gbp->count, tt0039_SHIPTO_LN_LEN);
		gbp->count += tt0039_SHIPTO_LN_LEN;

		if((strlen(ptr_recvbuf->shipto[gbp->i].shipto_fname)) > 0) 
		{
			ap_rprintf(r,"	 <SHIPTO_DETAIL>\n");
			ap_rprintf(r,"		<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_num));
			ap_rprintf(r,"		<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_fname));
			ap_rprintf(r,"		<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_lname));
	     	ap_rprintf(r,"	 </SHIPTO_DETAIL>\n");
		}
	}

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0040>\n", gbp->tt_betag);

    return(0);
}

