/* tt0007_defs.h Order Review */


#define  tt0007_LAN_SEND_BUF_LEN        89 //88  //77  //50
#define  tt0007_LAN_RECV_BUF_LEN        6782 //6128 //6125 //6000

/* Web to MACS Buffer  */

#define  tt0007_REQ_ID_LEN             4
#define  tt0007_REC_ID_LEN             4
#define  tt0007_COMPANY_LEN            2
#define  tt0007_DIVISION_LEN           2
#define  tt0007_USER_ID_LEN            16
#define  tt0007_IP_ADDR_LEN            16
#define  tt0007_FILLER_LEN             25

#define  tt0007_ORD_NO_LEN             8
#define  tt0007_PAGE_NO_LEN            3
#define  tt0007_CUST_EDP_LEN           9


/* MACS to Web Buffer   */

#define  tt0007_REQ_ID_LEN             4
#define  tt0007_REC_ID_LEN             4
#define  tt0007_USER_ID_LEN            16
#define  tt0007_SUCCESS_FLAG_LEN       1
#define  tt0007_ERR_LEN                80
#define  tt0007_SEND_FILLER_LEN        45


#define  tt0007_ORD_DATE_LEN           8
#define  tt0007_DOL_LEN                9
#define  tt0007_PAY_METH_LEN           20
#define  tt0007_LEVEL_LEN              3
#define  tt0007_FILLER1_LEN            69

#define  tt0007_LEVEL_COUNT            10

#define  tt0007_ADDR_LEN               30
#define  tt0007_LINE_ITM_LEN           3
#define  tt0007_STATUS_LEN             20
#define  tt0007_SHIP_METH_LEN          20
#define  tt0007_SM_TYPE_LEN            1
#define  tt0007_SHIP_DATE_LEN          8
#define  tt0007_TRACK_LEN              20
#define  tt0007_SUFFIX_LEN             4
#define  tt0007_FILLER2_LEN            16

#define  tt0007_LINE_ITEM_COUNT        20

#define  tt0007_ITM_NO_LEN             20
#define  tt0007_ITM_DESC_LEN           50
#define  tt0007_ITM_STATUS_LEN         20
#define  tt0007_ITM_QTY_LEN            6
#define  tt0007_ITM_PRICE_LEN          10
#define  tt0007_GFT_CERT_LEN           1

#define  tt0007_PY_EDP_LEN             9
#define  tt0007_PY_CUSTNO_LEN          10
#define  tt0007_PY_FNAME_LEN           16
#define  tt0007_PY_MI_LEN              1
#define  tt0007_PY_LNAME_LEN           16
#define  tt0007_PY_COMPANY_LEN         30
#define  tt0007_PY_STREET_LEN          30
#define  tt0007_PY_REF1_LEN            30
#define  tt0007_PY_REF2_LEN            30
#define  tt0007_PY_CITY_LEN            30
#define  tt0007_PY_STATE_LEN           2
#define  tt0007_PY_ZIP_LEN             14

#define  tt0007_DM_EDP_LEN             9
#define  tt0007_DM_CUSTNO_LEN          10
#define  tt0007_DM_FNAME_LEN           16
#define  tt0007_DM_MI_LEN              1
#define  tt0007_DM_LNAME_LEN           16
#define  tt0007_DM_COMPANY_LEN         30
#define  tt0007_DM_STREET_LEN          30
#define  tt0007_DM_REF1_LEN            30
#define  tt0007_DM_REF2_LEN            30
#define  tt0007_DM_CITY_LEN            30
#define  tt0007_DM_STATE_LEN           2
#define  tt0007_DM_ZIP_LEN             14

#define  tt0007_SI_EDP_LEN             9
#define  tt0007_SI_CUSTNO_LEN          10
#define  tt0007_SI_FNAME_LEN           16
#define  tt0007_SI_MI_LEN              1
#define  tt0007_SI_LNAME_LEN           16
#define  tt0007_SI_COMPANY_LEN         30
#define  tt0007_SI_STREET_LEN          30
#define  tt0007_SI_REF1_LEN            30
#define  tt0007_SI_REF2_LEN            30
#define  tt0007_SI_CITY_LEN            30
#define  tt0007_SI_STATE_LEN           2
#define  tt0007_SI_ZIP_LEN             14

typedef struct
{
    char request_id[tt0007_REQ_ID_LEN+1];
    char record_id[tt0007_REC_ID_LEN+1];
    char company[tt0007_COMPANY_LEN+1];
    char division[tt0007_DIVISION_LEN+1];
    char userid[tt0007_USER_ID_LEN+1];
    char ip_address[tt0007_IP_ADDR_LEN+1];
        char filler[tt0007_FILLER_LEN+1];
    char ord_num[tt0007_ORD_NO_LEN+1];
    char page_no[tt0007_PAGE_NO_LEN+1];
    char cust_edp[tt0007_CUST_EDP_LEN+1];

} tt0007_st_send;

