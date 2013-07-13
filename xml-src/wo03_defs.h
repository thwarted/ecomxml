/* wo03_defs.h  CATALOG REQUEST TRANSACTION*/

#define  wo03_LAN_SEND_BUF_LEN        19269  //19253  //19233 //19208  
#define  wo03_LAN_RECV_BUF_LEN        1117   //1108 //1099  //1095 //1050

// Header Definitions

#define  REQ_ID_LEN                                     4
#define  TRAN_ID_LEN                            4
#define  COMPANY_LEN                            2
#define  DIVISION_LEN                           2
#define  IP_ADDRESS_LEN                         16
#define  UID_LEN                                        16

// 11 record defines

#define  R11_REC_TYPE_LEN                       2
#define  R11_FILLER1_LEN                        8
#define  R11_SRC_LEN                            8
#define  R11_FDATE_LEN                          8
#define  R11_FNAME_LEN              16
#define  R11_MI_LEN                 1
#define  R11_LNAME_LEN              16
#define  R11_TITLE_LEN              2
#define  R11_COMPANY_NAME_LEN       30
#define  R11_REF_LEN                30
#define  R11_STREET_LEN             30
#define  R11_CITY_LEN               30
#define  R11_STATE_LEN              2
#define  R11_ZIP_CODE_LEN           14
#define  R11_PHONE_LEN              14
#define  R11_COUNTRY_LEN            4
#define  R11_CUST_NUM_LEN                       24
#define  R11_CUST_TYPE_LEN                      2
#define  R11_CC_TYPE_LEN                        2
#define  R11_CC_NUM_LEN                         20
#define  R11_CC_EXP_YY_LEN                      2
#define  R11_CC_EXP_MM_LEN                      2
#define  R11_ERROR_LEN                          1
#define  R11_FAX_LEN                            14

// 13 Record Defines

#define  R13_REC_TYPE_LEN                       2
#define  R13_FILLER1_LEN                        8
#define  R13_EMAIL_LEN                          50
#define  R13_NET_PASS_LEN           16
#define  R13_RECAP_FLAG_LEN         1
#define  R13_IP_ADD_LEN                         16
#define  R13_USER_ID_LEN                        16
#define  R13_DEMO_LEN                           100
#define  R13_FILLER2_LEN                        111

// 15 Record Defines

#define  R15_REC_TYPE_LEN                       2
#define  R15_FILLER1_LEN            8
#define  R15_REMIT_LEN                          8
#define  R15_FUT_DATE_LEN                       8
#define  R15_PO_LEN                 20
#define  R15_ENT_DATE_LEN                       8
#define  R15_ENT_TIME_LEN                       6
#define  R15_ENT_BY_LEN                         8
#define  R15_HLD_ORD_LEN                        1
#define  R15_EXPEDITE_LEN           1
#define  R15_NO_BO_LEN                          1
#define  R15_POSTPONE_LEN                       1
#define  R15_NO_INSUR_LEN                       1
#define  R15_CC_UP_FRNT_LEN                     1
#define  R15_OVERPAYMENT_LEN            1
#define  R15_SO_FLAG_LEN                        1
#define  R15_GENDER_LEN                         1
#define  R15_FILLER2_LEN            5
#define  R15_OPT_CODE_LEN                       4
#define  R15_SHIP_METH_LEN          2
#define  R15_DEMO_LEN               100
#define  R15_SALES_TERR_LEN                     4
#define  R15_SALES_REP_LEN                      4
#define  R15_FAX_LEN                            14
#define  R15_ORD_XREF_LEN                       24
#define  R15_CUST_TYPE_LEN                      2
#define  R15_ERR_LEN                            1
#define  R15_INSTALLMENT_LEN            1
#define  R15_SHIP_DATE_LEN          8
#define  R15_USER_HLD_LEN                       1
#define  R15_BANK_LEN                           24
#define  R15_RUT_LEN                            10
#define  R15_FILLER3_LEN            17
#define  R15_CONT_FLAG_LEN                      1
#define  R15_BYPASS_ADV_LEN                     1
#define  R15_DISC_CODE_LEN                      1
#define  R15_BIRTHDAY_LEN                       8
#define  R15_FILLER4_LEN            10


