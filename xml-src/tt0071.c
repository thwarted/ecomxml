/* tt0071.c Get CC  */
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


int             tt0071_CatSendStr(struct global_struct *, char *, tt0071_st_send *);
int             tt0071_ParceRecvStr(tt0071_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0071_lt_process(request_rec *, struct global_struct *, char *);


int tt0071_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0071_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0071_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0071_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0071 = (tt0071_st_send *)malloc(sizeof(tt0071_st_send));
        gbp->recvbuf0071 = (tt0071_st_recv *)malloc(sizeof(tt0071_st_recv));


        gbp->sendbufcat = malloc(tt0071_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0071_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0071_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0071_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0071,  '\0',  sizeof(tt0071_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0071->request_id, "XML");
    strcpy(gbp->sendbuf0071->record_id, "0071");

        strcpy(gbp->sendbuf0071->ip_address, r->connection->remote_ip);
    get_tag_data("COMPANY", gbp->sendbuf0071->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0071->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0071->userid,gbp,stdout_buffer);
        get_tag_data("CUST_EDP", gbp->sendbuf0071->cust_edp,gbp,stdout_buffer);
        

        if(tt0071_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0071) == tt0071_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0071);
                        free (gbp->recvbuf0071);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0071_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0071);
                        free (gbp->recvbuf0071);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0071_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0071);
                        free (gbp->recvbuf0071);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0071);
                        free (gbp->recvbuf0071);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0071_ParceRecvStr(gbp->recvbuf0071, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0071);
                free (gbp->recvbuf0071);
        }
        else
        {
                XML_Error("tt0071_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0071);
                free (gbp->recvbuf0071);
                return(-1);
        }

    return(0);
}


int tt0071_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0071_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%09d",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        atoi(ptr_sendbuf->cust_edp));
            
    return(gbp->j);
}


int tt0071_ParceRecvStr(tt0071_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"http://localhost/xml-dtd/tt0072.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0072 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0072.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0072 %s\"tt0072\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0071_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0071_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0071_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0071_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0071_REC_ID_LEN );
    gbp->count += tt0071_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0071_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0071_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0071_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0071_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0071_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0071_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0071_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0071_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0071_ERR_LEN;

        gbp->count += tt0071_SEND_FILLER_LEN;


        memset(ptr_recvbuf->cust_edp,'\0', tt0071_EDP_LEN+1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0071_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0071_EDP_LEN;


        
    for(gbp->i = 0; gbp->i < tt0071_CC_COUNT; gbp->i++) 
        {

                memset(ptr_recvbuf->cc[gbp->i].cc_type,'\0', tt0071_CC_TYPE_LEN+1);
                memcpy(ptr_recvbuf->cc[gbp->i].cc_type, sz_recvbufcat + gbp->count, tt0071_CC_TYPE_LEN);
                gbp->count += tt0071_CC_TYPE_LEN;
                sprintf(ptr_recvbuf->cc[gbp->i].cc_type, "%s", 
                                                rtrim(ptr_recvbuf->cc[gbp->i].cc_type, tt0071_CC_TYPE_LEN+1,gbp));

                
                memset(ptr_recvbuf->cc[gbp->i].cc_desc,'\0', tt0071_CC_DESC_LEN+1);
                memcpy(ptr_recvbuf->cc[gbp->i].cc_desc, sz_recvbufcat + gbp->count, tt0071_CC_DESC_LEN);
                gbp->count += tt0071_CC_DESC_LEN;

                memset(ptr_recvbuf->cc[gbp->i].cc_num,'\0', tt0071_CC_NUM_LEN+1);
                memcpy(ptr_recvbuf->cc[gbp->i].cc_num, sz_recvbufcat + gbp->count, tt0071_CC_NUM_LEN);
                gbp->count += tt0071_CC_NUM_LEN;
                sprintf(ptr_recvbuf->cc[gbp->i].cc_num, "%s", 
                                                rtrim(ptr_recvbuf->cc[gbp->i].cc_num, tt0071_CC_NUM_LEN+1,gbp));

                memset(ptr_recvbuf->cc[gbp->i].cc_exp_date,'\0', tt0071_CC_EXP_LEN+1);
                memcpy(ptr_recvbuf->cc[gbp->i].cc_exp_date, sz_recvbufcat + gbp->count, tt0071_CC_EXP_LEN);
                gbp->count += tt0071_CC_EXP_LEN;

                memset(ptr_recvbuf->cc[gbp->i].cc_exp_flag,'\0', tt0071_CC_FLAG_LEN+1);
                memcpy(ptr_recvbuf->cc[gbp->i].cc_exp_flag, sz_recvbufcat + gbp->count, tt0071_CC_FLAG_LEN);
                gbp->count += tt0071_CC_FLAG_LEN;
                
                if(((strlen(ptr_recvbuf->cc[gbp->i].cc_num)) > 0) ||
                  ((strlen(ptr_recvbuf->cc[gbp->i].cc_type)) > 0))      
                {
                                ap_rprintf(r,"		<CC_DETAIL>\n");
                                ap_rprintf(r,"			<CC_TYPE>%s</CC_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->cc[gbp->i].cc_type));
                                ap_rprintf(r,"			<CC_DESCRIPTION>%s</CC_DESCRIPTION>\n", handle_special_chars(gbp,ptr_recvbuf->cc[gbp->i].cc_desc));
                                ap_rprintf(r,"			<CC_NUM>%s</CC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->cc[gbp->i].cc_num));
                                ap_rprintf(r,"			<CC_EXP_DATE>%s</CC_EXP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->cc[gbp->i].cc_exp_date));
                                ap_rprintf(r,"			<CC_EXP_FLAG>%s</CC_EXP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->cc[gbp->i].cc_exp_flag));
                        ap_rprintf(r,"		</CC_DETAIL>\n");
                }
        }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0072>\n", gbp->tt_betag);

    return(0);
}

