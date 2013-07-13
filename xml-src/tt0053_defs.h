/* tt0053_defs.h  Variable Kit Validation */

/* Web to MACS Buffer  */

#define  tt0053_LAN_SEND_BUF_LEN        693 //691 //666    
#define  tt0053_LAN_RECV_BUF_LEN        23045 //23000  

#define  tt0053_REQ_ID_LEN             4
#define  tt0053_REC_ID_LEN             4
#define  tt0053_COMPANY_LEN            2
#define  tt0053_DIVISION_LEN           2
#define  tt0053_USER_ID_LEN            16
#define  tt0053_IP_ADDR_LEN            16
#define  tt0053_FILLER_LEN				25

#define  tt0053_ITM_NO_LEN			 20
#define  tt0053_UPSL_LEN				 1

#define  tt0053_R1_ITEM_COUNT	    	 25

#define  tt0053_R1_ITM_NO_LEN			 20
#define  tt0053_R1_QUTY_LEN			 4

/* MACS to Web Buffer  */

#define  tt0053_REQ_ID_LEN             4
#define  tt0053_REC_ID_LEN             4
#define  tt0053_USER_ID_LEN            16
#define  tt0053_SUCCESS_FLAG_LEN        1
#define  tt0053_ERR_LEN				80
#define  tt0053_SEND_FILLER_LEN			45


#define  tt0053_ITM_NO_LEN			 20
#define  tt0053_EDP_LEN				 9
#define  tt0053_DESC_LEN				 50
#define  tt0053_UPS_LEN				 640
#define  tt0053_IMG_OVRD_LEN			 8
#define  tt0053_PRICE_LEN				 10
#define  tt0053_AVAIL_LEN				 5
#define  tt0053_R1_COUNT_LEN			 2
#define  tt0053_CUSTOM_LEN			 1

#define  tt0053_R1_COMP_COUNT			 25

#define  tt0053_R1_ITM_NO_LEN			 20
#define  tt0053_R1_EDP_LEN			 9
#define  tt0053_R1_DESC_LEN			 50
#define  tt0053_R1_QTY_LEN			 5
#define  tt0053_R1_UPS_LEN			 640
#define  tt0053_R1_IMG_OVRD_LEN		 8
#define  tt0053_R1_PRICE_LEN			 10
#define  tt0053_R1_AVAIL_LEN			 5


typedef struct
{
    char request_id[tt0053_REQ_ID_LEN+1];
    char record_id[tt0053_REC_ID_LEN+1];
    char company[tt0053_COMPANY_LEN+1];
    char division[tt0053_DIVISION_LEN+1];
    char userid[tt0053_USER_ID_LEN+1];
    char ip_address[tt0053_IP_ADDR_LEN+1];
	char filler[tt0053_FILLER_LEN+1];
	char kit_item_num[tt0053_ITM_NO_LEN+1];
	char upsl_A[tt0053_UPSL_LEN+1];
	char upsl_B[tt0053_UPSL_LEN+1];
	char upsl_C[tt0053_UPSL_LEN+1];
	char upsl_D[tt0053_UPSL_LEN+1];
	char r1_item_num[tt0053_R1_ITEM_COUNT][tt0053_R1_ITM_NO_LEN+1];
	char r1_item_qty[tt0053_R1_ITEM_COUNT][tt0053_R1_QUTY_LEN+1];
        
} tt0053_st_send;



typedef struct
{
	char r1_item_num[tt0053_R1_ITM_NO_LEN+1];
	char r1_item_edp[tt0053_R1_EDP_LEN+1];
	char r1_item_desc[tt0053_R1_DESC_LEN+1];
	char r1_item_qty[tt0053_R1_QTY_LEN+1];
	char r1_upsl_X[tt0053_R1_UPS_LEN+1];
	char r1_image_ovrd[tt0053_R1_IMG_OVRD_LEN+1];
	char r1_item_price[tt0053_R1_PRICE_LEN+1];
	char r1_item_avail_inv[tt0053_R1_AVAIL_LEN+1];
	
} tt0053_r1_comp_array;



typedef struct
{
    char request_id[tt0053_REQ_ID_LEN+1];
    char record_id[tt0053_REC_ID_LEN+1];
    char userid[tt0053_USER_ID_LEN+1];
    char success[tt0053_SUCCESS_FLAG_LEN+1];
	char err_message[tt0053_ERR_LEN+1];
	char filler[tt0053_SEND_FILLER_LEN+1];

	char kit_item_num[tt0053_ITM_NO_LEN+1];
	char kit_item_edp[tt0053_EDP_LEN+1];
	char kit_item_desc[tt0053_DESC_LEN+1];
	char upsl_X[tt0053_UPS_LEN+1];
	char upsl_A[tt0053_UPS_LEN+1];
	char upsl_B[tt0053_UPS_LEN+1];
	char upsl_C[tt0053_UPS_LEN+1];
	char upsl_D[tt0053_UPS_LEN+1];
	char image_ovrd[tt0053_IMG_OVRD_LEN+1];
	char kit_item_price[tt0053_PRICE_LEN+1];
	char kit_item_avail_inv[tt0053_AVAIL_LEN+1];
	char r1_count[tt0053_R1_COUNT_LEN+1];
	char customization_flag[tt0053_CUSTOM_LEN+1];
	tt0053_r1_comp_array r1_component[tt0053_R1_COMP_COUNT];

} tt0053_st_recv;

