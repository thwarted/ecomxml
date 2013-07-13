
/* xml.c  - the common code and routines */
/* This is the XML to MACS code for SGA/WebOrder         */
/*                                                       */

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#endif

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <malloc.h>


#include "funcs.h"
#include "externs.h"
#include "httpd.h"
#include "http_protocol.h"

#ifdef LINUX
#include <unistd.h>
#include <linux/limits.h>

#define  _MAX_PATH  PATH_MAX 
#define _getcwd getcwd

#endif
#ifdef SOLARIS2
#include <unistd.h>
#include <limits.h>

#define  _MAX_PATH  PATH_MAX 
#define _getcwd getcwd

#endif


/*************************************************************************/
/*                                                                       */
/* Profile.H                                                             */
/*                                                                       */
/*                                                                       */
/* Special Header information for profile processing.                    */
/*                                                                       */
/*************************************************************************/


char reqstr[3072];                      /* POST request string */
char suffix[03];                        /* Profile Suffix # */
char writedata[1024];                   /* data to put in file */

/* Debug vars */

int debugmode;                          /* Debug Mode.  1 = on */
char theuserid[17];                     /* User ID, if one exists */


/* From parser.c */

#define  ISDIGIT(c)   ( c>='0' && c<='9' ) ? 1 : 0
#define  ISALPHA(c)   ( c>='a' && c<='z' )|( c>='A' && c<='Z' ) ? 1 : 0
#define  TOLOWER(c)   ( c>='A' && c<='Z' ) ? (c + 32) : c


char	*rtrim(char *, int, struct global_struct *);
char    *handle_special_chars(struct global_struct *, char *);
int		getpath(struct global_struct *);
//void 	GetTmpPath();						
/* FROM SGTR0101 -> per rich */
void	GetInf(struct global_struct *);
//int		isempty(char *, int);

int		handle_sga_xml_data(char *, request_rec *, struct global_struct *);
int		setup_transaction_type(int, request_rec *, struct global_struct *, char *);
int		fill_working_struct(struct global_struct *, char *);
int		initialize_tag_data(struct global_struct *);
int		XML_Error(char *, char *, char *, char *, request_rec *, struct global_struct *);
void	XML_Error_Trailer(request_rec *, struct global_struct *);
void	reparse_customer_data(request_rec *, struct global_struct *);
int		XML_Expat_Error(char *, char *, char *, char *, request_rec *, struct global_struct *);

int     get_tag_from_seq(int, char *, char *, struct global_struct *);
void    set_tag_name(char *, char *, int);
int     get_tag_data(char *, char *, struct global_struct *, char *);

char	*xml_time(void);



/* ********  DO NOT MESS WITH THESE AT ALL!  ******* */

/* DO NOT MAKE CHANGES HERE UNLESS YOU KNOW WHAT IS GOING ON IN HERE */

char r_tag[] = "sg_data_to_macs";
char o_tag[] = "user_data_for_return_with_response";
char m_tag[] = "sg_data_from_macs";
char rs_tag[] = "user_data_returned_with_response";

char xml_vers_message[] = "<?xml version=\"1.0\"?>";

char sga_message[] = "<!-- THIS IS THE ACTUAL DATA REQUIRED FOR TRANSACTION COMPLETION -->";
char pt_message[] = "<!-- THIS IS ADDITIONAL CLIENT PASS THROUGH DATA -->";

char e1_message[] = "<!-- THIS IS AN ERROR MESSAGE INDICATING AN ERROR IN TRANSACTION COMPLETION -->";

char tt_tag[] = "tt";

char wo_tag[] = "wo";

char url_tag[] = "../xml-dtd/";

char originator[] = "Smith-Gardner";

char label[] = "DTD XML" ;

char sg_version[] = "1.0";

char language[] = "EN";


/* DO NOT MAKE CHANGES HERE UNLESS YOU KNOW WHAT IS GOING ON IN HERE */


char pathname[_MAX_PATH];
int kill_tmp_file;



char *rtrim(char *request, int req_len, struct global_struct *gbp) 				/* FROM SGTR0101 -> per rich*/
{

    gbp->got_char = 0;

    for(gbp->ret = (req_len - 1); gbp->ret >= 0; gbp->ret--)
    {

        if(isspace(request[gbp->ret]) || request[gbp->ret]=='\0') 
        {
            if (gbp->got_char == 0) 
                request[gbp->ret]='\0';
        }
        else 
            gbp->got_char = 1;
    }

    return(request);
}