typedef struct
{
    char item_num[tt0007_ITM_NO_LEN+1];
    char item_desc[tt0007_ITM_DESC_LEN+1];
    char item_status[tt0007_ITM_STATUS_LEN+1];
    char item_qty[tt0007_ITM_QTY_LEN+1];
    char item_price[tt0007_ITM_PRICE_LEN+1];
        char item_gift_cert[tt0007_GFT_CERT_LEN];

} tt0007_line_item_array;

typedef struct
{
    char addr_1[tt0007_ADDR_LEN+1];
    char addr_2[tt0007_ADDR_LEN+1];
    char addr_3[tt0007_ADDR_LEN+1];
    char addr_4[tt0007_ADDR_LEN+1];
    char addr_5[tt0007_ADDR_LEN+1];
    char addr_6[tt0007_ADDR_LEN+1];
    char addr_7[tt0007_ADDR_LEN+1];
    char line_item_num[tt0007_LINE_ITM_LEN+1];
    char status[tt0007_STATUS_LEN+1];
    char ship_method[tt0007_SHIP_METH_LEN+1];
    char ship_method_type[tt0007_SM_TYPE_LEN+1];
    char ship_date[tt0007_SHIP_DATE_LEN+1];
    char track_num[tt0007_TRACK_LEN+1];
    char suffix[tt0007_SUFFIX_LEN+1];
    char filler2[tt0007_FILLER2_LEN+1];
    tt0007_line_item_array item[tt0007_LINE_ITEM_COUNT];

} tt0007_level_array;


typedef struct
{
    char request_id[tt0007_REQ_ID_LEN+1];
    char record_id[tt0007_REC_ID_LEN+1];
    char userid[tt0007_USER_ID_LEN+1];
    char success[tt0007_SUCCESS_FLAG_LEN+1];
        char err_message[tt0007_ERR_LEN+1];
        char filler[tt0007_SEND_FILLER_LEN+1];


    char ord_date[tt0007_ORD_DATE_LEN+1];
    char ord_tot_dol[tt0007_DOL_LEN+1];
    char ord_prod_dol[tt0007_DOL_LEN+1];
    char ord_post_dol[tt0007_DOL_LEN+1];
    char ord_tax_dol[tt0007_DOL_LEN+1];
    char ord_disc_dol[tt0007_DOL_LEN+1];
        char ord_credit_dol[tt0007_DOL_LEN+1];
        char ord_remit_dol[tt0007_DOL_LEN+1];
        char ord_grand_tot_dol[tt0007_DOL_LEN+1];
    char pay_method[tt0007_PAY_METH_LEN+1];
    char num_levels[tt0007_LEVEL_LEN+1];
        char page_no[tt0007_LEVEL_LEN+1];
        char tot_levels[tt0007_LEVEL_LEN+1];
    char filler1[tt0007_FILLER1_LEN+1];
    tt0007_level_array level[tt0007_LEVEL_COUNT];

        char py_edp[tt0007_PY_EDP_LEN+1];
        char py_custno[tt0007_PY_CUSTNO_LEN+1];
        char py_fname[tt0007_PY_FNAME_LEN+1];
        char py_mi[tt0007_PY_MI_LEN+1];
        char py_lname[tt0007_PY_LNAME_LEN+1];
        char py_company[tt0007_PY_COMPANY_LEN+1];
        char py_street[tt0007_PY_STREET_LEN+1];
        char py_ref1[tt0007_PY_REF1_LEN+1];
        char py_ref2[tt0007_PY_REF2_LEN+1];
        char py_city[tt0007_PY_CITY_LEN+1];
        char py_state[tt0007_PY_STATE_LEN+1];
        char py_zip[tt0007_PY_ZIP_LEN+1];

        char dm_edp[tt0007_DM_EDP_LEN+1];
        char dm_custno[tt0007_DM_CUSTNO_LEN+1];
        char dm_fname[tt0007_DM_FNAME_LEN+1];
        char dm_mi[tt0007_DM_MI_LEN+1];
        char dm_lname[tt0007_DM_LNAME_LEN+1];
        char dm_company[tt0007_DM_COMPANY_LEN+1];
        char dm_street[tt0007_DM_STREET_LEN+1];
        char dm_ref1[tt0007_DM_REF1_LEN+1];
        char dm_ref2[tt0007_DM_REF2_LEN+1];
        char dm_city[tt0007_DM_CITY_LEN+1];
        char dm_state[tt0007_DM_STATE_LEN+1];
        char dm_zip[tt0007_DM_ZIP_LEN+1];

        char si_edp[tt0007_SI_EDP_LEN+1];
        char si_custno[tt0007_SI_CUSTNO_LEN+1];
        char si_fname[tt0007_SI_FNAME_LEN+1];
        char si_mi[tt0007_SI_MI_LEN+1];
        char si_lname[tt0007_SI_LNAME_LEN+1];
        char si_company[tt0007_SI_COMPANY_LEN+1];
        char si_street[tt0007_SI_STREET_LEN+1];
        char si_ref1[tt0007_SI_REF1_LEN+1];
        char si_ref2[tt0007_SI_REF2_LEN+1];
        char si_city[tt0007_SI_CITY_LEN+1];
        char si_state[tt0007_SI_STATE_LEN+1];
        char si_zip[tt0007_SI_ZIP_LEN+1];

} tt0007_st_recv;
