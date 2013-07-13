/* tt0019.c Online CC Authorization */
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


int             tt0019_CatSendStr(struct global_struct *, char *, tt0019_st_send *);
int             tt0019_ParceRecvStr(tt0019_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0019_lt_process(request_rec *, struct global_struct *, char *);


int tt0019_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */

        return(0);

}

int tt0019_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0019_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

/******************************************************************/
/*                                                                */
/*  Process the request                                           */
/*                                                                */
/******************************************************************/


int tt0019_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0019 = (tt0019_st_send *)malloc(sizeof(tt0019_st_send));
        gbp->recvbuf0019 = (tt0019_st_recv *)malloc(sizeof(tt0019_st_recv));

        gbp->sendbufcat = malloc(tt0019_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0019_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0019_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0019_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0019,  '\0',  sizeof(tt0019_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0019->request_id, "XML");
    strcpy(gbp->sendbuf0019->record_id, "0019");

        strcpy(gbp->sendbuf0019->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY", gbp->sendbuf0019->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0019->division,gbp,stdout_buffer);
        get_tag_data("UID",  gbp->sendbuf0019->userid,gbp,stdout_buffer);
        get_tag_data("CUST_EDP",  gbp->sendbuf0019->cust_edp,gbp,stdout_buffer);
        get_tag_data("CC_TYPE",  gbp->sendbuf0019->cc_type,gbp,stdout_buffer);
        get_tag_data("CC_NUM",  gbp->sendbuf0019->cc_num,gbp,stdout_buffer);
        get_tag_data("CC_EXP",  gbp->tagname,gbp,stdout_buffer);

    strncpy(gbp->sendbuf0019->cc_exp_yy, gbp->tagname + 2, 2);
    strncpy(gbp->sendbuf0019->cc_exp_mm, gbp->tagname, 2);

        get_tag_data("TRANSACTION_AMT",  gbp->sendbuf0019->transaction_amt,gbp,stdout_buffer);
        get_tag_data("BL_BIRTH_DATE",    gbp->sendbuf0019->birthday,gbp,stdout_buffer);
        get_tag_data("SV_BAL_INQ",    gbp->sendbuf0019->sv_bal_inq,gbp,stdout_buffer);


    if((tt0019_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0019)) == tt0019_LAN_SEND_BUF_LEN)
        {
        if((gbp->sock = sga_connect( gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
        {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0019);
                        free (gbp->recvbuf0019);
            return(-1);
        }

        if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0019_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0019);
                        free (gbp->recvbuf0019);
            return(-1);
        }

        if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0019_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0019);
                        free (gbp->recvbuf0019);
            return(-1);
        }


        if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
        {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0019);
                        free (gbp->recvbuf0019);
            return(-1);
        }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

        tt0019_ParceRecvStr(gbp->recvbuf0019, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0019);
                free (gbp->recvbuf0019);
        }
        else
        {
                XML_Error("tt0019_CatSend","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0019);
                free (gbp->recvbuf0019);
                return(-1);
        }

    return 0;

}


int tt0019_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0019_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%09d%-2.2s%-20.20s%-2.2s%-2.2s%08.2f%-8.8s%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
            atoi(ptr_sendbuf->cust_edp),
            ptr_sendbuf->cc_type,
            ptr_sendbuf->cc_num,
            ptr_sendbuf->cc_exp_yy,
            ptr_sendbuf->cc_exp_mm,
            atof(ptr_sendbuf->transaction_amt),
			ptr_sendbuf->birthday,
			ptr_sendbuf->sv_bal_inq);

    return(gbp->j);
}


int tt0019_ParceRecvStr(tt0019_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0020.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0020 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0020.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0020 %s\"tt0020\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    // Place the individual variables into target fields

    memset(ptr_recvbuf->request_id, '\0', tt0019_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0019_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0019_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0019_REC_ID_LEN + 1);
        memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0019_REC_ID_LEN );
    gbp->count += tt0019_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0019_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0019_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0019_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0019_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0019_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0019_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0019_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0019_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0019_ERR_LEN;

        gbp->count += tt0019_SEND_FILLER_LEN;

    memset(ptr_recvbuf->auth_code, '\0', tt0019_AUTH_CODE_LEN + 1);
    memcpy(ptr_recvbuf->auth_code, sz_recvbufcat + gbp->count, tt0019_AUTH_CODE_LEN);
        ap_rprintf(r,"		<CC_AUTH_CODE>%s</CC_AUTH_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->auth_code));
    gbp->count += tt0019_AUTH_CODE_LEN;

    memset(ptr_recvbuf->auth_action, '\0', tt0019_AUTH_ACTION_LEN + 1);
    memcpy(ptr_recvbuf->auth_action, sz_recvbufcat + gbp->count, tt0019_AUTH_ACTION_LEN);
        ap_rprintf(r,"		<AUTH_ACTION>%s</AUTH_ACTION>\n", handle_special_chars(gbp,ptr_recvbuf->auth_action));
    gbp->count += tt0019_AUTH_ACTION_LEN;

	memset(ptr_recvbuf->avs_code, '\0', tt0019_AVS_CODE_LEN + 1);
    memcpy(ptr_recvbuf->avs_code, sz_recvbufcat + gbp->count, tt0019_AVS_CODE_LEN);
        ap_rprintf(r,"		<AVS_CODE>%s</AVS_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->avs_code));
    gbp->count += tt0019_AVS_CODE_LEN;


	memset(ptr_recvbuf->cc_resp_code, '\0', tt0019_CC_RESP_CODE_LEN + 1);
    memcpy(ptr_recvbuf->cc_resp_code, sz_recvbufcat + gbp->count, tt0019_CC_RESP_CODE_LEN);
        ap_rprintf(r,"		<CC_RESP_CODE>%s</CC_RESP_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->cc_resp_code));
    gbp->count += tt0019_CC_RESP_CODE_LEN;

	memset(ptr_recvbuf->bml_account, '\0', tt0019_BML_ACCT_LEN + 1);
    memcpy(ptr_recvbuf->bml_account, sz_recvbufcat + gbp->count, tt0019_BML_ACCT_LEN);
        ap_rprintf(r,"		<BML_ACCOUNT>%s</BML_ACCOUNT>\n", handle_special_chars(gbp,ptr_recvbuf->bml_account));
    gbp->count += tt0019_BML_ACCT_LEN;

	memset(ptr_recvbuf->bml_reg_date, '\0', tt0019_BML_DATE_LEN + 1);
    memcpy(ptr_recvbuf->bml_reg_date, sz_recvbufcat + gbp->count, tt0019_BML_DATE_LEN);
        ap_rprintf(r,"		<BML_REG_DATE>%s</BML_REG_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->bml_reg_date));
    gbp->count += tt0019_BML_DATE_LEN;

	memset(ptr_recvbuf->sv_bal_amt, '\0', tt0019_SV_AMT_LEN + 1);
    memcpy(ptr_recvbuf->sv_bal_amt, sz_recvbufcat + gbp->count, tt0019_SV_AMT_LEN);
        ap_rprintf(r,"		<SV_BALANCE>%s</SV_BALANCE>\n", handle_special_chars(gbp,ptr_recvbuf->sv_bal_amt));
    gbp->count += tt0019_SV_AMT_LEN;

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0020>\n", gbp->tt_betag);

    return 0;
}




    




