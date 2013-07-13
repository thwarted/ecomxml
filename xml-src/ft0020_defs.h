/* ft0020_defs.h  List data */


#define  ft0020_LAN_SEND_BUF_LEN            99
#define  ft0020_LAN_RECV_BUF_LEN            28170

/* Web to MACS Buffer */

#define  ft0020_REQ_ID_LEN                 4
#define  ft0020_REC_ID_LEN                 4
#define  ft0020_COMPANY_LEN                2
#define  ft0020_DIVISION_LEN               2
#define  ft0020_USER_ID_LEN                16
#define  ft0020_IP_ADDR_LEN                16
#define  ft0020_FILLER_LEN                 25
#define  ft0020_TYPE_LEN                   2
#define  ft0020_PAGE_NO_LEN                4
#define  ft0020_PAGE_CNT_LEN               4
#define  ft0020_PARAM_LEN                  20


/* MACS to Web Buffer */

#define  ft0020_REQ_ID_LEN                 4
#define  ft0020_REC_ID_LEN                 4
#define  ft0020_USER_ID_LEN                16
#define  ft0020_SUCCESS_FLAG_LEN           1
#define  ft0020_ERR_LEN                    80
#define  ft0020_SEND_FILLER_LEN            45
#define  ft0020_LIST_CNT_LEN               3
#define  ft0020_PAGE_NO_LEN                4
#define  ft0020_LIST_SHT_LEN               20
#define  ft0020_TITLE_CODE_LEN             2
#define  ft0020_FILLER1_LEN                18
#define  ft0020_STATE_CODE_LEN             2
#define  ft0020_FILLER2_LEN                18
#define  ft0020_COUNTRY_CODE_LEN           4
#define  ft0020_FILLER3_LEN                16
#define  ft0020_CODE_ID_LEN                4
#define  ft0020_FILLER4_LEN                16
#define  ft0020_STAND_ALONE_LEN            1
#define  ft0020_DISC_CODE_LEN              1
#define  ft0020_DISC_TYPE_LEN              1
#define  ft0020_DISC_AMT_LEN               6
#define  ft0020_FILLER5_LEN                11
#define  ft0020_CODIV_LEN                 4
#define  ft0020_FILLER6_LEN                16
#define  ft0020_CUST_NO_LEN                10
#define  ft0020_FILLER7_LEN                10
#define  ft0020_PM_GROUP_LEN               2
#define  ft0020_PM_ID_LEN                  2
#define  ft0020_FILLER8_LEN                16
#define  ft0020_SM_LEN                     2
#define  ft0020_FILLER9_LEN                18
#define  ft0020_IN_PLAN_LEN                1
#define  ft0020_FILLER10_LEN               19 
#define  ft0020_KEY_LABEL_LEN              16
#define  ft0020_FILLER11_LEN               4 
#define  ft0020_FOC_CODE_LEN               1
#define  ft0020_FILLER12_LEN               19 
#define  ft0020_CP_SRC_LEN                 16
#define  ft0020_FILLER13_LEN               4 
#define  ft0020_CODIV_NO_LEN               4    
#define  ft0020_FILLER14_LEN               16 
#define  ft0020_DEPT_NO_LEN                4 
#define  ft0020_FILLER15_LEN               16 
#define  ft0020_KIT_HDR_LEN                20
#define  ft0020_PRINTER_LEN                8
#define  ft0020_FILLER16_LEN               12 
#define  ft0020_RA_NO_LEN                  9 
#define  ft0020_FILLER17_LEN               11 
#define  ft0020_VEND_NO_LEN                10
#define  ft0020_FILLER18_LEN               10 
#define  ft0020_WHSE_CODE_LEN              2 
#define  ft0020_FILLER19_LEN               18 
#define  ft0020_COMMENT_NO_LEN             4
#define  ft0020_FILLER20_LEN               16 
#define  ft0020_CHAIN_NO_LEN               4
#define  ft0020_FILLER21_LEN               16 
#define  ft0020_DISC_TABLE_LEN             4
#define  ft0020_FILLER22_LEN               16 
#define  ft0020_ITM_NO_LEN                 20
#define  ft0020_RES_LOC_LEN                4
#define  ft0020_FILLER23_LEN               16 
#define  ft0020_PROD_NO_LEN                20
#define  ft0020_CUSTOM_CODE_LEN            4
#define  ft0020_FILLER24_LEN               16 
#define  ft0020_LETTER_CODE_LEN            6
#define  ft0020_FILLER25_LEN               14 
#define  ft0020_STYLE_LEN                  10
#define  ft0020_FILLER26_LEN               10 
#define  ft0020_CT_LEN                     5
#define  ft0020_FILLER27_LEN               15 
#define  ft0020_LIST_LONG_LEN              60
#define  ft0020_TITLE_LEN                  12
#define  ft0020_FILLER28_LEN               48 
#define  ft0020_COUNTRY_LEN                20
#define  ft0020_FILLER29_LEN               40 
#define  ft0020_CODE_ABRV_LEN              20 
#define  ft0020_CODE_SPEC_LEN              40  
#define  ft0020_COMP_DESC_LEN              30
#define  ft0020_FILLER30_LEN               30 
#define  ft0020_PM_DESC_LEN                20
#define  ft0020_FILLER31_LEN               40 
#define  ft0020_SM_DESC_LEN                20 
#define  ft0020_FILLER32_LEN               40 
#define  ft0020_IN_DESC_LEN                30
#define  ft0020_FILLER33_LEN               30 
#define  ft0020_DATE_LEN                   8
#define  ft0020_FILLER34_LEN               52 
#define  ft0020_CODIV_NAME_LEN             30
#define  ft0020_DIV_NAME_LEN               30 
#define  ft0020_DEPT_NAME_LEN              30
#define  ft0020_FILLER35_LEN               30 
#define  ft0020_KIT_DESC_LEN               50
#define  ft0020_FILLER36_LEN               10 
#define  ft0020_VEND_NAME_LEN              30
#define  ft0020_FILLER37_LEN               30 
#define  ft0020_WHSE_DESC_LEN              60
#define  ft0020_COMMENT_DESC_LEN           20     
#define  ft0020_FILLER38_LEN               40 
#define  ft0020_CHAIN_DESC_LEN             30
#define  ft0020_FILLER39_LEN               30 
#define  ft0020_DISC_DESC_LEN              20
#define  ft0020_FILLER40_LEN               40 
#define  ft0020_OFFER_NO_LEN               8
#define  ft0020_EDP_NO_LEN                 9
#define  ft0020_FILLER41_LEN               43 
#define  ft0020_PROD_DESC_LEN              50
#define  ft0020_FILLER42_LEN               10 
#define  ft0020_LETTER_DESC_LEN            20 
#define  ft0020_FILLER43_LEN               40 
#define  ft0020_STYLE_DESC_LEN             30
#define  ft0020_FILLER44_LEN               30 
#define  ft0020_LIST_LOOP                  350 


