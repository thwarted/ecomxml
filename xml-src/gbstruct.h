#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#define _MAX_PATH PATH_MAX
#endif
#include "xmlparse.h"

#include "wo01_defs.h"
// #include "wo03_defs.h"

#include "tt0001_defs.h"
#include "tt0003_defs.h"
#include "tt0005_defs.h"
#include "tt0007_defs.h"
#include "tt0009_defs.h"
#include "tt0011_defs.h"
#include "tt0013_defs.h"
#include "tt0015_defs.h"
#include "tt0017_defs.h"
#include "tt0019_defs.h"
#include "tt0021_defs.h"
#include "tt0023_defs.h"
#include "tt0025_defs.h"
#include "tt0027_defs.h"
#include "tt0029_defs.h"
#include "tt0031_defs.h"
#include "tt0033_defs.h"
#include "tt0035_defs.h"
#include "tt0037_defs.h"
#include "tt0039_defs.h"
#include "tt0041_defs.h"
#include "tt0043_defs.h"
#include "tt0045_defs.h"
#include "tt0047_defs.h"
#include "tt0049_defs.h"
#include "tt0051_defs.h"
#include "tt0053_defs.h"
#include "tt0055_defs.h"
#include "tt0057_defs.h"
#include "tt0059_defs.h"
#include "tt0061_defs.h"
#include "tt0063_defs.h"
#include "tt0065_defs.h"
#include "tt0067_defs.h"
#include "tt0069_defs.h"
#include "tt0071_defs.h"
#include "tt0073_defs.h"
#include "tt0075_defs.h"
#include "tt0077_defs.h"
#include "tt0079_defs.h"
#include "tt0081_defs.h"
#include "tt0083_defs.h"
#include "tt0085_defs.h"
#include "tt0087_defs.h"
#include "tt0089_defs.h"
#include "tt0091_defs.h"
#include "tt0093_defs.h"
#include "tt0095_defs.h"
#include "tt0103_defs.h"
#include "tt0105_defs.h"
#include "tt0107_defs.h"
#include "tt0109_defs.h"
#include "tt0111_defs.h"
#include "tt0113_defs.h"
#include "tt0115_defs.h"
#include "tt0117_defs.h"
#include "tt0119_defs.h"
#include "tt0121_defs.h"
#include "tt0123_defs.h"
#include "tt0125_defs.h"
#include "tt0127_defs.h"
#include "tt0129_defs.h"
#include "tt0131_defs.h"
#include "tt0133_defs.h"
#include "tt0135_defs.h"
#include "tt0137_defs.h"

// bridal registry transactions

#include "ft0001_defs.h"
#include "ft0002_defs.h"
#include "ft0006_defs.h"
#include "ft0007_defs.h"
#include "ft0008_defs.h"
#include "ft0010_defs.h"
#include "ft0020_defs.h"
#include "ft0022_defs.h"
#include "ft0023_defs.h"
#include "ft0024_defs.h"
#include "ft0025_defs.h"
#include "ft0030_defs.h"
#include "ft0037_defs.h"
#include "ft0070_defs.h"
#include "ft0071_defs.h"
#include "ft0073_defs.h"
#include "ft0074_defs.h"
#include "ft0075_defs.h"
#include "ft0089_defs.h"
#include "ft0127_defs.h"

// wishlist transactions

#include "wl0003_defs.h"
#include "wl0005_defs.h"
#include "wl0006_defs.h"
#include "wl0007_defs.h"
#include "wl0008_defs.h"

/* This is where we define our global container for all transaction
   types... a structure that allows any transaction type to be held
   and with sufficient data to allow the automation of any required
   processing of the data, and either build the MACS sentences from
   XML or extract data from the MACS sentences into XML structures. 
*/

/* MAXIMUM TRANSACTION NUMBER */
#define MAXTRANSID              100     /* Set to the highest number transaction we currently support*/

