/* ft0030.c  Get Customization */
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


int             ft0030_CatSendStr(struct global_struct *, char *, ft0030_st_send *);
int             ft0030_ParceRecvStr(ft0030_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0030_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0030_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0030_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0030_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0030_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0030 = (ft0030_st_send *)malloc(sizeof(ft0030_st_send));
        gbp->ftrecvbuf0030 = (ft0030_st_recv *)malloc(sizeof(ft0030_st_recv));

        gbp->sendbufcat = malloc(ft0030_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0030_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0030_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0030_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0030,  '\0',  sizeof(ft0030_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0030->request_id, "XML");
    strcpy(gbp->ftsendbuf0030->record_id, "0030");
    strcpy(gbp->ftsendbuf0030->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0030->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0030->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0030->userid,gbp,stdout_buffer);
    get_tag_data("ORDER_NUM", gbp->ftsendbuf0030->order_no,gbp,stdout_buffer);
    get_tag_data("LINE_NUM", gbp->ftsendbuf0030->line_no,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_NUM", gbp->ftsendbuf0030->cust_no,gbp,stdout_buffer);
    get_tag_data("ITEM_NUM", gbp->ftsendbuf0030->item_no,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_CODE", gbp->ftsendbuf0030->cust_code,gbp,stdout_buffer);


        if(ft0030_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0030) == ft0030_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0030);
                        free (gbp->ftrecvbuf0030);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0030_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0030);
                        free (gbp->ftrecvbuf0030);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0030_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0030);
                        free (gbp->ftrecvbuf0030);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0030);
                        free (gbp->ftrecvbuf0030);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0030_ParceRecvStr(gbp->ftrecvbuf0030, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0030);
                free (gbp->ftrecvbuf0030);

        }
        else
        {
                XML_Error("ft0030_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0030);
                free (gbp->ftrecvbuf0030);
                return(-1);
        }

    return(0);
}


int ft0030_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0030_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-8.8s%04d%-10.10s%-20.20s%-4.4s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->order_no,
            atoi(ptr_sendbuf->line_no),
            ptr_sendbuf->cust_no,
            ptr_sendbuf->item_no,
            ptr_sendbuf->cust_code);

	return(gbp->j);
}


int ft0030_ParceRecvStr(ft0030_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0030.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0030 %s\"ft0030\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0030_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0030_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0030_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0030_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0030_REC_ID_LEN );
    gbp->count += ft0030_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0030_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0030_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0030_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0030_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0030_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0030_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0030_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0030_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0030_ERR_LEN;

    gbp->count += ft0030_SEND_FILLER_LEN;

    memset(ptr_recvbuf->cust_cnt,'\0', ft0030_CUST_CNT_LEN + 1);
    memcpy(ptr_recvbuf->cust_cnt, sz_recvbufcat + gbp->count, ft0030_CUST_CNT_LEN);
        ap_rprintf(r,"		<CUSTOMIZATION_COUNT>%s</CUSTOMIZATION_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->cust_cnt));
    gbp->count += ft0030_CUST_CNT_LEN;

    for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->cust_cnt); gbp->i++)
    {

        memset(ptr_recvbuf->customization_detail[gbp->i].prompt ,'\0', ft0030_PROMPT_LEN + 1);
        memcpy(ptr_recvbuf->customization_detail[gbp->i].prompt, sz_recvbufcat + gbp->count, ft0030_PROMPT_LEN);
        gbp->count += ft0030_PROMPT_LEN;

        memset(ptr_recvbuf->customization_detail[gbp->i].length ,'\0', ft0030_LENGTH_LEN + 1);
        memcpy(ptr_recvbuf->customization_detail[gbp->i].length, sz_recvbufcat + gbp->count, ft0030_LENGTH_LEN);
        gbp->count += ft0030_LENGTH_LEN;

        memset(ptr_recvbuf->customization_detail[gbp->i].value ,'\0', ft0030_VALUE_LEN + 1);
        memcpy(ptr_recvbuf->customization_detail[gbp->i].value, sz_recvbufcat + gbp->count, ft0030_VALUE_LEN);
        gbp->count += ft0030_VALUE_LEN;

        ap_rprintf(r,"          <CUSTOMIZATION_INFO>\n");
        ap_rprintf(r,"                  <PROMPT>%s</PROMPT>\n", handle_special_chars(gbp,ptr_recvbuf->customization_detail[gbp->i].prompt));
        ap_rprintf(r,"                  <LENGTH>%s</LENGTH>\n", handle_special_chars(gbp,ptr_recvbuf->customization_detail[gbp->i].length));
        ap_rprintf(r,"                  <VALUE>%s</VALUE>\n", handle_special_chars(gbp,ptr_recvbuf->customization_detail[gbp->i].value));
        ap_rprintf(r,"          <CUSTOMIZATION_INFO>\n");
	}


    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0030>\n", gbp->ft_betag);


    return(0);
}

