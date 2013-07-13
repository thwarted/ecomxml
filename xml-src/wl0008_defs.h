/* wl0008_defs.h   */


#define  wl0008_LAN_SEND_BUF_LEN           71           
#define  wl0008_LAN_RECV_BUF_LEN           350


/* Web to MACS Buffer */

#define  wl0008_REC_ID_LEN                 4
#define  wl0008_COMPANY_LEN                2
#define  wl0008_DIVISION_LEN               2
#define  wl0008_USER_ID_LEN                16
#define  wl0008_SEND_FILLER_LEN            27

/* MACS to Web Buffer */

#define  wl0008_REC_ID_LEN                 4
#define  wl0008_USER_ID_LEN                16
#define  wl0008_SUCCESS_FLAG_LEN           1
#define  wl0008_ERR_LEN                    80
#define  wl0008_RECV_FILLER_LEN            30

#define  wl0008_WL_ID_LEN                  16
#define  wl0008_CUST_EDP_LEN               9
#define  wl0008_NAME_LEN                   16
#define  wl0008_MI_LEN                     2
#define  wl0008_CITY_LEN                   30
#define  wl0008_STATE_LEN                  2
#define  wl0008_STREET_LEN                 30
#define  wl0008_ZIP_LEN                    14
#define  wl0008_CNTRY_CODE_LEN             4
#define  wl0008_PSWD_LEN                   16
#define  wl0008_EMAIL_LEN                  47


typedef struct
{
    char record_id[wl0008_REC_ID_LEN+1];
    char company[wl0008_COMPANY_LEN+1];
    char division[wl0008_DIVISION_LEN+1];
    char userid[wl0008_USER_ID_LEN+1];
    char wl_company[wl0008_COMPANY_LEN+1];
    char wl_division[wl0008_DIVISION_LEN+1];
    char wl_id[wl0008_USER_ID_LEN+1];
    char wl_filler[wl0008_SEND_FILLER_LEN+1];

} wl0008_st_send;


typedef struct
{
    char record_id[wl0008_REC_ID_LEN+1];
    char userid[wl0008_USER_ID_LEN+1];
    char success_flag[wl0008_SUCCESS_FLAG_LEN+1];
    char err_message[wl0008_ERR_LEN+1];
    char filler[wl0008_RECV_FILLER_LEN+1];
    char wl_id[wl0008_WL_ID_LEN+1];
    char cust_edp[wl0008_CUST_EDP_LEN+1];
    char fname[wl0008_NAME_LEN+1];
    char minitial[wl0008_MI_LEN+1];
    char lname[wl0008_NAME_LEN+1];
    char city[wl0008_CITY_LEN+1];
    char state[wl0008_STATE_LEN+1];
    char street[wl0008_STREET_LEN+1];
    char zip[wl0008_ZIP_LEN+1];
    char country_code[wl0008_CNTRY_CODE_LEN+1];
    char password[wl0008_PSWD_LEN+1];
    char email[wl0008_EMAIL_LEN+1];

} wl0008_st_recv;