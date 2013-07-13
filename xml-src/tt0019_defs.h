/* tt0019_defs.h  Online CC Authorization */

#define  tt0019_LAN_SEND_BUF_LEN      112 //85
#define  tt0019_LAN_RECV_BUF_LEN      165 //40

/* Web to MACS Buffer  */

#define  tt0019_REQ_ID_LEN             4
#define  tt0019_REC_ID_LEN               4
#define  tt0019_COMPANY_LEN            2
#define  tt0019_DIVISION_LEN           2
#define  tt0019_USER_ID_LEN            16
#define  tt0019_IP_ADDR_LEN            16
#define  tt0019_FILLER_LEN                              25


#define  tt0019_EDP_LEN                9
#define  tt0019_CC_TYPE_LEN            2
#define  tt0019_CC_NO_LEN              20
#define  tt0019_CC_EXP_YEAR_LEN        2
#define  tt0019_CC_EXP_MONTH_LEN       2
#define  tt0019_TRANS_AMT_LEN          8

/* MACS to Web Buffer  */

#define  tt0019_REQ_ID_LEN             4
#define  tt0019_REC_ID_LEN             4
#define  tt0019_USER_ID_LEN            16
#define  tt0019_SUCCESS_FLAG_LEN        1
#define  tt0019_ERR_LEN                         80
#define  tt0019_SEND_FILLER_LEN                 45


#define  tt0019_AUTH_CODE_LEN          6
#define  tt0019_AUTH_ACTION_LEN        1


typedef struct
{
    char request_id[tt0019_REQ_ID_LEN+1];
    char record_id[tt0019_REC_ID_LEN+1];
    char company[tt0019_COMPANY_LEN+1];
    char division[tt0019_DIVISION_LEN+1];
    char userid[tt0019_USER_ID_LEN+1];
    char ip_address[tt0019_IP_ADDR_LEN+1];
        char filler[tt0019_FILLER_LEN+1];
    char cust_edp[tt0019_EDP_LEN+1];
    char cc_type[tt0019_CC_TYPE_LEN+1];
    char cc_num[tt0019_CC_NO_LEN+1];
    char cc_exp_yy[tt0019_CC_EXP_YEAR_LEN+1];
    char cc_exp_mm[tt0019_CC_EXP_MONTH_LEN+1];
    char transaction_amt[tt0019_TRANS_AMT_LEN+1];

} tt0019_st_send;

typedef struct
{
    char request_id[tt0019_REQ_ID_LEN+1];
    char record_id[tt0019_REC_ID_LEN+1];
    char userid[tt0019_USER_ID_LEN+1];
    char success[tt0019_SUCCESS_FLAG_LEN+1];
        char err_message[tt0019_ERR_LEN+1];
        char filler[tt0019_SEND_FILLER_LEN+1];

    char auth_code[tt0019_AUTH_CODE_LEN+1];
    char auth_action[tt0019_AUTH_ACTION_LEN+1];

} tt0019_st_recv;