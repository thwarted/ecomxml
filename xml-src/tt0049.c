/* tt0049.c Variable Kit Selection */
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


int             tt0049_CatSendStr(struct global_struct *, char *, tt0049_st_send *);
int             tt0049_ParceRecvStr(tt0049_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0049_lt_process(request_rec *, struct global_struct *, char *);


int tt0049_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0049_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0049_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0049_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0049 = (tt0049_st_send *)malloc(sizeof(tt0049_st_send));
        gbp->recvbuf0049 = (tt0049_st_recv *)malloc(sizeof(tt0049_st_recv));

        gbp->sendbufcat = malloc(tt0049_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0049_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0049_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0049_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0049,  '\0',  sizeof(tt0049_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0049->request_id, "XML");
    strcpy(gbp->sendbuf0049->record_id, "0049");

        strcpy(gbp->sendbuf0049->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0049->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0049->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0049->userid,gbp,stdout_buffer);
        get_tag_data("KIT_PROD_NUM", gbp->sendbuf0049->kit_item_num,gbp,stdout_buffer);
        

        if(tt0049_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0049) == tt0049_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0049);
                        free (gbp->recvbuf0049);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0049_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0049);
                        free (gbp->recvbuf0049);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0049_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0049);
                        free (gbp->recvbuf0049);
                        return(-1);
                }

        
                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0049);
                        free (gbp->recvbuf0049);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0049_ParceRecvStr(gbp->recvbuf0049, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0049);
                free (gbp->recvbuf0049);
        }
        else
        {
                XML_Error("tt0049_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0049);
                free (gbp->recvbuf0049);
                return(-1);
        }

    return(0);
}


int tt0049_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0049_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-20.20s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->kit_item_num);
            
    return(gbp->j);
}


