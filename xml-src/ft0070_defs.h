/* ft0070_defs.h  Review wishlist */


#define  ft0070_LAN_SEND_BUF_LEN            126
#define  ft0070_LAN_RECV_BUF_LEN            6545

/* Web to MACS Buffer */

#define  ft0070_REQ_ID_LEN                 4
#define  ft0070_REC_ID_LEN                 4
#define  ft0070_COMPANY_LEN                2
#define  ft0070_DIVISION_LEN               2
#define  ft0070_USER_ID_LEN                16
#define  ft0070_IP_ADDR_LEN                16
#define  ft0070_FILLER_LEN                 25
#define  ft0070_ID_LEN                     16
#define  ft0070_SEQ_NO_LEN                 2
#define  ft0070_CUST_EDP_LEN               9
#define  ft0070_PASSWD_LEN                 16
#define  ft0070_FLAG_LEN                   1
#define  ft0070_PAGE_NO_LEN                4
#define  ft0070_PAGE_CNT_LEN               4


/* MACS to Web Buffer */

#define  ft0070_REQ_ID_LEN                 4
#define  ft0070_REC_ID_LEN                 4
#define  ft0070_USER_ID_LEN                16
#define  ft0070_SUCCESS_FLAG_LEN           1
#define  ft0070_ERR_LEN                    80
#define  ft0070_SEND_FILLER_LEN            45
#define  ft0070_ID_LEN                     16
#define  ft0070_CUST_EDP_LEN               9
#define  ft0070_NAME_LEN                   16
#define  ft0070_MI_LEN                     2
#define  ft0070_CITY_LEN                   30
#define  ft0070_STATE_LEN                  2
#define  ft0070_STREET_LEN                 30
#define  ft0070_ZIP_LEN                    14
#define  ft0070_PASSWD_LEN                 16
#define  ft0070_ITM_CNT_LEN                3
#define  ft0070_FLAG_LEN                   1
#define  ft0070_DATE_LEN                   8
#define  ft0070_GLB_LEN                    1
#define  ft0070_SOURCE_LEN                 16
#define  ft0070_PHONE_LEN                  16
#define  ft0070_PASSWORD_LEN               16
#define  ft0070_PREF_SHIPTO_LEN            9
#define  ft0070_ITM_EDP_LEN                9
#define  ft0070_ITM_NO_LEN                 20
#define  ft0070_LINE_NO_LEN                4
#define  ft0070_QTY_LEN                    4
#define  ft0070_DESC_LEN                   50
#define  ft0070_PRICE_LEN                  9
#define  ft0070_STATUS_LEN                 1
#define  ft0070_INV_LEN                    9
#define  ft0070_VENDOR_LEN                 10
#define  ft0070_PRODUCT_LOOP               50



typedef struct
{
    char request_id[ft0070_REQ_ID_LEN+1];
    char record_id[ft0070_REC_ID_LEN+1];
    char company[ft0070_COMPANY_LEN+1];
    char division[ft0070_DIVISION_LEN+1];
    char userid[ft0070_USER_ID_LEN+1];
    char ip_address[ft0070_IP_ADDR_LEN+1];
    char filler[ft0070_FILLER_LEN+1];
	char wl_company[ft0070_COMPANY_LEN+1];
	char wl_division[ft0070_DIVISION_LEN+1];
	char wl_id[ft0070_ID_LEN+1];
	char seq_no[ft0070_SEQ_NO_LEN+1];
	char cust_edp[ft0070_CUST_EDP_LEN+1];
	char password[ft0070_PASSWD_LEN+1];
	char login_flag[ft0070_FLAG_LEN+1];
	char multi_list[ft0070_FLAG_LEN+1];
	char page_no[ft0070_PAGE_NO_LEN+1];
	char page_cnt[ft0070_PAGE_CNT_LEN+1];

} ft0070_st_send;

typedef struct
{
    char item_edp[ft0070_ITM_EDP_LEN+1];
    char item_no[ft0070_ITM_NO_LEN+1];
    char line_no[ft0070_LINE_NO_LEN+1];
    char qty[ft0070_QTY_LEN+1];
    char ordered_qty[ft0070_QTY_LEN+1];
    char desc[ft0070_DESC_LEN+1];
    char custom_flag[ft0070_FLAG_LEN+1];
    char kit_flag[ft0070_FLAG_LEN+1];
    char price[ft0070_PRICE_LEN+1];
    char status[ft0070_STATUS_LEN+1];
    char avail_inv[ft0070_INV_LEN+1];
    char vendor[ft0070_VENDOR_LEN+1];
    
} product_70;


typedef struct
{
    char request_id[ft0070_REQ_ID_LEN+1];
    char record_id[ft0070_REC_ID_LEN+1];
    char userid[ft0070_USER_ID_LEN+1];
    char success_flag[ft0070_SUCCESS_FLAG_LEN+1];
    char err_message[ft0070_ERR_LEN+1];
    char filler[ft0070_SEND_FILLER_LEN+1];
    char wl_id[ft0070_ID_LEN+1];
    char cust_edp[ft0070_CUST_EDP_LEN+1];
    char fname[ft0070_NAME_LEN+1];
    char minitial[ft0070_MI_LEN+1];
    char lname[ft0070_NAME_LEN+1];
    char city[ft0070_CITY_LEN+1];
    char state[ft0070_STATE_LEN+1];
    char street[ft0070_STREET_LEN+1];
    char zip[ft0070_ZIP_LEN+1];
    char wpasswd[ft0070_PASSWD_LEN+1];
    char item_cnt[ft0070_ITM_CNT_LEN+1];
    char buyer_flag[ft0070_FLAG_LEN+1];
    char wed_date[ft0070_DATE_LEN+1];
    char ship_date[ft0070_DATE_LEN+1];
    char reg_date[ft0070_DATE_LEN+1];
    char glb[ft0070_GLB_LEN+1];
    char source[ft0070_SOURCE_LEN+1];
    char day_phone[ft0070_PHONE_LEN+1];
    char night_phone[ft0070_PHONE_LEN+1];
    char password[ft0070_PASSWORD_LEN+1];
    char pref_shipto[ft0070_PREF_SHIPTO_LEN+1];
    char g_fname[ft0070_NAME_LEN+1];
    char g_minit[ft0070_MI_LEN+1];
    char g_lname[ft0070_NAME_LEN+1];
	product_70 product_detail[ft0070_PRODUCT_LOOP];

} ft0070_st_recv;


