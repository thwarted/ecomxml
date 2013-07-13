/* tt0079_defs.h Coupon Review */

#define  tt0079_LAN_SEND_BUF_LEN        71 
#define  tt0079_LAN_RECV_BUF_LEN        512  

/* Web to MACS Buffer  */

#define  tt0079_REQ_ID_LEN             4
#define  tt0079_REC_ID_LEN             4
#define  tt0079_COMPANY_LEN            2
#define  tt0079_DIVISION_LEN           2
#define  tt0079_USER_ID_LEN            16
#define  tt0079_IP_ADDR_LEN            16
#define  tt0079_FILLER_LEN                              25

#define  tt0079_COUP_RVW_FLAG_LEN      2

/* MACS to Web Buffer  */

#define  tt0079_REQ_ID_LEN             4
#define  tt0079_REC_ID_LEN             4
#define  tt0079_USER_ID_LEN            16
#define  tt0079_SUCCESS_FLAG_LEN        1
#define  tt0079_ERR_LEN                         80
#define  tt0079_SEND_FILLER_LEN                 45

#define  tt0079_COUP_RVW_FLAG_LEN      2

#define  tt0079_COUPON_DATA_COUNT               18

#define  tt0079_COUP_RVW_CO_NUM_LEN       9
#define  tt0079_COUP_RVW_CO_AMOUNT_LEN    10
#define  tt0079_COUP_RVW_CO_TYPE_LEN      1 



typedef struct
{
    char request_id[tt0079_REQ_ID_LEN+1];
    char record_id[tt0079_REC_ID_LEN+1];
    char company[tt0079_COMPANY_LEN+1];
    char division[tt0079_DIVISION_LEN+1];
    char userid[tt0079_USER_ID_LEN+1];
    char ip_address[tt0079_IP_ADDR_LEN+1];
        char filler[tt0079_FILLER_LEN+1];
    char coup_rvw_flag[tt0079_COUP_RVW_FLAG_LEN+1];

} tt0079_st_send;


typedef struct
{
    char coup_rvw_co_num[tt0079_COUP_RVW_CO_NUM_LEN+1];
    char coup_rvw_co_amount[tt0079_COUP_RVW_CO_AMOUNT_LEN+1];
    char coup_rvw_co_type[tt0079_COUP_RVW_CO_TYPE_LEN+1];
    
} tt0079_coupon_data_array;



typedef struct
{
    char request_id[tt0079_REQ_ID_LEN+1];
    char record_id[tt0079_REC_ID_LEN+1];
    char userid[tt0079_USER_ID_LEN+1];
    char success[tt0079_SUCCESS_FLAG_LEN+1];
        char err_message[tt0079_ERR_LEN+1];
        char filler[tt0079_SEND_FILLER_LEN+1];

    char coup_rvw_flag[tt0079_COUP_RVW_FLAG_LEN+1];

    tt0079_coupon_data_array coupon_data[tt0079_COUPON_DATA_COUNT];

} tt0079_st_recv;