char *handle_special_chars(struct global_struct *gbp, char *in_string)
{

	gbp->p1=in_string;
	gbp->p2=gbp->out_string;
	gbp->p3=gbp->out_string;

	while (*(gbp->p1)!='\0')
	{
		switch (*(gbp->p1))
		{
		case ' ':
			(*(gbp->p2))=(*(gbp->p1));
			(gbp->p2)++;
			break;
		case '&':
			(*(gbp->p2))='&';
			(gbp->p2)++;
			(*(gbp->p2))='a';
			(gbp->p2)++;
			(*(gbp->p2))='m';
			(gbp->p2)++;
			(*(gbp->p2))='p';
			(gbp->p2)++;
			(*(gbp->p2))=';';
			(gbp->p2)++;
			(gbp->p3)=(gbp->p2);
			break;
		case '<':
			(*(gbp->p2))='&';
			(gbp->p2)++;
			(*(gbp->p2))='l';
			(gbp->p2)++;
			(*(gbp->p2))='t';
			(gbp->p2)++;
			(*(gbp->p2))=';';
			(gbp->p2)++;
			(gbp->p3)=(gbp->p2);
			break;
		case '>':
			(*(gbp->p2))='&';
			(gbp->p2)++;
			(*(gbp->p2))='g';
			(gbp->p2)++;
			(*(gbp->p2))='t';
			(gbp->p2)++;
			(*(gbp->p2))=';';
			(gbp->p2)++;
			(gbp->p3)=(gbp->p2);
			break;
		case '\'':
			(*(gbp->p2))='&';
			(gbp->p2)++;
			(*(gbp->p2))='a';
			(gbp->p2)++;
			(*(gbp->p2))='p';
			(gbp->p2)++;
			(*(gbp->p2))='o';
			(gbp->p2)++;
			(*(gbp->p2))='s';
			(gbp->p2)++;
			(*(gbp->p2))=';';
			(gbp->p2)++;
			(gbp->p3)=(gbp->p2);
			break;
		case '\"':
			(*(gbp->p2))='&';
			(gbp->p2)++;
			(*(gbp->p2))='q';
			(gbp->p2)++;
			(*(gbp->p2))='u';
			(gbp->p2)++;
			(*(gbp->p2))='o';
			(gbp->p2)++;
			(*(gbp->p2))='t';
			(gbp->p2)++;
			(*(gbp->p2))=';';
			(gbp->p2)++;
			(gbp->p3)=(gbp->p2);
			break;
		default:
			(*(gbp->p2))=(*(gbp->p1));
			(gbp->p2)++;
			(gbp->p3)=(gbp->p2);

		}

		(gbp->p1)++;
	}
	*(gbp->p3) = '\0';

	return(gbp->out_string);
}

int getpath(struct global_struct *gbp)
{

    if((gbp->buffer = malloc( _MAX_PATH )) == NULL)
	{
		printf("Could not allocate memory!\n");
		return(-1);
	}

    memset(gbp->rootpath,'\0', _MAX_PATH);

    /* Get the current working directory: */
    if( _getcwd( gbp->buffer, _MAX_PATH ) == NULL )
        return(-1);

    gbp->req_len = strlen(gbp->buffer);

    strncpy(gbp->rootpath, gbp->buffer, gbp->req_len);

    gbp->req_len = strlen(gbp->rootpath);

#ifdef WIN32
    if(gbp->rootpath[gbp->req_len-1] == '\\') 
		gbp->rootpath[gbp->req_len-1] = '\0';

    sprintf(gbp->rootpath,"%s\\profiles",gbp->rootpath);
#endif

#ifdef LINUX 
    if(gbp->rootpath[gbp->req_len-1] == '/') 
		gbp->rootpath[gbp->req_len-1] = '\0';

    sprintf(gbp->rootpath,"%s/profiles",gbp->rootpath);
#endif
#ifdef SOLARIS2 
    if(gbp->rootpath[gbp->req_len-1] == '/') 
		gbp->rootpath[gbp->req_len-1] = '\0';

    sprintf(gbp->rootpath,"%s/profiles",gbp->rootpath);
#endif

    free(gbp->buffer);

    return(0);
}

