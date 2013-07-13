/* tt0087.c Variable Custom Data */
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


int             tt0087_CatSendStr(struct global_struct *,char *, tt0087_st_send *);
int             tt0087_ParceRecvStr(tt0087_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0087_lt_process(request_rec *, struct global_struct *, char *);


int tt0087_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0087_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0087_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0087_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0087 = (tt0087_st_send *)malloc(sizeof(tt0087_st_send));
        gbp->recvbuf0087 = (tt0087_st_recv *)malloc(sizeof(tt0087_st_recv));

        gbp->sendbufcat = malloc(tt0087_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0087_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', tt0087_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0087_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0087,  '\0',  sizeof(tt0087_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0087->request_id, "XML");
    strcpy(gbp->sendbuf0087->record_id, "0087");

        strcpy(gbp->sendbuf0087->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0087->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0087->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0087->userid,gbp,stdout_buffer);
        get_tag_data("PROD_NUM", gbp->sendbuf0087->item_num,gbp,stdout_buffer);

        for(gbp->i = 0; gbp->i < tt0087_CUSTOM_ITEM_COUNT; gbp->i++)
        {
                sprintf(gbp->tagname, "CUSTOM_ITEM_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0087->custom_items[gbp->i].custom_item,gbp,stdout_buffer);
				sprintf(gbp->tagname, "CUSTOMIZATION_COMP_EDP_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0087->custom_items[gbp->i].comp_edp_no,gbp,stdout_buffer);

        }
        
        get_tag_data("SHIPTO_NUM", gbp->sendbuf0087->shipto_num,gbp,stdout_buffer);
        get_tag_data("PAGE_NO", gbp->sendbuf0087->page_no,gbp,stdout_buffer);
        get_tag_data("ITEM_LINE_NUMBER", gbp->sendbuf0087->item_line_number,gbp,stdout_buffer);
        get_tag_data("COMP_SEQ_NUMBER", gbp->sendbuf0087->comp_seq_number,gbp,stdout_buffer);
        get_tag_data("FLAG", gbp->sendbuf0087->flag,gbp,stdout_buffer);
        get_tag_data("ITEM_TYPE", gbp->sendbuf0087->item_type,gbp,stdout_buffer);
        get_tag_data("D1_CUSTOM_FRAME", gbp->sendbuf0087->customframe,gbp,stdout_buffer);

        
        if(tt0087_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0087) == tt0087_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0087);
                        free (gbp->recvbuf0087);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0087_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0087);
                        free (gbp->recvbuf0087);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0087_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0087);
                        free (gbp->recvbuf0087);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0087);
                        free (gbp->recvbuf0087);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0087_ParceRecvStr(gbp->recvbuf0087, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0087);
                free (gbp->recvbuf0087);

        }
        else
        {
                XML_Error("tt0087_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0087);
                free (gbp->recvbuf0087);
                return(-1);
        }

    return(0);
}


