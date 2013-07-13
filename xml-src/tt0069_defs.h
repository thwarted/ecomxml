/* tt0069_defs.h Password Request */

/* Web to MACS Buffer  */

#define  tt0069_LAN_SEND_BUF_LEN        116 //89    
#define  tt0069_LAN_RECV_BUF_LEN        245 //200  

#define  tt0069_REQ_ID_LEN             4
#define  tt0069_REC_ID_LEN             4
#define  tt0069_COMPANY_LEN            2
#define  tt0069_DIVISION_LEN           2
#define  tt0069_USER_ID_LEN            16
#define  tt0069_IP_ADDR_LEN			 16
#define  tt0069_FILLER_LEN			25

#define  tt0069_EMAIL_LEN				 47 

/* MACS to Web Buffer */

#define  tt0069_REQ_ID_LEN             4
#define  tt0069_REC_ID_LEN             4
#define  tt69_USER_ID_LEN            16
#define  tt0069_SUCCESS_FLAG_LEN        1
#define  tt0069_ERR_LEN				80
#define  tt0069_SEND_FILLER_LEN			45


#define  tt0069_EMAIL_LEN				 47 


typedef struct
{
    char request_id[tt0069_REQ_ID_LEN+1];
    char record_id[tt0069_REC_ID_LEN+1];
    char company[tt0069_COMPANY_LEN+1];
    char division[tt0069_DIVISION_LEN+1];
    char userid[tt0069_USER_ID_LEN+1];
    char ip_address[tt0069_IP_ADDR_LEN+1];
	char filler[tt0069_FILLER_LEN+1];
    char email_addr[tt0069_EMAIL_LEN+1];
    
} tt0069_st_send;


typedef struct
{
    char request_id[tt0069_REQ_ID_LEN+1];
    char record_id[tt0069_REC_ID_LEN+1];
    char userid[tt0069_USER_ID_LEN+1];
    char success[tt0069_SUCCESS_FLAG_LEN+1];
	char err_message[tt0069_ERR_LEN+1];
	char filler[tt0069_SEND_FILLER_LEN+1];

	char email_addr[tt0069_EMAIL_LEN+1];
	
} tt0069_st_recv;

