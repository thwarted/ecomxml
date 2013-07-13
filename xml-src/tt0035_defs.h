/* tt0035_defs.h  Add ShipTo */

/* tt0035_defs.h */

#define  tt0035_LAN_SEND_BUF_LEN        112 //110  //101     
#define  tt0035_LAN_RECV_BUF_LEN        6484 //5545  //245   

/* Web to MACS Buffer  */

#define  tt0035_REQ_ID_LEN             4
#define  tt0035_REC_ID_LEN             4
#define  tt0035_COMPANY_LEN            2
#define  tt0035_DIVISION_LEN           2
#define  tt0035_USER_ID_LEN            16
#define  tt0035_IP_ADDR_LEN                      16
#define  tt0035_FILLER_LEN                      25

#define  tt0035_SHIPTO_FN_LEN                    16
#define  tt0035_SHIPTO_LN_LEN                    16
#define  tt0035_SHIP_TO_EDP_LEN                         9
#define  tt0035_SHIPTO_NO_LEN                    2


/* MACS to Web Buffer  */

#define  tt0035_REQ_ID_LEN             4
#define  tt0035_REC_ID_LEN             4
#define  tt0035_USER_ID_LEN            16
#define  tt0035_SUCCESS_FLAG_LEN        1
#define  tt0035_ERR_LEN                         80
#define  tt0035_SEND_FILLER_LEN                 45


#define  tt0035_SHIPTO_NO_LEN                    2
#define  tt0035_SHIPTO_FN_LEN                    16
#define  tt0035_SHIPTO_LN_LEN                    16

#define  tt0035_EXISTING_SHIPTO_DATA    20

#define  tt0035_SHIPTO_FN_LEN                    16
#define  tt0035_SHIPTO_LN_LEN                    16
#define  tt0035_SHIPTO_MI_LEN                   1
#define  tt0035_SHIPTO_TITLE_LEN                2
#define  tt0035_SHIPTO_CO_NAME_LEN              30
#define  tt0035_SHIPTO_STREET_LEN               30
#define  tt0035_SHIPTO_CITY_LEN                 30
#define  tt0035_SHIPTO_STATE_LEN                2
#define  tt0035_SHIPTO_ZIP_LEN                  14
#define  tt0035_SHIPTO_COUNTRY_LEN              20
#define  tt0035_SHIPTO_REF1_LEN                 30
#define  tt0035_SHIPTO_REF2_LEN                 30
#define  tt0035_SHIPTO_JAP_REF1_LEN             25
#define  tt0035_SHIPTO_JAP_PREF_LEN             20
#define  tt0035_SHIPTO_JAP_APT_LEN              15
#define  tt0035_SHIPTO_PHONE_NO_LEN             14
#define  tt0035_SHIPTO_CUST_NO_LEN              10
#define  tt0035_SHIPTO_EDP_LEN                  9
#define  tt0035_JAP_FLAG_LEN                    1



typedef struct
{
    char request_id[tt0035_REQ_ID_LEN+1];
    char record_id[tt0035_REC_ID_LEN+1];
    char company[tt0035_COMPANY_LEN+1];
    char division[tt0035_DIVISION_LEN+1];
    char userid[tt0035_USER_ID_LEN+1];
    char ip_address[tt0035_IP_ADDR_LEN+1];
        char filler[tt0035_FILLER_LEN+1];
        char shipto_fname[tt0035_SHIPTO_FN_LEN+1];
        char shipto_lname[tt0035_SHIPTO_LN_LEN+1];
        char ship_to_edp[tt0035_SHIP_TO_EDP_LEN+1];
        char shipto_num[tt0035_SHIPTO_NO_LEN+1];
        
} tt0035_st_send;


typedef struct
{
    char shipto_num[tt0035_SHIPTO_NO_LEN+1];
        char shipto_fname[tt0035_SHIPTO_FN_LEN+1];
        char shipto_lname[tt0035_SHIPTO_LN_LEN+1];
        char shipto_mi[tt0035_SHIPTO_MI_LEN+1];
        char shipto_title[tt0035_SHIPTO_TITLE_LEN+1];
        char shipto_co_name[tt0035_SHIPTO_CO_NAME_LEN+1];
        char shipto_street[tt0035_SHIPTO_STREET_LEN+1];
        char shipto_city[tt0035_SHIPTO_CITY_LEN+1];
        char shipto_state[tt0035_SHIPTO_STATE_LEN+1];
        char shipto_zip[tt0035_SHIPTO_ZIP_LEN+1];
        char shipto_country[tt0035_SHIPTO_COUNTRY_LEN+1];       
        char shipto_ref1[tt0035_SHIPTO_REF1_LEN+1];
        char shipto_ref2[tt0035_SHIPTO_REF2_LEN+1];
        char shipto_jap_ref1[tt0035_SHIPTO_JAP_REF1_LEN+1];
        char shipto_jap_pref[tt0035_SHIPTO_JAP_PREF_LEN+1];
        char shipto_jap_apt[tt0035_SHIPTO_JAP_APT_LEN+1];
        char shipto_phone_no[tt0035_SHIPTO_PHONE_NO_LEN+1];
        char shipto_cust_no[tt0035_SHIPTO_CUST_NO_LEN+1];
        char shipto_edp[tt0035_SHIPTO_EDP_LEN+1];
        char jap_flag[tt0035_JAP_FLAG_LEN+1];
} tt0035_existing_shipto_data_array;


typedef struct
{
    char request_id[tt0035_REQ_ID_LEN+1];
    char record_id[tt0035_REC_ID_LEN+1];
    char userid[tt0035_USER_ID_LEN+1];
    char success[tt0035_SUCCESS_FLAG_LEN+1];
        char err_message[tt0035_ERR_LEN+1];
        char filler[tt0035_SEND_FILLER_LEN+1];

        char shipto_num[tt0035_SHIPTO_NO_LEN+1];
        char shipto_fname[tt0035_SHIPTO_FN_LEN+1];
        char shipto_lname[tt0035_SHIPTO_LN_LEN+1];
        tt0035_existing_shipto_data_array existing_shipto_data[tt0035_EXISTING_SHIPTO_DATA];


} tt0035_st_recv;

