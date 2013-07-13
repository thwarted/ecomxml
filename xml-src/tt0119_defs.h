/* tt0119_defs.h  Get Open AR */


#define  tt0119_LAN_SEND_BUF_LEN           91   //83 
#define  tt0119_LAN_RECV_BUF_LEN           4428 //4420 


/* Web to MACS Buffer */

#define  tt0119_REQ_ID_LEN                 4
#define  tt0119_REC_ID_LEN                     4
#define  tt0119_COMPANY_LEN                2
#define  tt0119_DIVISION_LEN               2
#define  tt0119_USER_ID_LEN                16
#define  tt0119_IP_ADDR_LEN                16
#define  tt0119_FILLER_LEN                                 25

#define  tt0119_OA_TRANS_FLAG_LEN          1
#define  tt0119_ENCRYPT_CUST_EDP_LEN       17
#define  tt0119_PAGE_NUM_LEN               4

/* MACS to Web Buffer */

#define  tt0119_REQ_ID_LEN                 4
#define  tt0119_REC_ID_LEN                 4
#define  tt0119_USER_ID_LEN                16
#define  tt0119_SUCCESS_FLAG_LEN           1
#define  tt0119_ERR_LEN                            80
#define  tt0119_SEND_FILLER_LEN                    45


#define  tt0119_OA_TRANS_FLAG_LEN          1
#define  tt0119_OA_CURR_PAGE_LEN           4
#define  tt0119_NUM_VALID_OPENAR_LEN       6
#define  tt0119_CURRENCY_SYMBOL_LEN        4
#define  tt0119_ENCRYPT_CUST_EDP_LEN       17
#define  tt0119_CUST_NUM_LEN                       10
#define  tt0119_FNAME_LEN                              16
#define  tt0119_MID_INIT_LEN                       1
#define  tt0119_LNAME_LEN                              16
#define  tt0119_COMP_NAME_LEN                      30
#define  tt0119_STREET_LEN                             30
#define  tt0119_REF1_LEN                               30
#define  tt0119_REF2_LEN                               30
#define  tt0119_CITY_LEN                               30
#define  tt0119_STATE_LEN                              2
#define  tt0119_ZIPCODE_LEN                            14
#define  tt0119_TOT_AVAIL_CRED_LEN         14
#define  tt0119_TOT_UNAPL_CRED_LEN         14
#define  tt0119_TOT_DEBITS_LEN             14
#define  tt0119_TOT_OPEN_DEBITS_LEN        14
#define  tt0119_NUM_AVAIL_CRED_LEN         7
#define  tt0119_NUM_UNAPL_CRED_LEN         7
#define  tt0119_NUM_DEBITS_LEN             7

#define  tt0119_AR_DATA_COUNT              20

#define  tt0119_AR_COMPANY_LEN             2
#define  tt0119_AR_DIVISION_LEN            2
#define  tt0119_AR_TRANS_DATE_LEN              8
#define  tt0119_AR_ORIG_REF_LEN            12 
#define  tt0119_AR_TOT_DOL_LEN             14
#define  tt0119_AR_OPEN_DOL_LEN            14
#define  tt0119_AR_APPL_CR_FLAG_LEN                1
#define  tt0119_AR_POST_REF_LEN            12
#define  tt0119_AR_PO_NUMBER_LEN           30
#define  tt0119_AR_DUE_DATE_LEN            8
#define  tt0119_AR_TYPE_LEN                2
#define  tt0119_AR_TYPE_TEXT_LEN           20
#define  tt0119_AR_TYPE_FLAG_LEN           1
#define  tt0119_AR_STATUS_LEN              2
#define  tt0119_AR_COMMENT_LEN             60
#define  tt0119_AR_COMMENT_TYPE_LEN        2
#define  tt0119_AR_COMMENT_DATE_LEN        8


