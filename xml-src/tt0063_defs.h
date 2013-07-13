/* tt0063_defs.h  Gift Certificate Validation */

/* Web to MACS Buffer  */

#define  tt0063_LAN_SEND_BUF_LEN        341 //314
#define  tt0063_LAN_RECV_BUF_LEN        2245 //2200  

#define  tt0063_REQ_ID_LEN             4
#define  tt0063_REC_ID_LEN             4
#define  tt0063_COMPANY_LEN            2
#define  tt0063_DIVISION_LEN           2
#define  tt0063_USER_ID_LEN            16
#define  tt0063_IP_ADDR_LEN                      16
#define  tt0063_FILLER_LEN                      25

#define  tt0063_GC_COUNT                                 18

#define  tt0063_GC_NO_LEN                                8
#define  tt0063_GC_CHK_LEN                       1
#define  tt0063_GC_AMT_LEN                       6

#define  tt0063_GC_FLAG_LEN                      2


/* MACS to Web Buffer  */

#define  tt0063_REQ_ID_LEN             4
#define  tt0063_REC_ID_LEN             4
#define  tt0063_USER_ID_LEN            16
#define  tt0063_SUCCESS_FLAG_LEN        1
#define  tt0063_ERR_LEN                         80
#define  tt0063_SEND_FILLER_LEN                 45


#define  tt0063_VALID_GC_COUNT           18

#define  tt0063_VAL_GC_NO_LEN                    9
#define  tt0063_VAL_GC_AMT_LEN         7

#define  tt0063_INVALID_GC_COUNT                 18

#define  tt0063_INVAL_GC_NO_LEN          9
#define  tt0063_INVAL_GC_AMT_LEN       7
#define  tt0063_INVAL_GC_REJ_LEN                 80

#define  tt0063_GC_FLAG_LEN                      2

typedef struct
{
    char gc_num[tt0063_GC_NO_LEN+1];
        char gc_chk_dig[tt0063_GC_CHK_LEN+1];
        char gc_amt[tt0063_GC_AMT_LEN+1];

} tt0063_gc_array;

typedef struct
{
    char request_id[tt0063_REQ_ID_LEN+1];
    char record_id[tt0063_REC_ID_LEN+1];
    char company[tt0063_COMPANY_LEN+1];
    char division[tt0063_DIVISION_LEN+1];
    char userid[tt0063_USER_ID_LEN+1];
    char ip_address[tt0063_IP_ADDR_LEN+1];
        char filler[tt0063_FILLER_LEN+1];
        tt0063_gc_array gc[tt0063_GC_COUNT]; 
        char gc_flag[tt0063_GC_FLAG_LEN+1];
    
} tt0063_st_send;

typedef struct
{
    char valid_gc_num[tt0063_VAL_GC_NO_LEN+1];
        char valid_gc_amt[tt0063_VAL_GC_AMT_LEN+1];

} tt0063_valid_gc_array;

typedef struct
{
    char invalid_gc_num[tt0063_INVAL_GC_NO_LEN+1];
        char invalid_gc_amt[tt0063_INVAL_GC_AMT_LEN+1];
        char invalid_gc_rej[tt0063_INVAL_GC_REJ_LEN+1];

} tt0063_invalid_gc_array;


typedef struct
{
    char request_id[tt0063_REQ_ID_LEN+1];
    char record_id[tt0063_REC_ID_LEN+1];
    char userid[tt0063_USER_ID_LEN+1];
    char success[tt0063_SUCCESS_FLAG_LEN+1];
        char err_message[tt0063_ERR_LEN+1];
        char filler[tt0063_SEND_FILLER_LEN+1];

        tt0063_valid_gc_array valid_gc[tt0063_VALID_GC_COUNT];
        tt0063_invalid_gc_array invalid_gc[tt0063_INVALID_GC_COUNT];
        char gc_flag[tt0063_GC_FLAG_LEN+1];

} tt0063_st_recv;

