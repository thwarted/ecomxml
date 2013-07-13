/* wo01_defs.h  ORDER RECAP TRANSACTION*/

#define  wo01_LAN_SEND_BUF_LEN        96078 //96069 //19605  //19589  //19569 //19553 //19233 //19208  
#define  wo01_LAN_RECV_BUF_LEN        19503 //19479 //9975 //9974 //9965 //2539 //2359  //2355 //1095 //1050  

// Sending and Receiving Header Definitions
#define   NUMBER_OF_RECORDS 300

#define  REQ_ID_LEN                                     4
#define  TRAN_ID_LEN                            4
#define  COMPANY_LEN                            2
#define  DIVISION_LEN                           2
#define  UID_LEN                                        16
#define  IP_ADDRESS_LEN                         16
#define  HEADER_FILLER_LEN                      25

// 10 record defines

#define  R10_REC_TYPE_LEN                       2
#define  R10_ORDER_NUM_LEN                      8
#define  R10_SRC_LEN                            16
#define  R10_MAIL_DATE_LEN                      8
#define  R10_PAY_METHOD_LEN                     2
#define  R10_FNAME_LEN              16
#define  R10_MI_LEN                 1
#define  R10_LNAME_LEN              16
#define  R10_TITLE_LEN              2
#define  R10_COMPANY_NAME_LEN       30
#define  R10_REF_LEN                30
#define  R10_STREET_LEN             30
#define  R10_CITY_LEN               30
#define  R10_STATE_LEN              2
#define  R10_ZIP_CODE_LEN           14
#define  R10_COUNTRY_LEN            4
#define  R10_PHONE_LEN              14
#define  R10_OLD_CUSTNO_LEN         24
#define  R10_CUSTNO_LEN             9
#define  R10_CUST_CHKDIG_LEN            1
#define  R10_CUST_EDP_LEN           9
#define  R10_NO_PROMO_LEN           1
#define  R10_NO_RENT_LEN            1
#define  R10_FILLER2_LEN                        6

// 12 record defines

#define  R12_REC_TYPE_LEN                       2
#define  R12_FILLER1_LEN                        32
#define  R12_FNAME_LEN              16
#define  R12_MI_LEN                 1
#define  R12_LNAME_LEN              16
#define  R12_TITLE_LEN              2
#define  R12_COMPANY_NAME_LEN       30
#define  R12_REF_LEN                30
#define  R12_STREET_LEN             30
#define  R12_CITY_LEN               30
#define  R12_STATE_LEN              2
#define  R12_ZIP_CODE_LEN           14
#define  R12_PHONE_LEN              14
#define  R12_COUNTRY_LEN            4
#define  R12_OLD_CUSTNO_LEN         24
#define  R12_CUSTNO_LEN             9
#define  R12_CUST_CHKDIG_LEN            1
#define  R12_FILLER2_LEN                        33

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

// 16 Record Defines

#define  R16_REC_TYPE_LEN                       2
#define  R16_FILLER1_LEN                        8
#define  R16_ORD_COMMENT_LEN            60
#define  R16_PRT_PKSLP_LEN                      1
#define  R16_PRT_LABEL_LEN                      1
#define  R16_FILLER2_LEN                        1
#define  R16_FILLER3_LEN                        1
#define  R16_FILLER4_LEN                        184

// 17 Record Defines

#define  R17_REC_TYPE_LEN                       2
#define  R17_FILLER1_LEN                        8

#define  R17_COUP_NUM_COUNT                     18

#define  R17_COUP_NUM_LEN                       9
#define  R17_PERCENT_LEN                        2
#define  R17_DOLL_LEN                           6

#define  R17_FILLER2_LEN                        4


// 20 Record Defines

#define  R20_REC_TYPE_LEN                       2
#define  R20_FILLER1_LEN                        8
#define  R20_CC_TYPE_LEN            2
#define  R20_CC_CARD_NO_LEN         20
#define  R20_CC_EXP_YY_LEN              2
#define  R20_CC_EXP_MM_LEN              2
#define  R20_CC_AUTH_CODE_LEN       6
#define  R20_CC_AUTH_FLAG_LEN       1
#define  R20_FILLER2_LEN                        277

// 21 Record Defines