typedef struct
{

    char rec_type[R11_REC_TYPE_LEN+1];
        char filler1[R11_FILLER1_LEN+1];
        char source[R11_SRC_LEN+1];
        char first_date[R11_FDATE_LEN+1];
    char first_name[R11_FNAME_LEN+1];
    char middle_init[R11_MI_LEN+1];
    char last_name[R11_LNAME_LEN+1];
    char title[R11_TITLE_LEN+1];
    char company_name[R11_COMPANY_NAME_LEN+1];
    char address_ref_1[R11_REF_LEN+1];
    char address_ref_2[R11_REF_LEN+1];
    char street[R11_STREET_LEN+1];
    char city[R11_CITY_LEN+1];
    char state[R11_STATE_LEN+1];
    char zip_code[R11_ZIP_CODE_LEN+1];
        char phone[R11_PHONE_LEN+1];
    char country_code[R11_COUNTRY_LEN+1];
    char old_cust_num[R11_CUST_NUM_LEN+1];
        char cust_type[R11_CUST_TYPE_LEN+1];
        char cc_type[R11_CC_TYPE_LEN+1];
        char cc_num[R11_CC_NUM_LEN+1];
        char cc_exp_yy[R11_CC_EXP_YY_LEN+1];
        char cc_exp_mm[R11_CC_EXP_MM_LEN+1];
        char error_code[R11_ERROR_LEN+1];
        char fax_phone[R11_FAX_LEN+1];

} wo03_rec11;


typedef struct
{

        char rec_type[R13_REC_TYPE_LEN+1];
        char filler1[R13_FILLER1_LEN+1];
    char email_addr[R13_EMAIL_LEN+1];
    char net_password[R13_NET_PASS_LEN+1];
    char recap_flag[R13_RECAP_FLAG_LEN+1];
    char ip_address[R13_IP_ADD_LEN+1];
    char user_id[R13_USER_ID_LEN+1];
        char demograph[R13_DEMO_LEN+1];
    char filler2[R13_FILLER2_LEN+1];

} wo03_rec13;


typedef struct
{

        char rec_type[R15_REC_TYPE_LEN+1];
        char filler1[R15_FILLER1_LEN+1];
        char remit_amt[R15_REMIT_LEN+1];
        char future_date[R15_FUT_DATE_LEN+1];
    char po_number[R15_PO_LEN+1];
        char entered_date[R15_ENT_DATE_LEN+1];
        char entered_time[R15_ENT_TIME_LEN+1];
        char entered_by[R15_ENT_BY_LEN+1];
        char hold_order[R15_HLD_ORD_LEN+1];
    char expedite[R15_EXPEDITE_LEN+1];
        char no_backorder[R15_NO_BO_LEN+1];
        char postpone_order[R15_POSTPONE_LEN+1];
        char no_insurance[R15_NO_INSUR_LEN+1];
        char cc_up_front[R15_CC_UP_FRNT_LEN+1];
        char overpayment[R15_OVERPAYMENT_LEN+1];
        char standing_order[R15_SO_FLAG_LEN+1];
        char gender[R15_GENDER_LEN+1];
        char filler2[R15_FILLER2_LEN+1];
        char opt_code[R15_OPT_CODE_LEN+1];
    char ship_method[R15_SHIP_METH_LEN+1];
    char demographic[R15_DEMO_LEN+1];
        char sales_terr[R15_SALES_TERR_LEN+1];
        char sales_rep[R15_SALES_REP_LEN+1];
        char fax_phone[R15_FAX_LEN+1];
        char order_xref[R15_ORD_XREF_LEN+1];
    char cust_type[R15_CUST_TYPE_LEN+1];
    char error_flag[R15_ERR_LEN+1];
        char installment[R15_INSTALLMENT_LEN+1];
        char ship_date[R15_SHIP_DATE_LEN+1];
        char user_hold_code[R15_USER_HLD_LEN+1];
        char bank_acct[R15_BANK_LEN+1];
        char rut_num[R15_RUT_LEN];
        char filler3[R15_FILLER3_LEN+1];
        char continuity_flag[R15_CONT_FLAG_LEN+1];
        char bypass_adv[R15_BYPASS_ADV_LEN+1];
        char disc_code1[R15_DISC_CODE_LEN+1];
        char disc_code2[R15_DISC_CODE_LEN+1];
        char birthday[R15_BIRTHDAY_LEN+1];
        char filler4[R15_FILLER4_LEN+1];
        
} wo03_rec15;


