/* ft0023.c  Validate source */
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


int             ft0023_CatSendStr(struct global_struct *, char *, ft0023_st_send *);
int             ft0023_ParceRecvStr(ft0023_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0023_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0023_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0023_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0023_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0023_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0023 = (ft0023_st_send *)malloc(sizeof(ft0023_st_send));
        gbp->ftrecvbuf0023 = (ft0023_st_recv *)malloc(sizeof(ft0023_st_recv));

        gbp->sendbufcat = malloc(ft0023_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0023_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0023_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0023_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0023,  '\0',  sizeof(ft0023_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0023->request_id, "XML");
    strcpy(gbp->ftsendbuf0023->record_id, "0023");
    strcpy(gbp->ftsendbuf0023->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0023->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0023->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0023->userid,gbp,stdout_buffer);
    get_tag_data("OFFER", gbp->ftsendbuf0023->offer,gbp,stdout_buffer);
    get_tag_data("USE", gbp->ftsendbuf0023->use,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_NUM", gbp->ftsendbuf0023->cust_no,gbp,stdout_buffer);


        if(ft0023_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0023) == ft0023_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0023);
                        free (gbp->ftrecvbuf0023);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0023_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0023);
                        free (gbp->ftrecvbuf0023);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0023_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0023);
                        free (gbp->ftrecvbuf0023);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0023);
                        free (gbp->ftrecvbuf0023);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0023_ParceRecvStr(gbp->ftrecvbuf0023, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0023);
                free (gbp->ftrecvbuf0023);

        }
        else
        {
                XML_Error("ft0023_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0023);
                free (gbp->ftrecvbuf0023);
                return(-1);
        }

    return(0);
}


int ft0023_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0023_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-8.8s%-8.8s%-10.10s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->offer,
            ptr_sendbuf->use,
            ptr_sendbuf->cust_no);

	return(gbp->j);
}


int ft0023_ParceRecvStr(ft0023_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0023.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0023 %s\"ft0023\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0023_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0023_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0023_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0023_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0023_REC_ID_LEN );
    gbp->count += ft0023_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0023_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0023_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0023_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0023_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0023_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0023_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0023_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0023_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0023_ERR_LEN;

    gbp->count += ft0023_SEND_FILLER_LEN;

    memset(ptr_recvbuf->source,'\0', ft0023_SOURCE_LEN + 1);
    memcpy(ptr_recvbuf->source, sz_recvbufcat + gbp->count, ft0023_SOURCE_LEN);
        ap_rprintf(r,"		<SOURCE>%s</SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->source));
    gbp->count += ft0023_SOURCE_LEN;

    memset(ptr_recvbuf->company,'\0', ft0023_COMPANY_LEN + 1);
    memcpy(ptr_recvbuf->company, sz_recvbufcat + gbp->count, ft0023_COMPANY_LEN);
        ap_rprintf(r,"		<COMPANY>%s</COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->company));
    gbp->count += ft0023_COMPANY_LEN;

    memset(ptr_recvbuf->division,'\0', ft0023_DIVISION_LEN + 1);
    memcpy(ptr_recvbuf->division, sz_recvbufcat + gbp->count, ft0023_DIVISION_LEN);
        ap_rprintf(r,"		<DIVISION>%s</DIVISION>\n", handle_special_chars(gbp,ptr_recvbuf->division));
    gbp->count += ft0023_DIVISION_LEN;

    memset(ptr_recvbuf->install_flag,'\0', ft0023_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->install_flag, sz_recvbufcat + gbp->count, ft0023_FLAG_LEN);
        ap_rprintf(r,"		<INSTALL_FLAG>%s</INSTALL_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->install_flag));
    gbp->count += ft0023_FLAG_LEN;

    memset(ptr_recvbuf->invalid_cc,'\0', ft0023_CC_LEN + 1);
    memcpy(ptr_recvbuf->invalid_cc, sz_recvbufcat + gbp->count, ft0023_CC_LEN);
        ap_rprintf(r,"		<INVALID_CC>%s</INVALID_CC>\n", handle_special_chars(gbp,ptr_recvbuf->invalid_cc));
    gbp->count += ft0023_CC_LEN;

    memset(ptr_recvbuf->future_date,'\0', ft0023_DATE_LEN + 1);
    memcpy(ptr_recvbuf->future_date, sz_recvbufcat + gbp->count, ft0023_DATE_LEN);
        ap_rprintf(r,"		<FUTURE_DATE>%s</FUTURE_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->future_date));
    gbp->count += ft0023_DATE_LEN;

    for( gbp->i = 0; gbp->i < ft0023_PRODUCT_LOOP; gbp->i++)
    {

        memset(ptr_recvbuf->insert_product[gbp->i].ins_product ,'\0', ft0023_PRODUCT_LEN + 1);
        memcpy(ptr_recvbuf->insert_product[gbp->i].ins_product, sz_recvbufcat + gbp->count, ft0023_PRODUCT_LEN);
        gbp->count += ft0023_PRODUCT_LEN;

        ap_rprintf(r,"          <INSERT_PRODUCT_INFO>\n");
        ap_rprintf(r,"                  <INSERT_PRODUCT>%s</INSERT_PRODUCT>\n", handle_special_chars(gbp,ptr_recvbuf->insert_product[gbp->i].ins_product));
        ap_rprintf(r,"          <INSERT_PRODUCT_INFO>\n");
	}

    memset(ptr_recvbuf->ship_method,'\0', ft0023_SHIP_METH_LEN + 1);
    memcpy(ptr_recvbuf->ship_method, sz_recvbufcat + gbp->count, ft0023_SHIP_METH_LEN);
        ap_rprintf(r,"		<SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method));
    gbp->count += ft0023_SHIP_METH_LEN;

    memset(ptr_recvbuf->discount_table,'\0', ft0023_DISC_LEN + 1);
    memcpy(ptr_recvbuf->discount_table, sz_recvbufcat + gbp->count, ft0023_DISC_LEN);
        ap_rprintf(r,"		<DISCOUNT_TABLE>%s</DISCOUNT_TABLE>\n", handle_special_chars(gbp,ptr_recvbuf->discount_table));
    gbp->count += ft0023_DISC_LEN;

    memset(ptr_recvbuf->exp_chain,'\0', ft0023_EXP_CHAIN_LEN + 1);
    memcpy(ptr_recvbuf->exp_chain, sz_recvbufcat + gbp->count, ft0023_EXP_CHAIN_LEN);
        ap_rprintf(r,"		<EXPEDITE_CHAIN>%s</EXPEDITE_CHAIN>\n", handle_special_chars(gbp,ptr_recvbuf->exp_chain));
    gbp->count += ft0023_EXP_CHAIN_LEN;

    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0023>\n", gbp->ft_betag);


    return(0);
}

