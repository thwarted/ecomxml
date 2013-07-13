/* tt0047_defs.h  Get Order Info */

/* Web to MACS Buffer  */

#define  tt0047_LAN_SEND_BUF_LEN       69   //42    
#define  tt0047_LAN_RECV_BUF_LEN       1010 //1006 //926   

#define  tt0047_REQ_ID_LEN             4
#define  tt0047_REC_ID_LEN             4
#define  tt0047_COMPANY_LEN            2
#define  tt0047_DIVISION_LEN           2
#define  tt0047_USER_ID_LEN            16
#define  tt0047_IP_ADDR_LEN            16
#define  tt0047_FILLER_LEN             25

/* MACS to Web Buffer  */

#define  tt0047_REQ_ID_LEN             4
#define  tt0047_REC_ID_LEN             4
#define  tt0047_USER_ID_LEN            16
#define  tt0047_SUCCESS_FLAG_LEN       1
#define  tt0047_ERR_LEN                80
#define  tt0047_SEND_FILLER_LEN        45


#define  tt0047_PAY_METH_LEN           2
#define  tt0047_FN_LEN                 16
#define  tt0047_MI_LEN                 1
#define  tt0047_LN_LEN                 16
#define  tt0047_TITLE_LEN              2
#define  tt0047_COMP_LEN               30
#define  tt0047_JAP_LEN                1
#define  tt0047_REF_LEN                30
#define  tt0047_STREET_LEN             30
#define  tt0047_CITY_LEN               30
#define  tt0047_STATE_LEN              2
#define  tt0047_ZIP_LEN                14
#define  tt0047_COUNTRY_LEN            4
#define  tt0047_PHONE_LEN              14
#define  tt0047_PROMO_LEN              1
#define  tt0047_RENT_LEN               1
#define  tt0047_REMIT_LEN              8
#define  tt0047_FO_LEN                 8
#define  tt0047_PO_LEN                 20
#define  tt0047_HOLD_COMP_LEN          1
#define  tt0047_EXP_ORD_LEN            1
#define  tt0047_BACKORD_LEN            1
#define  tt0047_POST_ORD_LEN           1
#define  tt0047_INSUR_LEN              1
#define  tt0047_UP_FRT_LEN             1
#define  tt0047_OVERPAYMENT_LEN        1
#define  tt0047_STAND_ORD_LEN          1
#define  tt0047_GENDER_LEN             1
#define  tt0047_SHIP_METH_LEN          2
#define  tt0047_DEMOGRAPH_LEN          50
#define  tt0047_FAX_LEN                14
#define  tt0047_ORD_XREF_LEN           24
#define  tt0047_CUST_TYPE_LEN          2
#define  tt0047_INSTAL_LEN             1
#define  tt0047_DISC_LEN               1
#define  tt0047_BIRTHDAY_LEN           8
#define  tt0047_PASSWORD_LEN           16
#define  tt0047_EMAIL_LEN              50
#define  tt0047_CC_TYPE_LEN            2
#define  tt0047_CC_NUM_LEN             20
#define  tt0047_CC_EXP_LEN             2
#define  tt0047_SHIPTO_NO_LEN          2
#define  tt0047_SHIPTO_FN_LEN          16
#define  tt0047_SHIPTO_MI_LEN          1
#define  tt0047_SHIPTO_LN_LEN          16
#define  tt0047_SHIPTO_TITLE_LEN       2
#define  tt0047_SHIPTO_COMP_LEN        30
#define  tt0047_SHIPTO_JAP_LEN         1
#define  tt0047_SHIPTO_REF_LEN         30
#define  tt0047_SHIPTO_STREET_LEN      30
#define  tt0047_SHIPTO_CITY_LEN        30
#define  tt0047_SHIPTO_STATE_LEN       2
#define  tt0047_SHIPTO_ZIP_LEN         14
#define  tt0047_SHIPTO_COUNTRY_LEN     4
#define  tt0047_SHIPTO_PHONE_LEN       14 
#define  tt0047_SHIP_METHOD_DESC_LEN   80
#define  tt0047_CC_SEC_CODE_LEN        4



typedef struct
{
    char request_id[tt0047_REQ_ID_LEN+1];
    char record_id[tt0047_REC_ID_LEN+1];
    char company[tt0047_COMPANY_LEN+1];
    char division[tt0047_DIVISION_LEN+1];
    char userid[tt0047_USER_ID_LEN+1];
    char ip_address[tt0047_IP_ADDR_LEN+1];
    char filler[tt0047_FILLER_LEN+1];
        
} tt0047_st_send;


