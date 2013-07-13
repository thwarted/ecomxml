/* tt0095_defs.h Standing Order Add */

#define  tt0095_LAN_SEND_BUF_LEN        289 
#define  tt0095_LAN_RECV_BUF_LEN        150 

/* Web to MACS Buffer  */

#define  tt0095_REQ_ID_LEN             4
#define  tt0095_REC_ID_LEN             4
#define  tt0095_COMPANY_LEN            2
#define  tt0095_DIVISION_LEN           2
#define  tt0095_USER_ID_LEN            16
#define  tt0095_IP_ADDR_LEN            16
#define  tt0095_FILLER_LEN				25

#define  tt0095_SHIPTO_COUNT             20

#define  tt0095_SHIPTO_NO_LEN             2
#define  tt0095_SHIPTO_EDP_LEN                9

/* MACS to Web Buffer  */

#define  tt0095_REQ_ID_LEN             4
#define  tt0095_REC_ID_LEN             4
#define  tt0095_USER_ID_LEN            16
#define  tt0095_SUCCESS_FLAG_LEN        1
#define  tt0095_ERR_LEN				80
#define  tt0095_SEND_FILLER_LEN			45




typedef struct
{
    char request_id[tt0095_REQ_ID_LEN+1];
    char record_id[tt0095_REC_ID_LEN+1];
    char company[tt0095_COMPANY_LEN+1];
    char division[tt0095_DIVISION_LEN+1];
    char userid[tt0095_USER_ID_LEN+1];
    char ip_address[tt0095_IP_ADDR_LEN+1];
	char filler[tt0095_FILLER_LEN+1];
    char shipto_no[tt0095_SHIPTO_COUNT][tt0095_SHIPTO_NO_LEN+1];
	char shipto_edp[tt0095_SHIPTO_COUNT][tt0095_SHIPTO_EDP_LEN+1];

} tt0095_st_send;



typedef struct
{
    char request_id[tt0095_REQ_ID_LEN+1];
    char record_id[tt0095_REC_ID_LEN+1];
    char userid[tt0095_USER_ID_LEN+1];
    char success[tt0095_SUCCESS_FLAG_LEN+1];
	char err_message[tt0095_ERR_LEN+1];
	char filler[tt0095_SEND_FILLER_LEN+1];

} tt0095_st_recv;