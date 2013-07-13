/* tt0031_defs.h Delete Item */

/* Web to MACS Buffer  */

#define  tt0031_LAN_SEND_BUF_LEN       1322 //1295
#define  tt0031_LAN_RECV_BUF_LEN       12147 //12045 //12000

#define  tt0031_REQ_ID_LEN             4
#define  tt0031_REC_ID_LEN               4
#define  tt0031_COMPANY_LEN            2
#define  tt0031_DIVISION_LEN           2
#define  tt0031_USER_ID_LEN            16
#define  tt0031_IP_ADDR_LEN            16
#define  tt0031_FILLER_LEN                              25

#define  tt0031_ITM_NO_LEN             20
#define  tt0031_QTY_LEN                4
#define  tt0031_CART_FLAG_LEN          1
#define  tt0031_CART_LINE_NO_LEN       3
#define  tt0031_UPSELL_REQUEST_LEN     1
#define  tt0031_IS_STYLE_FLAG_LEN      1

#define  tt0031_CUSTOMIZE_COUNT      20

#define  tt0031_CUSTOMIZE_LEN                    60

#define  tt0031_INV_REQ_LEN                      1
#define  tt0031_TVS_ITM_LEN                      1
#define  tt0031_TVS_SRC_LEN                      16
#define  tt0031_SHIPTO_NO_LEN                    2


/* MACS to Web Buffer  */

#define  tt0031_REQ_ID_LEN             4
#define  tt0031_REC_ID_LEN             4
#define  tt0031_USER_ID_LEN            16
#define  tt0031_SUCCESS_FLAG_LEN        1
#define  tt0031_ERR_LEN                         80
#define  tt0031_SEND_FILLER_LEN                 45


#define  tt0031_EDP_LEN                9
#define  tt0031_ITM_NO_LEN                   20
#define  tt0031_DESC_LEN                         50
#define  tt0031_PRICE_LEN              10
#define  tt0031_AVAIL_FLAG_LEN         1
#define  tt0031_AVAIL_INV_LEN          7
#define  tt0031_PO_LEN                 8
#define  tt0031_UPS_INFO_LEN           640
#define  tt0031_MPB_METH_LEN           1
#define  tt0031_MPB_UPS_LEN            80

#define  tt0031_MPB_COUNT                            9

#define  tt0031_MPB_QTY_LEN            5
#define  tt0031_MPB_PRICE_LEN          10

#define  tt0031_CUST_FLAG_LEN          1
#define  tt0031_UPS_REQ_LEN                      640
#define  tt0031_IMG_OVRD_LEN                 8
#define  tt0031_STYLE_ID_LEN           12
#define  tt0031_L1_TYPE_LEN            2
#define  tt0031_L1_NAME_LEN            15
#define  tt0031_L2_TYPE_LEN            2
#define  tt0031_L2_NAME_LEN            15
#define  tt0031_L3_TYPE_LEN            2
#define  tt0031_L3_NAME_LEN            15

#define  tt0031_STYLE_COUNT                  50

#define  tt0031_STYLE_DESC_LEN         20

#define  tt0031_TVS_SRC_LEN                      16

#define  tt0031_MASTER_PRICE_LEN                 10

#define  tt0031_PROMPT_COUNT             50

#define  tt0031_PROM_ITM_EDP_LEN                 9
#define  tt0031_PROM_ITM_NO_LEN          20
#define  tt0031_PROM_ITM_DESC_LEN                50
#define  tt0031_PROM_ITM_PRICE_LEN       10
#define  tt0031_PROM_ITM_AVAIL_INV_LEN 7


typedef struct
{
    char customization[tt0031_CUSTOMIZE_LEN+1];

} tt0031_customize_array;

