/* ft0071_defs.h  Custom letter */


#define  ft0071_LAN_SEND_BUF_LEN            105
#define  ft0071_LAN_RECV_BUF_LEN            26560

/* Web to MACS Buffer */

#define  ft0071_REQ_ID_LEN                 4
#define  ft0071_REC_ID_LEN                 4
#define  ft0071_COMPANY_LEN                2
#define  ft0071_DIVISION_LEN               2
#define  ft0071_USER_ID_LEN                16
#define  ft0071_IP_ADDR_LEN                16
#define  ft0071_FILLER_LEN                 25
#define  ft0071_PAGE_NO_LEN                4
#define  ft0071_PAGE_CNT_LEN               4
#define  ft0071_LETTER_NO_LEN              6
#define  ft0071_CUST_NO_LEN                10
#define  ft0071_ORDER_LEN                  12


/* MACS to Web Buffer */

#define  ft0071_REQ_ID_LEN                 4
#define  ft0071_REC_ID_LEN                 4
#define  ft0071_USER_ID_LEN                16
#define  ft0071_SUCCESS_FLAG_LEN           1
#define  ft0071_ERR_LEN                    80
#define  ft0071_SEND_FILLER_LEN            45
#define  ft0071_LINE_CNT_LEN               3
#define  ft0071_LINE_LEN                   132
#define  ft0071_LINE_LOOP                  200



typedef struct
{
    char request_id[ft0071_REQ_ID_LEN+1];
    char record_id[ft0071_REC_ID_LEN+1];
    char company[ft0071_COMPANY_LEN+1];
    char division[ft0071_DIVISION_LEN+1];
    char userid[ft0071_USER_ID_LEN+1];
    char ip_address[ft0071_IP_ADDR_LEN+1];
    char filler[ft0071_FILLER_LEN+1];
	char page_no[ft0071_PAGE_NO_LEN+1];
	char page_cnt[ft0071_PAGE_CNT_LEN+1];
	char letter_no[ft0071_LETTER_NO_LEN+1];
	char cust_no[ft0071_CUST_NO_LEN+1];
	char full_order_no[ft0071_ORDER_LEN+1];

} ft0071_st_send;

typedef struct
{
    char line[ft0071_LINE_LEN+1];
    
} customization_71;


typedef struct
{
    char request_id[ft0071_REQ_ID_LEN+1];
    char record_id[ft0071_REC_ID_LEN+1];
    char userid[ft0071_USER_ID_LEN+1];
    char success_flag[ft0071_SUCCESS_FLAG_LEN+1];
    char err_message[ft0071_ERR_LEN+1];
    char filler[ft0071_SEND_FILLER_LEN+1];
    char line_cnt[ft0071_LINE_CNT_LEN+1];
	customization_71 customization_line[ft0071_LINE_LOOP];

} ft0071_st_recv;


