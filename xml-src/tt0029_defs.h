/* tt0029_defs.h Add Item into Cart */

/* Web to MACS Buffer  */

#define  tt0029_LAN_SEND_BUF_LEN       1353  //1352
#define  tt0029_LAN_RECV_BUF_LEN       12210 //12108 //12048 //12045 //12000

#define  tt0029_REQ_ID_LEN             4
#define  tt0029_REC_ID_LEN               4
#define  tt0029_COMPANY_LEN            2
#define  tt0029_DIVISION_LEN           2
#define  tt0029_USER_ID_LEN            16
#define  tt0029_IP_ADDR_LEN            16
#define  tt0029_FILLER_LEN                              25

#define  tt0029_ITM_NO_LEN             20
#define  tt0029_QTY_LEN                6
#define  tt0029_CART_FLAG_LEN          1
#define  tt0029_CART_LINE_NO_LEN       3
#define  tt0029_UPSELL_REQUEST_LEN     1
#define  tt0029_IS_STYLE_FLAG_LEN      1

#define  tt0029_CUSTOMIZE_COUNT      20

#define  tt0029_CUSTOMIZE_LEN                    60

#define  tt0029_INV_REQ_LEN                      1
#define  tt0029_TVS_ITM_LEN                      1
#define  tt0029_TVS_SRC_LEN                      16
#define  tt0029_SHIPTO_NO_LEN                    2
#define  tt0029_SHIP_TO_EDP_LEN       9
#define  tt0029_WL_NO_LEN                               16
#define  tt0029_LINE_NO_LEN                             4


/* MACS to Web Buffer  */

#define  tt0029_REQ_ID_LEN             4
#define  tt0029_REC_ID_LEN             4
#define  tt0029_USER_ID_LEN            16
#define  tt0029_SUCCESS_FLAG_LEN        1
#define  tt0029_ERR_LEN                         80
#define  tt0029_SEND_FILLER_LEN                 45


#define  tt0029_EDP_LEN                9
#define  tt0029_ITM_NO_LEN                   20
#define  tt0029_DESC_LEN                         50
#define  tt0029_PRICE_LEN              10
#define  tt0029_AVAIL_FLAG_LEN         1
#define  tt0029_AVAIL_INV_LEN          7
#define  tt0029_PO_LEN                 8
#define  tt0029_UPS_INFO_LEN           640
#define  tt0029_MPB_METH_LEN           1
#define  tt0029_MPB_UPS_LEN            80

#define  tt0029_MPB_COUNT                            9

#define  tt0029_MPB_QTY_LEN            6
#define  tt0029_MPB_PRICE_LEN          10

#define  tt0029_CUST_FLAG_LEN          1
#define  tt0029_UPS_REQ_LEN                      640
#define  tt0029_IMG_OVRD_LEN                 8
#define  tt0029_STYLE_ID_LEN           12
#define  tt0029_L1_TYPE_LEN            2
#define  tt0029_L1_NAME_LEN            15
#define  tt0029_L2_TYPE_LEN            2
#define  tt0029_L2_NAME_LEN            15
#define  tt0029_L3_TYPE_LEN            2
#define  tt0029_L3_NAME_LEN            15

#define  tt0029_STYLE_COUNT                  50

#define  tt0029_STYLE_DESC_LEN         20

#define  tt0029_TVS_SRC_LEN                      16

#define  tt0029_MASTER_PRICE_LEN                 10

#define  tt0029_PROMPT_COUNT             50

#define  tt0029_PROM_ITM_EDP_LEN        9
#define  tt0029_PROM_ITM_NO_LEN             20
#define  tt0029_PROM_ITM_DESC_LEN       50
#define  tt0029_PROM_ITM_PRICE_LEN          10
#define  tt0029_PROM_ITM_AVAIL_INV_LEN  7

#define  tt0029_ITEM_LINE_NUMBER_LEN    3
#define  tt0029_FLAG_LEN                                1


typedef struct
{
    char customization[tt0029_CUSTOMIZE_LEN+1];

} tt0029_customize_array;

