/* tt0125_defs.h Create/Update Predictive Response Survey Data */


#define  tt0125_LAN_SEND_BUF_LEN        179 
#define  tt0125_LAN_RECV_BUF_LEN        232  

/* Web to MACS Buffer  */

#define  tt0125_REQ_ID_LEN             4
#define  tt0125_REC_ID_LEN             4
#define  tt0125_COMPANY_LEN            2
#define  tt0125_DIVISION_LEN           2
#define  tt0125_USER_ID_LEN            16
#define  tt0125_IP_ADDR_LEN            16
#define  tt0125_FILLER_LEN                     25

#define  tt0125_TRANS_FLAG_LEN         1
#define  tt0125_CUST_EDP_LEN               9
#define  tt0125_SURVEY_DATA_LEN        100


/* MACS to Web Buffer   */

#define  tt0125_REQ_ID_LEN             4
#define  tt0125_REC_ID_LEN             4
#define  tt0125_USER_ID_LEN            16
#define  tt0125_SUCCESS_FLAG_LEN       1
#define  tt0125_ERR_LEN                        80
#define  tt0125_SEND_FILLER_LEN            45

#define  tt0125_TRANS_FLAG_LEN         1
#define  tt0125_CUST_EDP_LEN           9
#define  tt0125_CUST_NO_LEN            10
#define  tt0125_FNAME_LEN              16
#define  tt0125_LNAME_LEN              16
#define  tt0125_COMP_NAME_LEN          30


typedef struct
{
    char request_id[tt0125_REQ_ID_LEN+1];
    char record_id[tt0125_REC_ID_LEN+1];
    char company[tt0125_COMPANY_LEN+1];
    char division[tt0125_DIVISION_LEN+1];
    char userid[tt0125_USER_ID_LEN+1];
    char ip_address[tt0125_IP_ADDR_LEN+1];
        char filler[tt0125_FILLER_LEN+1];

    char trans_flag[tt0125_TRANS_FLAG_LEN+1];
    char cust_edp[tt0125_CUST_EDP_LEN+1];
    char survey_data[tt0125_SURVEY_DATA_LEN+1];


} tt0125_st_send;




typedef struct
{
    char request_id[tt0125_REQ_ID_LEN+1];
    char record_id[tt0125_REC_ID_LEN+1];
    char userid[tt0125_USER_ID_LEN+1];
    char success[tt0125_SUCCESS_FLAG_LEN+1];
        char err_message[tt0125_ERR_LEN+1];
        char filler[tt0125_SEND_FILLER_LEN+1];


    char trans_flag[tt0125_TRANS_FLAG_LEN+1];
    char cust_edp[tt0125_CUST_EDP_LEN+1];
    char cust_no[tt0125_CUST_NO_LEN+1];
    char fname[tt0125_FNAME_LEN+1];
    char lname[tt0125_LNAME_LEN+1];
        char comp_name[tt0125_COMP_NAME_LEN+1];
        
} tt0125_st_recv;