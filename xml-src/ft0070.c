/* ft0070.c  Review wishlist */
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


int             ft0070_CatSendStr(struct global_struct *, char *, ft0070_st_send *);
int             ft0070_ParceRecvStr(ft0070_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0070_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0070_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0070_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0070_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0070_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0070 = (ft0070_st_send *)malloc(sizeof(ft0070_st_send));
        gbp->ftrecvbuf0070 = (ft0070_st_recv *)malloc(sizeof(ft0070_st_recv));

        gbp->sendbufcat = malloc(ft0070_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0070_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0070_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0070_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0070,  '\0',  sizeof(ft0070_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0070->request_id, "XML");
    strcpy(gbp->ftsendbuf0070->record_id, "0070");
    strcpy(gbp->ftsendbuf0070->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0070->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0070->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0070->userid,gbp,stdout_buffer);
    get_tag_data("WL_COMPANY", gbp->ftsendbuf0070->wl_company,gbp,stdout_buffer);
    get_tag_data("WL_DIVISION", gbp->ftsendbuf0070->wl_division,gbp,stdout_buffer);
    get_tag_data("WL_ID", gbp->ftsendbuf0070->wl_id,gbp,stdout_buffer);
    get_tag_data("SEQ_NUM", gbp->ftsendbuf0070->seq_no,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_EDP", gbp->ftsendbuf0070->cust_edp,gbp,stdout_buffer);
    get_tag_data("PASSWORD", gbp->ftsendbuf0070->password,gbp,stdout_buffer);
    get_tag_data("LOGIN_FLAG", gbp->ftsendbuf0070->login_flag,gbp,stdout_buffer);
    get_tag_data("MULTI_LIST", gbp->ftsendbuf0070->multi_list,gbp,stdout_buffer);
    get_tag_data("PAGE_NUM", gbp->ftsendbuf0070->page_no,gbp,stdout_buffer);
    get_tag_data("PAGE_COUNT", gbp->ftsendbuf0070->page_cnt,gbp,stdout_buffer);


        if(ft0070_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0070) == ft0070_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0070);
                        free (gbp->ftrecvbuf0070);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0070_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0070);
                        free (gbp->ftrecvbuf0070);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0070_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0070);
                        free (gbp->ftrecvbuf0070);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0070);
                        free (gbp->ftrecvbuf0070);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0070_ParceRecvStr(gbp->ftrecvbuf0070, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0070);
                free (gbp->ftrecvbuf0070);

        }
        else
        {
                XML_Error("ft0070_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0070);
                free (gbp->ftrecvbuf0070);
                return(-1);
        }

    return(0);
}


int ft0070_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0070_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-2.2s%-2.2s%-16.16s%02d%-9.9s%-16.16s%-1.1s%-1.1s%04d%04d",

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
            atoi(ptr_sendbuf->seq_no),
            ptr_sendbuf->cust_edp,
            ptr_sendbuf->password,
            ptr_sendbuf->login_flag,
            ptr_sendbuf->multi_list,
            atoi(ptr_sendbuf->page_no),
            atoi(ptr_sendbuf->page_cnt));


	return(gbp->j);
}


