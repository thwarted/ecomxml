/* ft0002_defs.h  Customer Lookup */


#define  ft0002_LAN_SEND_BUF_LEN            2685
#define  ft0002_LAN_RECV_BUF_LEN            30000


/* Web to MACS Buffer */

#define  ft0002_REQ_ID_LEN                 4
#define  ft0002_REC_ID_LEN                 4
#define  ft0002_COMPANY_LEN                2
#define  ft0002_DIVISION_LEN               2
#define  ft0002_USER_ID_LEN                16
#define  ft0002_IP_ADDR_LEN                16
#define  ft0002_FILLER_LEN                 25
#define  ft0002_SEARCH_LEN                 80
#define  ft0002_START_LEN                  8
#define  ft0002_END_LEN                    8
#define  ft0002_PAGE_NO_LEN                4
#define  ft0002_PAGE_CNT_LEN               4
#define  ft0002_SEARCH_TYPE_LEN            2
#define  ft0002_FULL_ORDER_LEN             12
#define  ft0002_MODE_LEN                   1
#define  ft0002_CUST_NO_LEN                10
#define  ft0002_CUST_NO_LOOP               248


/* MACS to Web Buffer */

#define  ft0002_REQ_ID_LEN                 4
#define  ft0002_REC_ID_LEN                 4
#define  ft0002_USER_ID_LEN                16
#define  ft0002_SUCCESS_FLAG_LEN           1
#define  ft0002_ERR_LEN                    80
#define  ft0002_SEND_FILLER_LEN            45
#define  ft0002_CNT_LEN                    3
#define  ft0002_PAGE_NO_LEN                4
#define  ft0002_CUST_NO_LEN                10
#define  ft0002_NAME_LEN                   16
#define  ft0002_MI_LEN                     2
#define  ft0002_COMP_LEN                   30
#define  ft0002_STREET_LEN				   30	
#define  ft0002_CITY_LEN                   30
#define  ft0002_STATE_LEN                  2
#define  ft0002_ZIP_LEN                    14
#define  ft0002_TYPE_LEN                   20
#define  ft0002_STATUS_LEN                 20
#define  ft0002_ACTION_LEN                 20
#define  ft0002_FLAG_LEN                   1
#define  ft0002_USER_LEN                   8
#define  ft0002_DATE_LEN                   8
#define  ft0002_TIME_LEN                   4
#define  ft0002_ORDER_LEN                  8
#define  ft0002_REF_LEN                    30 
#define  ft0002_COUNTRY_CODE_LEN           4               
#define  ft0002_COUNTRY_LEN                20
#define  ft0002_PHONE_LEN                  16
#define  ft0002_EMAIL_LEN                  50
#define  ft0002_TITLE_CODE_LEN             2
#define  ft0002_TITLE_LEN                  12
#define  ft0002_TYPE_CODE_LEN              2
#define  ft0002_TYPE_LEN                   20
#define  ft0002_STATUS_CODE_LEN            1
#define  ft0002_ACTION_CODE_LEN            1
#define  ft0002_ACTION_LEN                 20
#define  ft0002_CUST_WORTH_LEN             4
#define  ft0002_DATE_LEN                   8
#define  ft0002_TOT_ORDER_LEN              4 
#define  ft0002_TOT_DOL_LEN                9
#define  ft0002_PASSWD_LEN                 16
#define  ft0002_SOURCE_LEN                 16
#define  ft0002_CREDIT_LEN                 10
#define  ft0002_NO_PROMO_CODE_LEN          1 
#define  ft0002_NO_PROMO_LEN               20
#define  ft0002_NO_RENT_CODE_LEN           1
#define  ft0002_NO_RENT_LEN                20
#define  ft0002_DAY_LEN                    4
#define  ft0002_PCT_LEN                    5
#define  ft0002_CR_LIMIT_LEN               7
#define  ft0002_CONTRACT_LEN               8
#define  ft0002_COD_LIMIT_LEN              7
#define  ft0002_CUST_EDP_LEN               9
#define  ft0002_MEMBER_STATUS_LEN          1
#define  ft0002_GENDER_CODE_LEN            1
#define  ft0002_GENDER_LEN                 20
#define  ft0002_LANG_CODE_LEN              1
#define  ft0002_LANG_LEN                   20
#define  ft0002_UPS_TYPE_LEN               1
#define  ft0002_CUSTOMER_LOOP              248
#define  ft0002_NS_LOOP                    100
#define  ft0002_DEATIL_LOOP                46

typedef struct
{
    char request_id[ft0002_REQ_ID_LEN+1];
    char record_id[ft0002_REC_ID_LEN+1];
    char company[ft0002_COMPANY_LEN+1];
    char division[ft0002_DIVISION_LEN+1];
    char userid[ft0002_USER_ID_LEN+1];
    char ip_address[ft0002_IP_ADDR_LEN+1];
    char filler[ft0002_FILLER_LEN+1];
	char search[ft0002_SEARCH_LEN+1];
	char first_start[ft0002_START_LEN+1];
	char first_end[ft0002_END_LEN+1];
	char last_start[ft0002_START_LEN+1];
	char last_end[ft0002_END_LEN+1];
	char page_no[ft0002_PAGE_NO_LEN+1];
	char page_cnt[ft0002_PAGE_CNT_LEN+1];
	char search_type[ft0002_SEARCH_TYPE_LEN+1];
	char cust_mode[ft0002_MODE_LEN+1];
	char full_order_no[ft0002_FULL_ORDER_LEN+1];
	char list_mode[ft0002_MODE_LEN+1];
    char cust_no[ft0002_CUST_NO_LOOP][ft0002_CUST_NO_LEN+1];

} ft0002_st_send;