typedef struct
{
    char request_id[ft0020_REQ_ID_LEN+1];
    char record_id[ft0020_REC_ID_LEN+1];
    char company[ft0020_COMPANY_LEN+1];
    char division[ft0020_DIVISION_LEN+1];
    char userid[ft0020_USER_ID_LEN+1];
    char ip_address[ft0020_IP_ADDR_LEN+1];
    char filler[ft0020_FILLER_LEN+1];
	char list_type[ft0020_TYPE_LEN+1];
	char page_no[ft0020_PAGE_NO_LEN+1];
	char page_cnt[ft0020_PAGE_CNT_LEN+1];
    char list_param[ft0020_PARAM_LEN+1];

} ft0020_st_send;

typedef struct
{
    char list_short[ft0020_LIST_SHT_LEN+1];
    char title_code[ft0020_TITLE_CODE_LEN+1];
    //char filler1[ft0020_FILLER1_LEN+1];
    char state_code[ft0020_STATE_CODE_LEN+1];
    //char filler2[ft0020_FILLER2_LEN+1];
    char country_code[ft0020_COUNTRY_CODE_LEN+1];
    //char filler3[ft0020_FILLER3_LEN+1];
    char code_id[ft0020_CODE_ID_LEN+1];
    //char filler4[ft0020_FILLER4_LEN+1];
    char stand_alone[ft0020_STAND_ALONE_LEN+1];
    char disc_code[ft0020_DISC_CODE_LEN+1];
    char disc_type[ft0020_DISC_TYPE_LEN+1];
    char disc_amt[ft0020_DISC_AMT_LEN+1];
    //char filler5[ft0020_FILLER5_LEN+1];
    char codiv[ft0020_CODIV_LEN+1];
    //char filler6[ft0020_FILLER6_LEN+1];
    char cust_no[ft0020_CUST_NO_LEN+1];
    //char filler7[ft0020_FILLER7_LEN+1];
    char pm_group[ft0020_PM_GROUP_LEN+1];
    char pm_id[ft0020_PM_ID_LEN+1];
    //char filler8[ft0020_FILLER8_LEN+1];
    char ship_method[ft0020_SM_LEN+1];
    //char filler9[ft0020_FILLER9_LEN+1];
    char in_plan[ft0020_IN_PLAN_LEN+1];
    //char filler10[ft0020_FILLER10_LEN+1];
    char key_label[ft0020_KEY_LABEL_LEN+1];
    //char filler11[ft0020_FILLER11_LEN+1];
    char foc_code[ft0020_FOC_CODE_LEN+1];
    //char filler12[ft0020_FILLER12_LEN+1];
    char cp_source[ft0020_CP_SRC_LEN+1];
    //char filler13[ft0020_FILLER13_LEN+1];
    char codiv_no[ft0020_CODIV_NO_LEN+1];
    //char filler14[ft0020_FILLER14_LEN+1];
    char dept_num[ft0020_DEPT_NO_LEN+1];
    //char filler15[ft0020_FILLER15_LEN+1];
    char kit_hdr[ft0020_KIT_HDR_LEN+1];
    char printer[ft0020_PRINTER_LEN+1];
    //char filler16[ft0020_FILLER16_LEN+1];
    char ra_no[ft0020_RA_NO_LEN+1];
    //char filler17[ft0020_FILLER17_LEN+1];
    char vend_no[ft0020_VEND_NO_LEN+1];
    //char filler18[ft0020_FILLER18_LEN+1];
    char whse_code[ft0020_WHSE_CODE_LEN+1];
    //char filler19[ft0020_FILLER19_LEN+1];
    char comment_no[ft0020_COMMENT_NO_LEN+1];
    //char filler20[ft0020_FILLER20_LEN+1];
    char chain_no[ft0020_CHAIN_NO_LEN+1];
    //char filler21[ft0020_FILLER21_LEN+1];
    char disc_table[ft0020_DISC_TABLE_LEN+1];
    //char filler22[ft0020_FILLER22_LEN+1];
    char item_no[ft0020_ITM_NO_LEN+1];
    char res_loc[ft0020_RES_LOC_LEN+1];
    //char filler23[ft0020_FILLER23_LEN+1];
    char prod_no[ft0020_PROD_NO_LEN+1];
    char custom_codes[ft0020_CUSTOM_CODE_LEN+1];
    //char filler24[ft0020_FILLER24_LEN+1];
    char letter_code[ft0020_LETTER_CODE_LEN+1];
    //char filler25[ft0020_FILLER25_LEN+1];
    char style_table[ft0020_STYLE_LEN+1];
    //char filler26[ft0020_FILLER26_LEN+1];
    char ct[ft0020_CT_LEN+1];
    //char filler27[ft0020_FILLER27_LEN+1];
    char list_long[ft0020_LIST_LONG_LEN+1];
    char title[ft0020_TITLE_LEN+1];
    //char filler28[ft0020_FILLER28_LEN+1];
    char country[ft0020_COUNTRY_LEN+1];
    //char filler29[ft0020_FILLER29_LEN+1];
    char code_abrv[ft0020_CODE_ABRV_LEN+1];
    char code_spec[ft0020_CODE_SPEC_LEN+1];
    char comp_desc[ft0020_COMP_DESC_LEN+1];
    //char filler30[ft0020_FILLER30_LEN+1];
    char pm_desc[ft0020_PM_DESC_LEN+1];
    //char filler31[ft0020_FILLER31_LEN+1];
    char sm_desc[ft0020_SM_DESC_LEN+1];
    //char filler32[ft0020_FILLER32_LEN+1];
    char in_desc[ft0020_IN_DESC_LEN+1];
    //char filler33[ft0020_FILLER33_LEN+1];
    char foc_date[ft0020_DATE_LEN+1];
    //char filler34[ft0020_FILLER34_LEN+1];
    char codiv_name[ft0020_CODIV_NAME_LEN+1];
    char div_name[ft0020_DIV_NAME_LEN+1];
    char dept_name[ft0020_DEPT_NAME_LEN+1];
    //char filler35[ft0020_FILLER35_LEN+1];
    char kit_desc[ft0020_KIT_DESC_LEN+1];
    //char filler36[ft0020_FILLER36_LEN+1];
    char vend_name[ft0020_VEND_NAME_LEN+1];
    //char filler37[ft0020_FILLER37_LEN+1];
    char whse_desc[ft0020_WHSE_DESC_LEN+1];
    char comment_desc[ft0020_COMMENT_DESC_LEN+1];
    //char filler38[ft0020_FILLER38_LEN+1];
    char chain_desc[ft0020_CHAIN_DESC_LEN+1];
    //char filler39[ft0020_FILLER39_LEN+1];
    char disc_desc[ft0020_DISC_DESC_LEN+1];
    //char filler40[ft0020_FILLER40_LEN+1];
    char offer_no[ft0020_OFFER_NO_LEN+1];
    char edp_no[ft0020_EDP_NO_LEN+1];
    //char filler41[ft0020_FILLER41_LEN+1];
    char prod_desc[ft0020_PROD_DESC_LEN+1];
    //char filler42[ft0020_FILLER42_LEN+1];
    char letter_desc[ft0020_LETTER_DESC_LEN+1];
    //char filler43[ft0020_FILLER43_LEN+1];
    char style_desc[ft0020_STYLE_DESC_LEN+1];
    //char filler44[ft0020_FILLER44_LEN+1];

} list_20;


typedef struct
{
    char request_id[ft0020_REQ_ID_LEN+1];
    char record_id[ft0020_REC_ID_LEN+1];
    char userid[ft0020_USER_ID_LEN+1];
    char success_flag[ft0020_SUCCESS_FLAG_LEN+1];
    char err_message[ft0020_ERR_LEN+1];
    char filler[ft0020_SEND_FILLER_LEN+1];
    char list_cnt[ft0020_LIST_CNT_LEN+1];
    char page_no[ft0020_PAGE_NO_LEN+1];
	list_20 list_detail[ft0020_LIST_LOOP];

} ft0020_st_recv;


