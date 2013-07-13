/* tt0037.c Check ShipTo */
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


int             tt0037_CatSendStr(struct global_struct *, char *, tt0037_st_send *);
int             tt0037_ParceRecvStr(tt0037_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0037_lt_process(request_rec *, struct global_struct *, char *);


int tt0037_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0037_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0037_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0037_lt_process(request_rec *r, struct global_struct *gbp, char * stdout_buffer)
{
        gbp->sendbuf0037 = (tt0037_st_send *)malloc(sizeof(tt0037_st_send));
        gbp->recvbuf0037 = (tt0037_st_recv *)malloc(sizeof(tt0037_st_recv));

        gbp->sendbufcat = malloc(tt0037_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0037_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0037_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0037_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0037,  '\0',  sizeof(tt0037_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0037->request_id, "XML");
    strcpy(gbp->sendbuf0037->record_id, "0037");

        strcpy(gbp->sendbuf0037->ip_address, r->connection->remote_ip);


    get_tag_data("COMPANY", gbp->sendbuf0037->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0037->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0037->userid,gbp,stdout_buffer);
        

        if(tt0037_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0037) == tt0037_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0037);
                        free (gbp->recvbuf0037);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0037_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0037);
                        free (gbp->recvbuf0037);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0037_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0037);
                        free (gbp->recvbuf0037);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0037);
                        free (gbp->recvbuf0037);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0037_ParceRecvStr(gbp->recvbuf0037, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0037);
                free (gbp->recvbuf0037);
        }
        else
        {
                XML_Error("tt0037_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0037);
                free (gbp->recvbuf0037);
                return(-1);
        }

    return(0);
}


int tt0037_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0037_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler);
            
    return(gbp->j);
}