#define  R21_REC_TYPE_LEN                       2
#define  R21_FILLER1_LEN                        8
#define  R21_ORD_COMMENT_LEN            60
#define  R21_PRT_PKSLP_LEN                      1
#define  R21_PRT_LABEL_LEN                      1
#define  R21_PRT_GREET_LEN                      1
#define  R21_PRT_INVO_LEN                       1
#define  R21_FILLER2_LEN                        182

// 30 Record Defines

#define  R30_REC_TYPE_LEN                       2
#define  R30_FILLER1_LEN                        8
#define  R30_FNAME_LEN              16
#define  R30_MI_LEN                 1
#define  R30_LNAME_LEN              16
#define  R30_TITLE_LEN              2
#define  R30_COMPANY_NAME_LEN       30
#define  R30_REF_LEN                30
#define  R30_STREET_LEN             30
#define  R30_CITY_LEN               30
#define  R30_STATE_LEN              2
#define  R30_ZIP_CODE_LEN           14
#define  R30_COUNTRY_LEN            4
#define  R30_PHONE_LEN              14
#define  R30_OLD_CUSTNO_LEN         24
#define  R30_CUSTNO_LEN             9
#define  R30_CUST_CHKDIG_LEN            1
#define  R30_CUST_EDP_LEN                       9
#define  R30_CONV_FLAG_LEN          1
#define  R30_FUT_DATE_LEN                       8
#define  R30_SHIP_METH_LEN                      2
#define  R30_SHIPTO_NUM_LEN                     2
#define  R30_FILLER2_LEN                        35

// 31 Record Defines

#define  R31_REC_TYPE_LEN                       2
#define  R31_FILLER1_LEN                        8
#define  R31_ORD_COMMENT_LEN            60
#define  R31_PRT_PKSLP_LEN                      1
#define  R31_PRT_LABEL_LEN                      1
#define  R31_PRT_GREET_LEN                      1
//#define  R31_PRT_INVO_LEN                     1
#define  R31_FILLER2_LEN                        184

// 33 Record Defines

#define  R33_REC_TYPE_LEN                       2
#define  R33_FILLER1_LEN                        8

#define  R33_EMAIL_ADDR_LEN                     50

#define  R33_FILLER2_LEN                        260


// 40 Record Defines

#define  R40_REC_TYPE_LEN                       2
#define  R40_FILLER1_LEN                        10
#define  R40_QTY_LEN                4
#define  R40_ITEM_EDP_LEN           9
#define  R40_TAX_XMP_NO_LEN         60
#define  R40_ITEM_NO_LEN            20
#define  R40_TAX_AMT_LEN            7
#define  R40_TAX_RATE_LEN           6
#define  R40_TAX_XMP_FLAG_LEN       1
#define  R40_SHIP_METH_LEN                  2
#define  R40_PH_LEN                         7
#define  R40_PRICE_LEN              9
#define  R40_SHIP_DATE_LEN                  8
#define  R40_POS_LEN                        1
#define  R40_POST_LEN                       7
#define  R40_ASSOC_LINE_LEN                     3
#define  R40_ASSOC_TYPE_LEN                     1
#define  R40_FILLER2_LEN                        153
#define  R40_OVERIDE_PRICE_LEN          1
#define  R40_SALES_LEN                          1
#define  R40_RET_FLAG_LEN                       1
#define  R40_POVRD_LEN                          2
#define  R40_FILLER3_LEN                        5

// 41 Record Defines

#define  R41_REC_TYPE_LEN                       2
#define  R41_FILLER1_LEN                        8
#define  R41_CUSTOMIZE_LEN                      60
#define  R41_BANK_LEN                           1
#define  R41_KIT_EDP_LEN                        9
#define  R41_FILLER2_LEN                        158
#define  R41_FLAG_LEN                           1
#define  R41_ITEM_NO_LEN                        20

// 42 Record Defines

#define  R42_REC_TYPE_LEN                       2
#define  R42_FILLER1_LEN                        8
#define  R42_QTY_LEN                6
#define  R42_ITEM_EDP_LEN           9
#define  R42_TAX_XMP_NO_LEN         60
#define  R42_ITEM_NO_LEN            20
#define  R42_TAX_AMT_LEN            7
#define  R42_TAX_RATE_LEN           6
#define  R42_TAX_XMP_FLAG_LEN       1
#define  R42_SHIP_METH_LEN                  2
#define  R42_PH_AMT_LEN                         7
#define  R42_PRICE_LEN                          9
#define  R42_FILLER2_LEN                        183


