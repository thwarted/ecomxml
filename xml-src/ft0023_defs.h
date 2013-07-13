/* ft0023_defs.h  Validate Source */


#define  ft0023_LAN_SEND_BUF_LEN            95
#define  ft0023_LAN_RECV_BUF_LEN            250


/* Web to MACS Buffer */

#define  ft0023_REQ_ID_LEN                 4
#define  ft0023_REC_ID_LEN                 4
#define  ft0023_COMPANY_LEN                2
#define  ft0023_DIVISION_LEN               2
#define  ft0023_USER_ID_LEN                16
#define  ft0023_IP_ADDR_LEN                16
#define  ft0023_FILLER_LEN                 25
#define  ft0023_OFFER_LEN                  8
#define  ft0023_USE_LEN                    8
#define  ft0023_CUST_NO_LEN                10


/* MACS to Web Buffer */

#define  ft0023_REQ_ID_LEN                 4
#define  ft0023_REC_ID_LEN                 4
#define  ft0023_USER_ID_LEN                16
#define  ft0023_SUCCESS_FLAG_LEN           1
#define  ft0023_ERR_LEN                    80
#define  ft0023_SEND_FILLER_LEN            45
#define  ft0023_SOURCE_LEN                 16
#define  ft0023_COMPANY_LEN                2
#define  ft0023_DIVISION_LEN               2
#define  ft0023_FLAG_LEN                   1
#define  ft0023_CC_LEN                     10
#define  ft0023_DATE_LEN                   8
#define  ft0023_PRODUCT_LEN                20
#define  ft0023_SHIP_METH_LEN              2
#define  ft0023_DISC_LEN                   8
#define  ft0023_EXP_CHAIN_LEN              1
#define  ft0023_PRODUCT_LOOP               2


typedef struct
{
    char request_id[ft0023_REQ_ID_LEN+1];
    char record_id[ft0023_REC_ID_LEN+1];
    char company[ft0023_COMPANY_LEN+1];
    char division[ft0023_DIVISION_LEN+1];
    char userid[ft0023_USER_ID_LEN+1];
    char ip_address[ft0023_IP_ADDR_LEN+1];
    char filler[ft0023_FILLER_LEN+1];
	char offer[ft0023_OFFER_LEN+1];
	char use[ft0023_USE_LEN+1];
	char cust_no[ft0023_CUST_NO_LEN+1];

} ft0023_st_send;

typedef struct
{
    char ins_product[ft0023_PRODUCT_LEN+1];

} product_23;


typedef struct
{
    char request_id[ft0023_REQ_ID_LEN+1];
    char record_id[ft0023_REC_ID_LEN+1];
    char userid[ft0023_USER_ID_LEN+1];
    char success_flag[ft0023_SUCCESS_FLAG_LEN+1];
    char err_message[ft0023_ERR_LEN+1];
    char filler[ft0023_SEND_FILLER_LEN+1];
	char source[ft0023_SOURCE_LEN+1];
	char company[ft0023_COMPANY_LEN+1];
	char division[ft0023_DIVISION_LEN+1];
	char install_flag[ft0023_FLAG_LEN+1];
	char invalid_cc[ft0023_CC_LEN+1];
	char future_date[ft0023_DATE_LEN+1];
	product_23 insert_product[ft0023_PRODUCT_LOOP];
	char ship_method[ft0023_SHIP_METH_LEN+1];
	char discount_table[ft0023_DISC_LEN+1];
	char exp_chain[ft0023_EXP_CHAIN_LEN+1];

} ft0023_st_recv;


