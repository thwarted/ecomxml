/* tt0001_defs.h  Product Quote */

/* Web to MACS Buffer  */

#define  tt0001_LAN_SEND_BUF_LEN       1322 //1295
#define  tt0001_LAN_RECV_BUF_LEN       11468 //12045 //12000

#define  tt0001_REQ_ID_LEN             4
#define  tt0001_REC_ID_LEN		       4
#define  tt0001_COMPANY_LEN            2
#define  tt0001_DIVISION_LEN           2
#define  tt0001_USER_ID_LEN            16
#define  tt0001_IP_ADDR_LEN            16
#define  tt0001_FILLER_LEN			   25

#define  tt0001_ITM_NO_LEN             20
#define  tt0001_QTY_LEN                4
#define  tt0001_CART_FLAG_LEN          1
#define  tt0001_CART_LINE_NO_LEN       3
#define  tt0001_UPSELL_REQUEST_LEN     1
#define  tt0001_IS_STYLE_FLAG_LEN      1

#define  tt0001_CUSTOMIZE_COUNT	     20

#define  tt0001_CUSTOMIZE_LEN			 60

#define  tt0001_INV_REQ_LEN			 1
#define  tt0001_TVS_ITM_LEN			 1
#define  tt0001_TVS_SRC_LEN			 16
#define  tt0001_SHIPTO_NO_LEN			 2


/* MACS to Web Buffer  */

#define  tt0001_REQ_ID_LEN             4
#define  tt0001_REC_ID_LEN             4
#define  tt0001_USER_ID_LEN            16
#define  tt0001_SUCCESS_FLAG_LEN       1
#define  tt0001_TVS_ERR_LEN			 80
#define  tt0001_SEND_FILLER_LEN			45


#define  tt0001_EDP_LEN                9
#define  tt0001_ITM_NO_LEN		     20
#define  tt0001_DESC_LEN		    	 50
#define  tt0001_PRICE_LEN              10
#define  tt0001_AVAIL_FLAG_LEN         1
#define  tt0001_AVAIL_INV_LEN          5
#define  tt0001_PO_LEN                 8
#define  tt0001_UPS_INFO_LEN           640
#define  tt0001_MPB_METH_LEN           1
#define  tt0001_MPB_UPS_LEN            80

#define  tt0001_MPB_COUNT			     9

#define  tt0001_MPB_QTY_LEN            5
#define  tt0001_MPB_PRICE_LEN          10

#define  tt0001_CUST_FLAG_LEN          1
#define  tt0001_UPS_REQ_LEN			 640
#define  tt0001_IMG_OVRD_LEN		     8
#define  tt0001_STYLE_ID_LEN           12
#define  tt0001_L1_TYPE_LEN            2
#define  tt0001_L1_NAME_LEN            15
#define  tt0001_L2_TYPE_LEN            2
#define  tt0001_L2_NAME_LEN            15
#define  tt0001_L3_TYPE_LEN            2
#define  tt0001_L3_NAME_LEN            15

#define  tt0001_STYLE_COUNT		     50

#define  tt0001_STYLE_DESC_LEN         20

#define  tt0001_TVS_SRC_LEN			 16

#define  tt0001_MASTER_PRICE_LEN		 10

#define  tt0001_PROMPT_COUNT	         50

#define  tt0001_PROM_ITM_EDP_LEN		 9
#define  tt0001_PROM_ITM_NO_LEN		 20
#define  tt0001_PROM_ITM_DESC_LEN		 50
#define  tt0001_PROM_ITM_PRICE_LEN	 10
#define  tt0001_PROM_ITM_AVAIL_INV_LEN 5

#define  tt0001_GWRAP_FLAG_LEN		1

typedef struct
{
    char customization[tt0001_CUSTOMIZE_LEN+1];

} tt0001_customize_array;

