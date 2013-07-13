/* tt0093_defs.h Standing Order Maintenance */

#define  tt0093_LAN_SEND_BUF_LEN        17529 //18629 
#define  tt0093_LAN_RECV_BUF_LEN        159  

/* Web to MACS Buffer  */

#define  tt0093_REQ_ID_LEN             4
#define  tt0093_REC_ID_LEN             4
#define  tt0093_COMPANY_LEN            2
#define  tt0093_DIVISION_LEN           2
#define  tt0093_USER_ID_LEN            16
#define  tt0093_IP_ADDR_LEN            16
#define  tt0093_FILLER_LEN				25

#define  tt0093_UPDATE_FLAG_LEN			1
#define  tt0093_HDR_CUST_EDP_LEN		9
#define  tt0093_HDR_PM_LEN				20
#define  tt0093_HDR_PM_TYPE_LEN			02
#define  tt0093_HDR_CC_NO_LEN			20
#define  tt0093_HDR_CC_EXP_LEN			04
#define  tt0093_HDR_CO_LEN				02
#define  tt0093_HDR_DIV_LEN				02

#define  tt0093_STAND_ORD_DETAIL		100

#define  tt0093_DELETE_ITEM_LEN			1
#define  tt0093_DTL_SHIPTO_EDP_LEN		9
#define  tt0093_DTL_SHIPTO_SM_LEN		80
#define  tt0093_DTL_SHIPTO_SM_TYPE_LEN	2
#define  tt0093_DTL_NEXT_DATE_LEN		8
#define  tt0093_DTL_TEMP_DATE_LEN		8
#define  tt0093_DTL_STOP_DATE_LEN		8
#define  tt0093_DTL_POST_START_LEN		4
#define  tt0093_DTL_POST_STOP_LEN		4
#define  tt0093_DTL_ITEM_EDP_LEN		9
#define  tt0093_DTL_ITEM_NO_LEN			9
#define  tt0093_DTL_ITEM_SHIPS_LEN		4
#define  tt0093_DTL_ITEM_INT_N_LEN		3
#define  tt0093_DTL_ITEM_INT_X_LEN		25



/* MACS to Web Buffer  */

#define  tt0093_REQ_ID_LEN             4
#define  tt0093_REC_ID_LEN             4
#define  tt0093_USER_ID_LEN            16
#define  tt0093_SUCCESS_FLAG_LEN        1
#define  tt0093_ERR_LEN				80
#define  tt0093_SEND_FILLER_LEN			45

#define  tt0093_MESSAGE_LEN				80
#define  tt0093_CUST_EDP_LEN			9


typedef struct
{
	char dtl_delete_item[tt0093_DELETE_ITEM_LEN+1];
    char dtl_shipto_edp[tt0093_DTL_SHIPTO_EDP_LEN+1];
	char dtl_shipto_sm[tt0093_DTL_SHIPTO_SM_LEN+1];
	char dtl_shipto_sm_type[tt0093_DTL_SHIPTO_SM_TYPE_LEN+1];
	char dtl_next_date[tt0093_DTL_NEXT_DATE_LEN+1];
	char dtl_temp_date[tt0093_DTL_TEMP_DATE_LEN+1];
	char dtl_stop_date[tt0093_DTL_STOP_DATE_LEN+1];
	char dtl_post_start[tt0093_DTL_POST_START_LEN+1];
	char dtl_post_stop[tt0093_DTL_POST_STOP_LEN+1];
	char dtl_item_edp[tt0093_DTL_ITEM_EDP_LEN+1];
	char dtl_item_no[tt0093_DTL_ITEM_NO_LEN+1];
	char dtl_item_ships[tt0093_DTL_ITEM_SHIPS_LEN+1];
	char dtl_item_int_n[tt0093_DTL_ITEM_INT_N_LEN+1];
	char dtl_item_int_x[tt0093_DTL_ITEM_INT_X_LEN+1];
} tt0093_stand_ord_detail_array;

typedef struct
{
    char request_id[tt0093_REQ_ID_LEN+1];
    char record_id[tt0093_REC_ID_LEN+1];
    char company[tt0093_COMPANY_LEN+1];
    char division[tt0093_DIVISION_LEN+1];
    char userid[tt0093_USER_ID_LEN+1];
    char ip_address[tt0093_IP_ADDR_LEN+1];
	char filler[tt0093_FILLER_LEN+1];

	char update_flag[tt0093_UPDATE_FLAG_LEN+1];
    char hdr_cust_edp[tt0093_HDR_CUST_EDP_LEN+1];
	char hdr_pm[tt0093_HDR_PM_LEN+1];
	char hdr_pm_type[tt0093_HDR_PM_TYPE_LEN+1];
	char hdr_cc_no[tt0093_HDR_CC_NO_LEN+1];
	char hdr_cc_exp[tt0093_HDR_CC_EXP_LEN+1];
	char hdr_co[tt0093_HDR_CO_LEN+1];
	char hdr_div[tt0093_HDR_DIV_LEN+1];

    tt0093_stand_ord_detail_array stand_ord_detail[tt0093_STAND_ORD_DETAIL];

} tt0093_st_send;


typedef struct
{
    char request_id[tt0093_REQ_ID_LEN+1];
    char record_id[tt0093_REC_ID_LEN+1];
    char userid[tt0093_USER_ID_LEN+1];
    char success[tt0093_SUCCESS_FLAG_LEN+1];
	char err_message[tt0093_ERR_LEN+1];
	char filler[tt0093_SEND_FILLER_LEN+1];

	char message[tt0093_MESSAGE_LEN+1];
	char cust_edp[tt0093_CUST_EDP_LEN+1];

} tt0093_st_recv;