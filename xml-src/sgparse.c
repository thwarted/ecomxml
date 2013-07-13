#ifdef WIN32
#include <windows.h>
//#include <winsock.h>
#include <string.h>
#include <process.h>
#include <io.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_main.h"
#include "http_protocol.h"
#include "util_script.h"
#include "xmlparse.h"

# include "externs.h"

#ifdef LINUX
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>


#endif

#ifdef SOLARIS2
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#endif

/*
#include <io.h>
*/
extern int handle_sga_xml_data(char *, request_rec *, struct global_struct *);
extern void	XML_Error_Trailer(request_rec *, struct global_struct *);

// EXPAT STUFF  ################################################
/*	int Eventcnt = 0;


void
default_hndl(void *data, const char *s, int len) {
  fwrite(s, len, sizeof(char), stdout);
}  // End default_hndl 

//****************************************************************
// ** Call from within a handler to print the currently recognized
// ** document fragment. Temporarily set the default handler and then
// ** invoke it via the the XML_DefaultCurrent call.
// *
void
printcurrent(XML_Parser p) {
  XML_SetDefaultHandler(p, default_hndl);
  XML_DefaultCurrent(p);
  XML_SetDefaultHandler(p, (XML_DefaultHandler) 0);
}  // End printcurrent 

void
start_hndl(void *data, const char *el, const char **attr) {

	printf("\n%4d: Start tag %s - ", Eventcnt++, el);

	printcurrent((XML_Parser) data);
}  // End of start_hndl


void
end_hndl(void *data, const char *el) {
  printf("\n%4d: End tag %s -\n", Eventcnt++, el);
}  // End of end_hndl 

void
char_hndl(void *data, const char *txt, int txtlen) {
  printf("\n%4d: Text - ", Eventcnt++);
  fwrite(txt, txtlen, sizeof(char), stdout);
}  // End char_hndl 

void
proc_hndl(void *data, const char *target, const char *pidata) {
  printf("\n%4d: Processing Instruction - ", Eventcnt++);
  printcurrent((XML_Parser) data);
}  // End proc_hndl 

*/
// END OF EXPAT STUFF ###########################################

int sgparse(char *stdout_buffer, request_rec *r, struct global_struct *);




int sgparse(char *stdout_buffer, request_rec *r, struct global_struct *gbp)
{
//	static int first = 0;
//	int sga_xml = -1;
//	struct global_struct gb;
//	int len;
//	XML_Parser Xp;
//	char linenumber[100];
//	int i=0;
	// setting up of the variables 
	gbp->XML_ERROR_HEADER = -1;  
	gbp->this_transaction = 0;
	
//	first++;
//ap_rprintf(r,"BEFORE EXPAT = %s",stdout_buffer);	
/*
#ifdef WIN32
	printf("The request : %s  PID = %d : COUNT = %d\n",r->the_request,
					_getpid(),first);
#else
	printf("The request : %s  PID = %d : COUNT = %d\n",r->the_request,
					getpid(),first);
#endif
*/			
//Call Expat  (XML Parser)

  gbp->Xp = XML_ParserCreate(NULL);
  if (! gbp->Xp) {
    //printf("Couldn't allocate memory for xml parser\n");

	XML_Expat_Error("XML Parser", "Couldn't allocate memory for XML parser", "validation","XML Parser Error",r,gbp);
    return 0;
  }

 // XML_UseParserAsHandlerArg(p);
 // XML_SetElementHandler(p, start_hndl, end_hndl);
 // XML_SetCharacterDataHandler(p, char_hndl);
 // XML_SetProcessingInstructionHandler(p, proc_hndl);
//ap_rprintf(r,"starting expat");
//ap_rprintf(r,"%s",stdout_buffer);
//    len = strlen(stdout_buffer);

    if (! XML_Parse(gbp->Xp, stdout_buffer, strlen(stdout_buffer), 1)) {
	sprintf(gbp->linenumber,"line number %d", XML_GetCurrentLineNumber(gbp->Xp));
	XML_Expat_Error("XML Parser Error at", (char *)gbp->linenumber , 
		(char *)XML_ErrorString(XML_GetErrorCode(gbp->Xp)) ,"XML Syntax Error",r,gbp);

      return 0;
    }



//End of Expat handling

	gbp->reqdata="a b c d e f g h i j k l m n o p q r s t u v w x y z";	

//SG parse	
//	ap_rprintf(r,"Going into Handle_SGA_XML_Data");
//	sga_xml = handle_sga_xml_data(stdout_buffer, r,gbp);   // sending "request" i.e. 0 argument 	
	handle_sga_xml_data(stdout_buffer, r,gbp);   // sending "request" i.e. 0 argument 	

	if(gbp->XML_ERROR_HEADER == 0)
		XML_Error_Trailer(r, gbp);

	//ap_rprintf(r,"RXP_MAIN : 12 : count = %d\n", first );
#ifdef WIN32
	
	return 0;
#endif
}

