/* tt0017.c  Style Validation */
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


int             tt0017_CatSendStr(struct global_struct *, char *, tt0017_st_send *);
int             tt0017_ParceRecvStr(tt0017_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0017_lt_process(request_rec *, struct global_struct *, char *);


int tt0017_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);

}

int tt0017_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0017_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0017_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0017 = (tt0017_st_send *)malloc(sizeof(tt0017_st_send));
        gbp->recvbuf0017 = (tt0017_st_recv *)malloc(sizeof(tt0017_st_recv));

        gbp->sendbufcat = malloc(tt0017_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0017_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0017_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0017_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0017,  '\0',  sizeof(tt0017_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0017->request_id, "XML");
    strcpy(gbp->sendbuf0017->record_id, "0017");

        strcpy(gbp->sendbuf0017->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0017->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0017->division,gbp,stdout_buffer);
        get_tag_data("UID",  gbp->sendbuf0017->userid,gbp,stdout_buffer);
        get_tag_data("STYLE_ID", gbp->sendbuf0017->style_id,gbp,stdout_buffer);
        get_tag_data("L3_TYPE", gbp->sendbuf0017->l3_type,gbp,stdout_buffer);
        get_tag_data("L2_TYPE", gbp->sendbuf0017->l2_type,gbp,stdout_buffer);
        get_tag_data("L1_TYPE", gbp->sendbuf0017->l1_type,gbp,stdout_buffer);
        get_tag_data("L3_NAME", gbp->sendbuf0017->l3_desc,gbp,stdout_buffer);
        get_tag_data("L2_NAME", gbp->sendbuf0017->l2_desc,gbp,stdout_buffer);
        get_tag_data("L1_NAME", gbp->sendbuf0017->l1_desc,gbp,stdout_buffer);
        
        
        if((tt0017_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0017)) == tt0017_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0017);
                        free (gbp->recvbuf0017);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0017_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0017);
                        free (gbp->recvbuf0017);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0017_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0017);
                        free (gbp->recvbuf0017);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0017);
                        free (gbp->recvbuf0017);
                        return(-1);
                }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0017_ParceRecvStr(gbp->recvbuf0017, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0017);
                free (gbp->recvbuf0017);
        }
        else
        {
                XML_Error("tt0017_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0017);
                free (gbp->recvbuf0017);
                return(-1);
        }

    return(0);
}


int tt0017_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0017_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-12.12s%-2.2s%-2.2s%-2.2s%-20.20s%-20.20s%-20.20s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->style_id,
            ptr_sendbuf->l3_type,
            ptr_sendbuf->l2_type,
            ptr_sendbuf->l1_type,
            ptr_sendbuf->l3_desc,
            ptr_sendbuf->l2_desc,
            ptr_sendbuf->l1_desc);

    return(gbp->j);
}


int tt0017_ParceRecvStr(tt0017_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0018.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0018 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0018.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0018 %s\"tt0018\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id, '\0', tt0017_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0017_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0017_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0017_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0017_REC_ID_LEN );
    gbp->count += tt0017_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0017_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0017_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0017_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0017_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0017_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0017_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0017_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0017_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0017_ERR_LEN;

        gbp->count += tt0017_SEND_FILLER_LEN;


    memset(ptr_recvbuf->item_num, '\0', tt0017_ITM_NO_LEN+1);
    memcpy(ptr_recvbuf->item_num, sz_recvbufcat + gbp->count, tt0017_ITM_NO_LEN);
        ap_rprintf(r,"		<PROD_NUM>%s</PROD_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->item_num));
    gbp->count += tt0017_ITM_NO_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0018>\n", gbp->tt_betag);

    return(0);
}





