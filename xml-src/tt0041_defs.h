/* tt0041_defs.h  Add Order Info */

/* Web to MACS Buffer   and   MACS to Web Buffer  */

#define  tt0041_LAN_SEND_BUF_LEN        819 //815 //788    
#define  tt0041_LAN_RECV_BUF_LEN        926 //800 

#define  tt0041_REQ_ID_LEN              4
#define  tt0041_REC_ID_LEN              4
#define  tt0041_COMPANY_LEN             2
#define  tt0041_DIVISION_LEN            2
#define  tt0041_USER_ID_LEN             16
#define  tt0041_IP_ADDR_LEN             16
#define  tt0041_FILLER_LEN              25

#define  tt0041_PAY_METH_LEN            2
#define  tt0041_FN_LEN                  16
#define  tt0041_MI_LEN                  1
#define  tt0041_LN_LEN                  16
#define  tt0041_TITLE_LEN               2
#define  tt0041_COMP_LEN                30
#define  tt0041_JAP_LEN                 1
#define  tt0041_REF_LEN                 30
#define  tt0041_STREET_LEN              30
#define  tt0041_CITY_LEN                30
#define  tt0041_STATE_LEN               2
#define  tt0041_ZIP_LEN                 14
#define  tt0041_COUNTRY_LEN             4
#define  tt0041_PHONE_LEN               14
#define  tt0041_PROMO_LEN               1
#define  tt0041_RENT_LEN                1
#define  tt0041_REMIT_LEN               8
#define  tt0041_FO_LEN                  8
#define  tt0041_PO_LEN                  20
#define  tt0041_HOLD_COMP_LEN           1
#define  tt0041_EXP_ORD_LEN             1
#define  tt0041_BACKORD_LEN             1
#define  tt0041_POST_ORD_LEN            1
#define  tt0041_INSUR_LEN               1
#define  tt0041_UP_FRT_LEN              1
#define  tt0041_OVERPAYMENT_LEN         1
#define  tt0041_STAND_ORD_LEN           1
#define  tt0041_GENDER_LEN              1
#define  tt0041_SHIP_METH_LEN           2
#define  tt0041_DEMOGRAPH_LEN           50
#define  tt0041_FAX_LEN                 14
#define  tt0041_ORD_XREF_LEN            24
#define  tt0041_CUST_TYPE_LEN           2
#define  tt0041_INSTAL_LEN              1
#define  tt0041_DISC_LEN                1
#define  tt0041_BIRTHDAY_LEN            8
#define  tt0041_PASSWORD_LEN            16
#define  tt0041_EMAIL_LEN               50
#define  tt0041_CC_TYPE_LEN             2
#define  tt0041_CC_NUM_LEN              20
#define  tt0041_CC_EXP_LEN              2
#define  tt0041_SHIPTO_NO_LEN           2
#define  tt0041_SHIPTO_FN_LEN           16
#define  tt0041_SHIPTO_MI_LEN           1
#define  tt0041_SHIPTO_LN_LEN           16
#define  tt0041_SHIPTO_TITLE_LEN        2
#define  tt0041_SHIPTO_COMP_LEN         30
#define  tt0041_SHIPTO_JAP_LEN          1
#define  tt0041_SHIPTO_REF_LEN          30
#define  tt0041_SHIPTO_STREET_LEN       30
#define  tt0041_SHIPTO_CITY_LEN         30
#define  tt0041_SHIPTO_STATE_LEN        2
#define  tt0041_SHIPTO_ZIP_LEN          14
#define  tt0041_SHIPTO_COUNTRY_LEN      4
#define  tt0041_SHIPTO_PHONE_LEN        14 
#define  tt0041_CC_SEC_CODE_LEN         4
#define  tt0041_SUCCESS_FLAG_LEN        1
#define  tt0041_ERR_LEN                 80
#define  tt0041_SEND_FILLER_LEN         45



