/* tt0075_defs.h  Get RFM Data */

/* Web to MACS Buffer  */

#define  tt0075_LAN_SEND_BUF_LEN        78 //51    
#define  tt0075_LAN_RECV_BUF_LEN        295 //250  

#define  tt0075_REQ_ID_LEN             4
#define  tt0075_REC_ID_LEN             4
#define  tt0075_COMPANY_LEN            2
#define  tt0075_DIVISION_LEN           2
#define  tt0075_USER_ID_LEN            16
#define  tt0075_IP_ADDR_LEN                      16
#define  tt0075_FILLER_LEN                      25
#define  tt0075_EDP_LEN                          9

/* MACS to Web Buffer  */

#define  tt0075_REQ_ID_LEN             4
#define  tt0075_REC_ID_LEN             4
#define  tt0075_USER_ID_LEN            16
#define  tt0075_SUCCESS_FLAG_LEN        1
#define  tt0075_ERR_LEN                         80
#define  tt0075_SEND_FILLER_LEN                 45


#define  tt0075_EDP_LEN                          9
#define  tt0075_CUST_TYPE_LEN                    2
#define  tt0075_CUST_STATUS_LEN          2
#define  tt0075_RECENCY_LEN                      1
#define  tt0075_FREQ_LEN                                 1
#define  tt0075_MONETARY_LEN                     1
#define  tt0075_TOT_DOLL_LEN                     9
#define  tt0075_TOT_ORD_LEN                      4
#define  tt0075_CURR_DATE_LEN                    8
#define  tt0075_ORIG_DATE_LEN                    8
#define  tt0075_ORIG_SRC_LEN                     16
#define  tt0075_LAST_SRC_LEN                     16
#define  tt0075_PURCH_CAT_LEN                    26

typedef struct
{
    char request_id[tt0075_REQ_ID_LEN+1];
    char record_id[tt0075_REC_ID_LEN+1];
    char company[tt0075_COMPANY_LEN+1];
    char division[tt0075_DIVISION_LEN+1];
    char userid[tt0075_USER_ID_LEN+1];
    char ip_address[tt0075_IP_ADDR_LEN+1];
        char filler[tt0075_FILLER_LEN+1];
        char cust_edp[tt0075_EDP_LEN+1];
        
} tt0075_st_send;


typedef struct
{
    char request_id[tt0075_REQ_ID_LEN+1];
    char record_id[tt0075_REC_ID_LEN+1];
    char userid[tt0075_USER_ID_LEN+1];
    char success[tt0075_SUCCESS_FLAG_LEN+1];
        char err_message[tt0075_ERR_LEN+1];
        char filler[tt0075_SEND_FILLER_LEN+1];

        char cust_edp[tt0075_EDP_LEN+1];
        char cust_type[tt0075_CUST_TYPE_LEN+1];
        char cust_status[tt0075_CUST_STATUS_LEN+1];
        char recency_score[tt0075_RECENCY_LEN+1];
        char frequency_score[tt0075_FREQ_LEN+1];
        char monetary_score[tt0075_MONETARY_LEN+1];
        char cust_tot_doll[tt0075_TOT_DOLL_LEN+1];
        char cust_tot_ord[tt0075_TOT_ORD_LEN+1];
        char cust_curr_date[tt0075_CURR_DATE_LEN+1];
        char cust_orig_date[tt0075_ORIG_DATE_LEN+1];
        char cust_orig_source[tt0075_ORIG_SRC_LEN+1];
        char cust_last_source[tt0075_LAST_SRC_LEN+1];
        char cust_purch_cat1[tt0075_PURCH_CAT_LEN+1];
        char cust_purch_cat2[tt0075_PURCH_CAT_LEN+1];
        
} tt0075_st_recv;

