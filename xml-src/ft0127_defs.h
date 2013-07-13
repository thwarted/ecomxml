/* ft0127_defs.h  Write Order Action */


#define  ft0127_LAN_SEND_BUF_LEN            143
#define  ft0127_LAN_RECV_BUF_LEN            160

/* Web to MACS Buffer */

#define  ft0127_REQ_ID_LEN                 4
#define  ft0127_REC_ID_LEN                 4
#define  ft0127_COMPANY_LEN                2
#define  ft0127_DIVISION_LEN               2
#define  ft0127_USER_ID_LEN                16
#define  ft0127_IP_ADDR_LEN                16
#define  ft0127_FILLER_LEN                 25
#define  ft0127_ORDER_LEN                  12
#define  ft0127_MSG_LEN                    6
#define  ft0127_ACTION_LEN                 40
#define  ft0127_DATE_LEN                   8


/* MACS to Web Buffer */

#define  ft0127_REQ_ID_LEN                 4
#define  ft0127_REC_ID_LEN                 4
#define  ft0127_USER_ID_LEN                16
#define  ft0127_SUCCESS_FLAG_LEN           1
#define  ft0127_ERR_LEN                    80
#define  ft0127_SEND_FILLER_LEN            45


typedef struct
{
    char request_id[ft0127_REQ_ID_LEN+1];
    char record_id[ft0127_REC_ID_LEN+1];
    char company[ft0127_COMPANY_LEN+1];
    char division[ft0127_DIVISION_LEN+1];
    char userid[ft0127_USER_ID_LEN+1];
    char ip_address[ft0127_IP_ADDR_LEN+1];
    char filler[ft0127_FILLER_LEN+1];
	char full_order_no[ft0127_ORDER_LEN+1];
	char message_cd[ft0127_MSG_LEN+1];
	char action_cd[ft0127_ACTION_LEN+1];
    char entry_date[ft0127_DATE_LEN+1];
    char action_date[ft0127_DATE_LEN+1];

} ft0127_st_send;



typedef struct
{
    char request_id[ft0127_REQ_ID_LEN+1];
    char record_id[ft0127_REC_ID_LEN+1];
    char userid[ft0127_USER_ID_LEN+1];
    char success_flag[ft0127_SUCCESS_FLAG_LEN+1];
    char err_message[ft0127_ERR_LEN+1];
    char filler[ft0127_SEND_FILLER_LEN+1];

} ft0127_st_recv;


