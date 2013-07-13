/* wo01.c */
/* modified for phase 2 - JL */
#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <malloc.h>


#include "sgsock.h"
#include "funcs.h"
#include "externs.h"
#include "http_protocol.h"

#define   SUCCESS     1
#define   FAILURE     0


int             wo01_CatSendStr10(char *, wo01_rec10 *);
int             wo01_CatSendStr12(char *, wo01_rec12 *);
int             wo01_CatSendStr13(char *, wo01_rec13 *);
int             wo01_CatSendStr15(char *, wo01_rec15 *);
int             wo01_CatSendStr16(char *, wo01_rec16 *);
int             wo01_CatSendStr17(char *, wo01_rec17 *);
int             wo01_CatSendStr20(char *, wo01_rec20 *);
int             wo01_CatSendStr21(char *, wo01_rec21 *);
int             wo01_CatSendStr30(char *, wo01_rec30 *);
int             wo01_CatSendStr31(char *, wo01_rec31 *);
int             wo01_CatSendStr33(char *, wo01_rec33 *);
int             wo01_CatSendStr40(char *, wo01_rec40 *, wo01_tmp_vars *);
int             wo01_CatSendStr41(char *, wo01_rec41 *);
int             wo01_CatSendStr42(char *, wo01_rec42 *);
int             wo01_CatSendStrblank(char *);

int             wo01_ParceRecvStr(wo01_recv *, char *, request_rec *, struct global_struct *);
int             wo01_lt_process(request_rec *, struct global_struct *, char *);


int wo01_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wo01_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wo01_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wo01_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wo01_send_rec = (wo01_send_records *)malloc(sizeof(wo01_send_records));

        if (gbp->wo01_send_rec == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate gbp->wo01_send_rec\n");
                fflush(stdout);
                return(-1);
        }

        gbp->max_record = 0;

        gbp->R16_count = 0;
        gbp->R21_count = 0;
        gbp->R30_count = 0;
        gbp->R31_count = 0;
        gbp->R33_count = 0;
        gbp->R40_count = 0;
        gbp->R41_count = 0;
        gbp->R42_count = 0;


        gbp->sendbufcat = malloc(wo01_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate sendbufcat\n");
                fflush(stdout);
                return(-1);
        }

        gbp->recvbufcat = malloc(wo01_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate recvbufcat\n");
                fflush(stdout);
                return(-1);

        }

    memset(gbp->sendbufcat, '\0', wo01_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wo01_LAN_RECV_BUF_LEN + 1);

        memset(gbp->request_id   ,'\0', REQ_ID_LEN + 1);
        memset(gbp->company      ,'\0', COMPANY_LEN + 1);
        memset(gbp->division     ,'\0', DIVISION_LEN + 1);  

/* fill the header */
        sprintf(gbp->confirmbuf, "     ");
        strcpy(gbp->request_id, "XML");
        strcpy(gbp->tran_id, "CONF");
        strcpy(gbp->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY"      , gbp->company,gbp,stdout_buffer);
        get_tag_data("DIVISION"     , gbp->division,gbp,stdout_buffer);
        get_tag_data("UID"                      , gbp->uid,gbp,stdout_buffer);

/*  record 10  */
        
        memset(&gbp->wo01_send_rec->rec10              ,'\0', sizeof(gbp->wo01_send_rec->rec10));
//      memset(gbp->wo01_send_rec->rec10.filler1       ,' ', R10_FILLER1_LEN+1);
        memset(gbp->wo01_send_rec->rec10.filler2       ,' ', R10_FILLER2_LEN+1);

/* record 12  */

        memset(&gbp->wo01_send_rec->rec12              ,'\0', sizeof(gbp->wo01_send_rec->rec12));
        memset(gbp->wo01_send_rec->rec12.filler1       ,' ', R12_FILLER1_LEN+1);
        memset(gbp->wo01_send_rec->rec12.filler2       ,' ', R12_FILLER2_LEN+1);

/* record 13  */

        memset(&gbp->wo01_send_rec->rec13              ,'\0', sizeof(gbp->wo01_send_rec->rec13));
        memset(gbp->wo01_send_rec->rec13.filler1       ,' ', R13_FILLER1_LEN+1);
        memset(gbp->wo01_send_rec->rec13.filler2       ,' ', R13_FILLER2_LEN+1);

/*  record 15  */

        memset(&gbp->wo01_send_rec->rec15              ,'\0', sizeof(gbp->wo01_send_rec->rec15));
        memset(gbp->wo01_send_rec->rec15.filler1       ,' ', R15_FILLER1_LEN+1);
        memset(gbp->wo01_send_rec->rec15.filler2       ,' ', R15_FILLER2_LEN+1);
        memset(gbp->wo01_send_rec->rec15.filler3       ,' ', R15_FILLER3_LEN+1);
        memset(gbp->wo01_send_rec->rec15.filler4       ,' ', R15_FILLER4_LEN+1);

/*  record 16  */

        
        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec16[gbp->i]              ,'\0', sizeof(wo01_rec16));
                memset(gbp->wo01_send_rec->rec16[gbp->i].filler1       ,' ', R16_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec16[gbp->i].filler2       ,' ', R16_FILLER2_LEN+1);
                memset(gbp->wo01_send_rec->rec16[gbp->i].filler3       ,' ', R16_FILLER3_LEN+1);
                memset(gbp->wo01_send_rec->rec16[gbp->i].filler4       ,' ', R16_FILLER4_LEN+1);
        }

/*  record 17  */

        memset(&gbp->wo01_send_rec->rec17              ,'\0', sizeof(gbp->wo01_send_rec->rec17));
        memset(gbp->wo01_send_rec->rec17.filler1       ,' ', R17_FILLER1_LEN+1);
        memset(gbp->wo01_send_rec->rec17.filler2       ,' ', R17_FILLER2_LEN+1);

/*  record 20  */

        memset(&gbp->wo01_send_rec->rec20              ,'\0', sizeof(gbp->wo01_send_rec->rec20));
        memset(gbp->wo01_send_rec->rec20.filler1       ,' ', R20_FILLER1_LEN+1);
        memset(gbp->wo01_send_rec->rec20.filler2       ,' ', R20_FILLER2_LEN+1);
        
/*  record 21  */

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec21[gbp->i]              ,'\0', sizeof(wo01_rec21));
                memset(gbp->wo01_send_rec->rec21[gbp->i].filler1       ,' ', R21_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec21[gbp->i].filler2       ,' ', R21_FILLER2_LEN+1);
        }

/*  record 30  */

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec30[gbp->i]              ,'\0', sizeof(wo01_rec30));
                memset(gbp->wo01_send_rec->rec30[gbp->i].filler1       ,' ', R30_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec30[gbp->i].filler2       ,' ', R30_FILLER2_LEN+1);
        }

/*  record 31  */
        
        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec31[gbp->i]              ,'\0', sizeof(wo01_rec31));
                memset(gbp->wo01_send_rec->rec31[gbp->i].filler1       ,' ', R31_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec31[gbp->i].filler2       ,' ', R31_FILLER2_LEN+1);
        }
        
/*  record 33  */

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec33[gbp->i]              ,'\0', sizeof(wo01_rec33));
                memset(gbp->wo01_send_rec->rec33[gbp->i].filler1       ,' ', R33_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec33[gbp->i].filler2       ,' ', R33_FILLER2_LEN+1);
        }

/*  record 40 */

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec40[gbp->i]              ,'\0', sizeof(wo01_rec40));
                memset(gbp->wo01_send_rec->rec40[gbp->i].filler1       ,' ', R40_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec40[gbp->i].filler2       ,' ', R40_FILLER2_LEN+1);
                memset(gbp->wo01_send_rec->rec40[gbp->i].filler3       ,' ', R40_FILLER3_LEN+1);
        }

/*  record 41 */

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec41[gbp->i]              ,'\0', sizeof(wo01_rec41));
                memset(gbp->wo01_send_rec->rec41[gbp->i].filler1       ,' ', R41_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec41[gbp->i].filler2       ,' ', R41_FILLER2_LEN+1);
        }

/*  record 42  */

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(&gbp->wo01_send_rec->rec42[gbp->i]              ,'\0', sizeof(wo01_rec42));
                memset(gbp->wo01_send_rec->rec42[gbp->i].filler1       ,' ', R42_FILLER1_LEN+1);
                memset(gbp->wo01_send_rec->rec42[gbp->i].filler2       ,' ', R42_FILLER2_LEN+1);
        }

/* Get the record name from the seq tag
   This function also finds maximum number of records present in one order  */
        
        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                memset(gbp->tagname, '\0', 5);
                if((get_tag_from_seq(gbp->i+1, gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, stdout_buffer,gbp)) == -1)
                {
                        gbp->max_record = gbp->i;
                        break;
                }
                gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].seq_count = gbp->i;
        }

