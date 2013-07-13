/* tt0009_defs.h  Customer Inquiry */


#define  tt0009_LAN_SEND_BUF_LEN          338  //337  
#define  tt0009_LAN_RECV_BUF_LEN          732  //545  //528  //525  

/* Web to MACS Buffer  */

#define  tt0009_REQ_ID_LEN                4
#define  tt0009_REC_ID_LEN                4
#define  tt0009_COMPANY_LEN               2
#define  tt0009_DIVISION_LEN              2
#define  tt0009_USER_ID_LEN               16
#define  tt0009_IP_ADDR_LEN               16
#define  tt0009_FILLER_LEN                25

#define  tt0009_PASSWORD_LEN              16
#define  tt0009_ORD_NO_LEN                8
#define  tt0009_EDP_LEN                   9
#define  tt0009_CART_UPDATE_LEN           1
#define  tt0009_FN_LEN                    16
#define  tt0009_LN_LEN                    16
#define  tt0009_COMP_NAME_LEN             30
#define  tt0009_STREET_LEN                30
#define  tt0009_CITY_LEN                  30
#define  tt0009_STATE_LEN                 2
#define  tt0009_ZIP_LEN                   14
#define  tt0009_COUNTRY_LEN               4
#define  tt0009_PHONE_LEN                 16
#define  tt0009_EMAIL_LEN                 50
#define  tt0009_CUST_NO_LEN               10
#define  tt0009_CUSTOMER_LOGON_LEN        1

/* MACS to Web Buffer  */

#define  tt0009_REQ_ID_LEN                4
#define  tt0009_REC_ID_LEN                4
#define  tt0009_USER_ID_LEN               16
#define  tt0009_SUCCESS_FLAG_LEN          1
#define  tt0009_ERR_LEN                   80
#define  tt0009_SEND_FILLER_LEN           45



#define  tt0009_PASSWORD_LEN              16
#define  tt0009_MORE_LEN                  1
#define  tt0009_EDP_LEN                   9
#define  tt0009_TITLE_LEN                 2
#define  tt0009_FN_LEN                    16
#define  tt0009_LN_LEN                    16
#define  tt0009_COMP_NAME_LEN             30
#define  tt0009_STREET_LEN                30
#define  tt0009_CITY_LEN                  30
#define  tt0009_STATE_LEN                 2
#define  tt0009_ZIP_LEN                   14
#define  tt0009_RET_COUNTRY_LEN           20
#define  tt0009_PHONE_LEN                 16
#define  tt0009_EMAIL_LEN                 50
#define  tt0009_REF_LEN                   30
#define  tt0009_CUST_NO_LEN               10
#define  tt0009_RETURNING_CUST_LEN        1
#define  tt0009_STAND_ORD_CUST_LEN        1
#define  tt0009_CUSTOMER_LOGON_LEN        1
#define  tt0009_ENCRYPT_CUST_EDP_LEN      17
#define  tt0009_BIRTHDATE_LEN             8
#define  tt0009_FLAG_LEN                  1
#define  tt0009_UPS_TYPE_LEN              1
#define  tt0009_GENDER_LEN                1
#define  tt0009_LANGUAGE_LEN              1
#define  tt0009_POST_CODE_LEN             2
#define  tt0009_CUST_TYPE_LEN             2
#define  tt0009_CUST_STATUS_LEN           2
#define  tt0009_PROMOTE_LEN               1
#define  tt0009_RENT_LEN                  1
#define  tt0009_MEMBER_ID_LEN             11
#define  tt0009_CONTRACT_NO_LEN           8
#define  tt0009_CREDIT_LIMIT_LEN          9
#define  tt0009_TERMS_DAYS_LEN            4
#define  tt0009_TERMS_PCT_LEN             5
#define  tt0009_NORM_DAYS_LEN             4
#define  tt0009_TOTAL_OPEN_LEN            12
#define  tt0009_TOTAL_POTENTIAL_LEN       12
#define  tt0009_DISC_PCT_LEN              5
#define  tt0009_COD_LIMIT_LEN             9
#define  tt0009_COD_COMMENT_CD_LEN        3
#define  tt0009_SALES_TERRITORY_LEN       4
#define  tt0009_SALES_REP_LEN             4
#define  tt0009_INV_REQ_LEN               2
#define  tt0009_INV_HAND_LEN              2
#define  tt0009_TAX_EXEMPT_LEN            1
#define  tt0009_STD_DISC_LEN              1
#define  tt0009_TAX_ID_LEN                15
#define  tt0009_MBR_DURATION_LEN          2
#define  tt0009_MBR_ACT_CHAIN_LEN         4
#define  tt0009_MBR_EXP_CODE_LEN          1
#define  tt0009_POSTG_EXMPT_LEN           1
#define  tt0009_INSUR_EXMPT_LEN           1
#define  tt0009_MBR_INIT_DATE_LEN         8
#define  tt0009_SUPP_STMT_LEN             1
#define  tt0009_BILL_COMPLETE_LEN         1
#define  tt0009_CA_STATUS_LEN             2
#define  tt0009_MBR_EXP_DATE_LEN          8
#define  tt0009_MBR_STATUS_LEN            1
#define  tt0009_ORIG_DATE_LEN             8
#define  tt0009_MI_LEN                    1