typedef struct
{
    char request_id[tt0001_REQ_ID_LEN+1];
    char record_id[tt0001_REC_ID_LEN+1];
    char company[tt0001_COMPANY_LEN+1];
    char division[tt0001_DIVISION_LEN+1];
    char userid[tt0001_USER_ID_LEN+1];
    char ip_address[tt0001_IP_ADDR_LEN+1];
	char filler[tt0001_FILLER_LEN+1];
    char item_num[tt0001_ITM_NO_LEN+1];
    char item_qty[tt0001_QTY_LEN+1];
    char cart_flag[tt0001_CART_FLAG_LEN+1];
    char cart_line_num[tt0001_CART_LINE_NO_LEN+1];
    char upsell_req_a[tt0001_UPSELL_REQUEST_LEN+1];
    char upsell_req_b[tt0001_UPSELL_REQUEST_LEN+1];
    char upsell_req_c[tt0001_UPSELL_REQUEST_LEN+1];
    char upsell_req_d[tt0001_UPSELL_REQUEST_LEN+1];
    char is_style_flag[tt0001_IS_STYLE_FLAG_LEN+1];

    tt0001_customize_array customize[tt0001_CUSTOMIZE_COUNT];

	char inventory_reqd[tt0001_INV_REQ_LEN+1];
	char tvs_item[tt0001_TVS_ITM_LEN+1];
	char tvs_source[tt0001_TVS_SRC_LEN+1];
	char shipto_num[tt0001_SHIPTO_NO_LEN+1];

} tt0001_st_send;


typedef struct
{
    char mpb_qty[tt0001_MPB_QTY_LEN+1];

} tt0001_mpb_qty_array;


typedef struct
{
    char mpb_price[tt0001_MPB_PRICE_LEN+1];

} tt0001_mpb_price_array;


typedef struct
{
    char l1_entry[tt0001_STYLE_DESC_LEN+1];
    char l2_entry[tt0001_STYLE_DESC_LEN+1];
    char l3_entry[tt0001_STYLE_DESC_LEN+1];

} tt0001_style_array;

typedef struct
{
    char prmt_item_edp[tt0001_PROM_ITM_EDP_LEN+1];
	char prmt_item_num[tt0001_PROM_ITM_NO_LEN+1];
	char prmt_item_desc[tt0001_PROM_ITM_DESC_LEN+1];
	char prmt_item_price[tt0001_PROM_ITM_PRICE_LEN+1];
	char prmt_item_avail_inv[tt0001_PROM_ITM_AVAIL_INV_LEN+1];
    
} tt0001_prompt_array;



typedef struct
{
    char request_id[tt0001_REQ_ID_LEN+1];
    char record_id[tt0001_REC_ID_LEN+1];
    char userid[tt0001_USER_ID_LEN+1];
    char success_flag[tt0001_SUCCESS_FLAG_LEN+1];
	char tvs_err_message[tt0001_TVS_ERR_LEN+1];
	char filler[tt0001_SEND_FILLER_LEN+1];

    char item_edp[tt0001_EDP_LEN+1];
    char item_num[tt0001_ITM_NO_LEN+1];
    char item_desc[tt0001_DESC_LEN+1];
    char item_price[tt0001_PRICE_LEN+1];
    char item_avail_flag[tt0001_AVAIL_FLAG_LEN+1];
    char item_avail_inv[tt0001_AVAIL_INV_LEN+1];
    char item_po_date[tt0001_PO_LEN+1];
    char upsell_info[tt0001_UPS_INFO_LEN+1];
    char mpb_method[tt0001_MPB_METH_LEN+1];
    char mpb_upsell[tt0001_MPB_UPS_LEN+1];

    tt0001_mpb_qty_array mpb_qty[tt0001_MPB_COUNT];
    tt0001_mpb_price_array mpb_price[tt0001_MPB_COUNT];

    char customization_flag[tt0001_CUST_FLAG_LEN+1];
    char upsell_a[tt0001_UPS_REQ_LEN+1];
    char upsell_b[tt0001_UPS_REQ_LEN+1];
    char upsell_c[tt0001_UPS_REQ_LEN+1];
    char upsell_d[tt0001_UPS_REQ_LEN+1];
    char image_ovrd[tt0001_IMG_OVRD_LEN+1];
    char style_id[tt0001_STYLE_ID_LEN+1];
    char l1_type[tt0001_L1_TYPE_LEN+1];
    char l1_name[tt0001_L1_NAME_LEN+1];
    char l2_type[tt0001_L2_TYPE_LEN+1];
    char l2_name[tt0001_L2_NAME_LEN+1];
    char l3_type[tt0001_L3_TYPE_LEN+1];
    char l3_name[tt0001_L3_NAME_LEN+1];

    tt0001_style_array style[tt0001_STYLE_COUNT];

	char tvs_source[tt0001_TVS_SRC_LEN+1];
	char item_master_price[tt0001_MASTER_PRICE_LEN+1];

	tt0001_prompt_array prompt[tt0001_PROMPT_COUNT];

	char gwrap_flag[tt0001_GWRAP_FLAG_LEN+1];
} tt0001_st_recv;

