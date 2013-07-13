/* tt0133.c ShipMethod Rates */

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



int             tt0133_CatSendStr(struct global_struct *,char *, tt0133_st_send *);
int             tt0133_ParceRecvStr(tt0133_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             tt0133_lt_process(request_rec *, struct global_struct *, char *);



int tt0133_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)                    /* Get the current working path */
                return(-1);

    GetInf(gbp);                                                        /* Read info from INF file */
    

        return(0);
}

int tt0133_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0133_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}

int tt0133_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
	double item_price;

        gbp->sendbuf0133 = (tt0133_st_send *)malloc(sizeof(tt0133_st_send));
        gbp->recvbuf0133 = (tt0133_st_recv *)malloc(sizeof(tt0133_st_recv));

        gbp->sendbufcat = malloc(tt0133_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(tt0133_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

    memset(gbp->sendbufcat, '\0', tt0133_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0133_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0133,  '\0',  sizeof(tt0133_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0133->request_id, "XML");
    strcpy(gbp->sendbuf0133->record_id, "0133");

        strcpy(gbp->sendbuf0133->ip_address, r->connection->remote_ip);
    // Fill the buffers

        get_tag_data("COMPANY", gbp->sendbuf0133->company,gbp,stdout_buffer);
        get_tag_data("DIVISION", gbp->sendbuf0133->division,gbp,stdout_buffer);
        get_tag_data("UID", gbp->sendbuf0133->userid,gbp,stdout_buffer);

		get_tag_data("OFFER", gbp->sendbuf0133->offer,gbp,stdout_buffer);
		get_tag_data("COD_FLAG", gbp->sendbuf0133->cod_flag,gbp,stdout_buffer);
		get_tag_data("BU_CUST_NO", gbp->sendbuf0133->bu_cust_no,gbp,stdout_buffer);
		get_tag_data("BU_COUNTRY", gbp->sendbuf0133->bu_country,gbp,stdout_buffer);
		get_tag_data("BU_CITY", gbp->sendbuf0133->bu_city,gbp,stdout_buffer);
		get_tag_data("BU_ZIP", gbp->sendbuf0133->bu_zip,gbp,stdout_buffer);

  	    for(gbp->i = 0; gbp->i < tt0133_SHIPTO_ARRAY_LEN; gbp->i++)
	    {
                sprintf(gbp->tagname, "ST_CUST_NO_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->shipto[gbp->i].cust_no,gbp,stdout_buffer);

                sprintf(gbp->tagname, "ST_COUNTRY_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->shipto[gbp->i].country,gbp,stdout_buffer);

                sprintf(gbp->tagname, "ST_CITY_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->shipto[gbp->i].city,gbp,stdout_buffer);
				sprintf(gbp->tagname, "ST_ZIP_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->shipto[gbp->i].zip,gbp,stdout_buffer);
				sprintf(gbp->tagname, "ST_SHIPTO_NO_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->shipto[gbp->i].st_no,gbp,stdout_buffer);
				
        }

		for(gbp->i = 0; gbp->i < tt0133_ITEM_ARRAY_LEN; gbp->i++)
	    {
                sprintf(gbp->tagname, "ITEM_NO_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->items[gbp->i].item_no,gbp,stdout_buffer);

                sprintf(gbp->tagname, "ITEM_SHIPTO_NO_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->items[gbp->i].item_shipto,gbp,stdout_buffer);

                sprintf(gbp->tagname, "ITEM_QTY_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->items[gbp->i].item_qty,gbp,stdout_buffer);
				sprintf(gbp->tagname, "ITEM_UNIT_PRICE_%d", gbp->i+1);
                get_tag_data(gbp->tagname, gbp->sendbuf0133->items[gbp->i].unit_price,gbp,stdout_buffer);
				item_price = atof(gbp->sendbuf0133->items[gbp->i].unit_price);
				item_price *=100;
				sprintf(gbp->sendbuf0133->items[gbp->i].unit_price,"%09d",(int)item_price);
        }
       

        if((tt0133_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0133)) == tt0133_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0133);
                        free (gbp->recvbuf0133);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0133_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0133);
                        free (gbp->recvbuf0133);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0133_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0133);
                        free (gbp->recvbuf0133);
                        return(-1);
                }


                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->sendbuf0133);
                        free (gbp->recvbuf0133);
                        return(-1);
                }
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                tt0133_ParceRecvStr(gbp->recvbuf0133, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0133);
                free (gbp->recvbuf0133);
        }
    else
    {           
		        ap_rprintf(r, "Isufficient memory available %d \n",gbp->j);
                XML_Error("tt0133_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->sendbuf0133);
                free (gbp->recvbuf0133);
                return(-1);
    }

    return(0);
}

int tt0133_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0133_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-8.8s%-1.1s%-25.25s%-10.10s%-4.4s%-20.20s%-10.10s%-50.50s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,

			ptr_sendbuf->offer,
			ptr_sendbuf->cod_flag,
			ptr_sendbuf->filler1,
			ptr_sendbuf->bu_cust_no,
			ptr_sendbuf->bu_country,
			ptr_sendbuf->bu_city,
			ptr_sendbuf->bu_zip,
			ptr_sendbuf->bu_filler

            );

	for( gbp->i=0;gbp->i < tt0133_SHIPTO_ARRAY_LEN;(gbp->i)++)
	{
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-10.10s",ptr_sendbuf->shipto[gbp->i].cust_no);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-4.4s",ptr_sendbuf->shipto[gbp->i].country);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-20.20s",ptr_sendbuf->shipto[gbp->i].city);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-10.10s",ptr_sendbuf->shipto[gbp->i].zip);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-2.2s",ptr_sendbuf->shipto[gbp->i].st_no);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-50.50s",ptr_sendbuf->shipto[gbp->i].filler);
	}

	for( gbp->i=0;gbp->i < tt0133_ITEM_ARRAY_LEN;(gbp->i)++)
	{
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-20.20s",ptr_sendbuf->items[gbp->i].item_no);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-2.2s",ptr_sendbuf->items[gbp->i].item_shipto);
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%04d",atoi(ptr_sendbuf->items[gbp->i].item_qty));
		gbp->j += sprintf((sz_sendbufcat + gbp->j),"%-9.9s",ptr_sendbuf->items[gbp->i].unit_price);
	}


    return(gbp->j);
}


