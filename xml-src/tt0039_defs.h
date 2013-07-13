/* tt0039_defs.h  Update Ship_To*/

#define  tt0039_LAN_SEND_BUF_LEN        990  // 738 //729  //679     
#define  tt0039_LAN_RECV_BUF_LEN        184  //926 //800  

/* Web to MACS Buffer  */

#define  tt0039_REQ_ID_LEN              4
#define  tt0039_REC_ID_LEN              4
#define  tt0039_COMPANY_LEN             2
#define  tt0039_DIVISION_LEN            2
#define  tt0039_USER_ID_LEN             16
#define  tt0039_IP_ADDR_LEN			    16
#define  tt0039_FILLER_LEN			    25

#define  tt0039_SHIPTO_NO_LEN			 2
#define  tt0039_SHIPTO_FN_LEN			 16
#define  tt0039_SHIPTO_MI_LEN			 1
#define  tt0039_SHIPTO_LN_LEN			 16
#define  tt0039_SHIPTO_TITLE_LEN	     2
#define  tt0039_SHIPTO_COMP_LEN		     30
#define  tt0039_JAP_LEN			         1
#define  tt0039_SHIPTO_REF1_LEN		     30
#define  tt0039_SHIPTO_REF2_LEN		     30
#define  tt0039_SHIPTO_STREET_LEN		 30
#define  tt0039_SHIPTO_CITY_LEN		     30
#define  tt0039_SHIPTO_STATE_LEN		 2
#define  tt0039_SHIPTO_ZIP_LEN		     14
#define  tt0039_SHIPTO_COUNTRY_LEN	     4
#define  tt0039_SHIPTO_PHONE_LEN	     14
#define  tt0039_SHIPTO_FO_LEN     	     8
#define  tt0039_SHIPTO_COM_LEN	         60
#define  tt0039_SHIPTO_PKSLP_LEN	     1
#define  tt0039_SHIPTO_LAB_LEN	         1
#define  tt0039_SHIPTO_GRT_LEN		     1
#define  tt0039_SHIPTO_SHIP_METH_LEN	 2
#define  tt0039_SHIPTO_EDP_LEN			 9
#define  tt0039_SHIP_TO_EMAIL_LEN        50

/* MACS to Web Buffer  */

#define  tt0039_REQ_ID_LEN               4
#define  tt0039_REC_ID_LEN               4
#define  tt0039_IP_ADDR_LEN			     16
#define  tt0039_SUCCESS_FLAG_LEN         1
#define  tt0039_ERR_LEN				     80
#define  tt0039_SEND_FILLER_LEN			 45


#define  tt0039_SHIPTO_COUNT             1

#define  tt0039_SHIPTO_NO_LEN			 2
#define  tt0039_SHIPTO_FN_LEN			 16
#define  tt0039_SHIPTO_LN_LEN			 16


typedef struct
{
    char request_id[tt0039_REQ_ID_LEN+1];
    char record_id[tt0039_REC_ID_LEN+1];
    char company[tt0039_COMPANY_LEN+1];
    char division[tt0039_DIVISION_LEN+1];
    char userid[tt0039_USER_ID_LEN+1];
    char ip_address[tt0039_IP_ADDR_LEN+1];
	char filler[tt0039_FILLER_LEN+1];
	char shipto_num[tt0039_SHIPTO_NO_LEN+1];
	char shipto_fname[tt0039_SHIPTO_FN_LEN+1];
	char shipto_minit[tt0039_SHIPTO_MI_LEN+1];
	char shipto_lname[tt0039_SHIPTO_LN_LEN+1];
	char shipto_title[tt0039_SHIPTO_TITLE_LEN+1];
	char shipto_company[tt0039_SHIPTO_COMP_LEN+1];
	char shipto_jap_flag[tt0039_JAP_LEN+1];
	char shipto_ref1[tt0039_SHIPTO_REF1_LEN+1];
	char shipto_ref2[tt0039_SHIPTO_REF2_LEN+1];
	char shipto_street[tt0039_SHIPTO_STREET_LEN+1];
	char shipto_city[tt0039_SHIPTO_CITY_LEN+1];
	char shipto_state[tt0039_SHIPTO_STATE_LEN+1];
	char shipto_zipcode[tt0039_SHIPTO_ZIP_LEN+1];
	char shipto_country[tt0039_SHIPTO_COUNTRY_LEN+1];
	char shipto_phone[tt0039_SHIPTO_PHONE_LEN+1];
	char shipto_fo_date[tt0039_SHIPTO_FO_LEN+1];

	char shipto_comment1[tt0039_SHIPTO_COM_LEN+1];
	char shipto_comment2[tt0039_SHIPTO_COM_LEN+1];
	char shipto_pkslp1[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label1[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet1[tt0039_SHIPTO_GRT_LEN+1];
	char shipto_pkslp2[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label2[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet2[tt0039_SHIPTO_GRT_LEN+1];

	char shipto_ship_method[tt0039_SHIPTO_SHIP_METH_LEN+1];

	char shipto_comment3[tt0039_SHIPTO_COM_LEN+1];
	char shipto_comment4[tt0039_SHIPTO_COM_LEN+1];
	char shipto_pkslp3[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label3[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet3[tt0039_SHIPTO_GRT_LEN+1];
	char shipto_pkslp4[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label4[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet4[tt0039_SHIPTO_GRT_LEN+1];

	char shipto_comment5[tt0039_SHIPTO_COM_LEN+1];
	char shipto_comment6[tt0039_SHIPTO_COM_LEN+1];
	char shipto_pkslp5[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label5[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet5[tt0039_SHIPTO_GRT_LEN+1];
	char shipto_pkslp6[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label6[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet6[tt0039_SHIPTO_GRT_LEN+1];

	char shipto_comment7[tt0039_SHIPTO_COM_LEN+1];
	char shipto_comment8[tt0039_SHIPTO_COM_LEN+1];
	char shipto_pkslp7[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label7[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet7[tt0039_SHIPTO_GRT_LEN+1];
	char shipto_pkslp8[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label8[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet8[tt0039_SHIPTO_GRT_LEN+1];

	char shipto_comment9[tt0039_SHIPTO_COM_LEN+1];
	char shipto_comment10[tt0039_SHIPTO_COM_LEN+1];
	char shipto_pkslp9[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label9[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet9[tt0039_SHIPTO_GRT_LEN+1];
	char shipto_pkslp10[tt0039_SHIPTO_PKSLP_LEN+1];
	char shipto_label10[tt0039_SHIPTO_LAB_LEN+1];
	char shipto_greet10[tt0039_SHIPTO_GRT_LEN+1];

	char shipto_edp[tt0039_SHIPTO_EDP_LEN+1];
	char ship_to_email[tt0039_SHIP_TO_EMAIL_LEN+1];

     
} tt0039_st_send;

typedef struct
{
    char shipto_num[tt0039_SHIPTO_NO_LEN+1];
	char shipto_fname[tt0039_SHIPTO_FN_LEN+1];
	char shipto_lname[tt0039_SHIPTO_LN_LEN+1];

} tt0039_shipto_array;


typedef struct
{
    char request_id[tt0039_REQ_ID_LEN+1];
    char record_id[tt0039_REC_ID_LEN+1];
    char userid[tt0039_USER_ID_LEN+1];
    char success[tt0039_SUCCESS_FLAG_LEN+1];
	char err_message[tt0039_ERR_LEN+1];
	char filler[tt0039_SEND_FILLER_LEN+1];

	tt0039_shipto_array shipto[tt0039_SHIPTO_COUNT];

} tt0039_st_recv;

