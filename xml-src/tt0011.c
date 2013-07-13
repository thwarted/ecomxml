/* tt0011.c  Source Input */
/* modified for phase 2 - JL */
#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>


#include "sgsock.h"
#include "funcs.h"
#include "externs.h"
#include "http_protocol.h"


int		tt0011_CatSendStr(struct global_struct *, char *, tt0011_st_send *);
int		tt0011_ParceSourceReply(tt0011_st_recv *, char *, request_rec *,
							  struct global_struct *);
char	*tt0011_getsource(char *, struct global_struct *);
int		tt0011_lt_process(request_rec *, struct global_struct *, char *);



int tt0011_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)			/* Get the current working path */
		return(-1);

    GetInf(gbp);							/* Read info from INF file */


	return(0);
}


int tt0011_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((tt0011_lt_process(r,gbp,stdout_buffer)) == -1)
		return(-1);

    return(0);
}

int tt0011_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{

	gbp->sendbuf0011 = (tt0011_st_send *)malloc(sizeof(tt0011_st_send));
	gbp->recvbuf0011 = (tt0011_st_recv *)malloc(sizeof(tt0011_st_recv));

	gbp->sendbufcat = malloc(tt0011_LAN_SEND_BUF_LEN + 1);
	if (gbp->sendbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

	gbp->recvbufcat = malloc(tt0011_LAN_RECV_BUF_LEN + 1);
	if (gbp->recvbufcat == NULL)
	{
		ap_rprintf(r, "Isufficient memory available\n");
		exit(1);
	}

    memset(gbp->sendbufcat, '\0', tt0011_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', tt0011_LAN_RECV_BUF_LEN + 1);

    memset(gbp->sendbuf0011,  '\0',  sizeof(tt0011_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->sendbuf0011->request_id, "XML");
    strcpy(gbp->sendbuf0011->record_id, "0011");

	strcpy(gbp->sendbuf0011->ip_address, r->connection->remote_ip);

    memset(gbp->tempsrc,'\0', 80);
    memset(gbp->srctype,'\0', 2);

    get_tag_data("COMPANY", gbp->sendbuf0011->company,gbp,stdout_buffer);
	get_tag_data("DIVISION", gbp->sendbuf0011->division,gbp,stdout_buffer);
	get_tag_data("UID", gbp->sendbuf0011->userid,gbp,stdout_buffer);

    // P132546-01: ACTION_FLAG can have the following values: "U", "D", "Q"
    // Ravi - Begin

	get_tag_data("ACTION_FLAG", gbp->sendbuf0011->action_flag,gbp,stdout_buffer);

	// strcpy(gbp->sendbuf0011->action_flag, "U");

    // Ravi - End

	get_tag_data("SRCT", gbp->srctype,gbp,stdout_buffer);
	get_tag_data("SRCC", gbp->tempsrc,gbp,stdout_buffer);

    // If source-type = "L", get source from file
    if (gbp->srctype[0]=='L') 
        sprintf(gbp->sendbuf0011->source_code,"%s", tt0011_getsource(gbp->tempsrc,gbp));
    else 
        memcpy(gbp->sendbuf0011->source_code, gbp->tempsrc + 0, 16);


    if((tt0011_CatSendStr(gbp, gbp->sendbufcat, gbp->sendbuf0011)) == tt0011_LAN_SEND_BUF_LEN)
    {

		if((gbp->sock = sga_connect(gbp->hphost, gbp->webport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
		{
			XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0011);
			free (gbp->recvbuf0011);
			return(-1);
		}

		if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, tt0011_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0011);
			free (gbp->recvbuf0011);
			return(-1);
		}

		if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, tt0011_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0011);
			free (gbp->recvbuf0011);
			return(-1);
		}


		if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
		{
			XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
			free (gbp->sendbufcat);
			free (gbp->recvbufcat);
			free (gbp->sendbuf0011);
			free (gbp->recvbuf0011);
			return(-1);
		}
/*
		gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
		sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

		tt0011_ParceSourceReply(gbp->recvbuf0011, gbp->recvbufcat,r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0011);
		free (gbp->recvbuf0011);
    }
    else
    {
		XML_Error("tt0011_CatSendSource","Failed filling the send buffer","communications","-1",r,gbp);
		free (gbp->sendbufcat);
		free (gbp->recvbufcat);
		free (gbp->sendbuf0011);
		free (gbp->recvbuf0011);
		return(-1);
    }

    return(0);
}

int tt0011_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, tt0011_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s%-16.16s%-1.1s",
            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
			ptr_sendbuf->filler,
            ptr_sendbuf->source_code,
            ptr_sendbuf->action_flag);

    return(gbp->j);
}


int tt0011_ParceSourceReply(tt0011_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
						  struct global_struct *gbp)
{

    // Place the individual variables into target fields
	gbp->count = 0;

	ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

	ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/tt0012.xsl\"?>\n");

#endif
/*
		ap_rprintf(r,"<!DOCTYPE %s0012 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

	ap_rprintf(r,"                     \"%s%s0012.dtd\">\n", url_tag, tt_tag);
*/
	ap_rprintf(r,"%s0012 %s\"tt0012\">\n", gbp->tt_btag, gbp->bitag);
	ap_rprintf(r,"%s\n", sga_message);
	ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id, '\0', tt0011_REQ_ID_LEN + 1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, tt0011_REQ_ID_LEN);
	ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += tt0011_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id, '\0', tt0011_REC_ID_LEN + 1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, tt0011_REC_ID_LEN);
    gbp->count += tt0011_REC_ID_LEN;

	memset(ptr_recvbuf->userid, '\0', tt0011_USER_ID_LEN + 1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, tt0011_USER_ID_LEN);
	ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += tt0011_USER_ID_LEN;

    memset(ptr_recvbuf->success,'\0', tt0011_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success, sz_recvbufcat + gbp->count, tt0011_SUCCESS_FLAG_LEN);
	ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success));
    gbp->count += tt0011_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', tt0011_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, tt0011_ERR_LEN);
	ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += tt0011_ERR_LEN;

	gbp->count += tt0011_SEND_FILLER_LEN;


    memset(ptr_recvbuf->valid_src, '\0', tt0011_VAL_SRC_LEN + 1);
    memcpy(ptr_recvbuf->valid_src, sz_recvbufcat + gbp->count, tt0011_VAL_SRC_LEN);
	ap_rprintf(r,"		<VALID_SOURCE>%s</VALID_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->valid_src));
    gbp->count += tt0011_VAL_SRC_LEN;

    memset(ptr_recvbuf->source_code, '\0', tt0011_SRC_LEN + 1);
    memcpy(ptr_recvbuf->source_code, sz_recvbufcat + gbp->count, tt0011_SRC_LEN);
	ap_rprintf(r,"		<SOURCE_CODE>%s</SOURCE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->source_code));
    gbp->count += tt0011_SRC_LEN;

	ap_rprintf(r,"	%s>\n", gbp->metag);
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);
	ap_rprintf(r,"%s0012>\n", gbp->tt_betag);

    return 0;
}