int tt0037_ParceRecvStr(tt0037_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0038.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0038 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0038.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0038 %s\"tt0038\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0037_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0037_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0037_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0037_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0037_REC_ID_LEN );
    gbp->count += tt0037_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0037_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0037_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0037_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0037_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0037_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0037_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0037_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0037_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0037_ERR_LEN;

        gbp->count += tt0037_SEND_FILLER_LEN;


        
    for(gbp->i = 0; gbp->i < tt0037_SHIPTO_COUNT; gbp->i++) 
        {

    
                memset(ptr_recvbuf->shipto[gbp->i].shipto_num,'\0', tt0037_SHIPTO_NO_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_num, sz_recvbufcat + gbp->count, tt0037_SHIPTO_NO_LEN);
                gbp->count += tt0037_SHIPTO_NO_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_fname,'\0', tt0037_SHIPTO_FN_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_fname, sz_recvbufcat + gbp->count, tt0037_SHIPTO_FN_LEN);
                gbp->count += tt0037_SHIPTO_FN_LEN;
                sprintf(ptr_recvbuf->shipto[gbp->i].shipto_fname,"%s",
                                        rtrim(ptr_recvbuf->shipto[gbp->i].shipto_fname, tt0037_SHIPTO_FN_LEN+1,gbp));


                memset(ptr_recvbuf->shipto[gbp->i].shipto_lname,'\0', tt0037_SHIPTO_LN_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_lname, sz_recvbufcat + gbp->count, tt0037_SHIPTO_LN_LEN);
                gbp->count += tt0037_SHIPTO_LN_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_mi,'\0', tt0037_SHIPTO_MI_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_mi, sz_recvbufcat + gbp->count, tt0037_SHIPTO_MI_LEN);
                gbp->count += tt0037_SHIPTO_MI_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_title,'\0', tt0037_SHIPTO_TITLE_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_title, sz_recvbufcat + gbp->count, tt0037_SHIPTO_TITLE_LEN);
                gbp->count += tt0037_SHIPTO_TITLE_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_co_name,'\0', tt0037_SHIPTO_CO_NAME_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_co_name, sz_recvbufcat + gbp->count, tt0037_SHIPTO_CO_NAME_LEN);
                gbp->count += tt0037_SHIPTO_CO_NAME_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_street,'\0', tt0037_SHIPTO_STREET_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_street, sz_recvbufcat + gbp->count, tt0037_SHIPTO_STREET_LEN);
                gbp->count += tt0037_SHIPTO_STREET_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_city,'\0', tt0037_SHIPTO_CITY_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_city, sz_recvbufcat + gbp->count, tt0037_SHIPTO_CITY_LEN);
                gbp->count += tt0037_SHIPTO_CITY_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_state,'\0', tt0037_SHIPTO_STATE_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_state, sz_recvbufcat + gbp->count, tt0037_SHIPTO_STATE_LEN);
                gbp->count += tt0037_SHIPTO_STATE_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_zip,'\0', tt0037_SHIPTO_ZIP_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_zip, sz_recvbufcat + gbp->count, tt0037_SHIPTO_ZIP_LEN);
                gbp->count += tt0037_SHIPTO_ZIP_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_country,'\0', tt0037_SHIPTO_COUNTRY_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_country, sz_recvbufcat + gbp->count, tt0037_SHIPTO_COUNTRY_LEN);
                gbp->count += tt0037_SHIPTO_COUNTRY_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_ref1,'\0', tt0037_SHIPTO_REF1_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_ref1, sz_recvbufcat + gbp->count, tt0037_SHIPTO_REF1_LEN);
                gbp->count += tt0037_SHIPTO_REF1_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_ref2,'\0', tt0037_SHIPTO_REF2_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_ref2, sz_recvbufcat + gbp->count, tt0037_SHIPTO_REF2_LEN);
                gbp->count += tt0037_SHIPTO_REF2_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_phone_no,'\0', tt0037_SHIPTO_PHONE_NO_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_phone_no, sz_recvbufcat + gbp->count, tt0037_SHIPTO_PHONE_NO_LEN);
                gbp->count += tt0037_SHIPTO_PHONE_NO_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_cust_no,'\0', tt0037_SHIPTO_CUST_NO_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_cust_no, sz_recvbufcat + gbp->count, tt0037_SHIPTO_CUST_NO_LEN);
                gbp->count += tt0037_SHIPTO_CUST_NO_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_edp,'\0', tt0037_SHIPTO_EDP_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_edp, sz_recvbufcat + gbp->count, tt0037_SHIPTO_EDP_LEN);
                gbp->count += tt0037_SHIPTO_EDP_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].jap_flag,'\0', tt0037_JAP_FLAG_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].jap_flag, sz_recvbufcat + gbp->count, tt0037_JAP_FLAG_LEN);
                gbp->count += tt0037_JAP_FLAG_LEN;

                if (ptr_recvbuf->shipto[gbp->i].jap_flag[0] == 'Y')
        {
                memset(ptr_recvbuf->shipto[gbp->i].shipto_jap_ref1,'\0', tt0037_SHIPTO_JAP_REF1_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_jap_ref1, ptr_recvbuf->shipto[gbp->i].shipto_ref1, tt0037_SHIPTO_JAP_REF1_LEN);

                memset(ptr_recvbuf->shipto[gbp->i].shipto_jap_pref,'\0', tt0037_SHIPTO_JAP_PREF_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_jap_pref, ptr_recvbuf->shipto[gbp->i].shipto_ref1+25, 5);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_jap_pref+5, ptr_recvbuf->shipto[gbp->i].shipto_ref2, 15);

                memset(ptr_recvbuf->shipto[gbp->i].shipto_jap_apt,'\0', tt0037_SHIPTO_JAP_APT_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_jap_apt, ptr_recvbuf->shipto[gbp->i].shipto_ref2+15, 15);

                memset(ptr_recvbuf->shipto[gbp->i].shipto_ref1,'\0', tt0037_SHIPTO_REF1_LEN+1);
                memset(ptr_recvbuf->shipto[gbp->i].shipto_ref2,'\0', tt0037_SHIPTO_REF2_LEN+1);

        }

                if((strlen(ptr_recvbuf->shipto[gbp->i].shipto_fname)) > 0)
                {
                        ap_rprintf(r,"		<SHIPTO_DETAIL>\n");
                        ap_rprintf(r,"			<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_num));
                        ap_rprintf(r,"			<SHIPTO_FNAME>%s</SHIPTO_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_fname));
                        ap_rprintf(r,"			<SHIPTO_LNAME>%s</SHIPTO_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_lname));
                        ap_rprintf(r,"			<SHIPTO_MI>%s</SHIPTO_MI>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_mi));
                        ap_rprintf(r,"			<SHIPTO_TITLE>%s</SHIPTO_TITLE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_title));
                        ap_rprintf(r,"			<SHIPTO_CO_NAME>%s</SHIPTO_CO_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_co_name));
                        ap_rprintf(r,"			<SHIPTO_STREET>%s</SHIPTO_STREET>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_street));
                        ap_rprintf(r,"			<SHIPTO_CITY>%s</SHIPTO_CITY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_city));
                        ap_rprintf(r,"			<SHIPTO_STATE>%s</SHIPTO_STATE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_state));
                        ap_rprintf(r,"			<SHIPTO_ZIP>%s</SHIPTO_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_zip));
                        ap_rprintf(r,"			<SHIPTO_COUNTRY>%s</SHIPTO_COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_country));
        if (ptr_recvbuf->shipto[gbp->i].jap_flag[0] == 'Y')
        {
                        ap_rprintf(r,"			<SHIPTO_REF1>%s</SHIPTO_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_ref1));
                        ap_rprintf(r,"			<SHIPTO_REF2>%s</SHIPTO_REF2>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_ref2));
        }else{
                        ap_rprintf(r,"			<SHIPTO_JAP_REF1>%s</SHIPTO_JAP_REF1>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_jap_ref1));
                        ap_rprintf(r,"			<SHIPTO_JAP_PREF>%s</SHIPTO_JAP_PREF>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_jap_pref));
                        ap_rprintf(r,"			<SHIPTO_JAP_APT>%s</SHIPTO_JAP_APT>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_jap_apt));
        }
                        ap_rprintf(r,"			<SHIPTO_PHONE_NO>%s</SHIPTO_PHONE_NO>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_phone_no));
                        ap_rprintf(r,"			<SHIPTO_CUST_NO>%s</SHIPTO_CUST_NO>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_cust_no));
                        ap_rprintf(r,"			<SHIPTO_EDP>%s</SHIPTO_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_edp));
                        ap_rprintf(r,"			<JAP_FLAG>%s</JAP_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].jap_flag));
                ap_rprintf(r,"		</SHIPTO_DETAIL>\n");
                }
        }

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0038>\n", gbp->tt_betag);

    return(0);
}

