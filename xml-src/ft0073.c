/* ft0073.c  Add to wishlist */
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


int             ft0073_CatSendStr(struct global_struct *, char *, ft0073_st_send *);
int             ft0073_ParceRecvStr(ft0073_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0073_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0073_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0073_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0073_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0073_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0073 = (ft0073_st_send *)malloc(sizeof(ft0073_st_send));
        gbp->ftrecvbuf0073 = (ft0073_st_recv *)malloc(sizeof(ft0073_st_recv));

        gbp->sendbufcat = malloc(ft0073_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0073_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0073_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0073_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0073,  '\0',  sizeof(ft0073_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0073->request_id, "XML");
    strcpy(gbp->ftsendbuf0073->record_id, "0073");
    strcpy(gbp->ftsendbuf0073->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0073->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0073->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0073->userid,gbp,stdout_buffer);
    get_tag_data("WL_COMPANY", gbp->ftsendbuf0073->wl_company,gbp,stdout_buffer);
    get_tag_data("WL_DIVISION", gbp->ftsendbuf0073->wl_division,gbp,stdout_buffer);
    get_tag_data("WL_ID", gbp->ftsendbuf0073->wl_id,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_EDP", gbp->ftsendbuf0073->cust_edp,gbp,stdout_buffer);
    get_tag_data("ITEM_NUM", gbp->ftsendbuf0073->item_no,gbp,stdout_buffer);
    get_tag_data("ITEM_QTY", gbp->ftsendbuf0073->qty,gbp,stdout_buffer);
    get_tag_data("CUSTOM_FLAG", gbp->ftsendbuf0073->custom_flag,gbp,stdout_buffer);
    get_tag_data("KIT_FLAG", gbp->ftsendbuf0073->kit_flag,gbp,stdout_buffer);
    get_tag_data("PASSWORD", gbp->ftsendbuf0073->password,gbp,stdout_buffer);

    for (gbp->i = 0; gbp->i < ft0073_CUSTOM_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "CUSTOMIZATION_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0073->custom[gbp->i],gbp,stdout_buffer);

	}

    for (gbp->i = 0; gbp->i < ft0073_R1_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "R1_ITEM_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0073->r1_item_no[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "R1_QTY_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0073->r1_item_qty[gbp->i],gbp,stdout_buffer);
	}

    get_tag_data("MULTI_LIST", gbp->ftsendbuf0073->multi_list,gbp,stdout_buffer);
    get_tag_data("WED_DATE", gbp->ftsendbuf0073->wed_date,gbp,stdout_buffer);
    get_tag_data("SHIP_DATE", gbp->ftsendbuf0073->shp_date,gbp,stdout_buffer);
    get_tag_data("REG_DATE", gbp->ftsendbuf0073->reg_date,gbp,stdout_buffer);
    get_tag_data("GLB", gbp->ftsendbuf0073->glb,gbp,stdout_buffer);
    get_tag_data("SOURCE", gbp->ftsendbuf0073->source,gbp,stdout_buffer);
    get_tag_data("PASSWORD", gbp->ftsendbuf0073->password,gbp,stdout_buffer);
    get_tag_data("PREF_SHIPTO", gbp->ftsendbuf0073->pref_shipto,gbp,stdout_buffer);


        if(ft0073_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0073) == ft0073_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0073);
                        free (gbp->ftrecvbuf0073);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0073_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0073);
                        free (gbp->ftrecvbuf0073);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0073_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0073);
                        free (gbp->ftrecvbuf0073);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0073);
                        free (gbp->ftrecvbuf0073);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0073_ParceRecvStr(gbp->ftrecvbuf0073, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0073);
                free (gbp->ftrecvbuf0073);

        }
        else
        {
                XML_Error("ft0073_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0073);
                free (gbp->ftrecvbuf0073);
                return(-1);
        }

    return(0);
}


