/* wl0007_defs.h   */


#define  wl0007_LAN_SEND_BUF_LEN           86    
#define  wl0007_LAN_RECV_BUF_LEN           140


/* Web to MACS Buffer */

#define  wl0007_REC_ID_LEN                 4
#define  wl0007_COMPANY_LEN                2
#define  wl0007_DIVISION_LEN               2
#define  wl0007_USER_ID_LEN                16
#define  wl0007_SEND_FILLER_LEN            31

#define  wl0007_CUST_EDP_LEN               9
#define  wl0007_PSWD_LEN                   16
#define  wl0007_FLAG_LEN                   1
#define  wl0007_FILLER_LEN                 5

/* MACS to Web Buffer */

#define  wl0007_REC_ID_LEN                 4
#define  wl0007_USER_ID_LEN                16
#define  wl0007_SUCCESS_FLAG_LEN           1
#define  wl0007_ERR_LEN                    80
#define  wl0007_RECV_FILLER_LEN            30


typedef struct
{
    char record_id[wl0007_REC_ID_LEN+1];
    char company[wl0007_COMPANY_LEN+1];
    char division[wl0007_DIVISION_LEN+1];
    char userid[wl0007_USER_ID_LEN+1];
    char filler1[wl0007_SEND_FILLER_LEN+1];
    char cust_edp[wl0007_CUST_EDP_LEN+1];
    char password[wl0007_PSWD_LEN+1];
    char ovrd_flag[wl0007_FLAG_LEN+1];
    char filler2[wl0007_FILLER_LEN+1];

} wl0007_st_send;


typedef struct
{
    char record_id[wl0007_REC_ID_LEN+1];
    char userid[wl0007_USER_ID_LEN+1];
    char success_flag[wl0007_SUCCESS_FLAG_LEN+1];
    char err_message[wl0007_ERR_LEN+1];
    char filler[wl0007_RECV_FILLER_LEN+1];

} wl0007_st_recv;