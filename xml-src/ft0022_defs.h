/* ft0022_defs.h  Update Customer Data */


#define  ft0022_LAN_SEND_BUF_LEN            437
#define  ft0022_LAN_RECV_BUF_LEN            169


/* Web to MACS Buffer */

#define  ft0022_REQ_ID_LEN                 4
#define  ft0022_REC_ID_LEN                 4
#define  ft0022_COMPANY_LEN                2
#define  ft0022_DIVISION_LEN               2
#define  ft0022_USER_ID_LEN                16
#define  ft0022_IP_ADDR_LEN                16
#define  ft0022_FILLER_LEN                 25
#define  ft0022_CUST_NO_LEN                10
#define  ft0022_FNAME_LEN                  16
#define  ft0022_LNAME_LEN                  16
#define  ft0022_MI_LEN                     1
#define  ft0022_COMP_NAME_LEN              30
#define  ft0022_REF1_LEN                   30
#define  ft0022_REF2_LEN                   30
#define  ft0022_STREET_LEN                 30
#define  ft0022_CITY_LEN                   30
#define  ft0022_STATE_LEN                  2 
#define  ft0022_ZIP_LEN                    14
#define  ft0022_COUNTRY_CODE_LEN           4
#define  ft0022_DAY_PHONE_LEN              16
#define  ft0022_NIGHT_PHONE_LEN            16
#define  ft0022_FAX_PHONE_LEN              16
#define  ft0022_EMAIL_LEN                  50
#define  ft0022_TITLE_CODE_LEN             2
#define  ft0022_TYPE_LEN                   1
#define  ft0022_STATUS_LEN                 1
#define  ft0022_ACTION_LEN                 1
#define  ft0022_XREF_TYPE_LEN              2
#define  ft0022_XREF_CUST_NO_LEN           10
#define  ft0022_SOURCE_LEN                 16
#define  ft0022_UPDATE_WEBPASS_LEN         1
#define  ft0022_WEBPASS_LEN                16
#define  ft0022_NO_PROMO_LEN               1
#define  ft0022_NO_RENT_LEN                1
#define  ft0022_XREF_DELETE_FLAG_LEN       1
#define  ft0022_GENDER_LEN                 1
#define  ft0022_LANGUAGE_LEN               1
#define  ft0022_UPS_TYPE_LEN               1


/* MACS to Web Buffer */

#define  ft0022_REQ_ID_LEN                 4
#define  ft0022_REC_ID_LEN                 4
#define  ft0022_USER_ID_LEN                16
#define  ft0022_SUCCESS_FLAG_LEN           1
#define  ft0022_ERR_LEN                    80
#define  ft0022_SEND_FILLER_LEN            45
#define  ft0022_CUST_EDP_LEN               9


typedef struct
{
    char request_id[ft0022_REQ_ID_LEN+1];
    char record_id[ft0022_REC_ID_LEN+1];
    char company[ft0022_COMPANY_LEN+1];
    char division[ft0022_DIVISION_LEN+1];
    char userid[ft0022_USER_ID_LEN+1];
    char ip_address[ft0022_IP_ADDR_LEN+1];
    char filler[ft0022_FILLER_LEN+1];
	char cust_no[ft0022_CUST_NO_LEN+1];
	char fname[ft0022_FNAME_LEN+1];
	char lname[ft0022_LNAME_LEN+1];
	char mi[ft0022_MI_LEN+1];
	char comp_name[ft0022_COMP_NAME_LEN+1];
	char ref1[ft0022_REF1_LEN+1];
	char ref2[ft0022_REF2_LEN+1];
	char street[ft0022_STREET_LEN+1];
	char city[ft0022_CITY_LEN+1];
	char state[ft0022_STATE_LEN+1];
	char zip[ft0022_ZIP_LEN+1];
	char country_code[ft0022_COUNTRY_CODE_LEN+1];
	char day_phone[ft0022_DAY_PHONE_LEN+1];
	char night_phone[ft0022_NIGHT_PHONE_LEN+1];
	char fax_phone[ft0022_FAX_PHONE_LEN+1];
	char email[ft0022_EMAIL_LEN+1];
	char title_code[ft0022_TITLE_CODE_LEN+1];
	char type[ft0022_TYPE_LEN+1];
	char status[ft0022_STATUS_LEN+1];
	char action[ft0022_ACTION_LEN+1];
	char xref_type[ft0022_XREF_TYPE_LEN+1];
	char xref_cust_no[ft0022_XREF_CUST_NO_LEN+1];
	char source[ft0022_SOURCE_LEN+1];
	char update_webpass[ft0022_UPDATE_WEBPASS_LEN+1];
	char webpass[ft0022_WEBPASS_LEN+1];
	char no_promo[ft0022_NO_PROMO_LEN+1];
	char no_rent[ft0022_NO_RENT_LEN+1];
	char xref_delete_flag[ft0022_XREF_DELETE_FLAG_LEN+1];
	char gender[ft0022_GENDER_LEN+1];
	char language[ft0022_LANGUAGE_LEN+1];
	char ups_type[ft0022_UPS_TYPE_LEN+1];

} ft0022_st_send;


typedef struct
{
    char request_id[ft0022_REQ_ID_LEN+1];
    char record_id[ft0022_REC_ID_LEN+1];
    char userid[ft0022_USER_ID_LEN+1];
    char success_flag[ft0022_SUCCESS_FLAG_LEN+1];
    char err_message[ft0022_ERR_LEN+1];
    char filler[ft0022_SEND_FILLER_LEN+1];
	char cust_no[ft0022_CUST_NO_LEN+1];
	char cust_edp[ft0022_CUST_EDP_LEN+1];

} ft0022_st_recv;


