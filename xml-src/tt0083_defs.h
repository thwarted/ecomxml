/* tt0083_defs.h Get Flooz Merchant Info */

/* Web to MACS Buffer  */

#define  tt0083_LAN_SEND_BUF_LEN        69 //42    
#define  tt0083_LAN_RECV_BUF_LEN        195 //150

#define  tt0083_REQ_ID_LEN             4
#define  tt0083_REC_ID_LEN             4
#define  tt0083_COMPANY_LEN            2
#define  tt0083_DIVISION_LEN           2
#define  tt0083_USER_ID_LEN            16
#define  tt0083_IP_ADDR_LEN                      16
#define  tt0083_FILLER_LEN                      25

/* MACS to Web Buffer */

#define  tt0083_REQ_ID_LEN             4
#define  tt0083_REC_ID_LEN             4
#define  tt0083_USER_ID_LEN            16
#define  tt0083_SUCCESS_FLAG_LEN        1
#define  tt0083_ERR_LEN                         80
#define  tt0083_SEND_FILLER_LEN                 45


#define  tt0083_FLOOZ_LEN                                11

typedef struct
{
    char request_id[tt0083_REQ_ID_LEN+1];
    char record_id[tt0083_REC_ID_LEN+1];
    char company[tt0083_COMPANY_LEN+1];
    char division[tt0083_DIVISION_LEN+1];
    char userid[tt0083_USER_ID_LEN+1];
    char ip_address[tt0083_IP_ADDR_LEN+1];
        char filler[tt0083_FILLER_LEN+1];
        
} tt0083_st_send;


typedef struct
{
    char request_id[tt0083_REQ_ID_LEN+1];
    char record_id[tt0083_REC_ID_LEN+1];
    char userid[tt0083_USER_ID_LEN+1];
    char success[tt0083_SUCCESS_FLAG_LEN+1];
        char err_message[tt0083_ERR_LEN+1];
        char filler[tt0083_SEND_FILLER_LEN+1];

        char flooz_code[tt0083_FLOOZ_LEN+1];

} tt0083_st_recv;