typedef struct
{
       char request_id[tt0009_REQ_ID_LEN+1];
       char record_id[tt0009_REC_ID_LEN+1];
       char company[tt0009_COMPANY_LEN+1];
       char division[tt0009_DIVISION_LEN+1];
       char userid[tt0009_USER_ID_LEN+1];
       char ip_address[tt0009_IP_ADDR_LEN+1];
           char filler[tt0009_FILLER_LEN+1];
       char password[tt0009_PASSWORD_LEN+1];
       char ord_num[tt0009_ORD_NO_LEN+1];
       char cust_edp[tt0009_EDP_LEN+1];
       char cart_update[tt0009_CART_UPDATE_LEN+1];
       char fname[tt0009_FN_LEN+1];
       char lname[tt0009_LN_LEN+1];
       char comp_name[tt0009_COMP_NAME_LEN+1];
       char street[tt0009_STREET_LEN+1];
       char city[tt0009_CITY_LEN+1];
       char state[tt0009_STATE_LEN+1];
       char zipcode[tt0009_ZIP_LEN+1];
       char country[tt0009_COUNTRY_LEN+1];
       char day_phone[tt0009_PHONE_LEN+1];
       char night_phone[tt0009_PHONE_LEN+1];
       char email_addr[tt0009_EMAIL_LEN+1];
           char cust_num[tt0009_CUST_NO_LEN+1];
           char customer_logon[tt0009_CUSTOMER_LOGON_LEN+1];

} tt0009_st_send;