typedef struct
{
    char request_id[tt0041_REQ_ID_LEN+1];
    char record_id[tt0041_REC_ID_LEN+1];
    char company[tt0041_COMPANY_LEN+1];
    char division[tt0041_DIVISION_LEN+1];
    char userid[tt0041_USER_ID_LEN+1];
    char ip_address[tt0041_IP_ADDR_LEN+1];
        char filler[tt0041_FILLER_LEN+1];
        char pay_method[tt0041_PAY_METH_LEN+1];
        char fname[tt0041_FN_LEN+1];
        char minit[tt0041_MI_LEN+1];
        char lname[tt0041_LN_LEN+1];
        char title_code[tt0041_TITLE_LEN+1];
        char company_name[tt0041_COMP_LEN+1];
        char jap_flag[tt0041_JAP_LEN+1];
        char addr_ref1[tt0041_REF_LEN+1];
        char addr_ref2[tt0041_REF_LEN+1];
        char street[tt0041_STREET_LEN+1];
        char city[tt0041_CITY_LEN+1];
        char state[tt0041_STATE_LEN+1];
        char zipcode[tt0041_ZIP_LEN+1];
        char country[tt0041_COUNTRY_LEN+1];
        char day_phone[tt0041_PHONE_LEN+1];
        char ngt_phone[tt0041_PHONE_LEN+1];
        char no_promo[tt0041_PROMO_LEN+1];
        char no_rent[tt0041_RENT_LEN+1];
        char remit_amt[tt0041_REMIT_LEN+1];
        char future_date[tt0041_FO_LEN+1];
        char po_num[tt0041_PO_LEN+1];
        char hold_complete[tt0041_HOLD_COMP_LEN+1];
        char expedite_ord[tt0041_EXP_ORD_LEN+1];
        char no_backord[tt0041_BACKORD_LEN+1];
        char postpone_ord[tt0041_POST_ORD_LEN+1];
        char no_insurance[tt0041_INSUR_LEN+1];
        char cc_up_front[tt0041_UP_FRT_LEN+1];
        char overpayment[tt0041_OVERPAYMENT_LEN+1];
        char standing_ord[tt0041_STAND_ORD_LEN+1];
        char gender[tt0041_GENDER_LEN+1];
        char ship_method[tt0041_SHIP_METH_LEN+1];
        char demograph1[tt0041_DEMOGRAPH_LEN+1];
        char demograph2[tt0041_DEMOGRAPH_LEN+1];
        char fax_phone[tt0041_FAX_LEN+1];
        char order_xref[tt0041_ORD_XREF_LEN+1];
        char cust_type[tt0041_CUST_TYPE_LEN+1];
        char installments[tt0041_INSTAL_LEN+1];
        char discount_code1[tt0041_DISC_LEN+1];
        char discount_code2[tt0041_DISC_LEN+1];
        char birthday[tt0041_BIRTHDAY_LEN+1];
        char password[tt0041_PASSWORD_LEN+1];
        char email_address[tt0041_EMAIL_LEN+1];
        char cc_num[tt0041_CC_NUM_LEN+1];
        char cc_exp_month[tt0041_CC_EXP_LEN+1];
        char cc_exp_year[tt0041_CC_EXP_LEN+1];
        char shipto_fname[tt0041_SHIPTO_FN_LEN+1];
        char shipto_minit[tt0041_SHIPTO_MI_LEN+1];
        char shipto_lname[tt0041_SHIPTO_LN_LEN+1];
        char shipto_title_code[tt0041_SHIPTO_TITLE_LEN+1];
        char shipto_company_name[tt0041_SHIPTO_COMP_LEN+1];
        char shipto_jap_flag[tt0041_JAP_LEN+1];
        char shipto_addr_ref1[tt0041_SHIPTO_REF_LEN+1];
        char shipto_addr_ref2[tt0041_SHIPTO_REF_LEN+1];
        char shipto_street[tt0041_SHIPTO_STREET_LEN+1];
        char shipto_city[tt0041_SHIPTO_CITY_LEN+1];
        char shipto_state[tt0041_SHIPTO_STATE_LEN+1];
        char shipto_zipcode[tt0041_SHIPTO_ZIP_LEN+1];
        char shipto_country[tt0041_SHIPTO_COUNTRY_LEN+1];
        char shipto_phone[tt0041_SHIPTO_PHONE_LEN+1];
		char cc_sec_code[tt0041_CC_SEC_CODE_LEN+1];
        
} tt0041_st_send;



