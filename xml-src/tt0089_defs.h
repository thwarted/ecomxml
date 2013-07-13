/* tt0089_defs.h  Add/Remove Standing Order Item */

/* Web to MACS Buffer  */

#define  tt0089_LAN_SEND_BUF_LEN       90
#define  tt0089_LAN_RECV_BUF_LEN       150

#define  tt0089_REQ_ID_LEN             4
#define  tt0089_REC_ID_LEN               4
#define  tt0089_COMPANY_LEN            2
#define  tt0089_DIVISION_LEN           2
#define  tt0089_USER_ID_LEN            16
#define  tt0089_IP_ADDR_LEN            16
#define  tt0089_FILLER_LEN                              25

#define  tt0089_ITEM_NO_LEN                      20
#define  tt0089_ADD_REMOVE_FLAG_LEN              1

/* MACS to Web Buffer  */

#define  tt0089_REQ_ID_LEN             4
#define  tt0089_REC_ID_LEN             4
#define  tt0089_USER_ID_LEN            16
#define  tt0089_SUCCESS_FLAG_LEN        1
#define  tt0089_ERR_LEN                         80
#define  tt0089_SEND_FILLER_LEN                 45

typedef struct
{
    char request_id[tt0089_REQ_ID_LEN+1];
    char record_id[tt0089_REC_ID_LEN+1];
    char company[tt0089_COMPANY_LEN+1];
    char division[tt0089_DIVISION_LEN+1];
    char userid[tt0089_USER_ID_LEN+1];
    char ip_address[tt0089_IP_ADDR_LEN+1];
        char filler[tt0089_FILLER_LEN+1];
        char item_no[tt0089_ITEM_NO_LEN+1];
        char add_remove_flag[tt0089_ADD_REMOVE_FLAG_LEN+1];

} tt0089_st_send;


typedef struct
{
    char request_id[tt0089_REQ_ID_LEN+1];
    char record_id[tt0089_REC_ID_LEN+1];
    char userid[tt0089_USER_ID_LEN+1];
    char success[tt0089_SUCCESS_FLAG_LEN+1];
        char err_message[tt0089_ERR_LEN+1];
        char filler[tt0089_SEND_FILLER_LEN+1];

} tt0089_st_recv;