#define MAXSYMBOLS              100     /* The maximum number of symbols to parse thru ANY xml 
                                                                transaction... probably more than enough                 */
#define MAXSYMBOLLENGTH 1000    /* The maximum symbol length...                              */

#define MAXDATALENGTH   50 /* 700 150 /*80*/


#define TAG_BUFFER_LENGTH 80

#define  MAXSYMBOL               26
#define  MAXSYMBOLLEN        5
#define  MAXDATALEN                  10

#define  TAG_SIZE            50
#define INPUT_BUFFER_SIZE    150000

/* tags used in wo01 */
#define  ADDRESS_LEN                60
#define  R11_REFERENCE_LEN          14
#define  R15_REFERENCE_LEN          100

/* THE REQUEST STRUCTURE*/
typedef struct {

        int tttype_id;          /* the transaction type id # */
        char *sgadata;          /* an array of pointers to possible requests */
        char *requestdata;      /* an array of pointers to possible request data */

} req_xml;



struct global_struct
{
        /* This structure has been created to accommodate global 
           variables of the previous "rxpapp" utility.
        */
        char stdout_buffer[INPUT_BUFFER_SIZE];
//      char *stdout_buffer;
        int     XML_ERROR_HEADER;
        char rootpath[_MAX_PATH];               
        char company[3];                        
        char division[3];
        int errstatus; 

        int k,l;
        char ch, tmp[2];

        XML_Parser Xp;
        char linenumber[100];

    int start_len, got_char;
        int totleft;
        int thissend;
        int bytes_sent;

    int req_len;
    char *buffer;
        char tttype_buf[TAG_BUFFER_LENGTH];
        char *begin_tag;
        char *begin_tag_id;
        char *end_tag;
        char *begin_sgadata_tag;
        char *end_sgadata_tag;
        char *begin_requestdata_tag;
        char *end_requestdata_tag;
        char *settag;
        int ret;
        char *buf;
        char sz_tag_name1[TAG_SIZE];
        char *pStart;
        char *open_tag;
        char *close_tag;
    char    confirmbuf[5 + 1];
    char    tagname[TAG_SIZE];

        char *sendbufcat;
        char *recvbufcat;

        char *first;
        char *last;
        char    uid[UID_LEN+1];
        char    ip_address[IP_ADDRESS_LEN+1];
        char    request_id[REQ_ID_LEN + 1];
        char    tran_id[TRAN_ID_LEN+1];

        int data_start_pos;
        int data_end_pos;

        int count;
        int where;

        char    srctype[2];
    char    tempsrc[80];

        int a; 
		int x, y; /* variables used in remove_comma() */
		char *tempreq; /* variables used in remove_comma() */
        char *line;
        char filename[1024];                    /* built filename */
        char imagepath[_MAX_PATH];              /* path where the images are */
        char logfile[30];                                               /*name of order logging file*/

        FILE *infile;                           /* Profile file handle */
        int tt_ok;
        int success;

        /* INF File information */
        char webexec[80];                       
    char tmaxexec[80];                     
    char hphost[80];
	char wlport[80];
    char fvfport[10];                       
    char webport[10];                       
    char tmaxport[10];                     
    char reqfile[30];                      
    char imageloc[60]; 
        char out_string[5000];
        char *p1;
        char *p2;
        char *p3;

    char    totbuf[201];

        int     this_transaction;
        int useget;

        char reqdata_tag[MAXSYMBOL][MAXSYMBOLLEN];
        char reqdata_data[MAXSYMBOL][MAXDATALEN];
//      char *reqdata = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
//      char reqdata[51];
        char *reqdata;
    int rv,i,j, offset1;

char tt_btag[TAG_BUFFER_LENGTH];
char tt_betag[TAG_BUFFER_LENGTH];

char wo_btag[TAG_BUFFER_LENGTH];
char wo_betag[TAG_BUFFER_LENGTH];

char ft_btag[TAG_BUFFER_LENGTH];
char ft_betag[TAG_BUFFER_LENGTH];

char wl_btag[TAG_BUFFER_LENGTH];
char wl_betag[TAG_BUFFER_LENGTH];

char tag[5];

