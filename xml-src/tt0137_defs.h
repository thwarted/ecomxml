/* tt0137_defs.h  Shopping Request */


#define  tt0137_LAN_SEND_BUF_LEN          74           
#define  tt0137_LAN_RECV_BUF_LEN          180   


/* Web to MACS Buffer */

#define  tt0137_REQ_ID_LEN                 4
#define  tt0137_REC_ID_LEN                 4
#define  tt0137_COMPANY_LEN                2
#define  tt0137_DIVISION_LEN               2
#define  tt0137_USER_ID_LEN                16
#define  tt0137_IP_ADDR_LEN                16
#define  tt0137_FILLER_LEN                 25
#define  tt0137_FLAG_LEN                   1

/* MACS to Web Buffer */

#define  tt0137_REQ_ID_LEN                 4
#define  tt0137_REC_ID_LEN                 4
#define  tt0137_USER_ID_LEN                16
#define  tt0137_SUCCESS_FLAG_LEN           1
#define  tt0137_ERR_LEN                    80
#define  tt0137_SEND_FILLER_LEN            45
#define  tt0137_DUP_CART_ID_LEN            16

typedef struct
{
    char request_id[tt0137_REQ_ID_LEN+1];
    char record_id[tt0137_REC_ID_LEN+1];
    char company[tt0137_COMPANY_LEN+1];
    char division[tt0137_DIVISION_LEN+1];
    char userid[tt0137_USER_ID_LEN+1];
    char ip_address[tt0137_IP_ADDR_LEN+1];
    char filler[tt0137_FILLER_LEN+1];
	char custom_flag[tt0137_FLAG_LEN+1];
	char shipto_flag[tt0137_FLAG_LEN+1];
	char giftwrap_flag[tt0137_FLAG_LEN+1];
	char source_flag[tt0137_FLAG_LEN+1];
	char comment_flag[tt0137_FLAG_LEN+1];

} tt0137_st_send;

typedef struct
{
    char request_id[tt0137_REQ_ID_LEN+1];
    char record_id[tt0137_REC_ID_LEN+1];
    char userid[tt0137_USER_ID_LEN+1];
    char success_flag[tt0137_SUCCESS_FLAG_LEN+1];
    char err_message[tt0137_ERR_LEN+1];
    char filler[tt0137_SEND_FILLER_LEN+1];
    char dup_cartid[tt0137_DUP_CART_ID_LEN+1];

} tt0137_st_recv;