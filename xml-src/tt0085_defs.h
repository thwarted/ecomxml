/* tt0085_defs.h Get Dynamic Upsell */

/* Web to MACS Buffer  */

#define  tt0085_LAN_SEND_BUF_LEN        99 //72    
#define  tt0085_LAN_RECV_BUF_LEN        2585 //2545 //2500

#define  tt0085_REQ_ID_LEN             4
#define  tt0085_REC_ID_LEN             4
#define  tt0085_COMPANY_LEN            2
#define  tt0085_DIVISION_LEN           2
#define  tt0085_USER_ID_LEN            16
#define  tt0085_IP_ADDR_LEN                      16
#define  tt0085_FILLER_LEN                              25

#define  tt0085_ITM_NO_LEN                       20
#define  tt0085_EDP_LEN                          9
#define  tt0085_PRED_LEN                                 1

/* MACS to Web Buffer */

#define  tt0085_REQ_ID_LEN             4
#define  tt0085_REC_ID_LEN             4
#define  tt0085_USER_ID_LEN            16
#define  tt0085_SUCCESS_FLAG_LEN        1
#define  tt0085_ERR_LEN                         80
#define  tt0085_SEND_FILLER_LEN                 45


#define  tt0085_ITM_NO_LEN                       20
#define  tt0085_EDP_LEN                          9
#define  tt0085_DESC_LEN                                 50
#define  tt0085_COUNT_LEN                                2

#define  tt0085_UPS_PROD_COUNT           10

#define  tt0085_ITM_NO_LEN                       20
#define  tt0085_EDP_LEN                          9
#define  tt0085_DESC_LEN                                 50
#define  tt0085_IMG_LEN                          8
#define  tt0085_PRICE_LEN                                10
#define  tt0085_AVAIL_LEN                                7

#define  tt0085_UPS_CAT_COUNT            10

#define  tt0085_ITM_NO_LEN                       20
#define  tt0085_EDP_LEN                          9
#define  tt0085_DESC_LEN                                 50
#define  tt0085_IMG_LEN                          8
#define  tt0085_PRICE_LEN                                10
#define  tt0085_AVAIL_LEN                                7


typedef struct
{
    char request_id[tt0085_REQ_ID_LEN+1];
    char record_id[tt0085_REC_ID_LEN+1];
    char company[tt0085_COMPANY_LEN+1];
    char division[tt0085_DIVISION_LEN+1];
    char userid[tt0085_USER_ID_LEN+1];
    char ip_address[tt0085_IP_ADDR_LEN+1];
        char filler[tt0085_FILLER_LEN+1];
        char item_num[tt0085_ITM_NO_LEN+1];
        char cust_edp[tt0085_EDP_LEN+1];
        char predictive_type[tt0085_PRED_LEN+1];
        
} tt0085_st_send;

typedef struct
{
        char ups_prod_num[tt0085_ITM_NO_LEN+1];
        char ups_prod_edp[tt0085_EDP_LEN+1];
        char ups_prod_desc[tt0085_DESC_LEN+1];
        char ups_prod_image[tt0085_IMG_LEN+1];
        char ups_prod_price[tt0085_PRICE_LEN+1];
        char ups_prod_avail_inv[tt0085_AVAIL_LEN+1];
    
} tt0085_ups_prod_array;

typedef struct
{
        char ups_cat_num[tt0085_ITM_NO_LEN+1];
        char ups_cat_edp[tt0085_EDP_LEN+1];
        char ups_cat_desc[tt0085_DESC_LEN+1];
        char ups_cat_image[tt0085_IMG_LEN+1];
        char ups_cat_price[tt0085_PRICE_LEN+1];
        char ups_cat_avail_inv[tt0085_AVAIL_LEN+1];
    
} tt0085_ups_cat_array;


typedef struct
{
    char request_id[tt0085_REQ_ID_LEN+1];
    char record_id[tt0085_REC_ID_LEN+1];
    char userid[tt0085_USER_ID_LEN+1];
    char success[tt0085_SUCCESS_FLAG_LEN+1];
        char err_message[tt0085_ERR_LEN+1];
        char filler[tt0085_SEND_FILLER_LEN+1];

        char item_num[tt0085_ITM_NO_LEN+1];
        char item_edp[tt0085_EDP_LEN+1];
        char item_desc[tt0085_DESC_LEN+1];
        char prod_count[tt0085_COUNT_LEN+1];
        char cat_count[tt0085_COUNT_LEN+1];
        tt0085_ups_prod_array ups_prod[tt0085_UPS_PROD_COUNT+1];
        tt0085_ups_cat_array ups_cat[tt0085_UPS_CAT_COUNT+1];

} tt0085_st_recv;

