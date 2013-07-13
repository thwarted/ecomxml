/* tt0033_defs.h  Get ShipTo */

#define  tt0033_LAN_SEND_BUF_LEN        69 //42    
#define  tt0033_LAN_RECV_BUF_LEN        3712 //1026 //900  

/* Web to MACS Buffer  */

#define  tt0033_REQ_ID_LEN             4
#define  tt0033_REC_ID_LEN             4
#define  tt0033_COMPANY_LEN            2
#define  tt0033_DIVISION_LEN           2
#define  tt0033_USER_ID_LEN            16
#define  tt0033_IP_ADDR_LEN                      16
#define  tt0033_FILLER_LEN

/* MACS to Web Buffer */

#define  tt0033_REQ_ID_LEN             4
#define  tt0033_REC_ID_LEN             4
#define  tt0033_USER_ID_LEN            16
#define  tt0033_SUCCESS_FLAG_LEN        1
#define  tt0033_ERR_LEN                         80
#define  tt0033_SEND_FILLER_LEN                 45


#define  tt0033_SHIPTO_COUNT           99

#define  tt0033_SHIPTO_NO_LEN                    2
#define  tt0033_SHIPTO_FN_LEN                    16
#define  tt0033_SHIPTO_LN_LEN                    16


typedef struct
{
    char request_id[tt0033_REQ_ID_LEN+1];
    char record_id[tt0033_REC_ID_LEN+1];
    char company[tt0033_COMPANY_LEN+1];
    char division[tt0033_DIVISION_LEN+1];
    char userid[tt0033_USER_ID_LEN+1];
    char ip_address[tt0033_IP_ADDR_LEN+1];
        char filler[tt0033_FILLER_LEN+1];
        
} tt0033_st_send;

typedef struct
{
    char shipto_num[tt0033_SHIPTO_NO_LEN+1];
        char shipto_fname[tt0033_SHIPTO_FN_LEN+1];
        char shipto_lname[tt0033_SHIPTO_LN_LEN+1];

} tt0033_shipto_array;


typedef struct
{
    char request_id[tt0033_REQ_ID_LEN+1];
    char record_id[tt0033_REC_ID_LEN+1];
    char userid[tt0033_USER_ID_LEN+1];
    char success[tt0033_SUCCESS_FLAG_LEN+1];
        char err_message[tt0033_ERR_LEN+1];
        char filler[tt0033_SEND_FILLER_LEN+1];

        tt0033_shipto_array shipto[tt0033_SHIPTO_COUNT];

} tt0033_st_recv;

