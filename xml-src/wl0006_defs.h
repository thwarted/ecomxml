/* wl0006_defs.h   */


#define  wl0006_LAN_SEND_BUF_LEN           2404     
#define  wl0006_LAN_RECV_BUF_LEN           140


/* Web to MACS Buffer */

#define  wl0006_REC_ID_LEN                 4
#define  wl0006_COMPANY_LEN                2
#define  wl0006_DIVISION_LEN               2
#define  wl0006_USER_ID_LEN                16
#define  wl0006_PROFILE_LEN                2
#define  wl0006_SEND_FILLER_LEN            8

#define  wl0006_EMAIL_LEN                  47
#define  wl0006_EMAIL_LOOP                 50

/* MACS to Web Buffer */

#define  wl0006_REC_ID_LEN                 4
#define  wl0006_USER_ID_LEN                16
#define  wl0006_SUCCESS_FLAG_LEN           1
#define  wl0006_ERR_LEN                    80
#define  wl0006_RECV_FILLER_LEN            30


typedef struct
{
    char record_id[wl0006_REC_ID_LEN+1];
    char company[wl0006_COMPANY_LEN+1];
    char division[wl0006_DIVISION_LEN+1];
    char userid[wl0006_USER_ID_LEN+1];
    char wl_company[wl0006_COMPANY_LEN+1];
    char wl_division[wl0006_DIVISION_LEN+1];
    char wl_id[wl0006_USER_ID_LEN+1];
    char wl_profile[wl0006_PROFILE_LEN+1];
    char wl_filler[wl0006_SEND_FILLER_LEN+1];
    char email[wl0006_EMAIL_LOOP][wl0006_EMAIL_LEN+1];

} wl0006_st_send;


typedef struct
{
    char record_id[wl0006_REC_ID_LEN+1];
    char userid[wl0006_USER_ID_LEN+1];
    char success_flag[wl0006_SUCCESS_FLAG_LEN+1];
    char err_message[wl0006_ERR_LEN+1];
    char filler[wl0006_RECV_FILLER_LEN+1];

} wl0006_st_recv;