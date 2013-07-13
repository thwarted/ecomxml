/* ft0025.c  Get upsell */
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


int             ft0025_CatSendStr(struct global_struct *, char *, ft0025_st_send *);
int             ft0025_ParceRecvStr(ft0025_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0025_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0025_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0025_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0025_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0025_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0025 = (ft0025_st_send *)malloc(sizeof(ft0025_st_send));
        gbp->ftrecvbuf0025 = (ft0025_st_recv *)malloc(sizeof(ft0025_st_recv));

        gbp->sendbufcat = malloc(ft0025_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0025_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0025_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0025_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0025,  '\0',  sizeof(ft0025_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0025->request_id, "XML");
    strcpy(gbp->ftsendbuf0025->record_id, "0025");
    strcpy(gbp->ftsendbuf0025->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0025->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0025->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0025->userid,gbp,stdout_buffer);
    get_tag_data("UPSELL_TYPE", gbp->ftsendbuf0025->upsell_type,gbp,stdout_buffer);
    get_tag_data("OFFER_NUM", gbp->ftsendbuf0025->offer_no,gbp,stdout_buffer);
    get_tag_data("ITEM_NUM", gbp->ftsendbuf0025->item_no,gbp,stdout_buffer);
    get_tag_data("UPSELL_KEY_TYPE", gbp->ftsendbuf0025->upsell_key_type,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_NUM", gbp->ftsendbuf0025->cust_no,gbp,stdout_buffer);
    get_tag_data("CALLED_BY", gbp->ftsendbuf0025->called_by,gbp,stdout_buffer);


        if(ft0025_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0025) == ft0025_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0025);
                        free (gbp->ftrecvbuf0025);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0025_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0025);
                        free (gbp->ftrecvbuf0025);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0025_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0025);
                        free (gbp->ftrecvbuf0025);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0025);
                        free (gbp->ftrecvbuf0025);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0025_ParceRecvStr(gbp->ftrecvbuf0025, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0025);
                free (gbp->ftrecvbuf0025);

        }
        else
        {
                XML_Error("ft0025_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0025);
                free (gbp->ftrecvbuf0025);
                return(-1);
        }

    return(0);
}


int ft0025_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0025_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-2.2s%-8.8s%-20.20s%-2.2s%-10.10s%-8.8s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->upsell_type,
            ptr_sendbuf->offer_no,
            ptr_sendbuf->item_no,
            ptr_sendbuf->upsell_key_type,
            ptr_sendbuf->cust_no,
            ptr_sendbuf->called_by);

	return(gbp->j);
}


