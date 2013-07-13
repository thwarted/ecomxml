/* tt0057_defs.h  Variable Kit Put In Cart */

/* Web to MACS Buffer  */

#define  tt0057_LAN_SEND_BUF_LEN        696 //669    
#define  tt0057_LAN_RECV_BUF_LEN        20101 //20049 //20045 //20000  

#define  tt0057_REQ_ID_LEN             4
#define  tt0057_REC_ID_LEN             4
#define  tt0057_COMPANY_LEN            2
#define  tt0057_DIVISION_LEN           2
#define  tt0057_USER_ID_LEN            16
#define  tt0057_IP_ADDR_LEN            16
#define  tt0057_FILLER_LEN                              25

#define  tt0057_ITM_NO_LEN                       20
#define  tt0057_QUTY_LEN                                 4
#define  tt0057_INV_LEN                          1
#define  tt0057_SHIPTO_NO_LEN                    2

#define  tt0057_R1_ITEM_COUNT            25

#define  tt0057_R1_ITM_NO_LEN                    20
#define  tt0057_R1_QUTY_LEN                      4

/* MACS to Web Buffer  */

#define  tt0057_REQ_ID_LEN             4
#define  tt0057_REC_ID_LEN             4
#define  tt0057_USER_ID_LEN            16
#define  tt0057_SUCCESS_FLAG_LEN        1
#define  tt0057_ERR_LEN                         80
#define  tt0057_SEND_FILLER_LEN                 45


#define  tt0057_ITM_NO_LEN                       20
#define  tt0057_EDP_LEN                          9
#define  tt0057_DESC_LEN                                 50
#define  tt0057_UPS_LEN                          640
#define  tt0057_IMG_OVRD_LEN                     8
#define  tt0057_PRICE_LEN                                10
#define  tt0057_AVAIL_LEN                                7
#define  tt0057_QTY_LEN                      5
#define  tt0057_MESSAGE_LEN                      80
#define  tt0057_CUSTOM_FLAG_LEN         1
#define  tt0057_ITEM_LINE_NUMBER_LEN    3
#define  tt0057_R1_COUNT_LEN                     2
#define  tt0057_SUCCESS_LEN                      1

#define  tt0057_R1_COMP_COUNT                    25

#define  tt0057_R1_ITM_NO_LEN                    20
#define  tt0057_R1_EDP_LEN                       9
#define  tt0057_R1_DESC_LEN                      50
#define  tt0057_R1_QTY_LEN                       5
#define  tt0057_R1_UPS_LEN                       640
#define  tt0057_R1_IMG_OVRD_LEN          8
#define  tt0057_R1_PRICE_LEN                     10
#define  tt0057_R1_AVAIL_LEN                     7


typedef struct
{
    char request_id[tt0057_REQ_ID_LEN+1];
    char record_id[tt0057_REC_ID_LEN+1];
    char company[tt0057_COMPANY_LEN+1];
    char division[tt0057_DIVISION_LEN+1];
    char userid[tt0057_USER_ID_LEN+1];
    char ip_address[tt0057_IP_ADDR_LEN+1];
        char filler[tt0057_FILLER_LEN+1];
        char kit_item_num[tt0057_ITM_NO_LEN+1];
        char kit_item_qty[tt0057_QUTY_LEN+1];
        char inv_reqd[tt0057_INV_LEN+1];
        char shipto_num[tt0057_SHIPTO_NO_LEN+1];
        char r1_item_num[tt0057_R1_ITEM_COUNT][tt0057_R1_ITM_NO_LEN+1];
        char r1_item_qty[tt0057_R1_ITEM_COUNT][tt0057_R1_QUTY_LEN+1];
        
} tt0057_st_send;



typedef struct
{
        char r1_item_num[tt0057_R1_ITM_NO_LEN+1];
        char r1_item_edp[tt0057_R1_EDP_LEN+1];
        char r1_item_desc[tt0057_R1_DESC_LEN+1];
        char r1_item_qty[tt0057_R1_QTY_LEN+1];
        char r1_upsl_X[tt0057_R1_UPS_LEN+1];
        char r1_image_ovrd[tt0057_R1_IMG_OVRD_LEN+1];
        char r1_item_price[tt0057_R1_PRICE_LEN+1];
        char r1_item_avail_inv[tt0057_R1_AVAIL_LEN+1];
        
} tt0057_r1_comp_array;



typedef struct
{
    char request_id[tt0057_REQ_ID_LEN+1];
    char record_id[tt0057_REC_ID_LEN+1];
    char userid[tt0057_USER_ID_LEN+1];
    char success[tt0057_SUCCESS_FLAG_LEN+1];
        char err_message[tt0057_ERR_LEN+1];
        char filler[tt0057_SEND_FILLER_LEN+1];

        char kit_item_num[tt0057_ITM_NO_LEN+1];
        char kit_item_edp[tt0057_EDP_LEN+1];
        char kit_item_desc[tt0057_DESC_LEN+1];
        char upsl_X[tt0057_UPS_LEN+1];
        char image_ovrd[tt0057_IMG_OVRD_LEN+1];
        char kit_item_price[tt0057_PRICE_LEN+1];
        char kit_item_avail_inv[tt0057_AVAIL_LEN+1];
        char kit_item_qty[tt0057_QTY_LEN+1];
        char custom_flag[tt0057_CUSTOM_FLAG_LEN+1];
        char item_line_number[tt0057_ITEM_LINE_NUMBER_LEN+1];
        char r1_count[tt0057_R1_COUNT_LEN+1];
        tt0057_r1_comp_array r1_component[tt0057_R1_COMP_COUNT];

} tt0057_st_recv;

