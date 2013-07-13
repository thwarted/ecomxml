/* ft0025_defs.h  Get Upsell */


#define  ft0025_LAN_SEND_BUF_LEN            119
#define  ft0025_LAN_RECV_BUF_LEN            1360


/* Web to MACS Buffer */

#define  ft0025_REQ_ID_LEN                 4
#define  ft0025_REC_ID_LEN                 4
#define  ft0025_COMPANY_LEN                2
#define  ft0025_DIVISION_LEN               2
#define  ft0025_USER_ID_LEN                16
#define  ft0025_IP_ADDR_LEN                16
#define  ft0025_FILLER_LEN                 25
#define  ft0025_TYPE_LEN                   2
#define  ft0025_OFFER_LEN                  8
#define  ft0025_ITEM_NO_LEN                20
#define  ft0025_CUST_NO_LEN                10
#define  ft0025_CALLED_LEN                 8


/* MACS to Web Buffer */

#define  ft0025_REQ_ID_LEN                 4
#define  ft0025_REC_ID_LEN                 4
#define  ft0025_USER_ID_LEN                16
#define  ft0025_SUCCESS_FLAG_LEN           1
#define  ft0025_ERR_LEN                    80
#define  ft0025_SEND_FILLER_LEN            45
#define  ft0025_UPSELL_LINE_LEN            80
#define  ft0025_ALT_LEN                    9
#define  ft0025_ADD_LEN                    9
#define  ft0025_WEB_LEN                    9
#define  ft0025_ITEM_NO_LEN                20
#define  ft0025_DESC_LEN                   50
#define  ft0025_UPSELL_LOOP                10 



typedef struct
{
    char request_id[ft0025_REQ_ID_LEN+1];
    char record_id[ft0025_REC_ID_LEN+1];
    char company[ft0025_COMPANY_LEN+1];
    char division[ft0025_DIVISION_LEN+1];
    char userid[ft0025_USER_ID_LEN+1];
    char ip_address[ft0025_IP_ADDR_LEN+1];
    char filler[ft0025_FILLER_LEN+1];
	char upsell_type[ft0025_TYPE_LEN+1];
	char offer_no[ft0025_OFFER_LEN+1];
	char item_no[ft0025_ITEM_NO_LEN+1];
	char upsell_key_type[ft0025_TYPE_LEN+1];
	char cust_no[ft0025_CUST_NO_LEN+1];
	char called_by[ft0025_CALLED_LEN+1];

} ft0025_st_send;

typedef struct
{
    char upsell_line[ft0025_UPSELL_LINE_LEN+1];

} upsell_25;


typedef struct
{
    char request_id[ft0025_REQ_ID_LEN+1];
    char record_id[ft0025_REC_ID_LEN+1];
    char userid[ft0025_USER_ID_LEN+1];
    char success_flag[ft0025_SUCCESS_FLAG_LEN+1];
    char err_message[ft0025_ERR_LEN+1];
    char filler[ft0025_SEND_FILLER_LEN+1];
	upsell_25 upsell_detail[ft0025_UPSELL_LOOP];
	char alt[ft0025_ALT_LEN+1];
	char add[ft0025_ADD_LEN+1];
	char web_pr1[ft0025_WEB_LEN+1];
	char web_pr2[ft0025_WEB_LEN+1];
	char web_pr3[ft0025_WEB_LEN+1];
	char alt_item_no[ft0025_ITEM_NO_LEN+1];
	char add_item_no[ft0025_ITEM_NO_LEN+1];
	char pr1_item_no[ft0025_ITEM_NO_LEN+1];
	char pr2_item_no[ft0025_ITEM_NO_LEN+1];
	char pr3_item_no[ft0025_ITEM_NO_LEN+1];
	char alt_item_desc[ft0025_DESC_LEN+1];
	char add_item_desc[ft0025_DESC_LEN+1];
	char pr1_item_desc[ft0025_DESC_LEN+1];
	char pr2_item_desc[ft0025_DESC_LEN+1];
	char pr3_item_desc[ft0025_DESC_LEN+1];

} ft0025_st_recv;


