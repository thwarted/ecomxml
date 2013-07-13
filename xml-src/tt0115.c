/* tt0115.c Update 3pty in cart */

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


int             tt0115_CatSendStr(struct global_struct *,char *, tt0115_st_send *);
int             tt0115_ParceRecvStr(tt0115_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0115_lt_process(request_rec *, struct global_struct *, char *);



int tt0115_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */
    

        return(0);
}

int tt0115_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0115_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0115_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->sendbuf0115 = (tt0115_st_send *)malloc(sizeof(tt0115_st_send));
        gbp->recvbuf0115 = (tt0115_st_recv *)malloc(sizeof(tt0115_st_recv));

        gbp->sendbufcat = malloc(tt0115_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0115_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0115_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0115_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0115,  '\0',  sizeof(tt0115_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0115->request_id, "XML");
    strcpy(gbp->sendbuf0115->record_id, "0115");

        strcpy(gbp->sendbuf0115->ip_address, r->connection->remote_ip);
    // Fill the buffers

        get_tag_data("COMPANY", gbp->sendbuf0115->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0115->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0115->userid,gbp,stdout_buffer);
        get_tag_data("THIRD_PTY_TRANS_FLAG", gbp->sendbuf0115->trans_flag,gbp,stdout_buffer);
        get_tag_data("THIRD_PTY_FLAG", gbp->sendbuf0115->pty_flag,gbp,stdout_buffer);
        get_tag_data("THIRD_PTY_EDP", gbp->sendbuf0115->pty_edp,gbp,stdout_buffer);

        if((tt0115_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0115)) == tt0115_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0115);
                        free (gbp->recvbuf0115);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0115_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0115);
                        free (gbp->recvbuf0115);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0115_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0115);
                        free (gbp->recvbuf0115);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0115);
                        free (gbp->recvbuf0115);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0115_ParceRecvStr(gbp->recvbuf0115, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0115);
                free (gbp->recvbuf0115);
        }
    else
    {
                XML_Error("tt0115_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0115);
                free (gbp->recvbuf0115);
                return(-1);
    }

    return(0);
}

int tt0115_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0115_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-1.1s%-1.1s%-9.9s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            ptr_sendbuf->trans_flag,
            ptr_sendbuf->pty_flag,
            ptr_sendbuf->pty_edp);

    return(gbp->j);
}