/* start filling the records */

        memset(gbp->wo01_send_rec->tmaxrec, '\0', NUMBER_OF_RECORDS*321);

        for(gbp->i = 0; gbp->i < NUMBER_OF_RECORDS; gbp->i++)
        {
                if(gbp->i >= gbp->max_record)
                {
                        if(wo01_CatSendStrblank(gbp->wo01_send_rec->tmaxrec[gbp->i]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R10") == 0) 
                {
                        strcpy(gbp->wo01_send_rec->rec10.rec_type, "10");
                        get_tag_data("R10_ORDER_NUM"       , gbp->wo01_send_rec->rec10.order_num,gbp,stdout_buffer);
                        get_tag_data("R10_SOURCE"       , gbp->wo01_send_rec->rec10.source,gbp,stdout_buffer);
                        get_tag_data("R10_MAIL_DATE"    , gbp->wo01_send_rec->rec10.mail_date,gbp,stdout_buffer);
                        get_tag_data("R10_PAY_METHOD"   , gbp->wo01_send_rec->rec10.pay_method,gbp,stdout_buffer);
                        get_tag_data("R10_FNAME"        , gbp->wo01_send_rec->rec10.first_name,gbp,stdout_buffer);
                        get_tag_data("R10_MINITIAL"     , gbp->wo01_send_rec->rec10.middle_init,gbp,stdout_buffer);
                        get_tag_data("R10_LNAME"        , gbp->wo01_send_rec->rec10.last_name,gbp,stdout_buffer);
                        get_tag_data("R10_TITLE"        , gbp->wo01_send_rec->rec10.title,gbp,stdout_buffer);
                        get_tag_data("R10_COMPANY_NAME" , gbp->wo01_send_rec->rec10.company_name,gbp,stdout_buffer);
                        get_tag_data("R10_ADDRESS_REF1" , gbp->wo01_send_rec->rec10.address_ref_1,gbp,stdout_buffer);
                        get_tag_data("R10_ADDRESS_REF2" , gbp->wo01_send_rec->rec10.address_ref_2,gbp,stdout_buffer);
                        get_tag_data("R10_STREET"       , gbp->wo01_send_rec->rec10.street,gbp,stdout_buffer);
                        get_tag_data("R10_CITY"         , gbp->wo01_send_rec->rec10.city,gbp,stdout_buffer);
                        get_tag_data("R10_STATE"        , gbp->wo01_send_rec->rec10.state,gbp,stdout_buffer);
                        get_tag_data("R10_ZIP_CODE"     , gbp->wo01_send_rec->rec10.zip_code,gbp,stdout_buffer);
                        get_tag_data("R10_COUNTRY_CODE" , gbp->wo01_send_rec->rec10.country_code,gbp,stdout_buffer);
                        get_tag_data("R10_DAY_PHONE"    , gbp->wo01_send_rec->rec10.day_phone,gbp,stdout_buffer);
                        get_tag_data("R10_NGT_PHONE"    , gbp->wo01_send_rec->rec10.night_phone,gbp,stdout_buffer);
                        get_tag_data("R10_OLD_CUST_NUM" , gbp->wo01_send_rec->rec10.old_custno,gbp,stdout_buffer);
                        get_tag_data("R10_CUST_NUM"     , gbp->wo01_send_rec->rec10.cust_no,gbp,stdout_buffer);
                        get_tag_data("R10_CUST_CHKDIG"  , gbp->wo01_send_rec->rec10.cust_chkdig,gbp,stdout_buffer);
                        get_tag_data("R10_CUST_EDP"     , gbp->wo01_send_rec->rec10.cust_edp,gbp,stdout_buffer);
                        get_tag_data("R10_NO_PROMO"     , gbp->wo01_send_rec->rec10.no_promo,gbp,stdout_buffer);
                        get_tag_data("R10_NO_RENT"      , gbp->wo01_send_rec->rec10.no_rent,gbp,stdout_buffer);

                        if(wo01_CatSendStr10(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec10) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R12") == 0) 
                {
                        strcpy(gbp->wo01_send_rec->rec12.rec_type, "12");
                        get_tag_data("R12_FNAME"        , gbp->wo01_send_rec->rec12.first_name,gbp,stdout_buffer);
                        get_tag_data("R12_MINITIAL"     , gbp->wo01_send_rec->rec12.middle_init,gbp,stdout_buffer);
                        get_tag_data("R12_LNAME"        , gbp->wo01_send_rec->rec12.last_name,gbp,stdout_buffer);
                        get_tag_data("R12_TITLE"        , gbp->wo01_send_rec->rec12.title,gbp,stdout_buffer);
                        get_tag_data("R12_COMPANY_NAME" , gbp->wo01_send_rec->rec12.company_name,gbp,stdout_buffer);
                        get_tag_data("R12_ADDRESS_REF1" , gbp->wo01_send_rec->rec12.address_ref_1,gbp,stdout_buffer);
                        get_tag_data("R12_ADDRESS_REF2" , gbp->wo01_send_rec->rec12.address_ref_2,gbp,stdout_buffer);
                        get_tag_data("R12_STREET"       , gbp->wo01_send_rec->rec12.street,gbp,stdout_buffer);
                        get_tag_data("R12_CITY"         , gbp->wo01_send_rec->rec12.city,gbp,stdout_buffer);
                        get_tag_data("R12_STATE"        , gbp->wo01_send_rec->rec12.state,gbp,stdout_buffer);
                        get_tag_data("R12_ZIP_CODE"     , gbp->wo01_send_rec->rec12.zip_code,gbp,stdout_buffer);
                        get_tag_data("R12_PHONE"        , gbp->wo01_send_rec->rec12.phone,gbp,stdout_buffer);
                        get_tag_data("R12_COUNTRY_CODE" , gbp->wo01_send_rec->rec12.country_code,gbp,stdout_buffer);
                        get_tag_data("R12_OLD_CUST_NUM" , gbp->wo01_send_rec->rec12.old_custno,gbp,stdout_buffer);
                        get_tag_data("R12_CUST_NUM"     , gbp->wo01_send_rec->rec12.cust_no,gbp,stdout_buffer);
                        get_tag_data("R12_CUST_CHKDIG"  , gbp->wo01_send_rec->rec12.cust_chkdig,gbp,stdout_buffer);

                        if(wo01_CatSendStr12(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec12) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R13") == 0) 
                {
                        strcpy(gbp->wo01_send_rec->rec13.rec_type, "13");
                        get_tag_data("R13_EMAIL_ADDR"   , gbp->wo01_send_rec->rec13.email_addr,gbp,stdout_buffer);
                        get_tag_data("R13_NET_PASSWORD" , gbp->wo01_send_rec->rec13.net_password,gbp,stdout_buffer);
                        get_tag_data("R13_RECAP_FLAG"   , gbp->wo01_send_rec->rec13.recap_flag,gbp,stdout_buffer);

                        if(gbp->wo01_send_rec->rec13.recap_flag[0] == 'R')      strcpy(gbp->tran_id, "RECA");

                        strcpy(gbp->wo01_send_rec->rec13.ip_address, r->connection->remote_ip);

                        get_tag_data("R13_USER_ID"      , gbp->wo01_send_rec->rec13.user_id,gbp,stdout_buffer);
                        get_tag_data("R13_DEMOGRAPH"    , gbp->wo01_send_rec->rec13.demograph,gbp,stdout_buffer);
                
                        if(wo01_CatSendStr13(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec13) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R15") == 0) 
                {
                        strcpy(gbp->wo01_send_rec->rec15.rec_type, "15");
                        get_tag_data("R15_REMIT_AMT"    , gbp->wo01_send_rec->rec15.remit_amt,gbp,stdout_buffer);
                        get_tag_data("R15_FUTURE_DATE"  , gbp->wo01_send_rec->rec15.future_date,gbp,stdout_buffer);
                        get_tag_data("R15_PURCHASE_ORD" , gbp->wo01_send_rec->rec15.po_number,gbp,stdout_buffer);
                        get_tag_data("R15_ENTERED_DATE" , gbp->wo01_send_rec->rec15.entered_date,gbp,stdout_buffer);
                        get_tag_data("R15_ENTERED_TIME" , gbp->wo01_send_rec->rec15.entered_time,gbp,stdout_buffer);
                        get_tag_data("R15_ENTERED_BY"   , gbp->wo01_send_rec->rec15.entered_by,gbp,stdout_buffer);
                        get_tag_data("R15_HOLD_COMPLETE", gbp->wo01_send_rec->rec15.hold_order,gbp,stdout_buffer);
                        get_tag_data("R15_EXPEDITE_ORD" , gbp->wo01_send_rec->rec15.expedite,gbp,stdout_buffer);
                        get_tag_data("R15_NO_BACKORDER" , gbp->wo01_send_rec->rec15.no_backorder,gbp,stdout_buffer);
                        get_tag_data("R15_POSTPONE_ORD" , gbp->wo01_send_rec->rec15.postpone_order,gbp,stdout_buffer);
                        get_tag_data("R15_NO_INSURANCE" , gbp->wo01_send_rec->rec15.no_insurance,gbp,stdout_buffer);
                        get_tag_data("R15_CCUF_FRONT"   , gbp->wo01_send_rec->rec15.cc_up_front,gbp,stdout_buffer);
                        get_tag_data("R15_OVERPAYMENT"  , gbp->wo01_send_rec->rec15.overpayment,gbp,stdout_buffer);
                        get_tag_data("R15_STANDING_ORD" , gbp->wo01_send_rec->rec15.standing_order,gbp,stdout_buffer);
                        get_tag_data("R15_GENDER"       , gbp->wo01_send_rec->rec15.gender,gbp,stdout_buffer);
                        get_tag_data("R15_OPT_CODE"     , gbp->wo01_send_rec->rec15.opt_code,gbp,stdout_buffer);
                        get_tag_data("R15_SHIP_METHOD"  , gbp->wo01_send_rec->rec15.ship_method,gbp,stdout_buffer);
                        get_tag_data("R15_DEMOGRAPHICS" , gbp->wo01_send_rec->rec15.demographic,gbp,stdout_buffer);
                        get_tag_data("R15_SALES_TERR"   , gbp->wo01_send_rec->rec15.sales_terr,gbp,stdout_buffer);
                        get_tag_data("R15_SALES_REP"    , gbp->wo01_send_rec->rec15.sales_rep,gbp,stdout_buffer);
                        get_tag_data("R15_FAX_PHONE"    , gbp->wo01_send_rec->rec15.fax_phone,gbp,stdout_buffer);
                        get_tag_data("R15_ORDER_XREF"   , gbp->wo01_send_rec->rec15.order_xref,gbp,stdout_buffer);
                        get_tag_data("R15_CUST_TYPE"    , gbp->wo01_send_rec->rec15.cust_type,gbp,stdout_buffer);
                        get_tag_data("R15_ERROR_FLAG"   , gbp->wo01_send_rec->rec15.error_flag,gbp,stdout_buffer);
                        get_tag_data("R15_INSTALLMENT"  , gbp->wo01_send_rec->rec15.installment,gbp,stdout_buffer);
                        get_tag_data("R15_SHIP_DATE"    , gbp->wo01_send_rec->rec15.ship_date,gbp,stdout_buffer);
                        get_tag_data("R15_USER_HOLD_CODE", gbp->wo01_send_rec->rec15.user_hold_code,gbp,stdout_buffer);
                        get_tag_data("R15_BANK_ACCT"    , gbp->wo01_send_rec->rec15.bank_acct,gbp,stdout_buffer);
                        get_tag_data("R15_RUT_NUM"      , gbp->wo01_send_rec->rec15.rut_num,gbp,stdout_buffer);
                        get_tag_data("R15_CONTINUITY_FLAG", gbp->wo01_send_rec->rec15.continuity_flag,gbp,stdout_buffer);
                        get_tag_data("R15_BYPASS_ADV"   , gbp->wo01_send_rec->rec15.bypass_adv,gbp,stdout_buffer);
                        get_tag_data("R15_DISCOUNT_CODE1", gbp->wo01_send_rec->rec15.disc_code1,gbp,stdout_buffer);
                        get_tag_data("R15_DISCOUNT_CODE2", gbp->wo01_send_rec->rec15.disc_code2,gbp,stdout_buffer);
                        get_tag_data("R15_BIRTHDAY"     , gbp->wo01_send_rec->rec15.birthday,gbp,stdout_buffer);

                
                        if(wo01_CatSendStr15(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec15) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R16") == 0) 
                {
                        gbp->R16_count++;
                        
                        strcpy(gbp->wo01_send_rec->rec16[gbp->R16_count - 1].rec_type, "16");

                        set_tag_name(gbp->tagname, "R16_%d_ORD_COMMENT1", gbp->R16_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec16[gbp->R16_count - 1].ord_comment_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R16_%d_ORD_COMMENT2", gbp->R16_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec16[gbp->R16_count - 1].ord_comment_2,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R16_%d_PRT_ON_PKSLP1", gbp->R16_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec16[gbp->R16_count - 1].prt_on_pkslp_1,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R16_%d_PRT_ON_LABEL1", gbp->R16_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec16[gbp->R16_count - 1].prt_on_label_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R16_%d_PRT_ON_PKSLP2", gbp->R16_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec16[gbp->R16_count - 1].prt_on_pkslp_2,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R16_%d_PRT_ON_LABEL2", gbp->R16_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec16[gbp->R16_count - 1].prt_on_label_2,gbp,stdout_buffer);
                
                        if(wo01_CatSendStr16(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec16[gbp->R16_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }
                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R17") == 0) 
                {

                        strcpy(gbp->wo01_send_rec->rec17.rec_type, "17");
                
                        for(gbp->j = 0; gbp->j < R17_COUP_NUM_COUNT; gbp->j++)
                        {
                                set_tag_name(gbp->tagname, "R17_%d_COUPON_NUM", gbp->j+1);
                                get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec17.coup_num[gbp->j],gbp,stdout_buffer);
                                
                                set_tag_name(gbp->tagname, "R17_%d_PERCENT_OFF", gbp->j+1);
                                get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec17.percent_off[gbp->j],gbp,stdout_buffer);

                                set_tag_name(gbp->tagname, "R17_%d_DOLLAR_OFF", gbp->j+1);
                                get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec17.dollar_off[gbp->j],gbp,stdout_buffer);
                        }

                        if(wo01_CatSendStr17(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec17) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }
                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R20") == 0) 
                {

                        strcpy(gbp->wo01_send_rec->rec20.rec_type, "20");
                        get_tag_data("R20_CC_TYPE"      , gbp->wo01_send_rec->rec20.cc_type,gbp,stdout_buffer);
                        get_tag_data("R20_CC_CARD_NUM"  , gbp->wo01_send_rec->rec20.cc_card_no,gbp,stdout_buffer);
                        get_tag_data("R20_CC_EXP_YEAR"  , gbp->wo01_send_rec->rec20.cc_exp_yy,gbp,stdout_buffer);
                        get_tag_data("R20_CC_EXP_MONTH" , gbp->wo01_send_rec->rec20.cc_exp_mm,gbp,stdout_buffer);
                        get_tag_data("R20_CC_AUTH_CODE" , gbp->wo01_send_rec->rec20.cc_auth_code,gbp,stdout_buffer);
                        get_tag_data("R20_CC_AUTH_FLAG" , gbp->wo01_send_rec->rec20.cc_auth_flag,gbp,stdout_buffer);
        
                        if(wo01_CatSendStr20(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec20) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R21") == 0) 
                {
                        gbp->R21_count++;
                        
                        strcpy(gbp->wo01_send_rec->rec21[gbp->R21_count - 1].rec_type, "21");

                        set_tag_name(gbp->tagname, "R21_%d_ORD_COMMENT1", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].ord_comment_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R21_%d_ORD_COMMENT2", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].ord_comment_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_PKSLP1", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_pkslp_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_LABEL1", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_label_1,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_GREET1", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_greet_1,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_PKSLP2", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_pkslp_2,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_LABEL2", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_label_2,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_GREET2", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_greet_2,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_INVO1", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_invo_1,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R21_%d_PRT_ON_INVO2", gbp->R21_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec21[gbp->R21_count - 1].prt_on_invo_2,gbp,stdout_buffer);
                        
                        if(wo01_CatSendStr21(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec21[gbp->R21_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R30") == 0) 
                {
                        gbp->R30_count++;
                        
                        strcpy(gbp->wo01_send_rec->rec30[gbp->R30_count - 1].rec_type, "30");

                        set_tag_name(gbp->tagname, "R30_%d_FNAME", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].first_name,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_MINITIAL", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].middle_init,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_LNAME", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].last_name,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_TITLE", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].title,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_COMPANY_NAME", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].company_name,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_ADDRESS_REF1", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].address_ref_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_ADDRESS_REF2", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].address_ref_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_STREET", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].street,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_CITY", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].city,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_STATE", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].state,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_ZIP_CODE", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].zip_code,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_COUNTRY_CODE", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].country_code,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_PHONE", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].phone,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_OLD_CUST_NUM", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].old_custno,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_CUST_NUM", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].cust_no,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_CUST_CHKDIG", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].cust_chkdig,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_CUST_EDP", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].cust_edp,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_CONV_FLAG", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].conv_flag,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_FUTURE_DATE", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].future_date,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_SHIP_METHOD", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].ship_method,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R30_%d_SHIPTO_NUM", gbp->R30_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec30[gbp->R30_count - 1].shipto_num,gbp,stdout_buffer);


                        if(wo01_CatSendStr30(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec30[gbp->R30_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R31") == 0) 
                {
                        gbp->R31_count++;
                
                        strcpy(gbp->wo01_send_rec->rec31[gbp->R31_count - 1].rec_type, "31");

                        set_tag_name(gbp->tagname, "R31_%d_ORD_COMMENT1", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].ord_comment_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_ORD_COMMENT2", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].ord_comment_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_PRT_ON_PKSLP1", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].prt_on_pkslp_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_PRT_ON_LABEL1", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].prt_on_label_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_PRT_ON_GREET1", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].prt_on_greet_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_PRT_ON_PKSLP2", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].prt_on_pkslp_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_PRT_ON_LABEL2", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].prt_on_label_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R31_%d_PRT_ON_GREET2", gbp->R31_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec31[gbp->R31_count - 1].prt_on_greet_2,gbp,stdout_buffer);

                        if(wo01_CatSendStr31(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec31[gbp->R31_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }
                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R33") == 0) 
                {
                        gbp->R33_count++;
                
                        strcpy(gbp->wo01_send_rec->rec33[gbp->R33_count - 1].rec_type, "33");

                        set_tag_name(gbp->tagname, "R33_%d_ST_EMAIL_ADDR", gbp->R33_count);
                        get_tag_data(gbp->tagname , gbp->wo01_send_rec->rec33[gbp->R33_count - 1].email_addr,gbp,stdout_buffer);


                        if(wo01_CatSendStr33(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec33[gbp->R33_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }
                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R40") == 0) 
                {
                        gbp->R40_count++;
                        strcpy(gbp->wo01_send_rec->rec40[gbp->R40_count - 1].rec_type, "40");
                        
                        set_tag_name(gbp->tagname, "R40_%d_ITEM_QTY", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].item_qty,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_ITEM_EDP", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].item_edp,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_TAX_NUM", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].tax_no,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_ITEM_NUM", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].item_no,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_TAX_AMT", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].tax_amt,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_TAX_RATE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].tax_rate,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_TAX_EXEMPT", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].tax_exempt,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_SHIP_METHOD", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].ship_method,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_PH_AMT", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].ph_amt,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_PRICE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].price,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_SHIP_DATE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].ship_date,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_POS_RET", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].pos_ret,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_ADDITIONAL_POSTAGE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].add_postage,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_ASSO_PROD_NUM", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].assoc_prod_num,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_ASSOC_TYPE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].assoc_type,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_OVERIDE_PRICE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].overide_price,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_CLUB_SALES_TYPE", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].club_sales_type,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_RETURN_FLAG", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].return_flag,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R40_%d_POVRD_REASON", gbp->R40_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec40[gbp->R40_count - 1].povrd_reason,gbp,stdout_buffer);

                        if(wo01_CatSendStr40(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec40[gbp->R40_count - 1], &gbp->wo01_send_rec->tmp_vars) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R41") == 0) 
                {
                        gbp->R41_count++;
                        
                        strcpy(gbp->wo01_send_rec->rec41[gbp->R41_count - 1].rec_type, "41");

                        set_tag_name(gbp->tagname, "R41_%d_CUSTOMIZE1", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].customize_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R41_%d_CUSTOMIZE2", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].customize_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R41_%d_BANK_ACCT1", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].bank_acct_1,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R41_%d_BANK_ACCT2", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].bank_acct_2,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R41_%d_KIT_COMP_EDP", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].kit_comp_edp,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R41_%d_FLAG", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].flag,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R41_%d_ITEM_NUM", gbp->R41_count);
                        get_tag_data(gbp->tagname   , gbp->wo01_send_rec->rec41[gbp->R41_count - 1].item_num,gbp,stdout_buffer);

                        if(wo01_CatSendStr41(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec41[gbp->R41_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }
                else if(strcmp(gbp->wo01_send_rec->wo01_oseq_tag[gbp->i].tag, "R42") == 0) 
                {
                        gbp->R42_count++;
        
                        strcpy(gbp->wo01_send_rec->rec42[gbp->R42_count - 1].rec_type, "42");

                        set_tag_name(gbp->tagname, "R42_%d_ITEM_QTY", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].item_qty,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_ITEM_EDP", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].item_edp,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_TAX_NUM", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].tax_no,gbp,stdout_buffer);
                        
                        set_tag_name(gbp->tagname, "R42_%d_ITEM_NUM", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].item_no,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_TAX_AMT", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].tax_amt,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_TAX_RATE", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].tax_rate,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_TAX_EXEMPT", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].tax_exempt,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_SHIP_METHOD", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].ship_method,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_PH_AMT", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].post_amt,gbp,stdout_buffer);

                        set_tag_name(gbp->tagname, "R42_%d_PRICE", gbp->R42_count);
                        get_tag_data(gbp->tagname    , gbp->wo01_send_rec->rec42[gbp->R42_count - 1].price,gbp,stdout_buffer);
                                        
                        if(wo01_CatSendStr42(gbp->wo01_send_rec->tmaxrec[gbp->i], &gbp->wo01_send_rec->rec42[gbp->R42_count - 1]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                                return(-1);
                        }
                }

        }

/* Create one huge string with all information to be passed */
        gbp->k = sprintf(gbp->sendbufcat, 
                 "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s",
                                           gbp->request_id,
                                           gbp->tran_id,
                                           gbp->company,
                                           gbp->division,
                                           gbp->uid,
                                           gbp->ip_address,
                                           " ");
        for (gbp->i=0; gbp->i < NUMBER_OF_RECORDS; gbp->i++){
                if ((gbp->wo01_send_rec->tmaxrec[gbp->i][0]!='\0')&&(gbp->wo01_send_rec->tmaxrec[gbp->i][1]!='\0')){
                        gbp->k += sprintf(gbp->sendbufcat+gbp->k, "%-320.320s", gbp->wo01_send_rec->tmaxrec[gbp->i]);
                }else
                {
                        *(gbp->sendbufcat+gbp->k)='*';
                        *(gbp->sendbufcat+gbp->k+1)='*';
                        gbp->k+=2;
                        break;
                }
        }

//#################################################################
/*
ap_rprintf(r,"%-69.69s\n",gbp->sendbufcat);
ap_rprintf(r,"<p>");
for (gbp->i=0; gbp->i<60; gbp->i++)
{
        if(1){   //(gbp->sendbufcat+69+(i*320)+1) != '\0'
        ap_rprintf(r,"%-320.320s\n",gbp->sendbufcat+69+(i*320));
        ap_rprintf(r,"<p>");
        }
}*/
//#################################################################

//if(strlen(gbp->sendbufcat) == wo01_LAN_SEND_BUF_LEN)
if(strlen(gbp->sendbufcat) <= wo01_LAN_SEND_BUF_LEN)
{
        if((gbp->sock = sga_connect(gbp->hphost, gbp->tmaxport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
        {
                XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                return(-1);
        }

        if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, gbp->k,r,gbp)) == SOCKET_ERROR)
        {
                XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                return(-1);
        }

        if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wo01_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                return(-1);
        }

        /* Do an additional send and recieve for confirmation  */
        if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
        {
                XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
                return(-1);
        }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
        sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

        wo01_ParceRecvStr(&gbp->wo01recvbuf, gbp->recvbufcat,r,gbp);
        
        free(gbp->sendbufcat);
        free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);

}
else
{
/*              ap_rprintf(r,"Couldn't stuff the send buffer: stuffed -> %d\n", ret);  */
        XML_Error("wo01_sendbufcat","Failed filling the send buffer","communications","-1",r,gbp);
        free(gbp->sendbufcat);
        free(gbp->recvbufcat);
                                free(gbp->wo01_send_rec);
        return(-1);
}

    return 0;
                                           
}
        
/******************************************************************/
/*                                                                                                                                */
/*     Create the send string from our current data               */
/*                                                                                                                                */
/******************************************************************/

int     wo01_CatSendStrblank(char *sz_tmaxrec)
{
        memset(sz_tmaxrec, ' ', 320);
        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}



int     wo01_CatSendStr10(char *sz_tmaxrec, wo01_rec10 *ptr_rec10)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-16.16s%-8.8s%-2.2s%-16.16s%-1.1s%-16.16s%-2.2s%-"
                                  "30.30s%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-"
                                  "4.4s%-14.14s%-14.14s%-24.24s%09d%-1.1s%09d%-1.1s%-"
                                  "1.1s%-6.6s",
                                  ptr_rec10->rec_type,
                                  ptr_rec10->order_num,
                                  ptr_rec10->source,
                                  ptr_rec10->mail_date,
                                  ptr_rec10->pay_method,
                                  ptr_rec10->first_name,
                                  ptr_rec10->middle_init,
                                  ptr_rec10->last_name,
                                  ptr_rec10->title,
                                  ptr_rec10->company_name,
                                  ptr_rec10->address_ref_1,
                                  ptr_rec10->address_ref_2,
                                  ptr_rec10->street,
                                  ptr_rec10->city,
                                  ptr_rec10->state,
                                  ptr_rec10->zip_code,
                                  ptr_rec10->country_code,
                                  ptr_rec10->day_phone,
                                  ptr_rec10->night_phone,
                                  ptr_rec10->old_custno,
                                  atoi(ptr_rec10->cust_no),
                                  ptr_rec10->cust_chkdig,
                                  atoi(ptr_rec10->cust_edp),
                                  ptr_rec10->no_promo,
                                  ptr_rec10->no_rent,
                                  ptr_rec10->filler2);
        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr12(char *sz_tmaxrec, wo01_rec12 *ptr_rec12)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-32.32s%-16.16s%-1.1s%-16.16s%-2.2s%-30.30s%-"
                                  "30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-14.14s%-"
                                  "4.4s%-24.24s%09d%-1.1s%-33.33s",
                                  ptr_rec12->rec_type,
                                  ptr_rec12->filler1,
                                  ptr_rec12->first_name,
                                  ptr_rec12->middle_init,
                                  ptr_rec12->last_name,
                                  ptr_rec12->title,
                                  ptr_rec12->company_name,
                                  ptr_rec12->address_ref_1,
                                  ptr_rec12->address_ref_2,
                                  ptr_rec12->street,
                                  ptr_rec12->city,
                                  ptr_rec12->state,
                                  ptr_rec12->zip_code,
                                  ptr_rec12->phone,
                                  ptr_rec12->country_code,
                                  ptr_rec12->old_custno,
                                  atoi(ptr_rec12->cust_no),
                                  ptr_rec12->cust_chkdig,
                                  ptr_rec12->filler2);
        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr13(char *sz_tmaxrec, wo01_rec13 *ptr_rec13)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-50.50s%-16.16s%-1.1s%-16.16s%-"
                                  "16.16s%-100.100s%-111.111s",
                                  ptr_rec13->rec_type,
                                  ptr_rec13->filler1,
                                  ptr_rec13->email_addr,
                                  ptr_rec13->net_password,
                                  ptr_rec13->recap_flag,
                                  ptr_rec13->ip_address,
                                  ptr_rec13->user_id,
                                  ptr_rec13->demograph,
                                  ptr_rec13->filler2);
        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr15(char *sz_tmaxrec, wo01_rec15 *ptr_rec15)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%08d%-8.8s%-20.20s%-8.8s%-6.6s%-8.8s%-1.1s%-"
                                  "1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-1.1s%-5.5s"
                                  "%-4.4s%-2.2s%-100.100s%-4.4s%-4.4s%-14.14s%-24.24s%-"
                                  "2.2s%-1.1s%-1.1s%-8.8s%-1.1s%-24.24s%-10.10s%-17.17s"
                                  "%-1.1s%-1.1s%-1.1s%-1.1s%-8.8s%-10.10s",
                                  ptr_rec15->rec_type,
                                  ptr_rec15->filler1,
                                  (int)((atof(ptr_rec15->remit_amt))*100),
                                  ptr_rec15->future_date,
                                  ptr_rec15->po_number,
                                  ptr_rec15->entered_date,
                                  ptr_rec15->entered_time,
                                  ptr_rec15->entered_by,
                                  ptr_rec15->hold_order,
                                  ptr_rec15->expedite,
                                  ptr_rec15->no_backorder,
                                  ptr_rec15->postpone_order,
                                  ptr_rec15->no_insurance,
                                  ptr_rec15->cc_up_front,
                                  ptr_rec15->overpayment,
                                  ptr_rec15->standing_order,
                                  ptr_rec15->gender,
                                  ptr_rec15->filler2,
                                  ptr_rec15->opt_code,
                                  ptr_rec15->ship_method,
                                  ptr_rec15->demographic,
                                  ptr_rec15->sales_terr,
                                  ptr_rec15->sales_rep,
                                  ptr_rec15->fax_phone,
                                  ptr_rec15->order_xref,
                                  ptr_rec15->cust_type,
                                  ptr_rec15->error_flag,
                                  ptr_rec15->installment,
                                  ptr_rec15->ship_date,
                                  ptr_rec15->user_hold_code,
                                  ptr_rec15->bank_acct,
                                  ptr_rec15->rut_num,
                                  ptr_rec15->filler3,
                                  ptr_rec15->continuity_flag,
                                  ptr_rec15->bypass_adv,
                                  ptr_rec15->disc_code1,
                                  ptr_rec15->disc_code2,
                                  ptr_rec15->birthday,
                                  ptr_rec15->filler4);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr16(char *sz_tmaxrec, wo01_rec16 *ptr_rec16)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s"
                                  "%-1.1s%-1.1s%-184.184s",
                                  ptr_rec16->rec_type,
                                  ptr_rec16->filler1,
                                  ptr_rec16->ord_comment_1,
                                  ptr_rec16->ord_comment_2,
                                  ptr_rec16->prt_on_pkslp_1,
                                  ptr_rec16->prt_on_label_1,
                                  ptr_rec16->filler2,
                                  ptr_rec16->prt_on_pkslp_2,
                                  ptr_rec16->prt_on_label_2,
                                  ptr_rec16->filler3,
                                  ptr_rec16->filler4);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr17(char *sz_tmaxrec, wo01_rec17 *ptr_rec17)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d"
                                  "%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d"
                                  "%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d"
                                  "%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d"
                                  "%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d"
                                  "%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-9.9s%-2.2s%06d%-4.4s",
                                  ptr_rec17->rec_type,
                                  ptr_rec17->filler1,
                                  ptr_rec17->coup_num[0],
                                  ptr_rec17->percent_off[0],
                                  (int)((atof(ptr_rec17->dollar_off[0]))*100),
                                  ptr_rec17->coup_num[1],
                                  ptr_rec17->percent_off[1],
                                  (int)((atof(ptr_rec17->dollar_off[1]))*100),
                                  ptr_rec17->coup_num[2],
                                  ptr_rec17->percent_off[2],
                                  (int)((atof(ptr_rec17->dollar_off[2]))*100),
                                  ptr_rec17->coup_num[3],
                                  ptr_rec17->percent_off[3],
                                  (int)((atof(ptr_rec17->dollar_off[3]))*100),
                                  ptr_rec17->coup_num[4],
                                  ptr_rec17->percent_off[4],
                                  (int)((atof(ptr_rec17->dollar_off[4]))*100),
                                  ptr_rec17->coup_num[5],
                                  ptr_rec17->percent_off[5],
                                  (int)((atof(ptr_rec17->dollar_off[5]))*100),
                                  ptr_rec17->coup_num[6],
                                  ptr_rec17->percent_off[6],
                                  (int)((atof(ptr_rec17->dollar_off[6]))*100),
                                  ptr_rec17->coup_num[7],
                                  ptr_rec17->percent_off[7],
                                  (int)((atof(ptr_rec17->dollar_off[7]))*100),
                                  ptr_rec17->coup_num[8],
                                  ptr_rec17->percent_off[8],
                                  (int)((atof(ptr_rec17->dollar_off[8]))*100),
                                  ptr_rec17->coup_num[9],
                                  ptr_rec17->percent_off[9],
                                  (int)((atof(ptr_rec17->dollar_off[9]))*100),
                                  ptr_rec17->coup_num[10],
                                  ptr_rec17->percent_off[10],
                                  (int)((atof(ptr_rec17->dollar_off[10]))*100),
                                  ptr_rec17->coup_num[11],
                                  ptr_rec17->percent_off[11],
                                  (int)((atof(ptr_rec17->dollar_off[11]))*100),
                                  ptr_rec17->coup_num[12],
                                  ptr_rec17->percent_off[12],
                                  (int)((atof(ptr_rec17->dollar_off[12]))*100),
                                  ptr_rec17->coup_num[13],
                                  ptr_rec17->percent_off[13],
                                  (int)((atof(ptr_rec17->dollar_off[13]))*100),
                                  ptr_rec17->coup_num[14],
                                  ptr_rec17->percent_off[14],
                                  (int)((atof(ptr_rec17->dollar_off[14]))*100),
                                  ptr_rec17->coup_num[15],
                                  ptr_rec17->percent_off[15],
                                  (int)((atof(ptr_rec17->dollar_off[15]))*100),
                                  ptr_rec17->coup_num[16],
                                  ptr_rec17->percent_off[16],
                                  (int)((atof(ptr_rec17->dollar_off[16]))*100),
                                  ptr_rec17->coup_num[17],
                                  ptr_rec17->percent_off[17],
                                  (int)((atof(ptr_rec17->dollar_off[17]))*100),
                                  ptr_rec17->filler2);
                                  
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}
        

