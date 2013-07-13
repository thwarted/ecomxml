/* tt0127_defs.h Create/Update Predictive Response Survey Data */


#define  tt0127_LAN_SEND_BUF_LEN        78 
#define  tt0127_LAN_RECV_BUF_LEN        331  

/* Web to MACS Buffer  */

#define  tt0127_REQ_ID_LEN             4
#define  tt0127_REC_ID_LEN             4
#define  tt0127_COMPANY_LEN            2
#define  tt0127_DIVISION_LEN           2
#define  tt0127_USER_ID_LEN            16
#define  tt0127_IP_ADDR_LEN            16
#define  tt0127_FILLER_LEN                     25

#define  tt0127_CUST_EDP_LEN               9



/* MACS to Web Buffer   */

#define  tt0127_REQ_ID_LEN             4
#define  tt0127_REC_ID_LEN             4
#define  tt0127_USER_ID_LEN            16
#define  tt0127_SUCCESS_FLAG_LEN       1
#define  tt0127_ERR_LEN                        80
#define  tt0127_SEND_FILLER_LEN            45

#define  tt0127_CUST_EDP_LEN           9
#define  tt0127_CUST_NO_LEN            10
#define  tt0127_FNAME_LEN              16
#define  tt0127_LNAME_LEN              16
#define  tt0127_COMP_NAME_LEN          30
#define  tt0127_SURVEY_DATA_LEN        100


typedef struct
{
    char request_id[tt0127_REQ_ID_LEN+1];
    char record_id[tt0127_REC_ID_LEN+1];
    char company[tt0127_COMPANY_LEN+1];
    char division[tt0127_DIVISION_LEN+1];
    char userid[tt0127_USER_ID_LEN+1];
    char ip_address[tt0127_IP_ADDR_LEN+1];
        char filler[tt0127_FILLER_LEN+1];

    char cust_edp[tt0127_CUST_EDP_LEN+1];
    

} tt0127_st_send;




typedef struct
{
    char request_id[tt0127_REQ_ID_LEN+1];
    char record_id[tt0127_REC_ID_LEN+1];
    char userid[tt0127_USER_ID_LEN+1];
    char success[tt0127_SUCCESS_FLAG_LEN+1];
        char err_message[tt0127_ERR_LEN+1];
        char filler[tt0127_SEND_FILLER_LEN+1];

    char cust_edp[tt0127_CUST_EDP_LEN+1];
    char cust_no[tt0127_CUST_NO_LEN+1];
    char fname[tt0127_FNAME_LEN+1];
    char lname[tt0127_LNAME_LEN+1];
        char comp_name[tt0127_COMP_NAME_LEN+1];
        char survey_data[tt0127_SURVEY_DATA_LEN+1];
        
} tt0127_st_recv;