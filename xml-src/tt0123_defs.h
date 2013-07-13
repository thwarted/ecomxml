/* tt0123_defs.h   Cart Inventory Data */

/* Web to MACS Buffer  */

#define  tt0123_LAN_SEND_BUF_LEN        69    
#define  tt0123_LAN_RECV_BUF_LEN        22678  

#define  tt0123_REQ_ID_LEN             4
#define  tt0123_REC_ID_LEN             4
#define  tt0123_COMPANY_LEN            2
#define  tt0123_DIVISION_LEN           2
#define  tt0123_USER_ID_LEN            16
#define  tt0123_IP_ADDR_LEN			 16
#define  tt0123_FILLER_LEN			25


/* MACS to Web Buffer  */

#define  tt0123_REQ_ID_LEN             4
#define  tt0123_REC_ID_LEN             4
#define  tt0123_USER_ID_LEN            16
#define  tt0123_SUCCESS_FLAG_LEN        1
#define  tt0123_ERR_LEN				80
#define  tt0123_SEND_FILLER_LEN			45

#define  tt0123_CUST_EDP_LEN		9
#define  tt0123_SOURCE_LEN			16
#define  tt0123_ITEM_COUNT_LEN		3

#define  tt0123_ITEM_COUNT           250

#define  tt0123_ITEM_EDP_LEN		9
#define  tt0123_ITEM_NO_LEN			20
#define  tt0123_DESC_LEN			50
#define  tt0123_CUSTOM_FLAG_LEN		1
#define  tt0123_QTY_LEN				5
#define  tt0123_AVAIL_LEN			5

typedef struct
{
    char request_id[tt0123_REQ_ID_LEN+1];
    char record_id[tt0123_REC_ID_LEN+1];
    char company[tt0123_COMPANY_LEN+1];
    char division[tt0123_DIVISION_LEN+1];
    char userid[tt0123_USER_ID_LEN+1];
	char filler[tt0123_FILLER_LEN+1];
    char ip_address[tt0123_IP_ADDR_LEN+1];
        
} tt0123_st_send;

typedef struct
{

	char item_edp[tt0123_ITEM_EDP_LEN+1];
	char item_no[tt0123_ITEM_NO_LEN+1];
	char desc[tt0123_DESC_LEN+1];
	char custom_flag[tt0123_CUSTOM_FLAG_LEN+1];
	char qty[tt0123_QTY_LEN+1];
	char avail[tt0123_AVAIL_LEN+1];

} tt0123_item_array;


typedef struct
{
    char request_id[tt0123_REQ_ID_LEN+1];
    char record_id[tt0123_REC_ID_LEN+1];
    char userid[tt0123_USER_ID_LEN+1];
    char success[tt0123_SUCCESS_FLAG_LEN+1];
	char err_message[tt0123_ERR_LEN+1];
	char filler[tt0123_SEND_FILLER_LEN+1];
	char cust_edp[tt0123_CUST_EDP_LEN+1];
	char source[tt0123_SOURCE_LEN+1];
	char item_count[tt0123_ITEM_COUNT+1];

	tt0123_item_array item[tt0123_ITEM_COUNT];

} tt0123_st_recv;