int     wo01_CatSendStr20(char *sz_tmaxrec, wo01_rec20 *ptr_rec20)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-2.2s%-20.20s%-2.2s%-2.2s%-6.6s%-1.1s"
                                  "%-277.277s",
                                  ptr_rec20->rec_type,
                                  ptr_rec20->filler1,
                                  ptr_rec20->cc_type,
                                  ptr_rec20->cc_card_no,
                                  ptr_rec20->cc_exp_yy,
                                  ptr_rec20->cc_exp_mm,
                                  ptr_rec20->cc_auth_code,
                                  ptr_rec20->cc_auth_flag,
                                  ptr_rec20->filler2);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr21(char *sz_tmaxrec, wo01_rec21 *ptr_rec21)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s"
                                  "%-1.1s%-1.1s%-1.1s%-1.1s%-182.182s",
                                  ptr_rec21->rec_type,
                                  ptr_rec21->filler1,
                                  ptr_rec21->ord_comment_1,
                                  ptr_rec21->ord_comment_2,
                                  ptr_rec21->prt_on_pkslp_1,
                                  ptr_rec21->prt_on_label_1,
                                  ptr_rec21->prt_on_greet_1,
                                  ptr_rec21->prt_on_pkslp_2,
                                  ptr_rec21->prt_on_label_2,
                                  ptr_rec21->prt_on_greet_2,
                                  ptr_rec21->prt_on_invo_1,
                                  ptr_rec21->prt_on_invo_2,
                                  ptr_rec21->filler2);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr30(char *sz_tmaxrec, wo01_rec30 *ptr_rec30)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-16.16s%-1.1s%-16.16s%-2.2s%-30.30s"
                                  "%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-4.4s"
                                  "%-14.14s%-24.24s%09d%-1.1s%09d%-1.1s%-8.8s"
                                  "%-2.2s%-2.2s%-35.35s",

                                  ptr_rec30->rec_type,
                                  ptr_rec30->filler1,
                                  ptr_rec30->first_name,
                                  ptr_rec30->middle_init,
                                  ptr_rec30->last_name,
                                  ptr_rec30->title,
                                  ptr_rec30->company_name,
                                  ptr_rec30->address_ref_1,
                                  ptr_rec30->address_ref_2,
                                  ptr_rec30->street,
                                  ptr_rec30->city,
                                  ptr_rec30->state,
                                  ptr_rec30->zip_code,
                                  ptr_rec30->country_code,
                                  ptr_rec30->phone,
                                  ptr_rec30->old_custno,
                                  atoi(ptr_rec30->cust_no),
                                  ptr_rec30->cust_chkdig,
                                  atoi(ptr_rec30->cust_edp),
                                  ptr_rec30->conv_flag,
                                  ptr_rec30->future_date,
                                  ptr_rec30->ship_method,
                                  ptr_rec30->shipto_num,
                                  ptr_rec30->filler2);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}


