/* wl0005.c   */
/*modified for phase 2 - JL */
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


int             wl0005_CatSendStr(struct global_struct *, char *, wl0005_st_send *);
int             wl0005_ParceRecvStr(wl0005_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             wl0005_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int wl0005_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wl0005_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wl0005_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wl0005_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wlsendbuf0005 = (wl0005_st_send *)malloc(sizeof(wl0005_st_send));
        gbp->wlrecvbuf0005 = (wl0005_st_recv *)malloc(sizeof(wl0005_st_recv));

        gbp->sendbufcat = malloc(wl0005_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(wl0005_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', wl0005_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wl0005_LAN_RECV_BUF_LEN + 1);

    memset(gbp->wlsendbuf0005,  '\0',  sizeof(wl0005_st_send));

    sprintf(gbp->confirmbuf, "     ");

    get_tag_data("TRANSACTION_ID", gbp->wlsendbuf0005->record_id,gbp,stdout_buffer);
    get_tag_data("COMPANY", gbp->wlsendbuf0005->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->wlsendbuf0005->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->wlsendbuf0005->userid,gbp,stdout_buffer);
    get_tag_data("EMAIL", gbp->wlsendbuf0005->email,gbp,stdout_buffer);
    get_tag_data("LNAME", gbp->wlsendbuf0005->lname,gbp,stdout_buffer);
    get_tag_data("FNAME", gbp->wlsendbuf0005->fname,gbp,stdout_buffer);
    get_tag_data("CITY", gbp->wlsendbuf0005->city,gbp,stdout_buffer);
    get_tag_data("STATE", gbp->wlsendbuf0005->state,gbp,stdout_buffer);

        if(wl0005_CatSendStr(gbp, gbp->sendbufcat, gbp->wlsendbuf0005) == wl0005_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->wlport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0005);
                        free (gbp->wlrecvbuf0005);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, wl0005_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0005);
                        free (gbp->wlrecvbuf0005);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wl0005_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0005);
                        free (gbp->wlrecvbuf0005);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0005);
                        free (gbp->wlrecvbuf0005);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                wl0005_ParceRecvStr(gbp->wlrecvbuf0005, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0005);
                free (gbp->wlrecvbuf0005);

        }
        else
        {
                XML_Error("wl0005_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0005);
                free (gbp->wlrecvbuf0005);
                return(-1);
        }

    return(0);
}


int wl0005_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, wl0005_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-2.2s%-2.2s%-16.16s%-30.30s"
			"%-47.47s%-16.16s%-16.16s%-30.30s%-2.2s",

            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->filler,
            ptr_sendbuf->email,
            ptr_sendbuf->lname,
            ptr_sendbuf->fname,
            ptr_sendbuf->city,
            ptr_sendbuf->state);

    return(gbp->j);
}


int wl0005_ParceRecvStr(wl0005_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wl0005.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, wl_tag);
*/
        ap_rprintf(r,"%s0005 %s\"wl0005\">\n", gbp->wl_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->record_id,'\0', wl0005_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, wl0005_REC_ID_LEN );
    gbp->count += wl0005_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', wl0005_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, wl0005_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += wl0005_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', wl0005_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, wl0005_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += wl0005_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', wl0005_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, wl0005_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += wl0005_ERR_LEN;

    gbp->count += wl0005_RECV_FILLER_LEN;

    memset(ptr_recvbuf->count,'\0', wl0005_COUNT_LEN + 1);
    memcpy(ptr_recvbuf->count, sz_recvbufcat + gbp->count, wl0005_COUNT_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->count));
    gbp->count += wl0005_COUNT_LEN;

	for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->count); gbp->i++)
	{
		memset(ptr_recvbuf->customer_detail[gbp->i].lname ,'\0', wl0005_NAME_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].lname, sz_recvbufcat + gbp->count, wl0005_NAME_LEN);
		gbp->count += wl0005_NAME_LEN;

		memset(ptr_recvbuf->customer_detail[gbp->i].fname ,'\0', wl0005_NAME_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].fname, sz_recvbufcat + gbp->count, wl0005_NAME_LEN);
		gbp->count += wl0005_NAME_LEN;

		memset(ptr_recvbuf->customer_detail[gbp->i].street ,'\0', wl0005_STREET_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].street, sz_recvbufcat + gbp->count, wl0005_STREET_LEN);
		gbp->count += wl0005_STREET_LEN;

		memset(ptr_recvbuf->customer_detail[gbp->i].city ,'\0', wl0005_CITY_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].city, sz_recvbufcat + gbp->count, wl0005_CITY_LEN);
		gbp->count += wl0005_CITY_LEN;

		memset(ptr_recvbuf->customer_detail[gbp->i].state ,'\0', wl0005_STATE_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].state, sz_recvbufcat + gbp->count, wl0005_STATE_LEN);
		gbp->count += wl0005_STATE_LEN;

		memset(ptr_recvbuf->customer_detail[gbp->i].seq_no ,'\0', wl0005_SEQ_NO_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].seq_no, sz_recvbufcat + gbp->count, wl0005_SEQ_NO_LEN);
		gbp->count += wl0005_SEQ_NO_LEN;

		memset(ptr_recvbuf->customer_detail[gbp->i].password_req ,'\0', wl0005_PSWD_FLAG_LEN + 1);
		memcpy(ptr_recvbuf->customer_detail[gbp->i].password_req, sz_recvbufcat + gbp->count, wl0005_PSWD_FLAG_LEN);
		gbp->count += wl0005_PSWD_FLAG_LEN;


		ap_rprintf(r,"          <CUSTOMER_INFO>\n");
    		ap_rprintf(r,"                  <LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].lname));
    		ap_rprintf(r,"                  <FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].fname));
    		ap_rprintf(r,"                  <STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].street));
    		ap_rprintf(r,"                  <CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].city));
    		ap_rprintf(r,"                  <STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].state));
    		ap_rprintf(r,"                  <SEQ_NUM>%s</SEQ_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].seq_no));
    		ap_rprintf(r,"                  <PASSWORD_REQ>%s</PASSWORD_REQ>\n", handle_special_chars(gbp,ptr_recvbuf->customer_detail[gbp->i].password_req));
		ap_rprintf(r,"          </CUSTOMER_INFO>\n");
	}


 
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0005>\n", gbp->wl_betag);


    return(0);
}

