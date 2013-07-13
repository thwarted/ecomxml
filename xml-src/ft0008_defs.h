/* ft0008_defs.h  Get Comments */


#define  ft0008_LAN_SEND_BUF_LEN            103
#define  ft0008_LAN_RECV_BUF_LEN            4740


/* Web to MACS Buffer */

#define  ft0008_REQ_ID_LEN                 4
#define  ft0008_REC_ID_LEN                 4
#define  ft0008_COMPANY_LEN                2
#define  ft0008_DIVISION_LEN               2
#define  ft0008_USER_ID_LEN                16
#define  ft0008_IP_ADDR_LEN                16
#define  ft0008_FILLER_LEN                 25
#define  ft0008_CUST_NO_LEN                10
#define  ft0008_ORDER_NO_LEN               12
#define  ft0008_PAGE_NO_LEN                4
#define  ft0008_PAGE_CNT_LEN               4
#define  ft0008_CALLED_LEN                 2


/* MACS to Web Buffer */

#define  ft0008_REQ_ID_LEN                 4
#define  ft0008_REC_ID_LEN                 4
#define  ft0008_USER_ID_LEN                16
#define  ft0008_SUCCESS_FLAG_LEN           1
#define  ft0008_ERR_LEN                    80
#define  ft0008_SEND_FILLER_LEN            45
#define  ft0008_COMM_CNT_LEN               3
#define  ft0008_PAGE_NO_LEN                4 
#define  ft0008_FILLER1_LEN                2
#define  ft0008_COMMENT_LEN                60
#define  ft0008_FLAG_LEN                   1
#define  ft0008_DATE_LEN                   8
#define  ft0008_OPER_LEN                   8
#define  ft0008_TYPE_LEN                   8
#define  ft0008_SEQ_NO_LEN                 4
#define  ft0008_REC_NO_LEN                 9
#define  ft0008_COMMENT_LOOP               48  

typedef struct
{
    char request_id[ft0008_REQ_ID_LEN+1];
    char record_id[ft0008_REC_ID_LEN+1];
    char company[ft0008_COMPANY_LEN+1];
    char division[ft0008_DIVISION_LEN+1];
    char userid[ft0008_USER_ID_LEN+1];
    char ip_address[ft0008_IP_ADDR_LEN+1];
    char filler[ft0008_FILLER_LEN+1];
	char cust_no[ft0008_CUST_NO_LEN+1];
	char full_order_no[ft0008_ORDER_NO_LEN+1];
	char page_no[ft0008_PAGE_NO_LEN+1];
	char page_cnt[ft0008_PAGE_CNT_LEN+1];
	char who_called[ft0008_CALLED_LEN+1];
	char why_called[ft0008_CALLED_LEN+1];

} ft0008_st_send;

typedef struct
{
    char comment[ft0008_COMMENT_LEN+1];
    char print_pks[ft0008_FLAG_LEN+1];
    char print_inv[ft0008_FLAG_LEN+1];
    char print_lbl[ft0008_FLAG_LEN+1];
    char print_whs[ft0008_FLAG_LEN+1];
    char ci_date[ft0008_DATE_LEN+1];
    char ci_oper[ft0008_OPER_LEN+1];
    char ci_type[ft0008_TYPE_LEN+1];
    char seq_no[ft0008_SEQ_NO_LEN+1];
    char record_no[ft0008_REC_NO_LEN+1];

} comment_08;

typedef struct
{
    char request_id[ft0008_REQ_ID_LEN+1];
    char record_id[ft0008_REC_ID_LEN+1];
    char userid[ft0008_USER_ID_LEN+1];
    char success_flag[ft0008_SUCCESS_FLAG_LEN+1];
    char err_message[ft0008_ERR_LEN+1];
    char filler[ft0008_SEND_FILLER_LEN+1];
    char comment_cnt[ft0008_COMM_CNT_LEN+1];
    char page_no[ft0008_PAGE_NO_LEN+1];
    char filler1[ft0008_FILLER1_LEN+1];
	comment_08 comment_detail[ft0008_COMMENT_LOOP];

} ft0008_st_recv;


