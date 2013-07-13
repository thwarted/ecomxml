/* tt0127.c Get Predictive Response Survey Data */
 
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


int             tt0127_CatSendStr(struct global_struct *,char *, tt0127_st_send *);
int             tt0127_ParceRecvStr(tt0127_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0127_lt_process(request_rec *, struct global_struct *, char *);



int tt0127_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */
    

        return(0);
}

int tt0127_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0127_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0127_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0127 = (tt0127_st_send *)malloc(sizeof(tt0127_st_send));
        gbp->recvbuf0127 = (tt0127_st_recv *)malloc(sizeof(tt0127_st_recv));

        gbp->sendbufcat = malloc(tt0127_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0127_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0127_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0127_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0127,  '\0',  sizeof(tt0127_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0127->request_id, "XML");
    strcpy(gbp->sendbuf0127->record_id, "0127");

        strcpy(gbp->sendbuf0127->ip_address, r->connection->remote_ip);
    // Fill the buffers

        get_tag_data("COMPANY", gbp->sendbuf0127->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0127->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0127->userid,gbp,stdout_buffer);
        get_tag_data("CUST_EDP", gbp->sendbuf0127->cust_edp,gbp,stdout_buffer);
    
        if((tt0127_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0127)) == tt0127_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0127);
                        free (gbp->recvbuf0127);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0127_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0127);
                        free (gbp->recvbuf0127);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0127_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0127);
                        free (gbp->recvbuf0127);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0127);
                        free (gbp->recvbuf0127);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0127_ParceRecvStr(gbp->recvbuf0127, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0127);
                free (gbp->recvbuf0127);
        }
    else
    {
                XML_Error("tt0127_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0127);
                free (gbp->recvbuf0127);
                return(-1);
    }

    return(0);
}

int tt0127_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0127_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%9d",
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


int tt0127_ParceRecvStr(tt0127_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0008.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0008 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0008.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0128 %s\"tt0128\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0127_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0127_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0127_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0127_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0127_REC_ID_LEN);
    gbp->count += tt0127_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0127_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0127_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0127_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0127_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0127_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0127_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0127_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0127_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0127_ERR_LEN;

        gbp->count += tt0127_SEND_FILLER_LEN;

    memset(ptr_recvbuf->cust_edp, '\0', tt0127_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, tt0127_CUST_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += tt0127_CUST_EDP_LEN;

    memset(ptr_recvbuf->cust_no, '\0', tt0127_CUST_NO_LEN + 1);
    memcpy(ptr_recvbuf->cust_no, sz_recvbufcat + gbp->count, tt0127_CUST_NO_LEN);
        ap_rprintf(r,"		<CUST_NO>%s</CUST_NO>\n", handle_special_chars(gbp,ptr_recvbuf->cust_no));
    gbp->count += tt0127_CUST_NO_LEN;

    memset(ptr_recvbuf->fname, '\0', tt0127_FNAME_LEN + 1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, tt0127_FNAME_LEN);
        ap_rprintf(r,"		<FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += tt0127_FNAME_LEN;

    memset(ptr_recvbuf->lname, '\0', tt0127_LNAME_LEN + 1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, tt0127_LNAME_LEN);
        ap_rprintf(r,"		<LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += tt0127_LNAME_LEN;

    memset(ptr_recvbuf->comp_name, '\0', tt0127_COMP_NAME_LEN + 1);
    memcpy(ptr_recvbuf->comp_name, sz_recvbufcat + gbp->count, tt0127_COMP_NAME_LEN);
        ap_rprintf(r,"		<COMP_NAME>%s</COMP_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->comp_name));
    gbp->count += tt0127_COMP_NAME_LEN;

    memset(ptr_recvbuf->survey_data, '\0', tt0127_SURVEY_DATA_LEN + 1);
    memcpy(ptr_recvbuf->survey_data, sz_recvbufcat + gbp->count, tt0127_SURVEY_DATA_LEN);
        ap_rprintf(r,"		<SURVEY_DATA>%s</SURVEY_DATA>\n", handle_special_chars(gbp,ptr_recvbuf->survey_data));
    gbp->count += tt0127_SURVEY_DATA_LEN;

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0128>\n", gbp->tt_betag);

    return 0;
}


