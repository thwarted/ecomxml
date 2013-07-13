/* tt0123.c cART iNVENTORY dATA */

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


int		tt0123_CatSendStr(struct global_struct *,char *, tt0123_st_send *);
int		tt0123_ParceRecvStr(tt0123_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0123_lt_process(request_rec *, struct global_struct *, char *);


int tt0123_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0123_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0123_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0123_lt_process(request_rec *r, struct global_struct *gbp, char * stdout_buffer)
{
	gbp->sendbuf0123 = (tt0123_st_send *)malloc(sizeof(tt0123_st_send));
	gbp->recvbuf0123 = (tt0123_st_recv *)malloc(sizeof(tt0123_st_recv));


	gbp->sendbufcat = malloc(tt0123_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0123_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0123_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0123_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0123,  '\0',  sizeof(tt0123_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0123->request_id, "XML");
    strcpy(gbp->sendbuf0123->record_id, "0123");

	strcpy(gbp->sendbuf0123->ip_address, r->connection->remote_ip);


    get_tag_data("COMPANY", gbp->sendbuf0123->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0123->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0123->userid,gbp,stdout_buffer);
	

	if(tt0123_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0123) == tt0123_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0123);
			free (gbp->recvbuf0123);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0123_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0123);
			free (gbp->recvbuf0123);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0123_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0123);
			free (gbp->recvbuf0123);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0123);
			free (gbp->recvbuf0123);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0123_ParceRecvStr(gbp->recvbuf0123, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0123);
		free (gbp->recvbuf0123);
	}
	else
	{
		XML_Error("tt0123_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0123);
		free (gbp->recvbuf0123);
		return(-1);
	}

    return(0);
}


int tt0123_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0123_st_send *ptr_sendbuf)
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


int tt0123_ParceRecvStr(tt0123_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
    gbp->count = 0;
	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0124.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0124 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0124.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0124 %s\"tt0124\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0123_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0123_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0123_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0123_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0123_REC_ID_LEN );
    gbp->count += tt0123_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0123_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0123_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0123_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0123_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0123_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0123_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0123_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0123_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0123_ERR_LEN;

	gbp->count += tt0123_SEND_FILLER_LEN;

    memset(ptr_recvbuf->cust_edp,'\0', tt0123_CUST_EDP_LEN+1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0123_CUST_EDP_LEN);
	ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0123_CUST_EDP_LEN;

    memset(ptr_recvbuf->source,'\0', tt0123_SOURCE_LEN+1);
    memcpy(ptr_recvbuf->source, sz_recvbufcat + gbp->count, tt0123_SOURCE_LEN);
	ap_rprintf(r,"		<SOURCE>%s</SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->source));
    gbp->count += tt0123_SOURCE_LEN;

    memset(ptr_recvbuf->item_count,'\0', tt0123_ITEM_COUNT_LEN+1);
    memcpy(ptr_recvbuf->item_count, sz_recvbufcat + gbp->count, tt0123_ITEM_COUNT_LEN);
	ap_rprintf(r,"		<ITEM_COUNT>%s</ITEM_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->item_count));
    gbp->count += tt0123_ITEM_COUNT_LEN;

    	
    for(gbp->i = 0; gbp->i < atoi(ptr_recvbuf->item_count); gbp->i++) 
	{

    	ap_rprintf(r,"	<ITEM_DETAIL>\n");

	    memset(ptr_recvbuf->item[gbp->i].item_edp,'\0', tt0123_ITEM_EDP_LEN+1);
		memcpy(ptr_recvbuf->item[gbp->i].item_edp, sz_recvbufcat + gbp->count, tt0123_ITEM_EDP_LEN);
		ap_rprintf(r,"		<ITEM_EDP>%s</ITEM_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_edp));
		gbp->count += tt0123_ITEM_EDP_LEN;

	    memset(ptr_recvbuf->item[gbp->i].item_no,'\0', tt0123_ITEM_NO_LEN+1);
		memcpy(ptr_recvbuf->item[gbp->i].item_no, sz_recvbufcat + gbp->count, tt0123_ITEM_NO_LEN);
		ap_rprintf(r,"		<ITEM_NO>%s</ITEM_NO>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].item_no));
		gbp->count += tt0123_ITEM_NO_LEN;

	    memset(ptr_recvbuf->item[gbp->i].desc,'\0', tt0123_DESC_LEN+1);
		memcpy(ptr_recvbuf->item[gbp->i].desc, sz_recvbufcat + gbp->count, tt0123_DESC_LEN);
		ap_rprintf(r,"		<DESC>%s</DESC>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].desc));
		gbp->count += tt0123_DESC_LEN;

	    memset(ptr_recvbuf->item[gbp->i].custom_flag,'\0', tt0123_CUSTOM_FLAG_LEN+1);
		memcpy(ptr_recvbuf->item[gbp->i].custom_flag, sz_recvbufcat + gbp->count, tt0123_CUSTOM_FLAG_LEN);
		ap_rprintf(r,"		<CUSTOM_FLAG>%s</CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].custom_flag));
		gbp->count += tt0123_CUSTOM_FLAG_LEN;

	    memset(ptr_recvbuf->item[gbp->i].qty,'\0', tt0123_QTY_LEN+1);
		memcpy(ptr_recvbuf->item[gbp->i].qty, sz_recvbufcat + gbp->count, tt0123_QTY_LEN);
		ap_rprintf(r,"		<QTY>%s</QTY>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].qty));
		gbp->count += tt0123_QTY_LEN;

	    memset(ptr_recvbuf->item[gbp->i].avail,'\0', tt0123_AVAIL_LEN+1);
		memcpy(ptr_recvbuf->item[gbp->i].avail, sz_recvbufcat + gbp->count, tt0123_AVAIL_LEN);
		ap_rprintf(r,"		<AVAIL>%s</AVAIL>\n", handle_special_chars(gbp,ptr_recvbuf->item[gbp->i].avail));
		gbp->count += tt0123_AVAIL_LEN;

		ap_rprintf(r,"	</ITEM_DETAIL>\n");

	}

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0124>\n", gbp->tt_betag);

    return(0);
}