typedef struct
{

    char rec_type[R10_REC_TYPE_LEN+1];
        char order_num[R10_ORDER_NUM_LEN+1];
        char source[R10_SRC_LEN+1];
        char mail_date[R10_MAIL_DATE_LEN+1];
    char pay_method[R10_PAY_METHOD_LEN+1];
    char first_name[R10_FNAME_LEN+1];
    char middle_init[R10_MI_LEN+1];
    char last_name[R10_LNAME_LEN+1];
    char title[R10_TITLE_LEN+1];
    char company_name[R10_COMPANY_NAME_LEN+1];
    char address_ref_1[R10_REF_LEN+1];
    char address_ref_2[R10_REF_LEN+1];
    char street[R10_STREET_LEN+1];
    char city[R10_CITY_LEN+1];
    char state[R10_STATE_LEN+1];
    char zip_code[R10_ZIP_CODE_LEN+1];
    char country_code[R10_COUNTRY_LEN+1];
    char day_phone[R10_PHONE_LEN+1];
    char night_phone[R10_PHONE_LEN+1];
    char old_custno[R10_OLD_CUSTNO_LEN+1];
    char cust_no[R10_CUSTNO_LEN+1];
        char cust_chkdig[R10_CUST_CHKDIG_LEN+1];
    char cust_edp[R10_CUST_EDP_LEN+1];
    char no_promo[R10_NO_PROMO_LEN+1];
    char no_rent[R10_NO_RENT_LEN+1];
        char filler2[R10_FILLER2_LEN+1];
    
} wo01_rec10;

typedef struct
{

        char rec_type[R12_REC_TYPE_LEN+1];
        char filler1[R12_FILLER1_LEN+1];
    char first_name[R12_FNAME_LEN+1];
    char middle_init[R12_MI_LEN+1];
    char last_name[R12_LNAME_LEN+1];
    char title[R12_TITLE_LEN+1];
    char company_name[R12_COMPANY_NAME_LEN+1];
    char address_ref_1[R12_REF_LEN+1];
    char address_ref_2[R12_REF_LEN+1];
    char street[R12_STREET_LEN+1];
    char city[R12_CITY_LEN+1];
    char state[R12_STATE_LEN+1];
    char zip_code[R12_ZIP_CODE_LEN+1];
        char phone[R12_PHONE_LEN+1];
    char country_code[R12_COUNTRY_LEN+1];
    char old_custno[R12_OLD_CUSTNO_LEN+1];
    char cust_no[R12_CUSTNO_LEN+1];
        char cust_chkdig[R12_CUST_CHKDIG_LEN+1];
        char filler2[R12_FILLER2_LEN+1];
    
} wo01_rec12;

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

} wo01_rec13;

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
        
} wo01_rec15;

typedef struct
{
    
        char rec_type[R16_REC_TYPE_LEN+1];
        char filler1[R16_FILLER1_LEN+1];
        char ord_comment_1[R16_ORD_COMMENT_LEN+1];
        char ord_comment_2[R16_ORD_COMMENT_LEN+1];
        char prt_on_pkslp_1[R16_PRT_PKSLP_LEN+1];
        char prt_on_label_1[R16_PRT_LABEL_LEN+1];
        char filler2[R16_FILLER2_LEN+1];
        char prt_on_pkslp_2[R16_PRT_PKSLP_LEN+1];
        char prt_on_label_2[R16_PRT_LABEL_LEN+1];
        char filler3[R16_FILLER3_LEN+1];
    char filler4[R16_FILLER4_LEN+1];

} wo01_rec16;

typedef struct
{
    
        char rec_type[R17_REC_TYPE_LEN+1];
        char filler1[R17_FILLER1_LEN+1];

        char coup_num[R17_COUP_NUM_COUNT][R17_COUP_NUM_LEN+1];
        char percent_off[R17_COUP_NUM_COUNT][R17_PERCENT_LEN+1];
        char dollar_off[R17_COUP_NUM_COUNT][R17_DOLL_LEN+1];
        
        char filler2[R17_FILLER2_LEN+1];

} wo01_rec17;


typedef struct
{

        char rec_type[R20_REC_TYPE_LEN+1];
        char filler1[R20_FILLER1_LEN+1];
    char cc_type[R20_CC_TYPE_LEN+1];
    char cc_card_no[R20_CC_CARD_NO_LEN+1];
    char cc_exp_yy[R20_CC_EXP_YY_LEN+1];
    char cc_exp_mm[R20_CC_EXP_MM_LEN+1];
    char cc_auth_code[R20_CC_AUTH_CODE_LEN+1];
    char cc_auth_flag[R20_CC_AUTH_FLAG_LEN+1];
        char filler2[R20_FILLER2_LEN+1];

} wo01_rec20;

