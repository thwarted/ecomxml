/* ft0074.c  Edit wishlist*/
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


int             ft0074_CatSendStr(struct global_struct *, char *, ft0074_st_send *);
int             ft0074_ParceRecvStr(ft0074_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0074_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0074_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0074_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0074_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0074_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0074 = (ft0074_st_send *)malloc(sizeof(ft0074_st_send));
        gbp->ftrecvbuf0074 = (ft0074_st_recv *)malloc(sizeof(ft0074_st_recv));

        gbp->sendbufcat = malloc(ft0074_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0074_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0074_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0074_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0074,  '\0',  sizeof(ft0074_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0074->request_id, "XML");
    strcpy(gbp->ftsendbuf0074->record_id, "0074");
    strcpy(gbp->ftsendbuf0074->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0074->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0074->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0074->userid,gbp,stdout_buffer);
    get_tag_data("WL_COMPANY", gbp->ftsendbuf0074->wl_company,gbp,stdout_buffer);
    get_tag_data("WL_DIVISION", gbp->ftsendbuf0074->wl_division,gbp,stdout_buffer);
    get_tag_data("WL_ID", gbp->ftsendbuf0074->wl_id,gbp,stdout_buffer);


    for (gbp->i = 0; gbp->i < ft0074_EW_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "EW_ITEM_EDP_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0074->ew_item_edp[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "EW_ITEM_NO_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0074->ew_item_no[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "EW_LINE_NUM_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0074->ew_line_no[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "EW_QTY_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0074->ew_qty[gbp->i],gbp,stdout_buffer);

	}

    get_tag_data("EW_DELETE_LIST", gbp->ftsendbuf0074->delete_list,gbp,stdout_buffer);


        if(ft0074_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0074) == ft0074_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0074);
                        free (gbp->ftrecvbuf0074);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0074_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0074);
                        free (gbp->ftrecvbuf0074);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0074_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0074);
                        free (gbp->ftrecvbuf0074);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0074);
                        free (gbp->ftrecvbuf0074);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0074_ParceRecvStr(gbp->ftrecvbuf0074, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0074);
                free (gbp->ftrecvbuf0074);

        }
        else
        {
                XML_Error("ft0074_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0074);
                free (gbp->ftrecvbuf0074);
                return(-1);
        }

    return(0);
}


