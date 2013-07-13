/* ft0024_defs.h  Product Shop */


#define  ft0024_LAN_SEND_BUF_LEN            957
#define  ft0024_LAN_RECV_BUF_LEN            29270


/* Web to MACS Buffer */

#define  ft0024_REQ_ID_LEN                 4
#define  ft0024_REC_ID_LEN                 4
#define  ft0024_COMPANY_LEN                2
#define  ft0024_DIVISION_LEN               2
#define  ft0024_USER_ID_LEN                16
#define  ft0024_IP_ADDR_LEN                16
#define  ft0024_FILLER_LEN                 25
#define  ft0024_PAGE_NO_LEN                4
#define  ft0024_PAGE_CNT_LEN               4
#define  ft0024_ITEM_NO_LEN                20
#define  ft0024_ITEM_QTY_LEN               4
#define  ft0024_CUST_NO_LEN                10
#define  ft0024_OFFER_NO_LEN               8
#define  ft0024_CUST_TYPE_LEN              2
#define  ft0024_CONTRACT_LEN               8
#define  ft0024_VAR_ITEM_LEN               20
#define  ft0024_VAR_QTY_LEN                4
#define  ft0024_VAR_PRICE_LEN              9
#define  ft0024_SHOP_TYPE_LEN              1
#define  ft0024_SHIPTO_CNT_LEN             2
#define  ft0024_VAR_COMP_LOOP              25


/* MACS to Web Buffer */

#define  ft0024_REQ_ID_LEN                 4
#define  ft0024_REC_ID_LEN                 4
#define  ft0024_USER_ID_LEN                16
#define  ft0024_SUCCESS_FLAG_LEN           1
#define  ft0024_ERR_LEN                    80
#define  ft0024_SEND_FILLER_LEN            45
#define  ft0024_SHOP_CNT_LEN               3
#define  ft0024_PAGE_NO_LEN                4
#define  ft0024_FLAG_LEN                   1
#define  ft0024_ITEM_NO_LEN                20
#define  ft0024_DESC_LEN                   50
#define  ft0024_PRICE_LEN                  10
#define  ft0024_INV_LEN                    6
#define  ft0024_STATUS_LEN                 20
#define  ft0024_STATUS_CODE_LEN            2
#define  ft0024_EDP_LEN                    9
#define  ft0024_DISC_CODE_LEN              1
#define  ft0024_WAREHSE_LEN                2
#define  ft0024_WEIGHT_LEN                 9
#define  ft0024_COST_LEN                   12
#define  ft0024_QTY_LEN                    4
#define  ft0024_SOURCE_LEN                 16
#define  ft0024_STYLE_LEN                  12
#define  ft0024_DIMENSION_LEN              10
#define  ft0024_SHIP_METHOD_LEN            2
#define  ft0024_SM_DESC_LEN                20
#define  ft0024_DATE_LEN                   8
#define  ft0024_IMG_OVRD_LEN               8
#define  ft0024_AMT_LEN                    7
#define  ft0024_COMPANY_LEN                2
#define  ft0024_DIVISION_LEN               2
#define  ft0024_GROUP_LEN                  4
#define  ft0024_OFFER_LEN                  8
#define  ft0024_CATEGORY_LEN               4
#define  ft0024_REORDER_LEN                9
#define  ft0024_DUE_QTY_LEN                9
#define  ft0024_PRODUCT_LOOP               75 



typedef struct
{
    char request_id[ft0024_REQ_ID_LEN+1];
    char record_id[ft0024_REC_ID_LEN+1];
    char company[ft0024_COMPANY_LEN+1];
    char division[ft0024_DIVISION_LEN+1];
    char userid[ft0024_USER_ID_LEN+1];
    char ip_address[ft0024_IP_ADDR_LEN+1];
    char filler[ft0024_FILLER_LEN+1];
	char page_no[ft0024_PAGE_NO_LEN+1];
	char page_cnt[ft0024_PAGE_CNT_LEN+1];
	char item_no[ft0024_ITEM_NO_LEN+1];
	char item_qty[ft0024_ITEM_QTY_LEN+1];
	char cust_no[ft0024_CUST_NO_LEN+1];
	char offer_no[ft0024_OFFER_NO_LEN+1];
	char cust_type[ft0024_CUST_TYPE_LEN+1];
	char contract[ft0024_CONTRACT_LEN+1];
    char var_item_no[ft0024_VAR_COMP_LOOP][ft0024_VAR_ITEM_LEN+1];
    char var_item_qty[ft0024_VAR_COMP_LOOP][ft0024_VAR_QTY_LEN+1];
    char var_item_price[ft0024_VAR_COMP_LOOP][ft0024_VAR_PRICE_LEN+1];
	char shop_type[ft0024_SHOP_TYPE_LEN+1];
	char shipto_cnt[ft0024_SHIPTO_CNT_LEN+1];

} ft0024_st_send;

