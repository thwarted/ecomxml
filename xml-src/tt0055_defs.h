/* tt0055_defs.h Get Order Comments */

/* Web to MACS Buffer  */

#define  tt0055_LAN_SEND_BUF_LEN        69 //42
#define  tt0055_LAN_RECV_BUF_LEN        826 //700  

#define  tt0055_REQ_ID_LEN             4
#define  tt0055_REC_ID_LEN             4
#define  tt0055_COMPANY_LEN            2
#define  tt0055_DIVISION_LEN           2
#define  tt0055_USER_ID_LEN            16
#define  tt0055_IP_ADDR_LEN			 16
#define  tt0055_FILLER_LEN				25


/* MACS to Web Buffer  */

#define  tt0055_REQ_ID_LEN             4
#define  tt0055_REC_ID_LEN             4
#define  tt0055_USER_ID_LEN            16
#define  tt0055_SUCCESS_FLAG_LEN        1
#define  tt0055_ERR_LEN				80
#define  tt0055_SEND_FILLER_LEN			45


#define  tt0055_ORD_COMMENT_COUNT		 5

#define  tt0055_SEQ_LEN				 2
#define  tt0055_COMM_LEN				 60
#define  tt0055_PKSLP_LEN				 1
#define  tt0055_LABEL_LEN				 1
#define  tt0055_GREET_LEN				 1
#define  tt0055_INVO_LEN				 1



typedef struct
{
    char request_id[tt0055_REQ_ID_LEN+1];
    char record_id[tt0055_REC_ID_LEN+1];
    char company[tt0055_COMPANY_LEN+1];
    char division[tt0055_DIVISION_LEN+1];
    char userid[tt0055_USER_ID_LEN+1];
    char ip_address[tt0055_IP_ADDR_LEN+1];
	char filler[tt0055_FILLER_LEN+1];
	    
} tt0055_st_send;


typedef struct
{
	char seq_num[tt0055_SEQ_LEN+1];
	char ord_comment1[tt0055_COMM_LEN+1];
	char prt_pkslp1[tt0055_PKSLP_LEN+1];
	char prt_label1[tt0055_LABEL_LEN+1];
	char prt_greet1[tt0055_GREET_LEN+1];
	char prt_invoice1[tt0055_INVO_LEN+1];
	char ord_comment2[tt0055_COMM_LEN+1];
	char prt_pkslp2[tt0055_PKSLP_LEN+1];
	char prt_label2[tt0055_LABEL_LEN+1];
	char prt_greet2[tt0055_GREET_LEN+1];
	char prt_invoice2[tt0055_INVO_LEN+1];

} tt0055_ord_comment_array;


typedef struct
{
    char request_id[tt0055_REQ_ID_LEN+1];
    char record_id[tt0055_REC_ID_LEN+1];
    char userid[tt0055_USER_ID_LEN+1];
    char success[tt0055_SUCCESS_FLAG_LEN+1];
	char err_message[tt0055_ERR_LEN+1];
	char filler[tt0055_SEND_FILLER_LEN+1];

	tt0055_ord_comment_array order_comment[tt0055_ORD_COMMENT_COUNT];

} tt0055_st_recv;