typedef struct
{
       char request_id[tt0009_REQ_ID_LEN+1];
       char record_id[tt0009_REC_ID_LEN+1];
       char userid[tt0009_USER_ID_LEN+1];
       char success[tt0009_SUCCESS_FLAG_LEN+1];
       char err_message[tt0009_ERR_LEN+1];
       char filler[tt0009_SEND_FILLER_LEN+1];

       char password[tt0009_PASSWORD_LEN+1];
       char more_flag[tt0009_MORE_LEN+1];
       char cust_edp[tt0009_EDP_LEN+1];
       char title[tt0009_TITLE_LEN+1];
	   char fname[tt0009_FN_LEN+1];
       char lname[tt0009_LN_LEN+1];
       char comp_name[tt0009_COMP_NAME_LEN+1];
       char street[tt0009_STREET_LEN+1];
       char city[tt0009_CITY_LEN+1];
       char state[tt0009_STATE_LEN+1];
       char zipcode[tt0009_ZIP_LEN+1];
       char country[tt0009_RET_COUNTRY_LEN+1];
       char day_phone[tt0009_PHONE_LEN+1];
       char night_phone[tt0009_PHONE_LEN+1];
	   char fax_phone[tt0009_PHONE_LEN+1];
       char email_addr[tt0009_EMAIL_LEN+1];
       char ref1[tt0009_REF_LEN+1];
       char ref2[tt0009_REF_LEN+1];
       char cust_num[tt0009_CUST_NO_LEN+1];
       char returning_cust[tt0009_RETURNING_CUST_LEN+1];
       char stand_ord_cust[tt0009_STAND_ORD_CUST_LEN+1];
       char customer_logon[tt0009_CUSTOMER_LOGON_LEN+1];
       char encrypt_cust_edp[tt0009_ENCRYPT_CUST_EDP_LEN+1];
	   char birthdate[tt0009_BIRTHDATE_LEN+1];
	   char ref_flag[tt0009_FLAG_LEN+1];
	   char ups_type[tt0009_UPS_TYPE_LEN+1];
	   char gender[tt0009_GENDER_LEN+1];
	   char language[tt0009_LANGUAGE_LEN+1];
	   char post_code[tt0009_POST_CODE_LEN+1];
	   char cust_type[tt0009_CUST_TYPE_LEN+1];
	   char cust_stat[tt0009_CUST_STATUS_LEN+1];
	   char promote[tt0009_PROMOTE_LEN+1];
	   char rent[tt0009_RENT_LEN+1];
	   char member_id[tt0009_MEMBER_ID_LEN+1];
	   char contract_no[tt0009_CONTRACT_NO_LEN+1];
	   char credit_limit[tt0009_CREDIT_LIMIT_LEN+1];
	   char terms_days[tt0009_TERMS_DAYS_LEN+1];
	   char terms_pct[tt0009_TERMS_PCT_LEN+1];
	   char norm_days[tt0009_NORM_DAYS_LEN+1];
	   char total_open[tt0009_TOTAL_OPEN_LEN+1];
	   char total_potential[tt0009_TOTAL_POTENTIAL_LEN+1];
	   char disc_pct[tt0009_DISC_PCT_LEN+1];
	   char cod_limit[tt0009_COD_LIMIT_LEN+1];
	   char cod_comment_cd[tt0009_COD_COMMENT_CD_LEN+1];
	   char sales_territory[tt0009_SALES_TERRITORY_LEN+1];
	   char sales_rep[tt0009_SALES_REP_LEN+1];
	   char inv_req[tt0009_INV_REQ_LEN+1];
	   char inv_hand[tt0009_INV_HAND_LEN+1];
	   char tax_exempt[tt0009_TAX_EXEMPT_LEN+1];
	   char std_disc[tt0009_STD_DISC_LEN+1];
	   char tax_id[tt0009_TAX_ID_LEN+1];
	   char mbr_duration[tt0009_MBR_DURATION_LEN+1];
	   char mbr_act_chain[tt0009_MBR_ACT_CHAIN_LEN+1];
	   char mbr_exp_code[tt0009_MBR_EXP_CODE_LEN+1];
	   char postg_exmpt[tt0009_POSTG_EXMPT_LEN+1];
	   char insur_exmpt[tt0009_INSUR_EXMPT_LEN+1];
	   char mbr_init_date[tt0009_MBR_INIT_DATE_LEN+1];
	   char supp_stmt[tt0009_SUPP_STMT_LEN+1];
	   char bill_complete[tt0009_BILL_COMPLETE_LEN+1];
	   char ca_status[tt0009_CA_STATUS_LEN+1];
	   char mbr_exp_date[tt0009_MBR_EXP_DATE_LEN+1];
	   char mbr_status[tt0009_MBR_STATUS_LEN+1];
	   char orig_date[tt0009_ORIG_DATE_LEN+1];
	   char middle_ini[tt0009_MI_LEN+1];

} tt0009_st_recv;