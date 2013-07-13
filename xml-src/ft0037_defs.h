/* ft0037_defs.h  Control Block */


#define  ft0037_LAN_SEND_BUF_LEN            71
#define  ft0037_LAN_RECV_BUF_LEN            24160

/* Web to MACS Buffer */

#define  ft0037_REQ_ID_LEN                 4
#define  ft0037_REC_ID_LEN                 4
#define  ft0037_COMPANY_LEN                2
#define  ft0037_DIVISION_LEN               2
#define  ft0037_USER_ID_LEN                16
#define  ft0037_IP_ADDR_LEN                16
#define  ft0037_FILLER_LEN                 25
#define  ft0037_TYPE_LEN                   1
#define  ft0037_MODE_LEN                   1


/* MACS to Web Buffer */

#define  ft0037_REQ_ID_LEN                 4
#define  ft0037_REC_ID_LEN                 4
#define  ft0037_USER_ID_LEN                16
#define  ft0037_SUCCESS_FLAG_LEN           1
#define  ft0037_ERR_LEN                    80
#define  ft0037_SEND_FILLER_LEN            45
#define  ft0037_CTL_ID_LEN                 20
#define  ft0037_CTL_LEN_LEN                4
#define  ft0037_CTL_LOOP                   1000



typedef struct
{
    char request_id[ft0037_REQ_ID_LEN+1];
    char record_id[ft0037_REC_ID_LEN+1];
    char company[ft0037_COMPANY_LEN+1];
    char division[ft0037_DIVISION_LEN+1];
    char userid[ft0037_USER_ID_LEN+1];
    char ip_address[ft0037_IP_ADDR_LEN+1];
    char filler[ft0037_FILLER_LEN+1];
	char type[ft0037_TYPE_LEN+1];
	char mode[ft0037_MODE_LEN+1];

} ft0037_st_send;

typedef struct
{
    char ctl_id[ft0037_CTL_ID_LEN+1];
    char ctl_len[ft0037_CTL_LEN_LEN+1];

} control_37;


typedef struct
{
    char request_id[ft0037_REQ_ID_LEN+1];
    char record_id[ft0037_REC_ID_LEN+1];
    char userid[ft0037_USER_ID_LEN+1];
    char success_flag[ft0037_SUCCESS_FLAG_LEN+1];
    char err_message[ft0037_ERR_LEN+1];
    char filler[ft0037_SEND_FILLER_LEN+1];
	control_37 control_desc[ft0037_CTL_LOOP];

} ft0037_st_recv;


