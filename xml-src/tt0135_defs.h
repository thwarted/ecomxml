/* tt0135_defs.h Ship Method Rates */


#define  tt0135_LAN_SEND_BUF_LEN       71
#define  tt0135_LAN_RECV_BUF_LEN       150

/* Web to MACS Buffer  */

#define  tt0135_REQ_ID_LEN             4
#define  tt0135_REC_ID_LEN             4
#define  tt0135_COMPANY_LEN            2
#define  tt0135_DIVISION_LEN           2
#define  tt0135_USER_ID_LEN            16
#define  tt0135_IP_ADDR_LEN            16
#define  tt0135_FILLER_LEN             25
#define  tt0135_SHIP_METHOD_LEN		   2


/* MACS to Web Buffer   */

#define  tt0135_REQ_ID_LEN             4
#define  tt0135_REC_ID_LEN             4
#define  tt0135_USER_ID_LEN            16
#define  tt0135_SUCCESS_FLAG_LEN       1
#define  tt0135_ERR_LEN                80
#define  tt0135_SEND_FILLER_LEN        45


typedef struct
{
    char request_id[tt0135_REQ_ID_LEN+1];
    char record_id[tt0135_REC_ID_LEN+1];
    char company[tt0135_COMPANY_LEN+1];
    char division[tt0135_DIVISION_LEN+1];
    char userid[tt0135_USER_ID_LEN+1];
    char ip_address[tt0135_IP_ADDR_LEN+1];
    char filler[tt0135_FILLER_LEN+1];
	char ship_method[tt0135_SHIP_METHOD_LEN+1];
 
} tt0135_st_send;



typedef struct
{
    char request_id[tt0135_REQ_ID_LEN+1];
    char record_id[tt0135_REC_ID_LEN+1];
    char userid[tt0135_USER_ID_LEN+1];
    char success[tt0135_SUCCESS_FLAG_LEN+1];
    char err_message[tt0135_ERR_LEN+1];
    char filler[tt0135_SEND_FILLER_LEN+1];

} tt0135_st_recv;


