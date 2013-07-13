/* tt0077_defs.h Coupon Validation */

#define  tt0077_LAN_SEND_BUF_LEN        233 
#define  tt0077_LAN_RECV_BUF_LEN        1916  

/* Web to MACS Buffer  */

#define  tt0077_REQ_ID_LEN             4
#define  tt0077_REC_ID_LEN             4
#define  tt0077_COMPANY_LEN            2
#define  tt0077_DIVISION_LEN           2
#define  tt0077_USER_ID_LEN            16
#define  tt0077_IP_ADDR_LEN            16
#define  tt0077_FILLER_LEN                              25

#define  tt0077_COUP_VAL_FLAG_LEN      2

#define  tt0077_COUPON_DATA_COUNT               18

#define  tt0077_COUP_VAL_CO_NUM_LEN     9

/* MACS to Web Buffer  */

#define  tt0077_REQ_ID_LEN             4
#define  tt0077_REC_ID_LEN             4
#define  tt0077_USER_ID_LEN            16
#define  tt0077_SUCCESS_FLAG_LEN        1
#define  tt0077_ERR_LEN                         80
#define  tt0077_SEND_FILLER_LEN                 45

#define  tt0077_COUPON_DATA_COUNT               18

#define  tt0077_COUP_VAL_FLAG_LEN           2
#define  tt0077_COUP_VAL_GOOD_CO_NUM_LEN    9
#define  tt0077_COUP_VAL_BAD_CO_NUM_LEN     9 
#define  tt0077_COUP_VAL_BAD_CO_REJECT_LEN      80



typedef struct
{
    char request_id[tt0077_REQ_ID_LEN+1];
    char record_id[tt0077_REC_ID_LEN+1];
    char company[tt0077_COMPANY_LEN+1];
    char division[tt0077_DIVISION_LEN+1];
    char userid[tt0077_USER_ID_LEN+1];
    char ip_address[tt0077_IP_ADDR_LEN+1];
        char filler[tt0077_FILLER_LEN+1];
    char coup_val_flag[tt0077_COUP_VAL_FLAG_LEN+1];
        char coup_val_co_num[tt0077_COUPON_DATA_COUNT][tt0077_COUP_VAL_CO_NUM_LEN+1];

} tt0077_st_send;


typedef struct
{
    char coup_val_bad_co_num[tt0077_COUP_VAL_BAD_CO_NUM_LEN+1];
    char coup_val_bad_co_reject[tt0077_COUP_VAL_BAD_CO_REJECT_LEN+1];
    
} tt0077_coupon_data_array;



typedef struct
{
    char request_id[tt0077_REQ_ID_LEN+1];
    char record_id[tt0077_REC_ID_LEN+1];
    char userid[tt0077_USER_ID_LEN+1];
    char success[tt0077_SUCCESS_FLAG_LEN+1];
        char err_message[tt0077_ERR_LEN+1];
        char filler[tt0077_SEND_FILLER_LEN+1];

    char coup_val_flag[tt0077_COUP_VAL_FLAG_LEN+1];
        char coup_val_good_co_num[tt0077_COUPON_DATA_COUNT][tt0077_COUP_VAL_GOOD_CO_NUM_LEN+1];

    tt0077_coupon_data_array coupon_data[tt0077_COUPON_DATA_COUNT];

} tt0077_st_recv;
