/* tt0043_defs.h Add Order Comments */

/* Web to MACS Buffer  */

#define  tt0043_LAN_SEND_BUF_LEN        709 //682
#define  tt0043_LAN_RECV_BUF_LEN        826 //700  

#define  tt0043_REQ_ID_LEN             4
#define  tt0043_REC_ID_LEN             4
#define  tt0043_COMPANY_LEN            2
#define  tt0043_DIVISION_LEN           2
#define  tt0043_USER_ID_LEN            16
#define  tt0043_IP_ADDR_LEN                      16
#define  tt0043_FILLER_LEN                      25

#define  tt0043_ORD_COMM_COUNT           10

#define  tt0043_COMM_LEN                                 60
#define  tt0043_PKSLP_LEN                                1
#define  tt0043_LABEL_LEN                                1
#define  tt0043_GREET_LEN                                1
#define  tt0043_INVO_LEN                                 1

/* MACS to Web Buffer  */

#define  tt0043_REQ_ID_LEN             4
#define  tt0043_REC_ID_LEN             4
#define  tt0043_USER_ID_LEN            16
#define  tt0043_SUCCESS_FLAG_LEN        1
#define  tt0043_ERR_LEN                         80
#define  tt0043_SEND_FILLER_LEN                 45


#define  tt0043_ORD_COMMENT_COUNT                5

#define  tt0043_SEQ_LEN                          2
#define  tt0043_COMM_LEN                                 60
#define  tt0043_PKSLP_LEN                                1
#define  tt0043_LABEL_LEN                                1
#define  tt0043_GREET_LEN                                1
#define  tt0043_INVO_LEN                                 1

typedef struct
{
        char ord_comment[tt0043_COMM_LEN+1];
        char prt_pkslp[tt0043_PKSLP_LEN+1];
        char prt_label[tt0043_LABEL_LEN+1];
        char prt_greet[tt0043_GREET_LEN+1];
        char prt_invoice[tt0043_INVO_LEN+1];

} tt0043_send_ord_comment_array;


typedef struct
{
    char request_id[tt0043_REQ_ID_LEN+1];
    char record_id[tt0043_REC_ID_LEN+1];
    char company[tt0043_COMPANY_LEN+1];
    char division[tt0043_DIVISION_LEN+1];
    char userid[tt0043_USER_ID_LEN+1];
    char ip_address[tt0043_IP_ADDR_LEN+1];
        char filler[tt0043_FILLER_LEN+1];
        tt0043_send_ord_comment_array send_order_comment[tt0043_ORD_COMM_COUNT];
            
} tt0043_st_send;


typedef struct
{
        char seq_num[tt0043_SEQ_LEN+1];
        char ord_comment1[tt0043_COMM_LEN+1];
        char prt_pkslp1[tt0043_PKSLP_LEN+1];
        char prt_label1[tt0043_LABEL_LEN+1];
        char prt_greet1[tt0043_GREET_LEN+1];
        char prt_invoice1[tt0043_INVO_LEN+1];
        char ord_comment2[tt0043_COMM_LEN+1];
        char prt_pkslp2[tt0043_PKSLP_LEN+1];
        char prt_label2[tt0043_LABEL_LEN+1];
        char prt_greet2[tt0043_GREET_LEN+1];
        char prt_invoice2[tt0043_INVO_LEN+1];

} tt0043_recv_ord_comment_array;


typedef struct
{
    char request_id[tt0043_REQ_ID_LEN+1];
    char record_id[tt0043_REC_ID_LEN+1];
    char userid[tt0043_USER_ID_LEN+1];
    char success[tt0043_SUCCESS_FLAG_LEN+1];
        char err_message[tt0043_ERR_LEN+1];
        char filler[tt0043_SEND_FILLER_LEN+1];

        tt0043_recv_ord_comment_array recv_order_comment[tt0043_ORD_COMMENT_COUNT];

} tt0043_st_recv;

