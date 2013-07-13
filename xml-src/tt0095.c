/* tt0095.c Standing Order Add */

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


int		tt0095_CatSendStr(struct global_struct *, char *, tt0095_st_send *);
int		tt0095_ParceRecvStr(tt0095_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0095_lt_process(request_rec *, struct global_struct *, char *);


int tt0095_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)			/* Get the current working path */
		return(-1);

    GetInf(gbp);							/* Read info from INF file */

	return(0);
}

int tt0095_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0095_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0095_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0095 = (tt0095_st_send *)malloc(sizeof(tt0095_st_send));
	gbp->recvbuf0095 = (tt0095_st_recv *)malloc(sizeof(tt0095_st_recv));
	
	gbp->sendbufcat = malloc(tt0095_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0095_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0095_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0095_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0095,  '\0',  sizeof(tt0095_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0095->request_id, "XML");
    strcpy(gbp->sendbuf0095->record_id, "0095");

	strcpy(gbp->sendbuf0095->ip_address, r->connection->remote_ip);

	get_tag_data("COMPANY", gbp->sendbuf0095->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0095->division,gbp,stdout_buffer);
	get_tag_data("UID", gbp->sendbuf0095->userid,gbp,stdout_buffer);

    for (gbp->i = 0; gbp->i < tt0095_SHIPTO_COUNT; gbp->i++) 
	{
        sprintf(gbp->tagname, "SHIPTO_NO_%d", gbp->i+1);
		get_tag_data(gbp->tagname,  gbp->sendbuf0095->shipto_no[gbp->i],gbp,stdout_buffer);
		
		sprintf(gbp->tagname, "SHIPTO_EDP_%d", gbp->i+1);
		get_tag_data(gbp->tagname,  gbp->sendbuf0095->shipto_edp[gbp->i],gbp,stdout_buffer);
    }

    if((tt0095_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0095)) == tt0095_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0095);
			free (gbp->recvbuf0095);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0095_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0095);
			free (gbp->recvbuf0095);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0095_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0095);
			free (gbp->recvbuf0095);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0095);
			free (gbp->recvbuf0095);
			return(-1);
		}

//        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);

		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0095_ParceRecvStr(gbp->recvbuf0095, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0095);
		free (gbp->recvbuf0095);

	}
	else
	{
		XML_Error("tt0095_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0095);
		free (gbp->recvbuf0095);

		return(-1);
	}

    return 0;
}


int tt0095_CatSendStr(struct global_struct *gbp, char *sendbufcat, tt0095_st_send *sendbuf)
{
    gbp->j = sprintf(sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s"
			"%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s"
			"%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s"
			"%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s%-2.2s%-9.9s"
			,
            sendbuf->request_id,
            sendbuf->record_id,
            sendbuf->company,
            sendbuf->division,
            sendbuf->userid,
            sendbuf->ip_address,
			sendbuf->filler,
			sendbuf->shipto_no[0],
			sendbuf->shipto_edp[0],
			sendbuf->shipto_no[1],
			sendbuf->shipto_edp[1],
			sendbuf->shipto_no[2],
			sendbuf->shipto_edp[2],
			sendbuf->shipto_no[3],
			sendbuf->shipto_edp[3],
			sendbuf->shipto_no[4],
			sendbuf->shipto_edp[4],
			sendbuf->shipto_no[5],
			sendbuf->shipto_edp[5],
			sendbuf->shipto_no[6],
			sendbuf->shipto_edp[6],
			sendbuf->shipto_no[7],
			sendbuf->shipto_edp[7],
			sendbuf->shipto_no[8],
			sendbuf->shipto_edp[8],
			sendbuf->shipto_no[9],
			sendbuf->shipto_edp[9],
			sendbuf->shipto_no[10],
			sendbuf->shipto_edp[10],
			sendbuf->shipto_no[11],
			sendbuf->shipto_edp[11],
			sendbuf->shipto_no[12],
			sendbuf->shipto_edp[12],
			sendbuf->shipto_no[13],
			sendbuf->shipto_edp[13],
			sendbuf->shipto_no[14],
			sendbuf->shipto_edp[14],
			sendbuf->shipto_no[15],
			sendbuf->shipto_edp[15],
			sendbuf->shipto_no[16],
			sendbuf->shipto_edp[16],
			sendbuf->shipto_no[17],
			sendbuf->shipto_edp[17],
			sendbuf->shipto_no[18],
			sendbuf->shipto_edp[18],
			sendbuf->shipto_no[19],
			sendbuf->shipto_edp[19]
			);
			
    return(gbp->j);
}
    

int tt0095_ParceRecvStr(tt0095_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0096.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0096 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0096.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0096 %s\"tt0096\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

	memset(ptr_recvbuf->request_id,'\0', tt0095_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0095_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0095_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0095_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0095_REC_ID_LEN );
    gbp->count += tt0095_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0095_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0095_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0095_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0095_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0095_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0095_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0095_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0095_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0095_ERR_LEN;

	gbp->count += tt0095_SEND_FILLER_LEN;

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0096>\n", gbp->tt_betag);

    return(0);
}




