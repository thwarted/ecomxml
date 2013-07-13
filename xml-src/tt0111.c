/* tt0111.c Validata Item in Cart */

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


int             tt0111_CatSendStr(struct global_struct *,char *, tt0111_st_send *);
int             tt0111_ParceRecvStr(tt0111_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0111_lt_process(request_rec *, struct global_struct *, char *);


int tt0111_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0111_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

    if((tt0111_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0111_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0111 = (tt0111_st_send *)malloc(sizeof(tt0111_st_send));
        gbp->recvbuf0111 = (tt0111_st_recv *)malloc(sizeof(tt0111_st_recv));

        gbp->sendbufcat = malloc(tt0111_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0111_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0111_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0111_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0111,  '\0',  sizeof(tt0111_st_send));

//    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0111->request_id, "XML");
    strcpy(gbp->sendbuf0111->record_id, "0111");

    get_tag_data("COMPANY", gbp->sendbuf0111->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0111->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0111->userid,gbp,stdout_buffer);

        strcpy(gbp->sendbuf0111->ip_address, r->connection->remote_ip);

        get_tag_data("CALLED_BY", gbp->sendbuf0111->called_by,gbp,stdout_buffer);
        get_tag_data("PAGE_NO", gbp->sendbuf0111->page_no,gbp,stdout_buffer);
        get_tag_data("LINE_NO", gbp->sendbuf0111->line_no,gbp,stdout_buffer);
        
        if(tt0111_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0111) == tt0111_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0111);
                        free (gbp->recvbuf0111);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0111_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0111);
                        free (gbp->recvbuf0111);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0111_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0111);
                        free (gbp->recvbuf0111);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0111);
                        free (gbp->recvbuf0111);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0111_ParceRecvStr(gbp->recvbuf0111, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbufcat);
                free (gbp->recvbuf0111);
        }
        else
        {
                XML_Error("tt0111_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbufcat);
                free (gbp->recvbuf0111);
                return(-1);
        }

    return(0);
}


int tt0111_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0111_st_send *ptr_sendbuf)
{

//    FILE *fp;

    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-8.8s%-2.2s%-3.3s",
                        
                        ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->called_by,
                        ptr_sendbuf->page_no,
                        ptr_sendbuf->line_no
            );

// Begin - RAVI Debug

//   fp = fopen("/tmp/xml_debug.log", "w");
//   fprintf(fp, "request_id = %s\n", ptr_sendbuf->request_id);
//   fprintf(fp, "record_id = %s\n", ptr_sendbuf->record_id);
//   fprintf(fp, "company = %s\n", ptr_sendbuf->company);
//   fprintf(fp, "division = %s\n", ptr_sendbuf->division);
//   fprintf(fp, "userid = %s\n", ptr_sendbuf->userid);
//   fprintf(fp, "ip_address = %s\n", ptr_sendbuf->ip_address);
//   fprintf(fp, "filler = %s\n", ptr_sendbuf->filler);
//   fprintf(fp, "called_by = %s\n", ptr_sendbuf->called_by);
//   fprintf(fp, "page_no = %s\n", ptr_sendbuf->page_no);
//   fprintf(fp, "line_no = %s\n", ptr_sendbuf->line_no);

//   fclose(fp);

// End - RAVI Debug

                        
    return(gbp->j);
}


