/* tt0081_defs.h Coupon Cancel */

#define  tt0081_LAN_SEND_BUF_LEN        143 
#define  tt0081_LAN_RECV_BUF_LEN        152  

/* Web to MACS Buffer  */

#define  tt0081_REQ_ID_LEN             4
#define  tt0081_REC_ID_LEN             4
#define  tt0081_COMPANY_LEN            2
#define  tt0081_DIVISION_LEN           2
#define  tt0081_USER_ID_LEN            16
#define  tt0081_IP_ADDR_LEN            16
#define  tt0081_FILLER_LEN                              25

#define  tt0081_COUP_CAN_FLAG_LEN      2

#define  tt0081_COUPON_DATA_COUNT               18

#define  tt0081_COUP_CAN_CO_NUM_LEN     4

/* MACS to Web Buffer  */

#define  tt0081_REQ_ID_LEN             4
#define  tt0081_REC_ID_LEN             4
#define  tt0081_USER_ID_LEN            16
#define  tt0081_SUCCESS_FLAG_LEN        1
#define  tt0081_ERR_LEN                         80
#define  tt0081_SEND_FILLER_LEN                 45

#define  tt0081_COUP_CAN_FLAG_LEN           2



typedef struct
{
    char request_id[tt0081_REQ_ID_LEN+1];
    char record_id[tt0081_REC_ID_LEN+1];
    char company[tt0081_COMPANY_LEN+1];
    char division[tt0081_DIVISION_LEN+1];
    char userid[tt0081_USER_ID_LEN+1];
    char ip_address[tt0081_IP_ADDR_LEN+1];
        char filler[tt0081_FILLER_LEN+1];
    char coup_can_flag[tt0081_COUP_CAN_FLAG_LEN+1];
        char coup_can_co_num[tt0081_COUPON_DATA_COUNT][tt0081_COUP_CAN_CO_NUM_LEN+1];

} tt0081_st_send;


typedef struct
{
    char request_id[tt0081_REQ_ID_LEN+1];
    char record_id[tt0081_REC_ID_LEN+1];
    char userid[tt0081_USER_ID_LEN+1];
    char success[tt0081_SUCCESS_FLAG_LEN+1];
        char err_message[tt0081_ERR_LEN+1];
        char filler[tt0081_SEND_FILLER_LEN+1];

    char coup_can_flag[tt0081_COUP_CAN_FLAG_LEN+1];

} tt0081_st_recv;