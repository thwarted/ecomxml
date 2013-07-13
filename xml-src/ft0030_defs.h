/* ft0030_defs.h  Get Customization */


#define  ft0030_LAN_SEND_BUF_LEN            115
#define  ft0030_LAN_RECV_BUF_LEN            1700


/* Web to MACS Buffer */

#define  ft0030_REQ_ID_LEN                 4
#define  ft0030_REC_ID_LEN                 4
#define  ft0030_COMPANY_LEN                2
#define  ft0030_DIVISION_LEN               2
#define  ft0030_USER_ID_LEN                16
#define  ft0030_IP_ADDR_LEN                16
#define  ft0030_FILLER_LEN                 25
#define  ft0030_ORDER_LEN                  8
#define  ft0030_LINE_LEN                   4
#define  ft0030_ITEM_NO_LEN                20
#define  ft0030_CUST_NO_LEN                10
#define  ft0030_CUST_CODE_LEN              4


/* MACS to Web Buffer */

#define  ft0030_REQ_ID_LEN                 4
#define  ft0030_REC_ID_LEN                 4
#define  ft0030_USER_ID_LEN                16
#define  ft0030_SUCCESS_FLAG_LEN           1
#define  ft0030_ERR_LEN                    80
#define  ft0030_SEND_FILLER_LEN            45
#define  ft0030_CUST_CNT_LEN               3
#define  ft0030_PROMPT_LEN                 20
#define  ft0030_LENGTH_LEN                 2
#define  ft0030_VALUE_LEN                  55
#define  ft0030_CUSTOMIZATION_LOOP         20



typedef struct
{
    char request_id[ft0030_REQ_ID_LEN+1];
    char record_id[ft0030_REC_ID_LEN+1];
    char company[ft0030_COMPANY_LEN+1];
    char division[ft0030_DIVISION_LEN+1];
    char userid[ft0030_USER_ID_LEN+1];
    char ip_address[ft0030_IP_ADDR_LEN+1];
    char filler[ft0030_FILLER_LEN+1];
	char order_no[ft0030_ORDER_LEN+1];
	char line_no[ft0030_LINE_LEN+1];
	char cust_no[ft0030_CUST_NO_LEN+1];
	char item_no[ft0030_ITEM_NO_LEN+1];
	char cust_code[ft0030_CUST_CODE_LEN+1];

} ft0030_st_send;

typedef struct
{
    char prompt[ft0030_PROMPT_LEN+1];
    char length[ft0030_LENGTH_LEN+1];
    char value[ft0030_VALUE_LEN+1];

} customization_30;


typedef struct
{
    char request_id[ft0030_REQ_ID_LEN+1];
    char record_id[ft0030_REC_ID_LEN+1];
    char userid[ft0030_USER_ID_LEN+1];
    char success_flag[ft0030_SUCCESS_FLAG_LEN+1];
    char err_message[ft0030_ERR_LEN+1];
    char filler[ft0030_SEND_FILLER_LEN+1];
	char cust_cnt[ft0030_CUST_CNT_LEN+1];
	customization_30 customization_detail[ft0030_CUSTOMIZATION_LOOP];

} ft0030_st_recv;