typedef struct
{

        char rec_type[R21_REC_TYPE_LEN+1];
        char filler1[R21_FILLER1_LEN+1];
    char ord_comment_1[R21_ORD_COMMENT_LEN+1];
    char ord_comment_2[R21_ORD_COMMENT_LEN+1];
        char prt_on_pkslp_1[R21_PRT_PKSLP_LEN+1];
        char prt_on_label_1[R21_PRT_LABEL_LEN+1];
        char prt_on_greet_1[R21_PRT_GREET_LEN+1];
        char prt_on_pkslp_2[R21_PRT_PKSLP_LEN+1];
        char prt_on_label_2[R21_PRT_LABEL_LEN+1];
        char prt_on_greet_2[R21_PRT_GREET_LEN+1];
        char prt_on_invo_1[R21_PRT_INVO_LEN+1];
        char prt_on_invo_2[R21_PRT_INVO_LEN+1];
        char filler2[R21_FILLER2_LEN+1];

} wo01_rec21;


typedef struct
{

        char rec_type[R30_REC_TYPE_LEN+1];
        char filler1[R30_FILLER1_LEN+1];
        char first_name[R30_FNAME_LEN+1];
    char middle_init[R30_MI_LEN+1];
    char last_name[R30_LNAME_LEN+1];
    char title[R30_TITLE_LEN+1];
    char company_name[R30_COMPANY_NAME_LEN+1];
    char address_ref_1[R30_REF_LEN+1];
    char address_ref_2[R30_REF_LEN+1];
    char street[R30_STREET_LEN+1];
    char city[R30_CITY_LEN+1];
    char state[R30_STATE_LEN+1];
    char zip_code[R30_ZIP_CODE_LEN+1];
        char country_code[R30_COUNTRY_LEN+1];
        char phone[R30_PHONE_LEN+1];
    char old_custno[R30_OLD_CUSTNO_LEN+1];
    char cust_no[R30_CUSTNO_LEN+1];
        char cust_chkdig[R30_CUST_CHKDIG_LEN+1];
        char cust_edp[R30_CUST_EDP_LEN+1];
    char conv_flag[R30_CONV_FLAG_LEN+1];
    char future_date[R30_FUT_DATE_LEN+1];
        char ship_method[R30_SHIP_METH_LEN+1];
        char shipto_num[R30_SHIPTO_NUM_LEN+1];
    char filler2[R30_FILLER2_LEN+1];

} wo01_rec30;

typedef struct
{

        char rec_type[R31_REC_TYPE_LEN+1];
        char filler1[R31_FILLER1_LEN+1];
    char ord_comment_1[R31_ORD_COMMENT_LEN+1];
    char ord_comment_2[R31_ORD_COMMENT_LEN+1];
        char prt_on_pkslp_1[R31_PRT_PKSLP_LEN+1];
        char prt_on_label_1[R31_PRT_LABEL_LEN+1];
        char prt_on_greet_1[R31_PRT_GREET_LEN+1];
        char prt_on_pkslp_2[R31_PRT_PKSLP_LEN+1];
        char prt_on_label_2[R31_PRT_LABEL_LEN+1];
        char prt_on_greet_2[R31_PRT_GREET_LEN+1];
/*      char prt_on_invo_1[R31_PRT_INVO_LEN+1];
        char prt_on_invo_2[R31_PRT_INVO_LEN+1];  */
        char filler2[R31_FILLER2_LEN+1];

} wo01_rec31;


typedef struct
{
    
        char rec_type[R33_REC_TYPE_LEN+1];
        char filler1[R33_FILLER1_LEN+1];

        char email_addr[R33_EMAIL_ADDR_LEN+1];
        
        char filler2[R33_FILLER2_LEN+1];

} wo01_rec33;