typedef struct
{
    char request_id[tt0119_REQ_ID_LEN+1];
    char record_id[tt0119_REC_ID_LEN+1];
    char company[tt0119_COMPANY_LEN+1];
    char division[tt0119_DIVISION_LEN+1];
    char userid[tt0119_USER_ID_LEN+1];
    char ip_address[tt0119_IP_ADDR_LEN+1];
        char filler[tt0119_FILLER_LEN+1];
    char oa_trans_flag[tt0119_OA_TRANS_FLAG_LEN+1];
    char encrypt_cust_edp[tt0119_ENCRYPT_CUST_EDP_LEN+1];
    char page_num[tt0119_PAGE_NUM_LEN+1];

} tt0119_st_send;

typedef struct
{
    char ar_company[tt0119_AR_COMPANY_LEN+1];
    char ar_division[tt0119_AR_DIVISION_LEN+1];
    char ar_trans_date[tt0119_AR_TRANS_DATE_LEN+1];
    char ar_orig_ref[tt0119_AR_ORIG_REF_LEN+1];
    char ar_tot_dol[tt0119_AR_TOT_DOL_LEN+1];
    char ar_open_dol[tt0119_AR_OPEN_DOL_LEN+1];
    char ar_appl_cr_flag[tt0119_AR_APPL_CR_FLAG_LEN+1];
    char ar_post_ref[tt0119_AR_POST_REF_LEN+1];
    char ar_po_number[tt0119_AR_PO_NUMBER_LEN+1];
    char ar_due_date[tt0119_AR_DUE_DATE_LEN+1];
    char ar_type[tt0119_AR_TYPE_LEN+1];
    char ar_type_text[tt0119_AR_TYPE_TEXT_LEN+1];
    char ar_type_flag[tt0119_AR_TYPE_FLAG_LEN+1];
    char ar_status[tt0119_AR_STATUS_LEN+1];
    char ar_comment[tt0119_AR_COMMENT_LEN+1];
    char ar_comment_type[tt0119_AR_COMMENT_TYPE_LEN+1];
    char ar_comment_date[tt0119_AR_COMMENT_DATE_LEN+1];

} tt0119_ar_data_array;

typedef struct
{
    char request_id[tt0119_REQ_ID_LEN+1];
    char record_id[tt0119_REC_ID_LEN+1];
    char userid[tt0119_USER_ID_LEN+1];
    char success_flag[tt0119_SUCCESS_FLAG_LEN+1];
        char err_message[tt0119_ERR_LEN+1];
        char filler[tt0119_SEND_FILLER_LEN+1];

    char oa_trans_flag[tt0119_OA_TRANS_FLAG_LEN+1];
    char oa_curr_page[tt0119_OA_CURR_PAGE_LEN+1];
    char num_valid_openar[tt0119_NUM_VALID_OPENAR_LEN+1];
    char currency_symbol[tt0119_CURRENCY_SYMBOL_LEN+1];
    char encrypt_cust_edp[tt0119_ENCRYPT_CUST_EDP_LEN+1];
    char cust_num[tt0119_CUST_NUM_LEN+1];
    char fname[tt0119_FNAME_LEN+1];
    char mid_init[tt0119_MID_INIT_LEN+1];
    char lname[tt0119_LNAME_LEN+1];
    char comp_name[tt0119_COMP_NAME_LEN+1];
    char street[tt0119_STREET_LEN+1];
    char ref1[tt0119_REF1_LEN+1];
    char ref2[tt0119_REF2_LEN+1];
    char city[tt0119_CITY_LEN+1];
    char state[tt0119_STATE_LEN+1];
    char zipcode[tt0119_ZIPCODE_LEN+1];
    char tot_avail_cred[tt0119_TOT_AVAIL_CRED_LEN+1];
    char tot_unapl_cred[tt0119_TOT_UNAPL_CRED_LEN+1];
    char tot_debits[tt0119_TOT_DEBITS_LEN+1];
    char tot_open_debits[tt0119_TOT_OPEN_DEBITS_LEN+1];
    char num_avail_cred[tt0119_NUM_AVAIL_CRED_LEN+1];
    char num_unapl_cred[tt0119_NUM_UNAPL_CRED_LEN+1];
    char num_debits[tt0119_NUM_DEBITS_LEN+1];
    tt0119_ar_data_array ar_data[tt0119_AR_DATA_COUNT];

} tt0119_st_recv;
