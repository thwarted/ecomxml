/* tt0005.c  All Orders */
/*modified for phase 2 - JL */
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


int		tt0005_CatSendStr(struct global_struct *, char *, tt0005_st_send *);
int		tt0005_ParceRecvStr(tt0005_st_recv *, char *, request_rec *,
						  struct global_struct *);
int		tt0005_lt_process(request_rec *, struct global_struct *, char *);



int tt0005_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)			/* Get the current working path */
		return(-1);

    GetInf(gbp);							/* Read info from INF file */


	return(0);
}


int tt0005_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0005_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}


int tt0005_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	gbp->sendbuf0005 = (tt0005_st_send *)malloc(sizeof(tt0005_st_send));
	gbp->recvbuf0005 = (tt0005_st_recv *)malloc(sizeof(tt0005_st_recv));

	gbp->sendbufcat = malloc(tt0005_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0005_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0005_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0005_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0005,  '\0',  sizeof(tt0005_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0005->request_id, "XML");
    strcpy(gbp->sendbuf0005->record_id, "0005");

	strcpy(gbp->sendbuf0005->ip_address, r->connection->remote_ip);

   
	get_tag_data("COMPANY", gbp->sendbuf0005->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0005->division,gbp,stdout_buffer);
	get_tag_data("UID", gbp->sendbuf0005->userid,gbp,stdout_buffer);
	get_tag_data("CUST_EDP", gbp->sendbuf0005->cust_edp,gbp,stdout_buffer);
	get_tag_data("PAGE_NUM", gbp->sendbuf0005->page_num,gbp,stdout_buffer);
	
	if(atoi(gbp->sendbuf0005->page_num) < 1)
		strcpy(gbp->sendbuf0005->page_num, "0001");

	if((tt0005_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0005)) == tt0005_LAN_SEND_BUF_LEN)
	{
		if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0005);
			free (gbp->recvbuf0005);
			return(-1);
		}

	
		if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0005_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0005);
			free (gbp->recvbuf0005);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0005_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0005);
			free (gbp->recvbuf0005);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0005);
			free (gbp->recvbuf0005);
			return(-1);
		}

/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/

		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0005_ParceRecvStr(gbp->recvbuf0005, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0005);
		free (gbp->recvbuf0005);
	}
	else
	{
		XML_Error("tt0005_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0005);
		free (gbp->recvbuf0005);
		return(-1);
	}

    return(0);
}

/******************************************************************/
/*                                                                */
/*   Create the send string from our current data                 */
/*                                                                */
/******************************************************************/

int tt0005_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0005_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4s%-4s%-2s%-2s%-16.16s%-16.16s%-25.25s%09d%04d",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
            atoi(ptr_sendbuf->cust_edp),
			atoi(ptr_sendbuf->page_num));

    return(gbp->j);
}


/******************************************************************/
/*                                                                */
/*   Parse the answer                                             */
/*                                                                */
/******************************************************************/

