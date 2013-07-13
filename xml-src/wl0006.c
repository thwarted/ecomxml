/* wl0006.c   */
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


int             wl0006_CatSendStr(struct global_struct *, char *, wl0006_st_send *);
int             wl0006_ParceRecvStr(wl0006_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             wl0006_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int wl0006_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wl0006_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wl0006_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wl0006_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wlsendbuf0006 = (wl0006_st_send *)malloc(sizeof(wl0006_st_send));
        gbp->wlrecvbuf0006 = (wl0006_st_recv *)malloc(sizeof(wl0006_st_recv));

        gbp->sendbufcat = malloc(wl0006_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(wl0006_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', wl0006_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wl0006_LAN_RECV_BUF_LEN + 1);

    memset(gbp->wlsendbuf0006,  '\0',  sizeof(wl0006_st_send));

    sprintf(gbp->confirmbuf, "     ");

    get_tag_data("TRANSACTION_ID", gbp->wlsendbuf0006->record_id,gbp,stdout_buffer);
    get_tag_data("COMPANY", gbp->wlsendbuf0006->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->wlsendbuf0006->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->wlsendbuf0006->userid,gbp,stdout_buffer);
    get_tag_data("WL_COMPANY", gbp->wlsendbuf0006->wl_company,gbp,stdout_buffer);
    get_tag_data("WL_DIVISION", gbp->wlsendbuf0006->wl_division,gbp,stdout_buffer);
    get_tag_data("WL_ID", gbp->wlsendbuf0006->wl_id,gbp,stdout_buffer);
    get_tag_data("WL_PROFILE", gbp->wlsendbuf0006->wl_profile,gbp,stdout_buffer);

    for (gbp->i = 0; gbp->i < wl0006_EMAIL_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "EMAIL_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->wlsendbuf0006->email[gbp->i],gbp,stdout_buffer);

	}

        if(wl0006_CatSendStr(gbp, gbp->sendbufcat, gbp->wlsendbuf0006) == wl0006_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->wlport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0006);
                        free (gbp->wlrecvbuf0006);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, wl0006_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0006);
                        free (gbp->wlrecvbuf0006);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wl0006_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0006);
                        free (gbp->wlrecvbuf0006);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->wlsendbuf0006);
                        free (gbp->wlrecvbuf0006);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                wl0006_ParceRecvStr(gbp->wlrecvbuf0006, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0006);
                free (gbp->wlrecvbuf0006);

        }
        else
        {
                XML_Error("wl0006_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->wlsendbuf0006);
                free (gbp->wlrecvbuf0006);
                return(-1);
        }

    return(0);
}


int wl0006_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, wl0006_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-2.2s%-2.2s%-16.16s%-2.2s%-2.2s%-16.16s%-2.2s%-8.8s"
			"%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s"
			"%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s"
			"%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s"
			"%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s"
			"%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s%-47.47s",

            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->wl_company,
            ptr_sendbuf->wl_division,
            ptr_sendbuf->wl_id,
            ptr_sendbuf->wl_profile,
            ptr_sendbuf->wl_filler,
            ptr_sendbuf->email[0],
            ptr_sendbuf->email[1],
            ptr_sendbuf->email[2],
            ptr_sendbuf->email[3],
            ptr_sendbuf->email[4],
            ptr_sendbuf->email[5],
            ptr_sendbuf->email[6],
            ptr_sendbuf->email[7],
            ptr_sendbuf->email[8],
            ptr_sendbuf->email[9],
            ptr_sendbuf->email[10],
            ptr_sendbuf->email[11],
            ptr_sendbuf->email[12],
            ptr_sendbuf->email[13],
            ptr_sendbuf->email[14],
            ptr_sendbuf->email[15],
            ptr_sendbuf->email[16],
            ptr_sendbuf->email[17],
			ptr_sendbuf->email[18],
            ptr_sendbuf->email[19],
            ptr_sendbuf->email[20],
            ptr_sendbuf->email[21],
            ptr_sendbuf->email[22],
            ptr_sendbuf->email[23],
            ptr_sendbuf->email[24],
            ptr_sendbuf->email[25],
            ptr_sendbuf->email[26],
            ptr_sendbuf->email[27],
            ptr_sendbuf->email[28],
            ptr_sendbuf->email[29],
            ptr_sendbuf->email[30],
            ptr_sendbuf->email[31],
            ptr_sendbuf->email[32],
            ptr_sendbuf->email[33],
            ptr_sendbuf->email[34],
            ptr_sendbuf->email[35],
            ptr_sendbuf->email[36],
            ptr_sendbuf->email[37],
            ptr_sendbuf->email[38],
            ptr_sendbuf->email[39],
            ptr_sendbuf->email[40],
            ptr_sendbuf->email[41],
            ptr_sendbuf->email[42],
            ptr_sendbuf->email[43],
            ptr_sendbuf->email[44],
            ptr_sendbuf->email[45],
            ptr_sendbuf->email[46],
            ptr_sendbuf->email[47],
            ptr_sendbuf->email[48],
            ptr_sendbuf->email[49]);

    return(gbp->j);
}


int wl0006_ParceRecvStr(wl0006_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wl0006.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, wl_tag);
*/
        ap_rprintf(r,"%s0006 %s\"wl0006\">\n", gbp->wl_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->record_id,'\0', wl0006_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, wl0006_REC_ID_LEN );
    gbp->count += wl0006_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', wl0006_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, wl0006_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += wl0006_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', wl0006_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, wl0006_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += wl0006_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', wl0006_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, wl0006_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += wl0006_ERR_LEN;

    gbp->count += wl0006_RECV_FILLER_LEN;

 
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0006>\n", gbp->wl_betag);


    return(0);
}