int ft0074_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0074_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-2.2s%-2.2s%-16.16s"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d%-9.9s%-20.20s%04d%04d"
			"%-1.1s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->wl_company,
            ptr_sendbuf->wl_division,
            ptr_sendbuf->wl_id,
            ptr_sendbuf->ew_item_edp[0],
            ptr_sendbuf->ew_item_no[0],
            ptr_sendbuf->ew_line_no[0],
            ptr_sendbuf->ew_qty[0],
            ptr_sendbuf->ew_item_edp[1],
            ptr_sendbuf->ew_item_no[1],
            ptr_sendbuf->ew_line_no[1],
            ptr_sendbuf->ew_qty[1],
            ptr_sendbuf->ew_item_edp[2],
            ptr_sendbuf->ew_item_no[2],
            ptr_sendbuf->ew_line_no[2],
            ptr_sendbuf->ew_qty[2],
            ptr_sendbuf->ew_item_edp[3],
            ptr_sendbuf->ew_item_no[3],
            ptr_sendbuf->ew_line_no[3],
            ptr_sendbuf->ew_qty[3],
            ptr_sendbuf->ew_item_edp[4],
            ptr_sendbuf->ew_item_no[4],
            ptr_sendbuf->ew_line_no[4],
            ptr_sendbuf->ew_qty[4],
            ptr_sendbuf->ew_item_edp[5],
            ptr_sendbuf->ew_item_no[5],
            ptr_sendbuf->ew_line_no[5],
            ptr_sendbuf->ew_qty[5],
            ptr_sendbuf->ew_item_edp[6],
            ptr_sendbuf->ew_item_no[6],
            ptr_sendbuf->ew_line_no[6],
            ptr_sendbuf->ew_qty[6],
            ptr_sendbuf->ew_item_edp[7],
            ptr_sendbuf->ew_item_no[7],
            ptr_sendbuf->ew_line_no[7],
            ptr_sendbuf->ew_qty[7],
            ptr_sendbuf->ew_item_edp[8],
            ptr_sendbuf->ew_item_no[8],
            ptr_sendbuf->ew_line_no[8],
            ptr_sendbuf->ew_qty[8],
            ptr_sendbuf->ew_item_edp[9],
            ptr_sendbuf->ew_item_no[9],
            ptr_sendbuf->ew_line_no[9],
            ptr_sendbuf->ew_qty[9],
            ptr_sendbuf->ew_item_edp[10],
            ptr_sendbuf->ew_item_no[10],
            ptr_sendbuf->ew_line_no[10],
            ptr_sendbuf->ew_qty[10],
            ptr_sendbuf->ew_item_edp[11],
            ptr_sendbuf->ew_item_no[11],
            ptr_sendbuf->ew_line_no[11],
            ptr_sendbuf->ew_qty[11],
            ptr_sendbuf->ew_item_edp[12],
            ptr_sendbuf->ew_item_no[12],
            ptr_sendbuf->ew_line_no[12],
            ptr_sendbuf->ew_qty[12],
            ptr_sendbuf->ew_item_edp[13],
            ptr_sendbuf->ew_item_no[13],
            ptr_sendbuf->ew_line_no[13],
            ptr_sendbuf->ew_qty[13],
            ptr_sendbuf->ew_item_edp[14],
            ptr_sendbuf->ew_item_no[14],
            ptr_sendbuf->ew_line_no[14],
            ptr_sendbuf->ew_qty[14],
            ptr_sendbuf->ew_item_edp[15],
            ptr_sendbuf->ew_item_no[15],
            ptr_sendbuf->ew_line_no[15],
            ptr_sendbuf->ew_qty[15],
            ptr_sendbuf->ew_item_edp[16],
            ptr_sendbuf->ew_item_no[16],
            ptr_sendbuf->ew_line_no[16],
            ptr_sendbuf->ew_qty[16],
            ptr_sendbuf->ew_item_edp[17],
            ptr_sendbuf->ew_item_no[17],
            ptr_sendbuf->ew_line_no[17],
            ptr_sendbuf->ew_qty[17],
            ptr_sendbuf->ew_item_edp[18],
            ptr_sendbuf->ew_item_no[18],
            ptr_sendbuf->ew_line_no[18],
            ptr_sendbuf->ew_qty[18],
            ptr_sendbuf->ew_item_edp[19],
            ptr_sendbuf->ew_item_no[19],
            ptr_sendbuf->ew_line_no[19],
            ptr_sendbuf->ew_qty[19],
            ptr_sendbuf->ew_item_edp[20],
            ptr_sendbuf->ew_item_no[20],
            ptr_sendbuf->ew_line_no[20],
            ptr_sendbuf->ew_qty[20],
            ptr_sendbuf->ew_item_edp[21],
            ptr_sendbuf->ew_item_no[21],
            ptr_sendbuf->ew_line_no[21],
            ptr_sendbuf->ew_qty[21],
            ptr_sendbuf->ew_item_edp[22],
            ptr_sendbuf->ew_item_no[22],
            ptr_sendbuf->ew_line_no[22],
            ptr_sendbuf->ew_qty[22],
            ptr_sendbuf->ew_item_edp[23],
            ptr_sendbuf->ew_item_no[23],
            ptr_sendbuf->ew_line_no[23],
            ptr_sendbuf->ew_qty[23],
            ptr_sendbuf->ew_item_edp[24],
            ptr_sendbuf->ew_item_no[24],
            ptr_sendbuf->ew_line_no[24],
            ptr_sendbuf->ew_qty[24],
            ptr_sendbuf->ew_item_edp[25],
            ptr_sendbuf->ew_item_no[25],
            ptr_sendbuf->ew_line_no[25],
            ptr_sendbuf->ew_qty[25],
            ptr_sendbuf->ew_item_edp[26],
            ptr_sendbuf->ew_item_no[26],
            ptr_sendbuf->ew_line_no[26],
            ptr_sendbuf->ew_qty[26],
            ptr_sendbuf->ew_item_edp[27],
            ptr_sendbuf->ew_item_no[27],
            ptr_sendbuf->ew_line_no[27],
            ptr_sendbuf->ew_qty[27],
            ptr_sendbuf->ew_item_edp[28],
            ptr_sendbuf->ew_item_no[28],
            ptr_sendbuf->ew_line_no[28],
            ptr_sendbuf->ew_qty[28],
            ptr_sendbuf->ew_item_edp[29],
            ptr_sendbuf->ew_item_no[29],
            ptr_sendbuf->ew_line_no[29],
            ptr_sendbuf->ew_qty[29],
            ptr_sendbuf->ew_item_edp[30],
            ptr_sendbuf->ew_item_no[30],
            ptr_sendbuf->ew_line_no[30],
            ptr_sendbuf->ew_qty[30],
            ptr_sendbuf->ew_item_edp[31],
            ptr_sendbuf->ew_item_no[31],
            ptr_sendbuf->ew_line_no[31],
            ptr_sendbuf->ew_qty[31],
            ptr_sendbuf->ew_item_edp[32],
            ptr_sendbuf->ew_item_no[32],
            ptr_sendbuf->ew_line_no[32],
            ptr_sendbuf->ew_qty[32],
            ptr_sendbuf->ew_item_edp[33],
            ptr_sendbuf->ew_item_no[33],
            ptr_sendbuf->ew_line_no[33],
            ptr_sendbuf->ew_qty[33],
            ptr_sendbuf->ew_item_edp[34],
            ptr_sendbuf->ew_item_no[34],
            ptr_sendbuf->ew_line_no[34],
            ptr_sendbuf->ew_qty[34],
            ptr_sendbuf->ew_item_edp[35],
            ptr_sendbuf->ew_item_no[35],
            ptr_sendbuf->ew_line_no[35],
            ptr_sendbuf->ew_qty[35],
            ptr_sendbuf->ew_item_edp[36],
            ptr_sendbuf->ew_item_no[36],
            ptr_sendbuf->ew_line_no[36],
            ptr_sendbuf->ew_qty[36],
            ptr_sendbuf->ew_item_edp[37],
            ptr_sendbuf->ew_item_no[37],
            ptr_sendbuf->ew_line_no[37],
            ptr_sendbuf->ew_qty[37],
            ptr_sendbuf->ew_item_edp[38],
            ptr_sendbuf->ew_item_no[38],
            ptr_sendbuf->ew_line_no[38],
            ptr_sendbuf->ew_qty[38],
            ptr_sendbuf->ew_item_edp[39],
            ptr_sendbuf->ew_item_no[39],
            ptr_sendbuf->ew_line_no[39],
            ptr_sendbuf->ew_qty[39],
            ptr_sendbuf->ew_item_edp[40],
            ptr_sendbuf->ew_item_no[40],
            ptr_sendbuf->ew_line_no[40],
            ptr_sendbuf->ew_qty[40],
            ptr_sendbuf->ew_item_edp[41],
            ptr_sendbuf->ew_item_no[41],
            ptr_sendbuf->ew_line_no[41],
            ptr_sendbuf->ew_qty[41],
            ptr_sendbuf->ew_item_edp[42],
            ptr_sendbuf->ew_item_no[42],
            ptr_sendbuf->ew_line_no[42],
            ptr_sendbuf->ew_qty[42],
            ptr_sendbuf->ew_item_edp[43],
            ptr_sendbuf->ew_item_no[43],
            ptr_sendbuf->ew_line_no[43],
            ptr_sendbuf->ew_qty[43],
            ptr_sendbuf->ew_item_edp[44],
            ptr_sendbuf->ew_item_no[44],
            ptr_sendbuf->ew_line_no[44],
            ptr_sendbuf->ew_qty[44],
            ptr_sendbuf->ew_item_edp[45],
            ptr_sendbuf->ew_item_no[45],
            ptr_sendbuf->ew_line_no[45],
            ptr_sendbuf->ew_qty[45],
            ptr_sendbuf->ew_item_edp[46],
            ptr_sendbuf->ew_item_no[46],
            ptr_sendbuf->ew_line_no[46],
            ptr_sendbuf->ew_qty[46],
            ptr_sendbuf->ew_item_edp[47],
            ptr_sendbuf->ew_item_no[47],
            ptr_sendbuf->ew_line_no[47],
            ptr_sendbuf->ew_qty[47],
            ptr_sendbuf->ew_item_edp[48],
            ptr_sendbuf->ew_item_no[48],
            ptr_sendbuf->ew_line_no[48],
            ptr_sendbuf->ew_qty[48],
            ptr_sendbuf->ew_item_edp[49],
            ptr_sendbuf->ew_item_no[49],
            ptr_sendbuf->ew_line_no[49],
            ptr_sendbuf->ew_qty[49],
            ptr_sendbuf->delete_list);

	return(gbp->j);
}


int ft0074_ParceRecvStr(ft0074_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0074.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0074 %s\"ft0074\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0074_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0074_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0074_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0074_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0074_REC_ID_LEN );
    gbp->count += ft0074_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0074_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0074_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0074_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0074_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0074_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0074_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0074_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0074_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0074_ERR_LEN;

    gbp->count += ft0074_SEND_FILLER_LEN;

    memset(ptr_recvbuf->delete_all_item,'\0', ft0074_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->delete_all_item, sz_recvbufcat + gbp->count, ft0074_FLAG_LEN);
        ap_rprintf(r,"		<DELETE_ALL_ITEM>%s</DELETE_ALL_ITEM>\n", handle_special_chars(gbp,ptr_recvbuf->delete_all_item));
    gbp->count += ft0074_FLAG_LEN;
    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0074>\n", gbp->ft_betag);


    return(0);
}

