/* tt0117_defs.h Get 3pty in cart */


#define  tt0117_LAN_SEND_BUF_LEN        71
#define  tt0117_LAN_RECV_BUF_LEN        374

/* Web to MACS Buffer  */

#define  tt0117_REQ_ID_LEN              4
#define  tt0117_REC_ID_LEN                  4
#define  tt0117_COMPANY_LEN             2
#define  tt0117_DIVISION_LEN            2
#define  tt0117_USER_ID_LEN             16
#define  tt0117_IP_ADDR_LEN             16
#define  tt0117_FILLER_LEN                          25

#define  tt0117_TRANS_FLAG_LEN                  1
#define  tt0117_PTY_FLAG_LEN                1


/* MACS to Web Buffer   */

#define  tt0117_REQ_ID_LEN              4
#define  tt0117_REC_ID_LEN              4
#define  tt0117_USER_ID_LEN             16
#define  tt0117_SUCCESS_FLAG_LEN        1
#define  tt0117_ERR_LEN                         80
#define  tt0117_SEND_FILLER_LEN                 45

#define  tt0117_TRANS_FLAG_LEN                  1
#define  tt0117_PTY_FLAG_LEN                1
#define  tt0117_PY_EDP_LEN                          9
#define  tt0117_PY_CUSTNO_LEN               10
#define  tt0117_PY_FNAME_LEN                16
#define  tt0117_PY_MI_LEN                           1
#define  tt0117_PY_LNAME_LEN                16
#define  tt0117_PY_COMPANY_LEN              30
#define  tt0117_PY_STREET_LEN               30
#define  tt0117_PY_REF1_LEN                         30
#define  tt0117_PY_REF2_LEN                         30
#define  tt0117_PY_CITY_LEN                         30
#define  tt0117_PY_STATE_LEN                2
#define  tt0117_PY_ZIP_LEN                          14
#define  tt0117_PY_COUNTRY_CODE_LEN     4



typedef struct
{
    char request_id[tt0117_REQ_ID_LEN+1];
    char record_id[tt0117_REC_ID_LEN+1];
    char company[tt0117_COMPANY_LEN+1];
    char division[tt0117_DIVISION_LEN+1];
    char userid[tt0117_USER_ID_LEN+1];
    char ip_address[tt0117_IP_ADDR_LEN+1];
        char filler[tt0117_FILLER_LEN+1];
    char trans_flag[tt0117_TRANS_FLAG_LEN+1];
    char pty_flag[tt0117_PTY_FLAG_LEN+1];
} tt0117_st_send;


typedef struct
{
    char request_id[tt0117_REQ_ID_LEN+1];
    char record_id[tt0117_REC_ID_LEN+1];
    char userid[tt0117_USER_ID_LEN+1];
    char success[tt0117_SUCCESS_FLAG_LEN+1];
        char err_message[tt0117_ERR_LEN+1];
        char filler[tt0117_SEND_FILLER_LEN+1];

        char trans_flag[tt0117_TRANS_FLAG_LEN+1];
        char pty_flag[tt0117_PTY_FLAG_LEN+1];
        char py_edp[tt0117_PY_EDP_LEN+1];
        char py_custno[tt0117_PY_CUSTNO_LEN+1];
        char py_fname[tt0117_PY_FNAME_LEN+1];
        char py_mi[tt0117_PY_MI_LEN+1];
        char py_lname[tt0117_PY_LNAME_LEN+1];
        char py_company[tt0117_PY_COMPANY_LEN+1];
        char py_street[tt0117_PY_STREET_LEN+1];
        char py_ref1[tt0117_PY_REF1_LEN+1];
        char py_ref2[tt0117_PY_REF2_LEN+1];
        char py_city[tt0117_PY_CITY_LEN+1];
        char py_state[tt0117_PY_STATE_LEN+1];
        char py_zip[tt0117_PY_ZIP_LEN+1];
        char py_country_code[tt0117_PY_COUNTRY_CODE_LEN+1];

} tt0117_st_recv;