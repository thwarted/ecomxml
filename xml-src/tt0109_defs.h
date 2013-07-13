/* tt0109_defs.h  Update Giftwrap */


#define  tt0109_LAN_SEND_BUF_LEN      115 
#define  tt0109_LAN_RECV_BUF_LEN      302

/* Web to MACS Buffer  */

#define  tt0109_REQ_ID_LEN             4
#define  tt0109_REC_ID_LEN             4
#define  tt0109_COMPANY_LEN            2
#define  tt0109_DIVISION_LEN           2
#define  tt0109_USER_ID_LEN            16
#define  tt0109_IP_ADDR_LEN            16
#define  tt0109_FILLER_LEN                              25

#define  tt0109_GWRAP_FLAG_LEN                  1
#define  tt0109_PROD_NUM_LEN             20
#define  tt0109_SHOP_CART_PAGE_NUM_LEN                          2
#define  tt0109_SHOP_CART_LINE_NUM_LEN        3
#define  tt0109_GWRAP_ITEM_NO_LEN                       20

/* MACS to Web Buffer  */

#define  tt0109_REQ_ID_LEN             4
#define  tt0109_REC_ID_LEN             4
#define  tt0109_USER_ID_LEN            16
#define  tt0109_SUCCESS_FLAG_LEN        1
#define  tt0109_ERR_LEN                         80
#define  tt0109_SEND_FILLER_LEN                 45

#define  tt0109_GWRAP_FLAG_LEN                  1
#define  tt0109_PROD_NUM_LEN             20
#define  tt0109_PROD_DESC_LEN                   50
#define  tt0109_SHOP_CART_PAGE_NUM_LEN          2
#define  tt0109_SHOP_CART_LINE_NUM_LEN          3
#define  tt0109_PROD_QTY_LEN                            5
#define  tt0109_GWRAP_ITEM_NO_LEN                       20
#define  tt0109_GWRAP_DESC_LEN                          50
#define  tt0109_GWRAP_CUSTOM_FLAG_LEN           1

typedef struct
{
    char request_id[tt0109_REQ_ID_LEN + 1];
    char record_id[tt0109_REC_ID_LEN + 1];
    char company[tt0109_COMPANY_LEN + 1];
    char division[tt0109_DIVISION_LEN + 1];
    char userid[tt0109_USER_ID_LEN + 1];
    char ip_address[tt0109_IP_ADDR_LEN + 1];
        char filler[tt0109_FILLER_LEN+1];
        char gwrap_flag[tt0109_GWRAP_FLAG_LEN+1];
    char prod_num[tt0109_PROD_NUM_LEN + 1];
        char shop_cart_page_num[tt0109_SHOP_CART_PAGE_NUM_LEN + 1];
    char shop_cart_line_num[tt0109_SHOP_CART_LINE_NUM_LEN + 1];
        char gwrap_item_no[tt0109_GWRAP_ITEM_NO_LEN+1];

} tt0109_st_send;


typedef struct
{
    char request_id[tt0109_REQ_ID_LEN + 1];
    char record_id[tt0109_REC_ID_LEN + 1];
    char userid[tt0109_USER_ID_LEN + 1];
    char success[tt0109_SUCCESS_FLAG_LEN+1];
        char err_message[tt0109_ERR_LEN+1];
        char filler[tt0109_SEND_FILLER_LEN+1];

        char gwrap_flag[tt0109_GWRAP_FLAG_LEN+1];
        char prod_num[tt0109_PROD_NUM_LEN+1];
        char prod_desc[tt0109_PROD_DESC_LEN+1];
        char shop_cart_page_num[tt0109_SHOP_CART_PAGE_NUM_LEN+1];
        char shop_cart_line_num[tt0109_SHOP_CART_LINE_NUM_LEN+1];
        char prod_qty[tt0109_PROD_QTY_LEN+1];
        char gwrap_item_no[tt0109_GWRAP_ITEM_NO_LEN+1];
        char gwrap_desc[tt0109_GWRAP_DESC_LEN+1];
        char gwrap_custom_flag[tt0109_GWRAP_CUSTOM_FLAG_LEN+1];
} tt0109_st_recv;