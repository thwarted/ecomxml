/* ft0008.c  Get Comments */
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


int             ft0008_CatSendStr(struct global_struct *, char *, ft0008_st_send *);
int             ft0008_ParceRecvStr(ft0008_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0008_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0008_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0008_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0008_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0008_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0008 = (ft0008_st_send *)malloc(sizeof(ft0008_st_send));
        gbp->ftrecvbuf0008 = (ft0008_st_recv *)malloc(sizeof(ft0008_st_recv));

        gbp->sendbufcat = malloc(ft0008_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0008_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0008_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0008_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0008,  '\0',  sizeof(ft0008_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0008->request_id, "XML");
    strcpy(gbp->ftsendbuf0008->record_id, "0008");
    strcpy(gbp->ftsendbuf0008->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0008->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0008->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0008->userid,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_NUM", gbp->ftsendbuf0008->cust_no,gbp,stdout_buffer);
    get_tag_data("FULL_ORDER_NUM", gbp->ftsendbuf0008->full_order_no,gbp,stdout_buffer);
    get_tag_data("PAGE_NUM", gbp->ftsendbuf0008->page_no,gbp,stdout_buffer);
    get_tag_data("PAGE_COUNT", gbp->ftsendbuf0008->page_cnt,gbp,stdout_buffer);
    get_tag_data("WHO_CALLED", gbp->ftsendbuf0008->who_called,gbp,stdout_buffer);
    get_tag_data("WHY_CALLED", gbp->ftsendbuf0008->why_called,gbp,stdout_buffer);

        if(ft0008_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0008) == ft0008_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0008);
                        free (gbp->ftrecvbuf0008);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0008_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0008);
                        free (gbp->ftrecvbuf0008);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0008_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0008);
                        free (gbp->ftrecvbuf0008);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0008);
                        free (gbp->ftrecvbuf0008);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0008_ParceRecvStr(gbp->ftrecvbuf0008, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0008);
                free (gbp->ftrecvbuf0008);

        }
        else
        {
                XML_Error("ft0008_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0008);
                free (gbp->ftrecvbuf0008);
                return(-1);
        }

    return(0);
}


int ft0008_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0008_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-10.10s%-12.12s%04d%04d%-2.2s%-2.2s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->cust_no,
            ptr_sendbuf->full_order_no,
            ptr_sendbuf->page_no,
            ptr_sendbuf->page_cnt,
            ptr_sendbuf->who_called,
            ptr_sendbuf->why_called);

    return(gbp->j);
}


int ft0008_ParceRecvStr(ft0008_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0008.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0008 %s\"ft0008\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0008_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0008_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0008_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0008_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0008_REC_ID_LEN );
    gbp->count += ft0008_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0008_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0008_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0008_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0008_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0008_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0008_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0008_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0008_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0008_ERR_LEN;

    gbp->count += ft0008_SEND_FILLER_LEN;

    memset(ptr_recvbuf->comment_cnt,'\0', ft0008_COMM_CNT_LEN + 1);
    memcpy(ptr_recvbuf->comment_cnt, sz_recvbufcat + gbp->count, ft0008_COMM_CNT_LEN);
        ap_rprintf(r,"		<COMMENT_COUNT>%s</COMMENT_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->comment_cnt));
    gbp->count += ft0008_COMM_CNT_LEN;
    
    memset(ptr_recvbuf->page_no,'\0', ft0008_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, ft0008_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NUM>%s</PAGE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += ft0008_PAGE_NO_LEN;

    gbp->count += ft0008_FILLER1_LEN;

    for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->comment_cnt); gbp->i++)
    {

        memset(ptr_recvbuf->comment_detail[gbp->i].comment ,'\0', ft0008_COMMENT_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].comment, sz_recvbufcat + gbp->count, ft0008_COMMENT_LEN);
        gbp->count += ft0008_COMMENT_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].print_pks ,'\0', ft0008_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].print_pks, sz_recvbufcat + gbp->count, ft0008_FLAG_LEN);
        gbp->count += ft0008_FLAG_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].print_inv ,'\0', ft0008_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].print_inv, sz_recvbufcat + gbp->count, ft0008_FLAG_LEN);
        gbp->count += ft0008_FLAG_LEN;

		
        memset(ptr_recvbuf->comment_detail[gbp->i].print_lbl ,'\0', ft0008_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].print_lbl, sz_recvbufcat + gbp->count, ft0008_FLAG_LEN);
        gbp->count += ft0008_FLAG_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].print_whs ,'\0', ft0008_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].print_whs, sz_recvbufcat + gbp->count, ft0008_FLAG_LEN);
        gbp->count += ft0008_FLAG_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].ci_date ,'\0', ft0008_DATE_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].ci_date, sz_recvbufcat + gbp->count, ft0008_DATE_LEN);
        gbp->count += ft0008_DATE_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].ci_oper ,'\0', ft0008_OPER_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].ci_oper, sz_recvbufcat + gbp->count, ft0008_OPER_LEN);
        gbp->count += ft0008_OPER_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].ci_type ,'\0', ft0008_TYPE_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].ci_type, sz_recvbufcat + gbp->count, ft0008_TYPE_LEN);
        gbp->count += ft0008_TYPE_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].seq_no ,'\0', ft0008_SEQ_NO_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].seq_no, sz_recvbufcat + gbp->count, ft0008_SEQ_NO_LEN);
        gbp->count += ft0008_SEQ_NO_LEN;

        memset(ptr_recvbuf->comment_detail[gbp->i].record_no ,'\0', ft0008_REC_NO_LEN + 1);
        memcpy(ptr_recvbuf->comment_detail[gbp->i].record_no, sz_recvbufcat + gbp->count, ft0008_REC_NO_LEN);
        gbp->count += ft0008_REC_NO_LEN;

        ap_rprintf(r,"          <COMMENT_INFO>\n");
        ap_rprintf(r,"                  <COMMENT>%s</COMMENT>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].comment));
        ap_rprintf(r,"                  <PRINT_ON_PKS>%s</PRINT_ON_PKS>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].print_pks));
        ap_rprintf(r,"                  <PRINT_ON_INV>%s</PRINT_ON_INV>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].print_inv));
        ap_rprintf(r,"                  <PRINT_ON_LBL>%s</PRINT_ON_LBL>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].print_lbl));
        ap_rprintf(r,"                  <PRINT_ON_WHS>%s</PRINT_ON_WHS>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].print_whs));
        ap_rprintf(r,"                  <CI_DATE>%s</CI_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].ci_date));
        ap_rprintf(r,"                  <CI_OPER>%s</CI_OPER>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].ci_oper));
        ap_rprintf(r,"                  <CI_TYPE>%s</CI_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].ci_type));
        ap_rprintf(r,"                  <SEQ_NUM>%s</SEQ_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].seq_no));
        ap_rprintf(r,"                  <REC_NUM>%s</REC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->comment_detail[gbp->i].record_no));
        ap_rprintf(r,"          </COMMENT_INFO>\n");

	}

		ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0008>\n", gbp->ft_betag);


    return(0);
}

