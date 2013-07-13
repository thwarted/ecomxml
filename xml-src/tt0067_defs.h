/* tt0067_defs.h  Gift Certificate Cancel */

/* Web to MACS Buffer  */

#define  tt0067_LAN_SEND_BUF_LEN        233 //206
#define  tt0067_LAN_RECV_BUF_LEN        195 //150 

#define  tt0067_REQ_ID_LEN             4
#define  tt0067_REC_ID_LEN             4
#define  tt0067_COMPANY_LEN            2
#define  tt0067_DIVISION_LEN           2
#define  tt0067_USER_ID_LEN            16
#define  tt0067_IP_ADDR_LEN                      16
#define  tt0067_FILLER_LEN                              25

#define  tt0067_GC_COUNT                                 18

#define  tt0067_GC_NO_LEN                                8
#define  tt0067_GC_CHK_LEN                       1

#define  tt0067_GC_FLAG_LEN                      2


/* MACS to Web Buffer  */

#define  tt0067_REQ_ID_LEN             4
#define  tt0067_REC_ID_LEN             4
#define  tt0067_USER_ID_LEN            16
#define  tt0067_SUCCESS_FLAG_LEN        1
#define  tt0067_ERR_LEN                         80
#define  tt0067_SEND_FILLER_LEN                 45


#define  tt0067_GC_FLAG_LEN                      2


typedef struct
{
    char gc_num[tt0067_GC_NO_LEN+1];
        char gc_chk_dig[tt0067_GC_CHK_LEN+1];

} tt0067_gc_array;

typedef struct
{
    char request_id[tt0067_REQ_ID_LEN+1];
    char record_id[tt0067_REC_ID_LEN+1];
    char company[tt0067_COMPANY_LEN+1];
    char division[tt0067_DIVISION_LEN+1];
    char userid[tt0067_USER_ID_LEN+1];
    char ip_address[tt0067_IP_ADDR_LEN+1];
        char filler[tt0067_FILLER_LEN+1];
        tt0067_gc_array gc[tt0067_GC_COUNT]; 
        char gc_flag[tt0067_GC_FLAG_LEN+1];

} tt0067_st_send;


typedef struct
{
    char request_id[tt0067_REQ_ID_LEN+1];
    char record_id[tt0067_REC_ID_LEN+1];
    char userid[tt0067_USER_ID_LEN+1];
    char success[tt0067_SUCCESS_FLAG_LEN+1];
        char err_message[tt0067_ERR_LEN+1];
        char filler[tt0067_SEND_FILLER_LEN+1];

        char gc_flag[tt0067_GC_FLAG_LEN+1];

} tt0067_st_recv;