typedef struct
{
    char request_id[tt0041_REQ_ID_LEN+1];
    char record_id[tt0041_REC_ID_LEN+1];
    char userid[tt0041_USER_ID_LEN+1];
    char success[tt0041_SUCCESS_FLAG_LEN+1];
        char err_message[tt0041_ERR_LEN+1];
        char filler[tt0041_SEND_FILLER_LEN+1];

        char pay_method[tt0041_PAY_METH_LEN+1];
        char fname[tt0041_FN_LEN+1];
        char minit[tt0041_MI_LEN+1];
        char lname[tt0041_LN_LEN+1];
        char title_code[tt0041_TITLE_LEN+1];
        char company_name[tt0041_COMP_LEN+1];
        char jap_flag[tt0041_JAP_LEN+1];
        char addr_ref1[tt0041_REF_LEN+1];
        char addr_ref2[tt0041_REF_LEN+1];
        char street[tt0041_STREET_LEN+1];
        char city[tt0041_CITY_LEN+1];
        char state[tt0041_STATE_LEN+1];
        char zipcode[tt0041_ZIP_LEN+1];
        char country[tt0041_COUNTRY_LEN+1];
        char day_phone[tt0041_PHONE_LEN+1];
        char ngt_phone[tt0041_PHONE_LEN+1];
        char no_promo[tt0041_PROMO_LEN+1];
        char no_rent[tt0041_RENT_LEN+1];
        char remit_amt[tt0041_REMIT_LEN+1];
        char future_date[tt0041_FO_LEN+1];
        char po_num[tt0041_PO_LEN+1];
        char hold_complete[tt0041_HOLD_COMP_LEN+1];
        char expedite_ord[tt0041_EXP_ORD_LEN+1];
        char no_backord[tt0041_BACKORD_LEN+1];
        char postpone_ord[tt0041_POST_ORD_LEN+1];
        char no_insurance[tt0041_INSUR_LEN+1];
        char cc_up_front[tt0041_UP_FRT_LEN+1];
        char overpayment[tt0041_OVERPAYMENT_LEN+1];
        char standing_ord[tt0041_STAND_ORD_LEN+1];
        char gender[tt0041_GENDER_LEN+1];
        char ship_method[tt0041_SHIP_METH_LEN+1];
        char demograph1[tt0041_DEMOGRAPH_LEN+1];
        char demograph2[tt0041_DEMOGRAPH_LEN+1];
        char fax_phone[tt0041_FAX_LEN+1];
        char order_xref[tt0041_ORD_XREF_LEN+1];
        char cust_type[tt0041_CUST_TYPE_LEN+1];
        char installments[tt0041_INSTAL_LEN+1];
        char discount_code1[tt0041_DISC_LEN+1];
        char discount_code2[tt0041_DISC_LEN+1];
        char birthday[tt0041_BIRTHDAY_LEN+1];
        char password[tt0041_PASSWORD_LEN+1];
        char email_address[tt0041_EMAIL_LEN+1];
        char cc_type[tt0041_CC_TYPE_LEN+1];
        char cc_num[tt0041_CC_NUM_LEN+1];
        char cc_exp_month[tt0041_CC_EXP_LEN+1];
        char cc_exp_year[tt0041_CC_EXP_LEN+1];
        char shipto_num[tt0041_SHIPTO_NO_LEN+1];
        char shipto_fname[tt0041_SHIPTO_FN_LEN+1];
        char shipto_minit[tt0041_SHIPTO_MI_LEN+1];
        char shipto_lname[tt0041_SHIPTO_LN_LEN+1];
        char shipto_title_code[tt0041_SHIPTO_TITLE_LEN+1];
        char shipto_company_name[tt0041_SHIPTO_COMP_LEN+1];
        char shipto_jap_flag[tt0041_JAP_LEN+1];
        char shipto_addr_ref1[tt0041_SHIPTO_REF_LEN+1];
        char shipto_addr_ref2[tt0041_SHIPTO_REF_LEN+1];
        char shipto_street[tt0041_SHIPTO_STREET_LEN+1];
        char shipto_city[tt0041_SHIPTO_CITY_LEN+1];
        char shipto_state[tt0041_SHIPTO_STATE_LEN+1];
        char shipto_zipcode[tt0041_SHIPTO_ZIP_LEN+1];
        char shipto_country[tt0041_SHIPTO_COUNTRY_LEN+1];
        char shipto_phone[tt0041_SHIPTO_PHONE_LEN+1];

} tt0041_st_recv;

