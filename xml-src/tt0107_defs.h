/* tt0107_defs.h  Giftwrap Options*/


#define  tt0107_LAN_SEND_BUF_LEN      99 
#define  tt0107_LAN_RECV_BUF_LEN      5586 //5484

/* Web to MACS Buffer  */

#define  tt0107_REQ_ID_LEN             4
#define  tt0107_REC_ID_LEN             4
#define  tt0107_COMPANY_LEN            2
#define  tt0107_DIVISION_LEN           2
#define  tt0107_USER_ID_LEN            16
#define  tt0107_IP_ADDR_LEN            16
#define  tt0107_FILLER_LEN                              25

#define  tt0107_GWRAP_FLAG_LEN                  1
#define  tt0107_PROD_NUM_LEN             20
#define  tt0107_SHOP_CART_PAGE_NUM_LEN                          2
#define  tt0107_SHOP_CART_LINE_NUM_LEN        3
#define  tt0107_PROD_QTY_SEND_LEN                       4

/* MACS to Web Buffer  */

#define  tt0107_REQ_ID_LEN             4
#define  tt0107_REC_ID_LEN             4
#define  tt0107_USER_ID_LEN            16
#define  tt0107_SUCCESS_FLAG_LEN        1
#define  tt0107_ERR_LEN                         80
#define  tt0107_SEND_FILLER_LEN                 45

#define  tt0107_GWRAP_FLAG_LEN                  1
#define  tt0107_PROD_NUM_LEN             20
#define  tt0107_PROD_DESC_LEN                   50
#define  tt0107_SHOP_CART_PAGE_NUM_LEN          2
#define  tt0107_SHOP_CART_LINE_NUM_LEN          3
#define  tt0107_PROD_QTY_LEN                            5
#define  tt0107_GWRAP_EDP_NO_LEN                        9
#define  tt0107_GWRAP_ITEM_NO_LEN                       20
#define  tt0107_GWRAP_DESC_LEN                          50
#define  tt0107_GWRAP_PRICE_LEN                         10
#define  tt0107_GWRAP_AVAIL_INV_LEN                     7
#define  tt0107_GWRAP_CUSTOM_FLAG_LEN           1
#define  tt0107_GWRAP_IMAGE_OVERRIDE_LEN        8

#define  tt0107_PROMPT_DATA_COUNT                       50

#define  tt0107_PROMPT_PROD_EDP_LEN                     9
#define  tt0107_PROMPT_PROD_NUM_LEN                     20
#define  tt0107_PROMPT_PROD_DESC_LEN            50
#define  tt0107_PROMPT_PROD_PRICE_LEN           10
#define  tt0107_PROMPT_PROD_AVAIL_INV_LEN       7
#define  tt0107_PROMPT_PROD_CUST_FLAG_LEN       1
#define  tt0107_PROMPT_PROD_IMAGE_OVRD_LEN      8

typedef struct
{
    char request_id[tt0107_REQ_ID_LEN + 1];
    char record_id[tt0107_REC_ID_LEN + 1];
    char company[tt0107_COMPANY_LEN + 1];
    char division[tt0107_DIVISION_LEN + 1];
    char userid[tt0107_USER_ID_LEN + 1];
    char ip_address[tt0107_IP_ADDR_LEN + 1];
        char filler[tt0107_FILLER_LEN+1];
        char gwrap_flag[tt0107_GWRAP_FLAG_LEN+1];
    char prod_num[tt0107_PROD_NUM_LEN + 1];
        char shop_cart_page_num[tt0107_SHOP_CART_PAGE_NUM_LEN + 1];
    char shop_cart_line_num[tt0107_SHOP_CART_LINE_NUM_LEN + 1];
        char prod_qty[tt0107_PROD_QTY_SEND_LEN+1];

} tt0107_st_send;

typedef struct
{
        char prompt_prod_edp[tt0107_PROMPT_PROD_EDP_LEN+1];
        char prompt_prod_num[tt0107_PROMPT_PROD_NUM_LEN+1];
        char prompt_prod_desc[tt0107_PROMPT_PROD_DESC_LEN+1];
        char prompt_prod_price[tt0107_PROMPT_PROD_PRICE_LEN+1];
        char prompt_prod_avail_inv[tt0107_PROMPT_PROD_AVAIL_INV_LEN+1];
        char prompt_prod_cust_flag[tt0107_PROMPT_PROD_CUST_FLAG_LEN+1];
        char prompt_prod_image_ovrd[tt0107_PROMPT_PROD_IMAGE_OVRD_LEN+1];
} tt0107_prompt_array;

typedef struct
{
    char request_id[tt0107_REQ_ID_LEN + 1];
    char record_id[tt0107_REC_ID_LEN + 1];
    char userid[tt0107_USER_ID_LEN + 1];
    char success[tt0107_SUCCESS_FLAG_LEN+1];
        char err_message[tt0107_ERR_LEN+1];
        char filler[tt0107_SEND_FILLER_LEN+1];

        char gwrap_flag[tt0107_GWRAP_FLAG_LEN+1];
        char prod_num[tt0107_PROD_NUM_LEN+1];
        char prod_desc[tt0107_PROD_DESC_LEN+1];
        char shop_cart_page_num[tt0107_SHOP_CART_PAGE_NUM_LEN+1];
        char shop_cart_line_num[tt0107_SHOP_CART_LINE_NUM_LEN+1];
        char prod_qty[tt0107_PROD_QTY_LEN+1];
        char gwrap_edp_no[tt0107_GWRAP_EDP_NO_LEN+1];
        char gwrap_item_no[tt0107_GWRAP_ITEM_NO_LEN+1];
        char gwrap_desc[tt0107_GWRAP_DESC_LEN+1];
        char gwrap_price[tt0107_GWRAP_PRICE_LEN+1];
        char gwrap_avail_inv[tt0107_GWRAP_AVAIL_INV_LEN+1];
        char gwrap_custom_flag[tt0107_GWRAP_CUSTOM_FLAG_LEN+1];
        char gwrap_image_override[tt0107_GWRAP_IMAGE_OVERRIDE_LEN+1];
        tt0107_prompt_array p_array[tt0107_PROMPT_DATA_COUNT];
} tt0107_st_recv;