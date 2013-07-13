/* tt0091_defs.h Standing Order Review */

#define  tt0091_LAN_SEND_BUF_LEN        158 
#define  tt0091_LAN_RECV_BUF_LEN        29075 //28928 //28925//28941 

/* Web to MACS Buffer  */

#define  tt0091_REQ_ID_LEN             4
#define  tt0091_REC_ID_LEN             4
#define  tt0091_COMPANY_LEN            2
#define  tt0091_DIVISION_LEN           2
#define  tt0091_USER_ID_LEN            16
#define  tt0091_IP_ADDR_LEN            16
#define  tt0091_FILLER_LEN                              25

#define  tt0091_CUST_EDP_LEN                    9
#define  tt0091_UPDATE_MESSAGE_LEN              80

/* MACS to Web Buffer  */

#define  tt0091_REQ_ID_LEN             4
#define  tt0091_REC_ID_LEN             4
#define  tt0091_USER_ID_LEN            16
#define  tt0091_SUCCESS_FLAG_LEN        1
#define  tt0091_ERR_LEN                         80
#define  tt0091_SEND_FILLER_LEN                 45

#define  tt0091_HDR_CUST_EDP_LEN                9
#define  tt0091_HDR_FNAME_LEN                   16
#define  tt0091_HDR_LNAME_LEN                   16
#define  tt0091_HDR_PM_LEN                              20
#define  tt0091_HDR_PM_TYPE_LEN                 2
#define  tt0091_HDR_CC_NO_LEN                   4
#define  tt0091_HDR_CC_EXP_LEN                  4
#define  tt0091_HDR_CO_LEN                              2
#define  tt0091_HDR_DIV_LEN                             2
#define  tt0091_COUNT_LEN                               3
#define  tt0091_HDR_FULL_CC_NO_LEN               20
#define  tt0091_HDR_FILLER_LEN               100

#define  tt0091_STAND_ORD_DETAIL                100

#define  tt0091_DTL_SHIPTO_EDP_LEN              9
#define  tt0091_DTL_SHIPTO_FNAME_LEN    16
#define  tt0091_DTL_SHIPTO_LNAME_LEN    16
#define  tt0091_DTL_SHIPTO_SM_LEN               80
#define  tt0091_DTL_SHIPTO_SM_TYPE_LEN  2
#define  tt0091_DTL_NEXT_DATE_LEN               8
#define  tt0091_DTL_TEMP_DATE_LEN               8
#define  tt0091_DTL_STOP_DATE_LEN               8
#define  tt0091_DTL_POST_START_LEN              4
#define  tt0091_DTL_POST_STOP_LEN               4
#define  tt0091_DTL_ITEM_EDP_LEN                9
#define  tt0091_DTL_ITEM_NO_LEN                 20
#define  tt0091_DTL_ITEM_DESC_LEN               50
#define  tt0091_DTL_ITEM_QTY_LEN                9
#define  tt0091_DTL_ITEM_PRICE_LEN              10
#define  tt0091_DTL_ITEM_SHIPS_LEN              4
#define  tt0091_DTL_ITEM_INT_N_LEN              3
#define  tt0091_DTL_ITEM_INT_X_LEN              25
#define  tt0091_DTL_ITEM_STATUS_LEN             2



typedef struct
{
    char request_id[tt0091_REQ_ID_LEN+1];
    char record_id[tt0091_REC_ID_LEN+1];
    char company[tt0091_COMPANY_LEN+1];
    char division[tt0091_DIVISION_LEN+1];
    char userid[tt0091_USER_ID_LEN+1];
    char ip_address[tt0091_IP_ADDR_LEN+1];
        char filler[tt0091_FILLER_LEN+1];

        char cust_edp[tt0091_CUST_EDP_LEN+1];
        char update_message[tt0091_UPDATE_MESSAGE_LEN+1];

} tt0091_st_send;


typedef struct
{
    char dtl_shipto_edp[tt0091_DTL_SHIPTO_EDP_LEN+1];
        char dtl_shipto_fname[tt0091_DTL_SHIPTO_FNAME_LEN+1];
        char dtl_shipto_lname[tt0091_DTL_SHIPTO_LNAME_LEN+1];
        char dtl_shipto_sm[tt0091_DTL_SHIPTO_SM_LEN+1];
        char dtl_shipto_sm_type[tt0091_DTL_SHIPTO_SM_TYPE_LEN+1];
        char dtl_next_date[tt0091_DTL_NEXT_DATE_LEN+1];
        char dtl_temp_date[tt0091_DTL_TEMP_DATE_LEN+1];
        char dtl_stop_date[tt0091_DTL_STOP_DATE_LEN+1];
        char dtl_post_start[tt0091_DTL_POST_START_LEN+1];
        char dtl_post_stop[tt0091_DTL_POST_STOP_LEN+1];
        char dtl_item_edp[tt0091_DTL_ITEM_EDP_LEN+1];
        char dtl_item_no[tt0091_DTL_ITEM_NO_LEN+1];
        char dtl_item_desc[tt0091_DTL_ITEM_DESC_LEN+1];
        char dtl_item_qty[tt0091_DTL_ITEM_QTY_LEN+1];
        char dtl_item_price[tt0091_DTL_ITEM_PRICE_LEN+1];
        char dtl_item_ships[tt0091_DTL_ITEM_SHIPS_LEN+1];
        char dtl_item_int_n[tt0091_DTL_ITEM_INT_N_LEN+1];
        char dtl_item_int_x[tt0091_DTL_ITEM_INT_X_LEN+1];
        char dtl_item_status[tt0091_DTL_ITEM_STATUS_LEN+1];
} tt0091_stand_ord_detail_array;


typedef struct
{
    char request_id[tt0091_REQ_ID_LEN+1];
    char record_id[tt0091_REC_ID_LEN+1];
    char userid[tt0091_USER_ID_LEN+1];
    char success[tt0091_SUCCESS_FLAG_LEN+1];
        char err_message[tt0091_ERR_LEN+1];
        char filler[tt0091_SEND_FILLER_LEN+1];

    char hdr_cust_edp[tt0091_HDR_CUST_EDP_LEN+1];
    char hdr_fname[tt0091_HDR_FNAME_LEN+1];
    char hdr_lname[tt0091_HDR_LNAME_LEN+1];
        char hdr_pm[tt0091_HDR_PM_LEN+1];
        char hdr_pm_type[tt0091_HDR_PM_TYPE_LEN+1];
        char hdr_cc_no[tt0091_HDR_CC_NO_LEN+1];
        char hdr_cc_exp[tt0091_HDR_CC_EXP_LEN+1];
        char hdr_co[tt0091_HDR_CO_LEN+1];
        char hdr_div[tt0091_HDR_DIV_LEN+1];
        char count[tt0091_COUNT_LEN+1];
		char hdr_full_cc_no[tt0091_HDR_FULL_CC_NO_LEN+1];
		char hdr_filler_len[tt0091_HDR_FILLER_LEN+1];
        
    tt0091_stand_ord_detail_array stand_ord_detail[tt0091_STAND_ORD_DETAIL];

} tt0091_st_recv;