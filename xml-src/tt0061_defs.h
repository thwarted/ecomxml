/* tt0061_defs.h  Related Products */

/* Web to MACS Buffer  */

#define  tt0061_LAN_SEND_BUF_LEN        89 //62    
#define  tt0061_LAN_RECV_BUF_LEN        3093 //3045 //3000  

#define  tt0061_REQ_ID_LEN             4
#define  tt0061_REC_ID_LEN             4
#define  tt0061_COMPANY_LEN            2
#define  tt0061_DIVISION_LEN           2
#define  tt0061_USER_ID_LEN            16
#define  tt0061_IP_ADDR_LEN                      16
#define  tt0061_FILLER_LEN                      25
#define  tt0061_ITM_NO_LEN               20 

/* MACS to Web Buffer  */

#define  tt0061_REQ_ID_LEN             4
#define  tt0061_REC_ID_LEN             4
#define  tt0061_USER_ID_LEN            16
#define  tt0061_SUCCESS_FLAG_LEN        1
#define  tt0061_ERR_LEN                         80
#define  tt0061_SEND_FILLER_LEN                 45


#define  tt0061_ITM_NO_LEN                       20
#define  tt0061_ITM_COUNT_LEN                    2
#define  tt0061_DESC_LEN                                 50

#define  tt0061_REL_ITEM_COUNT           24

#define  tt0061_ITM_NO_LEN                       20
#define  tt0061_EDP_LEN                  9
#define  tt0061_DESC_LEN                                 50
#define  tt0061_PRICE_LEN                                10
#define  tt0061_AVAIL_LEN                                7
#define  tt0061_PO_LEN                           8
#define  tt0061_IMG_OVRD_LEN                     8


typedef struct
{
    char request_id[tt0061_REQ_ID_LEN+1];
    char record_id[tt0061_REC_ID_LEN+1];
    char company[tt0061_COMPANY_LEN+1];
    char division[tt0061_DIVISION_LEN+1];
    char userid[tt0061_USER_ID_LEN+1];
    char ip_address[tt0061_IP_ADDR_LEN+1];
        char filler[tt0061_FILLER_LEN+1];
    char req_item_num[tt0061_ITM_NO_LEN+1];
    
} tt0061_st_send;

typedef struct
{
    char rel_item_num[tt0061_ITM_NO_LEN+1];
        char rel_item_edp[tt0061_EDP_LEN+1];
        char rel_item_desc[tt0061_DESC_LEN+1];
        char rel_item_price[tt0061_PRICE_LEN+1];
        char rel_item_avail_inv[tt0061_AVAIL_LEN+1];
        char rel_item_po_date[tt0061_PO_LEN+1];
        char rel_item_image_ovrd[tt0061_IMG_OVRD_LEN+1];

} tt0061_rel_item_array;


typedef struct
{
    char request_id[tt0061_REQ_ID_LEN+1];
    char record_id[tt0061_REC_ID_LEN+1];
    char userid[tt0061_USER_ID_LEN+1];
    char success[tt0061_SUCCESS_FLAG_LEN+1];
        char err_message[tt0061_ERR_LEN+1];
        char filler[tt0061_SEND_FILLER_LEN+1];

        char req_item_num[tt0061_ITM_NO_LEN+1];
        char rel_item_count[tt0061_ITM_COUNT_LEN+1];
        char req_item_desc[tt0061_DESC_LEN+1];
        tt0061_rel_item_array rel_items[tt0061_REL_ITEM_COUNT];

} tt0061_st_recv;