       int      rc;
 
//      char btag[TAG_BUFFER_LENGTH];
    char bitag[TAG_BUFFER_LENGTH];
//    char betag[TAG_BUFFER_LENGTH];
    char rtag[TAG_BUFFER_LENGTH];               /*global_struct*/
    char ritag[TAG_BUFFER_LENGTH];
    char retag[TAG_BUFFER_LENGTH];
    char otag[TAG_BUFFER_LENGTH];
    char oitag[TAG_BUFFER_LENGTH];
    char oetag[TAG_BUFFER_LENGTH];

    char mtag[TAG_BUFFER_LENGTH];
    char mitag[TAG_BUFFER_LENGTH];
    char metag[TAG_BUFFER_LENGTH];

    char rstag[TAG_BUFFER_LENGTH];
    char rsitag[TAG_BUFFER_LENGTH];
    char rsetag[TAG_BUFFER_LENGTH];
    int requestdatacount ;
        int sgadatacount;
    char wsecode[180];

        /* Some static arrays to hold the actual request/data strings*/
        char global_sgadata[MAXSYMBOLS][MAXSYMBOLLENGTH];
        char global_requestdata[MAXSYMBOLS][MAXSYMBOLLENGTH];

        char global_data[MAXDATALENGTH];

        char global_sgadata_data[MAXSYMBOLS][MAXDATALENGTH];
        char global_requestdata_data[MAXSYMBOLS][MAXDATALENGTH];

        req_xml rqxml;

#ifdef WIN32
        HANDLE hInst;
        SOCKET sock;
    PHOSTENT phe;
    PSERVENT pse;
#else
        int sock;
    struct hostent *phe;
    struct servent *pse;
#endif
        wo01_send_records *wo01_send_rec;
        //wo03_send_records *wo03_send_rec;

        wo01_recv wo01recvbuf;
        //wo03_recv wo03recvbuf;

        int max_record;

        int R16_count;
        int R18_count;
        int R21_count;
        int R30_count;
        int R31_count;
        int R33_count;
        int R40_count;
        int R41_count;
        int R42_count;
		int R43_count;

		/* tags used in wo01 */
		char address[ADDRESS_LEN+1];
		char reference_11[R11_REFERENCE_LEN+1];
		char reference_15[R15_REFERENCE_LEN+1];


    tt0001_st_send *sendbuf0001;
    tt0001_st_recv *recvbuf0001;

    tt0003_st_send *sendbuf0003;
    tt0003_st_recv *recvbuf0003;

    tt0005_st_send *sendbuf0005;
    tt0005_st_recv *recvbuf0005;

    tt0007_st_send *sendbuf0007;
    tt0007_st_recv *recvbuf0007;

    tt0009_st_send *sendbuf0009;
    tt0009_st_recv *recvbuf0009;

    tt0011_st_send *sendbuf0011;
    tt0011_st_recv *recvbuf0011;

    tt0013_st_send *sendbuf0013;
    tt0013_st_recv *recvbuf0013;

    tt0015_st_send *sendbuf0015;
    tt0015_st_recv *recvbuf0015;

    tt0017_st_send *sendbuf0017;
    tt0017_st_recv *recvbuf0017;

    tt0019_st_send *sendbuf0019;
    tt0019_st_recv *recvbuf0019;

    tt0021_st_send *sendbuf0021;
    tt0021_st_recv *recvbuf0021;

    tt0023_st_send *sendbuf0023;
    tt0023_st_recv *recvbuf0023;

    tt0025_st_send *sendbuf0025;
    tt0025_st_recv *recvbuf0025;

    tt0027_st_send *sendbuf0027;
    tt0027_st_recv *recvbuf0027;

    tt0029_st_send *sendbuf0029;
    tt0029_st_recv *recvbuf0029;

