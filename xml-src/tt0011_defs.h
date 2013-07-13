/* tt0011_defs.h  Source Input */


#define  tt0011_LAN_SEND_BUF_LEN        86  //59
#define  tt0011_LAN_RECV_BUF_LEN        176 //50

/* Web to MACS Buffer  */

#define  tt0011_REQ_ID_LEN             4
#define  tt0011_REC_ID_LEN             4
#define  tt0011_COMPANY_LEN            2
#define  tt0011_DIVISION_LEN           2
#define  tt0011_USER_ID_LEN            16
#define  tt0011_IP_ADDR_LEN            16
#define  tt0011_FILLER_LEN                      25

#define  tt0011_SRC_LEN                      16
#define  tt0011_SRC_FLAG_LEN           1


/* MACS to Web Buffer  */

#define  tt0011_REQ_ID_LEN             4
#define  tt0011_REC_ID_LEN             4
#define  tt0011_USER_ID_LEN            16
#define  tt0011_SUCCESS_FLAG_LEN        1
#define  tt0011_ERR_LEN                  80
#define  tt0011_SEND_FILLER_LEN                 45


#define  tt0011_VAL_SRC_LEN            1
#define  tt0011_SRC_LEN                  16


typedef struct
{
    char request_id[tt0011_REQ_ID_LEN+1];
    char record_id[tt0011_REC_ID_LEN+1];
    char company[tt0011_COMPANY_LEN+1];
    char division[tt0011_DIVISION_LEN+1];
    char userid[tt0011_USER_ID_LEN+1];
    char ip_address[tt0011_IP_ADDR_LEN+1];
        char filler[tt0011_FILLER_LEN+1];
    char source_code[tt0011_SRC_LEN+1];
    char action_flag[tt0011_SRC_FLAG_LEN+1];

} tt0011_st_send;

typedef struct
{
    char request_id[tt0011_REQ_ID_LEN+1];
    char record_id[tt0011_REC_ID_LEN+1];
    char userid[tt0011_USER_ID_LEN+1];
    char success[tt0011_SUCCESS_FLAG_LEN+1];
        char err_message[tt0011_ERR_LEN+1];
        char filler[tt0011_SEND_FILLER_LEN+1];

    char valid_src[tt0011_VAL_SRC_LEN+1];
    char source_code[tt0011_SRC_LEN+1];

} tt0011_st_recv;