int tt0049_ParceRecvStr(tt0049_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;
        
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0050.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0050 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0050.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0050 %s\"tt0050\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0049_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0049_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0049_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0049_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0049_REC_ID_LEN );
    gbp->count += tt0049_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0049_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0049_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0049_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0049_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0049_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0049_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0049_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0049_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0049_ERR_LEN;

        gbp->count += tt0049_SEND_FILLER_LEN;


        memset(ptr_recvbuf->kit_item_num,'\0', tt0049_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->kit_item_num, sz_recvbufcat + gbp->count, tt0049_ITM_NO_LEN);
        ap_rprintf(r,"		<KIT_PROD_NUM>%s</KIT_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_num));
    gbp->count += tt0049_ITM_NO_LEN;

        memset(ptr_recvbuf->kit_item_edp,'\0', tt0049_EDP_LEN+1);
    memcpy(ptr_recvbuf->kit_item_edp, sz_recvbufcat + gbp->count, tt0049_EDP_LEN);
        ap_rprintf(r,"		<KIT_PROD_EDP>%s</KIT_PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_edp));
    gbp->count += tt0049_EDP_LEN;

        memset(ptr_recvbuf->kit_item_desc,'\0', tt0049_DESC_LEN+1);
    memcpy(ptr_recvbuf->kit_item_desc, sz_recvbufcat + gbp->count, tt0049_DESC_LEN);
        ap_rprintf(r,"		<KIT_PROD_DESCRIPTION>%s</KIT_PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->kit_item_desc));
    gbp->count += tt0049_DESC_LEN;

        memset(ptr_recvbuf->upsl_X,'\0', tt0049_UPS_LEN+1);
    memcpy(ptr_recvbuf->upsl_X, sz_recvbufcat + gbp->count, tt0049_UPS_LEN);
        ap_rprintf(r,"		<UPSL_X>%s</UPSL_X>\n", handle_special_chars(gbp,ptr_recvbuf->upsl_X));
    gbp->count += tt0049_UPS_LEN;

        memset(ptr_recvbuf->image_ovrd,'\0', tt0049_IMG_OVRD_LEN+1);
    memcpy(ptr_recvbuf->image_ovrd, sz_recvbufcat + gbp->count, tt0049_IMG_OVRD_LEN);
        ap_rprintf(r,"		<IMAGE_OVERRIDE>%s</IMAGE_OVERRIDE>\n", handle_special_chars(gbp,ptr_recvbuf->image_ovrd));
    gbp->count += tt0049_IMG_OVRD_LEN;

        memset(ptr_recvbuf->r1_count,'\0', tt0049_COUNT_LEN+1);
    memcpy(ptr_recvbuf->r1_count, sz_recvbufcat + gbp->count, tt0049_COUNT_LEN);
        ap_rprintf(r,"		<R1_COUNT>%s</R1_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->r1_count));
    gbp->count += tt0049_COUNT_LEN;

        memset(ptr_recvbuf->p1_count,'\0', tt0049_COUNT_LEN+1);
    memcpy(ptr_recvbuf->p1_count, sz_recvbufcat + gbp->count, tt0049_COUNT_LEN);
        ap_rprintf(r,"		<P1_COUNT>%s</P1_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->p1_count));
    gbp->count += tt0049_COUNT_LEN;


        for(gbp->i = 0; gbp->i < tt0049_R1_COMP_COUNT; gbp->i++)
        {

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_num,'\0', tt0049_R1_ITM_NO_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_num, sz_recvbufcat + gbp->count, tt0049_R1_ITM_NO_LEN);
                gbp->count += tt0049_R1_ITM_NO_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_edp,'\0', tt0049_R1_EDP_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_edp, sz_recvbufcat + gbp->count, tt0049_R1_EDP_LEN);
                gbp->count += tt0049_R1_EDP_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_desc,'\0', tt0049_R1_DESC_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_desc, sz_recvbufcat + gbp->count, tt0049_R1_DESC_LEN);
                gbp->count += tt0049_R1_DESC_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_item_qty,'\0', tt0049_R1_QTY_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_item_qty, sz_recvbufcat + gbp->count, tt0049_R1_QTY_LEN);
                gbp->count += tt0049_R1_QTY_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_upsl_X,'\0', tt0049_R1_UPS_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_upsl_X, sz_recvbufcat + gbp->count, tt0049_R1_UPS_LEN);
                gbp->count += tt0049_R1_UPS_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_image_ovrd,'\0', tt0049_R1_IMG_OVRD_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_image_ovrd, sz_recvbufcat + gbp->count, tt0049_R1_IMG_OVRD_LEN);
                gbp->count += tt0049_R1_IMG_OVRD_LEN;

                memset(ptr_recvbuf->r1_component[gbp->i].r1_flag,'\0', tt0049_R1_FLAG_LEN+1);
                memcpy(ptr_recvbuf->r1_component[gbp->i].r1_flag, sz_recvbufcat + gbp->count, tt0049_R1_FLAG_LEN);
                gbp->count += tt0049_R1_FLAG_LEN;
        }

        gbp->k = atoi(ptr_recvbuf->r1_count);
        if(gbp->k > (int)tt0049_R1_COMP_COUNT)
                gbp->k = (int)tt0049_R1_COMP_COUNT;


        for(gbp->i = 0; gbp->i < gbp->k; gbp->i++)
        {
                ap_rprintf(r,"		<R1_COMPONENTS_DETAILS>\n");
                ap_rprintf(r,"			<R1_PROD_NUM>%s</R1_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_num));
                ap_rprintf(r,"			<R1_PROD_EDP>%s</R1_PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_edp));
                ap_rprintf(r,"			<R1_PROD_DESCRIPTION>%s</R1_PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_desc));
                ap_rprintf(r,"			<R1_PROD_QTY>%s</R1_PROD_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_item_qty));
                ap_rprintf(r,"			<R1_UPSL_X>%s</R1_UPSL_X>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_upsl_X));
                ap_rprintf(r,"			<R1_IMAGE_OVERRIDE>%s</R1_IMAGE_OVERRIDE>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_image_ovrd));
                ap_rprintf(r,"			<R1_FLAG>%s</R1_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->r1_component[gbp->i].r1_flag));
                ap_rprintf(r,"		</R1_COMPONENTS_DETAILS>\n");
        }

        for(gbp->i = 0; gbp->i < tt0049_P1_COMP_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->p1_component[gbp->i].p1_item_num,'\0', tt0049_P1_ITM_NO_LEN+1);
                memcpy(ptr_recvbuf->p1_component[gbp->i].p1_item_num, sz_recvbufcat + gbp->count, tt0049_P1_ITM_NO_LEN);
                gbp->count += tt0049_P1_ITM_NO_LEN;

                memset(ptr_recvbuf->p1_component[gbp->i].p1_item_qty,'\0', tt0049_P1_QTY_LEN+1);
                memcpy(ptr_recvbuf->p1_component[gbp->i].p1_item_qty, sz_recvbufcat + gbp->count, tt0049_P1_QTY_LEN);
                gbp->count += tt0049_P1_QTY_LEN;
        }

        gbp->j = atoi(ptr_recvbuf->p1_count);
        if(gbp->j > (int)tt0049_P1_COMP_COUNT)
                gbp->j = (int)tt0049_P1_COMP_COUNT;

        for(gbp->i = 0; gbp->i < gbp->j; gbp->i++)
        {
                ap_rprintf(r,"		<P1_COMPONENTS_DETAILS>\n");
                ap_rprintf(r,"			<P1_PROD_NUM>%s</P1_PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->p1_component[gbp->i].p1_item_num));
                ap_rprintf(r,"			<P1_PROD_QTY>%s</P1_PROD_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->p1_component[gbp->i].p1_item_qty));
                ap_rprintf(r,"		</P1_COMPONENTS_DETAILS>\n");
        }



        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0050>\n", gbp->tt_betag);

    return(0);
}

