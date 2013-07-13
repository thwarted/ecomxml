/* tt0121_defs.h  Get Open AR */


#define  tt0121_LAN_SEND_BUF_LEN           103   //95 
#define  tt0121_LAN_RECV_BUF_LEN           3636  //3628


/* Web to MACS Buffer */

#define  tt0121_REQ_ID_LEN                 4
#define  tt0121_REC_ID_LEN                     4
#define  tt0121_COMPANY_LEN                2
#define  tt0121_DIVISION_LEN               2
#define  tt0121_USER_ID_LEN                16
#define  tt0121_IP_ADDR_LEN                16
#define  tt0121_FILLER_LEN                                 25

#define  tt0121_OA_TRANS_FLAG_LEN          1
#define  tt0121_DB_FULL_ORDER_NO_LEN       12
#define  tt0121_ENCRYPT_CUST_EDP_LEN       17
#define  tt0121_PAGE_NUM_LEN               4

/* MACS to Web Buffer */

#define  tt0121_REQ_ID_LEN                 4
#define  tt0121_REC_ID_LEN                 4
#define  tt0121_USER_ID_LEN                16
#define  tt0121_SUCCESS_FLAG_LEN           1
#define  tt0121_ERR_LEN                            80
#define  tt0121_SEND_FILLER_LEN                    45


#define  tt0121_OA_TRANS_FLAG_LEN          1
#define  tt0121_OA_CURR_PAGE_LEN           4
#define  tt0121_REC_COUNT_LEN              6
#define  tt0121_NUM_APPL_CREDS_LEN         7
#define  tt0121_TOT_APPL_CREDS_LEN         14
#define  tt0121_CURRENCY_SYMBOL_LEN        4
#define  tt0121_ENCRYPT_CUST_EDP_LEN       17
#define  tt0121_CUST_NUM_LEN               10
#define  tt0121_FNAME_LEN                  16
#define  tt0121_MID_INIT_LEN               1
#define  tt0121_LNAME_LEN                  16
#define  tt0121_COMP_NAME_LEN              30
#define  tt0121_STREET_LEN                 30
#define  tt0121_REF1_LEN                   30
#define  tt0121_REF2_LEN                   30
#define  tt0121_CITY_LEN                   30
#define  tt0121_STATE_LEN                  2
#define  tt0121_ZIPCODE_LEN                14

#define  tt0121_DB_FULL_ORDER_NO_LEN       12
#define  tt0121_DB_COMPANY_LEN             2
#define  tt0121_DB_DIVISION_LEN            2
#define  tt0121_DB_TOT_DOL_LEN             14
#define  tt0121_DB_OPEN_DOL_LEN            14
#define  tt0121_DB_TRANS_DATE_LEN          8
#define  tt0121_DB_PO_NUMBER_LEN           30
#define  tt0121_DB_DUE_DATE_LEN            8
#define  tt0121_DB_TYPE_LEN                2 
#define  tt0121_DB_TYPE_TEXT_LEN           20
#define  tt0121_DB_STATUS_LEN              2
#define  tt0121_DB_COMMENT_LEN             60
#define  tt0121_DB_COMMENT_TYPE_LEN        2
#define  tt0121_DB_COMMENT_DATE_LEN        8

#define  tt0121_CR_DATA_COUNT              20

#define  tt0121_CR_COMPANY_LEN             2
#define  tt0121_CR_DIVISION_LEN            2
#define  tt0121_CR_TRANS_DATE_LEN              8
#define  tt0121_CR_ORIG_REF_LEN            12 
#define  tt0121_CR_TOT_DOL_LEN             14
#define  tt0121_CR_POST_REF_LEN            12
#define  tt0121_CR_DUE_DATE_LEN            8
#define  tt0121_CR_TYPE_LEN                2
#define  tt0121_CR_TYPE_TEXT_LEN           20
#define  tt0121_CR_STATUS_LEN              2
#define  tt0121_CR_COMMENT_LEN             60
#define  tt0121_CR_COMMENT_TYPE_LEN        2
#define  tt0121_CR_COMMENT_DATE_LEN        8


