/* tt0021_defs.h  Product Pricing */


#define  tt0021_LAN_SEND_BUF_LEN        549 //522 
#define  tt0021_LAN_RECV_BUF_LEN        2215 //2175 //2000

/* Web to MACS Buffer  */

#define  tt0021_REQ_ID_LEN             4
#define  tt0021_REC_ID_LEN             4
#define  tt0021_COMPANY_LEN            2
#define  tt0021_DIVISION_LEN           2
#define  tt0021_USER_ID_LEN            16
#define  tt0021_IP_ADDR_LEN            16
#define  tt0021_FILLER_LEN                              25

#define  tt0021_ITEM_COUNT             20

#define  tt0021_ITM_NO_LEN             20
#define  tt0021_QTY_LEN                4

/* MACS to Web Buffer  */

#define  tt0021_REQ_ID_LEN             4
#define  tt0021_REC_ID_LEN             4
#define  tt0021_USER_ID_LEN            16
#define  tt0021_SUCCESS_FLAG_LEN        1
#define  tt0021_ERR_LEN                         80
#define  tt0021_SEND_FILLER_LEN                 45


#define tt0021_ITEM_COUNT              20

#define  tt0021_EDP_LEN                9
#define  tt0021_ITM_NO_LEN               20
#define  tt0021_DESC_LEN                                 50
#define  tt0021_PRICE_LEN                                10
#define  tt0021_AVAIL_FLAG_LEN           1
#define  tt0021_AVAIL_INV_LEN                    7
#define  tt0021_ITM_QTY_LEN            5


typedef struct
{
    char request_id[tt0021_REQ_ID_LEN+1];
    char record_id[tt0021_REC_ID_LEN+1];
    char company[tt0021_COMPANY_LEN+1];
    char division[tt0021_DIVISION_LEN+1];
    char userid[tt0021_USER_ID_LEN+1];
    char ip_address[tt0021_IP_ADDR_LEN+1];
        char filler[tt0021_FILLER_LEN+1];
    char item_num[tt0021_ITEM_COUNT][tt0021_ITM_NO_LEN+1];
        char item_qty[tt0021_ITEM_COUNT][tt0021_QTY_LEN+1];

} tt0021_st_send;

typedef struct
{
    char item_edp[tt0021_EDP_LEN+1];
    char item_num[tt0021_ITM_NO_LEN+1];
    char item_desc[tt0021_DESC_LEN+1];
    char item_price[tt0021_PRICE_LEN+1];
    char item_avail_flag[tt0021_AVAIL_FLAG_LEN+1];
    char item_avail_inv[tt0021_AVAIL_INV_LEN+1];
        char item_qty[tt0021_ITM_QTY_LEN +1];

} tt0021_item_array;

typedef struct
{
    char request_id[tt0021_REQ_ID_LEN+1];
    char record_id[tt0021_REC_ID_LEN+1];
    char userid[tt0021_USER_ID_LEN+1];
    char success[tt0021_SUCCESS_FLAG_LEN+1];
        char err_message[tt0021_ERR_LEN+1];
        char filler[tt0021_SEND_FILLER_LEN+1];

    tt0021_item_array item[tt0021_ITEM_COUNT];

} tt0021_st_recv;