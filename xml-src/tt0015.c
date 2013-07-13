/* tt0015.c  Customization Parameter */
/* modified for phase 2 - JL */
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


int             tt0015_CatSendStr(struct global_struct *, char *, tt0015_st_send *);
int             tt0015_ParceRecvStr(tt0015_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0015_lt_process(request_rec *, struct global_struct *, char *);


int tt0015_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);
}


int tt0015_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0015_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

/******************************************************************/
/*                                                                */
/*  Process the request                                           */
/*                                                                */
/******************************************************************/


int tt0015_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0015 = (tt0015_st_send *)malloc(sizeof(tt0015_st_send));
        gbp->recvbuf0015 = (tt0015_st_recv *)malloc(sizeof(tt0015_st_recv));

        gbp->sendbufcat = malloc(tt0015_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0015_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0015_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0015_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0015,  '\0',  sizeof(tt0015_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0015->request_id, "XML");
    strcpy(gbp->sendbuf0015->record_id, "0015");

        strcpy(gbp->sendbuf0015->ip_address, r->connection->remote_ip);
        get_tag_data("COMPANY", gbp->sendbuf0015->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0015->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0015->userid,gbp,stdout_buffer);
        get_tag_data("PROD_NUM", gbp->sendbuf0015->item_num,gbp,stdout_buffer);
        get_tag_data("SHIP_TO_EDP", gbp->sendbuf0015->ship_to_edp,gbp,stdout_buffer);


    if((tt0015_CatSendStr(gbp, gbp->sendbufcat,gbp->sendbuf0015)) == tt0015_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport,gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0015);
                        free (gbp->recvbuf0015);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0015_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0015);
                        free (gbp->recvbuf0015);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0015_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0015);
                        free (gbp->recvbuf0015);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0015);
                        free (gbp->recvbuf0015);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0015_ParceRecvStr(gbp->recvbuf0015, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0015);
                free (gbp->recvbuf0015);
        }
        else
        {
                XML_Error("tt0015_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0015);
                free (gbp->recvbuf0015);
                return(-1);
        }

    return(0);
}


int tt0015_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0015_st_send *ptr_sendbuf)
{
        gbp->j = sprintf(sz_sendbufcat, "%-4s%-4s%-2s%-2s%-16.16s%-16s%-25.25s%-20.20s%-9.9s",
                ptr_sendbuf->request_id,
                ptr_sendbuf->record_id,
                ptr_sendbuf->company,
                ptr_sendbuf->division,
                ptr_sendbuf->userid,
                ptr_sendbuf->ip_address,
                ptr_sendbuf->filler,
                ptr_sendbuf->item_num,
                ptr_sendbuf->ship_to_edp);

        return(gbp->j);
}


/******************************************************************/
/*                                                                */
/*   Parse the answer                                             */
/*                                                                */
/******************************************************************/

int tt0015_ParceRecvStr(tt0015_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    // Place the individual variables into target fields
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0016.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0016 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0016.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0016 %s\"tt0016\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0015_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0015_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0015_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0015_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0015_REC_ID_LEN );
    gbp->count += tt0015_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0015_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0015_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0015_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0015_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0015_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0015_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0015_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0015_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0015_ERR_LEN;

        gbp->count += tt0015_SEND_FILLER_LEN;


    memset(ptr_recvbuf->item_edp, '\0', tt0015_EDP_LEN + 1);
    memcpy(ptr_recvbuf->item_edp, sz_recvbufcat + gbp->count, tt0015_EDP_LEN );
        ap_rprintf(r,"		<PROD_EDP>%s</PROD_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->item_edp));
    gbp->count += tt0015_EDP_LEN;

    memset(ptr_recvbuf->item_num, '\0', tt0015_ITM_NO_LEN + 1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0015_ITM_NO_LEN );
        ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_num));
    gbp->count += tt0015_ITM_NO_LEN;

    memset(ptr_recvbuf->item_desc, '\0', tt0015_DESC_LEN + 1);
    memcpy(ptr_recvbuf->item_desc, sz_recvbufcat + gbp->count, tt0015_DESC_LEN);
        ap_rprintf(r,"		<PROD_DESCRIPTION>%s</PROD_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->item_desc));
    gbp->count += tt0015_DESC_LEN;

    memset(ptr_recvbuf->item_customization_flag, '\0', tt0015_CUSTOM_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->item_customization_flag, sz_recvbufcat + gbp->count, tt0015_CUSTOM_FLAG_LEN);
        ap_rprintf(r,"		<CUSTOMIZATION_FLAG>%s</CUSTOMIZATION_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->item_customization_flag));
    gbp->count += tt0015_CUSTOM_FLAG_LEN;

    // Build data from array

    for( gbp->i = 0; gbp->i < tt0015_CUSTOMIZE_COUNT; gbp->i++) 
        {
        memset(ptr_recvbuf->customization_prompt[gbp->i].prompt, '\0', tt0015_PROMPT_LEN + 1);
        memcpy(ptr_recvbuf->customization_prompt[gbp->i].prompt, sz_recvbufcat + gbp->count, tt0015_PROMPT_LEN);
        gbp->count += tt0015_PROMPT_LEN;
                sprintf(ptr_recvbuf->customization_prompt[gbp->i].prompt,"%s",
                                                        rtrim(ptr_recvbuf->customization_prompt[gbp->i].prompt, tt0015_PROMPT_LEN+1,gbp));

        memset(ptr_recvbuf->customization_prompt[gbp->i].resp_len, '\0', tt0015_RESP_LEN + 1);
        memcpy(ptr_recvbuf->customization_prompt[gbp->i].resp_len, sz_recvbufcat+gbp->count, tt0015_RESP_LEN);
        gbp->count += tt0015_RESP_LEN;

                if((strlen(ptr_recvbuf->customization_prompt[gbp->i].prompt)) > 0) 
                {
                        ap_rprintf(r,"		<CUSTOMIZATION_PROMPTS>\n");
                        ap_rprintf(r,"			<CUSTOMIZATION_TEXT>%s</CUSTOMIZATION_TEXT>\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->customization_prompt[gbp->i].prompt));
                        ap_rprintf(r,"			<RESPONSE_LENGTH>%s</RESPONSE_LENGTH>\n", 
                                                                                                handle_special_chars(gbp,ptr_recvbuf->customization_prompt[gbp->i].resp_len));
                        ap_rprintf(r,"		</CUSTOMIZATION_PROMPTS>\n");
                }

                   
    }
    memset(ptr_recvbuf->ship_to_edp, '\0', tt0015_SHIP_TO_EDP_LEN + 1);
    memcpy(ptr_recvbuf->ship_to_edp, sz_recvbufcat + gbp->count, tt0015_SHIP_TO_EDP_LEN);
        ap_rprintf(r,"		<SHIP_TO_EDP>%s</SHIP_TO_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->ship_to_edp));
    gbp->count += tt0015_SHIP_TO_EDP_LEN;

    memset(ptr_recvbuf->customframe, '\0', tt0015_CUSTOMFRAME_LEN + 1);
    memcpy(ptr_recvbuf->customframe, sz_recvbufcat + gbp->count, tt0015_CUSTOMFRAME_LEN);
        ap_rprintf(r,"		<D1_CUSTOM_FRAME>%s</D1_CUSTOM_FRAME>\n", handle_special_chars(gbp,ptr_recvbuf->customframe));
    gbp->count += tt0015_CUSTOMFRAME_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0016>\n", gbp->tt_betag);

    return 0;
}


