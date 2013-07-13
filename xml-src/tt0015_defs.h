/* tt0015_defs.h  Customization Parameters */


#define  tt0015_LAN_SEND_BUF_LEN        98 //89 
#define  tt0015_LAN_RECV_BUF_LEN        701 //700 //691  

/* Web to MACS Buffer  */

#define  tt0015_REQ_ID_LEN             4
#define  tt0015_REC_ID_LEN             4
#define  tt0015_COMPANY_LEN            2
#define  tt0015_DIVISION_LEN           2
#define  tt0015_USER_ID_LEN            16
#define  tt0015_IP_ADDR_LEN            16
#define  tt0015_FILLER_LEN			25

#define  tt0015_ITM_NO_LEN			 20
#define  tt0015_SHIP_TO_EDP_LEN      9
/* MACS to Web Buffer  */

#define  tt0015_REQ_ID_LEN             4
#define  tt0015_REC_ID_LEN             4
#define  tt0015_USER_ID_LEN            16
#define  tt0015_SUCCESS_FLAG_LEN        1
#define  tt0015_ERR_LEN			 80
#define  tt0015_SEND_FILLER_LEN			45


#define  tt0015_EDP_LEN                9
#define  tt0015_ITM_NO_LEN             20
#define  tt0015_DESC_LEN               50
#define  tt0015_CUSTOM_FLAG_LEN        1

#define  tt0015_CUSTOMIZE_COUNT        20

#define  tt0015_PROMPT_LEN             20
#define  tt0015_RESP_LEN               2
#define  tt0015_CUSTOMFRAME_LEN		1

typedef struct
{
    char request_id[tt0015_REQ_ID_LEN+1];
    char record_id[tt0015_REC_ID_LEN+1];
    char company[tt0015_COMPANY_LEN+1];
    char division[tt0015_DIVISION_LEN+1];
    char userid[tt0015_USER_ID_LEN+1];
    char ip_address[tt0015_IP_ADDR_LEN+1];
	char filler[tt0015_FILLER_LEN+1];
    char item_num[tt0015_ITM_NO_LEN+1];
	char ship_to_edp[tt0015_SHIP_TO_EDP_LEN+1];
} tt0015_st_send;

typedef struct
{
    char prompt[tt0015_PROMPT_LEN+1];
    char resp_len[tt0015_RESP_LEN+1];

} tt0015_customize_array;


typedef struct
{
    char request_id[tt0015_REQ_ID_LEN+1];
    char record_id[tt0015_REC_ID_LEN+1];
    char userid[tt0015_USER_ID_LEN+1];
    char success[tt0015_SUCCESS_FLAG_LEN+1];
	char err_message[tt0015_ERR_LEN+1];
	char filler[tt0015_SEND_FILLER_LEN+1];

    char item_edp[tt0015_EDP_LEN+1];
    char item_num[tt0015_ITM_NO_LEN+1];
    char item_desc[tt0015_DESC_LEN+1];
    char item_customization_flag[tt0015_CUSTOM_FLAG_LEN+1];
    tt0015_customize_array customization_prompt[tt0015_CUSTOMIZE_COUNT];
	char ship_to_edp[tt0015_SHIP_TO_EDP_LEN+1];
	char customframe[tt0015_CUSTOMFRAME_LEN+1];
} tt0015_st_recv;