typedef struct
{
    char request_id[tt0047_REQ_ID_LEN+1];
    char record_id[tt0047_REC_ID_LEN+1];
    char userid[tt0047_USER_ID_LEN+1];
    char success[tt0047_SUCCESS_FLAG_LEN+1];
    char err_message[tt0047_ERR_LEN+1];
    char filler[tt0047_SEND_FILLER_LEN+1];

    char pay_method[tt0047_PAY_METH_LEN+1];
    char fname[tt0047_FN_LEN+1];
    char minit[tt0047_MI_LEN+1];
    char lname[tt0047_LN_LEN+1];
    char title_code[tt0047_TITLE_LEN+1];
    char company_name[tt0047_COMP_LEN+1];
    char jap_flag[tt0047_JAP_LEN+1];
    char addr_ref1[tt0047_REF_LEN+1];
    char addr_ref2[tt0047_REF_LEN+1];
    char street[tt0047_STREET_LEN+1];
    char city[tt0047_CITY_LEN+1];
    char state[tt0047_STATE_LEN+1];
    char zipcode[tt0047_ZIP_LEN+1];
    char country[tt0047_COUNTRY_LEN+1];
    char day_phone[tt0047_PHONE_LEN+1];
    char ngt_phone[tt0047_PHONE_LEN+1];
    char no_promo[tt0047_PROMO_LEN+1];
    char no_rent[tt0047_RENT_LEN+1];
    char remit_amt[tt0047_REMIT_LEN+1];
    char future_date[tt0047_FO_LEN+1];
    char po_num[tt0047_PO_LEN+1];
    char hold_complete[tt0047_HOLD_COMP_LEN+1];
    char expedite_ord[tt0047_EXP_ORD_LEN+1];
    char no_backord[tt0047_BACKORD_LEN+1];
    char postpone_ord[tt0047_POST_ORD_LEN+1];
    char no_insurance[tt0047_INSUR_LEN+1];
    char cc_up_front[tt0047_UP_FRT_LEN+1];
    char overpayment[tt0047_OVERPAYMENT_LEN+1];
    char standing_ord[tt0047_STAND_ORD_LEN+1];
    char gender[tt0047_GENDER_LEN+1];
    char ship_method[tt0047_SHIP_METH_LEN+1];
    char demograph1[tt0047_DEMOGRAPH_LEN+1];
    char demograph2[tt0047_DEMOGRAPH_LEN+1];
    char fax_phone[tt0047_FAX_LEN+1];
    char order_xref[tt0047_ORD_XREF_LEN+1];
    char cust_type[tt0047_CUST_TYPE_LEN+1];
    char installments[tt0047_INSTAL_LEN+1];
    char discount_code1[tt0047_DISC_LEN+1];
    char discount_code2[tt0047_DISC_LEN+1];
    char birthday[tt0047_BIRTHDAY_LEN+1];
    char password[tt0047_PASSWORD_LEN+1];
    char email_address[tt0047_EMAIL_LEN+1];
    char cc_type[tt0047_CC_TYPE_LEN+1];
    char cc_num[tt0047_CC_NUM_LEN+1];
    char cc_exp_month[tt0047_CC_EXP_LEN+1];
    char cc_exp_year[tt0047_CC_EXP_LEN+1];
    char shipto_num[tt0047_SHIPTO_NO_LEN+1];
    char shipto_fname[tt0047_SHIPTO_FN_LEN+1];
    char shipto_minit[tt0047_SHIPTO_MI_LEN+1];
    char shipto_lname[tt0047_SHIPTO_LN_LEN+1];
    char shipto_title_code[tt0047_SHIPTO_TITLE_LEN+1];
    char shipto_company_name[tt0047_SHIPTO_COMP_LEN+1];
    char shipto_jap_flag[tt0047_JAP_LEN+1];
    char shipto_addr_ref1[tt0047_SHIPTO_REF_LEN+1];
    char shipto_addr_ref2[tt0047_SHIPTO_REF_LEN+1];
    char shipto_street[tt0047_SHIPTO_STREET_LEN+1];
    char shipto_city[tt0047_SHIPTO_CITY_LEN+1];
    char shipto_state[tt0047_SHIPTO_STATE_LEN+1];
    char shipto_zipcode[tt0047_SHIPTO_ZIP_LEN+1];
    char shipto_country[tt0047_SHIPTO_COUNTRY_LEN+1];
    char shipto_phone[tt0047_SHIPTO_PHONE_LEN+1];
    char ship_method_desc[tt0047_SHIP_METHOD_DESC_LEN+1];
	char cc_sec_code[tt0047_CC_SEC_CODE_LEN+1];

} tt0047_st_recv;