typedef struct
{

        char rec_type[R40_REC_TYPE_LEN+1];
        char filler1[R40_FILLER1_LEN+1];
        char item_qty[R40_QTY_LEN+1];
    char item_edp[R40_ITEM_EDP_LEN+1];
    char tax_no[R40_TAX_XMP_NO_LEN+1];
    char item_no[R40_ITEM_NO_LEN+1];
    char tax_amt[R40_TAX_AMT_LEN+1];
    char tax_rate[R40_TAX_RATE_LEN+1];
    char tax_exempt[R40_TAX_XMP_FLAG_LEN+1];
    char ship_method[R40_SHIP_METH_LEN+1];
        char ph_amt[R40_PH_LEN+1];
        char price[R40_PRICE_LEN+1];
        char ship_date[R40_SHIP_DATE_LEN+1];
        char pos_ret[R40_POS_LEN+1];
        char add_postage[R40_POST_LEN+1];
        char assoc_prod_num[R40_ASSOC_LINE_LEN+1];
        char assoc_type[R40_ASSOC_TYPE_LEN+1];
        char filler2[R40_FILLER2_LEN+1];
        char overide_price[R40_OVERIDE_PRICE_LEN+1];
        char club_sales_type[R40_SALES_LEN+1];
        char return_flag[R40_RET_FLAG_LEN+1];
        char povrd_reason[R40_POVRD_LEN+1];
        char filler3[R40_FILLER3_LEN+1];
    
} wo01_rec40;


typedef struct
{

        char rec_type[R41_REC_TYPE_LEN+1];
        char filler1[R41_FILLER1_LEN+1];
        char customize_1[R41_CUSTOMIZE_LEN+1];
        char customize_2[R41_CUSTOMIZE_LEN+1];
        char bank_acct_1[R41_BANK_LEN+1];
        char bank_acct_2[R41_BANK_LEN+1];
        char kit_comp_edp[R41_KIT_EDP_LEN+1];
        char filler2[R41_FILLER2_LEN+1];
        char flag[R41_FLAG_LEN+1];
        char item_num[R41_ITEM_NO_LEN+1];

} wo01_rec41;


typedef struct
{

        char rec_type[R42_REC_TYPE_LEN+1];
        char filler1[R42_FILLER1_LEN+1];
    char item_qty[R42_QTY_LEN+1];
    char item_edp[R42_ITEM_EDP_LEN+1];
    char tax_no[R42_TAX_XMP_NO_LEN+1];
    char item_no[R42_ITEM_NO_LEN+1];
    char tax_amt[R42_TAX_AMT_LEN+1];
    char tax_rate[R42_TAX_RATE_LEN+1];
    char tax_exempt[R42_TAX_XMP_FLAG_LEN+1];
    char ship_method[R42_SHIP_METH_LEN+1];
        char post_amt[R42_PH_AMT_LEN+1];
        char price[R42_PRICE_LEN+1];
        char filler2[R42_FILLER2_LEN+1];

} wo01_rec42;


// Reply from MACS

#define  USER_ID_LEN                16
#define  SUCCESS_FLAG_LEN           1
#define  REJECT_REASON_LEN          80

#define  PRODUCT_DOL_LEN            12 //9
#define  DISC_DOL_LEN               12 //9
#define  TAX_DOL_LEN                12 //9
#define  POSTAGE_DOL_LEN            12 //9
#define  TOTAL_DOL_LEN              12 //9
#define  PAY_METH_LEN               20
#define  SHIP_METH_LEN              80
#define  ORD_NUM_LEN                8
#define  SHIP_METH_CHANGE_LEN       1
#define  LINKSHARE_FLAG_LEN         1
#define  LINKSHARE_ID_LEN           64
#define  LINKSHARE_ZIP_LEN          10
#define  LINKSHARE_DATE_OUT_LEN     8
#define  LINKSHARE_TIME_OUT_LEN     6
#define  SHIPTO_NO_LEN              2
#define  NET_DOL_LEN                12 //9
#define  GIFT_DOL_LEN               12 //9

#define  SHIPTO_COUNT               99

#define  SHIPTO_ST_NO_LEN           2
#define  SHIPTO_EDP_LEN             9
#define  SHIPTO_SM_CODE_LEN         2
#define  SHIPTO_SM_DESC_LEN         80
#define  SHIPTO_SM_CHANGE_LEN       1

#define  FLOOZ_PM_LEN               1
#define  CUST_EDP_LEN               9
#define  GIFT_COUP_TYPE_LEN         1
#define  GIFT_WRAP_DOL_LEN          12 //9
#define  PTY_PAYER_FLAG             1
#define  AI_COUNT_LEN               3

