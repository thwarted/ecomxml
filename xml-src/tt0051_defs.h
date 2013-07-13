/* tt0051_defs.h  Variable Kit Get P1 */

/* Web to MACS Buffer  */

#define  tt0051_LAN_SEND_BUF_LEN        169 //142    
#define  tt0051_LAN_RECV_BUF_LEN        28125 //28000  

#define  tt0051_REQ_ID_LEN             4
#define  tt0051_REC_ID_LEN             4
#define  tt0051_COMPANY_LEN            2
#define  tt0051_DIVISION_LEN           2
#define  tt0051_USER_ID_LEN            16
#define  tt0051_IP_ADDR_LEN            16
#define  tt0051_FILLER_LEN				25

#define  tt0051_P1_ITEM_COUNT		     5

#define  tt0051_P1_ITM_NO_LEN			 20

/* MACS to Web Buffer  */

#define  tt0051_REQ_ID_LEN             4
#define  tt0051_REC_ID_LEN             4
#define  tt0051_USER_ID_LEN            16
#define  tt0051_SUCCESS_FLAG_LEN        1
#define  tt0051_ERR_LEN				80
#define  tt0051_SEND_FILLER_LEN			45


#define  tt0051_P1_COUNT_LEN			 2

#define  tt0051_P1_COMP_COUNT			 5

#define  tt0051_P1_ITM_NO_LEN			 20
#define  tt0051_P1_EDP_LEN			 9
#define  tt0051_P1_DESC_LEN			 50
#define  tt0051_P1_QTY_LEN			 5
#define  tt0051_P1_UPS_LEN			 640
#define  tt0051_P1_IMG_OVRD_LEN		 8
#define  tt0051_R1_COUNT_LEN			 2

#define  tt0051_R1_COMP_COUNT			 50

#define  tt0051_R1_ITM_NO_LEN			 20
#define  tt0051_R1_EDP_LEN			 9
#define  tt0051_R1_DESC_LEN			 50
#define  tt0051_R1_PRICE_LEN			 10
#define  tt0051_R1_AVAIL_LEN			 5


typedef struct
{
    char request_id[tt0051_REQ_ID_LEN+1];
    char record_id[tt0051_REC_ID_LEN+1];
    char company[tt0051_COMPANY_LEN+1];
    char division[tt0051_DIVISION_LEN+1];
    char userid[tt0051_USER_ID_LEN+1];
    char ip_address[tt0051_IP_ADDR_LEN+1];
	char filler[tt0051_FILLER_LEN+1];
	char p1_item_num[tt0051_P1_ITEM_COUNT][tt0051_P1_ITM_NO_LEN+1];
        
} tt0051_st_send;


typedef struct
{
	char r1_item_num[tt0051_R1_ITM_NO_LEN+1];
	char r1_item_edp[tt0051_R1_EDP_LEN+1];
	char r1_item_desc[tt0051_R1_DESC_LEN+1];
	char r1_item_price[tt0051_R1_PRICE_LEN+1];
	char r1_item_avail_inv[tt0051_R1_AVAIL_LEN+1];

} tt0051_r1_comp_array;


typedef struct
{
	char p1_item_num[tt0051_P1_ITM_NO_LEN+1];
	char p1_item_edp[tt0051_P1_EDP_LEN+1];
	char p1_item_desc[tt0051_P1_DESC_LEN+1];
	char p1_item_qty[tt0051_P1_QTY_LEN+1];
	char p1_upsl_X[tt0051_P1_UPS_LEN+1];
	char p1_image_ovrd[tt0051_P1_IMG_OVRD_LEN+1];
	char r1_count[tt0051_R1_COUNT_LEN+1];
	tt0051_r1_comp_array r1_component[tt0051_R1_COMP_COUNT];

} tt0051_p1_comp_array;



typedef struct
{
    char request_id[tt0051_REQ_ID_LEN+1];
    char record_id[tt0051_REC_ID_LEN+1];
    char userid[tt0051_USER_ID_LEN+1];
    char success[tt0051_SUCCESS_FLAG_LEN+1];
	char err_message[tt0051_ERR_LEN+1];
	char filler[tt0051_SEND_FILLER_LEN+1];

	char p1_count[tt0051_P1_COUNT_LEN+1];
	tt0051_p1_comp_array p1_component[tt0051_P1_COMP_COUNT];

} tt0051_st_recv;

