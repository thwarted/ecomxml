/* tt0071_defs.h Get CC Info */

/* Web to MACS Buffer  */

#define  tt0071_LAN_SEND_BUF_LEN        78 //51    
#define  tt0071_LAN_RECV_BUF_LEN        645 //600  

#define  tt0071_REQ_ID_LEN             4
#define  tt0071_REC_ID_LEN             4
#define  tt0071_COMPANY_LEN            2
#define  tt0071_DIVISION_LEN           2
#define  tt0071_USER_ID_LEN            16
#define  tt0071_IP_ADDR_LEN                      16
#define  tt0071_FILLER_LEN                              25
#define  tt0071_EDP_LEN                          9

/* MACS to Web Buffer */

#define  tt0071_REQ_ID_LEN             4
#define  tt0071_REC_ID_LEN             4
#define  tt0071_USER_ID_LEN            16
#define  tt0071_SUCCESS_FLAG_LEN        1
#define  tt0071_ERR_LEN                         80
#define  tt0071_SEND_FILLER_LEN                 45


#define  tt0071_EDP_LEN                          9

#define  tt0071_CC_COUNT                                 10

#define  tt0071_CC_TYPE_LEN                      2
#define  tt0071_CC_DESC_LEN                      20
#define  tt0071_CC_NUM_LEN                       20
#define  tt0071_CC_EXP_LEN                       4
#define  tt0071_CC_FLAG_LEN                      1


typedef struct
{
    char request_id[tt0071_REQ_ID_LEN+1];
    char record_id[tt0071_REC_ID_LEN+1];
    char company[tt0071_COMPANY_LEN+1];
    char division[tt0071_DIVISION_LEN+1];
    char userid[tt0071_USER_ID_LEN+1];
    char ip_address[tt0071_IP_ADDR_LEN+1];
        char filler[tt0071_FILLER_LEN+1];
        char cust_edp[tt0071_EDP_LEN+1];
        
} tt0071_st_send;

typedef struct
{
    char cc_type[tt0071_CC_TYPE_LEN+1];
        char cc_desc[tt0071_CC_DESC_LEN+1];
        char cc_num[tt0071_CC_NUM_LEN+1];
        char cc_exp_date[tt0071_CC_EXP_LEN+1];
        char cc_exp_flag[tt0071_CC_FLAG_LEN+1];

} tt0071_cc_array;


typedef struct
{
    char request_id[tt0071_REQ_ID_LEN+1];
    char record_id[tt0071_REC_ID_LEN+1];
    char userid[tt0071_USER_ID_LEN+1];
    char success[tt0071_SUCCESS_FLAG_LEN+1];
        char err_message[tt0071_ERR_LEN+1];
        char filler[tt0071_SEND_FILLER_LEN+1];

        char cust_edp[tt0071_EDP_LEN+1];
        tt0071_cc_array cc[tt0071_CC_COUNT];

} tt0071_st_recv;

