/* tt0013_defs.h  Shopping Cart Contents*/

#define  tt0013_LAN_SEND_BUF_LEN         72    //70 //43
#define  tt0013_LAN_RECV_BUF_LEN         8150 //7950  //8000 //7696

/* Web to MACS Buffer  */

#define  tt0013_REQ_ID_LEN               4
#define  tt0013_REC_ID_LEN               4
#define  tt0013_COMPANY_LEN              2
#define  tt0013_DIVISION_LEN             2
#define  tt0013_USER_ID_LEN              16
#define  tt0013_IP_ADDR_LEN              16
#define  tt0013_FILLER_LEN                           25

#define  tt0013_EMPTY_FLAG_LEN           1
#define  tt0013_PAGE_NO_LEN                          2

/* MACS to Web Buffer  */

#define  tt0013_REQ_ID_LEN               4
#define  tt0013_REC_ID_LEN               4
#define  tt0013_USER_ID_LEN              16
#define  tt0013_SUCCESS_FLAG_LEN         1
#define  tt0013_ERR_LEN                          80
#define  tt0013_SEND_FILLER_LEN              45


#define  tt0013_EDP_LEN                      9
#define  tt0013_SRC_LEN                      16
#define  tt0013_ITM_COUNT_LEN            3 
#define  tt0013_PAGE_NO_LEN                          2
#define  tt0013_PAGE_COUNT_LEN               2
#define  tt0013_CART_TOTAL_LEN               14

#define  tt0013_ITEM_COUNT                           50

#define  tt0013_ITM_EDP_LEN              9
#define  tt0013_ITM_NO_LEN                       20
#define  tt0013_ITM_QTY_LEN              5
#define  tt0013_ITM_DESC_LEN                     50
#define  tt0013_ITM_CUSTOM_FLAG_LEN      1
#define  tt0013_ITM_PRICE_LEN                10
#define  tt0013_ITM_AVAIL_INV_LEN                7
#define  tt0013_SHIPTO_NO_LEN                    2
#define  tt0013_SHIPTO_FN_LEN                    16
#define  tt0013_SHIPTO_LN_LEN                    16
#define  tt0013_STAND_ORD_ITEM_LEN       1
#define  tt0013_STAND_ORD_DATE_LEN       8
#define  tt0013_GWRAP_FLAG_LEN                   1
#define  tt0013_GWRAP_AVAIL_INV_LEN              7
#define  tt0013_FRAME_QTY_LEN                    5
#define  tt0013_PRICE_OVRD_FLAG_LEN              1

#define  tt0013_GC_FLAG_LEN                          2
#define  tt0013_STAND_ORD_CUST_LEN       1
#define  tt0013_RETURN_CUST_FLAG_LEN     1


typedef struct
{
    char request_id[tt0013_REQ_ID_LEN+1];
    char record_id[tt0013_REC_ID_LEN+1];
    char company[tt0013_COMPANY_LEN+1];
    char division[tt0013_DIVISION_LEN+1];
    char userid[tt0013_USER_ID_LEN+1];
    char ip_address[tt0013_IP_ADDR_LEN+1];
        char filler[tt0013_FILLER_LEN+1];
    char empty_flag[tt0013_EMPTY_FLAG_LEN+1];
        char page_no[tt0013_PAGE_NO_LEN+1];

} tt0013_st_send;

typedef struct
{
    char item_edp[tt0013_ITM_EDP_LEN+1];
    char item_num[tt0013_ITM_NO_LEN+1];
    char item_qty[tt0013_ITM_QTY_LEN+1];
    char item_desc[tt0013_ITM_DESC_LEN+1];
    char item_customization_flag[tt0013_ITM_CUSTOM_FLAG_LEN+1];
    char item_price[tt0013_ITM_PRICE_LEN+1];
    char item_avail_inv[tt0013_ITM_AVAIL_INV_LEN+1];
    char shipto_num[tt0013_SHIPTO_NO_LEN+1];
        char shipto_fname[tt0013_SHIPTO_FN_LEN+1];
        char shipto_lname[tt0013_SHIPTO_LN_LEN+1];
    char stand_ord_item[tt0013_STAND_ORD_ITEM_LEN+1];
        char stand_ord_date[tt0013_STAND_ORD_DATE_LEN+1];
        char gwrap_flag[tt0013_GWRAP_FLAG_LEN+1];
        char gwrap_avail_inv[tt0013_GWRAP_AVAIL_INV_LEN+1];
        char frame_qty[tt0013_FRAME_QTY_LEN+1];
        char price_ovrd_flag[tt0013_PRICE_OVRD_FLAG_LEN+1];
} tt0013_item_array;


typedef struct
{
    char request_id[tt0013_REQ_ID_LEN+1];
    char record_id[tt0013_REC_ID_LEN+1];
    char userid[tt0013_USER_ID_LEN+1];
    char success[tt0013_SUCCESS_FLAG_LEN+1];
        char err_message[tt0013_ERR_LEN+1];
        char filler[tt0013_SEND_FILLER_LEN+1];

    char cust_edp[tt0013_EDP_LEN+1];
    char source_code[tt0013_SRC_LEN+1];
    char item_count[tt0013_ITM_COUNT_LEN+1];
        char page_no[tt0013_PAGE_NO_LEN+1];
        char page_count[tt0013_PAGE_COUNT_LEN+1];
        char cart_total[tt0013_CART_TOTAL_LEN+1];
    tt0013_item_array item[tt0013_ITEM_COUNT];
        char gc_coup_flag[tt0013_GC_FLAG_LEN+1];
        char stand_ord_cust[tt0013_STAND_ORD_CUST_LEN+1];
        char return_cust_flag[tt0013_RETURN_CUST_FLAG_LEN+1];
} tt0013_st_recv;