/*
void GetTmpPath()						// FROM SGTR0101 -> per rich 
{
FILE *tmpfile;
char *tmp = NULL;
char *tmp2 = NULL;
int count = 0;
int len = 0;

    if((tmp = malloc(_MAX_PATH)) == NULL)
      return;
    if((tmp2 = malloc(_MAX_PATH)) == NULL)
      return;

    memset(tmp,'\0', _MAX_PATH);
    memset(tmp2,'\0', _MAX_PATH);

    if((_getcwd(tmp, _MAX_PATH)) == NULL)
      return;
#ifdef WIN32
    strcat(tmp, "\\parsertmp");
#endif
#ifdef LINUX
    strcat(tmp, "/parsertmp");
#endif
#ifdef SOLARIS2
    strcat(tmp, "/parsertmp");
#endif

    if((tmpfile = fopen(tmp, "r" )) == NULL ) 
    {
        printf("The temppath file could not be opened!, %s\n", tmp);
        return;
    }

    memset(pathname, '\0', _MAX_PATH);
    fgets(pathname, 80, tmpfile);               // Location of tmp files 
	len = (int)strlen(pathname);
	for(count = 0; count < len; count++)
	{
        if(pathname[count] == 0x0d)
			 pathname[count] = '\0';
		if(pathname[count] == 0x0a)
			 pathname[count] = '\0';

//		printf("%d ", pathname[count]);
	}
    fgets(tmp2, 80, tmpfile);                   // Read the kill_tmp_file value /
    sscanf(tmp2, "%d", &kill_tmp_file);
    fclose(tmpfile);
    free(tmp);
    free(tmp2);

 return;
}
*/
void GetInf(struct global_struct *gbp)						/* FROM SGTR0101 -> per rich */
{


    if((gbp->line = malloc(_MAX_PATH)) == NULL)
		return;

    memset(gbp->webexec,'\0',sizeof(gbp->webexec));
    memset(gbp->tmaxexec,'\0',sizeof(gbp->tmaxexec));
    memset(gbp->hphost,'\0',sizeof(gbp->hphost));
    memset(gbp->webport,'\0',sizeof(gbp->webport));
    memset(gbp->tmaxport,'\0',sizeof(gbp->tmaxport));
    memset(gbp->logfile,'\0',sizeof(gbp->logfile));

#ifdef WIN32
    sprintf(gbp->filename, "%s\\customer.inf", gbp->rootpath);
#endif
#ifdef LINUX
    //sprintf(filename, "%s/customer.inf", gbp->rootpath);
	sprintf(gbp->filename, "/usr/local/apache/profiles/customer.inf");
#endif
#ifdef SOLARIS2
    //sprintf(filename, "%s/customer.inf", gbp->rootpath);
	sprintf(gbp->filename, "/usr/local/apache/profiles/customer.inf");
#endif
//printf("%s filename\n", filename);
    if( (gbp->infile = fopen(gbp->filename, "r" )) == NULL ) 
    {
        printf("The settings file could not be opened! %s\n",gbp->filename );
        return;
    }

    memset(gbp->line,'\0',_MAX_PATH);
    fgets(gbp->line, _MAX_PATH, gbp->infile);             /* First line, comments */

    memset(gbp->line,'\0',_MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* WEBREQST EXEC */
    sprintf(gbp->webexec,"%s",rtrim(gbp->line,80,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* TAPEMACS EXEC */
    sprintf(gbp->tmaxexec,"%s",rtrim(gbp->line,80,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* HP Host Name */
    sprintf(gbp->hphost,"%s",rtrim(gbp->line,80,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* WEBREQST Port Name */
    sprintf(gbp->webport,"%s",rtrim(gbp->line,80,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* TAPEMACS Port Name */
    sprintf(gbp->tmaxport,"%s",rtrim(gbp->line,80,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* Order Log File Name */
    sprintf(gbp->logfile,"%s",rtrim(gbp->line,80,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* Catalog Request Log File Name */
    sprintf(gbp->reqfile,"%s",rtrim(gbp->line, _MAX_PATH,gbp));

    memset(gbp->line,'\0', _MAX_PATH);
    fgets(gbp->line,80,gbp->infile);               /* Location of image files */
    sprintf(gbp->imageloc,"%s",rtrim(gbp->line, _MAX_PATH,gbp));

    fclose(gbp->infile);

/*    Get the company and division from the executing script's filename.  */
//    memset(line,'\0', _MAX_PATH);
//    sprintf(line,"%s", getenv("SCRIPT_NAME"));
//    strncpy(gbp->company,line + (strlen(line)-8),2);
//    strncpy(gbp->division,line + (strlen(line)-6),2);

    free(gbp->line);


    gbp->req_len = strlen(gbp->rootpath);

    strncpy(gbp->imagepath,gbp->rootpath,gbp->req_len-9);

    sprintf(gbp->imagepath,"%s%s",gbp->imagepath,gbp->imageloc);

    gbp->req_len = strlen(gbp->imagepath);

    /* Change the forward slashes to backslashes */

    for(gbp->a = 0; gbp->a < gbp->req_len; gbp->a++) 
    {
		if (gbp->imagepath[gbp->a] == '/') 
			memset(gbp->imagepath + gbp->a, '\\', 1);
    }

    return;
}

/*
int isempty(char *request, int req_len)
{

    int i = 0;
	int got_it = 0;;

    for(i = req_len-1; i >= 0; i--)
        if ((request[i] == ' ') || (request[i] == '\0'))
            if (got_it == 0) 
			{
                request[i]='\0';
                got_it = 1;
            }

    if (strlen(request) > 0) 
        return(0);
    else 
        return(1);

}
*/
/*************************************************************************************/
/*                            NEW CODE FOR XML BEGINS HERE                           */
/*************************************************************************************/

int handle_sga_xml_data(char *stdout_buffer, request_rec *r, struct global_struct *gbp)
{

/*	char sgadata_buf[TAG_BUFFER_LENGTH];
	char requestdata_buf[TAG_BUFFER_LENGTH];  */

	if((gbp->settag = strstr(stdout_buffer, "<tt")) == NULL)
	{
		if((gbp->settag = strstr(stdout_buffer, "<wo")) == NULL)
		{
			printf(" The tag is not set\n");
			fflush(stdout);	
			free (stdout_buffer);
			return(1);	/* An Error Occurred */
		}
		else
		{
			memset(gbp->tag, '\0', 5);
			strncpy(gbp->tag, gbp->settag+1, 2);
		}
	}
	else
	{
		memset(gbp->tag, '\0', 5);
		strncpy(gbp->tag, gbp->settag+1, 2);
	}


	if((initialize_tag_data(gbp)) == -1)
	{
		XML_Error("handle_sga_xml_data -> initialize_tag_data","failed transaction","init","-1",r,gbp);
		return(1);	/* An Error Occurred */
	}

/* Here we will use the global buffer "extern char* stdout_buffer" as data source */

/* This buffer will not mean a thing to us if we dont start off with <ttXX>*/
/* It must also end with </ttXX>*/
/* The XX part of ttXX is any valid transaction type... i.e. 00, 01, 10, 99 */
/* Compare the necessaries here*/
/* Find the initial ttXX... this is the beginning of the ttXX XML document*/
/* If not found... this isn't an SGA XML doc... 
	just print the ORIGINAL buffer as the response*/

	if((gbp->begin_tag = strstr(stdout_buffer, gbp->tt_btag)) == NULL)
	{
		if((gbp->begin_tag = strstr(stdout_buffer, gbp->wo_btag)) == NULL)
		{
			XML_Error("handle_sga_xml_data","failed transaction","no begin tag","-1",r,gbp);
			return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
		}
	}
	gbp->tt_ok = 0;

/* Find the id of ttXX */

	if((gbp->begin_tag_id = strstr(stdout_buffer, gbp->bitag)) == NULL)
	{
		XML_Error("handle_sga_xml_data","failed transaction","no id tag","-1",r,gbp);
		return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
	}
	gbp->tt_ok = 1;

/* Find the secondary ttXX... this is the end of the ttXX XML document*/

	if((gbp->end_tag = strstr(stdout_buffer, gbp->tt_betag)) == NULL)
	{
		if((gbp->end_tag = strstr(stdout_buffer, gbp->wo_betag)) == NULL)
		{//ap_rprintf(r,"%s###%s###%s",stdout_buffer, gbp->tt_betag,gbp->wo_betag);
			XML_Error("handle_sga_xml_data","failed transaction","no end tag ()","-1",r,gbp);
			return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
		}
	}
	gbp->tt_ok = 2;


/* Find for sgadata TAGS */

	if((gbp->begin_sgadata_tag = strstr(stdout_buffer, gbp->rtag)) == NULL)
	{
		XML_Error("handle_sga_xml_data","failed transaction","no begin tag (required data)","-1",r,gbp);
		return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
	}
	gbp->tt_ok = 3;

	
/* Find the secondary sgadata... this is the end of the ttXX XML document*/
	
	if((gbp->end_sgadata_tag = strstr(stdout_buffer, gbp->retag)) == NULL)
	{
		XML_Error("handle_sga_xml_data","failed transaction","no end tag (required data)","-1",r,gbp);
		return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
	}
	gbp->tt_ok = 4;

/* Find for requestdata TAGS */

	if((gbp->begin_requestdata_tag = strstr(stdout_buffer, gbp->otag)) == NULL)
	{
		XML_Error("handle_sga_xml_data","failed transaction","no begin tag (pass thru data)","-1",r,gbp);
		return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
	}
	gbp->tt_ok = 6;
		

/* Find the secondary requestdata... this is the end of the ttXX XML document*/

	if((gbp->end_requestdata_tag = strstr(stdout_buffer, gbp->oetag)) == NULL)
	{
		XML_Error("handle_sga_xml_data","failed transaction","no end tag (pass thru data)","-1",r,gbp);
		return(1);	/* An Error Occurred... this wasn't an SGA XML Document */
	}
	gbp->tt_ok = 7;
		

	if(gbp->tt_ok == 7)
	{
		sscanf(gbp->begin_tag_id, "%s", gbp->tttype_buf);
		sscanf(gbp->tttype_buf+6, "%4d", &(gbp->this_transaction));
		gbp->tt_ok = 8;
	}

/* OK WE BELIEVE THIS A AN SGA DOCUMENT */
	if(gbp->tt_ok == 8) /* At this point we are still at the beginning of buffer_copy */
	{
		/* We NOW FEEL CERTAIN THAT WE HAVE a valid SGA XML Document */
		/* Do Some More Processing */
		/* AT THIS POINT WE USE  this_transaction  TO SELECT THE CORRECT transaction_type */

		gbp->success = setup_transaction_type(gbp->this_transaction,r,gbp,stdout_buffer);/* <-- IT's ALL DONE IN HERE... */
		if(gbp->success == 1) /* gbp->success */
		{
			/* This is where we do the necessary cleanup activities...*/
			return(0); /* Remove this to fall out to the end of the routine...*/
		}
		else if(gbp->success == -1)	/* FAILURE */
		{
			XML_Error("setup_transaction_type","failed transaction","in process inititation","-1",r,gbp);
			return(1);	/* An Error Occurred */
		}
		else if(gbp->success == -2)	/* FAILURE */
		{
			XML_Error("setup_transaction_type","failed transaction","in filling structure","-2",r,gbp);
			return(1);	/* An Error Occurred */
		}
		else if(gbp->success == -3)	/* FAILURE */
		{
			XML_Error("setup_transaction_type","failed transaction","in process conclusion","-3",r,gbp);
			return(1);	/* An Error Occurred */
		}
		else if(gbp->success == -4)	/* FAILURE */
		{
			XML_Error("setup_transaction_type","failed transaction","invalid XML document type","-4",r,gbp);
			return(1);	/* An Error Occurred */
		}

	}
	else
	{
		/* We DON'T HAVE a valid SGA XML Document... or something is wrong */
		/* Do No More Processing */

		XML_Error("handle_sga_xml_data","failed transaction","not a valid sga xml document","-1",r,gbp);
		return(1);	/* An Error Occurred */
	}

	return(0);
}


int setup_transaction_type(int tttype, request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
//	ap_rprintf(r,"%s",stdout_buffer); return 0;
	switch(tttype)
	{
		case 1:		
			if((strcmp(gbp->tag, "tt")) == 0)
			{
				if((tt0001_start(gbp)) != 0)
					return(-1);
				if((fill_working_struct(gbp,stdout_buffer)) != 0)
					return(-2);
				if((tt0001_end(r,gbp,stdout_buffer)) != 0)
					return(-3);
				return(1);
			}
			else if((strcmp(gbp->tag, wo_tag)) == 0)
			{
				if((wo01_start(gbp)) != 0)
					return(-1);
				if((fill_working_struct(gbp,stdout_buffer)) != 0)
					return(-2);  
				if((wo01_end(r,gbp,stdout_buffer)) != 0)
					return(-3);
				return(1);
			}
		break;

		case 3:
			if((strcmp(gbp->tag, tt_tag)) == 0)
			{
				if((tt0003_start(gbp)) != 0)
					return(-1);
				if((fill_working_struct(gbp,stdout_buffer)) != 0)
					return(-2);
				if((tt0003_end(r,gbp,stdout_buffer)) != 0)
					return(-3);
				return(1);
			}
			else if((strcmp(gbp->tag, wo_tag)) == 0)
			{
				if((wo03_start(gbp)) != 0)
					return(-1);
				if((fill_working_struct(gbp,stdout_buffer)) != 0)
					return(-2);  
				if((wo03_end(r,gbp,stdout_buffer)) != 0)
					return(-3);
				return(1);
			}
		break;

		case 5:		
			if((tt0005_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0005_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 7:			
			if((tt0007_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0007_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 9:
			if((tt0009_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0009_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 11:			
			if((tt0011_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0011_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 13:		
			if((tt0013_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0013_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 15:			
			if((tt0015_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0015_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 17:		
			if((tt0017_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0017_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 19:			
			if((tt0019_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0019_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 21:			
			if((tt0021_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0021_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 23:		
			if((tt0023_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0023_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 25:		
			if((tt0025_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0025_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 27:
			if((tt0027_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0027_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 29:		
			if((tt0029_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0029_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 31:		
			if((tt0031_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0031_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 33:		
			if((tt0033_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0033_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 35:			
			if((tt0035_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0035_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 37:		
			if((tt0037_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0037_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 39:			
			if((tt0039_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0039_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 41:		
			if((tt0041_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0041_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 43:			
			if((tt0043_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0043_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 45:		
			if((tt0045_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0045_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 47:			
			if((tt0047_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0047_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 49:			
			if((tt0049_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0049_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 51:			
			if((tt0051_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0051_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 53:			
			if((tt0053_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0053_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		
		case 55:		
			if((tt0055_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0055_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;   

		case 57:			
			if((tt0057_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0057_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 59:			
			if((tt0059_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0059_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 61:			
			if((tt0061_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0061_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 63:			
			if((tt0063_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0063_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 65:			
			if((tt0065_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0065_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 67:			
			if((tt0067_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0067_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;  

		case 69:			
			if((tt0069_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0069_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 71:			
			if((tt0071_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0071_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 73:			
			if((tt0073_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0073_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;


		case 75:			
			if((tt0075_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0075_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 77:			
			if((tt0077_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0077_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 79:			
			if((tt0079_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0079_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 81:			
			if((tt0081_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0081_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 83:			
			if((tt0083_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0083_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;


		case 85:			
			if((tt0085_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0085_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;


		case 87:			
			if((tt0087_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0087_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 89:			
			if((tt0089_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0089_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 91:			
			if((tt0091_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0091_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;

		case 93:			
			if((tt0093_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0093_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 95:			
			if((tt0095_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0095_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 105:			
			if((tt0105_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0105_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 107:			
			if((tt0107_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0107_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 109:			
			if((tt0109_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0109_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 111:			
			if((tt0111_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0111_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 113:			
			if((tt0113_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0113_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 115:			
			if((tt0115_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0115_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 117:			
			if((tt0117_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0117_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;
		case 123:			
			if((tt0123_start(gbp)) != 0)
				return(-1);
			if((fill_working_struct(gbp,stdout_buffer)) != 0)
				return(-2);
			if((tt0123_end(r,gbp,stdout_buffer)) != 0)
				return(-3);
			return(1);
		break;


		default: /* NOT A VALID TRANSACTION TYPE # */
			return(-4);
		break;
	}

	return(-1);
}


int fill_working_struct(struct global_struct *gbp, char *stdout_buffer)
{

	gbp->buf = malloc(MAXDATALEN);
	if(gbp->buf == NULL)
		return(1); 

	for( gbp->i = 0; gbp->i < MAXSYMBOL; gbp->i++)
	{
		gbp->ret = sscanf(gbp->reqdata, " %s ", gbp->reqdata_tag[gbp->i]);
		if(gbp->ret == 0)
			return(1);
		else if(gbp->ret > 0)
		{
			memset(gbp->buf, '\0', MAXDATALEN);
			gbp->ret = get_tag_data(gbp->reqdata_tag[gbp->i], gbp->buf,gbp,stdout_buffer); 
			if(gbp->ret == 0)
			{
				if(gbp->buf[0] == '\0')
					sprintf(gbp->reqdata_data[gbp->i], "*");
				else
					sprintf(gbp->reqdata_data[gbp->i], "%s", gbp->buf);
			}
		}
			gbp->reqdata++;
			gbp->reqdata += strlen(gbp->reqdata_tag[gbp->i]);
	}
	
	free (gbp->buf);
	return(0); 
}

int get_tag_from_seq(int n_seq, char *sz_tag_name, char *stdout_buffer, struct global_struct *gbp)
{

	memset(gbp->sz_tag_name1, '\0', TAG_SIZE);

	sprintf(gbp->sz_tag_name1, "_SEQ>%02d</", n_seq);
	gbp->pStart = strstr(stdout_buffer, gbp->sz_tag_name1);
	
	if(gbp->pStart == NULL)
		return -1;

	else
		gbp->pStart = gbp->pStart + 9;
		strncpy(sz_tag_name, gbp->pStart, 3);
		sz_tag_name[3] = '\0';
		return 0;
}


void set_tag_name(char *sz_tag_name, char *sz_format, int n_seq)
{
	memset(sz_tag_name, '\0', TAG_SIZE);
	sprintf(sz_tag_name, sz_format, n_seq);
}


int get_tag_data(char *sz_tag, char *sz_data, struct global_struct *gbp, char *stdout_buffer)
{


	gbp->open_tag = malloc(strlen(sz_tag) + 3);
	if(gbp->open_tag == NULL)
	{
		printf("Insufficient Memory for open_tag: %s\n", sz_tag);
		return(1);
	}
	sprintf(gbp->open_tag, "<%s>", sz_tag);

	gbp->close_tag = malloc(strlen(sz_tag) + 4);
	if(gbp->close_tag == NULL)
	{
		printf("Insufficient Memory for gbp->close_tag: %s\n", sz_tag);
		return(1);
	}
	sprintf(gbp->close_tag, "</%s>", sz_tag);

	if((gbp->first = strstr(stdout_buffer, gbp->open_tag)) == NULL)
	{
		free(gbp->open_tag);
		free(gbp->close_tag);
		return(1);
	}

    if((gbp->last = strstr(stdout_buffer, gbp->close_tag)) == NULL)
	{
		free(gbp->open_tag);
		free(gbp->close_tag);
		return(1);
	}
	                  
	gbp->data_start_pos = gbp->first - stdout_buffer;
	gbp->data_end_pos = gbp->last - stdout_buffer;

	gbp->data_start_pos += strlen(gbp->open_tag);
//	gbp->data_end_pos -= strlen(gbp->close_tag);

	for(gbp->count = gbp->data_start_pos, gbp->where = 0; gbp->count < gbp->data_end_pos; gbp->count++, gbp->where++)
	{
		if (stdout_buffer[gbp->count] == '&')
		{
			gbp->count+=1;
			if(stdout_buffer[gbp->count] == 'a' &&
				stdout_buffer[gbp->count+1] == 'm' &&
				stdout_buffer[gbp->count+2] == 'p' &&
				stdout_buffer[gbp->count+3] == ';')
			{
				sz_data[gbp->where] = '&';
				gbp->count+=3;
			}
			else
				if(stdout_buffer[gbp->count] == 'a' &&
					stdout_buffer[gbp->count+1] == 'p' &&
					stdout_buffer[gbp->count+2] == 'o' &&
					stdout_buffer[gbp->count+3] == 's' &&
					stdout_buffer[gbp->count+4] == ';')
				{
					sz_data[gbp->where] = '\'';
					gbp->count+=4;
				}
				else
					if(stdout_buffer[gbp->count] == 'q' &&
						stdout_buffer[gbp->count+1] == 'u' &&
						stdout_buffer[gbp->count+2] == 'o' &&
						stdout_buffer[gbp->count+3] == 't' &&
						stdout_buffer[gbp->count+4] == ';')
					{
						sz_data[gbp->where] = '\"';
						gbp->count+=4;
					}
					else
						if(stdout_buffer[gbp->count] == 'l' &&
							stdout_buffer[gbp->count+1] == 't' &&
							stdout_buffer[gbp->count+2] == ';')
						{
							sz_data[gbp->where] = '<';
							gbp->count+=2;
						}
						else 
							if(stdout_buffer[gbp->count] == 'g' &&
								stdout_buffer[gbp->count+1] == 't' &&
								stdout_buffer[gbp->count+2] == ';')
							{
								sz_data[gbp->where] = '>';
								gbp->count+=2;
							}

		}else
		{
			sz_data[gbp->where] = stdout_buffer[gbp->count];
		}
	}

/*	printf("%s = %s\n", sz_tag, sz_data);  */

	free(gbp->open_tag);
	free(gbp->close_tag);
	return 0;
}


int	initialize_tag_data(struct global_struct *gbp)
{

/* ********  DO NOT MESS WITH THESE AT ALL!  ******* */
	sprintf(gbp->tt_btag, "<%s", tt_tag);
	sprintf(gbp->tt_betag, "</%s", tt_tag);

	sprintf(gbp->wo_btag, "<%s", wo_tag);
	sprintf(gbp->wo_betag, "</%s", wo_tag);

	sprintf(gbp->bitag, "id=");

	sprintf(gbp->rtag, "<%s", r_tag);
	sprintf(gbp->retag, "</%s", r_tag);

	sprintf(gbp->otag, "<%s", o_tag);
	sprintf(gbp->oetag, "</%s", o_tag);

	sprintf(gbp->mtag, "<%s", m_tag);
	sprintf(gbp->metag, "</%s", m_tag);

	sprintf(gbp->rstag, "<%s", rs_tag);
	sprintf(gbp->rsetag, "</%s", rs_tag);

/* ********  DO NOT MESS WITH THESE AT ALL!  ******* */

	return(0);
}


int XML_Error(char *where, char *why, char *what, char *ecode, request_rec *r,
			  struct global_struct *gbp)
{
	char *timebf = NULL;

	timebf = xml_time();

	if(gbp->XML_ERROR_HEADER == -1)
	{
		ap_rprintf(r,"%s\n", xml_vers_message);
/*
		ap_rprintf(r,"<!DOCTYPE %s0000 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);

		ap_rprintf(r,"                     \"%s%s0000.dtd\">\n", url_tag, gbp->tag);
*/
		ap_rprintf(r,"<%s0000 %s\"%s0000\">\n", gbp->tag, gbp->bitag, gbp->tag);

		ap_rprintf(r,"%s\n", e1_message);

		gbp->XML_ERROR_HEADER = 0;
	}

	ap_rprintf(r,"	%s>\n", gbp->mtag);

	ap_rprintf(r,"	<TRANSACTION>%s%04d</TRANSACTION>\n", gbp->tag, gbp->this_transaction);

	ap_rprintf(r,"	<WHERE>%s</WHERE>\n", where);

	ap_rprintf(r,"	<WHY>%s</WHY>\n", why);

	ap_rprintf(r,"	<WHAT>%s</WHAT>\n", what);

	ap_rprintf(r,"	<ERROR_CODE>%s</ERROR_CODE>\n", ecode);

	ap_rprintf(r,"	<TIMESTAMP>%s</TIMESTAMP>\n", timebf);

	ap_rprintf(r,"	%s>\n\n", gbp->metag);

	if(timebf != NULL)
		free(timebf);

	return(1);
}


void XML_Error_Trailer(request_rec *r, struct global_struct *gbp)
{
	ap_rprintf(r,"%s\n", pt_message);
	ap_rprintf(r,"	%s>\n", gbp->rstag);

	reparse_customer_data(r,gbp);

	ap_rprintf(r,"	%s>\n", gbp->rsetag);

	ap_rprintf(r,"</%s0000>\n", gbp->tag);

	return;
}


void reparse_customer_data(request_rec *r, struct global_struct *gbp)
{
	for(gbp->count = 0; gbp->count < MAXSYMBOL; gbp->count++)
	{
		if(gbp->reqdata_data[gbp->count][0] != '*')
			ap_rprintf(r,"		<%s>%s</%s>\n", gbp->reqdata_tag[gbp->count], 
											handle_special_chars(gbp, gbp->reqdata_data[gbp->count]), 
											gbp->reqdata_tag[gbp->count]);
	}

	return;
}


char *xml_time(void)
{
    struct tm *newtime;
	time_t long_time;
	char temp[80];
    char am_pm[] = "AM";
	char *timebuf;

    time( &long_time );
    newtime = localtime( &long_time );

    if( newtime->tm_hour > 12 )
            strcpy( am_pm, "PM" );
    if( newtime->tm_hour > 12 )
            newtime->tm_hour -= 12;
    if( newtime->tm_hour == 0 )
            newtime->tm_hour = 12;

    sprintf(temp, "%.19s %s", asctime( newtime ), am_pm );
	timebuf = strdup(temp);

	return(timebuf);
}



//#########################################################################
// XML_Expat_Error
// JL
// Written specifically for the Expat parser that is called in sgparse.c
// Used to report the errors returned by Expat
// There is some hardcoded stuff in the strings compared to XML_Error
// Also contains what would normally be handled by XML_Error_Tailer

int XML_Expat_Error(char *where, char *why, char *what, char *ecode, request_rec *r, struct global_struct *gbp)
{
	char *timebf = NULL;
	extern char *xml_time(void);
	timebf = xml_time();

	ap_rprintf(r,"%s\n", xml_vers_message);
/*		ap_rprintf(r,"<!DOCTYPE %stt0000 PUBLIC \"-//%s//%s %s//%s\" \n", 
							gbp->tag, originator, label, sg_version, language);
	ap_rprintf(r,"                     \"%stt0000.dtd\">\n", url_tag);
*///	ap_rprintf(r,"%s\n", e0_message);
	ap_rprintf(r,"<ttwo0000 id=\"0000\" version=\"1.0\">\n");
	ap_rprintf(r,"%s\n", e1_message);
	ap_rprintf(r,"<sg_data_from_macs id=\"0000\">\n");

	ap_rprintf(r,"	<TRANSACTION>Not Determined</TRANSACTION>\n");

	ap_rprintf(r,"	<WHERE>%s</WHERE>\n", where);

	ap_rprintf(r,"	<WHY>%s</WHY>\n", why);

	ap_rprintf(r,"	<WHAT>%s</WHAT>\n", what);

	ap_rprintf(r,"	<ERROR_CODE>%s</ERROR_CODE>\n", ecode);

	ap_rprintf(r,"	<TIMESTAMP>%s</TIMESTAMP>\n", timebf);

	ap_rprintf(r,"</sg_data_from_macs>\n");

	ap_rprintf(r,"</ttwo0000>\n");


	if(timebf != NULL)
		free(timebf);

	return(-1);
}