    tt0031_st_send *sendbuf0031;
    tt0031_st_recv *recvbuf0031;

    tt0033_st_send *sendbuf0033;
    tt0033_st_recv *recvbuf0033;

    tt0035_st_send *sendbuf0035;
    tt0035_st_recv *recvbuf0035;

    tt0037_st_send *sendbuf0037;
    tt0037_st_recv *recvbuf0037;

    tt0039_st_send *sendbuf0039;
    tt0039_st_recv *recvbuf0039;

    tt0041_st_send *sendbuf0041;
    tt0041_st_recv *recvbuf0041;

    tt0043_st_send *sendbuf0043;
    tt0043_st_recv *recvbuf0043;

    tt0045_st_send *sendbuf0045;
    tt0045_st_recv *recvbuf0045;

    tt0047_st_send *sendbuf0047;
    tt0047_st_recv *recvbuf0047;

    tt0049_st_send *sendbuf0049;
    tt0049_st_recv *recvbuf0049;

    tt0051_st_send *sendbuf0051;
    tt0051_st_recv *recvbuf0051;

    tt0053_st_send *sendbuf0053;
    tt0053_st_recv *recvbuf0053;

    tt0055_st_send *sendbuf0055;
    tt0055_st_recv *recvbuf0055;

    tt0057_st_send *sendbuf0057;
    tt0057_st_recv *recvbuf0057;

    tt0059_st_send *sendbuf0059;
    tt0059_st_recv *recvbuf0059;

    tt0061_st_send *sendbuf0061;
    tt0061_st_recv *recvbuf0061;

    tt0063_st_send *sendbuf0063;
    tt0063_st_recv *recvbuf0063;

    tt0065_st_send *sendbuf0065;
    tt0065_st_recv *recvbuf0065;

    tt0067_st_send *sendbuf0067;
    tt0067_st_recv *recvbuf0067;

    tt0069_st_send *sendbuf0069;
    tt0069_st_recv *recvbuf0069;

    tt0071_st_send *sendbuf0071;
    tt0071_st_recv *recvbuf0071;

    tt0073_st_send *sendbuf0073;
    tt0073_st_recv *recvbuf0073;

    tt0075_st_send *sendbuf0075;
    tt0075_st_recv *recvbuf0075;

    tt0077_st_send *sendbuf0077;
    tt0077_st_recv *recvbuf0077;

    tt0079_st_send *sendbuf0079;
    tt0079_st_recv *recvbuf0079;

    tt0081_st_send *sendbuf0081;
    tt0081_st_recv *recvbuf0081;

    tt0083_st_send *sendbuf0083;
    tt0083_st_recv *recvbuf0083;

    tt0085_st_send *sendbuf0085;
    tt0085_st_recv *recvbuf0085;

    tt0087_st_send *sendbuf0087;
    tt0087_st_recv *recvbuf0087;

    tt0089_st_send *sendbuf0089;
    tt0089_st_recv *recvbuf0089;

    tt0091_st_send *sendbuf0091;
    tt0091_st_recv *recvbuf0091;

    tt0093_st_send *sendbuf0093;
    tt0093_st_recv *recvbuf0093;

    tt0095_st_send *sendbuf0095;
    tt0095_st_recv *recvbuf0095;

    tt0103_st_send *sendbuf0103;
    tt0103_st_recv *recvbuf0103;

    tt0105_st_send *sendbuf0105;
    tt0105_st_recv *recvbuf0105;

    tt0107_st_send *sendbuf0107;
    tt0107_st_recv *recvbuf0107;

    tt0109_st_send *sendbuf0109;
    tt0109_st_recv *recvbuf0109;

    tt0111_st_send *sendbuf0111;
    tt0111_st_recv *recvbuf0111;

    tt0113_st_send *sendbuf0113;
    tt0113_st_recv *recvbuf0113;

    tt0115_st_send *sendbuf0115;
    tt0115_st_recv *recvbuf0115;