int ft0073_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0073_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-2.2s%-2.2s%-16.16s%-9.9s%-20.20s%04d%-1.1s%-1.1s"
			"%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s"
			"%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s%-60.60s"
			"%-60.60s%-60.60s%-60.60s"
			"%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
			"%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
			"%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d%-20.20s%04d"
			"%-20.20s%04d"
			"%-1.1s%-8.8s%-8.8s%-8.8s%-1.1s%-16.16s%-16.16s%-9.9s",

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
            ptr_sendbuf->cust_edp,
            ptr_sendbuf->item_no,
            ptr_sendbuf->qty,
            ptr_sendbuf->custom_flag,
            ptr_sendbuf->kit_flag,
            ptr_sendbuf->custom[0],
            ptr_sendbuf->custom[1],
            ptr_sendbuf->custom[2],
            ptr_sendbuf->custom[3],
            ptr_sendbuf->custom[4],
            ptr_sendbuf->custom[5],
            ptr_sendbuf->custom[6],
            ptr_sendbuf->custom[7],
            ptr_sendbuf->custom[8],
            ptr_sendbuf->custom[9],
            ptr_sendbuf->custom[10],
            ptr_sendbuf->custom[11],
            ptr_sendbuf->custom[12],
            ptr_sendbuf->custom[13],
            ptr_sendbuf->custom[14],
            ptr_sendbuf->custom[15],
            ptr_sendbuf->custom[16],
            ptr_sendbuf->custom[17],
            ptr_sendbuf->custom[18],
            ptr_sendbuf->custom[19],
            ptr_sendbuf->custom[20],
            ptr_sendbuf->custom[21],
            ptr_sendbuf->custom[22],
            ptr_sendbuf->custom[23],
            ptr_sendbuf->custom[24],
            ptr_sendbuf->r1_item_no[0],
            ptr_sendbuf->r1_item_qty[0],
            ptr_sendbuf->r1_item_no[1],
            ptr_sendbuf->r1_item_qty[1],
            ptr_sendbuf->r1_item_no[2],
            ptr_sendbuf->r1_item_qty[2],
            ptr_sendbuf->r1_item_no[3],
            ptr_sendbuf->r1_item_qty[3],
            ptr_sendbuf->r1_item_no[4],
            ptr_sendbuf->r1_item_qty[4],
            ptr_sendbuf->r1_item_no[5],
            ptr_sendbuf->r1_item_qty[5],
            ptr_sendbuf->r1_item_no[6],
            ptr_sendbuf->r1_item_qty[6],
            ptr_sendbuf->r1_item_no[7],
            ptr_sendbuf->r1_item_qty[7],
            ptr_sendbuf->r1_item_no[8],
            ptr_sendbuf->r1_item_qty[8],
            ptr_sendbuf->r1_item_no[9],
            ptr_sendbuf->r1_item_qty[9],
            ptr_sendbuf->r1_item_no[10],
            ptr_sendbuf->r1_item_qty[10],
            ptr_sendbuf->r1_item_no[11],
            ptr_sendbuf->r1_item_qty[11],
            ptr_sendbuf->r1_item_no[12],
            ptr_sendbuf->r1_item_qty[12],
            ptr_sendbuf->r1_item_no[13],
            ptr_sendbuf->r1_item_qty[13],
            ptr_sendbuf->r1_item_no[14],
            ptr_sendbuf->r1_item_qty[14],
            ptr_sendbuf->r1_item_no[15],
            ptr_sendbuf->r1_item_qty[15],
            ptr_sendbuf->r1_item_no[16],
            ptr_sendbuf->r1_item_qty[16],
            ptr_sendbuf->r1_item_no[17],
            ptr_sendbuf->r1_item_qty[17],
            ptr_sendbuf->r1_item_no[18],
            ptr_sendbuf->r1_item_qty[18],
            ptr_sendbuf->r1_item_no[19],
            ptr_sendbuf->r1_item_qty[19],
            ptr_sendbuf->r1_item_no[20],
            ptr_sendbuf->r1_item_qty[20],
            ptr_sendbuf->r1_item_no[21],
            ptr_sendbuf->r1_item_qty[21],
            ptr_sendbuf->r1_item_no[22],
            ptr_sendbuf->r1_item_qty[22],
            ptr_sendbuf->r1_item_no[23],
            ptr_sendbuf->r1_item_qty[23],
            ptr_sendbuf->r1_item_no[24],
            ptr_sendbuf->r1_item_qty[24],
            ptr_sendbuf->multi_list,
            ptr_sendbuf->wed_date,
            ptr_sendbuf->shp_date,
            ptr_sendbuf->reg_date,
            ptr_sendbuf->glb,
            ptr_sendbuf->source,
            ptr_sendbuf->password,
            ptr_sendbuf->pref_shipto);

	return(gbp->j);
}


int ft0073_ParceRecvStr(ft0073_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0073.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0073 %s\"ft0073\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0073_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0073_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0073_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0073_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0073_REC_ID_LEN );
    gbp->count += ft0073_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0073_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0073_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0073_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0073_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0073_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0073_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0073_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0073_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0073_ERR_LEN;

    gbp->count += ft0073_SEND_FILLER_LEN;

    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0073>\n", gbp->ft_betag);


    return(0);
}

