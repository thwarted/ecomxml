/* tt0023.c  Customization Items */
/* modified for phase 2 */
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


int             tt0023_CatSendStr(struct global_struct *,char *, tt0023_st_send *);
int             tt0023_ParceRecvStr(tt0023_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0023_lt_process(request_rec *, struct global_struct *,char *);


int tt0023_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}

int tt0023_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0023_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

/******************************************************************/
/*                                                                */
/*  Process the request                                           */
/*                                                                */
/******************************************************************/


int tt0023_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0023 = (tt0023_st_send *)malloc(sizeof(tt0023_st_send));
        gbp->recvbuf0023 = (tt0023_st_recv *)malloc(sizeof(tt0023_st_recv));

        gbp->sendbufcat = malloc(tt0023_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0023_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0023_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0023_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0023,  '\0',  sizeof(tt0023_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0023->request_id, "XML");
    strcpy(gbp->sendbuf0023->record_id, "0023");

        strcpy(gbp->sendbuf0023->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0023->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0023->division,gbp,stdout_buffer);
        get_tag_data("UID",  gbp->sendbuf0023->userid,gbp,stdout_buffer);
        get_tag_data("PROD_NUM",  gbp->sendbuf0023->item_num,gbp,stdout_buffer);
        get_tag_data("PAGE_NO",  gbp->sendbuf0023->page_no,gbp,stdout_buffer);
        get_tag_data("SHOP_CART_LINE_NUM",  gbp->tagname,gbp,stdout_buffer);
        get_tag_data("COMP_SEQ_NUMBER",  gbp->sendbuf0023->comp_seq_number,gbp,stdout_buffer);
        get_tag_data("ITEM_TYPE",  gbp->sendbuf0023->item_type,gbp,stdout_buffer);
        get_tag_data("D1_CUSTOM_FRAME",  gbp->sendbuf0023->customframe,gbp,stdout_buffer);

    gbp->i = atoi(gbp->tagname);
    sprintf(gbp->sendbuf0023->cart_line_num, "%03d", gbp->i);

    
        if((tt0023_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0023)) == tt0023_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0023);
                        free (gbp->recvbuf0023);
                        return(-1);
                }

        
                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0023_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0023);
                        free (gbp->recvbuf0023);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0023_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0023);
                        free (gbp->recvbuf0023);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0023);
                        free (gbp->recvbuf0023);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0023_ParceRecvStr(gbp->recvbuf0023, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0023);
                free (gbp->recvbuf0023);
        }
        else
        {
                XML_Error("tt0023_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0023);
                free (gbp->recvbuf0023);
                return(-1);
        }

    return(0);
}


int tt0023_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0023_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-20.20s%-2.2s%-3.3s%-3.3s%-1.1s%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->item_num,
            ptr_sendbuf->page_no,
            ptr_sendbuf->cart_line_num,
                        ptr_sendbuf->comp_seq_number,
                        ptr_sendbuf->item_type,
                        ptr_sendbuf->customframe);

    return(gbp->j);
}