    tt0117_st_send *sendbuf0117;
    tt0117_st_recv *recvbuf0117;

    tt0119_st_send *sendbuf0119;
    tt0119_st_recv *recvbuf0119;

    tt0121_st_send *sendbuf0121;
    tt0121_st_recv *recvbuf0121;

    tt0123_st_send *sendbuf0123;
    tt0123_st_recv *recvbuf0123;

    tt0125_st_send *sendbuf0125;
    tt0125_st_recv *recvbuf0125;

    tt0127_st_send *sendbuf0127;
    tt0127_st_recv *recvbuf0127;

    tt0129_st_send *sendbuf0129;
    tt0129_st_recv *recvbuf0129;

    tt0131_st_send *sendbuf0131;
    tt0131_st_recv *recvbuf0131;

    tt0133_st_send *sendbuf0133;
    tt0133_st_recv *recvbuf0133;

    tt0135_st_send *sendbuf0135;
    tt0135_st_recv *recvbuf0135;

    tt0137_st_send *sendbuf0137;
    tt0137_st_recv *recvbuf0137;

	// bridal registry transactions

    ft0001_st_send *ftsendbuf0001;
    ft0001_st_recv *ftrecvbuf0001;

    ft0002_st_send *ftsendbuf0002;
    ft0002_st_recv *ftrecvbuf0002;

    ft0006_st_send *ftsendbuf0006;
    ft0006_st_recv *ftrecvbuf0006;

    ft0007_st_send *ftsendbuf0007;
    ft0007_st_recv *ftrecvbuf0007;

    ft0008_st_send *ftsendbuf0008;
    ft0008_st_recv *ftrecvbuf0008;

    ft0010_st_send *ftsendbuf0010;
    ft0010_st_recv *ftrecvbuf0010;

    ft0020_st_send *ftsendbuf0020;
    ft0020_st_recv *ftrecvbuf0020;

    ft0022_st_send *ftsendbuf0022;
    ft0022_st_recv *ftrecvbuf0022;

    ft0023_st_send *ftsendbuf0023;
    ft0023_st_recv *ftrecvbuf0023;

    ft0024_st_send *ftsendbuf0024;
    ft0024_st_recv *ftrecvbuf0024;

    ft0025_st_send *ftsendbuf0025;
    ft0025_st_recv *ftrecvbuf0025;

    ft0030_st_send *ftsendbuf0030;
    ft0030_st_recv *ftrecvbuf0030;

    ft0037_st_send *ftsendbuf0037;
    ft0037_st_recv *ftrecvbuf0037;

    ft0070_st_send *ftsendbuf0070;
    ft0070_st_recv *ftrecvbuf0070;

    ft0071_st_send *ftsendbuf0071;
    ft0071_st_recv *ftrecvbuf0071;

    ft0073_st_send *ftsendbuf0073;
    ft0073_st_recv *ftrecvbuf0073;

    ft0074_st_send *ftsendbuf0074;
    ft0074_st_recv *ftrecvbuf0074;

    ft0075_st_send *ftsendbuf0075;
    ft0075_st_recv *ftrecvbuf0075;

    ft0089_st_send *ftsendbuf0089;
    ft0089_st_recv *ftrecvbuf0089;

    ft0127_st_send *ftsendbuf0127;
    ft0127_st_recv *ftrecvbuf0127;

// wishlist transactions

    wl0003_st_send *wlsendbuf0003;
    wl0003_st_recv *wlrecvbuf0003;

    wl0005_st_send *wlsendbuf0005;
    wl0005_st_recv *wlrecvbuf0005;

    wl0006_st_send *wlsendbuf0006;
    wl0006_st_recv *wlrecvbuf0006;

    wl0007_st_send *wlsendbuf0007;
    wl0007_st_recv *wlrecvbuf0007;

    wl0008_st_send *wlsendbuf0008;
    wl0008_st_recv *wlrecvbuf0008;

};
