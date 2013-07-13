/* tt0111_defs.h  Validate Item In Cart */

/* Web to MACS Buffer  */

#define  tt0111_LAN_SEND_BUF_LEN        77     
#define  tt0111_LAN_RECV_BUF_LEN        20073 //20045 //20000  

#define  tt0111_REQ_ID_LEN             4
#define  tt0111_REC_ID_LEN             4
#define  tt0111_COMPANY_LEN            2
#define  tt0111_DIVISION_LEN           2
#define  tt0111_USER_ID_LEN            16
#define  tt0111_IP_ADDR_LEN            16
#define  tt0111_FILLER_LEN				25

#define  tt0111_CALLED_BY_LEN			 8

/* MACS to Web Buffer  */

#define  tt0111_REQ_ID_LEN             4
#define  tt0111_REC_ID_LEN             4
#define  tt0111_USER_ID_LEN            16
#define  tt0111_SUCCESS_FLAG_LEN        1
#define  tt0111_ERR_LEN				80
#define  tt0111_SEND_FILLER_LEN			45

#define  tt0111_CUST_EDP_LEN		9
#define  tt0111_SOURCE_LEN			16
#define  tt0111_ITEM_COUNT_LEN		3
#define  tt0111_CHECK_TYPE_LEN		2

#define  tt0111_ITEM_COUNT			 50

#define  tt0111_ITEM_EDP_LEN		9
#define  tt0111_ITEM_NUMBER_LEN		20
#define  tt0111_ITEM_DESCRIPTION_LEN	50
#define  tt0111_ITEM_LINE_NUMBER_LEN	3
#define  tt0111_ITEM_QUANTITY_LEN		4
#define  tt0111_SHIP_TO_NUMBER_LEN		2
#define  tt0111_SHIP_TO_FIRST_NAME_LEN	16
#define  tt0111_SHIP_TO_LAST_NAME_LEN	16
#define  tt0111_CUSTOM_FLAG_LEN			1
#define  tt0111_COMP_EDP_LEN			9
#define  tt0111_COMP_NUMBER_LEN			20
#define  tt0111_COMP_DESCRIPTION_LEN	50
#define  tt0111_COMP_SEQ_NUMBER_LEN		3


typedef struct
{
    char request_id[tt0111_REQ_ID_LEN+1];
    char record_id[tt0111_REC_ID_LEN+1];
    char company[tt0111_COMPANY_LEN+1];
    char division[tt0111_DIVISION_LEN+1];
    char userid[tt0111_USER_ID_LEN+1];
    char ip_address[tt0111_IP_ADDR_LEN+1];
	char filler[tt0111_FILLER_LEN+1];
	char called_by[tt0111_CALLED_BY_LEN+1];
	    
} tt0111_st_send;



typedef struct
{
	char item_edp[tt0111_ITEM_EDP_LEN+1];
	char item_number[tt0111_ITEM_NUMBER_LEN+1];
	char item_description[tt0111_ITEM_DESCRIPTION_LEN+1];
	char item_line_number[tt0111_ITEM_LINE_NUMBER_LEN+1];
	char item_quantity[tt0111_ITEM_QUANTITY_LEN+1];
	char ship_to_number[tt0111_SHIP_TO_NUMBER_LEN+1];
	char ship_to_first_name[tt0111_SHIP_TO_FIRST_NAME_LEN+1];
	char ship_to_last_name[tt0111_SHIP_TO_LAST_NAME_LEN+1];
	char custom_flag[tt0111_CUSTOM_FLAG_LEN+1];
	char comp_edp[tt0111_COMP_EDP_LEN+1];
	char comp_number[tt0111_COMP_NUMBER_LEN+1];
	char comp_description[tt0111_COMP_DESCRIPTION_LEN+1];
	char comp_seq_number[tt0111_COMP_SEQ_NUMBER_LEN+1];

} tt0111_item_array;



typedef struct
{
    char request_id[tt0111_REQ_ID_LEN+1];
    char record_id[tt0111_REC_ID_LEN+1];
    char userid[tt0111_USER_ID_LEN+1];
    char success[tt0111_SUCCESS_FLAG_LEN+1];
	char err_message[tt0111_ERR_LEN+1];
	char filler[tt0111_SEND_FILLER_LEN+1];

	char cust_edp[tt0111_CUST_EDP_LEN+1];
	char source[tt0111_SOURCE_LEN+1];
	char item_count[tt0111_ITEM_COUNT_LEN+1];
	char check_type[tt0111_CHECK_TYPE_LEN+1];

	tt0111_item_array items[tt0111_ITEM_COUNT];

} tt0111_st_recv;