typedef struct
{
    char cust_num[ft0002_CUST_NO_LEN+1];
    char fname[ft0002_NAME_LEN+1];
    char lname[ft0002_NAME_LEN+1];
    char minitial[ft0002_MI_LEN+1];
    char company[ft0002_COMP_LEN+1];
    char city[ft0002_CITY_LEN+1];
    char state[ft0002_STATE_LEN+1];
    char zip[ft0002_ZIP_LEN+1];

} customer_02;

typedef struct
{
    char cust_num[ft0002_CUST_NO_LEN+1];
    char fname[ft0002_NAME_LEN+1];
    char lname[ft0002_NAME_LEN+1];
    char minitial[ft0002_MI_LEN+1];
    char company[ft0002_COMP_LEN+1];
    char street[ft0002_STREET_LEN+1];
    char city[ft0002_CITY_LEN+1];
    char state[ft0002_STATE_LEN+1];
    char zip[ft0002_ZIP_LEN+1];
    char type[ft0002_TYPE_LEN+1];
    char status[ft0002_STATUS_LEN+1];
    char action[ft0002_ACTION_LEN+1];
    char old_cust_flag[ft0002_FLAG_LEN+1];
    char chg_user[ft0002_USER_LEN+1];
    char chg_date[ft0002_DATE_LEN+1];
    char chg_time[ft0002_TIME_LEN+1];
    char order_no[ft0002_ORDER_LEN+1];

} ns_02;

typedef struct
{
    char cust_num[ft0002_CUST_NO_LEN+1];
    char fname[ft0002_NAME_LEN+1];
    char lname[ft0002_NAME_LEN+1];
    char minitial[ft0002_MI_LEN+1];
    char company[ft0002_COMP_LEN+1];
    char ref1[ft0002_REF_LEN+1];
    char ref2[ft0002_REF_LEN+1];
    char street[ft0002_STREET_LEN+1];
    char city[ft0002_CITY_LEN+1];
    char state[ft0002_STATE_LEN+1];
    char zip[ft0002_ZIP_LEN+1];
    char country_code[ft0002_COUNTRY_CODE_LEN+1];
    char country[ft0002_COUNTRY_LEN+1];
    char day_phone[ft0002_PHONE_LEN+1];
    char night_phone[ft0002_PHONE_LEN+1];
    char fax_phone[ft0002_PHONE_LEN+1];
    char email[ft0002_EMAIL_LEN+1];
    char title_code[ft0002_TITLE_CODE_LEN+1];
    char title[ft0002_TITLE_LEN+1];
    char type_code[ft0002_TYPE_CODE_LEN+1];
    char type[ft0002_TYPE_LEN+1];
    char status_code[ft0002_STATUS_CODE_LEN+1];
    char status[ft0002_STATUS_LEN+1];
    char action_code[ft0002_ACTION_CODE_LEN+1];
    char action[ft0002_ACTION_LEN+1];
    char cust_worth[ft0002_CUST_WORTH_LEN+1];
    char last_date[ft0002_DATE_LEN+1];
    char total_order[ft0002_TOT_ORDER_LEN+1];
    char total_dol[ft0002_TOT_DOL_LEN+1];
    char password[ft0002_PASSWD_LEN+1];
    char last_mail_source[ft0002_SOURCE_LEN+1];
    char old_cust_flag[ft0002_FLAG_LEN+1];
    char credits[ft0002_CREDIT_LEN+1];
    char no_promo_code[ft0002_NO_PROMO_CODE_LEN+1];
    char no_promo[ft0002_NO_PROMO_LEN+1];
    char no_rent_code[ft0002_NO_RENT_CODE_LEN+1];
    char no_rent[ft0002_NO_RENT_LEN+1];
    char terms_days[ft0002_DAY_LEN+1];
    char terms_pct[ft0002_PCT_LEN+1];
    char cr_limit[ft0002_CR_LIMIT_LEN+1];
    char contract[ft0002_CONTRACT_LEN+1];
    char cod_limit[ft0002_COD_LIMIT_LEN+1];
    char cust_edp[ft0002_CUST_EDP_LEN+1];
    char reg_flag[ft0002_FLAG_LEN+1];
    char first_date[ft0002_DATE_LEN+1];
    char old_cust_no[ft0002_CUST_NO_LEN+1];
    char orig_source[ft0002_SOURCE_LEN+1];
    char member_status[ft0002_MEMBER_STATUS_LEN+1];
    char gender_code[ft0002_GENDER_CODE_LEN+1];
    char gender[ft0002_GENDER_LEN+1];
    char lang_code[ft0002_LANG_CODE_LEN+1];
    char language[ft0002_LANG_LEN+1];
    char ups_type[ft0002_UPS_TYPE_LEN+1];

} detail_02;

typedef struct
{
    char request_id[ft0002_REQ_ID_LEN+1];
    char record_id[ft0002_REC_ID_LEN+1];
    char userid[ft0002_USER_ID_LEN+1];
    char success_flag[ft0002_SUCCESS_FLAG_LEN+1];
    char err_message[ft0002_ERR_LEN+1];
    char filler[ft0002_SEND_FILLER_LEN+1];
	char cust_count[ft0002_CNT_LEN+1];
	char page_no[ft0002_PAGE_NO_LEN+1];
	customer_02 customer_info[ft0002_CUSTOMER_LOOP];
	ns_02 ns_info[ft0002_NS_LOOP];
	detail_02 detail_info[ft0002_DEATIL_LOOP];

} ft0002_st_recv;


