/* tt0027_defs.h  Link-Share */

/* Web to MACS Buffer  */

#define  tt0027_LAN_SEND_BUF_LEN       147 //120
#define  tt0027_LAN_RECV_BUF_LEN       156 //30

#define  tt0027_REQ_ID_LEN             4
#define  tt0027_REC_ID_LEN	         4
#define  tt0027_COMPANY_LEN            2
#define  tt0027_DIVISION_LEN           2
#define  tt0027_USER_ID_LEN            16
#define  tt0027_IP_ADDR_LEN            16
#define  tt0027_FILLER_LEN				25

#define  tt0027_LINK_ID_LEN			 64
#define  tt0027_LINK_TIME_LEN			 14

/* MACS to Web Buffer  */

#define  tt0027_REQ_ID_LEN             4
#define  tt0027_REC_ID_LEN             4
#define  tt0027_USER_ID_LEN            16
#define  tt0027_SUCCESS_FLAG_LEN        1
#define  tt0027_ERR_LEN				80
#define  tt0027_SEND_FILLER_LEN			45


#define  tt0027_LINK_OK_LEN			 1

typedef struct
{
    char request_id[tt0027_REQ_ID_LEN+1];
    char record_id[tt0027_REC_ID_LEN+1];
    char company[tt0027_COMPANY_LEN+1];
    char division[tt0027_DIVISION_LEN+1];
    char userid[tt0027_USER_ID_LEN+1];
    char ip_address[tt0027_IP_ADDR_LEN+1];
	char filler[tt0027_FILLER_LEN+1];
	char linkshare_id[tt0027_LINK_ID_LEN+1];
	char linkshare_time_in[tt0027_LINK_TIME_LEN+1];

} tt0027_st_send;


typedef struct
{
    char request_id[tt0027_REQ_ID_LEN+1];
    char record_id[tt0027_REC_ID_LEN+1];
    char userid[tt0027_USER_ID_LEN+1];
    char success[tt0027_SUCCESS_FLAG_LEN+1];
	char err_message[tt0027_ERR_LEN+1];
	char filler[tt0027_SEND_FILLER_LEN+1];

	char linkshare_ok[tt0027_LINK_OK_LEN];

} tt0027_st_recv;