int tt0111_ParceRecvStr(tt0111_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

// Ravi DEBUG

// ap_rprintf(r,"RECVBUF %s",sz_recvbufcat);

    gbp->count = 0;
        
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0112.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0112 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0112.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0112 %s\"tt0112\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0111_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0111_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0111_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0111_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0111_REC_ID_LEN );
    gbp->count += tt0111_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0111_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0111_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0111_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0111_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0111_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0111_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0111_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0111_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0111_ERR_LEN;

        gbp->count += tt0111_SEND_FILLER_LEN;


        memset(ptr_recvbuf->cust_edp,'\0', tt0111_CUST_EDP_LEN+1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0111_CUST_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0111_CUST_EDP_LEN;

        memset(ptr_recvbuf->source,'\0', tt0111_SOURCE_LEN+1);
    memcpy(ptr_recvbuf->source, sz_recvbufcat + gbp->count, tt0111_SOURCE_LEN);
        ap_rprintf(r,"		<SOURCE>%s</SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->source));
    gbp->count += tt0111_SOURCE_LEN;

        memset(ptr_recvbuf->item_count,'\0', tt0111_ITEM_COUNT_LEN+1);
    memcpy(ptr_recvbuf->item_count, sz_recvbufcat + gbp->count, tt0111_ITEM_COUNT_LEN);
        ap_rprintf(r,"		<ITEM_COUNT>%s</ITEM_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->item_count));
    gbp->count += tt0111_ITEM_COUNT_LEN;

        memset(ptr_recvbuf->check_type,'\0', tt0111_CHECK_TYPE_LEN+1);
    memcpy(ptr_recvbuf->check_type, sz_recvbufcat + gbp->count, tt0111_CHECK_TYPE_LEN);
        ap_rprintf(r,"		<CHECK_TYPE>%s</CHECK_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->check_type));
    gbp->count += tt0111_CHECK_TYPE_LEN;


        for(gbp->i = 0; gbp->i < tt0111_ITEM_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->items[gbp->i].item_edp,'\0', tt0111_ITEM_EDP_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].item_edp, sz_recvbufcat + gbp->count, tt0111_ITEM_EDP_LEN);
                gbp->count += tt0111_ITEM_EDP_LEN;

                memset(ptr_recvbuf->items[gbp->i].item_number,'\0', tt0111_ITEM_NUMBER_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].item_number, sz_recvbufcat + gbp->count, tt0111_ITEM_NUMBER_LEN);
                gbp->count += tt0111_ITEM_NUMBER_LEN;

                memset(ptr_recvbuf->items[gbp->i].item_description,'\0', tt0111_ITEM_DESCRIPTION_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].item_description, sz_recvbufcat + gbp->count, tt0111_ITEM_DESCRIPTION_LEN);
                gbp->count += tt0111_ITEM_DESCRIPTION_LEN;

                memset(ptr_recvbuf->items[gbp->i].item_line_number,'\0', tt0111_ITEM_LINE_NUMBER_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].item_line_number, sz_recvbufcat + gbp->count, tt0111_ITEM_LINE_NUMBER_LEN);
                gbp->count += tt0111_ITEM_LINE_NUMBER_LEN;

                memset(ptr_recvbuf->items[gbp->i].item_quantity,'\0', tt0111_ITEM_QUANTITY_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].item_quantity, sz_recvbufcat + gbp->count, tt0111_ITEM_QUANTITY_LEN);
                gbp->count += tt0111_ITEM_QUANTITY_LEN;

                memset(ptr_recvbuf->items[gbp->i].ship_to_number,'\0', tt0111_SHIP_TO_NUMBER_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].ship_to_number, sz_recvbufcat + gbp->count, tt0111_SHIP_TO_NUMBER_LEN);
                gbp->count += tt0111_SHIP_TO_NUMBER_LEN;

                memset(ptr_recvbuf->items[gbp->i].ship_to_first_name,'\0', tt0111_SHIP_TO_FIRST_NAME_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].ship_to_first_name, sz_recvbufcat + gbp->count, tt0111_SHIP_TO_FIRST_NAME_LEN);
                gbp->count += tt0111_SHIP_TO_FIRST_NAME_LEN;

                memset(ptr_recvbuf->items[gbp->i].ship_to_last_name,'\0', tt0111_SHIP_TO_LAST_NAME_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].ship_to_last_name, sz_recvbufcat + gbp->count, tt0111_SHIP_TO_LAST_NAME_LEN);
                gbp->count += tt0111_SHIP_TO_LAST_NAME_LEN;

                memset(ptr_recvbuf->items[gbp->i].custom_flag,'\0', tt0111_CUSTOM_FLAG_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].custom_flag, sz_recvbufcat + gbp->count, tt0111_CUSTOM_FLAG_LEN);
                gbp->count += tt0111_CUSTOM_FLAG_LEN;

                memset(ptr_recvbuf->items[gbp->i].comp_edp,'\0', tt0111_COMP_EDP_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].comp_edp, sz_recvbufcat + gbp->count, tt0111_COMP_EDP_LEN);
                gbp->count += tt0111_COMP_EDP_LEN;

                memset(ptr_recvbuf->items[gbp->i].comp_number,'\0', tt0111_COMP_NUMBER_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].comp_number, sz_recvbufcat + gbp->count, tt0111_COMP_NUMBER_LEN);
                gbp->count += tt0111_COMP_NUMBER_LEN;

                memset(ptr_recvbuf->items[gbp->i].comp_description,'\0', tt0111_COMP_DESCRIPTION_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].comp_description, sz_recvbufcat + gbp->count, tt0111_COMP_DESCRIPTION_LEN);
                gbp->count += tt0111_COMP_DESCRIPTION_LEN;

                memset(ptr_recvbuf->items[gbp->i].comp_seq_number,'\0', tt0111_COMP_SEQ_NUMBER_LEN+1);
                memcpy(ptr_recvbuf->items[gbp->i].comp_seq_number, sz_recvbufcat + gbp->count, tt0111_COMP_SEQ_NUMBER_LEN);
                gbp->count += tt0111_COMP_SEQ_NUMBER_LEN;
        }

        gbp->k = atoi(ptr_recvbuf->item_count);
        if(gbp->k > (int)tt0111_ITEM_COUNT)
                gbp->k = (int)tt0111_ITEM_COUNT;

        for(gbp->i = 0; gbp->i < gbp->k; gbp->i++)
        {
                ap_rprintf(r,"		<ITEM_DETAILS>\n");
                ap_rprintf(r,"			<ITEM_EDP>%s</ITEM_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].item_edp));
                ap_rprintf(r,"			<ITEM_NUMBER>%s</ITEM_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].item_number));
                ap_rprintf(r,"			<ITEM_DESCRIPTION>%s</ITEM_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].item_description));
                ap_rprintf(r,"			<ITEM_LINE_NUMBER>%s</ITEM_LINE_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].item_line_number));             
                ap_rprintf(r,"			<ITEM_QUANTITY>%s</ITEM_QUANTITY>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].item_quantity));              
                ap_rprintf(r,"			<SHIP_TO_NUMBER>%s</SHIP_TO_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].ship_to_number));           
                ap_rprintf(r,"			<SHIP_TO_FIRST_NAME>%s</SHIP_TO_FIRST_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].ship_to_first_name));               
                ap_rprintf(r,"			<SHIP_TO_LAST_NAME>%s</SHIP_TO_LAST_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].ship_to_last_name));          
                ap_rprintf(r,"			<CUSTOM_FLAG>%s</CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].custom_flag));            
                ap_rprintf(r,"			<COMP_EDP>%s</COMP_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].comp_edp));             
                ap_rprintf(r,"			<COMP_NUMBER>%s</COMP_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].comp_number));            
                ap_rprintf(r,"			<COMP_DESCRIPTION>%s</COMP_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].comp_description));             
                ap_rprintf(r,"			<COMP_SEQ_NUMBER>%s</COMP_SEQ_NUMBER>\n", handle_special_chars(gbp,ptr_recvbuf->items[gbp->i].comp_seq_number));                
                ap_rprintf(r,"		</ITEM_DETAILS>\n");         
        }
                
        
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0112>\n", gbp->tt_betag);

    return(0);
}