int tt0115_ParceRecvStr(tt0115_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0116.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0116 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0116.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0116 %s\"tt0116\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0115_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0115_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0115_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0115_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0115_REC_ID_LEN);
    gbp->count += tt0115_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0115_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0115_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0115_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0115_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0115_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0115_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0115_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0115_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0115_ERR_LEN;

        gbp->count += tt0115_SEND_FILLER_LEN;

    memset(ptr_recvbuf->trans_flag,'\0', tt0115_TRANS_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->trans_flag, sz_recvbufcat + gbp->count, tt0115_TRANS_FLAG_LEN);
        ap_rprintf(r,"		<THIRD_PTY_TRANS_FLAG>%s</THIRD_PTY_TRANS_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->trans_flag));
    gbp->count += tt0115_TRANS_FLAG_LEN;

    memset(ptr_recvbuf->pty_flag,'\0', tt0115_PTY_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->pty_flag, sz_recvbufcat + gbp->count, tt0115_PTY_FLAG_LEN);
        ap_rprintf(r,"		<THIRD_PTY_FLAG>%s</THIRD_PTY_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->pty_flag));
    gbp->count += tt0115_PTY_FLAG_LEN;

        memset(ptr_recvbuf->py_edp,'\0', tt0115_PY_EDP_LEN+1);
        memcpy(ptr_recvbuf->py_edp, sz_recvbufcat + gbp->count, tt0115_PY_EDP_LEN);
        ap_rprintf(r,"		<THIRD_PTY_EDP>%s</THIRD_PTY_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->py_edp));
        gbp->count += tt0115_PY_EDP_LEN;

        memset(ptr_recvbuf->py_custno,'\0', tt0115_PY_CUSTNO_LEN+1);
        memcpy(ptr_recvbuf->py_custno, sz_recvbufcat + gbp->count, tt0115_PY_CUSTNO_LEN);
        ap_rprintf(r,"		<THIRD_PTY_CUSTNO>%s</THIRD_PTY_CUSTNO>\n", handle_special_chars(gbp,ptr_recvbuf->py_custno));
        gbp->count += tt0115_PY_CUSTNO_LEN;

        memset(ptr_recvbuf->py_fname,'\0', tt0115_PY_FNAME_LEN+1);
        memcpy(ptr_recvbuf->py_fname, sz_recvbufcat + gbp->count, tt0115_PY_FNAME_LEN);
        ap_rprintf(r,"		<THIRD_PTY_FNAME>%s</THIRD_PTY_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->py_fname));
        gbp->count += tt0115_PY_FNAME_LEN;

        memset(ptr_recvbuf->py_mi,'\0', tt0115_PY_MI_LEN+1);
        memcpy(ptr_recvbuf->py_mi, sz_recvbufcat + gbp->count, tt0115_PY_MI_LEN);
        ap_rprintf(r,"		<THIRD_PTY_MI>%s</THIRD_PTY_MI>\n", handle_special_chars(gbp,ptr_recvbuf->py_mi));
        gbp->count += tt0115_PY_MI_LEN;

        memset(ptr_recvbuf->py_lname,'\0', tt0115_PY_LNAME_LEN+1);
        memcpy(ptr_recvbuf->py_lname, sz_recvbufcat + gbp->count, tt0115_PY_LNAME_LEN);
        ap_rprintf(r,"		<THIRD_PTY_LNAME>%s</THIRD_PTY_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->py_lname));
        gbp->count += tt0115_PY_LNAME_LEN;

        memset(ptr_recvbuf->py_company,'\0', tt0115_PY_COMPANY_LEN+1);
        memcpy(ptr_recvbuf->py_company, sz_recvbufcat + gbp->count, tt0115_PY_COMPANY_LEN);
        ap_rprintf(r,"		<THIRD_PTY_COMPANY>%s</THIRD_PTY_COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->py_company));
        gbp->count += tt0115_PY_COMPANY_LEN;

        memset(ptr_recvbuf->py_street,'\0', tt0115_PY_STREET_LEN+1);
        memcpy(ptr_recvbuf->py_street, sz_recvbufcat + gbp->count, tt0115_PY_STREET_LEN);
        ap_rprintf(r,"		<THIRD_PTY_STREET>%s</THIRD_PTY_STREEN>\n", handle_special_chars(gbp,ptr_recvbuf->py_street));
        gbp->count += tt0115_PY_STREET_LEN;

        memset(ptr_recvbuf->py_ref1,'\0', tt0115_PY_REF1_LEN+1);
        memcpy(ptr_recvbuf->py_ref1, sz_recvbufcat + gbp->count, tt0115_PY_REF1_LEN);
        ap_rprintf(r,"		<THIRD_PTY_REF1>%s</THIRD_PTY_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->py_ref1));
        gbp->count += tt0115_PY_REF1_LEN;

        memset(ptr_recvbuf->py_ref2,'\0', tt0115_PY_REF2_LEN+1);
        memcpy(ptr_recvbuf->py_ref2, sz_recvbufcat + gbp->count, tt0115_PY_REF2_LEN);
        ap_rprintf(r,"		<THIRD_PTY_REF2>%s</THIRD_PTY_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->py_ref2));
        gbp->count += tt0115_PY_REF2_LEN;

        memset(ptr_recvbuf->py_city,'\0', tt0115_PY_CITY_LEN+1);
        memcpy(ptr_recvbuf->py_city, sz_recvbufcat + gbp->count, tt0115_PY_CITY_LEN);
        ap_rprintf(r,"		<THIRD_PTY_CITY>%s</THIRD_PTY_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->py_city));
        gbp->count += tt0115_PY_CITY_LEN;
        
        memset(ptr_recvbuf->py_state,'\0', tt0115_PY_STATE_LEN+1);
        memcpy(ptr_recvbuf->py_state, sz_recvbufcat + gbp->count, tt0115_PY_STATE_LEN);
        ap_rprintf(r,"		<THIRD_PTY_STATE>%s</THIRD_PTY_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->py_state));
        gbp->count += tt0115_PY_STATE_LEN;

        memset(ptr_recvbuf->py_zip,'\0', tt0115_PY_ZIP_LEN+1);
        memcpy(ptr_recvbuf->py_zip, sz_recvbufcat + gbp->count, tt0115_PY_ZIP_LEN);
        ap_rprintf(r,"		<THIRD_PTY_ZIP>%s</THIRD_PTY_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->py_zip));
        gbp->count += tt0115_PY_ZIP_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0116>\n", gbp->tt_betag);

    return 0;
}


