/* ft0022.c Update Customer Data */

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


int             ft0022_CatSendStr(struct global_struct *, char *, ft0022_st_send *);
int             ft0022_ParceRecvStr(ft0022_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0022_lt_process(request_rec *, struct global_struct *, char *);



int ft0022_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */


        return(0);
}


int ft0022_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0022_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int ft0022_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

        gbp->ftsendbuf0022 = (ft0022_st_send *)malloc(sizeof(ft0022_st_send));
        gbp->ftrecvbuf0022 = (ft0022_st_recv *)malloc(sizeof(ft0022_st_recv));

        gbp->sendbufcat = malloc(ft0022_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0022_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', ft0022_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0022_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0022,  '\0',  sizeof(ft0022_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0022->request_id, "XML");
    strcpy(gbp->ftsendbuf0022->record_id, "0022");

        strcpy(gbp->ftsendbuf0022->ip_address, r->connection->remote_ip);
        
        get_tag_data("CUST_NO", gbp->ftsendbuf0022->cust_no,gbp,stdout_buffer);
        get_tag_data("FNAME", gbp->ftsendbuf0022->fname,gbp,stdout_buffer);
        get_tag_data("LNAME", gbp->ftsendbuf0022->lname,gbp,stdout_buffer);
        get_tag_data("MI", gbp->ftsendbuf0022->mi,gbp,stdout_buffer);
        get_tag_data("COMPANY_NAME", gbp->ftsendbuf0022->comp_name,gbp,stdout_buffer);
        get_tag_data("REF1", gbp->ftsendbuf0022->ref1,gbp,stdout_buffer);
        get_tag_data("REF2", gbp->ftsendbuf0022->ref2,gbp,stdout_buffer);
        get_tag_data("STREET", gbp->ftsendbuf0022->street,gbp,stdout_buffer);
        get_tag_data("CITY", gbp->ftsendbuf0022->city,gbp,stdout_buffer);
        get_tag_data("STATE", gbp->ftsendbuf0022->state,gbp,stdout_buffer);
        get_tag_data("ZIP", gbp->ftsendbuf0022->zip,gbp,stdout_buffer);
        get_tag_data("COUNTRY_CODE", gbp->ftsendbuf0022->country_code,gbp,stdout_buffer);
        get_tag_data("DAY_PHONE", gbp->ftsendbuf0022->day_phone,gbp,stdout_buffer);
        get_tag_data("NIGHT_PHONE", gbp->ftsendbuf0022->night_phone,gbp,stdout_buffer);
        get_tag_data("FAX_PHONE", gbp->ftsendbuf0022->fax_phone,gbp,stdout_buffer);
        get_tag_data("EMAIL", gbp->ftsendbuf0022->email,gbp,stdout_buffer);
        get_tag_data("TITLE_CODE", gbp->ftsendbuf0022->title_code,gbp,stdout_buffer);
        get_tag_data("TYPE", gbp->ftsendbuf0022->type,gbp,stdout_buffer);
        get_tag_data("STATUS", gbp->ftsendbuf0022->status,gbp,stdout_buffer);
        get_tag_data("ACTION", gbp->ftsendbuf0022->action,gbp,stdout_buffer);
        get_tag_data("XREF_TYPE", gbp->ftsendbuf0022->xref_type,gbp,stdout_buffer);
        get_tag_data("XREF_CUST_NO", gbp->ftsendbuf0022->xref_cust_no,gbp,stdout_buffer);
        get_tag_data("SOURCE", gbp->ftsendbuf0022->source,gbp,stdout_buffer);
        get_tag_data("UPDATE_WEBPASS", gbp->ftsendbuf0022->update_webpass,gbp,stdout_buffer);
        get_tag_data("WEBPASS", gbp->ftsendbuf0022->webpass,gbp,stdout_buffer);
        get_tag_data("NO_PROMO", gbp->ftsendbuf0022->no_promo,gbp,stdout_buffer);
        get_tag_data("NO_RENT", gbp->ftsendbuf0022->no_rent,gbp,stdout_buffer);
        get_tag_data("XREF_DELETE_FLAG", gbp->ftsendbuf0022->xref_delete_flag,gbp,stdout_buffer);
        get_tag_data("GENDER", gbp->ftsendbuf0022->gender,gbp,stdout_buffer);
        get_tag_data("LANGUAGE", gbp->ftsendbuf0022->language,gbp,stdout_buffer);
        get_tag_data("UPS_TYPE", gbp->ftsendbuf0022->ups_type,gbp,stdout_buffer);

    if((ft0022_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0022)) == ft0022_LAN_SEND_BUF_LEN)
    {

                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0022);
                        free (gbp->ftrecvbuf0022);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0022_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0022);
                        free (gbp->ftrecvbuf0022);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0022_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0022);
                        free (gbp->ftrecvbuf0022);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0022);
                        free (gbp->ftrecvbuf0022);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0022_ParceRecvStr(gbp->ftrecvbuf0022, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0022);
                free (gbp->ftrecvbuf0022);
    }
    else
    {
                XML_Error("ft0022_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0022);
                free (gbp->ftrecvbuf0022);
                return(-1);
    }

    return 0;
}







