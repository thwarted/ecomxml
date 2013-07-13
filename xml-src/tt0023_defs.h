/* tt0023_defs.h  Customization Review */


#define  tt0023_LAN_SEND_BUF_LEN      99 //98
#define  tt0023_LAN_RECV_BUF_LEN      1386 //1385 //1382 //1376 //1250

/* Web to MACS Buffer  */

#define  tt0023_REQ_ID_LEN             4
#define  tt0023_REC_ID_LEN             4
#define  tt0023_COMPANY_LEN            2
#define  tt0023_DIVISION_LEN           2
#define  tt0023_USER_ID_LEN            16
#define  tt0023_IP_ADDR_LEN            16
#define  tt0023_FILLER_LEN				25

#define  tt0023_ITM_NO_LEN             20
#define  tt0023_PAGE_NO_LEN				2
#define  tt0023_CART_LINE_NO_LEN        3
#define  tt0023_COMP_SEQ_NUMBER_LEN		3
#define  tt0023_ITEM_TYPE_LEN			1
#define  tt0023_CUSTOMFRAME_LEN			1

/* MACS to Web Buffer  */

#define  tt0023_REQ_ID_LEN             4
#define  tt0023_REC_ID_LEN             4
#define  tt0023_USER_ID_LEN            16
#define  tt0023_SUCCESS_FLAG_LEN        1
#define  tt0023_ERR_LEN				80
#define  tt0023_SEND_FILLER_LEN			45


#define  tt0023_LAST_LEN               2

#define tt0023_CUSTOMIZE_COUNT         20

#define  tt0023_CUSTOMIZE_LEN          60


#define  tt0023_ITEM_LINE_NUMBER_LEN	3
#define  tt0023_COMP_SEQ_NUMBER_LEN		3
#define  tt0023_CUSTOMFRAME_LEN			1

typedef struct
{
    char request_id[tt0023_REQ_ID_LEN + 1];
    char record_id[tt0023_REC_ID_LEN + 1];
    char company[tt0023_COMPANY_LEN + 1];
    char division[tt0023_DIVISION_LEN + 1];
    char userid[tt0023_USER_ID_LEN + 1];
    char ip_address[tt0023_IP_ADDR_LEN + 1];
	char filler[tt0023_FILLER_LEN+1];
    char item_num[tt0023_ITM_NO_LEN + 1];
	char page_no[tt0023_PAGE_NO_LEN + 1];
    char cart_line_num[tt0023_CART_LINE_NO_LEN + 1];
	char comp_seq_number[tt0023_COMP_SEQ_NUMBER_LEN+1];
	char item_type[tt0023_ITEM_TYPE_LEN+1];
	char customframe[tt0023_CUSTOMFRAME_LEN+1];

} tt0023_st_send;

typedef struct
{
    char customization_line[tt0023_CUSTOMIZE_LEN + 1];

} tt0023_customize_array;

typedef struct
{
    char request_id[tt0023_REQ_ID_LEN + 1];
    char record_id[tt0023_REC_ID_LEN + 1];
    char userid[tt0023_USER_ID_LEN + 1];
    char success[tt0023_SUCCESS_FLAG_LEN+1];
	char err_message[tt0023_ERR_LEN+1];
	char filler[tt0023_SEND_FILLER_LEN+1];

    char last_customization[tt0023_LAST_LEN + 1];
    tt0023_customize_array customization[tt0023_CUSTOMIZE_COUNT];
	char page_no[tt0023_PAGE_NO_LEN + 1];
	char item_line_number[tt0023_ITEM_LINE_NUMBER_LEN+1];
	char comp_seq_number[tt0023_COMP_SEQ_NUMBER_LEN+1];
	char item_type[tt0023_ITEM_TYPE_LEN+1];
	char customframe[tt0023_CUSTOMFRAME_LEN+1];

} tt0023_st_recv;