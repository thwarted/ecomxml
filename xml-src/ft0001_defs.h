/* ft0001_defs.h  LOGON */


#define  ft0001_LAN_SEND_BUF_LEN            119
#define  ft0001_LAN_RECV_BUF_LEN            200


/* Web to MACS Buffer */

#define  ft0001_REQ_ID_LEN                 4
#define  ft0001_REC_ID_LEN                 4
#define  ft0001_COMPANY_LEN                2
#define  ft0001_DIVISION_LEN               2
#define  ft0001_USER_ID_LEN                16
#define  ft0001_IP_ADDR_LEN                16
#define  ft0001_FILLER_LEN                 25
#define  ft0001_ID_LEN                     8
#define  ft0001_PASSWORD_LEN               8
#define  ft0001_GROUP_LEN                  8
#define  ft0001_ACCT_LEN                   8
#define  ft0001_FLAG_LEN                   1


/* MACS to Web Buffer */

#define  ft0001_REQ_ID_LEN                 4
#define  ft0001_REC_ID_LEN                 4
#define  ft0001_USER_ID_LEN                16
#define  ft0001_SUCCESS_FLAG_LEN           1
#define  ft0001_ERR_LEN                    80
#define  ft0001_SEND_FILLER_LEN            45
#define  ft0001_ID_LEN                     8
#define  ft0001_NAME_LEN                   20
#define  ft0001_SEC_USER_LEN               8
#define  ft0001_LOG_ACCT_LEN               8


typedef struct
{
    char request_id[ft0001_REQ_ID_LEN+1];
    char record_id[ft0001_REC_ID_LEN+1];
    char company[ft0001_COMPANY_LEN+1];
    char division[ft0001_DIVISION_LEN+1];
    char userid[ft0001_USER_ID_LEN+1];
    char ip_address[ft0001_IP_ADDR_LEN+1];
    char filler[ft0001_FILLER_LEN+1];
	char session_id[ft0001_ID_LEN+1];
	char user_passwd[ft0001_PASSWORD_LEN+1];
	char group[ft0001_GROUP_LEN+1];
	char group_passwd[ft0001_PASSWORD_LEN+1];
	char acct[ft0001_ACCT_LEN+1];
	char acct_passwd[ft0001_PASSWORD_LEN+1];
	char user_session_flag[ft0001_FLAG_LEN+1];
	char validate_flag[ft0001_FLAG_LEN+1];

} ft0001_st_send;


typedef struct
{
    char request_id[ft0001_REQ_ID_LEN+1];
    char record_id[ft0001_REC_ID_LEN+1];
    char userid[ft0001_USER_ID_LEN+1];
    char success_flag[ft0001_SUCCESS_FLAG_LEN+1];
    char err_message[ft0001_ERR_LEN+1];
    char filler[ft0001_SEND_FILLER_LEN+1];
	char user_id[ft0001_ID_LEN+1];
	char name[ft0001_NAME_LEN+1];
	char sec_user[ft0001_SEC_USER_LEN+1];
	char logon_acct[ft0001_LOG_ACCT_LEN+1];

} ft0001_st_recv;