int     wo01_CatSendStr31(char *sz_tmaxrec, wo01_rec31 *ptr_rec31)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-60.60s%-60.60s%-1.1s%-1.1s%-1.1s%-1.1s"
                                  "%-1.1s%-1.1s%-184.184s",

                                  ptr_rec31->rec_type,
                                  ptr_rec31->filler1,
                                  ptr_rec31->ord_comment_1,
                                  ptr_rec31->ord_comment_2,
                                  ptr_rec31->prt_on_pkslp_1,
                                  ptr_rec31->prt_on_label_1,
                                  ptr_rec31->prt_on_greet_1,
                                  ptr_rec31->prt_on_pkslp_2,
                                  ptr_rec31->prt_on_label_2,
                                  ptr_rec31->prt_on_greet_2,
                                  ptr_rec31->filler2);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr33(char *sz_tmaxrec, wo01_rec33 *ptr_rec33)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-50.50s%-260.260s",
                                  ptr_rec33->rec_type,
                                  ptr_rec33->filler1,
                                  ptr_rec33->email_addr,
                                  ptr_rec33->filler2);
        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr40(char *sz_tmaxrec, wo01_rec40 *ptr_rec40, wo01_tmp_vars *ptr_tmp)
{

 memset(ptr_tmp->tmp_tax_rate, '\0', R40_TAX_RATE_LEN+1);
 memset(ptr_tmp->tmp_tax_rate, ' ', R40_TAX_RATE_LEN);
 
 ptr_tmp->j = 0;
 ptr_tmp->tmp_ptr1 = ptr_rec40->tax_rate;

 while(*(ptr_tmp->tmp_ptr1) != '.' && *(ptr_tmp->tmp_ptr1) != '\0')
 {
  ptr_tmp->j++;
  ptr_tmp->tmp_ptr1++;
 }

 
 if(ptr_tmp->j > 2)
 {
  ptr_tmp->tmp_ptr1 = ptr_rec40->tax_rate;
  ptr_tmp->tmp_ptr2 = ptr_tmp->tmp_tax_rate;
  ptr_tmp->j = strlen(ptr_rec40->tax_rate);

  for(ptr_tmp->i=0; ptr_tmp->i <= ptr_tmp->j; ptr_tmp->i++)
  {

   if(ptr_tmp->i == 2)
   {
    *(ptr_tmp->tmp_ptr2) = '.';
    ptr_tmp->tmp_ptr2++;
   }

   if(*(ptr_tmp->tmp_ptr1)!= '.')
   {
    *(ptr_tmp->tmp_ptr2) = *(ptr_tmp->tmp_ptr1);
    ptr_tmp->tmp_ptr2++;
   }
   ptr_tmp->tmp_ptr1++;
  }

  strcpy(ptr_rec40->tax_rate, ptr_tmp->tmp_tax_rate);
 }

        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-10.10s%04d%09d%-60.60s%-20.20s%07d"
                                  "%06d%-1.1s%-2.2s%07d%09d%-8.8s%-1.1s%07d%-3.3s%-1.1s%-1.1s%-1.1s"
                                  "%-1.1s%-2.2s%-5.5s%-153.153s",

                                  ptr_rec40->rec_type,
                                  ptr_rec40->filler1,
                                  atoi(ptr_rec40->item_qty),
                                  atoi(ptr_rec40->item_edp),
                                  ptr_rec40->tax_no,
                                  ptr_rec40->item_no,
                                  (int)((atof(ptr_rec40->tax_amt))*100),
                                  (int)((atof(ptr_rec40->tax_rate))*10000),
                                  ptr_rec40->tax_exempt,
                                  ptr_rec40->ship_method,
                                  (int)(atof(ptr_rec40->ph_amt)*100),
                                  (int)((atof(ptr_rec40->price))*100),
                                  ptr_rec40->ship_date,
                                  ptr_rec40->pos_ret,
                                  (int)((atof(ptr_rec40->add_postage))*100),
                                  ptr_rec40->assoc_prod_num,
                                  ptr_rec40->assoc_type,
                                  ptr_rec40->overide_price,
                                  ptr_rec40->club_sales_type,
                                  ptr_rec40->return_flag,
                                  ptr_rec40->povrd_reason,
                                  ptr_rec40->filler3,
                                  ptr_rec40->filler2);
                                  

        sz_tmaxrec[320] = '\0';

    if(ptr_rec40->ph_amt[0] == '\0')
           memset(sz_tmaxrec + 121, ' ', 7);    
 
        if(ptr_rec40->add_postage[0] == '\0')
           memset(sz_tmaxrec + 146, ' ', 7);

        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr41(char *sz_tmaxrec, wo01_rec41 *ptr_rec41)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-60.60s%-60.60s%-1.1s%-1.1s%09d%-"
                                  "158.158s%-1.1s%-20.20s",
                                  ptr_rec41->rec_type,
                                  ptr_rec41->filler1,
                                  ptr_rec41->customize_1,
                                  ptr_rec41->customize_2,
                                  ptr_rec41->bank_acct_1,
                                  ptr_rec41->bank_acct_2,
                                  atoi(ptr_rec41->kit_comp_edp),
                                  ptr_rec41->filler2,
                                  ptr_rec41->flag,
                                  ptr_rec41->item_num);

        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}