#define  AUTO_INSERT_COUNT          250
#define  AI_ITEM_NO_LEN             20
#define  AI_ITEM_QTY_LEN            4
#define  AI_ITEM_PRICE_LEN          12
#define  AI_ITEM_ST_NUM_LEN         2
#define  WFILLER1_LEN               206

typedef struct
{
        char shipto_st_num[SHIPTO_ST_NO_LEN+1];
        char shipto_edp[SHIPTO_EDP_LEN+1];
        char shipto_sm_code[SHIPTO_SM_CODE_LEN+1];
        char shipto_sm_desc[SHIPTO_SM_DESC_LEN+1];
        char shipto_sm_change[SHIPTO_SM_CHANGE_LEN+1];

} shipto_array;

typedef struct
{    
        char ai_item_no[AI_ITEM_NO_LEN+1];
        char ai_item_qty[AI_ITEM_QTY_LEN+1];
        char ai_item_price[AI_ITEM_PRICE_LEN+1];
        char ai_item_st_num[AI_ITEM_ST_NUM_LEN+1];

} auto_insert_array;


typedef struct
{
    char request_id[REQ_ID_LEN+1];
        char tran_id[TRAN_ID_LEN+1];
    char user_id[USER_ID_LEN+1];
    char success_flag[SUCCESS_FLAG_LEN+1];
    char reject_reason[REJECT_REASON_LEN+1];

    char product_dol[PRODUCT_DOL_LEN+1];
    char discount_dol[DISC_DOL_LEN+1];
    char tax_dol[TAX_DOL_LEN+1];
    char postage_dol[POSTAGE_DOL_LEN+1];
    char total_dol[TOTAL_DOL_LEN+1];
    char pay_method[PAY_METH_LEN+1];
    char ship_method[SHIP_METH_LEN+1];
    char order_no[ORD_NUM_LEN+1];
    char ship_method_change[SHIP_METH_CHANGE_LEN+1];
        char linkshare_flag[LINKSHARE_FLAG_LEN+1];
        char linkshare_id[LINKSHARE_ID_LEN+1];
        char linkshare_zip[LINKSHARE_ZIP_LEN+1];
        char linkshare_date_out[LINKSHARE_DATE_OUT_LEN+1];
        char linkshare_time_out[LINKSHARE_TIME_OUT_LEN+1];
        char shipto_num[SHIPTO_NO_LEN+1];
        char net_dol[NET_DOL_LEN+1];
        char gift_dol[GIFT_DOL_LEN+1];
        shipto_array shipto[SHIPTO_COUNT];
        char flooz_pm[FLOOZ_PM_LEN+1];
        char cust_edp[CUST_EDP_LEN+1];
        char gift_coup_type[GIFT_COUP_TYPE_LEN+1];
        char gift_wrap_dol[GIFT_WRAP_DOL_LEN+1];
        char pty_payer_flag[PTY_PAYER_FLAG+1];
        char ai_count[AI_COUNT_LEN+1];
        auto_insert_array auto_insert[AUTO_INSERT_COUNT];
        char filler1[WFILLER1_LEN+1];

} wo01_recv;

typedef struct
{
        int seq_count;  /* number of records present per order */ 
        char tag[5];

} wo01_seq_tag;

typedef struct
{
	char tmp_tax_rate[R40_TAX_RATE_LEN+1];
	char *tmp_ptr1;
	char *tmp_ptr2;
	int i,j;
}wo01_tmp_vars;

typedef struct
{
    wo01_rec10 rec10;
        wo01_rec12 rec12;
        wo01_rec13 rec13;
        wo01_rec15 rec15;
        wo01_rec16 rec16[NUMBER_OF_RECORDS];
        wo01_rec17 rec17;
        wo01_rec20 rec20;
        wo01_rec21 rec21[NUMBER_OF_RECORDS];
        wo01_rec30 rec30[NUMBER_OF_RECORDS];
        wo01_rec31 rec31[NUMBER_OF_RECORDS];
        wo01_rec33 rec33[NUMBER_OF_RECORDS];
        wo01_rec40 rec40[NUMBER_OF_RECORDS];
        wo01_rec41 rec41[NUMBER_OF_RECORDS];
        wo01_rec42 rec42[NUMBER_OF_RECORDS];

        wo01_seq_tag wo01_oseq_tag[NUMBER_OF_RECORDS];

        char tmaxrec[NUMBER_OF_RECORDS][321];

        wo01_tmp_vars tmp_vars;

}wo01_send_records;