/* tt0059_defs.h  Variable Kit List */

/* Web to MACS Buffer  */

#define  tt0059_LAN_SEND_BUF_LEN        92 //65    
#define  tt0059_LAN_RECV_BUF_LEN        20125 //20073 //20045 //20000  

#define  tt0059_REQ_ID_LEN             4
#define  tt0059_REC_ID_LEN             4
#define  tt0059_COMPANY_LEN            2
#define  tt0059_DIVISION_LEN           2
#define  tt0059_USER_ID_LEN            16
#define  tt0059_IP_ADDR_LEN            16
#define  tt0059_FILLER_LEN                              25

#define  tt0059_ITM_NO_LEN                       20
#define  tt0059_CART_LINE_LEN                    3

/* MACS to Web Buffer  */

#define  tt0059_REQ_ID_LEN             4
#define  tt0059_REC_ID_LEN             4
#define  tt0059_USER_ID_LEN            16
#define  tt0059_SUCCESS_FLAG_LEN        1
#define  tt0059_ERR_LEN                         80
#define  tt0059_SEND_FILLER_LEN                 45


#define  tt0059_ITM_NO_LEN                       20
#define  tt0059_EDP_LEN                          9
#define  tt0059_DESC_LEN                                 50
#define  tt0059_UPS_LEN                          640
#define  tt0059_IMG_OVRD_LEN                     8
#define  tt0059_PRICE_LEN                                10
#define  tt0059_AVAIL_LEN                                7
#define  tt0059_QTY_LEN                      5
#define  tt0059_ITEM_LINE_NUMBER_LEN    3
#define  tt0059_R1_COUNT_LEN                     2

#define  tt0059_R1_COMP_COUNT                    25

#define  tt0059_R1_ITM_NO_LEN                    20
#define  tt0059_R1_EDP_LEN                       9
#define  tt0059_R1_DESC_LEN                      50
#define  tt0059_R1_QTY_LEN                       5
#define  tt0059_R1_UPS_LEN                       640
#define  tt0059_R1_IMG_OVRD_LEN          8
#define  tt0059_R1_PRICE_LEN                     10
#define  tt0059_R1_AVAIL_LEN                     7
#define  tt0059_R1_CUSTOM_FLAG_LEN      1

typedef struct
{
    char request_id[tt0059_REQ_ID_LEN+1];
    char record_id[tt0059_REC_ID_LEN+1];
    char company[tt0059_COMPANY_LEN+1];
    char division[tt0059_DIVISION_LEN+1];
    char userid[tt0059_USER_ID_LEN+1];
    char ip_address[tt0059_IP_ADDR_LEN+1];
        char filler[tt0059_FILLER_LEN+1];
        char kit_item_num[tt0059_ITM_NO_LEN+1];
        char cart_line_num[tt0059_CART_LINE_LEN+1];
            
} tt0059_st_send;



typedef struct
{
        char r1_item_num[tt0059_R1_ITM_NO_LEN+1];
        char r1_item_edp[tt0059_R1_EDP_LEN+1];
        char r1_item_desc[tt0059_R1_DESC_LEN+1];
        char r1_item_qty[tt0059_R1_QTY_LEN+1];
        char r1_upsl_X[tt0059_R1_UPS_LEN+1];
        char r1_image_ovrd[tt0059_R1_IMG_OVRD_LEN+1];
        char r1_item_price[tt0059_R1_PRICE_LEN+1];
        char r1_item_avail_inv[tt0059_R1_AVAIL_LEN+1];
        char r1_custom_flag[tt0059_R1_CUSTOM_FLAG_LEN+1];
} tt0059_r1_comp_array;



typedef struct
{
    char request_id[tt0059_REQ_ID_LEN+1];
    char record_id[tt0059_REC_ID_LEN+1];
    char userid[tt0059_USER_ID_LEN+1];
    char success[tt0059_SUCCESS_FLAG_LEN+1];
        char err_message[tt0059_ERR_LEN+1];
        char filler[tt0059_SEND_FILLER_LEN+1];

        char kit_item_num[tt0059_ITM_NO_LEN+1];
        char kit_item_edp[tt0059_EDP_LEN+1];
        char kit_item_desc[tt0059_DESC_LEN+1];
        char upsl_X[tt0059_UPS_LEN+1];
        char image_ovrd[tt0059_IMG_OVRD_LEN+1];
        char kit_item_price[tt0059_PRICE_LEN+1];
        char kit_item_avail_inv[tt0059_AVAIL_LEN+1];
        char kit_item_qty[tt0059_QTY_LEN+1];
        char item_line_number[tt0059_ITEM_LINE_NUMBER_LEN+1];
        char r1_count[tt0059_R1_COUNT_LEN+1];
        tt0059_r1_comp_array r1_component[tt0059_R1_COMP_COUNT];

} tt0059_st_recv;

