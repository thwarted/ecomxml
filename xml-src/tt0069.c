/* tt0069.c Password Request */
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


int		tt0069_CatSendStr(struct global_struct *, char *, tt0069_st_send *);
int		tt0069_ParceRecvStr(tt0069_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0069_lt_process(request_rec *, struct global_struct *, char *);


int tt0069_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
		return(-1);

    GetInf(gbp);

   return(0);

}


int tt0069_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0069_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0069_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0069 = (tt0069_st_send *)malloc(sizeof(tt0069_st_send));
	gbp->recvbuf0069 = (tt0069_st_recv *)malloc(sizeof(tt0069_st_recv));

	gbp->sendbufcat = malloc(tt0069_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0069_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0069_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0069_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0069,  '\0',  sizeof(tt0069_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0069->request_id, "XML");
    strcpy(gbp->sendbuf0069->record_id, "0069");

	strcpy(gbp->sendbuf0069->ip_address, r->connection->remote_ip);
    get_tag_data("COMPANY", gbp->sendbuf0069->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0069->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0069->userid,gbp,stdout_buffer);
	get_tag_data("EMAIL_ADDR", gbp->sendbuf0069->email_addr,gbp,stdout_buffer);
	

	if(tt0069_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0069) == tt0069_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0069);
			free (gbp->recvbuf0069);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0069_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0069);
			free (gbp->recvbuf0069);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0069_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0069);
			free (gbp->recvbuf0069);
			return(-1);
		}

		// Do an additional send and recieve for confirmation
		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0069);
			free (gbp->recvbuf0069);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0069_ParceRecvStr(gbp->recvbuf0069, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0069);
		free (gbp->recvbuf0069);
	}
	else
	{
		XML_Error("tt0069_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0069);
		free (gbp->recvbuf0069);
		return(-1);
	}

    return(0);
}


int tt0069_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0069_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-47.47s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
            ptr_sendbuf->email_addr);

    return(gbp->j);
}


int tt0069_ParceRecvStr(tt0069_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"http://localhost/xml-dtd/tt0070.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0070 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0070.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0070 %s\"tt0070\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0069_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0069_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0069_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0069_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0069_REC_ID_LEN );
    gbp->count += tt0069_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0069_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0069_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0069_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0069_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0069_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0069_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0069_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0069_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0069_ERR_LEN;

	gbp->count += tt0069_SEND_FILLER_LEN;


    memset(ptr_recvbuf->email_addr,'\0', tt0069_EMAIL_LEN+1);
    memcpy(ptr_recvbuf->email_addr, sz_recvbufcat + gbp->count, tt0069_EMAIL_LEN);
	ap_rprintf(r,"		<EMAIL_ADDR>%s</EMAIL_ADDR>\n", handle_special_chars(gbp,ptr_recvbuf->email_addr));
    gbp->count += tt0069_EMAIL_LEN;



	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0070>\n", gbp->tt_betag);

    return(0);
}

