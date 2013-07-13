/* tt0017_defs.h  Style Validation */


#define  tt0017_LAN_SEND_BUF_LEN        147 //120
#define  tt0017_LAN_RECV_BUF_LEN        175 //50 

/* Web to MACS Buffer  */

#define  tt0017_REQ_ID_LEN             4
#define  tt0017_REC_ID_LEN             4
#define  tt0017_COMPANY_LEN            2
#define  tt0017_DIVISION_LEN           2
#define  tt0017_USER_ID_LEN            16
#define  tt0017_IP_ADDR_LEN            16
#define  tt0017_FILLER_LEN                              25

#define  tt0017_STYLE_ID_LEN           12
#define  tt0017_L3_TYPE_LEN            2
#define  tt0017_L2_TYPE_LEN            2
#define  tt0017_L1_TYPE_LEN            2
#define  tt0017_L3_DESC_LEN            20
#define  tt0017_L2_DESC_LEN            20
#define  tt0017_L1_DESC_LEN            20

/* MACS to Web Buffer  */


#define  tt0017_REQ_ID_LEN             4 
#define  tt0017_REC_ID_LEN             4
#define  tt0017_USER_ID_LEN            16 
#define  tt0017_SUCCESS_FLAG_LEN        1
#define  tt0017_ERR_LEN                  80
#define  tt0017_SEND_FILLER_LEN                 45


#define  tt0017_ITM_NO_LEN             20

typedef struct
{
    char request_id[tt0017_REQ_ID_LEN+1];
    char record_id[tt0017_REC_ID_LEN+1];
    char company[tt0017_COMPANY_LEN+1];
    char division[tt0017_DIVISION_LEN+1];
    char userid[tt0017_USER_ID_LEN+1];
    char ip_address[tt0017_IP_ADDR_LEN+1];
        char filler[tt0017_FILLER_LEN+1];
    char style_id[tt0017_STYLE_ID_LEN+1];
    char l3_type[tt0017_L3_TYPE_LEN+1];
    char l2_type[tt0017_L2_TYPE_LEN+1];
    char l1_type[tt0017_L1_TYPE_LEN+1];
    char l3_desc[tt0017_L3_DESC_LEN+1];
    char l2_desc[tt0017_L2_DESC_LEN+1];
    char l1_desc[tt0017_L1_DESC_LEN+1];

} tt0017_st_send;

typedef struct
{
    char request_id[tt0017_REQ_ID_LEN+1];
    char record_id[tt0017_REC_ID_LEN+1];
    char userid[tt0017_USER_ID_LEN+1];
    char success[tt0017_SUCCESS_FLAG_LEN+1];
        char err_message[tt0017_ERR_LEN+1];
        char filler[tt0017_SEND_FILLER_LEN+1];

    char item_num[tt0017_ITM_NO_LEN+1];

} tt0017_st_recv;