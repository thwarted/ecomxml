/* tt0025_defs.h Modify Quantity */

#define  tt0025_LAN_SEND_BUF_LEN        371
#define  tt0025_LAN_RECV_BUF_LEN        7900

/* Web to MACS Buffer  */

#define  tt0025_REQ_ID_LEN             4
#define  tt0025_REC_ID_LEN             4
#define  tt0025_COMPANY_LEN            2
#define  tt0025_DIVISION_LEN           2
#define  tt0025_USER_ID_LEN            16
#define  tt0025_IP_ADDR_LEN            16
#define  tt0025_FILLER_LEN				25

#define  tt0025_MOD_QTY_COUNT			 50

#define  tt0025_LINE_NO_LEN            2
#define  tt0025_PAGE_NO_LEN            2
#define  tt0025_PAGE_COUNT_LEN         2
#define  tt0025_CART_TOTAL_LEN		   14
#define  tt0025_QTY_LEN		         4

/* MACS to Web Buffer  */

#define  tt0025_REQ_ID_LEN             4
#define  tt0025_REC_ID_LEN             4
#define  tt0025_USER_ID_LEN            16
#define  tt0025_SUCCESS_FLAG_LEN        1
#define  tt0025_ERR_LEN				80
#define  tt0025_SEND_FILLER_LEN			45


#define  tt0025_EDP_LEN                9
#define  tt0025_SRC_LEN		         16
#define  tt0025_ITM_COUNT_LEN          3 

#define  tt0025_ITEM_COUNT		     50

#define  tt0025_ITM_EDP_LEN            9
#define  tt0025_ITM_NO_LEN		     20
#define  tt0025_ITM_QTY_LEN            5
#define  tt0025_ITM_DESC_LEN			 50
#define  tt0025_ITM_CUSTOM_FLAG_LEN    1
#define  tt0025_ITM_PRICE_LEN		     10
#define  tt0025_ITM_AVAIL_INV_LEN		 5
#define  tt0025_SHIPTO_NO_LEN			 2
#define  tt0025_SHIPTO_FN_LEN			 16
#define  tt0025_SHIPTO_LN_LEN			 16
#define  tt0025_STAND_ORD_ITEM_LEN		1
#define  tt0025_STAND_ORD_DATE_LEN		8
#define  tt0025_STAND_ORD_ITEM_LEN       1
#define  tt0025_STAND_ORD_DATE_LEN        8
#define  tt0025_GWRAP_FLAG_LEN			1
#define  tt0025_GWRAP_AVAIL_INV_LEN			5
#define  tt0025_FRM_QTY_LEN				5


#define  tt0025_GC_FLAG_LEN			 2
#define  tt0025_STAND_ORD_CUST_LEN        1
#define  tt0025_RETURN_CUST_FLAG_LEN		1






typedef struct
{
    char request_id[tt0025_REQ_ID_LEN+1];
    char record_id[tt0025_REC_ID_LEN+1];
    char company[tt0025_COMPANY_LEN+1];
    char division[tt0025_DIVISION_LEN+1];
    char userid[tt0025_USER_ID_LEN+1];
    char ip_address[tt0025_IP_ADDR_LEN+1];
	char filler[tt0025_FILLER_LEN+1];
	char page_no[tt0025_PAGE_NO_LEN+1];
    char modify_line_num[tt0025_MOD_QTY_COUNT][tt0025_LINE_NO_LEN+1];
	char modify_new_qty[tt0025_MOD_QTY_COUNT][tt0025_QTY_LEN+1];

} tt0025_st_send;


typedef struct
{
    char item_edp[tt0025_ITM_EDP_LEN+1];
    char item_num[tt0025_ITM_NO_LEN+1];
    char item_qty[tt0025_ITM_QTY_LEN+1];
    char item_desc[tt0025_ITM_DESC_LEN+1];
    char item_customization_flag[tt0025_ITM_CUSTOM_FLAG_LEN+1];
    char item_price[tt0025_ITM_PRICE_LEN+1];
    char item_avail_inv[tt0025_ITM_AVAIL_INV_LEN+1];
    char shipto_num[tt0025_SHIPTO_NO_LEN+1];
	char shipto_fname[tt0025_SHIPTO_FN_LEN+1];
	char shipto_lname[tt0025_SHIPTO_LN_LEN+1];
    char stand_ord_item[tt0025_STAND_ORD_ITEM_LEN+1];
	char stand_ord_date[tt0025_STAND_ORD_DATE_LEN+1];
	char gwrap_flag[tt0025_GWRAP_FLAG_LEN+1];
	char gwrap_avail_inv[tt0025_GWRAP_AVAIL_INV_LEN+1];
	char frame_qty[tt0025_FRM_QTY_LEN+1];
    
} tt0025_item_array;



typedef struct
{
    char request_id[tt0025_REQ_ID_LEN+1];
    char record_id[tt0025_REC_ID_LEN+1];
    char userid[tt0025_USER_ID_LEN+1];
    char success[tt0025_SUCCESS_FLAG_LEN+1];
	char err_message[tt0025_ERR_LEN+1];
	char filler[tt0025_SEND_FILLER_LEN+1];

    char cust_edp[tt0025_EDP_LEN+1];
    char source_code[tt0025_SRC_LEN+1];
    char item_count[tt0025_ITM_COUNT_LEN+1];
	char page_no[tt0025_PAGE_NO_LEN+1];
	char page_count[tt0025_PAGE_COUNT_LEN+1];
	char cart_total[tt0025_CART_TOTAL_LEN+1];
    tt0025_item_array item[tt0025_ITEM_COUNT];
	char gc_coup_flag[tt0025_GC_FLAG_LEN+1];
	char stand_ord_cust[tt0025_STAND_ORD_CUST_LEN+1];
	char return_cust_flag[tt0025_RETURN_CUST_FLAG_LEN+1];

} tt0025_st_recv;
