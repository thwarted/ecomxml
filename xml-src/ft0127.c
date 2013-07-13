/* ft0127.c  Write order action */
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


int             ft0127_CatSendStr(struct global_struct *, char *, ft0127_st_send *);
int             ft0127_ParceRecvStr(ft0127_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0127_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0127_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0127_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0127_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0127_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0127 = (ft0127_st_send *)malloc(sizeof(ft0127_st_send));
        gbp->ftrecvbuf0127 = (ft0127_st_recv *)malloc(sizeof(ft0127_st_recv));

        gbp->sendbufcat = malloc(ft0127_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0127_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0127_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0127_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0127,  '\0',  sizeof(ft0127_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0127->request_id, "XML");
    strcpy(gbp->ftsendbuf0127->record_id, "0127");
    strcpy(gbp->ftsendbuf0127->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0127->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0127->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0127->userid,gbp,stdout_buffer);
    get_tag_data("FULL_ORDER_NUM", gbp->ftsendbuf0127->full_order_no,gbp,stdout_buffer);
    get_tag_data("MESSAGE_CD", gbp->ftsendbuf0127->message_cd,gbp,stdout_buffer);
    get_tag_data("ACTION_CD", gbp->ftsendbuf0127->action_cd,gbp,stdout_buffer);
    get_tag_data("ENTRY_DATE", gbp->ftsendbuf0127->entry_date,gbp,stdout_buffer);
    get_tag_data("ACTION_DATE", gbp->ftsendbuf0127->action_date,gbp,stdout_buffer);


        if(ft0127_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0127) == ft0127_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0127);
                        free (gbp->ftrecvbuf0127);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0127_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0127);
                        free (gbp->ftrecvbuf0127);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0127_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0127);
                        free (gbp->ftrecvbuf0127);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0127);
                        free (gbp->ftrecvbuf0127);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0127_ParceRecvStr(gbp->ftrecvbuf0127, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0127);
                free (gbp->ftrecvbuf0127);

        }
        else
        {
                XML_Error("ft0127_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0127);
                free (gbp->ftrecvbuf0127);
                return(-1);
        }

    return(0);
}


int ft0127_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0127_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-12.12s%-6.6s%-40.40s%-8.8s%-8.8s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->full_order_no,
            ptr_sendbuf->message_cd,
            ptr_sendbuf->action_cd,
            ptr_sendbuf->entry_date,
            ptr_sendbuf->action_date);


	return(gbp->j);
}


int ft0127_ParceRecvStr(ft0127_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0127.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0127 %s\"ft0127\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0127_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0127_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0127_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0127_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0127_REC_ID_LEN );
    gbp->count += ft0127_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0127_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0127_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0127_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0127_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0127_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0127_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0127_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0127_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0127_ERR_LEN;

    gbp->count += ft0127_SEND_FILLER_LEN;

    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0127>\n", gbp->ft_betag);


    return(0);
}

