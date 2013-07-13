/* tt0073_defs.h  Print Gift Certificate */

/* Web to MACS Buffer  */

#define  tt0073_LAN_SEND_BUF_LEN        105 //78    
#define  tt0073_LAN_RECV_BUF_LEN        345 //300  

#define  tt0073_REQ_ID_LEN             4
#define  tt0073_REC_ID_LEN             4
#define  tt0073_COMPANY_LEN            2
#define  tt0073_DIVISION_LEN           2
#define  tt0073_USER_ID_LEN            16
#define  tt0073_IP_ADDR_LEN			 16
#define  tt0073_FILLER_LEN			25

#define  tt0073_ORD_NO_LEN			 8
#define  tt0073_ORD_SUF_LEN			 4
#define  tt0073_ITM_NO_LEN			 20
#define  tt0073_PGN_LEN				 4

/* MACS to Web Buffer */

#define  tt0073_REQ_ID_LEN             4
#define  tt0073_REC_ID_LEN             4
#define  tt0073_USER_ID_LEN            16
#define  tt0073_SUCCESS_FLAG_LEN        1
#define  tt0073_ERR_LEN				80
#define  tt0073_SEND_FILLER_LEN			45


#define  tt0073_ORD_NO_LEN			 8
#define  tt0073_ORD_SUF_LEN			 4
#define  tt0073_FN_LEN				 16
#define  tt0073_LN_LEN				 16
#define  tt0073_SHIPTO_FN_LEN			 16
#define  tt0073_SHIPTO_LN_LEN			 16
#define  tt0073_ITM_NO_LEN			 20
#define  tt0073_GC_NO_LEN				 9
#define  tt0073_GC_AMT_LEN			 11
#define  tt0073_GC_EXP_LEN			 8
#define  tt0073_PGN_LEN				 4
#define  tt0073_TOT_PGN_LEN			 4


typedef struct
{
    char request_id[tt0073_REQ_ID_LEN+1];
    char record_id[tt0073_REC_ID_LEN+1];
    char company[tt0073_COMPANY_LEN+1];
    char division[tt0073_DIVISION_LEN+1];
    char userid[tt0073_USER_ID_LEN+1];
    char ip_address[tt0073_IP_ADDR_LEN+1];
	char filler[tt0073_FILLER_LEN+1];
	char ord_num[tt0073_ORD_NO_LEN+1];
	char ord_suffix[tt0073_ORD_SUF_LEN+1];
	char item_num[tt0073_ITM_NO_LEN+1];
	char page_num[tt0073_PGN_LEN+1];
        
} tt0073_st_send;


typedef struct
{
    char request_id[tt0073_REQ_ID_LEN+1];
    char record_id[tt0073_REC_ID_LEN+1];
    char userid[tt0073_USER_ID_LEN+1];
    char success[tt0073_SUCCESS_FLAG_LEN+1];
	char err_message[tt0073_ERR_LEN+1];
	char filler[tt0073_SEND_FILLER_LEN+1];

	char ord_num[tt0073_ORD_NO_LEN+1];
	char ord_suffix[tt0073_ORD_SUF_LEN+1];
	char fname[tt0073_FN_LEN+1];
	char lname[tt0073_LN_LEN+1];
	char shipto_fname[tt0073_SHIPTO_FN_LEN+1];
	char shipto_lname[tt0073_SHIPTO_LN_LEN+1];
	char item_num[tt0073_ITM_NO_LEN+1];
	char gc_num[tt0073_GC_NO_LEN+1];
	char gc_amount[tt0073_GC_AMT_LEN+1];
	char gc_exp_date[tt0073_GC_EXP_LEN+1];
	char current_page[tt0073_PGN_LEN+1];
	char total_page[tt0073_TOT_PGN_LEN+1];

} tt0073_st_recv;