int ft0025_ParceRecvStr(ft0025_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0025.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0025 %s\"ft0025\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0025_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0025_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0025_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0025_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0025_REC_ID_LEN );
    gbp->count += ft0025_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0025_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0025_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0025_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0025_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0025_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0025_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0025_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0025_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0025_ERR_LEN;

    gbp->count += ft0025_SEND_FILLER_LEN;

    for( gbp->i = 0; gbp->i < ft0025_UPSELL_LOOP; gbp->i++)
    {

        memset(ptr_recvbuf->upsell_detail[gbp->i].upsell_line ,'\0', ft0025_UPSELL_LINE_LEN + 1);
        memcpy(ptr_recvbuf->upsell_detail[gbp->i].upsell_line, sz_recvbufcat + gbp->count, ft0025_UPSELL_LINE_LEN);
        gbp->count += ft0025_UPSELL_LINE_LEN;

		if (strlen(ptr_recvbuf->upsell_detail[gbp->i].upsell_line) > 0 )
		{
		        ap_rprintf(r,"          <UPSELL_INFO>\n");
		        ap_rprintf(r,"                  <UPSELL_LINE>%s</UPSELL_LINE>\n", handle_special_chars(gbp,ptr_recvbuf->upsell_detail[gbp->i].upsell_line));
		        ap_rprintf(r,"          <UPSELL_INFO>\n");
		}
	}

    memset(ptr_recvbuf->alt,'\0', ft0025_ALT_LEN + 1);
    memcpy(ptr_recvbuf->alt, sz_recvbufcat + gbp->count, ft0025_ALT_LEN);
        ap_rprintf(r,"		<ALT>%s</ALT>\n", handle_special_chars(gbp,ptr_recvbuf->alt));
    gbp->count += ft0025_ALT_LEN;

    memset(ptr_recvbuf->add,'\0', ft0025_ADD_LEN + 1);
    memcpy(ptr_recvbuf->add, sz_recvbufcat + gbp->count, ft0025_ADD_LEN);
        ap_rprintf(r,"		<ADD>%s</ADD>\n", handle_special_chars(gbp,ptr_recvbuf->add));
    gbp->count += ft0025_ADD_LEN;

    memset(ptr_recvbuf->web_pr1,'\0', ft0025_WEB_LEN + 1);
    memcpy(ptr_recvbuf->web_pr1, sz_recvbufcat + gbp->count, ft0025_WEB_LEN);
        ap_rprintf(r,"		<WEB_PR1>%s</WEB_PR1>\n", handle_special_chars(gbp,ptr_recvbuf->web_pr1));
    gbp->count += ft0025_WEB_LEN;

    memset(ptr_recvbuf->web_pr2,'\0', ft0025_WEB_LEN + 1);
    memcpy(ptr_recvbuf->web_pr2, sz_recvbufcat + gbp->count, ft0025_WEB_LEN);
        ap_rprintf(r,"		<WEB_PR2>%s</WEB_PR2>\n", handle_special_chars(gbp,ptr_recvbuf->web_pr2));
    gbp->count += ft0025_WEB_LEN;

    memset(ptr_recvbuf->web_pr3,'\0', ft0025_WEB_LEN + 1);
    memcpy(ptr_recvbuf->web_pr3, sz_recvbufcat + gbp->count, ft0025_WEB_LEN);
        ap_rprintf(r,"		<WEB_PR3>%s</WEB_PR3>\n", handle_special_chars(gbp,ptr_recvbuf->web_pr3));
    gbp->count += ft0025_WEB_LEN;

    memset(ptr_recvbuf->alt_item_no,'\0', ft0025_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->alt_item_no, sz_recvbufcat + gbp->count, ft0025_ITEM_NO_LEN);
        ap_rprintf(r,"		<ALT_ITEM_NUM>%s</ALT_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->alt_item_no));
    gbp->count += ft0025_ITEM_NO_LEN;

    memset(ptr_recvbuf->add_item_no,'\0', ft0025_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->add_item_no, sz_recvbufcat + gbp->count, ft0025_ITEM_NO_LEN);
        ap_rprintf(r,"		<ADD_ITEM_NUM>%s</ADD_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->add_item_no));
    gbp->count += ft0025_ITEM_NO_LEN;

    memset(ptr_recvbuf->pr1_item_no,'\0', ft0025_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->pr1_item_no, sz_recvbufcat + gbp->count, ft0025_ITEM_NO_LEN);
        ap_rprintf(r,"		<PR1_ITEM_NUM>%s</PR1_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->pr1_item_no));
    gbp->count += ft0025_ITEM_NO_LEN;

    memset(ptr_recvbuf->pr2_item_no,'\0', ft0025_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->pr2_item_no, sz_recvbufcat + gbp->count, ft0025_ITEM_NO_LEN);
        ap_rprintf(r,"		<PR2_ITEM_NUM>%s</PR2_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->pr2_item_no));
    gbp->count += ft0025_ITEM_NO_LEN;

    memset(ptr_recvbuf->pr3_item_no,'\0', ft0025_ITEM_NO_LEN + 1);
    memcpy(ptr_recvbuf->pr3_item_no, sz_recvbufcat + gbp->count, ft0025_ITEM_NO_LEN);
        ap_rprintf(r,"		<PR3_ITEM_NUM>%s</PR3_ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->pr3_item_no));
    gbp->count += ft0025_ITEM_NO_LEN;

    memset(ptr_recvbuf->alt_item_desc,'\0', ft0025_DESC_LEN + 1);
    memcpy(ptr_recvbuf->alt_item_desc, sz_recvbufcat + gbp->count, ft0025_DESC_LEN);
        ap_rprintf(r,"		<ALT_ITEM_DESC>%s</ALT_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->alt_item_desc));
    gbp->count += ft0025_DESC_LEN;

    memset(ptr_recvbuf->add_item_desc,'\0', ft0025_DESC_LEN + 1);
    memcpy(ptr_recvbuf->add_item_desc, sz_recvbufcat + gbp->count, ft0025_DESC_LEN);
        ap_rprintf(r,"		<ADD_ITEM_DESC>%s</ADD_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->add_item_desc));
    gbp->count += ft0025_DESC_LEN;

    memset(ptr_recvbuf->pr1_item_desc,'\0', ft0025_DESC_LEN + 1);
    memcpy(ptr_recvbuf->pr1_item_desc, sz_recvbufcat + gbp->count, ft0025_DESC_LEN);
        ap_rprintf(r,"		<PR1_ITEM_DESC>%s</PR1_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->pr1_item_desc));
    gbp->count += ft0025_DESC_LEN;

    memset(ptr_recvbuf->pr2_item_desc,'\0', ft0025_DESC_LEN + 1);
    memcpy(ptr_recvbuf->pr2_item_desc, sz_recvbufcat + gbp->count, ft0025_DESC_LEN);
        ap_rprintf(r,"		<PR2_ITEM_DESC>%s</PR2_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->pr2_item_desc));
    gbp->count += ft0025_DESC_LEN;

    memset(ptr_recvbuf->pr3_item_desc,'\0', ft0025_DESC_LEN + 1);
    memcpy(ptr_recvbuf->pr3_item_desc, sz_recvbufcat + gbp->count, ft0025_DESC_LEN);
        ap_rprintf(r,"		<PR3_ITEM_DESC>%s</PR3_ITEM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->pr3_item_desc));
    gbp->count += ft0025_DESC_LEN;

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0025>\n", gbp->ft_betag);


    return(0);
}

