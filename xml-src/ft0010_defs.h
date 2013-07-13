/* ft0010_defs.h  Add comments */


#define  ft0010_LAN_SEND_BUF_LEN            4655
#define  ft0010_LAN_RECV_BUF_LEN            160


/* Web to MACS Buffer */

#define  ft0010_REQ_ID_LEN                 4
#define  ft0010_REC_ID_LEN                 4
#define  ft0010_COMPANY_LEN                2
#define  ft0010_DIVISION_LEN               2
#define  ft0010_USER_ID_LEN                16
#define  ft0010_IP_ADDR_LEN                16
#define  ft0010_FILLER_LEN                 25
#define  ft0010_CUST_NO_LEN                10
#define  ft0010_ORDER_NO_LEN               12
#define  ft0010_CALLED_LEN                 2
#define  ft0010_COMMENT_LEN                60
#define  ft0010_FLAG_LEN                   1
#define  ft0010_DATE_LEN                   8
#define  ft0010_OPER_LEN                   8
#define  ft0010_TYPE_LEN                   2
#define  ft0010_SEQ_NO_LEN                 4
#define  ft0010_REC_NO_LEN                 9
#define  ft0010_COMMENT_LOOP               48  



/* MACS to Web Buffer */

#define  ft0010_REQ_ID_LEN                 4
#define  ft0010_REC_ID_LEN                 4
#define  ft0010_USER_ID_LEN                16
#define  ft0010_SUCCESS_FLAG_LEN           1
#define  ft0010_ERR_LEN                    80
#define  ft0010_SEND_FILLER_LEN            45


typedef struct
{
    char request_id[ft0010_REQ_ID_LEN+1];
    char record_id[ft0010_REC_ID_LEN+1];
    char company[ft0010_COMPANY_LEN+1];
    char division[ft0010_DIVISION_LEN+1];
    char userid[ft0010_USER_ID_LEN+1];
    char ip_address[ft0010_IP_ADDR_LEN+1];
    char filler[ft0010_FILLER_LEN+1];
	char cust_no[ft0010_CUST_NO_LEN+1];
	char full_order_no[ft0010_ORDER_NO_LEN+1];
	char who_called[ft0010_CALLED_LEN+1];
	char why_called[ft0010_CALLED_LEN+1];
    char comment[ft0010_COMMENT_LOOP][ft0010_COMMENT_LEN+1];
    char print_pks[ft0010_COMMENT_LOOP][ft0010_FLAG_LEN+1];
    char print_inv[ft0010_COMMENT_LOOP][ft0010_FLAG_LEN+1];
    char print_lbl[ft0010_COMMENT_LOOP][ft0010_FLAG_LEN+1];
    char print_whs[ft0010_COMMENT_LOOP][ft0010_FLAG_LEN+1];
    char ci_date[ft0010_COMMENT_LOOP][ft0010_DATE_LEN+1];
    char ci_oper[ft0010_COMMENT_LOOP][ft0010_OPER_LEN+1];
    char ci_type[ft0010_COMMENT_LOOP][ft0010_TYPE_LEN+1];
    char seq_no[ft0010_COMMENT_LOOP][ft0010_SEQ_NO_LEN+1];
    char record_no[ft0010_COMMENT_LOOP][ft0010_REC_NO_LEN+1];

} ft0010_st_send;


typedef struct
{
    char request_id[ft0010_REQ_ID_LEN+1];
    char record_id[ft0010_REC_ID_LEN+1];
    char userid[ft0010_USER_ID_LEN+1];
    char success_flag[ft0010_SUCCESS_FLAG_LEN+1];
    char err_message[ft0010_ERR_LEN+1];
    char filler[ft0010_SEND_FILLER_LEN+1];

} ft0010_st_recv;