typedef struct
{
    char item_no[ft0024_ITEM_NO_LEN+1];
    char item_desc[ft0024_DESC_LEN+1];
    char price[ft0024_PRICE_LEN+1];
    char inv_sign[ft0024_FLAG_LEN+1];
    char avail_inv[ft0024_INV_LEN+1];
    char kit_qty[ft0024_INV_LEN+1];
    char item_status[ft0024_STATUS_LEN+1];
    char item_status_code[ft0024_STATUS_CODE_LEN+1];
    char oe_status[ft0024_STATUS_LEN+1];
    char edp_no[ft0024_EDP_LEN+1];
    char disc_code[ft0024_DISC_CODE_LEN+1];
    char alt_ph_flag[ft0024_FLAG_LEN+1];
    char warehouse[ft0024_WAREHSE_LEN+1];
    char weight[ft0024_WEIGHT_LEN+1];
    char assoc_flag[ft0024_FLAG_LEN+1];
    char cost[ft0024_COST_LEN+1];
    char min_price[ft0024_PRICE_LEN+1];
    char min_qty[ft0024_QTY_LEN+1];
    char max_qty[ft0024_QTY_LEN+1];
    char dim_weight[ft0024_WEIGHT_LEN+1];
    char source[ft0024_SOURCE_LEN+1];
    char full_price[ft0024_PRICE_LEN+1];
    char style[ft0024_STYLE_LEN+1];
    char length[ft0024_DIMENSION_LEN+1];
    char width[ft0024_DIMENSION_LEN+1];
    char depth[ft0024_DIMENSION_LEN+1];
    char ship_method[ft0024_SHIP_METHOD_LEN+1];
    char ship_method_desc[ft0024_SM_DESC_LEN+1];
    char create_date[ft0024_DATE_LEN+1];
    char img_ovrd[ft0024_IMG_OVRD_LEN+1];
    char ins_item_no[ft0024_ITEM_NO_LEN+1];
    char ins_item_qty[ft0024_QTY_LEN+1];
    char ph_exclude_flag[ft0024_FLAG_LEN+1];
    char ph_amount[ft0024_AMT_LEN+1];
    char company[ft0024_COMPANY_LEN+1];
    char division[ft0024_DIVISION_LEN+1];
    char multiprice_flag[ft0024_FLAG_LEN+1];
    char multi_group[ft0024_GROUP_LEN+1];
    char ret_flag[ft0024_FLAG_LEN+1];
    char drop_ship[ft0024_FLAG_LEN+1];
    char offer[ft0024_OFFER_LEN+1];
    char category[ft0024_CATEGORY_LEN+1];
    char reorder[ft0024_REORDER_LEN+1];
    char due_date[ft0024_DATE_LEN+1];
    char due_qty[ft0024_DUE_QTY_LEN+1];
    char restrict_ship[ft0024_FLAG_LEN+1];
    char future_date[ft0024_DATE_LEN+1];
    char ser_no_reqd[ft0024_FLAG_LEN+1];

} product_24;


typedef struct
{
    char request_id[ft0024_REQ_ID_LEN+1];
    char record_id[ft0024_REC_ID_LEN+1];
    char userid[ft0024_USER_ID_LEN+1];
    char success_flag[ft0024_SUCCESS_FLAG_LEN+1];
    char err_message[ft0024_ERR_LEN+1];
    char filler[ft0024_SEND_FILLER_LEN+1];
	char shop_cnt[ft0024_SHOP_CNT_LEN+1];
	char page_no[ft0024_PAGE_NO_LEN+1];
	char shop_flag[ft0024_FLAG_LEN+1];
	char style_found_flag[ft0024_FLAG_LEN+1];
	product_24 product_detail[ft0024_PRODUCT_LOOP];

} ft0024_st_recv;