int ft0070_ParceRecvStr(ft0070_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0070.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0070 %s\"ft0070\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0070_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0070_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0070_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0070_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0070_REC_ID_LEN );
    gbp->count += ft0070_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0070_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0070_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0070_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0070_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0070_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0070_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0070_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0070_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0070_ERR_LEN;

    gbp->count += ft0070_SEND_FILLER_LEN;

    memset(ptr_recvbuf->wl_id,'\0', ft0070_ID_LEN + 1);
    memcpy(ptr_recvbuf->wl_id, sz_recvbufcat + gbp->count, ft0070_ID_LEN);
        ap_rprintf(r,"		<WL_ID>%s</WL_ID>\n", handle_special_chars(gbp,ptr_recvbuf->wl_id));
    gbp->count += ft0070_ID_LEN;

    memset(ptr_recvbuf->cust_edp,'\0', ft0070_CUST_EDP_LEN + 1);
    memcpy(ptr_recvbuf->cust_edp, sz_recvbufcat + gbp->count, ft0070_CUST_EDP_LEN);
        ap_rprintf(r,"		<CUSTOMER_EDP>%s</CUSTOMER_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += ft0070_CUST_EDP_LEN;

    memset(ptr_recvbuf->fname,'\0', ft0070_NAME_LEN + 1);
    memcpy(ptr_recvbuf->fname, sz_recvbufcat + gbp->count, ft0070_NAME_LEN);
        ap_rprintf(r,"		<FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->fname));
    gbp->count += ft0070_NAME_LEN;

    memset(ptr_recvbuf->minitial,'\0', ft0070_MI_LEN + 1);
    memcpy(ptr_recvbuf->minitial, sz_recvbufcat + gbp->count, ft0070_MI_LEN);
        ap_rprintf(r,"		<MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->minitial));
    gbp->count += ft0070_MI_LEN;

    memset(ptr_recvbuf->lname,'\0', ft0070_NAME_LEN + 1);
    memcpy(ptr_recvbuf->lname, sz_recvbufcat + gbp->count, ft0070_NAME_LEN);
        ap_rprintf(r,"		<LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->lname));
    gbp->count += ft0070_NAME_LEN;

    memset(ptr_recvbuf->city,'\0', ft0070_CITY_LEN + 1);
    memcpy(ptr_recvbuf->city, sz_recvbufcat + gbp->count, ft0070_CITY_LEN);
        ap_rprintf(r,"		<CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->city));
    gbp->count += ft0070_CITY_LEN;

    memset(ptr_recvbuf->state,'\0', ft0070_STATE_LEN + 1);
    memcpy(ptr_recvbuf->state, sz_recvbufcat + gbp->count, ft0070_STATE_LEN);
        ap_rprintf(r,"		<STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->state));
    gbp->count += ft0070_STATE_LEN;

    memset(ptr_recvbuf->street,'\0', ft0070_STREET_LEN + 1);
    memcpy(ptr_recvbuf->street, sz_recvbufcat + gbp->count, ft0070_STREET_LEN);
        ap_rprintf(r,"		<STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->street));
    gbp->count += ft0070_STREET_LEN;

    memset(ptr_recvbuf->zip,'\0', ft0070_ZIP_LEN + 1);
    memcpy(ptr_recvbuf->zip, sz_recvbufcat + gbp->count, ft0070_ZIP_LEN);
        ap_rprintf(r,"		<ZIP>%s</ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->zip));
    gbp->count += ft0070_ZIP_LEN;

    memset(ptr_recvbuf->wpasswd,'\0', ft0070_PASSWD_LEN + 1);
    memcpy(ptr_recvbuf->wpasswd, sz_recvbufcat + gbp->count, ft0070_PASSWD_LEN);
        ap_rprintf(r,"		<WPASSWORD>%s</WPASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->wpasswd));
    gbp->count += ft0070_PASSWD_LEN;

    memset(ptr_recvbuf->item_cnt,'\0', ft0070_ITM_CNT_LEN + 1);
    memcpy(ptr_recvbuf->item_cnt, sz_recvbufcat + gbp->count, ft0070_ITM_CNT_LEN);
        ap_rprintf(r,"		<ITEM_COUNT>%s</ITEM_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->item_cnt));
    gbp->count += ft0070_ITM_CNT_LEN;

    memset(ptr_recvbuf->buyer_flag,'\0', ft0070_FLAG_LEN + 1);
    memcpy(ptr_recvbuf->buyer_flag, sz_recvbufcat + gbp->count, ft0070_FLAG_LEN);
        ap_rprintf(r,"		<BUYER_FLAG>%s</BUYER_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->buyer_flag));
    gbp->count += ft0070_FLAG_LEN;

    memset(ptr_recvbuf->wed_date,'\0', ft0070_DATE_LEN + 1);
    memcpy(ptr_recvbuf->wed_date, sz_recvbufcat + gbp->count, ft0070_DATE_LEN);
        ap_rprintf(r,"		<WED_DATE>%s</WED_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->wed_date));
    gbp->count += ft0070_DATE_LEN;

    memset(ptr_recvbuf->ship_date,'\0', ft0070_DATE_LEN + 1);
    memcpy(ptr_recvbuf->ship_date, sz_recvbufcat + gbp->count, ft0070_DATE_LEN);
        ap_rprintf(r,"		<SHIP_DATE>%s</SHIP_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->ship_date));
    gbp->count += ft0070_DATE_LEN;

    memset(ptr_recvbuf->reg_date,'\0', ft0070_DATE_LEN + 1);
    memcpy(ptr_recvbuf->reg_date, sz_recvbufcat + gbp->count, ft0070_DATE_LEN);
        ap_rprintf(r,"		<REG_DATE>%s</REG_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->reg_date));
    gbp->count += ft0070_DATE_LEN;

    memset(ptr_recvbuf->glb,'\0', ft0070_GLB_LEN + 1);
    memcpy(ptr_recvbuf->glb, sz_recvbufcat + gbp->count, ft0070_GLB_LEN);
        ap_rprintf(r,"		<GLB>%s</GLB>\n", handle_special_chars(gbp,ptr_recvbuf->glb));
    gbp->count += ft0070_GLB_LEN;

    memset(ptr_recvbuf->source,'\0', ft0070_SOURCE_LEN + 1);
    memcpy(ptr_recvbuf->source, sz_recvbufcat + gbp->count, ft0070_SOURCE_LEN);
        ap_rprintf(r,"		<SOURCE>%s</SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->source));
    gbp->count += ft0070_SOURCE_LEN;

    memset(ptr_recvbuf->day_phone,'\0', ft0070_PHONE_LEN + 1);
    memcpy(ptr_recvbuf->day_phone, sz_recvbufcat + gbp->count, ft0070_PHONE_LEN);
        ap_rprintf(r,"		<DAY_PHONE>%s</DAY_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->day_phone));
    gbp->count += ft0070_PHONE_LEN;

    memset(ptr_recvbuf->night_phone,'\0', ft0070_PHONE_LEN + 1);
    memcpy(ptr_recvbuf->night_phone, sz_recvbufcat + gbp->count, ft0070_PHONE_LEN);
        ap_rprintf(r,"		<NIGHT_PHONE>%s</NIGHT_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->night_phone));
    gbp->count += ft0070_PHONE_LEN;

    memset(ptr_recvbuf->password,'\0', ft0070_PASSWORD_LEN + 1);
    memcpy(ptr_recvbuf->password, sz_recvbufcat + gbp->count, ft0070_PASSWORD_LEN);
        ap_rprintf(r,"		<PASSWORD>%s</PASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->password));
    gbp->count += ft0070_SOURCE_LEN;

    memset(ptr_recvbuf->pref_shipto,'\0', ft0070_PREF_SHIPTO_LEN + 1);
    memcpy(ptr_recvbuf->pref_shipto, sz_recvbufcat + gbp->count, ft0070_PREF_SHIPTO_LEN);
        ap_rprintf(r,"		<PREF_SHIPTO>%s</PREF_SHIPTO>\n", handle_special_chars(gbp,ptr_recvbuf->pref_shipto));
    gbp->count += ft0070_PREF_SHIPTO_LEN;

    memset(ptr_recvbuf->g_fname,'\0', ft0070_NAME_LEN + 1);
    memcpy(ptr_recvbuf->g_fname, sz_recvbufcat + gbp->count, ft0070_NAME_LEN);
        ap_rprintf(r,"		<G_FNAME>%s</G_FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->g_fname));
    gbp->count += ft0070_NAME_LEN;

    memset(ptr_recvbuf->g_minit,'\0', ft0070_MI_LEN + 1);
    memcpy(ptr_recvbuf->g_minit, sz_recvbufcat + gbp->count, ft0070_MI_LEN);
        ap_rprintf(r,"		<G_MINITIAL>%s</G_MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->g_minit));
    gbp->count += ft0070_MI_LEN;

    memset(ptr_recvbuf->g_lname,'\0', ft0070_NAME_LEN + 1);
    memcpy(ptr_recvbuf->g_lname, sz_recvbufcat + gbp->count, ft0070_NAME_LEN);
        ap_rprintf(r,"		<G_LNAME>%s</G_LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->g_lname));
    gbp->count += ft0070_NAME_LEN;

    for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->item_cnt); gbp->i++)
    {

        memset(ptr_recvbuf->product_detail[gbp->i].item_edp ,'\0', ft0070_ITM_EDP_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].item_edp, sz_recvbufcat + gbp->count, ft0070_ITM_EDP_LEN);
        gbp->count += ft0070_ITM_EDP_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].item_no ,'\0', ft0070_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].item_no, sz_recvbufcat + gbp->count, ft0070_ITM_NO_LEN);
        gbp->count += ft0070_ITM_NO_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].line_no ,'\0', ft0070_LINE_NO_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].line_no, sz_recvbufcat + gbp->count, ft0070_LINE_NO_LEN);
        gbp->count += ft0070_LINE_NO_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].qty ,'\0', ft0070_QTY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].qty, sz_recvbufcat + gbp->count, ft0070_QTY_LEN);
        gbp->count += ft0070_QTY_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].ordered_qty ,'\0', ft0070_QTY_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].ordered_qty, sz_recvbufcat + gbp->count, ft0070_QTY_LEN);
        gbp->count += ft0070_QTY_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].desc ,'\0', ft0070_DESC_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].desc, sz_recvbufcat + gbp->count, ft0070_DESC_LEN);
        gbp->count += ft0070_DESC_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].custom_flag ,'\0', ft0070_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].custom_flag, sz_recvbufcat + gbp->count, ft0070_FLAG_LEN);
        gbp->count += ft0070_FLAG_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].kit_flag ,'\0', ft0070_FLAG_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].kit_flag, sz_recvbufcat + gbp->count, ft0070_FLAG_LEN);
        gbp->count += ft0070_FLAG_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].price ,'\0', ft0070_PRICE_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].price, sz_recvbufcat + gbp->count, ft0070_PRICE_LEN);
        gbp->count += ft0070_PRICE_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].status ,'\0', ft0070_STATUS_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].status, sz_recvbufcat + gbp->count, ft0070_STATUS_LEN);
        gbp->count += ft0070_STATUS_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].avail_inv ,'\0', ft0070_INV_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].avail_inv, sz_recvbufcat + gbp->count, ft0070_INV_LEN);
        gbp->count += ft0070_INV_LEN;
    
        memset(ptr_recvbuf->product_detail[gbp->i].vendor ,'\0', ft0070_VENDOR_LEN + 1);
        memcpy(ptr_recvbuf->product_detail[gbp->i].vendor, sz_recvbufcat + gbp->count, ft0070_VENDOR_LEN);
        gbp->count += ft0070_VENDOR_LEN;
    
    	ap_rprintf(r,"          <PRODUCT_INFO>\n");
		ap_rprintf(r,"                  <ITEM_EDP>%s</ITEM_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].item_edp));
		ap_rprintf(r,"                  <ITEM_NUM>%s</ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].item_no));
		ap_rprintf(r,"                  <LINE_NUM>%s</LINE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].line_no));
		ap_rprintf(r,"                  <QTY>%s</QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].qty));
		ap_rprintf(r,"                  <ORDERED_QTY>%s</ORDERED_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].ordered_qty));
		ap_rprintf(r,"                  <DESC>%s</DESC>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].desc));
		ap_rprintf(r,"                  <CUSTOM_FLAG>%s</CUSTOM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].custom_flag));
		ap_rprintf(r,"                  <KIT_FLAG>%s</KIT_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].kit_flag));
		ap_rprintf(r,"                  <PRICE>%s</PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].price));
		ap_rprintf(r,"                  <STATUS>%s</STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].status));
		ap_rprintf(r,"                  <AVAIL_INV>%s</AVAIL_INV>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].avail_inv));
		ap_rprintf(r,"                  <VENDOR>%s</VENDOR>\n", handle_special_chars(gbp,ptr_recvbuf->product_detail[gbp->i].vendor));
    	ap_rprintf(r,"          </PRODUCT_INFO>\n");

	}
    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0070>\n", gbp->ft_betag);


    return(0);
}

