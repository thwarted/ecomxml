/* wl0008.c   */
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


int             wl0008_CatSendStr(struct global_struct *, char *, wl0008_st_send *);
int             wl0008_ParceRecvStr(wl0008_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             wl0008_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int wl0008_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wl0008_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wl0008_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wl0008_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wlsendbuf0008 = (wl0008_st_send *)malloc(sizeof(wl0008_st_send));
        gbp->wlrecvbuf0008 = (wl0008_st_recv *)malloc(sizeof(wl0008_st_recv));

        gbp->sendbufcat = malloc(wl0008_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(wl0008_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', wl0008_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wl0008_LAN_RECV_BUF_LEN + 1);

    memset(gbp->wlsendbuf0008,  '\0',  sizeof(wl0008_st_send));

    sprintf(gbp->confirmbuf, "     ");

    get_tag_data("TRANSACTION_ID", gbp->wlsendbuf0008->record_id,gbp,stdout_buffer);
    get_tag_data("COMPANY", gbp->wlsendbuf0008->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->wlsendbuf0008->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->wlsendbuf0008->userid,gbp,stdout_buffer);
    get_tag_data("WL_COMPANY", gbp->wlsendbuf0008->wl_company,gbp,stdout_buffer);
    get_tag_data("WL_DIVISION", gbp->wlsendbuf0008->wl_division,gbp,stdout_buffer);
    get_tag_data("WL_ID", gbp->wlsendbuf0008->wl_id,gbp,stdout_buffer);

        if(wl0008_CatSendStr(gbp, gbp->sendbufcat, gbp->wlsendbuf0008) == wl0008_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->wlport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0008);
                        free (gbp->wlrecvbuf0008);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, wl0008_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0008);
                        free (gbp->wlrecvbuf0008);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wl0008_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0008);
                        free (gbp->wlrecvbuf0008);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0008);
                        free (gbp->wlrecvbuf0008);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                wl0008_ParceRecvStr(gbp->wlrecvbuf0008, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0008);
                free (gbp->wlrecvbuf0008);

        }
        else
        {
                XML_Error("wl0008_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0008);
                free (gbp->wlrecvbuf0008);
                return(-1);
        }

    return(0);
}


int wl0008_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, wl0008_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-2.2s%-2.2s%-16.16s%-2.2s%-2.2s%-16.16s%-27.27s",

            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->wl_company,
            ptr_sendbuf->wl_division,
            ptr_sendbuf->wl_id,
            ptr_sendbuf->wl_filler);

    return(gbp->j);
}


int wl0008_ParceRecvStr(wl0008_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wl0008.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, wl_tag);
*/
        ap_rprintf(r,"%s0008 %s\"wl0008\">\n", gbp->wl_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->record_id,'\0', wl0008_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, wl0008_REC_ID_LEN );
    gbp->count += wl0008_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', wl0008_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, wl0008_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += wl0008_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', wl0008_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, wl0008_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += wl0008_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', wl0008_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, wl0008_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += wl0008_ERR_LEN;

    gbp->count += wl0008_RECV_FILLER_LEN;

    memset(ptr_recvbuf->wl_id,'\0', wl0008_WL_ID_LEN + 1);
    memcpy(ptr_recvbuf->wl_id, sz_recvbufcat + gbp->count, wl0008_WL_ID_LEN);
        ap_rprintf(r,"		<WL_ID>%s</WL_ID>\n", handle_special_chars(gbp,ptr_recvbuf->wl_id));
    gbp->count += wl0008_WL_ID_LEN;

    memset(ptr_recvbuf->cust_edp,'\0', wl0008_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, wl0008_CUST_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += wl0008_CUST_EDP_LEN;

    memset(ptr_recvbuf->fname,'\0', wl0008_NAME_LEN + 1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, wl0008_NAME_LEN);
        ap_rprintf(r,"		<FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += wl0008_NAME_LEN;

    memset(ptr_recvbuf->minitial,'\0', wl0008_MI_LEN + 1);
    memcpy(ptr_recvbuf->minitial, sz_recvbufcat + gbp->count, wl0008_MI_LEN);
        ap_rprintf(r,"		<MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->minitial));
    gbp->count += wl0008_MI_LEN;

    memset(ptr_recvbuf->lname,'\0', wl0008_NAME_LEN + 1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, wl0008_NAME_LEN);
        ap_rprintf(r,"		<LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += wl0008_NAME_LEN;

    memset(ptr_recvbuf->city,'\0', wl0008_CITY_LEN + 1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, wl0008_CITY_LEN);
        ap_rprintf(r,"		<CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));
    gbp->count += wl0008_CITY_LEN;

    memset(ptr_recvbuf->state,'\0', wl0008_STATE_LEN + 1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, wl0008_STATE_LEN);
        ap_rprintf(r,"		<STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));
    gbp->count += wl0008_STATE_LEN;

    memset(ptr_recvbuf->street,'\0', wl0008_STREET_LEN + 1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, wl0008_STREET_LEN);
        ap_rprintf(r,"		<STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));
    gbp->count += wl0008_STREET_LEN;

    memset(ptr_recvbuf->zip,'\0', wl0008_ZIP_LEN + 1);
    memcpy(ptr_recvbuf->zip, sz_recvbufcat + gbp->count, wl0008_ZIP_LEN);
        ap_rprintf(r,"		<ZIP>%s</ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->zip));
    gbp->count += wl0008_ZIP_LEN;

    memset(ptr_recvbuf->country_code,'\0', wl0008_CNTRY_CODE_LEN + 1);
    memcpy(ptr_recvbuf->country_code, sz_recvbufcat + gbp->count, wl0008_CNTRY_CODE_LEN);
        ap_rprintf(r,"		<COUNTRY_CODE>%s</COUNTRY_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->country_code));
    gbp->count += wl0008_CNTRY_CODE_LEN;

    memset(ptr_recvbuf->password,'\0', wl0008_PSWD_LEN + 1);
    memcpy(ptr_recvbuf->password, sz_recvbufcat + gbp->count, wl0008_PSWD_LEN);
        ap_rprintf(r,"		<WPASSWORD>%s</WPASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->password));
    gbp->count += wl0008_PSWD_LEN;

    memset(ptr_recvbuf->email,'\0', wl0008_EMAIL_LEN + 1);
    memcpy(ptr_recvbuf->email, sz_recvbufcat + gbp->count, wl0008_EMAIL_LEN);
        ap_rprintf(r,"		<EMAIL>%s</EMAIL>\n", handle_special_chars(gbp,ptr_recvbuf->email));
    gbp->count += wl0008_EMAIL_LEN;



 
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0008>\n", gbp->wl_betag);


    return(0);
}

