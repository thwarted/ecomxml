/* ft0006_defs.h  List Letters */


#define  ft0006_LAN_SEND_BUF_LEN            77
#define  ft0006_LAN_RECV_BUF_LEN            2770


/* Web to MACS Buffer */

#define  ft0006_REQ_ID_LEN                 4
#define  ft0006_REC_ID_LEN                 4
#define  ft0006_COMPANY_LEN                2
#define  ft0006_DIVISION_LEN               2
#define  ft0006_USER_ID_LEN                16
#define  ft0006_IP_ADDR_LEN                16
#define  ft0006_FILLER_LEN                 25
#define  ft0006_PAGE_NO_LEN                4
#define  ft0006_PAGE_CNT_LEN               4


/* MACS to Web Buffer */

#define  ft0006_REQ_ID_LEN                 4
#define  ft0006_REC_ID_LEN                 4
#define  ft0006_USER_ID_LEN                16
#define  ft0006_SUCCESS_FLAG_LEN           1
#define  ft0006_ERR_LEN                    80
#define  ft0006_SEND_FILLER_LEN            45
#define  ft0006_CNT_LEN                    3
#define  ft0006_PAGE_NO_LEN                4
#define  ft0006_CODE_LEN                   6
#define  ft0006_DESC_LEN                   20
#define  ft0006_LETTER_LOOP                100


typedef struct
{
    char request_id[ft0006_REQ_ID_LEN+1];
    char record_id[ft0006_REC_ID_LEN+1];
    char company[ft0006_COMPANY_LEN+1];
    char division[ft0006_DIVISION_LEN+1];
    char userid[ft0006_USER_ID_LEN+1];
    char ip_address[ft0006_IP_ADDR_LEN+1];
    char filler[ft0006_FILLER_LEN+1];
	char page_no[ft0006_PAGE_NO_LEN+1];
	char page_cnt[ft0006_PAGE_CNT_LEN+1];

} ft0006_st_send;

typedef struct
{
    char corr_code[ft0006_CODE_LEN+1];
    char letter_desc[ft0006_DESC_LEN+1];

} letter_06;

typedef struct
{
    char request_id[ft0006_REQ_ID_LEN+1];
    char record_id[ft0006_REC_ID_LEN+1];
    char userid[ft0006_USER_ID_LEN+1];
    char success_flag[ft0006_SUCCESS_FLAG_LEN+1];
    char err_message[ft0006_ERR_LEN+1];
    char filler[ft0006_SEND_FILLER_LEN+1];
	char letter_cnt[ft0006_CNT_LEN+1];
	char page_no[ft0006_PAGE_NO_LEN+1];
	letter_06 letter_detail[ft0006_LETTER_LOOP];

} ft0006_st_recv;


