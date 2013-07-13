/* ft0020.c  List Data */
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


int             ft0020_CatSendStr(struct global_struct *, char *, ft0020_st_send *);
int             ft0020_ParceRecvStr(ft0020_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0020_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0020_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0020_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0020_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0020_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0020 = (ft0020_st_send *)malloc(sizeof(ft0020_st_send));
        gbp->ftrecvbuf0020 = (ft0020_st_recv *)malloc(sizeof(ft0020_st_recv));

        gbp->sendbufcat = malloc(ft0020_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0020_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0020_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0020_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0020,  '\0',  sizeof(ft0020_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0020->request_id, "XML");
    strcpy(gbp->ftsendbuf0020->record_id, "0020");
    strcpy(gbp->ftsendbuf0020->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0020->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0020->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0020->userid,gbp,stdout_buffer);
    get_tag_data("LIST_TYPE", gbp->ftsendbuf0020->list_type,gbp,stdout_buffer);
    get_tag_data("PAGE_NUM", gbp->ftsendbuf0020->page_no,gbp,stdout_buffer);
    get_tag_data("PAGE_COUNT", gbp->ftsendbuf0020->page_cnt,gbp,stdout_buffer);
    get_tag_data("LIST_PARM", gbp->ftsendbuf0020->list_param,gbp,stdout_buffer);


        if(ft0020_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0020) == ft0020_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0020);
                        free (gbp->ftrecvbuf0020);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0020_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0020);
                        free (gbp->ftrecvbuf0020);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0020_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0020);
                        free (gbp->ftrecvbuf0020);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0020);
                        free (gbp->ftrecvbuf0020);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0020_ParceRecvStr(gbp->ftrecvbuf0020, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0020);
                free (gbp->ftrecvbuf0020);

        }
        else
        {
                XML_Error("ft0020_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0020);
                free (gbp->ftrecvbuf0020);
                return(-1);
        }

    return(0);
}


int ft0020_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0020_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-2.2s%04d%04d%-20.20s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->list_type,
            ptr_sendbuf->page_no,
            ptr_sendbuf->page_cnt,
            ptr_sendbuf->list_param);

	
	return(gbp->j);
}


