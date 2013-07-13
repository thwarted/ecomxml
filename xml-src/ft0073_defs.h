/* ft0073_defs.h  Add to wish list */


#define  ft0073_LAN_SEND_BUF_LEN            2291
#define  ft0073_LAN_RECV_BUF_LEN            160

/* Web to MACS Buffer */

#define  ft0073_REQ_ID_LEN                 4
#define  ft0073_REC_ID_LEN                 4
#define  ft0073_COMPANY_LEN                2
#define  ft0073_DIVISION_LEN               2
#define  ft0073_USER_ID_LEN                16
#define  ft0073_IP_ADDR_LEN                16
#define  ft0073_FILLER_LEN                 25
#define  ft0073_ID_LEN                     16
#define  ft0073_CUST_EDP_LEN               9
#define  ft0073_ITM_NO_LEN                 20
#define  ft0073_QTY_LEN                    4
#define  ft0073_FLAG_LEN                   1
#define  ft0073_CUSTOM_LEN                 60
#define  ft0073_DATE_LEN                   8
#define  ft0073_GLB_LEN                    1
#define  ft0073_SOURCE_LEN                 16
#define  ft0073_PASSWORD_LEN               16
#define  ft0073_PREF_SHIPTO_LEN            9
#define  ft0073_CUSTOM_LOOP                25
#define  ft0073_R1_LOOP                    25


/* MACS to Web Buffer */

#define  ft0073_REQ_ID_LEN                 4
#define  ft0073_REC_ID_LEN                 4
#define  ft0073_USER_ID_LEN                16
#define  ft0073_SUCCESS_FLAG_LEN           1
#define  ft0073_ERR_LEN                    80
#define  ft0073_SEND_FILLER_LEN            45


typedef struct
{
    char request_id[ft0073_REQ_ID_LEN+1];
    char record_id[ft0073_REC_ID_LEN+1];
    char company[ft0073_COMPANY_LEN+1];
    char division[ft0073_DIVISION_LEN+1];
    char userid[ft0073_USER_ID_LEN+1];
    char ip_address[ft0073_IP_ADDR_LEN+1];
    char filler[ft0073_FILLER_LEN+1];
	char wl_company[ft0073_COMPANY_LEN+1];
	char wl_division[ft0073_DIVISION_LEN+1];
	char wl_id[ft0073_ID_LEN+1];
	char cust_edp[ft0073_CUST_EDP_LEN+1];
	char item_no[ft0073_ITM_NO_LEN+1];
	char qty[ft0073_QTY_LEN+1];
	char custom_flag[ft0073_FLAG_LEN+1];
	char kit_flag[ft0073_FLAG_LEN+1];
    char custom[ft0073_CUSTOM_LOOP][ft0073_CUSTOM_LEN+1];
    char r1_item_no[ft0073_R1_LOOP][ft0073_ITM_NO_LEN+1];
    char r1_item_qty[ft0073_R1_LOOP][ft0073_QTY_LEN+1];
	char multi_list[ft0073_FLAG_LEN+1];
	char wed_date[ft0073_DATE_LEN+1];
	char shp_date[ft0073_DATE_LEN+1];
	char reg_date[ft0073_DATE_LEN+1];
	char glb[ft0073_GLB_LEN+1];
	char source[ft0073_SOURCE_LEN+1];
	char password[ft0073_PASSWORD_LEN+1];
	char pref_shipto[ft0073_PREF_SHIPTO_LEN+1];


} ft0073_st_send;



typedef struct
{
    char request_id[ft0073_REQ_ID_LEN+1];
    char record_id[ft0073_REC_ID_LEN+1];
    char userid[ft0073_USER_ID_LEN+1];
    char success_flag[ft0073_SUCCESS_FLAG_LEN+1];
    char err_message[ft0073_ERR_LEN+1];
    char filler[ft0073_SEND_FILLER_LEN+1];

} ft0073_st_recv;


