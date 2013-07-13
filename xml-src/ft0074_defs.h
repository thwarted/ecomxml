/* ft0074_defs.h  Edit Wishlist */


#define  ft0074_LAN_SEND_BUF_LEN            1940
#define  ft0074_LAN_RECV_BUF_LEN            160

/* Web to MACS Buffer */

#define  ft0074_REQ_ID_LEN                 4
#define  ft0074_REC_ID_LEN                 4
#define  ft0074_COMPANY_LEN                2
#define  ft0074_DIVISION_LEN               2
#define  ft0074_USER_ID_LEN                16
#define  ft0074_IP_ADDR_LEN                16
#define  ft0074_FILLER_LEN                 25
#define  ft0074_ID_LEN                     16
#define  ft0074_ITM_EDP_LEN                9
#define  ft0074_ITM_NO_LEN                 20
#define  ft0074_LINE_NO_LEN                4
#define  ft0074_QTY_LEN                    4
#define  ft0074_FLAG_LEN                   1
#define  ft0074_EW_LOOP                    50


/* MACS to Web Buffer */

#define  ft0074_REQ_ID_LEN                 4
#define  ft0074_REC_ID_LEN                 4
#define  ft0074_USER_ID_LEN                16
#define  ft0074_SUCCESS_FLAG_LEN           1
#define  ft0074_ERR_LEN                    80
#define  ft0074_SEND_FILLER_LEN            45




typedef struct
{
    char request_id[ft0074_REQ_ID_LEN+1];
    char record_id[ft0074_REC_ID_LEN+1];
    char company[ft0074_COMPANY_LEN+1];
    char division[ft0074_DIVISION_LEN+1];
    char userid[ft0074_USER_ID_LEN+1];
    char ip_address[ft0074_IP_ADDR_LEN+1];
    char filler[ft0074_FILLER_LEN+1];
	char wl_company[ft0074_COMPANY_LEN+1];
	char wl_division[ft0074_DIVISION_LEN+1];
	char wl_id[ft0074_ID_LEN+1];
    char ew_item_edp[ft0074_EW_LOOP][ft0074_ITM_EDP_LEN+1];
    char ew_item_no[ft0074_EW_LOOP][ft0074_ITM_NO_LEN+1];
    char ew_line_no[ft0074_EW_LOOP][ft0074_LINE_NO_LEN+1];
    char ew_qty[ft0074_EW_LOOP][ft0074_QTY_LEN+1];
	char delete_list[ft0074_FLAG_LEN+1];

} ft0074_st_send;



typedef struct
{
    char request_id[ft0074_REQ_ID_LEN+1];
    char record_id[ft0074_REC_ID_LEN+1];
    char userid[ft0074_USER_ID_LEN+1];
    char success_flag[ft0074_SUCCESS_FLAG_LEN+1];
    char err_message[ft0074_ERR_LEN+1];
    char filler[ft0074_SEND_FILLER_LEN+1];
	char delete_all_item[ft0074_FLAG_LEN+1];

} ft0074_st_recv;