int tt0005_ParceRecvStr(tt0005_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
					  struct global_struct *gbp)
{
    // Place the individual variables into target fields
	gbp->count = 0;
	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0006.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0006 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0006.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0006 %s\"tt0006\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0005_REQ_ID_LEN + 1);
	memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0005_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0005_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0005_REC_ID_LEN + 1);
	memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0005_REC_ID_LEN );
    gbp->count += tt0005_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0005_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0005_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0005_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', tt0005_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, tt0005_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += tt0005_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0005_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0005_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0005_ERR_LEN;

	gbp->count += tt0005_SEND_FILLER_LEN;


    memset(ptr_recvbuf->valid_ord, '\0', tt0005_ORD_LEN + 1);
	memcpy(ptr_recvbuf->valid_ord, sz_recvbufcat + gbp->count, tt0005_ORD_LEN);
	ap_rprintf(r,"		<TOTAL_VALID_ORD>%s</TOTAL_VALID_ORD>\n", handle_special_chars(gbp,ptr_recvbuf->valid_ord));
	gbp->count += tt0005_ORD_LEN;

    memset(ptr_recvbuf->ord_curr_page, '\0', tt0005_CURR_PAGE_LEN + 1);
	memcpy(ptr_recvbuf->ord_curr_page, sz_recvbufcat + gbp->count, tt0005_CURR_PAGE_LEN);
	ap_rprintf(r,"		<PAGE_IN_ORD_REVIEW>%s</PAGE_IN_ORD_REVIEW>\n", handle_special_chars(gbp,ptr_recvbuf->ord_curr_page));
	gbp->count += tt0005_CURR_PAGE_LEN;

    // Build data from array

	gbp->k = atoi(ptr_recvbuf->valid_ord);
	if(gbp->k > (int)tt0005_ORDER_COUNT)
		gbp->k = (int)tt0005_ORDER_COUNT;


    for(gbp->i = 0; gbp->i < gbp->k; gbp->i++) 
	{
        memset(ptr_recvbuf->order[gbp->i].ord_num, '\0', tt0005_ORD_NO_LEN + 1);
        memcpy(ptr_recvbuf->order[gbp->i].ord_num, sz_recvbufcat + gbp->count, tt0005_ORD_NO_LEN);
        gbp->count += tt0005_ORD_NO_LEN;

		memset(ptr_recvbuf->order[gbp->i].mail_date, '\0', tt0005_MAIL_DATE_LEN + 1);
        memcpy(ptr_recvbuf->order[gbp->i].mail_date, sz_recvbufcat + gbp->count, tt0005_MAIL_DATE_LEN);
        gbp->count += tt0005_MAIL_DATE_LEN;

        memset(ptr_recvbuf->order[gbp->i].po_num, '\0', tt0005_PO_NO_LEN + 1);
        memcpy(ptr_recvbuf->order[gbp->i].po_num, sz_recvbufcat + gbp->count, tt0005_PO_NO_LEN);
        gbp->count += tt0005_PO_NO_LEN;

        memset(ptr_recvbuf->order[gbp->i].num_items, '\0', tt0005_NUM_OF_ITM_LEN + 1);
		memcpy(ptr_recvbuf->order[gbp->i].num_items, sz_recvbufcat + gbp->count, tt0005_NUM_OF_ITM_LEN);
        gbp->count += tt0005_NUM_OF_ITM_LEN;

        memset(ptr_recvbuf->order[gbp->i].ship_date, '\0', tt0005_SHIP_DATE_LEN + 1);
		memcpy(ptr_recvbuf->order[gbp->i].ship_date, sz_recvbufcat + gbp->count, tt0005_SHIP_DATE_LEN);
        gbp->count += tt0005_SHIP_DATE_LEN;

        memset(ptr_recvbuf->order[gbp->i].num_shipments, '\0', tt0005_NUM_OF_SHIPMENTS_LEN + 1);
		memcpy(ptr_recvbuf->order[gbp->i].num_shipments, sz_recvbufcat + gbp->count, tt0005_NUM_OF_SHIPMENTS_LEN);
        gbp->count += tt0005_NUM_OF_SHIPMENTS_LEN;

        memset(ptr_recvbuf->order[gbp->i].ord_status, '\0', tt0005_STATUS_LEN + 1);
        memcpy(ptr_recvbuf->order[gbp->i].ord_status,	 sz_recvbufcat + gbp->count, tt0005_STATUS_LEN);
        gbp->count += tt0005_STATUS_LEN;

        memset(ptr_recvbuf->order[gbp->i].pty_flag, '\0', tt0005_PTY_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->order[gbp->i].pty_flag,	 sz_recvbufcat + gbp->count, tt0005_PTY_FLAG_LEN);
        gbp->count += tt0005_PTY_FLAG_LEN;

		if((ptr_recvbuf->order[gbp->i].ord_num[0]) != ' ' &&
			(ptr_recvbuf->order[gbp->i].ord_num[1]) != ' ' &&
			(ptr_recvbuf->order[gbp->i].ord_num[2]) != ' ' &&
			(ptr_recvbuf->order[gbp->i].ord_num[3]) != ' ' &&
			(ptr_recvbuf->order[gbp->i].ord_num[4]) != ' ' &&
			(ptr_recvbuf->order[gbp->i].ord_num[5]) != ' ')
		{
			ap_rprintf(r,"		<ORDER_DETAILS>\n");
			ap_rprintf(r,"			<SHORT_ORD_NUM>%s</SHORT_ORD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].ord_num));
			ap_rprintf(r,"			<DATE_ORD_PLACED>%s</DATE_ORD_PLACED>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].mail_date));
			ap_rprintf(r,"			<PO_FOR_DROP_SHIP>%s</PO_FOR_DROP_SHIP>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].po_num));
			ap_rprintf(r,"			<NUM_OF_PROD>%s</NUM_OF_PROD>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].num_items));
			ap_rprintf(r,"			<LAST_SHIP_DATE>%s</LAST_SHIP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].ship_date));
			ap_rprintf(r,"			<NUM_OF_SHIPMENTS>%s</NUM_OF_SHIPMENTS>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].num_shipments));
			ap_rprintf(r,"			<ORD_STATUS>%s</ORD_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].ord_status));
			ap_rprintf(r,"			<THIRD_PTY_FLAG>%s</THIRD_PTY_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->order[gbp->i].pty_flag));
			ap_rprintf(r,"		</ORDER_DETAILS>\n");
		}

    }

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0006>\n", gbp->tt_betag);

    return(0);
}