int tt0023_ParceRecvStr(tt0023_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;
    // Place the individual variables into target fields

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0024.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0024 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0024.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0024 %s\"tt0024\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

        memset(ptr_recvbuf->request_id, '\0', tt0023_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0023_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0023_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0023_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0023_REC_ID_LEN );
    gbp->count += tt0023_REC_ID_LEN;

        memset(ptr_recvbuf->userid, '\0', tt0023_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0023_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0023_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0023_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0023_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0023_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0023_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0023_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0023_ERR_LEN;

        gbp->count += tt0023_SEND_FILLER_LEN;


    memset(ptr_recvbuf->last_customization, '\0', tt0023_LAST_LEN + 1);
    memcpy(ptr_recvbuf->last_customization, sz_recvbufcat + gbp->count, tt0023_LAST_LEN);
        ap_rprintf(r,"		<CUSTOMIZATION_LINE_NUM>%s</CUSTOMIZATION_LINE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->last_customization));
    gbp->count += tt0023_LAST_LEN;


    // Build data from array

    for(gbp->i = 0; gbp->i < tt0023_CUSTOMIZE_COUNT; gbp->i++) 
        {
        memset(ptr_recvbuf->customization[gbp->i].customization_line, '\0', tt0023_CUSTOMIZE_LEN +1);
                memcpy(ptr_recvbuf->customization[gbp->i].customization_line, sz_recvbufcat + gbp->count, tt0023_CUSTOMIZE_LEN);
        gbp->count += tt0023_CUSTOMIZE_LEN;

		memset(ptr_recvbuf->customization[gbp->i].comp_edp_no, '\0', tt0023_COMP_EDP_LEN +1);
                memcpy(ptr_recvbuf->customization[gbp->i].comp_edp_no, sz_recvbufcat + gbp->count, tt0023_COMP_EDP_LEN);
        gbp->count += tt0023_COMP_EDP_LEN;
    }

        gbp->k = atoi(ptr_recvbuf->last_customization);
        if(gbp->k > (int)tt0023_CUSTOMIZE_COUNT)
                gbp->k = (int)tt0023_CUSTOMIZE_COUNT;

    for(gbp->i = 0; gbp->i < gbp->k ; gbp->i++) 
        {
                ap_rprintf(r,"		<CUSTOMIZATION_DATA>\n");
                ap_rprintf(r,"			<CUSTOMIZATION_LINES>%s</CUSTOMIZATION_LINES>\n", handle_special_chars(gbp,ptr_recvbuf->customization[gbp->i].customization_line));
				ap_rprintf(r,"			<CUSTOMIZATION_COMP_EDP>%s</CUSTOMIZATION_COMP_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->customization[gbp->i].comp_edp_no));
                ap_rprintf(r,"		</CUSTOMIZATION_DATA>\n");
        }

    memset(ptr_recvbuf->page_no, '\0', tt0023_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, tt0023_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NO>%s</PAGE_NO>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += tt0023_PAGE_NO_LEN;

    memset(ptr_recvbuf->item_line_number, '\0', tt0023_ITEM_LINE_NUMBER_LEN + 1);
    memcpy(ptr_recvbuf->item_line_number, sz_recvbufcat + gbp->count, tt0023_ITEM_LINE_NUMBER_LEN);
        ap_rprintf(r,"		<ITEM_LINE_NUMBER>%s</ITEM_LINE_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->item_line_number));
    gbp->count += tt0023_ITEM_LINE_NUMBER_LEN;

    memset(ptr_recvbuf->comp_seq_number, '\0', tt0023_COMP_SEQ_NUMBER_LEN + 1);
    memcpy(ptr_recvbuf->comp_seq_number, sz_recvbufcat + gbp->count, tt0023_COMP_SEQ_NUMBER_LEN);
        ap_rprintf(r,"		<COMP_SEQ_NUMBER>%s</COMP_SEQ_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->comp_seq_number));
    gbp->count += tt0023_COMP_SEQ_NUMBER_LEN;

    memset(ptr_recvbuf->item_type, '\0', tt0023_ITEM_TYPE_LEN + 1);
    memcpy(ptr_recvbuf->item_type, sz_recvbufcat + gbp->count, tt0023_ITEM_TYPE_LEN);
        ap_rprintf(r,"		<ITEM_TYPE>%s</ITEM_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->item_type));
    gbp->count += tt0023_ITEM_TYPE_LEN;

    memset(ptr_recvbuf->customframe, '\0', tt0023_CUSTOMFRAME_LEN + 1);
    memcpy(ptr_recvbuf->customframe, sz_recvbufcat + gbp->count, tt0023_CUSTOMFRAME_LEN);
        ap_rprintf(r,"		<D1_CUSTOM_FRAME>%s</D1_CUSTOM_FRAME>\n", handle_special_chars(gbp,ptr_recvbuf->customframe));
    gbp->count += tt0023_CUSTOMFRAME_LEN;

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0024>\n", gbp->tt_betag);

    return 0;
}
