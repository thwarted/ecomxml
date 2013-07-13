/* ft0075_defs.h  Review Custom Kit */


#define  ft0075_LAN_SEND_BUF_LEN            115
#define  ft0075_LAN_RECV_BUF_LEN            3615

/* Web to MACS Buffer */

#define  ft0075_REQ_ID_LEN                 4
#define  ft0075_REC_ID_LEN                 4
#define  ft0075_COMPANY_LEN                2
#define  ft0075_DIVISION_LEN               2
#define  ft0075_USER_ID_LEN                16
#define  ft0075_IP_ADDR_LEN                16
#define  ft0075_FILLER_LEN                 25
#define  ft0075_ID_LEN                     16
#define  ft0075_ITM_NO_LEN                 20
#define  ft0075_LINE_NO_LEN                4
#define  ft0075_FLAG_LEN                   1


/* MACS to Web Buffer */

#define  ft0075_REQ_ID_LEN                 4
#define  ft0075_REC_ID_LEN                 4
#define  ft0075_USER_ID_LEN                16
#define  ft0075_SUCCESS_FLAG_LEN           1
#define  ft0075_ERR_LEN                    80
#define  ft0075_SEND_FILLER_LEN            45
#define  ft0075_CUST_EDP_LEN               9
#define  ft0075_ITM_CNT_LEN                3
#define  ft0075_ITM_EDP_LEN                9
#define  ft0075_ITM_NO_LEN                 20
#define  ft0075_QTY_LEN                    4
#define  ft0075_DESC_LEN                   50
#define  ft0075_PRICE_LEN                  9
#define  ft0075_FLAG_LEN                   1
#define  ft0075_CUSTOM_LEN                 60
#define  ft0075_CUSTOM_LOOP                25
#define  ft0075_R1_LOOP                    25


typedef struct
{
    char request_id[ft0075_REQ_ID_LEN+1];
    char record_id[ft0075_REC_ID_LEN+1];
    char company[ft0075_COMPANY_LEN+1];
    char division[ft0075_DIVISION_LEN+1];
    char userid[ft0075_USER_ID_LEN+1];
    char ip_address[ft0075_IP_ADDR_LEN+1];
    char filler[ft0075_FILLER_LEN+1];
	char wl_company[ft0075_COMPANY_LEN+1];
	char wl_division[ft0075_DIVISION_LEN+1];
	char wl_id[ft0075_ID_LEN+1];
    char item_no[ft0075_ITM_NO_LEN+1];
    char line_no[ft0075_LINE_NO_LEN+1];
	char custom_flag[ft0075_FLAG_LEN+1];
	char kit_flag[ft0075_FLAG_LEN+1];

} ft0075_st_send;


typedef struct
{
    char custom[ft0075_CUSTOM_LEN+1];

} customization_75;

typedef struct
{
    char r1_item_no[ft0075_ITM_NO_LEN+1];
    char r1_item_desc[ft0075_DESC_LEN+1];
    char r1_item_qty[ft0075_QTY_LEN+1];

} r1_comp_75;


typedef struct
{
    char request_id[ft0075_REQ_ID_LEN+1];
    char record_id[ft0075_REC_ID_LEN+1];
    char userid[ft0075_USER_ID_LEN+1];
    char success_flag[ft0075_SUCCESS_FLAG_LEN+1];
    char err_message[ft0075_ERR_LEN+1];
    char filler[ft0075_SEND_FILLER_LEN+1];
	char cust_edp[ft0075_CUST_EDP_LEN+1];
	char item_cnt[ft0075_ITM_CNT_LEN+1];
	char item_edp[ft0075_ITM_EDP_LEN+1];
	char item_no[ft0075_ITM_NO_LEN+1];
	char qty[ft0075_QTY_LEN+1];
	char desc[ft0075_DESC_LEN+1];
	char price[ft0075_PRICE_LEN+1];
	char custom_flag[ft0075_FLAG_LEN+1];
	customization_75 customization_line[ft0075_CUSTOM_LOOP];
	r1_comp_75 r1_comp_detail[ft0075_R1_LOOP];

} ft0075_st_recv;


