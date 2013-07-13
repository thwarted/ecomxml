/* tt0005_defs.h  All Orders */

#define  tt0005_LAN_SEND_BUF_LEN           82 //55 
#define  tt0005_LAN_RECV_BUF_LEN           1820 //1826 //1700

/* Web to MACS Buffer  */

#define  tt0005_REQ_ID_LEN                4
#define  tt0005_REC_ID_LEN                4
#define  tt0005_COMPANY_LEN               2
#define  tt0005_DIVISION_LEN              2
#define  tt0005_USER_ID_LEN               16
#define  tt0005_IP_ADDR_LEN               16
#define  tt0005_FILLER_LEN				25

#define  tt0005_EDP_LEN		            9
#define  tt0005_PAGE_NO_LEN               4


/* MACS to Web Buffer  */

#define  tt0005_REQ_ID_LEN                4
#define  tt0005_REC_ID_LEN                4
#define  tt0005_USER_ID_LEN               16
#define  tt0005_SUCCESS_FLAG_LEN       1
#define  tt0005_ERR_LEN			 80
#define  tt0005_SEND_FILLER_LEN			45


#define  tt0005_ORD_LEN		            6
#define  tt0005_CURR_PAGE_LEN				4

#define tt0005_ORDER_COUNT                20

#define  tt0005_ORD_NO_LEN				  8
#define  tt0005_MAIL_DATE_LEN             8
#define  tt0005_PO_NO_LEN                 26
#define  tt0005_NUM_OF_ITM_LEN            6
#define  tt0005_SHIP_DATE_LEN             8
#define  tt0005_NUM_OF_SHIPMENTS_LEN      6
#define  tt0005_STATUS_LEN                20
#define  tt0005_PTY_FLAG_LEN			  1


typedef struct
{
    char request_id[tt0005_REQ_ID_LEN+1];
    char record_id[tt0005_REC_ID_LEN+1];
    char company[tt0005_COMPANY_LEN+1];
    char division[tt0005_DIVISION_LEN+1];
    char userid[tt0005_USER_ID_LEN+1];
    char ip_address[tt0005_IP_ADDR_LEN+1];
	char filler[tt0005_FILLER_LEN+1];
    char cust_edp[tt0005_EDP_LEN+1];
	char page_num[tt0005_PAGE_NO_LEN+1];

} tt0005_st_send;

typedef struct
{
    char ord_num[tt0005_ORD_NO_LEN+1];
    char mail_date[tt0005_MAIL_DATE_LEN+1];
    char po_num[tt0005_PO_NO_LEN+1];
    char num_items[tt0005_NUM_OF_ITM_LEN+1];
    char ship_date[tt0005_SHIP_DATE_LEN+1];
    char num_shipments[tt0005_NUM_OF_SHIPMENTS_LEN+1];
    char ord_status[tt0005_STATUS_LEN+1];
	char pty_flag[tt0005_PTY_FLAG_LEN+1];

} tt0005_order_array;

typedef struct
{
    char request_id[tt0005_REQ_ID_LEN+1];
    char record_id[tt0005_REC_ID_LEN+1];
    char userid[tt0005_USER_ID_LEN+1];	
    char success_flag[tt0005_SUCCESS_FLAG_LEN+1];
	char err_message[tt0005_ERR_LEN+1];
	char filler[tt0005_SEND_FILLER_LEN+1];

	char valid_ord[tt0005_ORD_LEN+1]; 
	char ord_curr_page[tt0005_CURR_PAGE_LEN+1];
    tt0005_order_array order[tt0005_ORDER_COUNT];

} tt0005_st_recv;
