/* ft0007_defs.h  Send Letters */


#define  ft0007_LAN_SEND_BUF_LEN            97
#define  ft0007_LAN_RECV_BUF_LEN            160


/* Web to MACS Buffer */

#define  ft0007_REQ_ID_LEN                 4
#define  ft0007_REC_ID_LEN                 4
#define  ft0007_COMPANY_LEN                2
#define  ft0007_DIVISION_LEN               2
#define  ft0007_USER_ID_LEN                16
#define  ft0007_IP_ADDR_LEN                16
#define  ft0007_FILLER_LEN                 25
#define  ft0007_CUST_NO_LEN                10
#define  ft0007_ORDER_NO_LEN               12
#define  ft0007_LETTER_CODE_LEN            6


/* MACS to Web Buffer */

#define  ft0007_REQ_ID_LEN                 4
#define  ft0007_REC_ID_LEN                 4
#define  ft0007_USER_ID_LEN                16
#define  ft0007_SUCCESS_FLAG_LEN           1
#define  ft0007_ERR_LEN                    80
#define  ft0007_SEND_FILLER_LEN            45


typedef struct
{
    char request_id[ft0007_REQ_ID_LEN+1];
    char record_id[ft0007_REC_ID_LEN+1];
    char company[ft0007_COMPANY_LEN+1];
    char division[ft0007_DIVISION_LEN+1];
    char userid[ft0007_USER_ID_LEN+1];
    char ip_address[ft0007_IP_ADDR_LEN+1];
    char filler[ft0007_FILLER_LEN+1];
	char cust_no[ft0007_CUST_NO_LEN+1];
	char full_order_no[ft0007_ORDER_NO_LEN+1];
	char letter_code[ft0007_LETTER_CODE_LEN+1];

} ft0007_st_send;


typedef struct
{
    char request_id[ft0007_REQ_ID_LEN+1];
    char record_id[ft0007_REC_ID_LEN+1];
    char userid[ft0007_USER_ID_LEN+1];
    char success_flag[ft0007_SUCCESS_FLAG_LEN+1];
    char err_message[ft0007_ERR_LEN+1];
    char filler[ft0007_SEND_FILLER_LEN+1];

} ft0007_st_recv;


