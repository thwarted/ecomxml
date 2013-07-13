/* wl0003_defs.h   */


#define  wl0003_LAN_SEND_BUF_LEN           80           
#define  wl0003_LAN_RECV_BUF_LEN           3600 


/* Web to MACS Buffer */

#define  wl0003_REC_ID_LEN                 4
#define  wl0003_COMPANY_LEN                2
#define  wl0003_DIVISION_LEN               2
#define  wl0003_USER_ID_LEN                16
#define  wl0003_SEND_FILLER_LEN            10

#define  wl0003_ITM_NO_LEN                 20
#define  wl0003_LINE_NO_LEN                4
#define  wl0003_FLAG_LEN                   1

/* MACS to Web Buffer */

#define  wl0003_REC_ID_LEN                 4
#define  wl0003_USER_ID_LEN                16
#define  wl0003_SUCCESS_FLAG_LEN           1
#define  wl0003_ERR_LEN                    80
#define  wl0003_RECV_FILLER_LEN            30


#define  wl0003_CUST_EDP_LEN               9
#define  wl0003_ITM_COUNT_LEN              3
#define  wl0003_ITM_EDP_LEN                9
#define  wl0003_ITM_NO_LEN                 20
#define  wl0003_QTY_LEN                    4
#define  wl0003_DESC_LEN                   50
#define  wl0003_PRICE_LEN                  9
#define  wl0003_FLAG_LEN                   1
#define  wl0003_CUSTOM_LEN                 60
#define  wl0003_CUSTOM_LOOP                25
#define  wl0003_R1_LOOP                    25


typedef struct
{
    char record_id[wl0003_REC_ID_LEN+1];
    char company[wl0003_COMPANY_LEN+1];
    char division[wl0003_DIVISION_LEN+1];
    char userid[wl0003_USER_ID_LEN+1];
    char wl_company[wl0003_COMPANY_LEN+1];
    char wl_division[wl0003_DIVISION_LEN+1];
    char wl_id[wl0003_USER_ID_LEN+1];
    char wl_filler[wl0003_SEND_FILLER_LEN+1];
    char item_no[wl0003_ITM_NO_LEN+1];
    char line_no[wl0003_LINE_NO_LEN+1];
    char custom_flag[wl0003_FLAG_LEN+1];
    char kit_flag[wl0003_FLAG_LEN+1];

} wl0003_st_send;

typedef struct
{
    char custom[wl0003_CUSTOM_LEN+1];

} wl0003_custom;

typedef struct
{
    char r1_item_no[wl0003_ITM_NO_LEN+1];
    char r1_item_desc[wl0003_DESC_LEN+1];
    char r1_item_qty[wl0003_QTY_LEN+1];

} wl0003_r1_product;

typedef struct
{
    char record_id[wl0003_REC_ID_LEN+1];
    char userid[wl0003_USER_ID_LEN+1];
    char success_flag[wl0003_SUCCESS_FLAG_LEN+1];
    char err_message[wl0003_ERR_LEN+1];
    char filler[wl0003_RECV_FILLER_LEN+1];
    char cust_edp[wl0003_CUST_EDP_LEN+1];
    char item_count[wl0003_ITM_COUNT_LEN+1];
    char item_edp[wl0003_ITM_EDP_LEN+1];
    char item_no[wl0003_ITM_NO_LEN+1];
    char qty[wl0003_QTY_LEN+1];
    char desc[wl0003_DESC_LEN+1];
    char price[wl0003_PRICE_LEN+1];
    char custom_flag[wl0003_FLAG_LEN+1];
    wl0003_custom customization[wl0003_CUSTOM_LOOP];
    wl0003_r1_product r1_product_detail[wl0003_R1_LOOP];

} wl0003_st_recv;