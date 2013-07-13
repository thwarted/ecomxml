/* tt0065_defs.h  Gift Certificate Review */

/* Web to MACS Buffer  */

#define  tt0065_LAN_SEND_BUF_LEN        69 //42
#define  tt0065_LAN_RECV_BUF_LEN        445 //400  

#define  tt0065_REQ_ID_LEN             4
#define  tt0065_REC_ID_LEN             4
#define  tt0065_COMPANY_LEN            2
#define  tt0065_DIVISION_LEN           2
#define  tt0065_USER_ID_LEN            16
#define  tt0065_IP_ADDR_LEN                      16
#define  tt0065_FILLER_LEN                      25

/* MACS to Web Buffer  */

#define  tt0065_REQ_ID_LEN             4
#define  tt0065_REC_ID_LEN             4
#define  tt0065_USER_ID_LEN            16
#define  tt0065_SUCCESS_FLAG_LEN        1
#define  tt0065_ERR_LEN                         80
#define  tt0065_SEND_FILLER_LEN                 45



#define  tt0065_GC_COUNT                                 18

#define  tt0065_GC_NO_LEN                                8
#define  tt0065_GC_CHK_LEN                       1
#define  tt0065_GC_AMT_LEN                       7


typedef struct
{
    char request_id[tt0065_REQ_ID_LEN+1];
    char record_id[tt0065_REC_ID_LEN+1];
    char company[tt0065_COMPANY_LEN+1];
    char division[tt0065_DIVISION_LEN+1];
    char userid[tt0065_USER_ID_LEN+1];
    char ip_address[tt0065_IP_ADDR_LEN+1];
        char filler[tt0065_FILLER_LEN+1];
    
} tt0065_st_send;

typedef struct
{
    char gc_num[tt0065_GC_NO_LEN+1];
        char gc_chk_dig[tt0065_GC_CHK_LEN+1];
        char gc_amt[tt0065_GC_AMT_LEN+1];

} tt0065_gc_array;

typedef struct
{
    char request_id[tt0065_REQ_ID_LEN+1];
    char record_id[tt0065_REC_ID_LEN+1];
    char userid[tt0065_USER_ID_LEN+1];
    char success[tt0065_SUCCESS_FLAG_LEN+1];
        char err_message[tt0065_ERR_LEN+1];
        char filler[tt0065_SEND_FILLER_LEN+1];

        tt0065_gc_array gc[tt0065_GC_COUNT]; 

} tt0065_st_recv;

