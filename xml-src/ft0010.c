/* ft0010.c  Add Comments */
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


int             ft0010_CatSendStr(struct global_struct *, char *, ft0010_st_send *);
int             ft0010_ParceRecvStr(ft0010_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0010_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0010_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0010_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0010_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0010_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0010 = (ft0010_st_send *)malloc(sizeof(ft0010_st_send));
        gbp->ftrecvbuf0010 = (ft0010_st_recv *)malloc(sizeof(ft0010_st_recv));

        gbp->sendbufcat = malloc(ft0010_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0010_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0010_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0010_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0010,  '\0',  sizeof(ft0010_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0010->request_id, "XML");
    strcpy(gbp->ftsendbuf0010->record_id, "0010");
    strcpy(gbp->ftsendbuf0010->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0010->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0010->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0010->userid,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_NUM", gbp->ftsendbuf0010->cust_no,gbp,stdout_buffer);
    get_tag_data("FULL_ORDER_NUM", gbp->ftsendbuf0010->full_order_no,gbp,stdout_buffer);
    get_tag_data("WHO_CALLED", gbp->ftsendbuf0010->who_called,gbp,stdout_buffer);
    get_tag_data("WHY_CALLED", gbp->ftsendbuf0010->why_called,gbp,stdout_buffer);

    for (gbp->i = 0; gbp->i < ft0010_COMMENT_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "COMMENT_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->comment[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "PRINT_ON_PKS_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->print_pks[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "PRINT_ON_INV_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->print_inv[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "PRINT_ON_LBL_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->print_lbl[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "PRINT_ON_WHS_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->print_whs[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "CI_DATE_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->ci_date[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "CI_OPER_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->ci_oper[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "CI_TYPE_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->ci_type[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "SEQ_NUM_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->seq_no[gbp->i],gbp,stdout_buffer);

		sprintf(gbp->tagname, "REC_NUM_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0010->record_no[gbp->i],gbp,stdout_buffer);

	}

        if(ft0010_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0010) == ft0010_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0010);
                        free (gbp->ftrecvbuf0010);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0010_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0010);
                        free (gbp->ftrecvbuf0010);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0010_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0010);
                        free (gbp->ftrecvbuf0010);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0010);
                        free (gbp->ftrecvbuf0010);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0010_ParceRecvStr(gbp->ftrecvbuf0010, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0010);
                free (gbp->ftrecvbuf0010);

        }
        else
        {
                XML_Error("ft0010_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0010);
                free (gbp->ftrecvbuf0010);
                return(-1);
        }

    return(0);
}


int ft0010_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0010_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-10.10s%-12.12s%-2.2s%-2.2s"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d"
			"%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-8.8s%-2.2s%04d%09d",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->cust_no,
            ptr_sendbuf->full_order_no,
            ptr_sendbuf->who_called,
            ptr_sendbuf->why_called,
            ptr_sendbuf->comment[0],
            ptr_sendbuf->print_pks[0],
            ptr_sendbuf->print_inv[0],
            ptr_sendbuf->print_lbl[0],
            ptr_sendbuf->print_whs[0],
            ptr_sendbuf->ci_date[0],
            ptr_sendbuf->ci_oper[0],
            ptr_sendbuf->ci_type[0],
            ptr_sendbuf->seq_no[0],
            ptr_sendbuf->record_no[0],
            ptr_sendbuf->comment[1],
            ptr_sendbuf->print_pks[1],
            ptr_sendbuf->print_inv[1],
            ptr_sendbuf->print_lbl[1],
            ptr_sendbuf->print_whs[1],
            ptr_sendbuf->ci_date[1],
            ptr_sendbuf->ci_oper[1],
            ptr_sendbuf->ci_type[1],
            ptr_sendbuf->seq_no[1],
            ptr_sendbuf->record_no[1],
            ptr_sendbuf->comment[2],
            ptr_sendbuf->print_pks[2],
            ptr_sendbuf->print_inv[2],
            ptr_sendbuf->print_lbl[2],
            ptr_sendbuf->print_whs[2],
            ptr_sendbuf->ci_date[2],
            ptr_sendbuf->ci_oper[2],
            ptr_sendbuf->ci_type[2],
            ptr_sendbuf->seq_no[2],
            ptr_sendbuf->record_no[2],
            ptr_sendbuf->comment[3],
            ptr_sendbuf->print_pks[3],
            ptr_sendbuf->print_inv[3],
            ptr_sendbuf->print_lbl[3],
            ptr_sendbuf->print_whs[3],
            ptr_sendbuf->ci_date[3],
            ptr_sendbuf->ci_oper[3],
            ptr_sendbuf->ci_type[3],
            ptr_sendbuf->seq_no[3],
            ptr_sendbuf->record_no[3],
            ptr_sendbuf->comment[4],
            ptr_sendbuf->print_pks[4],
            ptr_sendbuf->print_inv[4],
            ptr_sendbuf->print_lbl[4],
            ptr_sendbuf->print_whs[4],
            ptr_sendbuf->ci_date[4],
            ptr_sendbuf->ci_oper[4],
            ptr_sendbuf->ci_type[4],
            ptr_sendbuf->seq_no[4],
            ptr_sendbuf->record_no[4],
            ptr_sendbuf->comment[5],
            ptr_sendbuf->print_pks[5],
            ptr_sendbuf->print_inv[5],
            ptr_sendbuf->print_lbl[5],
            ptr_sendbuf->print_whs[5],
            ptr_sendbuf->ci_date[5],
            ptr_sendbuf->ci_oper[5],
            ptr_sendbuf->ci_type[5],
            ptr_sendbuf->seq_no[5],
            ptr_sendbuf->record_no[5],
            ptr_sendbuf->comment[6],
            ptr_sendbuf->print_pks[6],
            ptr_sendbuf->print_inv[6],
            ptr_sendbuf->print_lbl[6],
            ptr_sendbuf->print_whs[6],
            ptr_sendbuf->ci_date[6],
            ptr_sendbuf->ci_oper[6],
            ptr_sendbuf->ci_type[6],
            ptr_sendbuf->seq_no[6],
            ptr_sendbuf->record_no[6],
            ptr_sendbuf->comment[7],
            ptr_sendbuf->print_pks[7],
            ptr_sendbuf->print_inv[7],
            ptr_sendbuf->print_lbl[7],
            ptr_sendbuf->print_whs[7],
            ptr_sendbuf->ci_date[7],
            ptr_sendbuf->ci_oper[7],
            ptr_sendbuf->ci_type[7],
            ptr_sendbuf->seq_no[7],
            ptr_sendbuf->record_no[7],
            ptr_sendbuf->comment[8],
            ptr_sendbuf->print_pks[8],
            ptr_sendbuf->print_inv[8],
            ptr_sendbuf->print_lbl[8],
            ptr_sendbuf->print_whs[8],
            ptr_sendbuf->ci_date[8],
            ptr_sendbuf->ci_oper[8],
            ptr_sendbuf->ci_type[8],
            ptr_sendbuf->seq_no[8],
            ptr_sendbuf->record_no[8],
            ptr_sendbuf->comment[9],
            ptr_sendbuf->print_pks[9],
            ptr_sendbuf->print_inv[9],
            ptr_sendbuf->print_lbl[9],
            ptr_sendbuf->print_whs[9],
            ptr_sendbuf->ci_date[9],
            ptr_sendbuf->ci_oper[9],
            ptr_sendbuf->ci_type[9],
            ptr_sendbuf->seq_no[9],
            ptr_sendbuf->record_no[9],
            ptr_sendbuf->comment[10],
            ptr_sendbuf->print_pks[10],
            ptr_sendbuf->print_inv[10],
            ptr_sendbuf->print_lbl[10],
            ptr_sendbuf->print_whs[10],
            ptr_sendbuf->ci_date[10],
            ptr_sendbuf->ci_oper[10],
            ptr_sendbuf->ci_type[10],
            ptr_sendbuf->seq_no[10],
            ptr_sendbuf->record_no[10],
            ptr_sendbuf->comment[11],
            ptr_sendbuf->print_pks[11],
            ptr_sendbuf->print_inv[11],
            ptr_sendbuf->print_lbl[11],
            ptr_sendbuf->print_whs[11],
            ptr_sendbuf->ci_date[11],
            ptr_sendbuf->ci_oper[11],
            ptr_sendbuf->ci_type[11],
            ptr_sendbuf->seq_no[11],
            ptr_sendbuf->record_no[11],
            ptr_sendbuf->comment[12],
            ptr_sendbuf->print_pks[12],
            ptr_sendbuf->print_inv[12],
            ptr_sendbuf->print_lbl[12],
            ptr_sendbuf->print_whs[12],
            ptr_sendbuf->ci_date[12],
            ptr_sendbuf->ci_oper[12],
            ptr_sendbuf->ci_type[12],
            ptr_sendbuf->seq_no[12],
            ptr_sendbuf->record_no[12],
            ptr_sendbuf->comment[13],
            ptr_sendbuf->print_pks[13],
            ptr_sendbuf->print_inv[13],
            ptr_sendbuf->print_lbl[13],
            ptr_sendbuf->print_whs[13],
            ptr_sendbuf->ci_date[13],
            ptr_sendbuf->ci_oper[13],
            ptr_sendbuf->ci_type[13],
            ptr_sendbuf->seq_no[13],
            ptr_sendbuf->record_no[13],
            ptr_sendbuf->comment[14],
            ptr_sendbuf->print_pks[14],
            ptr_sendbuf->print_inv[14],
            ptr_sendbuf->print_lbl[14],
            ptr_sendbuf->print_whs[14],
            ptr_sendbuf->ci_date[14],
            ptr_sendbuf->ci_oper[14],
            ptr_sendbuf->ci_type[14],
            ptr_sendbuf->seq_no[14],
            ptr_sendbuf->record_no[14],
            ptr_sendbuf->comment[15],
            ptr_sendbuf->print_pks[15],
            ptr_sendbuf->print_inv[15],
            ptr_sendbuf->print_lbl[15],
            ptr_sendbuf->print_whs[15],
            ptr_sendbuf->ci_date[15],
            ptr_sendbuf->ci_oper[15],
            ptr_sendbuf->ci_type[15],
            ptr_sendbuf->seq_no[15],
            ptr_sendbuf->record_no[15],
            ptr_sendbuf->comment[16],
            ptr_sendbuf->print_pks[16],
            ptr_sendbuf->print_inv[16],
            ptr_sendbuf->print_lbl[16],
            ptr_sendbuf->print_whs[16],
            ptr_sendbuf->ci_date[16],
            ptr_sendbuf->ci_oper[16],
            ptr_sendbuf->ci_type[16],
            ptr_sendbuf->seq_no[16],
            ptr_sendbuf->record_no[16],
            ptr_sendbuf->comment[17],
            ptr_sendbuf->print_pks[17],
            ptr_sendbuf->print_inv[17],
            ptr_sendbuf->print_lbl[17],
            ptr_sendbuf->print_whs[17],
            ptr_sendbuf->ci_date[17],
            ptr_sendbuf->ci_oper[17],
            ptr_sendbuf->ci_type[17],
            ptr_sendbuf->seq_no[17],
            ptr_sendbuf->record_no[17],
            ptr_sendbuf->comment[18],
            ptr_sendbuf->print_pks[18],
            ptr_sendbuf->print_inv[18],
            ptr_sendbuf->print_lbl[18],
            ptr_sendbuf->print_whs[18],
            ptr_sendbuf->ci_date[18],
            ptr_sendbuf->ci_oper[18],
            ptr_sendbuf->ci_type[18],
            ptr_sendbuf->seq_no[18],
            ptr_sendbuf->record_no[18],
            ptr_sendbuf->comment[19],
            ptr_sendbuf->print_pks[19],
            ptr_sendbuf->print_inv[19],
            ptr_sendbuf->print_lbl[19],
            ptr_sendbuf->print_whs[19],
            ptr_sendbuf->ci_date[19],
            ptr_sendbuf->ci_oper[19],
            ptr_sendbuf->ci_type[19],
            ptr_sendbuf->seq_no[19],
            ptr_sendbuf->record_no[19],
            ptr_sendbuf->comment[20],
            ptr_sendbuf->print_pks[20],
            ptr_sendbuf->print_inv[20],
            ptr_sendbuf->print_lbl[20],
            ptr_sendbuf->print_whs[20],
            ptr_sendbuf->ci_date[20],
            ptr_sendbuf->ci_oper[20],
            ptr_sendbuf->ci_type[20],
            ptr_sendbuf->seq_no[20],
            ptr_sendbuf->record_no[20],
            ptr_sendbuf->comment[21],
            ptr_sendbuf->print_pks[21],
            ptr_sendbuf->print_inv[21],
            ptr_sendbuf->print_lbl[21],
            ptr_sendbuf->print_whs[21],
            ptr_sendbuf->ci_date[21],
            ptr_sendbuf->ci_oper[21],
            ptr_sendbuf->ci_type[21],
            ptr_sendbuf->seq_no[21],
            ptr_sendbuf->record_no[21],
            ptr_sendbuf->comment[22],
            ptr_sendbuf->print_pks[22],
            ptr_sendbuf->print_inv[22],
            ptr_sendbuf->print_lbl[22],
            ptr_sendbuf->print_whs[22],
            ptr_sendbuf->ci_date[22],
            ptr_sendbuf->ci_oper[22],
            ptr_sendbuf->ci_type[22],
            ptr_sendbuf->seq_no[22],
            ptr_sendbuf->record_no[22],
            ptr_sendbuf->comment[23],
            ptr_sendbuf->print_pks[23],
            ptr_sendbuf->print_inv[23],
            ptr_sendbuf->print_lbl[23],
            ptr_sendbuf->print_whs[23],
            ptr_sendbuf->ci_date[23],
            ptr_sendbuf->ci_oper[23],
            ptr_sendbuf->ci_type[23],
            ptr_sendbuf->seq_no[23],
            ptr_sendbuf->record_no[23],
            ptr_sendbuf->comment[24],
            ptr_sendbuf->print_pks[24],
            ptr_sendbuf->print_inv[24],
            ptr_sendbuf->print_lbl[24],
            ptr_sendbuf->print_whs[24],
            ptr_sendbuf->ci_date[24],
            ptr_sendbuf->ci_oper[24],
            ptr_sendbuf->ci_type[24],
            ptr_sendbuf->seq_no[24],
            ptr_sendbuf->record_no[24],
            ptr_sendbuf->comment[25],
            ptr_sendbuf->print_pks[25],
            ptr_sendbuf->print_inv[25],
            ptr_sendbuf->print_lbl[25],
            ptr_sendbuf->print_whs[25],
            ptr_sendbuf->ci_date[25],
            ptr_sendbuf->ci_oper[25],
            ptr_sendbuf->ci_type[25],
            ptr_sendbuf->seq_no[25],
            ptr_sendbuf->record_no[25],
            ptr_sendbuf->comment[26],
            ptr_sendbuf->print_pks[26],
            ptr_sendbuf->print_inv[26],
            ptr_sendbuf->print_lbl[26],
            ptr_sendbuf->print_whs[26],
            ptr_sendbuf->ci_date[26],
            ptr_sendbuf->ci_oper[26],
            ptr_sendbuf->ci_type[26],
            ptr_sendbuf->seq_no[26],
            ptr_sendbuf->record_no[26],
            ptr_sendbuf->comment[27],
            ptr_sendbuf->print_pks[27],
            ptr_sendbuf->print_inv[27],
            ptr_sendbuf->print_lbl[27],
            ptr_sendbuf->print_whs[27],
            ptr_sendbuf->ci_date[27],
            ptr_sendbuf->ci_oper[27],
            ptr_sendbuf->ci_type[27],
            ptr_sendbuf->seq_no[27],
            ptr_sendbuf->record_no[27],
            ptr_sendbuf->comment[28],
            ptr_sendbuf->print_pks[28],
            ptr_sendbuf->print_inv[28],
            ptr_sendbuf->print_lbl[28],
            ptr_sendbuf->print_whs[28],
            ptr_sendbuf->ci_date[28],
            ptr_sendbuf->ci_oper[28],
            ptr_sendbuf->ci_type[28],
            ptr_sendbuf->seq_no[28],
            ptr_sendbuf->record_no[28],
            ptr_sendbuf->comment[29],
            ptr_sendbuf->print_pks[29],
            ptr_sendbuf->print_inv[29],
            ptr_sendbuf->print_lbl[29],
            ptr_sendbuf->print_whs[29],
            ptr_sendbuf->ci_date[29],
            ptr_sendbuf->ci_oper[29],
            ptr_sendbuf->ci_type[29],
            ptr_sendbuf->seq_no[29],
            ptr_sendbuf->record_no[29],
            ptr_sendbuf->comment[30],
            ptr_sendbuf->print_pks[30],
            ptr_sendbuf->print_inv[30],
            ptr_sendbuf->print_lbl[30],
            ptr_sendbuf->print_whs[30],
            ptr_sendbuf->ci_date[30],
            ptr_sendbuf->ci_oper[30],
            ptr_sendbuf->ci_type[30],
            ptr_sendbuf->seq_no[30],
            ptr_sendbuf->record_no[30],
            ptr_sendbuf->comment[31],
            ptr_sendbuf->print_pks[31],
            ptr_sendbuf->print_inv[31],
            ptr_sendbuf->print_lbl[31],
            ptr_sendbuf->print_whs[31],
            ptr_sendbuf->ci_date[31],
            ptr_sendbuf->ci_oper[31],
            ptr_sendbuf->ci_type[31],
            ptr_sendbuf->seq_no[31],
            ptr_sendbuf->record_no[31],
            ptr_sendbuf->comment[32],
            ptr_sendbuf->print_pks[32],
            ptr_sendbuf->print_inv[32],
            ptr_sendbuf->print_lbl[32],
            ptr_sendbuf->print_whs[32],
            ptr_sendbuf->ci_date[32],
            ptr_sendbuf->ci_oper[32],
            ptr_sendbuf->ci_type[32],
            ptr_sendbuf->seq_no[32],
            ptr_sendbuf->record_no[32],
            ptr_sendbuf->comment[33],
            ptr_sendbuf->print_pks[33],
            ptr_sendbuf->print_inv[33],
            ptr_sendbuf->print_lbl[33],
            ptr_sendbuf->print_whs[33],
            ptr_sendbuf->ci_date[33],
            ptr_sendbuf->ci_oper[33],
            ptr_sendbuf->ci_type[33],
            ptr_sendbuf->seq_no[33],
            ptr_sendbuf->record_no[33],
            ptr_sendbuf->comment[34],
            ptr_sendbuf->print_pks[34],
            ptr_sendbuf->print_inv[34],
            ptr_sendbuf->print_lbl[34],
            ptr_sendbuf->print_whs[34],
            ptr_sendbuf->ci_date[34],
            ptr_sendbuf->ci_oper[34],
            ptr_sendbuf->ci_type[34],
            ptr_sendbuf->seq_no[34],
            ptr_sendbuf->record_no[34],
            ptr_sendbuf->comment[35],
            ptr_sendbuf->print_pks[35],
            ptr_sendbuf->print_inv[35],
            ptr_sendbuf->print_lbl[35],
            ptr_sendbuf->print_whs[35],
            ptr_sendbuf->ci_date[35],
            ptr_sendbuf->ci_oper[35],
            ptr_sendbuf->ci_type[35],
            ptr_sendbuf->seq_no[35],
            ptr_sendbuf->record_no[35],
            ptr_sendbuf->comment[36],
            ptr_sendbuf->print_pks[36],
            ptr_sendbuf->print_inv[36],
            ptr_sendbuf->print_lbl[36],
            ptr_sendbuf->print_whs[36],
            ptr_sendbuf->ci_date[36],
            ptr_sendbuf->ci_oper[36],
            ptr_sendbuf->ci_type[36],
            ptr_sendbuf->seq_no[36],
            ptr_sendbuf->record_no[36],
            ptr_sendbuf->comment[37],
            ptr_sendbuf->print_pks[37],
            ptr_sendbuf->print_inv[37],
            ptr_sendbuf->print_lbl[37],
            ptr_sendbuf->print_whs[37],
            ptr_sendbuf->ci_date[37],
            ptr_sendbuf->ci_oper[37],
            ptr_sendbuf->ci_type[37],
            ptr_sendbuf->seq_no[37],
            ptr_sendbuf->record_no[37],
            ptr_sendbuf->comment[38],
            ptr_sendbuf->print_pks[38],
            ptr_sendbuf->print_inv[38],
            ptr_sendbuf->print_lbl[38],
            ptr_sendbuf->print_whs[38],
            ptr_sendbuf->ci_date[38],
            ptr_sendbuf->ci_oper[38],
            ptr_sendbuf->ci_type[38],
            ptr_sendbuf->seq_no[38],
            ptr_sendbuf->record_no[38],
            ptr_sendbuf->comment[39],
            ptr_sendbuf->print_pks[39],
            ptr_sendbuf->print_inv[39],
            ptr_sendbuf->print_lbl[39],
            ptr_sendbuf->print_whs[39],
            ptr_sendbuf->ci_date[39],
            ptr_sendbuf->ci_oper[39],
            ptr_sendbuf->ci_type[39],
            ptr_sendbuf->seq_no[39],
            ptr_sendbuf->record_no[39],
            ptr_sendbuf->comment[40],
            ptr_sendbuf->print_pks[40],
            ptr_sendbuf->print_inv[40],
            ptr_sendbuf->print_lbl[40],
            ptr_sendbuf->print_whs[40],
            ptr_sendbuf->ci_date[40],
            ptr_sendbuf->ci_oper[40],
            ptr_sendbuf->ci_type[40],
            ptr_sendbuf->seq_no[40],
            ptr_sendbuf->record_no[40],
            ptr_sendbuf->comment[41],
            ptr_sendbuf->print_pks[41],
            ptr_sendbuf->print_inv[41],
            ptr_sendbuf->print_lbl[41],
            ptr_sendbuf->print_whs[41],
            ptr_sendbuf->ci_date[41],
            ptr_sendbuf->ci_oper[41],
            ptr_sendbuf->ci_type[41],
            ptr_sendbuf->seq_no[41],
            ptr_sendbuf->record_no[41],
            ptr_sendbuf->comment[42],
            ptr_sendbuf->print_pks[42],
            ptr_sendbuf->print_inv[42],
            ptr_sendbuf->print_lbl[42],
            ptr_sendbuf->print_whs[42],
            ptr_sendbuf->ci_date[42],
            ptr_sendbuf->ci_oper[42],
            ptr_sendbuf->ci_type[42],
            ptr_sendbuf->seq_no[42],
            ptr_sendbuf->record_no[42],
            ptr_sendbuf->comment[43],
            ptr_sendbuf->print_pks[43],
            ptr_sendbuf->print_inv[43],
            ptr_sendbuf->print_lbl[43],
            ptr_sendbuf->print_whs[43],
            ptr_sendbuf->ci_date[43],
            ptr_sendbuf->ci_oper[43],
            ptr_sendbuf->ci_type[43],
            ptr_sendbuf->seq_no[43],
            ptr_sendbuf->record_no[43],
            ptr_sendbuf->comment[44],
            ptr_sendbuf->print_pks[44],
            ptr_sendbuf->print_inv[44],
            ptr_sendbuf->print_lbl[44],
            ptr_sendbuf->print_whs[44],
            ptr_sendbuf->ci_date[44],
            ptr_sendbuf->ci_oper[44],
            ptr_sendbuf->ci_type[44],
            ptr_sendbuf->seq_no[44],
            ptr_sendbuf->record_no[44],
            ptr_sendbuf->comment[45],
            ptr_sendbuf->print_pks[45],
            ptr_sendbuf->print_inv[45],
            ptr_sendbuf->print_lbl[45],
            ptr_sendbuf->print_whs[45],
            ptr_sendbuf->ci_date[45],
            ptr_sendbuf->ci_oper[45],
            ptr_sendbuf->ci_type[45],
            ptr_sendbuf->seq_no[45],
            ptr_sendbuf->record_no[45],
            ptr_sendbuf->comment[46],
            ptr_sendbuf->print_pks[46],
            ptr_sendbuf->print_inv[46],
            ptr_sendbuf->print_lbl[46],
            ptr_sendbuf->print_whs[46],
            ptr_sendbuf->ci_date[46],
            ptr_sendbuf->ci_oper[46],
            ptr_sendbuf->ci_type[46],
            ptr_sendbuf->seq_no[46],
            ptr_sendbuf->record_no[46],
            ptr_sendbuf->comment[47],
            ptr_sendbuf->print_pks[47],
            ptr_sendbuf->print_inv[47],
            ptr_sendbuf->print_lbl[47],
            ptr_sendbuf->print_whs[47],
            ptr_sendbuf->ci_date[47],
            ptr_sendbuf->ci_oper[47],
            ptr_sendbuf->ci_type[47],
            ptr_sendbuf->seq_no[47],
            ptr_sendbuf->record_no[47]);


	return(gbp->j);
}


int ft0010_ParceRecvStr(ft0010_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0010.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0010 %s\"ft0010\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0010_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0010_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0010_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0010_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0010_REC_ID_LEN );
    gbp->count += ft0010_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0010_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0010_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0010_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0010_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0010_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0010_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0010_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0010_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0010_ERR_LEN;

    gbp->count += ft0010_SEND_FILLER_LEN;

    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0010>\n", gbp->ft_betag);


    return(0);
}