int     wo01_CatSendStr42(char *sz_tmaxrec, wo01_rec42 *ptr_rec42)
{
        memset(sz_tmaxrec, ' ', 320);
        sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%06d%09d%-60.60s%-20.20s%07d"
                                  "%06d%-1.1s%-2.2s%07d%09d%-183.183s",
                                  ptr_rec42->rec_type,
                                  ptr_rec42->filler1,
                                  atoi(ptr_rec42->item_qty),
                                  atoi(ptr_rec42->item_edp),
                                  ptr_rec42->tax_no,
                                  ptr_rec42->item_no,
                                  (int)((atof(ptr_rec42->tax_amt))*100),
                                  (int)((atof(ptr_rec42->tax_rate))*10000),
                                  ptr_rec42->tax_exempt,
                                  ptr_rec42->ship_method,
                                  (int)((atof(ptr_rec42->post_amt))*100),
                                  (int)((atof(ptr_rec42->price))*100),
                                  ptr_rec42->filler2);
                                  
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}


int wo01_ParceRecvStr(wo01_recv *ptr_recvbuf, char *rtnbuf, request_rec *r, struct global_struct *gbp)
{
        gbp->count = 0;
    
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wo02.xsl\"?>\n");  

#endif
/*
        ap_rprintf(r,"<!DOCTYPE %s02 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                        wo_tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s02.dtd\">\n", url_tag, wo_tag);
*/
        ap_rprintf(r,"<!-- ORDER RECAP -->\n");


        ap_rprintf(r,"%s02 %s\"wo02\">\n", gbp->wo_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, rtnbuf + gbp->count, REQ_ID_LEN);
        ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += REQ_ID_LEN;

    memset(ptr_recvbuf->tran_id,'\0', TRAN_ID_LEN+1);
    memcpy(ptr_recvbuf->tran_id, rtnbuf + gbp->count, TRAN_ID_LEN);
    gbp->count += TRAN_ID_LEN;

    memset(ptr_recvbuf->user_id,'\0', USER_ID_LEN+1);
    memcpy(ptr_recvbuf->user_id, rtnbuf + gbp->count, USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->user_id));
    gbp->count += USER_ID_LEN;

        memset(ptr_recvbuf->success_flag,'\0', SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, rtnbuf + gbp->count, SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_FLAG>%s</SUCCESS_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += SUCCESS_FLAG_LEN;

        memset(ptr_recvbuf->reject_reason,'\0', REJECT_REASON_LEN+1);
    memcpy(ptr_recvbuf->reject_reason, rtnbuf + gbp->count, REJECT_REASON_LEN);
        ap_rprintf(r,"		<REJECT_REASON>%s</REJECT_REASON>\n", handle_special_chars(gbp,ptr_recvbuf->reject_reason));
    gbp->count += REJECT_REASON_LEN;

        gbp->count += 45;

        memset(ptr_recvbuf->product_dol,'\0', PRODUCT_DOL_LEN+1);
    memcpy(ptr_recvbuf->product_dol, rtnbuf + gbp->count, PRODUCT_DOL_LEN);
        ap_rprintf(r,"		<PRODUCT_DOL>%s</PRODUCT_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->product_dol));
    gbp->count += PRODUCT_DOL_LEN;

        memset(ptr_recvbuf->discount_dol,'\0', DISC_DOL_LEN+1);
    memcpy(ptr_recvbuf->discount_dol, rtnbuf + gbp->count, DISC_DOL_LEN);
        ap_rprintf(r,"		<DISCOUNT_DOL>%s</DISCOUNT_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->discount_dol));
    gbp->count += DISC_DOL_LEN;

        memset(ptr_recvbuf->tax_dol,'\0', TAX_DOL_LEN+1);
    memcpy(ptr_recvbuf->tax_dol, rtnbuf + gbp->count, TAX_DOL_LEN);
        ap_rprintf(r,"		<TAX_DOL>%s</TAX_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->tax_dol));
    gbp->count += TAX_DOL_LEN;

        memset(ptr_recvbuf->postage_dol,'\0', POSTAGE_DOL_LEN+1);
    memcpy(ptr_recvbuf->postage_dol, rtnbuf + gbp->count, POSTAGE_DOL_LEN);
        ap_rprintf(r,"		<POSTAGE_DOL>%s</POSTAGE_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->postage_dol));
    gbp->count += POSTAGE_DOL_LEN;

        memset(ptr_recvbuf->insurance_dol,'\0', INSURANCE_DOL_LEN+1);
    memcpy(ptr_recvbuf->insurance_dol, rtnbuf + gbp->count, INSURANCE_DOL_LEN);
        ap_rprintf(r,"		<INSURANCE_DOL>%s</INSURANCE_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->insurance_dol));
    gbp->count += INSURANCE_DOL_LEN;

        memset(ptr_recvbuf->total_dol,'\0', TOTAL_DOL_LEN+1);
    memcpy(ptr_recvbuf->total_dol, rtnbuf + gbp->count, TOTAL_DOL_LEN);
        ap_rprintf(r,"		<TOTAL_DOL>%s</TOTAL_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->total_dol));
    gbp->count += TOTAL_DOL_LEN;

        memset(ptr_recvbuf->pay_method,'\0', PAY_METH_LEN+1);
    memcpy(ptr_recvbuf->pay_method, rtnbuf + gbp->count, PAY_METH_LEN);
        ap_rprintf(r,"		<PAY_METHOD>%s</PAY_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->pay_method));
    gbp->count += PAY_METH_LEN;

        memset(ptr_recvbuf->ship_method,'\0', SHIP_METH_LEN+1);
    memcpy(ptr_recvbuf->ship_method, rtnbuf + gbp->count, SHIP_METH_LEN);
        ap_rprintf(r,"		<SHIP_METHOD>%s</SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method));
    gbp->count += SHIP_METH_LEN;

        memset(ptr_recvbuf->order_no,'\0', ORD_NUM_LEN+1);
    memcpy(ptr_recvbuf->order_no, rtnbuf + gbp->count, ORD_NUM_LEN);
        ap_rprintf(r,"		<ORDER_NUM>%s</ORDER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->order_no));
    gbp->count += ORD_NUM_LEN;


        memset(ptr_recvbuf->ship_method_change,'\0', SHIP_METH_CHANGE_LEN+1);
    memcpy(ptr_recvbuf->ship_method_change, rtnbuf + gbp->count, SHIP_METH_CHANGE_LEN);
        ap_rprintf(r,"		<CHANGE_SHIP_METHOD>%s</CHANGE_SHIP_METHOD>\n", handle_special_chars(gbp,ptr_recvbuf->ship_method_change));
    gbp->count += SHIP_METH_CHANGE_LEN;

        memset(ptr_recvbuf->linkshare_flag,'\0', LINKSHARE_FLAG_LEN+1);
    memcpy(ptr_recvbuf->linkshare_flag, rtnbuf + gbp->count, LINKSHARE_FLAG_LEN);
        ap_rprintf(r,"		<LINKSHARE_FLAG>%s</LINKSHARE_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->linkshare_flag));
    gbp->count += LINKSHARE_FLAG_LEN;

        memset(ptr_recvbuf->linkshare_id,'\0', LINKSHARE_ID_LEN+1);
    memcpy(ptr_recvbuf->linkshare_id, rtnbuf + gbp->count, LINKSHARE_ID_LEN);
        ap_rprintf(r,"		<LINKSHARE_ID>%s</LINKSHARE_ID>\n", handle_special_chars(gbp,ptr_recvbuf->linkshare_id));
    gbp->count += LINKSHARE_ID_LEN;

        memset(ptr_recvbuf->linkshare_zip,'\0', LINKSHARE_ZIP_LEN+1);
    memcpy(ptr_recvbuf->linkshare_zip, rtnbuf + gbp->count, LINKSHARE_ZIP_LEN);
        ap_rprintf(r,"		<LINKSHARE_ZIP>%s</LINKSHARE_ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->linkshare_zip));
    gbp->count += LINKSHARE_ZIP_LEN;

        memset(ptr_recvbuf->linkshare_date_out,'\0', LINKSHARE_DATE_OUT_LEN+1);
    memcpy(ptr_recvbuf->linkshare_date_out, rtnbuf + gbp->count, LINKSHARE_DATE_OUT_LEN);
        ap_rprintf(r,"		<LINKSHARE_DATE_OUT>%s</LINKSHARE_DATE_OUT>\n", handle_special_chars(gbp,ptr_recvbuf->linkshare_date_out));
    gbp->count += LINKSHARE_DATE_OUT_LEN;

        memset(ptr_recvbuf->linkshare_time_out,'\0', LINKSHARE_TIME_OUT_LEN+1);
    memcpy(ptr_recvbuf->linkshare_time_out, rtnbuf + gbp->count, LINKSHARE_TIME_OUT_LEN);
        ap_rprintf(r,"		<LINKSHARE_TIME_OUT>%s</LINKSHARE_TIME_OUT>\n", handle_special_chars(gbp,ptr_recvbuf->linkshare_time_out));
    gbp->count += LINKSHARE_TIME_OUT_LEN;

        memset(ptr_recvbuf->shipto_num,'\0', SHIPTO_NO_LEN+1);
    memcpy(ptr_recvbuf->shipto_num, rtnbuf + gbp->count, SHIPTO_NO_LEN);
        ap_rprintf(r,"		<SHIPTO_NUM>%s</SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto_num));
    gbp->count += SHIPTO_NO_LEN;

        memset(ptr_recvbuf->net_dol,'\0', NET_DOL_LEN+1);
    memcpy(ptr_recvbuf->net_dol, rtnbuf + gbp->count, NET_DOL_LEN);
        ap_rprintf(r,"		<NET_DOL>%s</NET_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->net_dol));
    gbp->count += NET_DOL_LEN;

        memset(ptr_recvbuf->gift_dol,'\0', GIFT_DOL_LEN+1);
    memcpy(ptr_recvbuf->gift_dol, rtnbuf + gbp->count, GIFT_DOL_LEN);
        ap_rprintf(r,"		<GIFT_DOL>%s</GIFT_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->gift_dol));
    gbp->count += GIFT_DOL_LEN;

        for(gbp->i = 0; gbp->i < SHIPTO_COUNT; gbp->i++)
        {
                memset(ptr_recvbuf->shipto[gbp->i].shipto_st_num,'\0', SHIPTO_ST_NO_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_st_num, rtnbuf + gbp->count, SHIPTO_ST_NO_LEN);
                gbp->count += SHIPTO_ST_NO_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_edp,'\0', SHIPTO_EDP_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_edp, rtnbuf + gbp->count, SHIPTO_EDP_LEN);
                gbp->count += SHIPTO_EDP_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_sm_code,'\0', SHIPTO_SM_CODE_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_sm_code, rtnbuf + gbp->count, SHIPTO_SM_CODE_LEN);
                gbp->count += SHIPTO_SM_CODE_LEN;
                sprintf(ptr_recvbuf->shipto[gbp->i].shipto_sm_code,"%s",
                                     rtrim(ptr_recvbuf->shipto[gbp->i].shipto_sm_code,SHIPTO_SM_CODE_LEN+1,gbp));

                memset(ptr_recvbuf->shipto[gbp->i].shipto_sm_desc,'\0', SHIPTO_SM_DESC_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_sm_desc, rtnbuf + gbp->count, SHIPTO_SM_DESC_LEN);
                gbp->count += SHIPTO_SM_DESC_LEN;

                memset(ptr_recvbuf->shipto[gbp->i].shipto_sm_change,'\0', SHIPTO_SM_CHANGE_LEN+1);
                memcpy(ptr_recvbuf->shipto[gbp->i].shipto_sm_change, rtnbuf + gbp->count, SHIPTO_SM_CHANGE_LEN);
                gbp->count += SHIPTO_SM_CHANGE_LEN;

                if((strlen(ptr_recvbuf->shipto[gbp->i].shipto_sm_code)) > 0)
                {
                        ap_rprintf(r,"		<SHIPTO_DETAILS>\n");
                        ap_rprintf(r,"			<SHIPTO_SHIPTO_NUM>%s</SHIPTO_SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_st_num));
                        ap_rprintf(r,"			<SHIPTO_EDP>%s</SHIPTO_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_edp));
                        ap_rprintf(r,"			<SHIPTO_SM_CODE>%s</SHIPTO_SM_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_sm_code));
                        ap_rprintf(r,"			<SHIPTO_SHIP_METHOD_DESC>%s</SHIPTO_SHIP_METHOD_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_sm_desc));
                        ap_rprintf(r,"			<SHIPTO_SM_CHANGE>%s</SHIPTO_SM_CHANGE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_sm_change));
                        ap_rprintf(r,"		</SHIPTO_DETAILS>\n");
                }
        }

        memset(ptr_recvbuf->flooz_pm,'\0', FLOOZ_PM_LEN+1);
    memcpy(ptr_recvbuf->flooz_pm, rtnbuf + gbp->count, FLOOZ_PM_LEN);
        ap_rprintf(r,"		<FLOOZ_PM_FLAG>%s</FLOOZ_PM_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->flooz_pm));
    gbp->count += FLOOZ_PM_LEN;

        memset(ptr_recvbuf->cust_edp,'\0', CUST_EDP_LEN+1);
    memcpy(ptr_recvbuf->cust_edp, rtnbuf + gbp->count, CUST_EDP_LEN);
        ap_rprintf(r,"		<CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->cust_edp));
    gbp->count += CUST_EDP_LEN;

    memset(ptr_recvbuf->gift_coup_type,'\0', GIFT_COUP_TYPE_LEN+1);
    memcpy(ptr_recvbuf->gift_coup_type, rtnbuf + gbp->count, GIFT_COUP_TYPE_LEN);
        ap_rprintf(r,"		<GIFT_COUPON_TYPE>%s</GIFT_COUPON_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->gift_coup_type));
    gbp->count += GIFT_COUP_TYPE_LEN;

        memset(ptr_recvbuf->gift_wrap_dol,'\0', GIFT_WRAP_DOL_LEN+1);
    memcpy(ptr_recvbuf->gift_wrap_dol, rtnbuf + gbp->count, GIFT_WRAP_DOL_LEN);
        ap_rprintf(r,"		<GIFT_WRAP_DOL>%s</GIFT_WRAP_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->gift_wrap_dol));
    gbp->count += GIFT_WRAP_DOL_LEN;

        memset(ptr_recvbuf->pty_payer_flag,'\0', PTY_PAYER_FLAG+1);
    memcpy(ptr_recvbuf->pty_payer_flag, rtnbuf + gbp->count, PTY_PAYER_FLAG);
        ap_rprintf(r,"		<THIRD_PTY_PAYER_FLAG>%s</THIRD_PTY_PAYER_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->pty_payer_flag));
    gbp->count += PTY_PAYER_FLAG;

memset(ptr_recvbuf->ai_count,'\0', AI_COUNT_LEN+1);
        memcpy(ptr_recvbuf->ai_count, rtnbuf + gbp->count, AI_COUNT_LEN);
        ap_rprintf(r,"		<NUM_OF_AUTO_INSERT_ITEMS>%s</NUM_OF_AUTO_INSERT_ITEMS>\n", handle_special_chars(gbp,ptr_recvbuf->ai_count));
        gbp->count += AI_COUNT_LEN;

        for(gbp->k = 0; gbp->k < AUTO_INSERT_COUNT; gbp->k++)
        {
                memset(ptr_recvbuf->auto_insert[gbp->k].ai_item_no,'\0', AI_ITEM_NO_LEN+1);
                memcpy(ptr_recvbuf->auto_insert[gbp->k].ai_item_no, rtnbuf + gbp->count, AI_ITEM_NO_LEN);
                gbp->count += AI_ITEM_NO_LEN;

                memset(ptr_recvbuf->auto_insert[gbp->k].ai_item_qty,'\0', AI_ITEM_QTY_LEN+1);
                memcpy(ptr_recvbuf->auto_insert[gbp->k].ai_item_qty, rtnbuf + gbp->count, AI_ITEM_QTY_LEN);
                gbp->count += AI_ITEM_QTY_LEN;

                memset(ptr_recvbuf->auto_insert[gbp->k].ai_item_price,'\0', AI_ITEM_PRICE_LEN+1);
                memcpy(ptr_recvbuf->auto_insert[gbp->k].ai_item_price, rtnbuf + gbp->count, AI_ITEM_PRICE_LEN);
                gbp->count += AI_ITEM_PRICE_LEN;

                memset(ptr_recvbuf->auto_insert[gbp->k].ai_item_st_num,'\0', AI_ITEM_ST_NUM_LEN+1);
                memcpy(ptr_recvbuf->auto_insert[gbp->k].ai_item_st_num, rtnbuf + gbp->count, AI_ITEM_ST_NUM_LEN);
                gbp->count += AI_ITEM_ST_NUM_LEN;

                if(atoi(ptr_recvbuf->auto_insert[gbp->k].ai_item_qty) > 0)
                {
                   ap_rprintf(r,"		<AUTO_INSERT_ITEM_NO>%s</AUTO_INSERT_ITEM_NO>\n", handle_special_chars(gbp,ptr_recvbuf->auto_insert[gbp->k].ai_item_no));
                   ap_rprintf(r,"		<AUTO_INSERT_QTY>%s</AUTO_INSERT_QTY>\n", handle_special_chars(gbp,ptr_recvbuf->auto_insert[gbp->k].ai_item_qty));
                   ap_rprintf(r,"		<AUTO_INSERT_PRICE>%s</AUTO_INSERT_PRICE>\n", handle_special_chars(gbp,ptr_recvbuf->auto_insert[gbp->k].ai_item_price));
                   ap_rprintf(r,"		<AUTO_INSERT_SHIPTO_NUM>%s</AUTO_INSERT_SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->auto_insert[gbp->k].ai_item_st_num));
                }

        }


        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag); 
        ap_rprintf(r,"%s02>\n", gbp->wo_betag);   

    return(0);
}


        
