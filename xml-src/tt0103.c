/* tt0103.c Backorder Cancel */

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



int tt0103_CatSendStr(struct global_struct *,char *, tt0103_st_send *);
int tt0103_ParceRecvStr(tt0103_st_recv *, char *, request_rec *, struct global_struct *);
int tt0103_lt_process(request_rec *, struct global_struct *, char *);

FILE *fp;

int tt0103_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                /* Read info from INF file */
    
    //FILE *fp;

	fp = fopen( "debugxml.txt", "a+" );
	fprintf(fp, "tt0103_start\n");
	fclose(fp);
        return(0);
}

int tt0103_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0103_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

	fp = fopen( "debugxml.txt", "a+" );
	fprintf(fp, "tt0103_end\n");
	fclose(fp);
    return(0);
}

int tt0103_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

	    fp = fopen( "debugxml.txt", "a+" );
	    fprintf(fp, "tt0103_lt_process 1\n");
	    fclose(fp);

        gbp->sendbuf0103 = (tt0103_st_send *)malloc(sizeof(tt0103_st_send));
        gbp->recvbuf0103 = (tt0103_st_recv *)malloc(sizeof(tt0103_st_recv));

        gbp->sendbufcat = malloc(tt0103_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0103_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0103_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0103_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0103,  '\0',  sizeof(tt0103_st_send));

	fp = fopen( "debugxml.txt", "a+" );
	fprintf(fp, "tt0103_lt_process 2\n");
	fclose(fp);

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0103->request_id, "XML");
    strcpy(gbp->sendbuf0103->record_id, "0103");

    strcpy(gbp->sendbuf0103->ip_address, r->connection->remote_ip);
    // Fill the buffers

    get_tag_data("COMPANY", gbp->sendbuf0103->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->sendbuf0103->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->sendbuf0103->userid,gbp,stdout_buffer);
    get_tag_data("ORDER_NO", gbp->sendbuf0103->order_no,gbp,stdout_buffer);
    get_tag_data("WHOLE_ORDER_FLAG", gbp->sendbuf0103->whole_order_flag,gbp,stdout_buffer);
    get_tag_data("CANCEL_LEVEL", gbp->sendbuf0103->cancel_level,gbp,stdout_buffer);
    get_tag_data("CANCEL_ITEM",gbp->sendbuf0103->cancel_item,gbp,stdout_buffer);
        
    fp = fopen( "debugxml.txt", "a+" );
	fprintf(fp, "tt0103_lt_process 3\n");
	fclose(fp);

        if((tt0103_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0103)) == tt0103_LAN_SEND_BUF_LEN)
        {

			    fp = fopen( "debugxml.txt", "a+" );
				fprintf(fp, "tt0103_lt_process 4\n");
				fclose(fp);
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0103);
                        free (gbp->recvbuf0103);
                        return(-1);
                }

				fp = fopen( "debugxml.txt", "a+" );
				fprintf(fp, "tt0103_lt_process 5 After Connect\n");
				fclose(fp);

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0103_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0103);
                        free (gbp->recvbuf0103);
                        return(-1);
                }

				fp = fopen( "debugxml.txt", "a+" );
				fprintf(fp, "tt0103_lt_process 6  After Send\n");
				fclose(fp);

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0103_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0103);
                        free (gbp->recvbuf0103);
                        return(-1);
                }

				fp = fopen( "debugxml.txt", "a+" );
				fprintf(fp, "tt0103_lt_process 7 After Recv\n");
				fclose(fp);


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0103);
                        free (gbp->recvbuf0103);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0103_ParceRecvStr(gbp->recvbuf0103, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0103);
                free (gbp->recvbuf0103);
        }
    else
    {
                XML_Error("tt0103_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0103);
                free (gbp->recvbuf0103);
                return(-1);
    }

    return(0);
}

int tt0103_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0103_st_send *ptr_sendbuf)
{
    fp = fopen( "debugxml.txt", "a+" );
	fprintf(fp, "tt0103_CatSendStr\n");
	fclose(fp);


    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-8.8s%-1.1s%-4.4s%03d",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->order_no,
            ptr_sendbuf->whole_order_flag,
			ptr_sendbuf->cancel_level,
			atoi(ptr_sendbuf->cancel_item));

	fp = fopen( "debugxml.txt", "a+" );
	fprintf(fp, "tt0103_CatSendStr End\n");
	fprintf(fp, "gbp->j  = %d\n", gbp->j);
	fclose(fp);

            
    return(gbp->j);
}


int tt0103_ParceRecvStr(tt0103_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0104.xsl\"?>\n");

#endif

        ap_rprintf(r,"%s0104 %s\"tt0104\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0103_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0103_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0103_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0103_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0103_REC_ID_LEN);
    gbp->count += tt0103_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0103_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0103_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0103_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0103_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0103_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0103_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0103_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0103_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0103_ERR_LEN;

    gbp->count += tt0103_SEND_FILLER_LEN;

    
    ap_rprintf(r,"	%s>\n", gbp->metag);
    ap_rprintf(r,"%s\n", pt_message);
    ap_rprintf(r,"	%s>\n", gbp->rstag);

    ap_rprintf(r,"	%s>\n", gbp->rsetag);
    ap_rprintf(r,"%s0104>\n", gbp->tt_betag);

    return 0;
}


