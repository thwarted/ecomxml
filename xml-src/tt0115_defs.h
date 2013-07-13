/* tt0115_defs.h Update 3pty in cart */


#define  tt0115_LAN_SEND_BUF_LEN        80
#define  tt0115_LAN_RECV_BUF_LEN        370

/* Web to MACS Buffer  */

#define  tt0115_REQ_ID_LEN             4
#define  tt0115_REC_ID_LEN               4
#define  tt0115_COMPANY_LEN            2
#define  tt0115_DIVISION_LEN           2
#define  tt0115_USER_ID_LEN            16
#define  tt0115_IP_ADDR_LEN            16
#define  tt0115_FILLER_LEN                      25

#define  tt0115_TRANS_FLAG_LEN                  1
#define  tt0115_PTY_FLAG_LEN            1
#define  tt0115_PTY_EDP_LEN             9


/* MACS to Web Buffer   */

#define  tt0115_REQ_ID_LEN             4
#define  tt0115_REC_ID_LEN             4
#define  tt0115_USER_ID_LEN            16
#define  tt0115_SUCCESS_FLAG_LEN        1
#define  tt0115_ERR_LEN                  80
#define  tt0115_SEND_FILLER_LEN                 45

#define  tt0115_TRANS_FLAG_LEN                  1
#define  tt0115_PTY_FLAG_LEN            1
#define  tt0115_PY_EDP_LEN                      9
#define  tt0115_PY_CUSTNO_LEN           10
#define  tt0115_PY_FNAME_LEN            16
#define  tt0115_PY_MI_LEN                       1
#define  tt0115_PY_LNAME_LEN            16
#define  tt0115_PY_COMPANY_LEN          30
#define  tt0115_PY_STREET_LEN           30
#define  tt0115_PY_REF1_LEN                     30
#define  tt0115_PY_REF2_LEN                     30
#define  tt0115_PY_CITY_LEN                     30
#define  tt0115_PY_STATE_LEN            2
#define  tt0115_PY_ZIP_LEN                      14



typedef struct
{
    char request_id[tt0115_REQ_ID_LEN+1];
    char record_id[tt0115_REC_ID_LEN+1];
    char company[tt0115_COMPANY_LEN+1];
    char division[tt0115_DIVISION_LEN+1];
    char userid[tt0115_USER_ID_LEN+1];
    char ip_address[tt0115_IP_ADDR_LEN+1];
        char filler[tt0115_FILLER_LEN+1];
    char trans_flag[tt0115_TRANS_FLAG_LEN+1];
    char pty_flag[tt0115_PTY_FLAG_LEN+1];
    char pty_edp[tt0115_PTY_EDP_LEN+1];

} tt0115_st_send;


typedef struct
{
    char request_id[tt0115_REQ_ID_LEN+1];
    char record_id[tt0115_REC_ID_LEN+1];
    char userid[tt0115_USER_ID_LEN+1];
    char success[tt0115_SUCCESS_FLAG_LEN+1];
        char err_message[tt0115_ERR_LEN+1];
        char filler[tt0115_SEND_FILLER_LEN+1];

        char trans_flag[tt0115_TRANS_FLAG_LEN+1];
        char pty_flag[tt0115_PTY_FLAG_LEN+1];
        char py_edp[tt0115_PY_EDP_LEN+1];
        char py_custno[tt0115_PY_CUSTNO_LEN+1];
        char py_fname[tt0115_PY_FNAME_LEN+1];
        char py_mi[tt0115_PY_MI_LEN+1];
        char py_lname[tt0115_PY_LNAME_LEN+1];
        char py_company[tt0115_PY_COMPANY_LEN+1];
        char py_street[tt0115_PY_STREET_LEN+1];
        char py_ref1[tt0115_PY_REF1_LEN+1];
        char py_ref2[tt0115_PY_REF2_LEN+1];
        char py_city[tt0115_PY_CITY_LEN+1];
        char py_state[tt0115_PY_STATE_LEN+1];
        char py_zip[tt0115_PY_ZIP_LEN+1];

} tt0115_st_recv;