int tt0133_ParceRecvStr(tt0133_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{
    gbp->count = 0;

        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0132.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0134 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0134.dtd\">\n", url_tag, tt_tag);
*/
        ap_rprintf(r,"%s0134 %s\"tt0134\">\n", gbp->tt_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);

    memset(ptr_recvbuf->request_id, '\0', tt0133_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0133_REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0133_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0133_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0133_REC_ID_LEN);
    gbp->count += tt0133_REC_ID_LEN;

    memset(ptr_recvbuf->userid, '\0', tt0133_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0133_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0133_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0133_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0133_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0133_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0133_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0133_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0133_ERR_LEN;

    gbp->count += tt0133_SEND_FILLER_LEN;

	for(gbp->i = 0; gbp->i < tt0133_RATES_ARRAY_LEN; gbp->i++)
    {
		 memset(ptr_recvbuf->rates[gbp->i].ship_method,'\0', tt0133_SHIP_METHOD_LEN+1);
         memcpy(ptr_recvbuf->rates[gbp->i].ship_method, sz_recvbufcat + gbp->count, tt0133_SHIP_METHOD_LEN);
         gbp->count += tt0133_SHIP_METHOD_LEN;

		 memset(ptr_recvbuf->rates[gbp->i].ship_method_desc,'\0', tt0133_SHIP_DESC_LEN+1);
         memcpy(ptr_recvbuf->rates[gbp->i].ship_method_desc, sz_recvbufcat + gbp->count, tt0133_SHIP_DESC_LEN);
         gbp->count += tt0133_SHIP_DESC_LEN;

         memset(ptr_recvbuf->rates[gbp->i].ph_amount,'\0', tt0133_PH_AMOUNT_LEN+1);
         memcpy(ptr_recvbuf->rates[gbp->i].ph_amount, sz_recvbufcat + gbp->count, tt0133_PH_AMOUNT_LEN);
         gbp->count += tt0133_PH_AMOUNT_LEN;

         memset(ptr_recvbuf->rates[gbp->i].comment,'\0', tt0133_COMMENT_LEN+1);
         memcpy(ptr_recvbuf->rates[gbp->i].comment, sz_recvbufcat + gbp->count, tt0133_COMMENT_LEN);
         gbp->count += tt0133_COMMENT_LEN;

		 if (atoi(ptr_recvbuf->rates[gbp->i].ship_method) > 0){

                ap_rprintf(r,"		<SHIP_METHOD_RATES>\n");
                ap_rprintf(r,"			<SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->rates[gbp->i].ship_method));
                ap_rprintf(r,"			<SHIP_METHOD_DESC>%s</SHIP_METHOD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->rates[gbp->i].ship_method_desc));
                ap_rprintf(r,"			<PH_AMOUNT>%s</PH_AMOUNT>\n", handle_special_chars(gbp,ptr_recvbuf->rates[gbp->i].ph_amount));
                ap_rprintf(r,"			<COMMENT>%s</COMMENT>\n", handle_special_chars(gbp,ptr_recvbuf->rates[gbp->i].comment));
                ap_rprintf(r,"		</SHIP_METHOD_RATES>\n");

         }
	}

        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0134>\n", gbp->tt_betag);

    return 0;
}


