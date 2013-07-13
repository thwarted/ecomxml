/* wo03.c */
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

int             wo03_CatSendStr11(char *, wo03_rec11 *);
int             wo03_CatSendStr13(char *, wo03_rec13 *);
int             wo03_CatSendStr15(char *, wo03_rec15 *);
int             wo03_CatSendStrblank(char *);

int             wo03_ParceRecvStr(wo03_recv *, char *, request_rec *, struct global_struct *);
int             wo03_lt_process(request_rec *, struct global_struct *, char *);


int wo03_start(struct global_struct *gbp)
{
    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int wo03_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((wo03_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int wo03_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->wo03_send_rec = (wo03_send_records *)malloc(sizeof(wo03_send_records));

        if (gbp->wo03_send_rec == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate gbp->wo01_send_rec\n");
                fflush(stdout);
                return(-1);
        }
        
        gbp->sendbufcat = malloc(wo03_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate sendbufcat\n");
                fflush(stdout);
                return(-1);
        }

        gbp->recvbufcat = malloc(wo03_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory to allocate recvbufcat\n");
                fflush(stdout);
                return(-1);
        }

    memset(gbp->sendbufcat, '\0', wo03_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', wo03_LAN_RECV_BUF_LEN + 1);

        memset(gbp->request_id   ,'\0', REQ_ID_LEN + 1);
        memset(gbp->company      ,'\0', COMPANY_LEN + 1);
        memset(gbp->division     ,'\0', DIVISION_LEN + 1);

/* fill the header */
        
        sprintf(gbp->confirmbuf, "     ");
        strcpy(gbp->request_id, "XML");
        strcpy(gbp->tran_id, "RQST");
        strcpy(gbp->ip_address, r->connection->remote_ip);

        get_tag_data("COMPANY"      , gbp->company,gbp,stdout_buffer);
        get_tag_data("DIVISION"     , gbp->division,gbp,stdout_buffer);
        get_tag_data("UID"                      , gbp->uid,gbp,stdout_buffer);


/*  record 11  */
        
//      memset(&gbp->wo03_send_rec->wo03rec11              ,'\0', sizeof(gbp->wo03_send_rec->wo03rec11));
        memset(gbp->wo03_send_rec->wo03rec11.filler1       ,' ', R11_FILLER1_LEN+1);

/* record 13  */

//      memset(&gbp->wo03_send_rec->wo03rec13              ,'\0', sizeof(gbp->wo03_send_rec->wo03rec13));
        memset(gbp->wo03_send_rec->wo03rec13.filler1       ,' ', R13_FILLER1_LEN+1);
        memset(gbp->wo03_send_rec->wo03rec13.filler2       ,' ', R13_FILLER2_LEN+1);

/*  record 15  */

//      memset(&gbp->wo03_send_rec->wo03rec15              ,'\0', sizeof(gbp->wo03_send_rec->wo03rec15));
        memset(gbp->wo03_send_rec->wo03rec15.filler1       ,' ', R15_FILLER1_LEN+1);
        memset(gbp->wo03_send_rec->wo03rec15.filler2       ,' ', R15_FILLER2_LEN+1);
        memset(gbp->wo03_send_rec->wo03rec15.filler3       ,' ', R15_FILLER3_LEN+1);
        memset(gbp->wo03_send_rec->wo03rec15.filler4       ,' ', R15_FILLER4_LEN+1);


/* start filling the records */

        memset(gbp->wo03_send_rec->tmaxrec, '\0', 19200);

                strcpy(gbp->wo03_send_rec->wo03rec11.rec_type, "11");
                get_tag_data("R11_SOURCE"       , gbp->wo03_send_rec->wo03rec11.source,gbp,stdout_buffer);
                get_tag_data("R11_FDATE"        , gbp->wo03_send_rec->wo03rec11.first_date,gbp,stdout_buffer);
                get_tag_data("R11_FNAME"        , gbp->wo03_send_rec->wo03rec11.first_name,gbp,stdout_buffer);
                get_tag_data("R11_MINITIAL"     , gbp->wo03_send_rec->wo03rec11.middle_init,gbp,stdout_buffer);
                get_tag_data("R11_LNAME"        , gbp->wo03_send_rec->wo03rec11.last_name,gbp,stdout_buffer);
                get_tag_data("R11_TITLE"        , gbp->wo03_send_rec->wo03rec11.title,gbp,stdout_buffer);
                get_tag_data("R11_COMPANY_NAME" , gbp->wo03_send_rec->wo03rec11.company_name,gbp,stdout_buffer);
                get_tag_data("R11_ADDRESS_REF1" , gbp->wo03_send_rec->wo03rec11.address_ref_1,gbp,stdout_buffer);
                get_tag_data("R11_ADDRESS_REF2" , gbp->wo03_send_rec->wo03rec11.address_ref_2,gbp,stdout_buffer);
                get_tag_data("R11_STREET"       , gbp->wo03_send_rec->wo03rec11.street,gbp,stdout_buffer);
                get_tag_data("R11_CITY"         , gbp->wo03_send_rec->wo03rec11.city,gbp,stdout_buffer);
                get_tag_data("R11_STATE"        , gbp->wo03_send_rec->wo03rec11.state,gbp,stdout_buffer);
                get_tag_data("R11_ZIP_CODE"     , gbp->wo03_send_rec->wo03rec11.zip_code,gbp,stdout_buffer);
                get_tag_data("R11_PHONE"        , gbp->wo03_send_rec->wo03rec11.phone,gbp,stdout_buffer);
                get_tag_data("R11_COUNTRY_CODE" , gbp->wo03_send_rec->wo03rec11.country_code,gbp,stdout_buffer);
                get_tag_data("R11_OLD_CUST_NUM" , gbp->wo03_send_rec->wo03rec11.old_cust_num,gbp,stdout_buffer);
                get_tag_data("R11_CUST_TYPE"    , gbp->wo03_send_rec->wo03rec11.cust_type,gbp,stdout_buffer);
                get_tag_data("R11_CC_TYPE"      , gbp->wo03_send_rec->wo03rec11.cc_type,gbp,stdout_buffer);
                get_tag_data("R11_CC_NUM"       , gbp->wo03_send_rec->wo03rec11.cc_num,gbp,stdout_buffer);
                get_tag_data("R11_CC_EXP_YEAR"  , gbp->wo03_send_rec->wo03rec11.cc_exp_yy,gbp,stdout_buffer);
                get_tag_data("R11_CC_EXP_MONTH" , gbp->wo03_send_rec->wo03rec11.cc_exp_mm,gbp,stdout_buffer);
                get_tag_data("R11_ERROR_CODE"   , gbp->wo03_send_rec->wo03rec11.error_code,gbp,stdout_buffer);
                get_tag_data("R11_FAX_PHONE"    , gbp->wo03_send_rec->wo03rec11.fax_phone,gbp,stdout_buffer);
                        

                if(wo03_CatSendStr11(gbp->wo03_send_rec->tmaxrec[0], &gbp->wo03_send_rec->wo03rec11) == FAILURE)
                {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo03_send_rec);
                                return(-1);
                }
                

                strcpy(gbp->wo03_send_rec->wo03rec13.rec_type, "13");
                get_tag_data("R13_EMAIL_ADDR"   , gbp->wo03_send_rec->wo03rec13.email_addr,gbp,stdout_buffer);
                get_tag_data("R13_NET_PASSWORD" , gbp->wo03_send_rec->wo03rec13.net_password,gbp,stdout_buffer);
                get_tag_data("R13_RECAP_FLAG"   , gbp->wo03_send_rec->wo03rec13.recap_flag,gbp,stdout_buffer);

                strcpy(gbp->wo03_send_rec->wo03rec13.ip_address, r->connection->remote_ip);

                get_tag_data("R13_USER_ID"      , gbp->wo03_send_rec->wo03rec13.user_id,gbp,stdout_buffer);
                get_tag_data("R13_DEMOGRAPH"    , gbp->wo03_send_rec->wo03rec13.demograph,gbp,stdout_buffer);
                
                if(wo03_CatSendStr13(gbp->wo03_send_rec->tmaxrec[1], &gbp->wo03_send_rec->wo03rec13) == FAILURE)
                {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo03_send_rec);
                                return(-1);
                }
        
                
                strcpy(gbp->wo03_send_rec->wo03rec15.rec_type, "15");
                get_tag_data("R15_REMIT_AMT"    , gbp->wo03_send_rec->wo03rec15.remit_amt,gbp,stdout_buffer);
                get_tag_data("R15_FUTURE_DATE"  , gbp->wo03_send_rec->wo03rec15.future_date,gbp,stdout_buffer);
                get_tag_data("R15_PURCHASE_ORD" , gbp->wo03_send_rec->wo03rec15.po_number,gbp,stdout_buffer);
                get_tag_data("R15_ENTERED_DATE" , gbp->wo03_send_rec->wo03rec15.entered_date,gbp,stdout_buffer);
                get_tag_data("R15_ENTERED_TIME" , gbp->wo03_send_rec->wo03rec15.entered_time,gbp,stdout_buffer);
                get_tag_data("R15_ENTERED_BY"   , gbp->wo03_send_rec->wo03rec15.entered_by,gbp,stdout_buffer);
                get_tag_data("R15_HOLD_COMPLETE", gbp->wo03_send_rec->wo03rec15.hold_order,gbp,stdout_buffer);
                get_tag_data("R15_EXPEDITE_ORD" , gbp->wo03_send_rec->wo03rec15.expedite,gbp,stdout_buffer);
                get_tag_data("R15_NO_BACKORDER" , gbp->wo03_send_rec->wo03rec15.no_backorder,gbp,stdout_buffer);
                get_tag_data("R15_POSTPONE_ORD" , gbp->wo03_send_rec->wo03rec15.postpone_order,gbp,stdout_buffer);
                get_tag_data("R15_NO_INSURANCE" , gbp->wo03_send_rec->wo03rec15.no_insurance,gbp,stdout_buffer);
                get_tag_data("R15_CCUF_FRONT"   , gbp->wo03_send_rec->wo03rec15.cc_up_front,gbp,stdout_buffer);
                get_tag_data("R15_OVERPAYMENT"  , gbp->wo03_send_rec->wo03rec15.overpayment,gbp,stdout_buffer);
                get_tag_data("R15_STANDING_ORD" , gbp->wo03_send_rec->wo03rec15.standing_order,gbp,stdout_buffer);
                get_tag_data("R15_GENDER"       , gbp->wo03_send_rec->wo03rec15.gender,gbp,stdout_buffer);
                get_tag_data("R15_OPT_CODE"     , gbp->wo03_send_rec->wo03rec15.opt_code,gbp,stdout_buffer);
                get_tag_data("R15_SHIP_METHOD"  , gbp->wo03_send_rec->wo03rec15.ship_method,gbp,stdout_buffer);
                get_tag_data("R15_DEMOGRAPHICS" , gbp->wo03_send_rec->wo03rec15.demographic,gbp,stdout_buffer);
                get_tag_data("R15_SALES_TERR"   , gbp->wo03_send_rec->wo03rec15.sales_terr,gbp,stdout_buffer);
                get_tag_data("R15_SALES_REP"    , gbp->wo03_send_rec->wo03rec15.sales_rep,gbp,stdout_buffer);
                get_tag_data("R15_FAX_PHONE"    , gbp->wo03_send_rec->wo03rec15.fax_phone,gbp,stdout_buffer);
                get_tag_data("R15_ORDER_XREF"   , gbp->wo03_send_rec->wo03rec15.order_xref,gbp,stdout_buffer);
                get_tag_data("R15_CUST_TYPE"    , gbp->wo03_send_rec->wo03rec15.cust_type,gbp,stdout_buffer);
                get_tag_data("R15_ERROR_FLAG"   , gbp->wo03_send_rec->wo03rec15.error_flag,gbp,stdout_buffer);
                get_tag_data("R15_INSTALLMENT"  , gbp->wo03_send_rec->wo03rec15.installment,gbp,stdout_buffer);
                get_tag_data("R15_SHIP_DATE"    , gbp->wo03_send_rec->wo03rec15.ship_date,gbp,stdout_buffer);
                get_tag_data("R15_USER_HOLD_CODE", gbp->wo03_send_rec->wo03rec15.user_hold_code,gbp,stdout_buffer);
                get_tag_data("R15_BANK_ACCT"    , gbp->wo03_send_rec->wo03rec15.bank_acct,gbp,stdout_buffer);
                get_tag_data("R15_RUT_NUM"      , gbp->wo03_send_rec->wo03rec15.rut_num,gbp,stdout_buffer);
                get_tag_data("R15_CONTINUITY_FLAG", gbp->wo03_send_rec->wo03rec15.continuity_flag,gbp,stdout_buffer);
                get_tag_data("R15_BYPASS_ADV"   , gbp->wo03_send_rec->wo03rec15.bypass_adv,gbp,stdout_buffer);
                get_tag_data("R15_DISCOUNT_CODE1", gbp->wo03_send_rec->wo03rec15.disc_code1,gbp,stdout_buffer);
                get_tag_data("R15_DISCOUNT_CODE2", gbp->wo03_send_rec->wo03rec15.disc_code2,gbp,stdout_buffer);
                get_tag_data("R15_BIRTHDAY"     , gbp->wo03_send_rec->wo03rec15.birthday,gbp,stdout_buffer);

                
                if(wo03_CatSendStr15(gbp->wo03_send_rec->tmaxrec[2], &gbp->wo03_send_rec->wo03rec15) == FAILURE)
                {
                                free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
                                free(gbp->recvbufcat);
                                return(-1);
                }

        for(gbp->i = 3; gbp->i < 60; gbp->i++)
                {
                        if(wo03_CatSendStrblank(gbp->wo03_send_rec->tmaxrec[gbp->i]) == FAILURE)
                        {
                                free(gbp->sendbufcat);
                                free(gbp->recvbufcat);
                                free(gbp->wo03_send_rec);
                                return(-1);
                        }
                }


/* Create one huge string with all information to be passed */

        sprintf(gbp->sendbufcat, 
                 "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%"
                              "-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%"
                                          "-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%"
                                          "-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%"
                                          "-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%"
                                          "-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s%-320s",
                                           gbp->request_id,
                                           gbp->tran_id,
                                           gbp->company,
                                           gbp->division,
                                           gbp->uid,
                                           gbp->ip_address,
                                           " ",
                                           gbp->wo03_send_rec->tmaxrec[0 ],
                                           gbp->wo03_send_rec->tmaxrec[1 ],
                                           gbp->wo03_send_rec->tmaxrec[2 ],
                                           gbp->wo03_send_rec->tmaxrec[3 ],
                                           gbp->wo03_send_rec->tmaxrec[4 ],
                                           gbp->wo03_send_rec->tmaxrec[5 ],
                                           gbp->wo03_send_rec->tmaxrec[6 ],
                                           gbp->wo03_send_rec->tmaxrec[7 ],
                                           gbp->wo03_send_rec->tmaxrec[8 ],
                                           gbp->wo03_send_rec->tmaxrec[9 ],
                                           gbp->wo03_send_rec->tmaxrec[10],
                                           gbp->wo03_send_rec->tmaxrec[11],
                                           gbp->wo03_send_rec->tmaxrec[12],
                                           gbp->wo03_send_rec->tmaxrec[13],
                                           gbp->wo03_send_rec->tmaxrec[14],
                                           gbp->wo03_send_rec->tmaxrec[15],
                                           gbp->wo03_send_rec->tmaxrec[16],
                                           gbp->wo03_send_rec->tmaxrec[17],
                                           gbp->wo03_send_rec->tmaxrec[18],
                                           gbp->wo03_send_rec->tmaxrec[19],
                                           gbp->wo03_send_rec->tmaxrec[20],
                                           gbp->wo03_send_rec->tmaxrec[21],
                                           gbp->wo03_send_rec->tmaxrec[22],
                                           gbp->wo03_send_rec->tmaxrec[23],
                                           gbp->wo03_send_rec->tmaxrec[24],
                                           gbp->wo03_send_rec->tmaxrec[25],
                                           gbp->wo03_send_rec->tmaxrec[26],
                                           gbp->wo03_send_rec->tmaxrec[27],
                                           gbp->wo03_send_rec->tmaxrec[28],
                                           gbp->wo03_send_rec->tmaxrec[29],
                                           gbp->wo03_send_rec->tmaxrec[30],
                                           gbp->wo03_send_rec->tmaxrec[31],
                                           gbp->wo03_send_rec->tmaxrec[32],
                                           gbp->wo03_send_rec->tmaxrec[33],
                                           gbp->wo03_send_rec->tmaxrec[34],
                                           gbp->wo03_send_rec->tmaxrec[35],
                                           gbp->wo03_send_rec->tmaxrec[36],
                                           gbp->wo03_send_rec->tmaxrec[37],
                                           gbp->wo03_send_rec->tmaxrec[38],
                                           gbp->wo03_send_rec->tmaxrec[39],
                                           gbp->wo03_send_rec->tmaxrec[40],
                                           gbp->wo03_send_rec->tmaxrec[41],
                                           gbp->wo03_send_rec->tmaxrec[42],
                                           gbp->wo03_send_rec->tmaxrec[43],
                                           gbp->wo03_send_rec->tmaxrec[44],
                                           gbp->wo03_send_rec->tmaxrec[45],
                                           gbp->wo03_send_rec->tmaxrec[46],
                                           gbp->wo03_send_rec->tmaxrec[47],
                                           gbp->wo03_send_rec->tmaxrec[48],
                                           gbp->wo03_send_rec->tmaxrec[49],
                                           gbp->wo03_send_rec->tmaxrec[50],
                                           gbp->wo03_send_rec->tmaxrec[51],
                                           gbp->wo03_send_rec->tmaxrec[52],
                                           gbp->wo03_send_rec->tmaxrec[53],
                                           gbp->wo03_send_rec->tmaxrec[54],
                                           gbp->wo03_send_rec->tmaxrec[55],
                                           gbp->wo03_send_rec->tmaxrec[56],
                                           gbp->wo03_send_rec->tmaxrec[57],
                                           gbp->wo03_send_rec->tmaxrec[58],
                                           gbp->wo03_send_rec->tmaxrec[59]);


if(strlen(gbp->sendbufcat) == wo03_LAN_SEND_BUF_LEN)
{
        if((gbp->sock = sga_connect(gbp->hphost, gbp->tmaxport, gbp->webexec, &gbp->rc,r,gbp)) == INVALID_SOCKET)
        {
                XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
                free(gbp->recvbufcat);
                return(-1);
        }

        if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, wo03_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
                free(gbp->recvbufcat);
                return(-1);
        }

        if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, wo03_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
        {
                XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
                free(gbp->recvbufcat);
                return(-1);
        }

        /* Do an additional send and recieve for confirmation  */
        if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
        {
                XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
                free(gbp->recvbufcat);
                return(-1);
        }
/*
        gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
        sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

        wo03_ParceRecvStr(&gbp->wo03recvbuf, gbp->recvbufcat,r,gbp);
        free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
        free(gbp->recvbufcat);

}
else
{
        XML_Error("sendbufcat","Failed filling the send buffer","communications","-1",r,gbp);
        free(gbp->sendbufcat);
                                free(gbp->wo03_send_rec);
        free(gbp->recvbufcat);
        return(-1);
}

    return 0;
                                           
}
        
/******************************************************************/
/*                                                                                                                                */
/*     Create the send string from our current data               */
/*                                                                                                                                */
/******************************************************************/
int     wo03_CatSendStrblank(char *sz_tmaxrec)
{
        memset(sz_tmaxrec, ' ', 320);
        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}


int     wo03_CatSendStr11(char *sz_tmaxrec, wo03_rec11 *ptr_rec11)
{
        memset(sz_tmaxrec, ' ', 320);
                sprintf(sz_tmaxrec,
                          "%-2.2s%-8.8s%-16.16s%-8.8s%-16.16s%-1.1s%-16.16s%-2.2s%-"
                                  "30.30s%-30.30s%-30.30s%-30.30s%-30.30s%-2.2s%-14.14s%-"
                                  "14.14s%-4.4s%-24.24s%-2.2s%-2.2s%-20.20s%-2.2s%-2.2s%-"
                                  "1.1s%-14.14s",

                                  ptr_rec11->rec_type,
                                  ptr_rec11->filler1,
                                  ptr_rec11->source,
                                  ptr_rec11->first_date,
                                  ptr_rec11->first_name,
                                  ptr_rec11->middle_init,
                                  ptr_rec11->last_name,
                                  ptr_rec11->title,
                                  ptr_rec11->company_name,
                                  ptr_rec11->address_ref_1,
                                  ptr_rec11->address_ref_2,
                                  ptr_rec11->street,
                                  ptr_rec11->city,
                                  ptr_rec11->state,
                                  ptr_rec11->zip_code,
                                  ptr_rec11->phone,
                                  ptr_rec11->country_code,
                                  ptr_rec11->old_cust_num,
                                  ptr_rec11->cust_type,
                                  ptr_rec11->cc_type,
                                  ptr_rec11->cc_num,
                                  ptr_rec11->cc_exp_yy,
                                  ptr_rec11->cc_exp_mm,
                                  ptr_rec11->error_code,
                                  ptr_rec11->fax_phone);

        
        sz_tmaxrec[320] = '\0';
        
        if(strlen(sz_tmaxrec) == 320)
                return (SUCCESS);
        
        else
                return (FAILURE);
}


int     wo03_CatSendStr13(char *sz_tmaxrec, wo03_rec13 *ptr_rec13)
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

int     wo03_CatSendStr15(char *sz_tmaxrec, wo03_rec15 *ptr_rec15)
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


int wo03_ParceRecvStr(wo03_recv *ptr_recvbuf, char *rtnbuf, request_rec *r, struct global_struct *gbp)
{
        gbp->count = 0;
   
//      ap_rprintf(r,"%s\n", rtnbuf);
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/wo04.xsl\"?>\n");  

#endif
/*
        ap_rprintf(r,"<!DOCTYPE %s04 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                        wo_tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s04.dtd\">\n", url_tag, wo_tag);
*/
        ap_rprintf(r,"<!-- CATALOG REQUEST -->\n");

        ap_rprintf(r,"%s04 %s\"wo04\">\n", gbp->wo_btag, gbp->bitag);
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
/*
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
    ap_rprintf(r,"      <INSURANCE_DOL>%s</INSURANCE_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->insurance_dol));
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
                        ap_rprintf(r,"      <SHIPTO_DETAILS>");
                        ap_rprintf(r,"		<SHIPTO_SHIPTO_NUM>%s</SHIPTO_SHIPTO_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_st_num));
                        ap_rprintf(r,"		<SHIPTO_SM_CODE>%s</SHIPTO_SM_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_sm_code));
                        ap_rprintf(r,"		<SHIPTO_SM_DESC>%s</SHIPTO_SM_DESC>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_sm_desc));
                        ap_rprintf(r,"		<SHIPTO_SM_CHANGE>%s</SHIPTO_SM_CHANGE>\n", handle_special_chars(gbp,ptr_recvbuf->shipto[gbp->i].shipto_sm_change));
                        ap_rprintf(r,"      </SHIPTO_DETAILS>");
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
*/
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);  
        ap_rprintf(r,"%s04>\n", gbp->wo_betag);  

    return(0);
}

