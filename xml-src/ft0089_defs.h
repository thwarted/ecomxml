/* ft0089_defs.h  Add Internal Credits */


#define  ft0089_LAN_SEND_BUF_LEN           88
#define  ft0089_LAN_RECV_BUF_LEN           150

/* Web to MACS Buffer */

#define  ft0089_REQ_ID_LEN                 4
#define  ft0089_REC_ID_LEN                 4
#define  ft0089_COMPANY_LEN                2
#define  ft0089_DIVISION_LEN               2
#define  ft0089_USER_ID_LEN                16
#define  ft0089_IP_ADDR_LEN                16
#define  ft0089_FILLER_LEN                 25
#define  ft0089_CUST_NO_LEN                10
#define  ft0089_CREDIT_AMT_LEN             9


/* MACS to Web Buffer */

#define  ft0089_REQ_ID_LEN                 4
#define  ft0089_REC_ID_LEN                 4
#define  ft0089_USER_ID_LEN                16
#define  ft0089_SUCCESS_FLAG_LEN           1
#define  ft0089_ERR_LEN                    80
#define  ft0089_SEND_FILLER_LEN            45

typedef struct
{
    char request_id[ft0089_REQ_ID_LEN+1];
    char record_id[ft0089_REC_ID_LEN+1];
    char company[ft0089_COMPANY_LEN+1];
    char division[ft0089_DIVISION_LEN+1];
    char userid[ft0089_USER_ID_LEN+1];
    char ip_address[ft0089_IP_ADDR_LEN+1];
    char filler[ft0089_FILLER_LEN+1];
    char cust_no[ft0089_CUST_NO_LEN+1];
    char credit_amt[ft0089_CREDIT_AMT_LEN+1];

} ft0089_st_send;


typedef struct
{
    char request_id[ft0089_REQ_ID_LEN+1];
    char record_id[ft0089_REC_ID_LEN+1];
    char userid[ft0089_USER_ID_LEN+1];
    char success_flag[ft0089_SUCCESS_FLAG_LEN+1];
    char err_message[ft0089_ERR_LEN+1];
    char filler[ft0089_SEND_FILLER_LEN+1];

} ft0089_st_recv;


