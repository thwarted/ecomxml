/* tt0063.c Gift Certificate Validation */
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


int             tt0063_CatSendStr(struct global_struct *, char *, tt0063_st_send *);
int             tt0063_ParceRecvStr(tt0063_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0063_lt_process(request_rec *, struct global_struct *, char *);


int tt0063_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int tt0063_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0063_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int tt0063_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->sendbuf0063 = (tt0063_st_send *)malloc(sizeof(tt0063_st_send));
        gbp->recvbuf0063 = (tt0063_st_recv *)malloc(sizeof(tt0063_st_recv));

        gbp->sendbufcat = malloc(tt0063_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0063_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0063_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0063_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0063,  '\0',  sizeof(tt0063_st_send));


    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0063->request_id, "XML");
    strcpy(gbp->sendbuf0063->record_id, "0063");

        strcpy(gbp->sendbuf0063->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->sendbuf0063->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0063->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0063->userid,gbp,stdout_buffer);

        for(gbp->i = 0; gbp->i < tt0063_GC_COUNT; gbp->i++)
        {
                sprintf(gbp->tagname, "GC_NUM_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0063->gc[gbp->i].gc_num,gbp,stdout_buffer);

                sprintf(gbp->tagname, "GC_CHK_DIG_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0063->gc[gbp->i].gc_chk_dig,gbp,stdout_buffer);

                sprintf(gbp->tagname, "GC_AMT_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0063->gc[gbp->i].gc_amt,gbp,stdout_buffer);

        }

    get_tag_data("GC_FLAG", gbp->sendbuf0063->gc_flag,gbp,stdout_buffer);
        

        if(tt0063_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0063) == tt0063_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0063);
                        free (gbp->recvbuf0063);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock,  gbp->sendbufcat, tt0063_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0063);
                        free (gbp->recvbuf0063);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0063_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0063);
                        free (gbp->recvbuf0063);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0063);
                        free (gbp->recvbuf0063);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0063_ParceRecvStr(gbp->recvbuf0063, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0063);
                free (gbp->recvbuf0063);

        }
        else
        {
                XML_Error("tt01_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0063);
                free (gbp->recvbuf0063);
                return(-1);
        }

    return(0);
}


int tt0063_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0063_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
                        "%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d"
                        "%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d"
                        "%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d"
                        "%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d"
                        "%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d"
                        "%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-8.8s%-1.1s%06d%-2.2s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
                        ptr_sendbuf->filler,
                        ptr_sendbuf->gc[0].gc_num,
                        ptr_sendbuf->gc[0].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[0].gc_amt))*100),
                        ptr_sendbuf->gc[1].gc_num,
                        ptr_sendbuf->gc[1].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[1].gc_amt))*100),
                        ptr_sendbuf->gc[2].gc_num,
                        ptr_sendbuf->gc[2].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[2].gc_amt))*100),
                        ptr_sendbuf->gc[3].gc_num,
                        ptr_sendbuf->gc[3].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[3].gc_amt))*100),
                        ptr_sendbuf->gc[4].gc_num,
                        ptr_sendbuf->gc[4].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[4].gc_amt))*100),
                        ptr_sendbuf->gc[5].gc_num,
                        ptr_sendbuf->gc[5].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[5].gc_amt))*100),
                        ptr_sendbuf->gc[6].gc_num,
                        ptr_sendbuf->gc[6].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[6].gc_amt))*100),
                        ptr_sendbuf->gc[7].gc_num,
                        ptr_sendbuf->gc[7].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[7].gc_amt))*100),
                        ptr_sendbuf->gc[8].gc_num,
                        ptr_sendbuf->gc[8].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[8].gc_amt))*100),
                        ptr_sendbuf->gc[9].gc_num,
                        ptr_sendbuf->gc[9].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[9].gc_amt))*100),
                        ptr_sendbuf->gc[10].gc_num,
                        ptr_sendbuf->gc[10].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[10].gc_amt))*100),
                        ptr_sendbuf->gc[11].gc_num,
                        ptr_sendbuf->gc[11].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[11].gc_amt))*100),
                        ptr_sendbuf->gc[12].gc_num,
                        ptr_sendbuf->gc[12].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[12].gc_amt))*100),
                        ptr_sendbuf->gc[13].gc_num,
                        ptr_sendbuf->gc[13].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[13].gc_amt))*100),
                        ptr_sendbuf->gc[14].gc_num,
                        ptr_sendbuf->gc[14].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[14].gc_amt))*100),
                        ptr_sendbuf->gc[15].gc_num,
                        ptr_sendbuf->gc[15].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[15].gc_amt))*100),
                        ptr_sendbuf->gc[16].gc_num,
                        ptr_sendbuf->gc[16].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[16].gc_amt))*100),
                        ptr_sendbuf->gc[17].gc_num,
                        ptr_sendbuf->gc[17].gc_chk_dig,
                        (int)((atof(ptr_sendbuf->gc[17].gc_amt))*100),
                        ptr_sendbuf->gc_flag);
            
    return(gbp->j);
}