int ft0020_ParceRecvStr(ft0020_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0020.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0020 %s\"ft0020\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0020_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0020_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0020_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0020_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0020_REC_ID_LEN );
    gbp->count += ft0020_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0020_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0020_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0020_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0020_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0020_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0020_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0020_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0020_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0020_ERR_LEN;

    gbp->count += ft0020_SEND_FILLER_LEN;

    memset(ptr_recvbuf->list_cnt,'\0', ft0020_LIST_CNT_LEN + 1);
    memcpy(ptr_recvbuf->list_cnt, sz_recvbufcat + gbp->count, ft0020_LIST_CNT_LEN);
        ap_rprintf(r,"		<LIST_COUNT>%s</LIST_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->list_cnt));
    gbp->count += ft0020_LIST_CNT_LEN;

    memset(ptr_recvbuf->page_no,'\0', ft0020_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, ft0020_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NUM>%s</PAGE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += ft0020_PAGE_NO_LEN;

    for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->list_cnt); gbp->i++)
    {

        memset(ptr_recvbuf->list_detail[gbp->i].list_short ,'\0', ft0020_LIST_SHT_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].list_short, sz_recvbufcat + gbp->count, ft0020_LIST_SHT_LEN);
        gbp->count += ft0020_LIST_SHT_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].title_code ,'\0', ft0020_TITLE_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].title_code, sz_recvbufcat + gbp->count, ft0020_TITLE_CODE_LEN);
        gbp->count += ft0020_TITLE_CODE_LEN;

        gbp->count += ft0020_FILLER1_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].state_code ,'\0', ft0020_STATE_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].state_code, sz_recvbufcat + gbp->count, ft0020_STATE_CODE_LEN);
        gbp->count += ft0020_STATE_CODE_LEN;

        gbp->count += ft0020_FILLER2_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].country_code ,'\0', ft0020_COUNTRY_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].country_code, sz_recvbufcat + gbp->count, ft0020_COUNTRY_CODE_LEN);
        gbp->count += ft0020_COUNTRY_CODE_LEN;

        gbp->count += ft0020_FILLER3_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].code_id ,'\0', ft0020_CODE_ID_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].code_id, sz_recvbufcat + gbp->count, ft0020_CODE_ID_LEN);
        gbp->count += ft0020_CODE_ID_LEN;

        gbp->count += ft0020_FILLER4_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].stand_alone ,'\0', ft0020_STAND_ALONE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].stand_alone, sz_recvbufcat + gbp->count, ft0020_STAND_ALONE_LEN);
        gbp->count += ft0020_STAND_ALONE_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].disc_code ,'\0', ft0020_DISC_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].disc_code, sz_recvbufcat + gbp->count, ft0020_DISC_CODE_LEN);
        gbp->count += ft0020_DISC_CODE_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].disc_type ,'\0', ft0020_DISC_TYPE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].disc_type, sz_recvbufcat + gbp->count, ft0020_DISC_TYPE_LEN);
        gbp->count += ft0020_DISC_TYPE_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].disc_amt ,'\0', ft0020_DISC_AMT_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].disc_amt, sz_recvbufcat + gbp->count, ft0020_DISC_AMT_LEN);
        gbp->count += ft0020_DISC_AMT_LEN;

        gbp->count += ft0020_FILLER5_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].codiv ,'\0', ft0020_CODIV_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].codiv, sz_recvbufcat + gbp->count, ft0020_CODIV_LEN);
        gbp->count += ft0020_CODIV_LEN;

        gbp->count += ft0020_FILLER6_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].cust_no ,'\0', ft0020_CUST_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].cust_no, sz_recvbufcat + gbp->count, ft0020_CUST_NO_LEN);
        gbp->count += ft0020_CUST_NO_LEN;

        gbp->count += ft0020_FILLER7_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].pm_group ,'\0', ft0020_PM_GROUP_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].pm_group, sz_recvbufcat + gbp->count, ft0020_PM_GROUP_LEN);
        gbp->count += ft0020_PM_GROUP_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].pm_id ,'\0', ft0020_PM_ID_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].pm_id, sz_recvbufcat + gbp->count, ft0020_PM_ID_LEN);
        gbp->count += ft0020_PM_ID_LEN;

        gbp->count += ft0020_FILLER8_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].ship_method ,'\0', ft0020_SM_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].ship_method, sz_recvbufcat + gbp->count, ft0020_SM_LEN);
        gbp->count += ft0020_SM_LEN;

        gbp->count += ft0020_FILLER9_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].in_plan ,'\0', ft0020_IN_PLAN_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].in_plan, sz_recvbufcat + gbp->count, ft0020_IN_PLAN_LEN);
        gbp->count += ft0020_IN_PLAN_LEN;

        gbp->count += ft0020_FILLER10_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].key_label ,'\0', ft0020_KEY_LABEL_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].key_label, sz_recvbufcat + gbp->count, ft0020_KEY_LABEL_LEN);
        gbp->count += ft0020_KEY_LABEL_LEN;

        gbp->count += ft0020_FILLER11_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].foc_code ,'\0', ft0020_FOC_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].foc_code, sz_recvbufcat + gbp->count, ft0020_FOC_CODE_LEN);
        gbp->count += ft0020_FOC_CODE_LEN;

        gbp->count += ft0020_FILLER12_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].cp_source ,'\0', ft0020_CP_SRC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].cp_source, sz_recvbufcat + gbp->count, ft0020_CP_SRC_LEN);
        gbp->count += ft0020_CP_SRC_LEN;

        gbp->count += ft0020_FILLER13_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].codiv_no ,'\0', ft0020_CODIV_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].codiv_no, sz_recvbufcat + gbp->count, ft0020_CODIV_NO_LEN);
        gbp->count += ft0020_CODIV_NO_LEN;

        gbp->count += ft0020_FILLER14_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].dept_num ,'\0', ft0020_DEPT_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].dept_num, sz_recvbufcat + gbp->count, ft0020_DEPT_NO_LEN);
        gbp->count += ft0020_DEPT_NO_LEN;

        gbp->count += ft0020_FILLER15_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].kit_hdr ,'\0', ft0020_KIT_HDR_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].kit_hdr, sz_recvbufcat + gbp->count, ft0020_KIT_HDR_LEN);
        gbp->count += ft0020_KIT_HDR_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].printer ,'\0', ft0020_PRINTER_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].printer, sz_recvbufcat + gbp->count, ft0020_PRINTER_LEN);
        gbp->count += ft0020_PRINTER_LEN;

        gbp->count += ft0020_FILLER16_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].ra_no ,'\0', ft0020_RA_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].ra_no, sz_recvbufcat + gbp->count, ft0020_RA_NO_LEN);
        gbp->count += ft0020_RA_NO_LEN;

        gbp->count += ft0020_FILLER17_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].vend_no ,'\0', ft0020_VEND_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].vend_no, sz_recvbufcat + gbp->count, ft0020_VEND_NO_LEN);
        gbp->count += ft0020_VEND_NO_LEN;

        gbp->count += ft0020_FILLER18_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].whse_code ,'\0', ft0020_WHSE_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].whse_code, sz_recvbufcat + gbp->count, ft0020_WHSE_CODE_LEN);
        gbp->count += ft0020_WHSE_CODE_LEN;

        gbp->count += ft0020_FILLER19_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].comment_no ,'\0', ft0020_COMMENT_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].comment_no, sz_recvbufcat + gbp->count, ft0020_COMMENT_NO_LEN);
        gbp->count += ft0020_COMMENT_NO_LEN;

        gbp->count += ft0020_FILLER20_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].chain_no ,'\0', ft0020_CHAIN_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].chain_no, sz_recvbufcat + gbp->count, ft0020_CHAIN_NO_LEN);
        gbp->count += ft0020_CHAIN_NO_LEN;

        gbp->count += ft0020_FILLER21_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].disc_table ,'\0', ft0020_DISC_TABLE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].disc_table, sz_recvbufcat + gbp->count, ft0020_DISC_TABLE_LEN);
        gbp->count += ft0020_DISC_TABLE_LEN;

        gbp->count += ft0020_FILLER22_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].item_no ,'\0', ft0020_ITM_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].item_no, sz_recvbufcat + gbp->count, ft0020_ITM_NO_LEN);
        gbp->count += ft0020_ITM_NO_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].res_loc ,'\0', ft0020_RES_LOC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].res_loc, sz_recvbufcat + gbp->count, ft0020_RES_LOC_LEN);
        gbp->count += ft0020_RES_LOC_LEN;

        gbp->count += ft0020_FILLER23_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].prod_no ,'\0', ft0020_PROD_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].prod_no, sz_recvbufcat + gbp->count, ft0020_PROD_NO_LEN);
        gbp->count += ft0020_PROD_NO_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].custom_codes ,'\0', ft0020_CUSTOM_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].custom_codes, sz_recvbufcat + gbp->count, ft0020_CUSTOM_CODE_LEN);
        gbp->count += ft0020_CUSTOM_CODE_LEN;

        gbp->count += ft0020_FILLER24_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].letter_code ,'\0', ft0020_LETTER_CODE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].letter_code, sz_recvbufcat + gbp->count, ft0020_LETTER_CODE_LEN);
        gbp->count += ft0020_LETTER_CODE_LEN;

        gbp->count += ft0020_FILLER25_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].style_table ,'\0', ft0020_STYLE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].style_table, sz_recvbufcat + gbp->count, ft0020_STYLE_LEN);
        gbp->count += ft0020_STYLE_LEN;

        gbp->count += ft0020_FILLER26_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].ct ,'\0', ft0020_CT_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].ct, sz_recvbufcat + gbp->count, ft0020_CT_LEN);
        gbp->count += ft0020_CT_LEN;

        gbp->count += ft0020_FILLER27_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].list_long ,'\0', ft0020_LIST_LONG_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].list_long, sz_recvbufcat + gbp->count, ft0020_LIST_LONG_LEN);
        gbp->count += ft0020_LIST_LONG_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].title ,'\0', ft0020_TITLE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].title, sz_recvbufcat + gbp->count, ft0020_TITLE_LEN);
        gbp->count += ft0020_TITLE_LEN;

        gbp->count += ft0020_FILLER28_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].country ,'\0', ft0020_COUNTRY_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].country, sz_recvbufcat + gbp->count, ft0020_COUNTRY_LEN);
        gbp->count += ft0020_COUNTRY_LEN;

        gbp->count += ft0020_FILLER29_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].code_abrv ,'\0', ft0020_CODE_ABRV_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].code_abrv, sz_recvbufcat + gbp->count, ft0020_CODE_ABRV_LEN);
        gbp->count += ft0020_CODE_ABRV_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].code_spec ,'\0', ft0020_CODE_SPEC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].code_spec, sz_recvbufcat + gbp->count, ft0020_CODE_SPEC_LEN);
        gbp->count += ft0020_CODE_SPEC_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].comp_desc ,'\0', ft0020_COMP_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].comp_desc, sz_recvbufcat + gbp->count, ft0020_COMP_DESC_LEN);
        gbp->count += ft0020_COMP_DESC_LEN;

        gbp->count += ft0020_FILLER30_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].pm_desc ,'\0', ft0020_PM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].pm_desc, sz_recvbufcat + gbp->count, ft0020_PM_DESC_LEN);
        gbp->count += ft0020_PM_DESC_LEN;

        gbp->count += ft0020_FILLER31_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].sm_desc ,'\0', ft0020_SM_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].sm_desc, sz_recvbufcat + gbp->count, ft0020_SM_DESC_LEN);
        gbp->count += ft0020_SM_DESC_LEN;

        gbp->count += ft0020_FILLER32_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].in_desc ,'\0', ft0020_IN_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].in_desc, sz_recvbufcat + gbp->count, ft0020_IN_DESC_LEN);
        gbp->count += ft0020_IN_DESC_LEN;

        gbp->count += ft0020_FILLER33_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].foc_date ,'\0', ft0020_DATE_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].foc_date, sz_recvbufcat + gbp->count, ft0020_DATE_LEN);
        gbp->count += ft0020_DATE_LEN;

        gbp->count += ft0020_FILLER34_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].codiv_name ,'\0', ft0020_CODIV_NAME_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].codiv_name, sz_recvbufcat + gbp->count, ft0020_CODIV_NAME_LEN);
        gbp->count += ft0020_CODIV_NAME_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].div_name ,'\0', ft0020_DIV_NAME_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].div_name, sz_recvbufcat + gbp->count, ft0020_DIV_NAME_LEN);
        gbp->count += ft0020_DIV_NAME_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].dept_name ,'\0', ft0020_DEPT_NAME_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].dept_name, sz_recvbufcat + gbp->count, ft0020_DEPT_NAME_LEN);
        gbp->count += ft0020_DEPT_NAME_LEN;

        gbp->count += ft0020_FILLER35_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].kit_desc ,'\0', ft0020_KIT_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].kit_desc, sz_recvbufcat + gbp->count, ft0020_KIT_DESC_LEN);
        gbp->count += ft0020_KIT_DESC_LEN;

        gbp->count += ft0020_FILLER36_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].vend_name ,'\0', ft0020_VEND_NAME_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].vend_name, sz_recvbufcat + gbp->count, ft0020_VEND_NAME_LEN);
        gbp->count += ft0020_VEND_NAME_LEN;

        gbp->count += ft0020_FILLER37_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].whse_desc ,'\0', ft0020_WHSE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].whse_desc, sz_recvbufcat + gbp->count, ft0020_WHSE_DESC_LEN);
        gbp->count += ft0020_WHSE_DESC_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].comment_desc ,'\0', ft0020_COMMENT_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].comment_desc, sz_recvbufcat + gbp->count, ft0020_COMMENT_DESC_LEN);
        gbp->count += ft0020_COMMENT_DESC_LEN;

        gbp->count += ft0020_FILLER38_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].chain_desc ,'\0', ft0020_CHAIN_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].chain_desc, sz_recvbufcat + gbp->count, ft0020_CHAIN_DESC_LEN);
        gbp->count += ft0020_CHAIN_DESC_LEN;

        gbp->count += ft0020_FILLER39_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].disc_desc ,'\0', ft0020_DISC_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].disc_desc, sz_recvbufcat + gbp->count, ft0020_DISC_DESC_LEN);
        gbp->count += ft0020_DISC_DESC_LEN;

        gbp->count += ft0020_FILLER40_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].offer_no ,'\0', ft0020_OFFER_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].offer_no, sz_recvbufcat + gbp->count, ft0020_OFFER_NO_LEN);
        gbp->count += ft0020_OFFER_NO_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].edp_no ,'\0', ft0020_EDP_NO_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].edp_no, sz_recvbufcat + gbp->count, ft0020_EDP_NO_LEN);
        gbp->count += ft0020_EDP_NO_LEN;

        gbp->count += ft0020_FILLER41_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].prod_desc ,'\0', ft0020_PROD_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].prod_desc, sz_recvbufcat + gbp->count, ft0020_PROD_DESC_LEN);
        gbp->count += ft0020_PROD_DESC_LEN;

        gbp->count += ft0020_FILLER42_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].letter_desc ,'\0', ft0020_LETTER_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].letter_desc, sz_recvbufcat + gbp->count, ft0020_LETTER_DESC_LEN);
        gbp->count += ft0020_LETTER_DESC_LEN;

        gbp->count += ft0020_FILLER43_LEN;

        memset(ptr_recvbuf->list_detail[gbp->i].style_desc ,'\0', ft0020_STYLE_DESC_LEN + 1);
        memcpy(ptr_recvbuf->list_detail[gbp->i].style_desc, sz_recvbufcat + gbp->count, ft0020_STYLE_DESC_LEN);
        gbp->count += ft0020_STYLE_DESC_LEN;

        gbp->count += ft0020_FILLER44_LEN;

    	ap_rprintf(r,"          <LIST_INFO>\n");
		ap_rprintf(r,"                  <LIST_SHORT>%s</LIST_SHORT>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].list_short));
		ap_rprintf(r,"                  <TITLE_CODE>%s</TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].title_code));
		ap_rprintf(r,"                  <STATE_CODE>%s</STATE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].state_code));
		ap_rprintf(r,"                  <COUNTRY_CODE>%s</COUNTRY_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].country_code));
		ap_rprintf(r,"                  <CODE_ID>%s</CODE_ID>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].code_id));
		ap_rprintf(r,"                  <STAND_ALONE>%s</STAND_ALONE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].stand_alone));
		ap_rprintf(r,"                  <DISCOUNT_CODE>%s</DISCOUNT_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].disc_code));
		ap_rprintf(r,"                  <DISCOUNT_TYPE>%s</DISCOUNT_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].disc_type));
		ap_rprintf(r,"                  <DISCOUNT_AMOUNT>%s</DISCOUNT_AMOUNT>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].disc_amt));
		ap_rprintf(r,"                  <CODIV>%s</CODIV>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].codiv));
		ap_rprintf(r,"                  <CUSTOMER_NUM>%s</CUSTOMER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].cust_no));
		ap_rprintf(r,"                  <PM_GROUP>%s</PM_GROUP>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].pm_group));
		ap_rprintf(r,"                  <PM_ID>%s</PM_ID>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].pm_id));
		ap_rprintf(r,"                  <SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].ship_method));
		ap_rprintf(r,"                  <IN_PLAN>%s</IN_PLAN>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].in_plan));
		ap_rprintf(r,"                  <KEY_LABEL>%s</KEY_LABEL>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].key_label));
		ap_rprintf(r,"                  <FOC_CODE>%s</FOC_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].foc_code));
		ap_rprintf(r,"                  <CP_SOURCE>%s</CP_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].cp_source));
		ap_rprintf(r,"                  <CODIV_NUM>%s</CODIV_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].codiv_no));
		ap_rprintf(r,"                  <DEPT_NUM>%s</DEPT_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].dept_num));
		ap_rprintf(r,"                  <KIT_HDR>%s</KIT_HDR>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].kit_hdr));
		ap_rprintf(r,"                  <PRINTER>%s</PRINTER>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].printer));
		ap_rprintf(r,"                  <RA_NUM>%s</RA_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].ra_no));
		ap_rprintf(r,"                  <VEND_NUM>%s</VEND_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].vend_no));
		ap_rprintf(r,"                  <WAREHOUSE_CODE>%s</WAREHOUSE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].whse_code));
		ap_rprintf(r,"                  <COMMNET_NUM>%s</COMMNET_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].comment_no));
		ap_rprintf(r,"                  <CHAIN_NUM>%s</CHAIN_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].chain_no));
		ap_rprintf(r,"                  <DISCOUNT_TABLE>%s</DISCOUNT_TABLE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].disc_table));
		ap_rprintf(r,"                  <ITEM_NUM>%s</ITEM_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].item_no));
		ap_rprintf(r,"                  <RES_LOC>%s</RES_LOC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].res_loc));
		ap_rprintf(r,"                  <PRODUCT_NUM>%s</PRODUCT_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].prod_no));
		ap_rprintf(r,"                  <CUSTOM_CODE>%s</CUSTOM_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].custom_codes));
		ap_rprintf(r,"                  <LETTER_CODE>%s</LETTER_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].letter_code));
		ap_rprintf(r,"                  <STYLE_TABLE>%s</STYLE_TABLE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].style_table));
		ap_rprintf(r,"                  <CT>%s</CT>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].ct));
		ap_rprintf(r,"                  <LIST_LONG>%s</LIST_LONG>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].list_long));
		ap_rprintf(r,"                  <TITLE>%s</TITLE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].title));
		ap_rprintf(r,"                  <COUNTRY>%s</COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].country));
		ap_rprintf(r,"                  <CODE_ABRV>%s</CODE_ABRV>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].code_abrv));
		ap_rprintf(r,"                  <CODE_SPEC>%s</CODE_SPEC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].code_spec));
		ap_rprintf(r,"                  <COMP_DESC>%s</COMP_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].comp_desc));
		ap_rprintf(r,"                  <PM_DESC>%s</PM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].pm_desc));
		ap_rprintf(r,"                  <SM_DESC>%s</SM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].sm_desc));
		ap_rprintf(r,"                  <IN_DESC>%s</IN_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].in_desc));
		ap_rprintf(r,"                  <FOC_DATE>%s</FOC_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].foc_date));
		ap_rprintf(r,"                  <CODIV_NAME>%s</CODIV_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].codiv_name));
		ap_rprintf(r,"                  <DIVISION_NAME>%s</DIVISION_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].div_name));
		ap_rprintf(r,"                  <DEPT_NAME>%s</DEPT_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].dept_name));
		ap_rprintf(r,"                  <KIT_DESC>%s</KIT_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].kit_desc));
		ap_rprintf(r,"                  <VEND_NAME>%s</VEND_NAME>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].vend_name));
		ap_rprintf(r,"                  <WAREHOUSE_DESC>%s</WAREHOUSE_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].whse_desc));
		ap_rprintf(r,"                  <COMMENT_DESC>%s</COMMENT_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].comment_desc));
		ap_rprintf(r,"                  <CHAIN_DESC>%s</CHAIN_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].chain_desc));
		ap_rprintf(r,"                  <DISCOUNT_DESC>%s</DISCOUNT_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].disc_desc));
		ap_rprintf(r,"                  <OFFER_NUM>%s</OFFER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].offer_no));
		ap_rprintf(r,"                  <EDP_NUM>%s</EDP_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].edp_no));
		ap_rprintf(r,"                  <PRODUCT_DESC>%s</PRODUCT_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].prod_desc));
		ap_rprintf(r,"                  <LETTER_DESC>%s</LETTER_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].letter_desc));
		ap_rprintf(r,"                  <STYLE_DESC>%s</STYLE_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->list_detail[gbp->i].style_desc));
    	ap_rprintf(r,"          </LIST_INFO>\n");

	}


    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0020>\n", gbp->ft_betag);


    return(0);
}

