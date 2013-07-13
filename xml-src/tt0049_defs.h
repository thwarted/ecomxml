/* tt0049_defs.h  Variable Kit Selection */

/* Web to MACS Buffer  */

#define  tt0049_LAN_SEND_BUF_LEN        89       //62    
#define  tt0049_LAN_RECV_BUF_LEN        20150    //20125 //20000  

#define  tt0049_REQ_ID_LEN             4
#define  tt0049_REC_ID_LEN             4
#define  tt0049_COMPANY_LEN            2
#define  tt0049_DIVISION_LEN           2
#define  tt0049_USER_ID_LEN            16
#define  tt0049_IP_ADDR_LEN            16
#define  tt0049_FILLER_LEN                      25
#define  tt0049_ITM_NO_LEN                       20

/* MACS to Web Buffer  */

#define  tt0049_REQ_ID_LEN             4
#define  tt0049_REC_ID_LEN             4
#define  tt0049_USER_ID_LEN            16
#define  tt0049_SUCCESS_FLAG_LEN        1
#define  tt0049_ERR_LEN                         80
#define  tt0049_SEND_FILLER_LEN                 45


#define  tt0049_ITM_NO_LEN                       20
#define  tt0049_EDP_LEN                          9
#define  tt0049_DESC_LEN                                 50
#define  tt0049_UPS_LEN                          640
#define  tt0049_IMG_OVRD_LEN                     8
#define  tt0049_COUNT_LEN                                2

#define  tt0049_R1_COMP_COUNT                    25

#define  tt0049_R1_ITM_NO_LEN                    20
#define  tt0049_R1_EDP_LEN                       9
#define  tt0049_R1_DESC_LEN                      50
#define  tt0049_R1_QTY_LEN                       5
#define  tt0049_R1_UPS_LEN                       640
#define  tt0049_R1_IMG_OVRD_LEN          8
#define  tt0049_R1_FLAG_LEN                     1

#define  tt0049_P1_COMP_COUNT                    25

#define  tt0049_P1_ITM_NO_LEN                    20
#define  tt0049_P1_QTY_LEN                       5


typedef struct
{
    char request_id[tt0049_REQ_ID_LEN+1];
    char record_id[tt0049_REC_ID_LEN+1];
    char company[tt0049_COMPANY_LEN+1];
    char division[tt0049_DIVISION_LEN+1];
    char userid[tt0049_USER_ID_LEN+1];
    char ip_address[tt0049_IP_ADDR_LEN+1];
        char filler[tt0049_FILLER_LEN +1];
        char kit_item_num[tt0049_ITM_NO_LEN+1];
        
} tt0049_st_send;


typedef struct
{
        char r1_item_num[tt0049_R1_ITM_NO_LEN+1];
        char r1_item_edp[tt0049_R1_EDP_LEN+1];
        char r1_item_desc[tt0049_R1_DESC_LEN+1];
        char r1_item_qty[tt0049_R1_QTY_LEN+1];
        char r1_upsl_X[tt0049_R1_UPS_LEN+1];
        char r1_image_ovrd[tt0049_R1_IMG_OVRD_LEN+1];
        char r1_flag[tt0049_R1_FLAG_LEN+1];

} tt0049_r1_comp_array;

typedef struct
{
        char p1_item_num[tt0049_P1_ITM_NO_LEN+1];
        char p1_item_qty[tt0049_P1_QTY_LEN+1];

} tt0049_p1_comp_array;



typedef struct
{
    char request_id[tt0049_REQ_ID_LEN+1];
    char record_id[tt0049_REC_ID_LEN+1];
    char userid[tt0049_USER_ID_LEN+1];
    char success[tt0049_SUCCESS_FLAG_LEN+1];
        char err_message[tt0049_ERR_LEN+1];
        char filler[tt0049_SEND_FILLER_LEN+1];

        char kit_item_num[tt0049_ITM_NO_LEN+1];
        char kit_item_edp[tt0049_EDP_LEN+1];
        char kit_item_desc[tt0049_DESC_LEN+1];
        char upsl_X[tt0049_UPS_LEN+1];
        char image_ovrd[tt0049_IMG_OVRD_LEN+1];
        char r1_count[tt0049_COUNT_LEN+1];
        char p1_count[tt0049_COUNT_LEN+1];
        tt0049_r1_comp_array r1_component[tt0049_R1_COMP_COUNT];
        tt0049_p1_comp_array p1_component[tt0049_P1_COMP_COUNT];

} tt0049_st_recv;