char *tt0011_getsource(char *source, struct global_struct *gbp)
{
    FILE *srcfile;                       // Source Code X-Ref

    char filename[256];
    char src[17];
    char srctext[80];
    char testtext[80];
    char *line;

    line = malloc(100);

    sprintf(filename,"%s\\custsrc.inf",gbp->rootpath);

    //PrintHeader();
    if( (srcfile = fopen(filename,"r" )) == NULL ) 
    {
        //ap_rprintf(r,"didnt find file!\n");
        free(line);
        return "  ";
    }

    sprintf(testtext,"%s",source);

    memset(line, '\0', 100);
    fgets(line, 100, srcfile);

    do {

        memset(srctext,'\0',80);
        memset(src,'\0',17);
        memcpy(src, line + 0, 16);
        memcpy(srctext, line + 16, strlen(source));

        //ap_rprintf(r,"---------------------\n<BR>");
        //ap_rprintf(r,"Looking For: '%s'<BR>\n",testtext);
        //ap_rprintf(r,"This test  : '%s'<BR>\n",srctext);


        if (strcmp(srctext,testtext)==0) 
		{
            //ap_rprintf(r,"Match!<BR>\n");
            strcpy(source,src);
            fclose(srcfile);
            free(line);
            return source;
        }

        if (fgets(line,100,srcfile) == NULL) 
		{
            fclose(srcfile);
            free(line);
            return("  ");
        }

    } while( line != NULL );

	return "  ";
}







    