typedef struct
{
    char request_id[tt0121_REQ_ID_LEN+1];
    char record_id[tt0121_REC_ID_LEN+1];
    char company[tt0121_COMPANY_LEN+1];
    char division[tt0121_DIVISION_LEN+1];
    char userid[tt0121_USER_ID_LEN+1];
    char ip_address[tt0121_IP_ADDR_LEN+1];
        char filler[tt0121_FILLER_LEN+1];
    char oa_trans_flag[tt0121_OA_TRANS_FLAG_LEN+1];
    char db_full_order_no[tt0121_DB_FULL_ORDER_NO_LEN+1];
    char encrypt_cust_edp[tt0121_ENCRYPT_CUST_EDP_LEN+1];
    char page_num[tt0121_PAGE_NUM_LEN+1];

} tt0121_st_send;

typedef struct
{
    char cr_company[tt0121_CR_COMPANY_LEN+1];
    char cr_division[tt0121_CR_DIVISION_LEN+1];
    char cr_trans_date[tt0121_CR_TRANS_DATE_LEN+1];
    char cr_orig_ref[tt0121_CR_ORIG_REF_LEN+1];
    char cr_tot_dol[tt0121_CR_TOT_DOL_LEN+1];
    char cr_post_ref[tt0121_CR_POST_REF_LEN+1];
    char cr_due_date[tt0121_CR_DUE_DATE_LEN+1];
    char cr_type[tt0121_CR_TYPE_LEN+1];
    char cr_type_text[tt0121_CR_TYPE_TEXT_LEN+1];
    char cr_status[tt0121_CR_STATUS_LEN+1];
    char cr_comment[tt0121_CR_COMMENT_LEN+1];
    char cr_comment_type[tt0121_CR_COMMENT_TYPE_LEN+1];
    char cr_comment_date[tt0121_CR_COMMENT_DATE_LEN+1];

} tt0121_cr_data_array;

typedef struct
{
    char request_id[tt0121_REQ_ID_LEN+1];
    char record_id[tt0121_REC_ID_LEN+1];
    char userid[tt0121_USER_ID_LEN+1];
    char success_flag[tt0121_SUCCESS_FLAG_LEN+1];
        char err_message[tt0121_ERR_LEN+1];
        char filler[tt0121_SEND_FILLER_LEN+1];

    char oa_trans_flag[tt0121_OA_TRANS_FLAG_LEN+1];
    char oa_curr_page[tt0121_OA_CURR_PAGE_LEN+1];
    char rec_count[tt0121_REC_COUNT_LEN+1];
    char num_appl_creds[tt0121_NUM_APPL_CREDS_LEN+1];
    char tot_appl_creds[tt0121_TOT_APPL_CREDS_LEN+1];
    char currency_symbol[tt0121_CURRENCY_SYMBOL_LEN+1];
    char encrypt_cust_edp[tt0121_ENCRYPT_CUST_EDP_LEN+1];
    char cust_num[tt0121_CUST_NUM_LEN+1];
    char fname[tt0121_FNAME_LEN+1];
    char mid_init[tt0121_MID_INIT_LEN+1];
    char lname[tt0121_LNAME_LEN+1];
    char comp_name[tt0121_COMP_NAME_LEN+1];
    char street[tt0121_STREET_LEN+1];
    char ref1[tt0121_REF1_LEN+1];
    char ref2[tt0121_REF2_LEN+1];
    char city[tt0121_CITY_LEN+1];
    char state[tt0121_STATE_LEN+1];
    char zipcode[tt0121_ZIPCODE_LEN+1];

    char db_full_order_no[tt0121_DB_FULL_ORDER_NO_LEN+1];
    char db_company[tt0121_DB_COMPANY_LEN+1];
    char db_division[tt0121_DB_DIVISION_LEN+1];
    char db_tot_dol[tt0121_DB_TOT_DOL_LEN+1];
    char db_open_dol[tt0121_DB_OPEN_DOL_LEN+1];
    char db_trans_date[tt0121_DB_TRANS_DATE_LEN+1];
    char db_po_number[tt0121_DB_PO_NUMBER_LEN+1];
    char db_due_date[tt0121_DB_DUE_DATE_LEN+1];
    char db_type[tt0121_DB_TYPE_LEN+1];
    char db_type_text[tt0121_DB_TYPE_TEXT_LEN+1];
    char db_status[tt0121_DB_STATUS_LEN+1];
    char db_comment[tt0121_DB_COMMENT_LEN+1];
    char db_comment_type[tt0121_DB_COMMENT_TYPE_LEN+1];
    char db_comment_date[tt0121_DB_COMMENT_DATE_LEN+1];
    tt0121_cr_data_array cr_data[tt0121_CR_DATA_COUNT];

} tt0121_st_recv;
