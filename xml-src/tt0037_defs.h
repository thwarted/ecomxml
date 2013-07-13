/* tt0037_defs.h   Check Ship_To */

/* Web to MACS Buffer  */

#define  tt0037_LAN_SEND_BUF_LEN        69 //42    
#define  tt0037_LAN_RECV_BUF_LEN        407 //6170 //926 //800  

#define  tt0037_REQ_ID_LEN             4
#define  tt0037_REC_ID_LEN             4
#define  tt0037_COMPANY_LEN            2
#define  tt0037_DIVISION_LEN           2
#define  tt0037_USER_ID_LEN            16
#define  tt0037_IP_ADDR_LEN			 16
#define  tt0037_FILLER_LEN			25


/* MACS to Web Buffer  */

#define  tt0037_REQ_ID_LEN             4
#define  tt0037_REC_ID_LEN             4
#define  tt0037_USER_ID_LEN            16
#define  tt0037_SUCCESS_FLAG_LEN        1
#define  tt0037_ERR_LEN				80
#define  tt0037_SEND_FILLER_LEN			45

#define  tt0037_SHIPTO_COUNT           1

#define  tt0037_SHIPTO_NO_LEN			 2
#define  tt0037_SHIPTO_FN_LEN			 16
#define  tt0037_SHIPTO_LN_LEN			 16
#define  tt0037_SHIPTO_MI_LEN			1
#define  tt0037_SHIPTO_TITLE_LEN		2
#define  tt0037_SHIPTO_CO_NAME_LEN		30
#define  tt0037_SHIPTO_STREET_LEN		30
#define  tt0037_SHIPTO_CITY_LEN			30
#define  tt0037_SHIPTO_STATE_LEN		2
#define  tt0037_SHIPTO_ZIP_LEN			14
#define  tt0037_SHIPTO_COUNTRY_LEN		4
#define  tt0037_SHIPTO_REF1_LEN			30
#define  tt0037_SHIPTO_REF2_LEN			30
#define  tt0037_SHIPTO_JAP_REF1_LEN		25
#define  tt0037_SHIPTO_JAP_PREF_LEN		20
#define  tt0037_SHIPTO_JAP_APT_LEN		15
#define  tt0037_SHIPTO_PHONE_NO_LEN		14
#define  tt0037_SHIPTO_CUST_NO_LEN		10
#define  tt0037_SHIPTO_EDP_LEN			9
#define  tt0037_JAP_FLAG_LEN			1



typedef struct
{
    char request_id[tt0037_REQ_ID_LEN+1];
    char record_id[tt0037_REC_ID_LEN+1];
    char company[tt0037_COMPANY_LEN+1];
    char division[tt0037_DIVISION_LEN+1];
    char userid[tt0037_USER_ID_LEN+1];
	char filler[tt0037_FILLER_LEN+1];
    char ip_address[tt0037_IP_ADDR_LEN+1];
        
} tt0037_st_send;

typedef struct
{
    char shipto_num[tt0037_SHIPTO_NO_LEN+1];
	char shipto_fname[tt0037_SHIPTO_FN_LEN+1];
	char shipto_lname[tt0037_SHIPTO_LN_LEN+1];
	char shipto_mi[tt0037_SHIPTO_MI_LEN+1];
	char shipto_title[tt0037_SHIPTO_TITLE_LEN+1];
	char shipto_co_name[tt0037_SHIPTO_CO_NAME_LEN+1];
	char shipto_street[tt0037_SHIPTO_STREET_LEN+1];
	char shipto_city[tt0037_SHIPTO_CITY_LEN+1];
	char shipto_state[tt0037_SHIPTO_STATE_LEN+1];
	char shipto_zip[tt0037_SHIPTO_ZIP_LEN+1];
	char shipto_country[tt0037_SHIPTO_COUNTRY_LEN+1];	
	char shipto_ref1[tt0037_SHIPTO_REF1_LEN+1];
	char shipto_ref2[tt0037_SHIPTO_REF2_LEN+1];
	char shipto_jap_ref1[tt0037_SHIPTO_JAP_REF1_LEN+1];
	char shipto_jap_pref[tt0037_SHIPTO_JAP_PREF_LEN+1];
	char shipto_jap_apt[tt0037_SHIPTO_JAP_APT_LEN+1];
	char shipto_phone_no[tt0037_SHIPTO_PHONE_NO_LEN+1];
	char shipto_cust_no[tt0037_SHIPTO_CUST_NO_LEN+1];
	char shipto_edp[tt0037_SHIPTO_EDP_LEN+1];
	char jap_flag[tt0037_JAP_FLAG_LEN+1];


} tt0037_shipto_array;


typedef struct
{
    char request_id[tt0037_REQ_ID_LEN+1];
    char record_id[tt0037_REC_ID_LEN+1];
    char userid[tt0037_USER_ID_LEN+1];
    char success[tt0037_SUCCESS_FLAG_LEN+1];
	char err_message[tt0037_ERR_LEN+1];
	char filler[tt0037_SEND_FILLER_LEN+1];

	tt0037_shipto_array shipto[tt0037_SHIPTO_COUNT];

} tt0037_st_recv;

