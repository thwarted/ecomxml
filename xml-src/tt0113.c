/* tt0113.c Get 3pty Existing */
/*modified for phase 2 - JL */
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


int             tt0113_CatSendStr(struct global_struct *,char *, tt0113_st_send *);
int             tt0113_ParceRecvStr(tt0113_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0113_lt_process(request_rec *, struct global_struct *, char *);



int tt0113_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */
    

        return(0);
}

int tt0113_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0113_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0113_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0113 = (tt0113_st_send *)malloc(sizeof(tt0113_st_send));
        gbp->recvbuf0113 = (tt0113_st_recv *)malloc(sizeof(tt0113_st_recv));

        gbp->sendbufcat = malloc(tt0113_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0113_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0113_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0113_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0113,  '\0',  sizeof(tt0113_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0113->request_id, "XML");
    strcpy(gbp->sendbuf0113->record_id, "0113");

        strcpy(gbp->sendbuf0113->ip_address, r->connection->remote_ip);
    // Fill the buffers

        get_tag_data("COMPANY", gbp->sendbuf0113->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0113->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0113->userid,gbp,stdout_buffer);
        get_tag_data("THIRD_PTY_TRANS_FLAG", gbp->sendbuf0113->trans_flag,gbp,stdout_buffer);
        get_tag_data("BUYER_EDP", gbp->sendbuf0113->buyer_edp,gbp,stdout_buffer);

        if((tt0113_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0113)) == tt0113_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0113);
                        free (gbp->recvbuf0113);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0113_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0113);
                        free (gbp->recvbuf0113);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0113_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0113);
                        free (gbp->recvbuf0113);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0113);
                        free (gbp->recvbuf0113);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0113_ParceRecvStr(gbp->recvbuf0113, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0113);
                free (gbp->recvbuf0113);
        }
    else
    {
                XML_Error("tt0113_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0113);
                free (gbp->recvbuf0113);
                return(-1);
    }

    return(0);
}

int tt0113_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0113_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-9.9s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->trans_flag,
            ptr_sendbuf->buyer_edp);

    return(gbp->j);
}