int tt0063_ParceRecvStr(tt0063_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
        gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"http://localhost/xml-dtd/tt0064.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0064 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0064.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0064 %s\"tt0064\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', tt0063_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0063_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0063_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', tt0063_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0063_REC_ID_LEN );
    gbp->count += tt0063_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', tt0063_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0063_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0063_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0063_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0063_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0063_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0063_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0063_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0063_ERR_LEN;

        gbp->count += tt0063_SEND_FILLER_LEN;

        for (gbp->i = 0; gbp->i < tt0063_VALID_GC_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->valid_gc[gbp->i].valid_gc_num,'\0', tt0063_VAL_GC_NO_LEN+1);
            memcpy(ptr_recvbuf->valid_gc[gbp->i].valid_gc_num, sz_recvbufcat + gbp->count, tt0063_VAL_GC_NO_LEN);
                gbp->count += tt0063_VAL_GC_NO_LEN;
                sprintf(ptr_recvbuf->valid_gc[gbp->i].valid_gc_num,"%s",rtrim(ptr_recvbuf->valid_gc[gbp->i].valid_gc_num, tt0063_VAL_GC_NO_LEN+1,gbp));

                memset(ptr_recvbuf->valid_gc[gbp->i].valid_gc_amt,'\0', tt0063_VAL_GC_AMT_LEN+1);
            memcpy(ptr_recvbuf->valid_gc[gbp->i].valid_gc_amt, sz_recvbufcat + gbp->count, tt0063_VAL_GC_AMT_LEN);
                gbp->count += tt0063_VAL_GC_AMT_LEN;

                if((strlen(ptr_recvbuf->valid_gc[gbp->i].valid_gc_num)) > 0) 
                {
                        ap_rprintf(r,"		<VALID_GC_DETAIL>\n");
                        ap_rprintf(r,"			<VALID_GC_NUM>%s</VALID_GC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->valid_gc[gbp->i].valid_gc_num));
                        ap_rprintf(r,"			<VALID_GC_AMT>%s</VALID_GC_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->valid_gc[gbp->i].valid_gc_amt));
                        ap_rprintf(r,"		</VALID_GC_DETAIL>\n");
                }
        }

        for (gbp->i = 0; gbp->i < tt0063_INVALID_GC_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_num,'\0', tt0063_INVAL_GC_NO_LEN+1);
            memcpy(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_num, sz_recvbufcat + gbp->count, tt0063_INVAL_GC_NO_LEN);
                gbp->count += tt0063_INVAL_GC_NO_LEN;
                sprintf(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_num,"%s",rtrim(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_num, tt0063_INVAL_GC_NO_LEN+1,gbp));

                memset(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_amt,'\0', tt0063_INVAL_GC_AMT_LEN+1);
            memcpy(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_amt, sz_recvbufcat + gbp->count, tt0063_INVAL_GC_AMT_LEN);
                gbp->count += tt0063_INVAL_GC_AMT_LEN;

                memset(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_rej,'\0', tt0063_INVAL_GC_REJ_LEN+1);
            memcpy(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_rej, sz_recvbufcat + gbp->count, tt0063_INVAL_GC_REJ_LEN);
                gbp->count += tt0063_INVAL_GC_REJ_LEN;

                if((strlen(ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_num)) > 0) 
                {
                        ap_rprintf(r,"		<INVALID_GC_DETAIL>\n");
                        ap_rprintf(r,"			<INVALID_GC_NUM>%s</INVALID_GC_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_num));
                        ap_rprintf(r,"			<INVALID_GC_AMT>%s</INVALID_GC_AMT>\n", handle_special_chars(gbp,ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_amt));
                        ap_rprintf(r,"			<INVALID_GC_REJECT>%s</INVALID_GC_REJECT>\n", handle_special_chars(gbp,ptr_recvbuf->invalid_gc[gbp->i].invalid_gc_rej));
                        ap_rprintf(r,"		</INVALID_GC_DETAIL>\n");
                }
        }

        memset(ptr_recvbuf->gc_flag,'\0', tt0063_GC_FLAG_LEN+1);
    memcpy(ptr_recvbuf->gc_flag, sz_recvbufcat + gbp->count, tt0063_GC_FLAG_LEN);
        ap_rprintf(r,"		<GC_FLAG>%s</GC_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->gc_flag));
    gbp->count += tt0063_GC_FLAG_LEN;


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0064>\n", gbp->tt_betag);

    return(0);
}