// Reply from MACS

#define  USER_ID_LEN                            16
#define  SUCCESS_FLAG_LEN                   1
#define  REJECT_REASON_LEN          80

#define  PRODUCT_DOL_LEN                        9
#define  DISC_DOL_LEN                       9
#define  TAX_DOL_LEN                        9
#define  POSTAGE_DOL_LEN                        9
#define  INSURANCE_DOL_LEN          9
#define  TOTAL_DOL_LEN                          9
#define  PAY_METH_LEN                       20
//#define  SHIP_METH_LEN                    20
#define  ORD_NUM_LEN                8
#define  SHIP_METH_CHANGE_LEN       1
#define  LINKSHARE_FLAG_LEN                     1
#define  LINKSHARE_ID_LEN                       64
#define  LINKSHARE_ZIP_LEN                      10
#define  LINKSHARE_DATE_OUT_LEN         8
#define  LINKSHARE_TIME_OUT_LEN         6
#define  SHIPTO_NO_LEN                      2
#define  NET_DOL_LEN                            9
#define  GIFT_DOL_LEN                           9

#define  wo03_SHIPTO_COUNT                              20

#define  SHIPTO_ST_NO_LEN               2
#define  SHIPTO_SM_CODE_LEN                     2
//#define  SHIPTO_SM_DESC_LEN                   20
#define  SHIPTO_SM_CHANGE_LEN           1

#define  FLOOZ_PM_LEN                           1
#define  CUST_EDP_LEN                           9

#define  FILLER1_LEN                            216


typedef struct
{
        char shipto_st_num[SHIPTO_ST_NO_LEN+1];
        char shipto_sm_code[SHIPTO_SM_CODE_LEN+1];
        char shipto_sm_desc[SHIPTO_SM_DESC_LEN+1];
        char shipto_sm_change[SHIPTO_SM_CHANGE_LEN+1];

} wo03shipto_array;

typedef struct
{
    char request_id[REQ_ID_LEN+1];
        char tran_id[TRAN_ID_LEN+1];
    char user_id[USER_ID_LEN+1];
    char success_flag[SUCCESS_FLAG_LEN+1];
    char ship_method_change[SHIP_METH_CHANGE_LEN+1];

    char product_dol[PRODUCT_DOL_LEN+1];
    char discount_dol[DISC_DOL_LEN+1];
    char tax_dol[TAX_DOL_LEN+1];
    char postage_dol[POSTAGE_DOL_LEN+1];
    char insurance_dol[INSURANCE_DOL_LEN+1];
    char total_dol[TOTAL_DOL_LEN+1];
    char pay_method[PAY_METH_LEN+1];
    char ship_method[SHIP_METH_LEN+1];
    char order_no[ORD_NUM_LEN+1];
    char reject_reason[REJECT_REASON_LEN+1];
        char linkshare_flag[LINKSHARE_FLAG_LEN+1];
        char linkshare_id[LINKSHARE_ID_LEN+1];
        char linkshare_zip[LINKSHARE_ZIP_LEN+1];
        char linkshare_date_out[LINKSHARE_DATE_OUT_LEN+1];
        char linkshare_time_out[LINKSHARE_TIME_OUT_LEN+1];
        char shipto_num[SHIPTO_NO_LEN+1];
        char net_dol[NET_DOL_LEN+1];
        char gift_dol[GIFT_DOL_LEN+1];
        wo03shipto_array shipto[wo03_SHIPTO_COUNT];
        char flooz_pm[FLOOZ_PM_LEN+1];
        char cust_edp[CUST_EDP_LEN+1];
        char filler1[FILLER1_LEN+1];

} wo03_recv;
typedef struct
{
    wo03_rec11 wo03rec11;
        wo03_rec13 wo03rec13;
        wo03_rec15 wo03rec15;

        char tmaxrec[60][321];

}wo03_send_records;