int tt0113_ParceRecvStr(tt0113_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0114.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0114 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0114.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0114 %s\"tt0114\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0113_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0113_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0113_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0113_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0113_REC_ID_LEN);
    gbp->count += tt0113_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0113_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0113_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0113_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0113_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0113_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0113_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0113_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0113_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0113_ERR_LEN;

        gbp->count += tt0113_SEND_FILLER_LEN;

    memset(ptr_recvbuf->trans_flag,'\0', tt0113_TRANS_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->trans_flag, sz_recvbufcat + gbp->count, tt0113_TRANS_FLAG_LEN);
        ap_rprintf(r,"		<THIRD_PTY_TRANS_FLAG>%s</THIRD_PTY_TRANS_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->trans_flag));
    gbp->count += tt0113_TRANS_FLAG_LEN;

    memset(ptr_recvbuf->buyer_edp,'\0', tt0113_BUYER_EDP_LEN + 1);
    memcpy(ptr_recvbuf->buyer_edp, sz_recvbufcat + gbp->count, tt0113_BUYER_EDP_LEN);
        ap_rprintf(r,"		<BUYER_EDP>%s</BUYER_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_edp));
    gbp->count += tt0113_BUYER_EDP_LEN;

    memset(ptr_recvbuf->buyer_custno,'\0', tt0113_BUYER_CUSTNO_LEN + 1);
    memcpy(ptr_recvbuf->buyer_custno, sz_recvbufcat + gbp->count, tt0113_BUYER_CUSTNO_LEN);
        ap_rprintf(r,"		<BUYER_CUSTNO>%s</BUYER_CUSTNO>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_custno));
    gbp->count += tt0113_BUYER_CUSTNO_LEN;

    memset(ptr_recvbuf->buyer_fname,'\0', tt0113_BUYER_FNAME_LEN + 1);
    memcpy(ptr_recvbuf->buyer_fname, sz_recvbufcat + gbp->count, tt0113_BUYER_FNAME_LEN);
        ap_rprintf(r,"		<BUYER_FNAME>%s</BUYER_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_fname));
    gbp->count += tt0113_BUYER_FNAME_LEN;

    memset(ptr_recvbuf->buyer_mi,'\0', tt0113_BUYER_MI_LEN + 1);
    memcpy(ptr_recvbuf->buyer_mi, sz_recvbufcat + gbp->count, tt0113_BUYER_MI_LEN);
        ap_rprintf(r,"		<BUYER_MI>%s</BUYER_MI>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_mi));
    gbp->count += tt0113_BUYER_MI_LEN;

    memset(ptr_recvbuf->buyer_lname,'\0', tt0113_BUYER_LNAME_LEN + 1);
    memcpy(ptr_recvbuf->buyer_lname, sz_recvbufcat + gbp->count, tt0113_BUYER_LNAME_LEN);
        ap_rprintf(r,"		<BUYER_LNAME>%s</BUYER_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_lname));
    gbp->count += tt0113_BUYER_LNAME_LEN;

    memset(ptr_recvbuf->buyer_company,'\0', tt0113_BUYER_COMPANY_LEN + 1);
    memcpy(ptr_recvbuf->buyer_company, sz_recvbufcat + gbp->count, tt0113_BUYER_COMPANY_LEN);
        ap_rprintf(r,"		<BUYER_COMPANY>%s</BUYER_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_company));
    gbp->count += tt0113_BUYER_COMPANY_LEN;

    memset(ptr_recvbuf->py_flag,'\0', tt0113_PY_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->py_flag, sz_recvbufcat + gbp->count, tt0113_PY_FLAG_LEN);
        ap_rprintf(r,"		<PY_FLAG>%s</PY_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->py_flag));
    gbp->count += tt0113_PY_FLAG_LEN;

        gbp->count += tt0113_RETURN_FILLER_LEN;

        for(gbp->i = 0; gbp->i < tt0113_PY_DATA_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->data[gbp->i].py_edp,'\0', tt0113_PY_EDP_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_edp, sz_recvbufcat + gbp->count, tt0113_PY_EDP_LEN);
                gbp->count += tt0113_PY_EDP_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_custno,'\0', tt0113_PY_CUSTNO_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_custno, sz_recvbufcat + gbp->count, tt0113_PY_CUSTNO_LEN);
                gbp->count += tt0113_PY_CUSTNO_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_fname,'\0', tt0113_PY_FNAME_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_fname, sz_recvbufcat + gbp->count, tt0113_PY_FNAME_LEN);
                gbp->count += tt0113_PY_FNAME_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_mi,'\0', tt0113_PY_MI_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_mi, sz_recvbufcat + gbp->count, tt0113_PY_MI_LEN);
                gbp->count += tt0113_PY_MI_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_lname,'\0', tt0113_PY_LNAME_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_lname, sz_recvbufcat + gbp->count, tt0113_PY_LNAME_LEN);
                gbp->count += tt0113_PY_LNAME_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_company,'\0', tt0113_PY_COMPANY_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_company, sz_recvbufcat + gbp->count, tt0113_PY_COMPANY_LEN);
                gbp->count += tt0113_PY_COMPANY_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_street,'\0', tt0113_PY_STREET_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_street, sz_recvbufcat + gbp->count, tt0113_PY_STREET_LEN);
                gbp->count += tt0113_PY_STREET_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_ref1,'\0', tt0113_PY_REF1_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_ref1, sz_recvbufcat + gbp->count, tt0113_PY_REF1_LEN);
                gbp->count += tt0113_PY_REF1_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_ref2,'\0', tt0113_PY_REF2_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_ref2, sz_recvbufcat + gbp->count, tt0113_PY_REF2_LEN);
                gbp->count += tt0113_PY_REF2_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_city,'\0', tt0113_PY_CITY_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_city, sz_recvbufcat + gbp->count, tt0113_PY_CITY_LEN);
                gbp->count += tt0113_PY_CITY_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_state,'\0', tt0113_PY_STATE_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_state, sz_recvbufcat + gbp->count, tt0113_PY_STATE_LEN);
                gbp->count += tt0113_PY_STATE_LEN;

                memset(ptr_recvbuf->data[gbp->i].py_zip,'\0', tt0113_PY_ZIP_LEN+1);
                memcpy(ptr_recvbuf->data[gbp->i].py_zip, sz_recvbufcat + gbp->count, tt0113_PY_ZIP_LEN);
                gbp->count += tt0113_PY_ZIP_LEN;

        if (atoi(ptr_recvbuf->data[gbp->i].py_edp) > 0){

                ap_rprintf(r,"		<PY_DATA>\n");
                ap_rprintf(r,"			<PY_EDP>%s</PY_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_edp));
                ap_rprintf(r,"			<PY_CUSTNO>%s</PY_CUSTNO>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_custno));
                ap_rprintf(r,"			<PY_FNAME>%s</PY_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_fname));
                ap_rprintf(r,"			<PY_MI>%s</PY_MI>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_mi));
                ap_rprintf(r,"			<PY_LNAME>%s</PY_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_lname));
                ap_rprintf(r,"			<PY_COMPANY>%s</PY_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_company));
                ap_rprintf(r,"			<PY_STREET>%s</PY_STREEN>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_street));
                ap_rprintf(r,"			<PY_REF1>%s</PY_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_ref1));
                ap_rprintf(r,"			<PY_REF2>%s</PY_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_ref2));
                ap_rprintf(r,"			<PY_CITY>%s</PY_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_city));
                ap_rprintf(r,"			<PY_STATE>%s</PY_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_state));
                ap_rprintf(r,"			<PY_ZIP>%s</PY_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->data[gbp->i].py_zip));
                ap_rprintf(r,"		</PY_DATA>\n");

        }

        }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0114>\n", gbp->tt_betag);

    return 0;
}


