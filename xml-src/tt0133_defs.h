/* tt0133_defs.h Ship Method Rates */


#define  tt0133_LAN_SEND_BUF_LEN       18451
#define  tt0133_LAN_RECV_BUF_LEN       1158

/* Web to MACS Buffer  */

#define  tt0133_REQ_ID_LEN             4
#define  tt0133_REC_ID_LEN             4
#define  tt0133_COMPANY_LEN            2
#define  tt0133_DIVISION_LEN           2
#define  tt0133_USER_ID_LEN            16
#define  tt0133_IP_ADDR_LEN            16
#define  tt0133_FILLER_LEN             25

#define  tt0133_OFFER_LEN				8
#define  tt0133_FLAG_LEN				1
#define  tt0133_FILLER1_LEN				25

#define  tt0133_CUSTNO_LEN				10
#define  tt0133_COUNTRY_LEN				4
#define  tt0133_CITY_LEN				20
#define  tt0133_ZIP_LEN					10
#define  tt0133_FILLER2_LEN				50
#define  tt0133_SHIPTO_LEN				2

#define  tt0133_ITEMNO_LEN				20
#define  tt0133_ITEM_QTY_LEN			4
#define  tt0133_PRICE_LEN				9

/* MACS to Web Buffer   */

#define  tt0133_REQ_ID_LEN             4
#define  tt0133_REC_ID_LEN             4
#define  tt0133_USER_ID_LEN            16
#define  tt0133_SUCCESS_FLAG_LEN       1
#define  tt0133_ERR_LEN                80
#define  tt0133_SEND_FILLER_LEN        45

#define  tt0133_RATES_ARRAY_LEN			9
#define  tt0133_SHIP_METHOD_LEN		    2
#define  tt0133_SHIP_DESC_LEN           20
#define  tt0133_PH_AMOUNT_LEN			12
#define  tt0133_COMMENT_LEN				78

#define  tt0133_SHIPTO_ARRAY_LEN		99
#define  tt0133_ITEM_ARRAY_LEN			250


typedef struct
{
    char cust_no[tt0133_CUSTNO_LEN+1];
	char country[tt0133_COUNTRY_LEN+1];
	char city[tt0133_CITY_LEN+1];
	char zip[tt0133_ZIP_LEN+1];
	char st_no[tt0133_SHIPTO_LEN+1];
	char filler[tt0133_FILLER2_LEN+1];

} tt0133_shipto;

typedef struct
{
    char item_no[tt0133_ITEMNO_LEN+1];
	char item_shipto[tt0133_SHIPTO_LEN+1];
	char item_qty[tt0133_ITEM_QTY_LEN+1];
	char unit_price[tt0133_PRICE_LEN+1];

} tt0133_items;

typedef struct
{
    char request_id[tt0133_REQ_ID_LEN+1];
    char record_id[tt0133_REC_ID_LEN+1];
    char company[tt0133_COMPANY_LEN+1];
    char division[tt0133_DIVISION_LEN+1];
    char userid[tt0133_USER_ID_LEN+1];
    char ip_address[tt0133_IP_ADDR_LEN+1];
    char filler[tt0133_FILLER_LEN+1];

	char offer[tt0133_OFFER_LEN+1];
	char cod_flag[tt0133_FLAG_LEN+1];
	char filler1[tt0133_FILLER1_LEN+1];

	char bu_cust_no[tt0133_CUSTNO_LEN+1];
	char bu_country[tt0133_COUNTRY_LEN+1];
	char bu_city[tt0133_CITY_LEN+1];
	char bu_zip[tt0133_ZIP_LEN+1];
	char bu_filler[tt0133_FILLER2_LEN+1];

	tt0133_shipto shipto[tt0133_SHIPTO_ARRAY_LEN];
	tt0133_items   items[tt0133_ITEM_ARRAY_LEN];
 
} tt0133_st_send;


typedef struct
{
    char ship_method[tt0133_SHIP_METHOD_LEN+1];
    char ship_method_desc[tt0133_SHIP_DESC_LEN+1];
    char ph_amount[tt0133_PH_AMOUNT_LEN+1];
    char comment[tt0133_COMMENT_LEN+1];
} tt0133_ship_array;

typedef struct
{
    char request_id[tt0133_REQ_ID_LEN+1];
    char record_id[tt0133_REC_ID_LEN+1];
    char userid[tt0133_USER_ID_LEN+1];
    char success[tt0133_SUCCESS_FLAG_LEN+1];
    char err_message[tt0133_ERR_LEN+1];
    char filler[tt0133_SEND_FILLER_LEN+1];

    tt0133_ship_array rates[tt0133_RATES_ARRAY_LEN];

} tt0133_st_recv;


