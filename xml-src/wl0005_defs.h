/* wl0005_defs.h   */


#define  wl0005_LAN_SEND_BUF_LEN           165           
#define  wl0005_LAN_RECV_BUF_LEN           1600 


/* Web to MACS Buffer */

#define  wl0005_REC_ID_LEN                 4
#define  wl0005_COMPANY_LEN                2
#define  wl0005_DIVISION_LEN               2
#define  wl0005_USER_ID_LEN                16
#define  wl0005_SEND_FILLER_LEN            30

#define  wl0005_EMAIL_LEN                  47
#define  wl0005_NAME_LEN                   16
#define  wl0005_CITY_LEN                   30
#define  wl0005_STATE_LEN                  2

/* MACS to Web Buffer */

#define  wl0005_REC_ID_LEN                 4
#define  wl0005_USER_ID_LEN                16
#define  wl0005_SUCCESS_FLAG_LEN           1
#define  wl0005_ERR_LEN                    80
#define  wl0005_RECV_FILLER_LEN            30


#define  wl0005_COUNT_LEN                  2
#define  wl0005_NAME_LEN                   16
#define  wl0005_STREET_LEN                 30
#define  wl0005_CITY_LEN                   30
#define  wl0005_STATE_LEN                  2
#define  wl0005_SEQ_NO_LEN                 2
#define  wl0005_PSWD_FLAG_LEN              1
#define  wl0005_CUSTOMER_LOOP              15


typedef struct
{
    char record_id[wl0005_REC_ID_LEN+1];
    char company[wl0005_COMPANY_LEN+1];
    char division[wl0005_DIVISION_LEN+1];
    char userid[wl0005_USER_ID_LEN+1];
    char filler[wl0005_SEND_FILLER_LEN+1];
    char email[wl0005_EMAIL_LEN+1];
    char lname[wl0005_NAME_LEN+1];
    char fname[wl0005_NAME_LEN+1];
    char city[wl0005_CITY_LEN+1];
    char state[wl0005_STATE_LEN+1];

} wl0005_st_send;

typedef struct
{
    char lname[wl0005_NAME_LEN+1];
    char fname[wl0005_NAME_LEN+1];
    char street[wl0005_STREET_LEN+1];
    char city[wl0005_CITY_LEN+1];
    char state[wl0005_STATE_LEN+1];
    char seq_no[wl0005_SEQ_NO_LEN+1];
    char password_req[wl0005_PSWD_FLAG_LEN+1];

} wl0005_customer;


typedef struct
{
    char record_id[wl0005_REC_ID_LEN+1];
    char userid[wl0005_USER_ID_LEN+1];
    char success_flag[wl0005_SUCCESS_FLAG_LEN+1];
    char err_message[wl0005_ERR_LEN+1];
    char filler[wl0005_RECV_FILLER_LEN+1];
    char count[wl0005_COUNT_LEN+1];
    wl0005_customer customer_detail[wl0005_CUSTOMER_LOOP];

} wl0005_st_recv;