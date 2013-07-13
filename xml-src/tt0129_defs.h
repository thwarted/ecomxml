/* tt0129_defs.h Order Review */


#define  tt0129_LAN_SEND_BUF_LEN       98    //90
#define  tt0129_LAN_RECV_BUF_LEN       11706 //11698

/* Web to MACS Buffer  */

#define  tt0129_REQ_ID_LEN             4
#define  tt0129_REC_ID_LEN                 4
#define  tt0129_COMPANY_LEN            2
#define  tt0129_DIVISION_LEN           2
#define  tt0129_USER_ID_LEN            16
#define  tt0129_IP_ADDR_LEN            16
#define  tt0129_FILLER_LEN                         25

#define  tt0129_ENCRYPT_CUST_EDP_LEN   17
#define  tt0129_ORD_NO_LEN                 8
#define  tt0129_ORD_SUFFIX_LEN         4


/* MACS to Web Buffer   */

#define  tt0129_REQ_ID_LEN             4
#define  tt0129_REC_ID_LEN             4
#define  tt0129_USER_ID_LEN            16
#define  tt0129_SUCCESS_FLAG_LEN       1
#define  tt0129_ERR_LEN                        80
#define  tt0129_SEND_FILLER_LEN            45

#define  tt0129_ENCRYPT_CUST_EDP_LEN   17
#define  tt0129_ORDER_NO_LEN           8 
#define  tt0129_ORD_SUFFIX_LEN         4
#define  tt0129_PRG_NAME_LEN           6
#define  tt0129_PRG_DESC_LEN           30
#define  tt0129_DATE_LEN               8
#define  tt0129_PRG_SHIPS_LEN          2
#define  tt0129_PRG_STATUS_CODE_LEN    1
#define  tt0129_PRG_STATUS_LEN         28 

#define  tt0129_FNAME_LEN              16 
#define  tt0129_LNAME_LEN              16 
#define  tt0129_BU_COMPANY_LEN         30 
#define  tt0129_STREET_LEN             30 
#define  tt0129_CITY_LEN               30 
#define  tt0129_STATE_LEN              2 
#define  tt0129_COUNTRY_LEN            20 
#define  tt0129_ZIP_LEN                14

#define  tt0129_PRG_BILLED_LEN         12
#define  tt0129_PRG_PAID_LEN           12
#define  tt0129_PRG_RETURNED_LEN       12
#define  tt0129_PRG_OTHER_PAID_LEN     12
#define  tt0129_ACTUAL_LVL_COUNT_LEN   2 

#define  tt0129_LEVEL_COUNT            99

#define  tt0129_LVL_ITEM_LEN           20 
#define  tt0129_LVL_DESC_LEN           50
#define  tt0129_LVL_STATUS_LEN         20
#define  tt0129_DATE_LEN               8
#define  tt0129_LVL_COLLECTOR_LEN      4
#define  tt0129_LVL_EFFORT_LEN         2
#define  tt0129_DATE_LEN               8

typedef struct
{
    char request_id[tt0129_REQ_ID_LEN+1];
    char record_id[tt0129_REC_ID_LEN+1];
    char company[tt0129_COMPANY_LEN+1];
    char division[tt0129_DIVISION_LEN+1];
    char userid[tt0129_USER_ID_LEN+1];
    char ip_address[tt0129_IP_ADDR_LEN+1];
        char filler[tt0129_FILLER_LEN+1];
    char encrypt_cust_edp[tt0129_ENCRYPT_CUST_EDP_LEN+1];
    char ord_num[tt0129_ORD_NO_LEN+1];
    char ord_suffix[tt0129_ORD_SUFFIX_LEN+1];

} tt0129_st_send;

typedef struct
{
    char lvl_item[tt0129_LVL_ITEM_LEN+1];
    char lvl_desc[tt0129_LVL_DESC_LEN+1];
    char lvl_status[tt0129_LVL_STATUS_LEN+1];
    char lvl_date[tt0129_DATE_LEN+1];
    char lvl_collector[tt0129_LVL_COLLECTOR_LEN+1];
    char lvl_effort[tt0129_LVL_EFFORT_LEN+1];
    char lvl_eff_date[tt0129_DATE_LEN+1];

} tt0129_level_array;

typedef struct
{
    char request_id[tt0129_REQ_ID_LEN+1];
    char record_id[tt0129_REC_ID_LEN+1];
    char userid[tt0129_USER_ID_LEN+1];
    char success[tt0129_SUCCESS_FLAG_LEN+1];
        char err_message[tt0129_ERR_LEN+1];
        char filler[tt0129_SEND_FILLER_LEN+1];

    char encrypt_cust_edp[tt0129_ENCRYPT_CUST_EDP_LEN+1];
    char ord_num[tt0129_ORD_NO_LEN+1];
    char ord_suffix[tt0129_ORD_SUFFIX_LEN+1];

    char prg_name[tt0129_PRG_NAME_LEN+1];
    char prg_desc[tt0129_PRG_DESC_LEN+1];
    char prg_start_date[tt0129_DATE_LEN+1];
    char prg_ships[tt0129_PRG_SHIPS_LEN+1];
    char prg_status_code[tt0129_PRG_STATUS_CODE_LEN+1];
    char prg_status[tt0129_PRG_STATUS_LEN+1];

        char prg_bu_fname[tt0129_FNAME_LEN+1];
        char prg_bu_lname[tt0129_LNAME_LEN+1];
        char prg_bu_company[tt0129_BU_COMPANY_LEN+1];
        char prg_bu_street[tt0129_STREET_LEN+1];
        char prg_bu_city[tt0129_CITY_LEN+1];
        char prg_bu_state[tt0129_STATE_LEN+1];
        char prg_bu_country[tt0129_COUNTRY_LEN+1];
        char prg_bu_zip[tt0129_ZIP_LEN+1];

        char prg_st_fname[tt0129_FNAME_LEN+1];
        char prg_st_lname[tt0129_LNAME_LEN+1];
        char prg_st_company[tt0129_BU_COMPANY_LEN+1];
        char prg_st_street[tt0129_STREET_LEN+1];
        char prg_st_city[tt0129_CITY_LEN+1];
        char prg_st_state[tt0129_STATE_LEN+1];
        char prg_st_country[tt0129_COUNTRY_LEN+1];
        char prg_st_zip[tt0129_ZIP_LEN+1];

        char prg_billed[tt0129_PRG_BILLED_LEN+1];
        char prg_paid[tt0129_PRG_PAID_LEN+1];
        char prg_returned[tt0129_PRG_RETURNED_LEN+1];
        char prg_other_paid[tt0129_PRG_OTHER_PAID_LEN+1];
        char actual_lvl_count[tt0129_ACTUAL_LVL_COUNT_LEN+1];

    tt0129_level_array item[tt0129_LEVEL_COUNT];

} tt0129_st_recv;
