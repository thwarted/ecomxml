/* tt0131_defs.h Order Review */


#define  tt0131_LAN_SEND_BUF_LEN       112   //104
#define  tt0131_LAN_RECV_BUF_LEN       179   //171

/* Web to MACS Buffer  */

#define  tt0131_REQ_ID_LEN             4
#define  tt0131_REC_ID_LEN                 4
#define  tt0131_COMPANY_LEN            2
#define  tt0131_DIVISION_LEN           2
#define  tt0131_USER_ID_LEN            16
#define  tt0131_IP_ADDR_LEN            16
#define  tt0131_FILLER_LEN                         25

#define  tt0131_ENCRYPT_CUST_EDP_LEN   17
#define  tt0131_ORD_NO_LEN                 8
#define  tt0131_ORD_SUFFIX_LEN         4
#define  tt0131_ACTION_CODE_LEN        2
#define  tt0131_FORMATED_DATE_LEN      10
#define  tt0131_LEVEL_NUM_LEN          2


/* MACS to Web Buffer   */

#define  tt0131_REQ_ID_LEN             4
#define  tt0131_REC_ID_LEN             4
#define  tt0131_USER_ID_LEN            16
#define  tt0131_SUCCESS_FLAG_LEN       1
#define  tt0131_ERR_LEN                        80
#define  tt0131_SEND_FILLER_LEN            45

#define  tt0131_ENCRYPT_CUST_EDP_LEN   17
#define  tt0131_ORDER_NO_LEN           8 
#define  tt0131_ORD_SUFFIX_LEN         4

typedef struct
{
    char request_id[tt0131_REQ_ID_LEN+1];
    char record_id[tt0131_REC_ID_LEN+1];
    char company[tt0131_COMPANY_LEN+1];
    char division[tt0131_DIVISION_LEN+1];
    char userid[tt0131_USER_ID_LEN+1];
    char ip_address[tt0131_IP_ADDR_LEN+1];
        char filler[tt0131_FILLER_LEN+1];
    char encrypt_cust_edp[tt0131_ENCRYPT_CUST_EDP_LEN+1];
    char ord_num[tt0131_ORD_NO_LEN+1];
    char ord_suffix[tt0131_ORD_SUFFIX_LEN+1];
    char action_code[tt0131_ACTION_CODE_LEN+1];
    char hold_till_date[tt0131_FORMATED_DATE_LEN+1];
    char level_num[tt0131_LEVEL_NUM_LEN+1];

} tt0131_st_send;


typedef struct
{
    char request_id[tt0131_REQ_ID_LEN+1];
    char record_id[tt0131_REC_ID_LEN+1];
    char userid[tt0131_USER_ID_LEN+1];
    char success[tt0131_SUCCESS_FLAG_LEN+1];
        char err_message[tt0131_ERR_LEN+1];
        char filler[tt0131_SEND_FILLER_LEN+1];

    char encrypt_cust_edp[tt0131_ENCRYPT_CUST_EDP_LEN+1];
    char ord_num[tt0131_ORD_NO_LEN+1];
    char ord_suffix[tt0131_ORD_SUFFIX_LEN+1];

} tt0131_st_recv;
