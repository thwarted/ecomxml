/* tt0103_defs.h BackOrder Cancel */


#define  tt0103_LAN_SEND_BUF_LEN        85   
#define  tt0103_LAN_RECV_BUF_LEN        150 

/* Web to MACS Buffer  */

#define  tt0103_REQ_ID_LEN             4
#define  tt0103_REC_ID_LEN             4
#define  tt0103_COMPANY_LEN            2
#define  tt0103_DIVISION_LEN           2
#define  tt0103_USER_ID_LEN            16
#define  tt0103_IP_ADDR_LEN            16
#define  tt0103_FILLER_LEN             25

#define  tt0103_ORDER_NO_LEN           8
#define  tt0103_FLAG_LEN               1
#define  tt0103_CANCEL_LEVEL_LEN       4
#define  tt0103_CANCEL_ITEM_LEN        3

/* MACS to Web Buffer   */

#define  tt0103_REQ_ID_LEN             4
#define  tt0103_REC_ID_LEN             4
#define  tt0103_USER_ID_LEN            16
#define  tt0103_SUCCESS_FLAG_LEN       1
#define  tt0103_ERR_LEN                80
#define  tt0103_SEND_FILLER_LEN        45




typedef struct
{
    char request_id[tt0103_REQ_ID_LEN+1];
    char record_id[tt0103_REC_ID_LEN+1];
    char company[tt0103_COMPANY_LEN+1];
    char division[tt0103_DIVISION_LEN+1];
    char userid[tt0103_USER_ID_LEN+1];
    char ip_address[tt0103_IP_ADDR_LEN+1];
    char filler[tt0103_FILLER_LEN+1];
	char order_no[tt0103_ORDER_NO_LEN+1];
	char whole_order_flag[tt0103_FLAG_LEN+1];
	char cancel_level[tt0103_CANCEL_LEVEL_LEN+1];
	char cancel_item[tt0103_CANCEL_ITEM_LEN+1];
    
} tt0103_st_send;




typedef struct
{
    char request_id[tt0103_REQ_ID_LEN+1];
    char record_id[tt0103_REC_ID_LEN+1];
    char userid[tt0103_USER_ID_LEN+1];
    char success[tt0103_SUCCESS_FLAG_LEN+1];
    char err_message[tt0103_ERR_LEN+1];
    char filler[tt0103_SEND_FILLER_LEN+1];

} tt0103_st_recv;