int tt0087_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0087_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-20.20s"
                        "%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s"
                        "%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s%-60.60s%-9.9s"
                        "%-2.2s%-2.2s%-3.3s%-3.3s%-1.1s%-1.1s%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->item_num,
            ptr_sendbuf->custom_items[0].custom_item,
			ptr_sendbuf->custom_items[0].comp_edp_no,
                        ptr_sendbuf->custom_items[1].custom_item,
						ptr_sendbuf->custom_items[1].comp_edp_no,
                        ptr_sendbuf->custom_items[2].custom_item,
						ptr_sendbuf->custom_items[2].comp_edp_no,
                        ptr_sendbuf->custom_items[3].custom_item,
						ptr_sendbuf->custom_items[3].comp_edp_no,
                        ptr_sendbuf->custom_items[4].custom_item,
						ptr_sendbuf->custom_items[4].comp_edp_no,
                        ptr_sendbuf->custom_items[5].custom_item,
						ptr_sendbuf->custom_items[5].comp_edp_no,
                        ptr_sendbuf->custom_items[6].custom_item,
						ptr_sendbuf->custom_items[6].comp_edp_no,
                        ptr_sendbuf->custom_items[7].custom_item,
						ptr_sendbuf->custom_items[7].comp_edp_no,
                        ptr_sendbuf->custom_items[8].custom_item,
						ptr_sendbuf->custom_items[8].comp_edp_no,
                        ptr_sendbuf->custom_items[9].custom_item,   
						ptr_sendbuf->custom_items[9].comp_edp_no,
                        ptr_sendbuf->custom_items[10].custom_item,
						ptr_sendbuf->custom_items[10].comp_edp_no,
                        ptr_sendbuf->custom_items[11].custom_item,
						ptr_sendbuf->custom_items[11].comp_edp_no,
                        ptr_sendbuf->custom_items[12].custom_item,
						ptr_sendbuf->custom_items[12].comp_edp_no,
                        ptr_sendbuf->custom_items[13].custom_item,
						ptr_sendbuf->custom_items[13].comp_edp_no,
                        ptr_sendbuf->custom_items[14].custom_item,
						ptr_sendbuf->custom_items[14].comp_edp_no,
                        ptr_sendbuf->custom_items[15].custom_item,
						ptr_sendbuf->custom_items[15].comp_edp_no,
                        ptr_sendbuf->custom_items[16].custom_item,
						ptr_sendbuf->custom_items[16].comp_edp_no,
                        ptr_sendbuf->custom_items[17].custom_item,
						ptr_sendbuf->custom_items[17].comp_edp_no,
                        ptr_sendbuf->custom_items[18].custom_item,
						ptr_sendbuf->custom_items[18].comp_edp_no,
                        ptr_sendbuf->custom_items[19].custom_item,
						ptr_sendbuf->custom_items[19].comp_edp_no,
                        ptr_sendbuf->shipto_num,
                        ptr_sendbuf->page_no,
                        ptr_sendbuf->item_line_number,
                        ptr_sendbuf->comp_seq_number,
                        ptr_sendbuf->flag,
                        ptr_sendbuf->item_type,
                        ptr_sendbuf->customframe);

    return(gbp->j);
}

int tt0087_ParceRecvStr(tt0087_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0088.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0088 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0088.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0088 %s\"tt0088\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0087_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0087_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0087_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0087_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0087_REC_ID_LEN );
    gbp->count += tt0087_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0087_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0087_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0087_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0087_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0087_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0087_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0087_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0087_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0087_ERR_LEN;

        gbp->count += tt0087_SEND_FILLER_LEN;


    memset(ptr_recvbuf->item_num,'\0', tt0087_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0087_ITM_NO_LEN);
        ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_num));
    gbp->count += tt0087_ITM_NO_LEN;

    memset(ptr_recvbuf->item_edp,'\0', tt0087_EDP_LEN+1);
    memcpy(ptr_recvbuf->item_edp, sz_recvbufcat + gbp->count, tt0087_EDP_LEN);
        ap_rprintf(r,"		<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item_edp));
    gbp->count += tt0087_EDP_LEN;

    memset(ptr_recvbuf->page_no,'\0', tt0087_PAGE_NO_LEN+1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, tt0087_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NO>%s</PAGE_NO>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += tt0087_PAGE_NO_LEN;

    memset(ptr_recvbuf->item_line_number,'\0', tt0087_ITEM_LINE_NUMBER_LEN+1);
    memcpy(ptr_recvbuf->item_line_number, sz_recvbufcat + gbp->count, tt0087_ITEM_LINE_NUMBER_LEN);
        ap_rprintf(r,"		<ITEM_LINE_NUMBER>%s</ITEM_LINE_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->item_line_number));
    gbp->count += tt0087_ITEM_LINE_NUMBER_LEN;

    memset(ptr_recvbuf->item_type,'\0', tt0087_ITEM_TYPE_LEN+1);
    memcpy(ptr_recvbuf->item_type, sz_recvbufcat + gbp->count, tt0087_ITEM_TYPE_LEN);
        ap_rprintf(r,"		<ITEM_TYPE>%s</ITEM_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->item_edp));
    gbp->count += tt0087_ITEM_TYPE_LEN;



        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0088>\n", gbp->tt_betag);


    return(0);
}

