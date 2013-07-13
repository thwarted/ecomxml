/* tt0087_defs.h  Tie Custom to Variable Kit */

/* Web to MACS Buffer  */

#define  tt0087_LAN_SEND_BUF_LEN       1302 //1301 //1298 //1291 //1264
#define  tt0087_LAN_RECV_BUF_LEN       185  //101 //199 //195 //150

#define  tt0087_REQ_ID_LEN             4
#define  tt0087_REC_ID_LEN                 4
#define  tt0087_COMPANY_LEN            2
#define  tt0087_DIVISION_LEN           2
#define  tt0087_USER_ID_LEN            16
#define  tt0087_IP_ADDR_LEN            16
#define  tt0087_FILLER_LEN                         25

#define  tt0087_ITM_NO_LEN             20

#define  tt0087_CUSTOM_ITEM_COUNT          20

#define  tt0087_CUSTOM_LEN                         60

#define  tt0087_SHIPTO_NO_LEN              2
#define  tt0087_PAGE_NO_LEN                        2
#define  tt0087_ITEM_LINE_NUMBER_LEN   3
#define  tt0087_COMP_SEQ_NUMBER_LEN        3
#define  tt0087_FLAG_LEN                           1
#define  tt0087_ITEM_TYPE_LEN              1
#define  tt0087_CUSTOMFRAME_LEN            1


/* MACS to Web Buffer  */

#define  tt0087_REQ_ID_LEN             4
#define  tt0087_REC_ID_LEN             4
#define  tt0087_USER_ID_LEN            16
#define  tt0087_SUCCESS_FLAG_LEN       1
#define  tt0087_ERR_LEN                            80
#define  tt0087_SEND_FILLER_LEN            45


#define  tt0087_ITM_NO_LEN                     20
#define  tt0087_EDP_LEN                9


typedef struct
{
    char custom_item[tt0087_CUSTOM_LEN+1];

} tt0087_custom_array;

typedef struct
{
    char request_id[tt0087_REQ_ID_LEN+1];
    char record_id[tt0087_REC_ID_LEN+1];
    char company[tt0087_COMPANY_LEN+1];
    char division[tt0087_DIVISION_LEN+1];
    char userid[tt0087_USER_ID_LEN+1];
    char ip_address[tt0087_IP_ADDR_LEN+1];
        char filler[tt0087_FILLER_LEN+1];
    char item_num[tt0087_ITM_NO_LEN+1];
    tt0087_custom_array custom_items[tt0087_CUSTOM_ITEM_COUNT];
        char shipto_num[tt0087_SHIPTO_NO_LEN+1];
        char page_no[tt0087_PAGE_NO_LEN+1];
        char item_line_number[tt0087_ITEM_LINE_NUMBER_LEN+1];
        char comp_seq_number[tt0087_COMP_SEQ_NUMBER_LEN+1];
        char flag[tt0087_FLAG_LEN+1];
        char item_type[tt0087_ITEM_TYPE_LEN+1];
        char customframe[tt0087_CUSTOMFRAME_LEN+1];

} tt0087_st_send;


typedef struct
{
    char request_id[tt0087_REQ_ID_LEN+1];
    char record_id[tt0087_REC_ID_LEN+1];
    char userid[tt0087_USER_ID_LEN+1];
    char success[tt0087_SUCCESS_FLAG_LEN+1];
        char err_message[tt0087_ERR_LEN+1];
        char filler[tt0087_SEND_FILLER_LEN+1];

    char item_num[tt0087_ITM_NO_LEN+1];
    char item_edp[tt0087_EDP_LEN+1];
        char page_no[tt0087_PAGE_NO_LEN+1];
        char item_line_number[tt0087_ITEM_LINE_NUMBER_LEN+1];
        char item_type[tt0087_ITEM_TYPE_LEN+1];

} tt0087_st_recv;