typedef struct
{
    char request_id[tt0031_REQ_ID_LEN+1];
    char record_id[tt0031_REC_ID_LEN+1];
    char company[tt0031_COMPANY_LEN+1];
    char division[tt0031_DIVISION_LEN+1];
    char userid[tt0031_USER_ID_LEN+1];
    char ip_address[tt0031_IP_ADDR_LEN+1];
        char filler[tt0031_FILLER_LEN+1];
    char item_num[tt0031_ITM_NO_LEN+1];
    char item_qty[tt0031_QTY_LEN+1];
    char cart_flag[tt0031_CART_FLAG_LEN+1];
    char cart_line_num[tt0031_CART_LINE_NO_LEN+1];
    char upsell_req_a[tt0031_UPSELL_REQUEST_LEN+1];
    char upsell_req_b[tt0031_UPSELL_REQUEST_LEN+1];
    char upsell_req_c[tt0031_UPSELL_REQUEST_LEN+1];
    char upsell_req_d[tt0031_UPSELL_REQUEST_LEN+1];
    char is_style_flag[tt0031_IS_STYLE_FLAG_LEN+1];

    tt0031_customize_array customize[tt0031_CUSTOMIZE_COUNT];

        char inventory_reqd[tt0031_INV_REQ_LEN+1];
        char tvs_item[tt0031_TVS_ITM_LEN+1];
        char tvs_source[tt0031_TVS_SRC_LEN+1];
        char shipto_num[tt0031_SHIPTO_NO_LEN+1];

} tt0031_st_send;


typedef struct
{
    char mpb_qty[tt0031_MPB_QTY_LEN+1];

} tt0031_mpb_qty_array;


typedef struct
{
    char mpb_price[tt0031_MPB_PRICE_LEN+1];

} tt0031_mpb_price_array;


typedef struct
{
    char l1_entry[tt0031_STYLE_DESC_LEN+1];
    char l2_entry[tt0031_STYLE_DESC_LEN+1];
    char l3_entry[tt0031_STYLE_DESC_LEN+1];

} tt0031_style_array;

typedef struct
{
    char prmt_item_edp[tt0031_PROM_ITM_EDP_LEN+1];
        char prmt_item_num[tt0031_PROM_ITM_NO_LEN+1];
        char prmt_item_desc[tt0031_PROM_ITM_DESC_LEN+1];
        char prmt_item_price[tt0031_PROM_ITM_PRICE_LEN+1];
        char prmt_item_avail_inv[tt0031_PROM_ITM_AVAIL_INV_LEN+1];
    
} tt0031_prompt_array;



typedef struct
{
    char request_id[tt0031_REQ_ID_LEN+1];
    char record_id[tt0031_REC_ID_LEN+1];
    char userid[tt0031_USER_ID_LEN+1];
    char success[tt0031_SUCCESS_FLAG_LEN+1];
        char err_message[tt0031_ERR_LEN+1];
        char filler[tt0031_SEND_FILLER_LEN+1];


    char item_edp[tt0031_EDP_LEN+1];
    char item_num[tt0031_ITM_NO_LEN+1];
    char item_desc[tt0031_DESC_LEN+1];
    char item_price[tt0031_PRICE_LEN+1];
    char item_avail_flag[tt0031_AVAIL_FLAG_LEN+1];
    char item_avail_inv[tt0031_AVAIL_INV_LEN+1];
    char item_po_date[tt0031_PO_LEN+1];
    char upsell_info[tt0031_UPS_INFO_LEN+1];
    char mpb_method[tt0031_MPB_METH_LEN+1];
    char mpb_upsell[tt0031_MPB_UPS_LEN+1];

    tt0031_mpb_qty_array mpb_qty[tt0031_MPB_COUNT];
    tt0031_mpb_price_array mpb_price[tt0031_MPB_COUNT];

    char customization_flag[tt0031_CUST_FLAG_LEN+1];
    char upsell_a[tt0031_UPS_REQ_LEN+1];
    char upsell_b[tt0031_UPS_REQ_LEN+1];
    char upsell_c[tt0031_UPS_REQ_LEN+1];
    char upsell_d[tt0031_UPS_REQ_LEN+1];
    char image_ovrd[tt0031_IMG_OVRD_LEN+1];
    char style_id[tt0031_STYLE_ID_LEN+1];
    char l1_type[tt0031_L1_TYPE_LEN+1];
    char l1_name[tt0031_L1_NAME_LEN+1];
    char l2_type[tt0031_L2_TYPE_LEN+1];
    char l2_name[tt0031_L2_NAME_LEN+1];
    char l3_type[tt0031_L3_TYPE_LEN+1];
    char l3_name[tt0031_L3_NAME_LEN+1];

    tt0031_style_array style[tt0031_STYLE_COUNT];

        char tvs_source[tt0031_TVS_SRC_LEN+1];
        char item_master_price[tt0031_MASTER_PRICE_LEN+1];

        tt0031_prompt_array prompt[tt0031_PROMPT_COUNT];

} tt0031_st_recv;

