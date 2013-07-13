#ifndef EXTERNS_H
#define EXTERNS_H

#ifndef GBSTRUCT_INCL
#define GBSTRUCT_INCL
#include "gbstruct.h"
#endif




//extern FILE *infile;                           /* Profile file handle */

//extern char reqstr[3072];                      /* POST request string */
//extern char filename[1024];                    /* built filename */
//extern char suffix[3];                         /* Profile Suffix # */
//extern char rootpath[_MAX_PATH];               /* root path */
//extern char imagepath[_MAX_PATH];              /* path where the images are */
//extern char writedata[1024];                   /* data to put in file */
//extern char company[3];                        /* Company */
//extern char division[3];                       /* Division */

/* Debug vars */

extern int debugmode;                          /* Debug Mode.  1 = on */
extern char theuserid[17];                     /* User ID, if one exists */


/* INF File information */

//extern char webexec[80];                       /* Executable location of WEBREQST */
//extern char tmaxexec[80];                      /* Executable location of TAPEMACS */
//extern char hphost[10];                        /* HP Host Name */
//extern char webport[10];                       /* Port of WEBREQST listener on HP */
//extern char tmaxport[10];                      /* Port of TAPEMACS listener on HP */
//extern char logfile[30];                       /* Name of order logging file */
//extern char reqfile[30];                       /* Name of catalog request file */
//extern char imageloc[60];                      /* Location of image files */

/* Global Identifiers */


/* From parser.c */

extern char     *rtrim(char *, int, struct global_struct *);
extern char    *handle_special_chars(struct global_struct *, char *);

extern int      getpath(struct global_struct *);
extern void     GetInf(struct global_struct *);

//extern int    isempty(char *, int);

/* End Of SGA Original Code Defs, vars, etc. */
/***************************************************/
/* These files, vars, etc. are for use with rxp.c and associated code...*/

extern int      handle_sga_xml_data(char *, request_rec *, struct global_struct *);
extern int      setup_transaction_type(int, request_rec *, struct global_struct *, char *);
extern int      fill_working_struct(struct global_struct *, char *);
extern int      initialize_tag_data(struct global_struct *);
extern int      XML_Error(char *, char *, char *, char *, request_rec *, struct global_struct *);
extern void     reparse_customer_data(request_rec *, struct global_struct *);
extern int              XML_Expat_Error(char *, char *, char *, char *, request_rec *, struct global_struct *);

extern int     get_tag_from_seq(int, char *, char *, struct global_struct *);
extern void    set_tag_name(char *, char *, int);
extern int     get_tag_data(char *, char *, struct global_struct *, char *);
extern int     get_node_pos(char *, struct global_struct *, char *);
extern int     node_get_tag_data(char *, char *, struct global_struct *, char *);

extern char pathname[_MAX_PATH];   /* defined in rxp.c */
extern int kill_tmp_file;


/* DO NOT MAKE CHANGES HERE UNLESS YOU KNOW WHAT IS GOING ON IN HERE */
/* ************  THESE ARE ALL DEFINED IN xml.c ****** */

extern char r_tag[];
extern char o_tag[];
extern char m_tag[];
extern char rs_tag[];

extern char xml_vers_message[];

extern char sga_message[];
extern char pt_message[];

extern char e1_message[];

extern char wo_tag[];

extern char tt_tag[];

extern char url_tag[];

extern char originator[];

extern char label[];

extern char sg_version[];

extern char language[];

/* DO NOT MAKE CHANGES HERE UNLESS YOU KNOW WHAT IS GOING ON IN HERE */

#endif
