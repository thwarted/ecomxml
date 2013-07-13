/* tt0003_defs.h  Shopping Request */


#define  tt0003_LAN_SEND_BUF_LEN            245//218 
#define  tt0003_LAN_RECV_BUF_LEN            2566 //2526 //2400 


/* Web to MACS Buffer */

#define  tt0003_REQ_ID_LEN                 4
#define  tt0003_REC_ID_LEN                   4
#define  tt0003_COMPANY_LEN                2
#define  tt0003_DIVISION_LEN               2
#define  tt0003_USER_ID_LEN                16
#define  tt0003_IP_ADDR_LEN                16
#define  tt0003_FILLER_LEN                              25

#define  tt0003_KEY_WORD_LEN               150
#define  tt0003_PAGE_NO_LEN                              4
#define  tt0003_PAGE_LEN                   2
#define  tt0003_CAT_ITM_NO_LEN             20

/* MACS to Web Buffer */

#define  tt0003_REQ_ID_LEN                 4
#define  tt0003_REC_ID_LEN                 4
#define  tt0003_USER_ID_LEN                16
#define  tt0003_SUCCESS_FLAG_LEN       1
#define  tt0003_ERR_LEN                  80
#define  tt0003_SEND_FILLER_LEN                 45


#define  tt0003_ITM_COUNT_LEN                            6
#define  tt0003_PAGE_NO_LEN                4

#define  tt0003_ITEM_COUNT                       20

#define  tt0003_ITM_EDP_LEN                9
#define  tt0003_ITM_NO_LEN                   20
#define  tt0003_ITM_DESC_LEN               50
#define  tt0003_ITM_PRICE_LEN              10
#define  tt0003_ITM_AVAIL_INV_LEN          7
#define  tt0003_ITM_PO_LEN                           8
#define  tt0003_ITM_IMAGE_OVRD_LEN         8

#define  tt0003_ITM_TYPE_LEN                             1


typedef struct
{
    char request_id[tt0003_REQ_ID_LEN+1];
    char record_id[tt0003_REC_ID_LEN+1];
    char company[tt0003_COMPANY_LEN+1];
    char division[tt0003_DIVISION_LEN+1];
    char userid[tt0003_USER_ID_LEN+1];
    char ip_address[tt0003_IP_ADDR_LEN+1];
        char filler[tt0003_FILLER_LEN+1];
    char key_word[tt0003_KEY_WORD_LEN+1];
    char curr_page_num[tt0003_PAGE_NO_LEN+1];
    char page_len[tt0003_PAGE_LEN+1];
        char cat_item_num[tt0003_CAT_ITM_NO_LEN+1];

} tt0003_st_send;

typedef struct
{
    char item_edp[tt0003_ITM_EDP_LEN+1];
    char item_num[tt0003_ITM_NO_LEN+1];
    char item_desc[tt0003_ITM_DESC_LEN+1];
    char item_price[tt0003_ITM_PRICE_LEN+1];
    char item_avail_inv[tt0003_ITM_AVAIL_INV_LEN+1];
    char item_po_date[tt0003_ITM_PO_LEN+1];
    char item_image_ovrd[tt0003_ITM_IMAGE_OVRD_LEN+1];
    char shopping_item_type[tt0003_ITM_TYPE_LEN+1];

} tt0003_item_array;

typedef struct
{
    char request_id[tt0003_REQ_ID_LEN+1];
    char record_id[tt0003_REC_ID_LEN+1];
    char userid[tt0003_USER_ID_LEN+1];
    char success_flag[tt0003_SUCCESS_FLAG_LEN+1];
        char err_message[tt0003_ERR_LEN+1];
        char filler[tt0003_SEND_FILLER_LEN+1];
    char item_count[tt0003_ITM_COUNT_LEN+1];
    char current_page[tt0003_PAGE_NO_LEN+1];
    tt0003_item_array item[tt0003_ITEM_COUNT];

} tt0003_st_recv;