typedef struct
{
    char request_id[tt0029_REQ_ID_LEN+1];
    char record_id[tt0029_REC_ID_LEN+1];
    char company[tt0029_COMPANY_LEN+1];
    char division[tt0029_DIVISION_LEN+1];
    char userid[tt0029_USER_ID_LEN+1];
    char ip_address[tt0029_IP_ADDR_LEN+1];
        char filler[tt0029_FILLER_LEN+1];
    char item_num[tt0029_ITM_NO_LEN+1];
    char item_qty[tt0029_QTY_LEN+1];
        char pred_flag[tt0029_FLAG_LEN+1];
    char cart_line_num[tt0029_CART_LINE_NO_LEN+1];
    char upsell_req_a[tt0029_UPSELL_REQUEST_LEN+1];
    char upsell_req_b[tt0029_UPSELL_REQUEST_LEN+1];
    char upsell_req_c[tt0029_UPSELL_REQUEST_LEN+1];
    char upsell_req_d[tt0029_UPSELL_REQUEST_LEN+1];
    char is_style_flag[tt0029_IS_STYLE_FLAG_LEN+1];

    tt0029_customize_array customize[tt0029_CUSTOMIZE_COUNT];

        char inventory_reqd[tt0029_INV_REQ_LEN+1];
        char tvs_item[tt0029_TVS_ITM_LEN+1];
        char tvs_source[tt0029_TVS_SRC_LEN+1];
        char shipto_num[tt0029_SHIPTO_NO_LEN+1];
        char ship_to_edp[tt0029_SHIP_TO_EDP_LEN+1];
        char wl_no[tt0029_WL_NO_LEN+1];
        char wl_line_no[tt0029_LINE_NO_LEN+1];

} tt0029_st_send;


typedef struct
{
    char mpb_qty[tt0029_MPB_QTY_LEN+1];

} tt0029_mpb_qty_array;


typedef struct
{
    char mpb_price[tt0029_MPB_PRICE_LEN+1];

} tt0029_mpb_price_array;


typedef struct
{
    char l1_entry[tt0029_STYLE_DESC_LEN+1];
    char l2_entry[tt0029_STYLE_DESC_LEN+1];
    char l3_entry[tt0029_STYLE_DESC_LEN+1];

} tt0029_style_array;

typedef struct
{
    char prmt_item_edp[tt0029_PROM_ITM_EDP_LEN+1];
        char prmt_item_num[tt0029_PROM_ITM_NO_LEN+1];
        char prmt_item_desc[tt0029_PROM_ITM_DESC_LEN+1];
        char prmt_item_price[tt0029_PROM_ITM_PRICE_LEN+1];
        char prmt_item_avail_inv[tt0029_PROM_ITM_AVAIL_INV_LEN+1];
    
} tt0029_prompt_array;



typedef struct
{
    char request_id[tt0029_REQ_ID_LEN+1];
    char record_id[tt0029_REC_ID_LEN+1];
    char userid[tt0029_USER_ID_LEN+1];
    char success[tt0029_SUCCESS_FLAG_LEN+1];
        char err_message[tt0029_ERR_LEN+1];
        char filler[tt0029_SEND_FILLER_LEN+1];


    char item_edp[tt0029_EDP_LEN+1];
    char item_num[tt0029_ITM_NO_LEN+1];
    char item_desc[tt0029_DESC_LEN+1];
    char item_price[tt0029_PRICE_LEN+1];
    char item_avail_flag[tt0029_AVAIL_FLAG_LEN+1];
    char item_avail_inv[tt0029_AVAIL_INV_LEN+1];
    char item_po_date[tt0029_PO_LEN+1];
    char upsell_info[tt0029_UPS_INFO_LEN+1];
    char mpb_method[tt0029_MPB_METH_LEN+1];
    char mpb_upsell[tt0029_MPB_UPS_LEN+1];

    tt0029_mpb_qty_array mpb_qty[tt0029_MPB_COUNT];
    tt0029_mpb_price_array mpb_price[tt0029_MPB_COUNT];

    char customization_flag[tt0029_CUST_FLAG_LEN+1];
    char upsell_a[tt0029_UPS_REQ_LEN+1];
    char upsell_b[tt0029_UPS_REQ_LEN+1];
    char upsell_c[tt0029_UPS_REQ_LEN+1];
    char upsell_d[tt0029_UPS_REQ_LEN+1];
    char image_ovrd[tt0029_IMG_OVRD_LEN+1];
    char style_id[tt0029_STYLE_ID_LEN+1];
    char l1_type[tt0029_L1_TYPE_LEN+1];
    char l1_name[tt0029_L1_NAME_LEN+1];
    char l2_type[tt0029_L2_TYPE_LEN+1];
    char l2_name[tt0029_L2_NAME_LEN+1];
    char l3_type[tt0029_L3_TYPE_LEN+1];
    char l3_name[tt0029_L3_NAME_LEN+1];

    tt0029_style_array style[tt0029_STYLE_COUNT];

        char tvs_source[tt0029_TVS_SRC_LEN+1];
        char item_master_price[tt0029_MASTER_PRICE_LEN+1];

        tt0029_prompt_array prompt[tt0029_PROMPT_COUNT];

        char item_line_number[tt0029_ITEM_LINE_NUMBER_LEN+1];

} tt0029_st_recv;

