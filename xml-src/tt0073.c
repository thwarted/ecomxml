/* tt0073.c Print Cift Certificate  */
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


int		tt0073_CatSendStr(struct global_struct *, char *, tt0073_st_send *);
int		tt0073_ParceRecvStr(tt0073_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0073_lt_process(request_rec *, struct global_struct *, char *);


int tt0073_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0073_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0073_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0073_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0073 = (tt0073_st_send *)malloc(sizeof(tt0073_st_send));
	gbp->recvbuf0073 = (tt0073_st_recv *)malloc(sizeof(tt0073_st_recv));

	gbp->sendbufcat = malloc(tt0073_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0073_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0073_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0073_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0073,  '\0',  sizeof(tt0073_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0073->request_id, "XML");
    strcpy(gbp->sendbuf0073->record_id, "0073");

	strcpy(gbp->sendbuf0073->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0073->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0073->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0073->userid,gbp,stdout_buffer);
	get_tag_data("ORD_NUM", gbp->sendbuf0073->ord_num,gbp,stdout_buffer);
	get_tag_data("ORD_SUFFIX", gbp->sendbuf0073->ord_suffix,gbp,stdout_buffer);
	get_tag_data("PROD_NUM", gbp->sendbuf0073->item_num,gbp,stdout_buffer);
	get_tag_data("PAGE_NUM", gbp->sendbuf0073->page_num,gbp,stdout_buffer);
	
	
	if(tt0073_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0073) == tt0073_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0073);
			free (gbp->recvbuf0073);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0073_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0073);
			free (gbp->recvbuf0073);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0073_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0073);
			free (gbp->recvbuf0073);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0073);
			free (gbp->recvbuf0073);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);
		
		tt0073_ParceRecvStr(gbp->recvbuf0073, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0073);
		free (gbp->recvbuf0073);
	}
	else
	{
		XML_Error("tt0073_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0073);
		free (gbp->recvbuf0073);
		return(-1);
	}

    return(0);
}


int tt0073_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0073_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-8.8s%-4.4s%-20.20s%04d",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
			ptr_sendbuf->ord_num,
			ptr_sendbuf->ord_suffix,
			ptr_sendbuf->item_num,
			atoi(ptr_sendbuf->page_num));
			
            
    return(gbp->j);
}


int tt0073_ParceRecvStr(tt0073_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"http://localhost/xml-dtd/tt0074.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0074 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0074.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0074 %s\"tt0074\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0073_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0073_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0073_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0073_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0073_REC_ID_LEN );
    gbp->count += tt0073_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0073_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0073_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0073_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0073_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0073_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0073_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0073_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0073_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0073_ERR_LEN;

	gbp->count += tt0073_SEND_FILLER_LEN;


	memset(ptr_recvbuf->ord_num,'\0', tt0073_ORD_NO_LEN+1);
    memcpy(ptr_recvbuf->ord_num, sz_recvbufcat + gbp->count, tt0073_ORD_NO_LEN);
	ap_rprintf(r,"		<ORD_NUM>%s</ORD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->ord_num));
    gbp->count += tt0073_ORD_NO_LEN;

	memset(ptr_recvbuf->ord_suffix,'\0', tt0073_ORD_SUF_LEN+1);
    memcpy(ptr_recvbuf->ord_suffix, sz_recvbufcat + gbp->count, tt0073_ORD_SUF_LEN);
	ap_rprintf(r,"		<ORD_SUFFIX>%s</ORD_SUFFIX>\n", handle_special_chars(gbp,ptr_recvbuf->ord_suffix));
    gbp->count += tt0073_ORD_SUF_LEN;

	memset(ptr_recvbuf->fname,'\0', tt0073_FN_LEN+1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0073_FN_LEN);
	ap_rprintf(r,"		<FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += tt0073_FN_LEN;

	memset(ptr_recvbuf->lname,'\0', tt0073_LN_LEN+1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0073_LN_LEN);
	ap_rprintf(r,"		<LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += tt0073_LN_LEN;

	memset(ptr_recvbuf->shipto_fname,'\0', tt0073_SHIPTO_FN_LEN+1);
    memcpy(ptr_recvbuf->shipto_fname, sz_recvbufcat + gbp->count, tt0073_SHIPTO_FN_LEN);
	ap_rprintf(r,"		<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_fname));
    gbp->count += tt0073_SHIPTO_FN_LEN;

	memset(ptr_recvbuf->shipto_lname,'\0', tt0073_SHIPTO_LN_LEN+1);
    memcpy(ptr_recvbuf->shipto_lname, sz_recvbufcat + gbp->count, tt0073_SHIPTO_LN_LEN);
	ap_rprintf(r,"		<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_lname));
    gbp->count += tt0073_SHIPTO_LN_LEN;

	memset(ptr_recvbuf->item_num,'\0', tt0073_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0073_ITM_NO_LEN);
	ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_num));
    gbp->count += tt0073_ITM_NO_LEN;

	memset(ptr_recvbuf->gc_num,'\0', tt0073_GC_NO_LEN+1);
    memcpy(ptr_recvbuf->gc_num, sz_recvbufcat + gbp->count, tt0073_GC_NO_LEN);
	ap_rprintf(r,"		<GC_NUM>%s</GC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->gc_num));
    gbp->count += tt0073_GC_NO_LEN;

	memset(ptr_recvbuf->gc_amount,'\0', tt0073_GC_AMT_LEN+1);
    memcpy(ptr_recvbuf->gc_amount, sz_recvbufcat + gbp->count, tt0073_GC_AMT_LEN);
	ap_rprintf(r,"		<GC_AMT>%s</GC_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->gc_amount));
    gbp->count += tt0073_GC_AMT_LEN;

	memset(ptr_recvbuf->gc_exp_date,'\0', tt0073_GC_EXP_LEN+1);
    memcpy(ptr_recvbuf->gc_exp_date, sz_recvbufcat + gbp->count, tt0073_GC_EXP_LEN);
	ap_rprintf(r,"		<GC_EXP_DATE>%s</GC_EXP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->gc_exp_date));
    gbp->count += tt0073_GC_EXP_LEN;

	memset(ptr_recvbuf->current_page,'\0', tt0073_PGN_LEN+1);
    memcpy(ptr_recvbuf->current_page, sz_recvbufcat + gbp->count, tt0073_PGN_LEN);
	ap_rprintf(r,"		<CURRENT_PAGE>%s</CURRENT_PAGE>\n", handle_special_chars(gbp,ptr_recvbuf->current_page));
    gbp->count += tt0073_PGN_LEN;

	memset(ptr_recvbuf->total_page,'\0', tt0073_TOT_PGN_LEN+1);
    memcpy(ptr_recvbuf->total_page, sz_recvbufcat + gbp->count, tt0073_TOT_PGN_LEN);
	ap_rprintf(r,"		<TOTAL_PAGE>%s</TOTAL_PAGE>\n", handle_special_chars(gbp,ptr_recvbuf->total_page));
    gbp->count += tt0073_TOT_PGN_LEN;


	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0074>\n", gbp->tt_betag);

    return(0);
}

