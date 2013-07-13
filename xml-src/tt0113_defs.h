/* tt0113_defs.h Get 3pty Existing */


#define  tt0113_LAN_SEND_BUF_LEN        79
#define  tt0113_LAN_RECV_BUF_LEN        8639

/* Web to MACS Buffer  */

#define  tt0113_REQ_ID_LEN             4
#define  tt0113_REC_ID_LEN               4
#define  tt0113_COMPANY_LEN            2
#define  tt0113_DIVISION_LEN           2
#define  tt0113_USER_ID_LEN            16
#define  tt0113_IP_ADDR_LEN            16
#define  tt0113_FILLER_LEN                      25

#define  tt0113_TRANS_FLAG_LEN                  1
#define  tt0113_BUYER_EDP_LEN           9


/* MACS to Web Buffer   */

#define  tt0113_REQ_ID_LEN             4
#define  tt0113_REC_ID_LEN             4
#define  tt0113_USER_ID_LEN            16
#define  tt0113_SUCCESS_FLAG_LEN        1
#define  tt0113_ERR_LEN                  80
#define  tt0113_SEND_FILLER_LEN                 45

#define  tt0113_TRANS_FLAG_LEN                  1
#define  tt0113_BUYER_EDP_LEN           9
#define  tt0113_BUYER_CUSTNO_LEN        10
#define  tt0113_BUYER_FNAME_LEN         16
#define  tt0113_BUYER_MI_LEN            1
#define  tt0113_BUYER_LNAME_LEN         16
#define  tt0113_BUYER_COMPANY_LEN       30
#define  tt0113_PY_FLAG_LEN                     1
#define  tt0113_RETURN_FILLER_LEN       5


#define  tt0113_PY_DATA_COUNT        20

#define  tt0113_PY_EDP_LEN                      9
#define  tt0113_PY_CUSTNO_LEN           10
#define  tt0113_PY_FNAME_LEN            16
#define  tt0113_PY_MI_LEN                       1
#define  tt0113_PY_LNAME_LEN            16
#define  tt0113_PY_COMPANY_LEN          30
#define  tt0113_PY_STREET_LEN           30
#define  tt0113_PY_REF1_LEN                     30
#define  tt0113_PY_REF2_LEN                     30
#define  tt0113_PY_CITY_LEN                     30
#define  tt0113_PY_STATE_LEN            2
#define  tt0113_PY_ZIP_LEN                      14

typedef struct
{
    char request_id[tt0113_REQ_ID_LEN+1];
    char record_id[tt0113_REC_ID_LEN+1];
    char company[tt0113_COMPANY_LEN+1];
    char division[tt0113_DIVISION_LEN+1];
    char userid[tt0113_USER_ID_LEN+1];
    char ip_address[tt0113_IP_ADDR_LEN+1];
        char filler[tt0113_FILLER_LEN+1];
    char trans_flag[tt0113_TRANS_FLAG_LEN+1];
    char buyer_edp[tt0113_BUYER_EDP_LEN+1];

} tt0113_st_send;


typedef struct
{
        char py_edp[tt0113_PY_EDP_LEN+1];
        char py_custno[tt0113_PY_CUSTNO_LEN+1];
        char py_fname[tt0113_PY_FNAME_LEN+1];
        char py_mi[tt0113_PY_MI_LEN+1];
        char py_lname[tt0113_PY_LNAME_LEN+1];
        char py_company[tt0113_PY_COMPANY_LEN+1];
        char py_street[tt0113_PY_STREET_LEN+1];
        char py_ref1[tt0113_PY_REF1_LEN+1];
        char py_ref2[tt0113_PY_REF2_LEN+1];
        char py_city[tt0113_PY_CITY_LEN+1];
        char py_state[tt0113_PY_STATE_LEN+1];
        char py_zip[tt0113_PY_ZIP_LEN+1];

} tt0113_data_array;


typedef struct
{
    char request_id[tt0113_REQ_ID_LEN+1];
    char record_id[tt0113_REC_ID_LEN+1];
    char userid[tt0113_USER_ID_LEN+1];
    char success[tt0113_SUCCESS_FLAG_LEN+1];
        char err_message[tt0113_ERR_LEN+1];
        char filler[tt0113_SEND_FILLER_LEN+1];

        char trans_flag[tt0113_TRANS_FLAG_LEN+1];
        char buyer_edp[tt0113_BUYER_EDP_LEN+1];
        char buyer_custno[tt0113_BUYER_CUSTNO_LEN+1];
        char buyer_fname[tt0113_BUYER_FNAME_LEN+1];
        char buyer_mi[tt0113_BUYER_MI_LEN+1];
        char buyer_lname[tt0113_BUYER_LNAME_LEN+1];
        char buyer_company[tt0113_BUYER_COMPANY_LEN+1];
        char py_flag[tt0113_PY_FLAG_LEN+1];

        tt0113_data_array data[tt0113_PY_DATA_COUNT];

} tt0113_st_recv;