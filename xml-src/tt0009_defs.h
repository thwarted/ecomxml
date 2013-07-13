/* tt0009_defs.h  Customer Inquiry */


#define  tt0009_LAN_SEND_BUF_LEN           338  //337  
#define  tt0009_LAN_RECV_BUF_LEN           528  //525  

/* Web to MACS Buffer  */

#define  tt0009_REQ_ID_LEN                4
#define  tt0009_REC_ID_LEN                4
#define  tt0009_COMPANY_LEN               2
#define  tt0009_DIVISION_LEN              2
#define  tt0009_USER_ID_LEN               16
#define  tt0009_IP_ADDR_LEN               16
#define  tt0009_FILLER_LEN				25

#define  tt0009_PASSWORD_LEN              16
#define  tt0009_ORD_NO_LEN		        8
#define  tt0009_EDP_LEN                   9
#define  tt0009_CART_UPDATE_LEN           1
#define  tt0009_FN_LEN                    16
#define  tt0009_LN_LEN                    16
#define  tt0009_COMP_NAME_LEN             30
#define  tt0009_STREET_LEN                30
#define  tt0009_CITY_LEN                  30
#define  tt0009_STATE_LEN                 2
#define  tt0009_ZIP_LEN                   14
#define  tt0009_COUNTRY_LEN               4
#define  tt0009_PHONE_LEN                 16
#define  tt0009_EMAIL_LEN                 50
#define  tt0009_CUST_NO_LEN				10
#define  tt0009_CUSTOMER_LOGON_LEN      1

/* MACS to Web Buffer  */

#define  tt0009_REQ_ID_LEN                4
#define  tt0009_REC_ID_LEN                4
#define  tt0009_USER_ID_LEN               16
#define  tt0009_SUCCESS_FLAG_LEN        1
#define  tt0009_ERR_LEN			 80
#define  tt0009_SEND_FILLER_LEN			45



#define  tt0009_PASSWORD_LEN              16
#define  tt0009_MORE_LEN					1
#define  tt0009_EDP_LEN                   9
#define  tt0009_FN_LEN                    16
#define  tt0009_LN_LEN                    16
#define  tt0009_COMP_NAME_LEN             30
#define  tt0009_STREET_LEN                30
#define  tt0009_CITY_LEN                  30
#define  tt0009_STATE_LEN                 2
#define  tt0009_ZIP_LEN                   14
#define  tt0009_RET_COUNTRY_LEN           20
#define  tt0009_PHONE_LEN                 16
#define  tt0009_EMAIL_LEN                 50
#define  tt0009_REF_LEN                   30
#define  tt0009_CUST_NO_LEN				10
#define  tt0009_RETURNING_CUST_LEN       1
#define  tt0009_STAND_ORD_CUST_LEN		1
#define  tt0009_CUSTOMER_LOGON_LEN		1

typedef struct
{
       char request_id[tt0009_REQ_ID_LEN+1];
       char record_id[tt0009_REC_ID_LEN+1];
       char company[tt0009_COMPANY_LEN+1];
       char division[tt0009_DIVISION_LEN+1];
       char userid[tt0009_USER_ID_LEN+1];
       char ip_address[tt0009_IP_ADDR_LEN+1];
	   char filler[tt0009_FILLER_LEN+1];
       char password[tt0009_PASSWORD_LEN+1];
       char ord_num[tt0009_ORD_NO_LEN+1];
       char cust_edp[tt0009_EDP_LEN+1];
       char cart_update[tt0009_CART_UPDATE_LEN+1];
       char fname[tt0009_FN_LEN+1];
       char lname[tt0009_LN_LEN+1];
       char comp_name[tt0009_COMP_NAME_LEN+1];
       char street[tt0009_STREET_LEN+1];
       char city[tt0009_CITY_LEN+1];
       char state[tt0009_STATE_LEN+1];
       char zipcode[tt0009_ZIP_LEN+1];
       char country[tt0009_COUNTRY_LEN+1];
       char day_phone[tt0009_PHONE_LEN+1];
       char night_phone[tt0009_PHONE_LEN+1];
       char email_addr[tt0009_EMAIL_LEN+1];
	   char cust_num[tt0009_CUST_NO_LEN+1];
	   char customer_logon[tt0009_CUSTOMER_LOGON_LEN+1];

} tt0009_st_send;


typedef struct
{
       char request_id[tt0009_REQ_ID_LEN+1];
       char record_id[tt0009_REC_ID_LEN+1];
       char userid[tt0009_USER_ID_LEN+1];
    char success[tt0009_SUCCESS_FLAG_LEN+1];
	char err_message[tt0009_ERR_LEN+1];
	char filler[tt0009_SEND_FILLER_LEN+1];

       char password[tt0009_PASSWORD_LEN+1];
	   char more_flag[tt0009_MORE_LEN+1];
       char cust_edp[tt0009_EDP_LEN+1];
       char fname[tt0009_FN_LEN+1];
       char lname[tt0009_LN_LEN+1];
       char comp_name[tt0009_COMP_NAME_LEN+1];
       char street[tt0009_STREET_LEN+1];
       char city[tt0009_CITY_LEN+1];
       char state[tt0009_STATE_LEN+1];
       char zipcode[tt0009_ZIP_LEN+1];
       char country[tt0009_RET_COUNTRY_LEN+1];
       char day_phone[tt0009_PHONE_LEN+1];
       char night_phone[tt0009_PHONE_LEN+1];
       char email_addr[tt0009_EMAIL_LEN+1];
       char ref1[tt0009_REF_LEN+1];
       char ref2[tt0009_REF_LEN+1];
	   char cust_num[tt0009_CUST_NO_LEN+1];
	char returning_cust[tt0009_RETURNING_CUST_LEN+1];
	char stand_ord_cust[tt0009_STAND_ORD_CUST_LEN+1];
	char customer_logon[tt0009_CUSTOMER_LOGON_LEN+1];

} tt0009_st_recv;