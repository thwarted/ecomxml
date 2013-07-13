/* tt0105_defs.h  Get Giftwrap */


#define  tt0105_LAN_SEND_BUF_LEN      95
#define  tt0105_LAN_RECV_BUF_LEN      1436

/* Web to MACS Buffer  */

#define  tt0105_REQ_ID_LEN             4
#define  tt0105_REC_ID_LEN             4
#define  tt0105_COMPANY_LEN            2
#define  tt0105_DIVISION_LEN           2
#define  tt0105_USER_ID_LEN            16
#define  tt0105_IP_ADDR_LEN            16
#define  tt0105_FILLER_LEN				25

#define  tt0105_GWRAP_FLAG_LEN			1
#define  tt0105_PROD_NUM_LEN             20
#define  tt0105_SHOP_CART_PAGE_NUM_LEN				2
#define  tt0105_SHOP_CART_LINE_NUM_LEN        3

/* MACS to Web Buffer  */

#define  tt0105_REQ_ID_LEN             4
#define  tt0105_REC_ID_LEN             4
#define  tt0105_USER_ID_LEN            16
#define  tt0105_SUCCESS_FLAG_LEN        1
#define  tt0105_ERR_LEN				80
#define  tt0105_SEND_FILLER_LEN			45

#define  tt0105_GWRAP_FLAG_LEN			1
#define  tt0105_PROD_NUM_LEN             20
#define  tt0105_PROD_DESC_LEN			50
#define  tt0105_SHOP_CART_PAGE_NUM_LEN		2
#define  tt0105_SHOP_CART_LINE_NUM_LEN		3
#define  tt0105_PROD_QTY_LEN				5
#define  tt0105_GWRAP_EDP_NO_LEN			9
#define  tt0105_GWRAP_ITEM_NO_LEN			20
#define  tt0105_GWRAP_DESC_LEN				50
#define  tt0105_GWRAP_PRICE_LEN				10
#define  tt0105_GWRAP_AVAIL_INV_LEN			5
#define  tt0105_GWRAP_CUSTOM_FLAG_LEN		1
#define  tt0105_GWRAP_IMAGE_OVERRIDE_LEN	8
#define  tt0105_GWRAP_CUSTOM_LAST_NO_LEN	2

#define  tt0105_CUSTOM_DATA_COUNT			20

#define  tt0105_GWRAP_CUSTOMIZ_LINES_LEN	55

typedef struct
{
    char request_id[tt0105_REQ_ID_LEN + 1];
    char record_id[tt0105_REC_ID_LEN + 1];
    char company[tt0105_COMPANY_LEN + 1];
    char division[tt0105_DIVISION_LEN + 1];
    char userid[tt0105_USER_ID_LEN + 1];
    char ip_address[tt0105_IP_ADDR_LEN + 1];
	char filler[tt0105_FILLER_LEN+1];
	char gwrap_flag[tt0105_GWRAP_FLAG_LEN+1];
    char prod_num[tt0105_PROD_NUM_LEN + 1];
	char shop_cart_page_num[tt0105_SHOP_CART_PAGE_NUM_LEN + 1];
    char shop_cart_line_num[tt0105_SHOP_CART_LINE_NUM_LEN + 1];

} tt0105_st_send;

typedef struct
{
    char gwrap_customiz_lines[tt0105_GWRAP_CUSTOMIZ_LINES_LEN + 1];

} tt0105_customize_array;

typedef struct
{
    char request_id[tt0105_REQ_ID_LEN + 1];
    char record_id[tt0105_REC_ID_LEN + 1];
    char userid[tt0105_USER_ID_LEN + 1];
    char success[tt0105_SUCCESS_FLAG_LEN+1];
	char err_message[tt0105_ERR_LEN+1];
	char filler[tt0105_SEND_FILLER_LEN+1];

	char gwrap_flag[tt0105_GWRAP_FLAG_LEN+1];
	char prod_num[tt0105_PROD_NUM_LEN+1];
	char prod_desc[tt0105_PROD_DESC_LEN+1];
	char shop_cart_page_num[tt0105_SHOP_CART_PAGE_NUM_LEN+1];
	char shop_cart_line_num[tt0105_SHOP_CART_LINE_NUM_LEN+1];
	char prod_qty[tt0105_PROD_QTY_LEN+1];
	char gwrap_edp_no[tt0105_GWRAP_EDP_NO_LEN+1];
	char gwrap_item_no[tt0105_GWRAP_ITEM_NO_LEN+1];
	char gwrap_desc[tt0105_GWRAP_DESC_LEN+1];
	char gwrap_price[tt0105_GWRAP_PRICE_LEN+1];
	char gwrap_avail_inv[tt0105_GWRAP_AVAIL_INV_LEN+1];
	char gwrap_custom_flag[tt0105_GWRAP_CUSTOM_FLAG_LEN+1];
	char gwrap_image_override[tt0105_GWRAP_IMAGE_OVERRIDE_LEN+1];
	char gwrap_custom_last_no[tt0105_GWRAP_CUSTOM_LAST_NO_LEN+1];
	tt0105_customize_array c_array[tt0105_CUSTOM_DATA_COUNT];
} tt0105_st_recv;