int ft0022_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0022_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-10.10s%-16.16s%-16.16s%-1.1s%-30.30s%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-4.4s%-16.16s%-16.16s%-16.16s%-50.50s%-2.2s%-2.2s%-1.1s%-1.1s%-2.2s%-10.10s%-16.16s%-1.1s%-16.16s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->cust_no,
            ptr_sendbuf->fname,
            ptr_sendbuf->lname,
            ptr_sendbuf->mi,
            ptr_sendbuf->comp_name,
            ptr_sendbuf->ref1,
            ptr_sendbuf->ref2,
            ptr_sendbuf->street,
            ptr_sendbuf->city,
            ptr_sendbuf->state,
            ptr_sendbuf->zip,
            ptr_sendbuf->country_code,
            ptr_sendbuf->day_phone,
            ptr_sendbuf->night_phone,
            ptr_sendbuf->fax_phone,
            ptr_sendbuf->email,
            ptr_sendbuf->title_code,
            ptr_sendbuf->status,
            ptr_sendbuf->action,
            ptr_sendbuf->xref_type,
            ptr_sendbuf->xref_cust_no,
            ptr_sendbuf->source,
            ptr_sendbuf->update_webpass,
            ptr_sendbuf->webpass,
            ptr_sendbuf->no_promo,
            ptr_sendbuf->no_rent,
            ptr_sendbuf->xref_delete_flag,
            ptr_sendbuf->gender,
            ptr_sendbuf->language,
            ptr_sendbuf->ups_type);

    return(gbp->j);
}


int ft0022_ParceRecvStr(ft0022_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0022.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0022 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0022.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0022 %s\"tt0022\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    // Place the individual variables into target fields

    memset(ptr_recvbuf->request_id, '\0', ft0022_REQ_ID_LEN + 1);
        memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0022_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0022_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', ft0022_REC_ID_LEN + 1);
        memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0022_REC_ID_LEN );
    gbp->count += ft0022_REC_ID_LEN;

        memset(ptr_recvbuf->userid, '\0', ft0022_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0022_USER_ID_LEN );
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0022_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0022_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0022_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0022_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0022_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0022_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0022_ERR_LEN;

        gbp->count += ft0022_SEND_FILLER_LEN;

        memset(ptr_recvbuf->cust_edp, '\0', ft0022_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, ft0022_CUST_EDP_LEN );
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += ft0022_CUST_EDP_LEN;

        memset(ptr_recvbuf->cust_no, '\0', ft0022_CUST_NO_LEN + 1);
    memcpy(ptr_recvbuf->cust_no, sz_recvbufcat + gbp->count, ft0022_CUST_NO_LEN );
        ap_rprintf(r,"		<CUST_NO>%s</CUST_NO>\n", handle_special_chars(gbp,ptr_recvbuf->cust_no));
    gbp->count += ft0022_CUST_NO_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0022>\n", gbp->tt_betag);

    return 0;
}

