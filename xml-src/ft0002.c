/* ft0002.c  Customer Lookup */
/*modified for phase 2 - JL */
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


int             ft0002_CatSendStr(struct global_struct *, char *, ft0002_st_send *);
int             ft0002_ParceRecvStr(ft0002_st_recv *, char *, request_rec *,
                                                  struct global_struct *);
int             ft0002_lt_process(request_rec *, struct global_struct *, char *);

extern char *stdout_buffer;


int ft0002_start(struct global_struct *gbp)
{

    if((getpath(gbp)) == -1)
                return(-1);

    GetInf(gbp);

   return(0);

}


int ft0002_end(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
    if((ft0002_lt_process(r,gbp,stdout_buffer)) == -1)
                return(-1);

    return(0);
}


int ft0002_lt_process(request_rec *r, struct global_struct *gbp, char *stdout_buffer)
{
        gbp->ftsendbuf0002 = (ft0002_st_send *)malloc(sizeof(ft0002_st_send));
        gbp->ftrecvbuf0002 = (ft0002_st_recv *)malloc(sizeof(ft0002_st_recv));

        gbp->sendbufcat = malloc(ft0002_LAN_SEND_BUF_LEN + 1);
        if (gbp->sendbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }

        gbp->recvbufcat = malloc(ft0002_LAN_RECV_BUF_LEN + 1);
        if (gbp->recvbufcat == NULL)
        {
                ap_rprintf(r, "Isufficient memory available\n");
                exit(1);
        }  

    memset(gbp->sendbufcat, '\0', ft0002_LAN_SEND_BUF_LEN + 1);
    memset(gbp->recvbufcat, '\0', ft0002_LAN_RECV_BUF_LEN + 1);

    memset(gbp->ftsendbuf0002,  '\0',  sizeof(ft0002_st_send));

    sprintf(gbp->confirmbuf, "     ");
    strcpy(gbp->ftsendbuf0002->request_id, "XML");
    strcpy(gbp->ftsendbuf0002->record_id, "0002");
    strcpy(gbp->ftsendbuf0002->ip_address, r->connection->remote_ip);

    get_tag_data("COMPANY", gbp->ftsendbuf0002->company,gbp,stdout_buffer);
    get_tag_data("DIVISION", gbp->ftsendbuf0002->division,gbp,stdout_buffer);
    get_tag_data("UID", gbp->ftsendbuf0002->userid,gbp,stdout_buffer);
    get_tag_data("SEARCH", gbp->ftsendbuf0002->search,gbp,stdout_buffer);
    get_tag_data("FIRST_START", gbp->ftsendbuf0002->first_start,gbp,stdout_buffer);
    get_tag_data("FIRST_END", gbp->ftsendbuf0002->first_end,gbp,stdout_buffer);
    get_tag_data("LAST_START", gbp->ftsendbuf0002->last_start,gbp,stdout_buffer);
    get_tag_data("LAST_END", gbp->ftsendbuf0002->last_end,gbp,stdout_buffer);
    get_tag_data("PAGE_NO", gbp->ftsendbuf0002->page_no,gbp,stdout_buffer);
    get_tag_data("PAGE_COUNT", gbp->ftsendbuf0002->page_cnt,gbp,stdout_buffer);
    get_tag_data("SEARCH_TYPE", gbp->ftsendbuf0002->search_type,gbp,stdout_buffer);
    get_tag_data("CUSTOMER_MODE", gbp->ftsendbuf0002->cust_mode,gbp,stdout_buffer);
    get_tag_data("FULL_ORDER_NUM", gbp->ftsendbuf0002->full_order_no,gbp,stdout_buffer);
    get_tag_data("LIST_MODE", gbp->ftsendbuf0002->list_mode,gbp,stdout_buffer);

    for (gbp->i = 0; gbp->i < ft0002_CUST_NO_LOOP; gbp->i++)
    {
		sprintf(gbp->tagname, "CUSTOMER_NUM_%d", gbp->i+1);
        get_tag_data(gbp->tagname,  gbp->ftsendbuf0002->cust_no[gbp->i],gbp,stdout_buffer);

	}

        if(ft0002_CatSendStr(gbp, gbp->sendbufcat, gbp->ftsendbuf0002) == ft0002_LAN_SEND_BUF_LEN)
        {
                if((gbp->sock = sga_connect(gbp->hphost, gbp->fvfport, gbp->webexec, &(gbp->rc),r,gbp)) == INVALID_SOCKET)
                {
                        XML_Error("sga_connect","failed to connect","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0002);
                        free (gbp->ftrecvbuf0002);
                        return(-1);
                }

                if((gbp->rc = sga_send(gbp->sock, gbp->sendbufcat, ft0002_LAN_SEND_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0002);
                        free (gbp->ftrecvbuf0002);
                        return(-1);
                }

                if((gbp->rc = sga_recv(gbp->sock, gbp->recvbufcat, ft0002_LAN_RECV_BUF_LEN,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_recv","failed to receive","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0002);
                        free (gbp->ftrecvbuf0002);
                        return(-1);
                }

                // Do an additional send and recieve for confirmation
                if((gbp->rc = sga_send(gbp->sock, gbp->confirmbuf, 5,r,gbp)) == SOCKET_ERROR)
                {
                        XML_Error("sga_send","failed to send ack","communications","-1",r,gbp);
                        free (gbp->sendbufcat);
                        free (gbp->recvbufcat);
                        free (gbp->ftsendbuf0002);
                        free (gbp->ftrecvbuf0002);
                        return(-1);
                }
                
/*
                gbp->rc = sga_recv(sock, gbp->confirmbuf, 5);
*/
                sga_close2(gbp->sock, (int)USE_SHUTDOWN,r,gbp);

                ft0002_ParceRecvStr(gbp->ftrecvbuf0002, gbp->recvbufcat,r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0002);
                free (gbp->ftrecvbuf0002);

        }
        else
        {
                XML_Error("ft0002_CatSendStr","Failed filling the send buffer","communications","-1",r,gbp);
                free (gbp->sendbufcat);
                free (gbp->recvbufcat);
                free (gbp->ftsendbuf0002);
                free (gbp->ftrecvbuf0002);
                return(-1);
        }

    return(0);
}


int ft0002_CatSendStr(struct global_struct *gbp, char *sz_sendbufcat, ft0002_st_send *ptr_sendbuf)
{
    gbp->j = sprintf(sz_sendbufcat,
            "%-4.4s%-4.4s%-2.2s%-2.2s%-16.16s%-16.16s%-25.25s"
			"%-80.80s%-8.8s%-8.8s%-8.8s%-8.8s%04d%04d%-2.2s%-1.1s%-12.12s%-1.1s",
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s"
			"%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s%-10.10s",

            ptr_sendbuf->request_id,
            ptr_sendbuf->record_id,
            ptr_sendbuf->company,
            ptr_sendbuf->division,
            ptr_sendbuf->userid,
            ptr_sendbuf->ip_address,
            ptr_sendbuf->filler,
            ptr_sendbuf->search,
            ptr_sendbuf->first_start,
            ptr_sendbuf->first_end,
            ptr_sendbuf->last_start,
            ptr_sendbuf->last_end,
            atoi(ptr_sendbuf->page_no),
            atoi(ptr_sendbuf->page_cnt),
            ptr_sendbuf->search_type,
            ptr_sendbuf->cust_mode,
            ptr_sendbuf->full_order_no,
            ptr_sendbuf->list_mode,
            ptr_sendbuf->cust_no[0],
            ptr_sendbuf->cust_no[1],
            ptr_sendbuf->cust_no[2],
            ptr_sendbuf->cust_no[3],
            ptr_sendbuf->cust_no[4],
            ptr_sendbuf->cust_no[5],
            ptr_sendbuf->cust_no[6],
            ptr_sendbuf->cust_no[7],
            ptr_sendbuf->cust_no[8],
            ptr_sendbuf->cust_no[9],
            ptr_sendbuf->cust_no[10],
            ptr_sendbuf->cust_no[11],
            ptr_sendbuf->cust_no[12],
            ptr_sendbuf->cust_no[13],
            ptr_sendbuf->cust_no[14],
            ptr_sendbuf->cust_no[15],
            ptr_sendbuf->cust_no[16],
            ptr_sendbuf->cust_no[17],
            ptr_sendbuf->cust_no[18],
            ptr_sendbuf->cust_no[19],
            ptr_sendbuf->cust_no[20],
            ptr_sendbuf->cust_no[21],
            ptr_sendbuf->cust_no[22],
            ptr_sendbuf->cust_no[23],
            ptr_sendbuf->cust_no[24],
            ptr_sendbuf->cust_no[25],
            ptr_sendbuf->cust_no[26],
            ptr_sendbuf->cust_no[27],
            ptr_sendbuf->cust_no[28],
            ptr_sendbuf->cust_no[29],
            ptr_sendbuf->cust_no[30],
            ptr_sendbuf->cust_no[31],
            ptr_sendbuf->cust_no[32],
            ptr_sendbuf->cust_no[33],
            ptr_sendbuf->cust_no[34],
            ptr_sendbuf->cust_no[35],
            ptr_sendbuf->cust_no[36],
            ptr_sendbuf->cust_no[37],
            ptr_sendbuf->cust_no[38],
            ptr_sendbuf->cust_no[39],
            ptr_sendbuf->cust_no[40],
            ptr_sendbuf->cust_no[41],
            ptr_sendbuf->cust_no[42],
            ptr_sendbuf->cust_no[43],
            ptr_sendbuf->cust_no[44],
            ptr_sendbuf->cust_no[45],
            ptr_sendbuf->cust_no[46],
            ptr_sendbuf->cust_no[47],
            ptr_sendbuf->cust_no[48],
            ptr_sendbuf->cust_no[49],
            ptr_sendbuf->cust_no[50],
            ptr_sendbuf->cust_no[51],
            ptr_sendbuf->cust_no[52],
            ptr_sendbuf->cust_no[53],
            ptr_sendbuf->cust_no[54],
            ptr_sendbuf->cust_no[55],
            ptr_sendbuf->cust_no[56],
            ptr_sendbuf->cust_no[57],
            ptr_sendbuf->cust_no[58],
            ptr_sendbuf->cust_no[59],
            ptr_sendbuf->cust_no[60],
            ptr_sendbuf->cust_no[61],
            ptr_sendbuf->cust_no[62],
            ptr_sendbuf->cust_no[63],
            ptr_sendbuf->cust_no[64],
            ptr_sendbuf->cust_no[65],
            ptr_sendbuf->cust_no[66],
            ptr_sendbuf->cust_no[67],
            ptr_sendbuf->cust_no[68],
            ptr_sendbuf->cust_no[69],
            ptr_sendbuf->cust_no[70],
            ptr_sendbuf->cust_no[71],
            ptr_sendbuf->cust_no[72],
            ptr_sendbuf->cust_no[73],
            ptr_sendbuf->cust_no[74],
            ptr_sendbuf->cust_no[75],
            ptr_sendbuf->cust_no[76],
            ptr_sendbuf->cust_no[77],
            ptr_sendbuf->cust_no[78],
            ptr_sendbuf->cust_no[79],
            ptr_sendbuf->cust_no[80],
            ptr_sendbuf->cust_no[81],
            ptr_sendbuf->cust_no[82],
            ptr_sendbuf->cust_no[83],
            ptr_sendbuf->cust_no[84],
            ptr_sendbuf->cust_no[85],
            ptr_sendbuf->cust_no[86],
            ptr_sendbuf->cust_no[87],
            ptr_sendbuf->cust_no[88],
            ptr_sendbuf->cust_no[89],
            ptr_sendbuf->cust_no[90],
            ptr_sendbuf->cust_no[91],
            ptr_sendbuf->cust_no[92],
            ptr_sendbuf->cust_no[93],
            ptr_sendbuf->cust_no[94],
            ptr_sendbuf->cust_no[95],
            ptr_sendbuf->cust_no[96],
            ptr_sendbuf->cust_no[97],
            ptr_sendbuf->cust_no[98],
            ptr_sendbuf->cust_no[99],
            ptr_sendbuf->cust_no[100],
            ptr_sendbuf->cust_no[101],
            ptr_sendbuf->cust_no[102],
            ptr_sendbuf->cust_no[103],
            ptr_sendbuf->cust_no[104],
            ptr_sendbuf->cust_no[105],
            ptr_sendbuf->cust_no[106],
            ptr_sendbuf->cust_no[107],
            ptr_sendbuf->cust_no[108],
            ptr_sendbuf->cust_no[109],
            ptr_sendbuf->cust_no[110],
            ptr_sendbuf->cust_no[111],
            ptr_sendbuf->cust_no[112],
            ptr_sendbuf->cust_no[113],
            ptr_sendbuf->cust_no[114],
            ptr_sendbuf->cust_no[115],
            ptr_sendbuf->cust_no[116],
            ptr_sendbuf->cust_no[117],
            ptr_sendbuf->cust_no[118],
            ptr_sendbuf->cust_no[119],
            ptr_sendbuf->cust_no[120],
            ptr_sendbuf->cust_no[121],
            ptr_sendbuf->cust_no[122],
            ptr_sendbuf->cust_no[123],
            ptr_sendbuf->cust_no[124],
            ptr_sendbuf->cust_no[125],
            ptr_sendbuf->cust_no[126],
            ptr_sendbuf->cust_no[127],
            ptr_sendbuf->cust_no[128],
            ptr_sendbuf->cust_no[129],
            ptr_sendbuf->cust_no[130],
            ptr_sendbuf->cust_no[131],
            ptr_sendbuf->cust_no[132],
            ptr_sendbuf->cust_no[133],
            ptr_sendbuf->cust_no[134],
            ptr_sendbuf->cust_no[135],
            ptr_sendbuf->cust_no[136],
            ptr_sendbuf->cust_no[137],
            ptr_sendbuf->cust_no[138],
            ptr_sendbuf->cust_no[139],
            ptr_sendbuf->cust_no[140],
            ptr_sendbuf->cust_no[141],
            ptr_sendbuf->cust_no[142],
            ptr_sendbuf->cust_no[143],
            ptr_sendbuf->cust_no[144],
            ptr_sendbuf->cust_no[145],
            ptr_sendbuf->cust_no[146],
            ptr_sendbuf->cust_no[147],
            ptr_sendbuf->cust_no[148],
            ptr_sendbuf->cust_no[149],
            ptr_sendbuf->cust_no[150],
            ptr_sendbuf->cust_no[151],
            ptr_sendbuf->cust_no[152],
            ptr_sendbuf->cust_no[153],
            ptr_sendbuf->cust_no[154],
            ptr_sendbuf->cust_no[155],
            ptr_sendbuf->cust_no[156],
            ptr_sendbuf->cust_no[157],
            ptr_sendbuf->cust_no[158],
            ptr_sendbuf->cust_no[159],
            ptr_sendbuf->cust_no[160],
            ptr_sendbuf->cust_no[161],
            ptr_sendbuf->cust_no[162],
            ptr_sendbuf->cust_no[163],
            ptr_sendbuf->cust_no[164],
            ptr_sendbuf->cust_no[165],
            ptr_sendbuf->cust_no[166],
            ptr_sendbuf->cust_no[167],
            ptr_sendbuf->cust_no[168],
            ptr_sendbuf->cust_no[169],
            ptr_sendbuf->cust_no[170],
            ptr_sendbuf->cust_no[171],
            ptr_sendbuf->cust_no[172],
            ptr_sendbuf->cust_no[173],
            ptr_sendbuf->cust_no[174],
            ptr_sendbuf->cust_no[175],
            ptr_sendbuf->cust_no[176],
            ptr_sendbuf->cust_no[177],
            ptr_sendbuf->cust_no[178],
            ptr_sendbuf->cust_no[179],
            ptr_sendbuf->cust_no[180],
            ptr_sendbuf->cust_no[181],
            ptr_sendbuf->cust_no[182],
            ptr_sendbuf->cust_no[183],
            ptr_sendbuf->cust_no[184],
            ptr_sendbuf->cust_no[185],
            ptr_sendbuf->cust_no[186],
            ptr_sendbuf->cust_no[187],
            ptr_sendbuf->cust_no[188],
            ptr_sendbuf->cust_no[189],
            ptr_sendbuf->cust_no[190],
            ptr_sendbuf->cust_no[191],
            ptr_sendbuf->cust_no[192],
            ptr_sendbuf->cust_no[193],
            ptr_sendbuf->cust_no[194],
            ptr_sendbuf->cust_no[195],
            ptr_sendbuf->cust_no[196],
            ptr_sendbuf->cust_no[197],
            ptr_sendbuf->cust_no[198],
            ptr_sendbuf->cust_no[199],
            ptr_sendbuf->cust_no[200],
            ptr_sendbuf->cust_no[201],
            ptr_sendbuf->cust_no[202],
            ptr_sendbuf->cust_no[203],
            ptr_sendbuf->cust_no[204],
            ptr_sendbuf->cust_no[205],
            ptr_sendbuf->cust_no[206],
            ptr_sendbuf->cust_no[207],
            ptr_sendbuf->cust_no[208],
            ptr_sendbuf->cust_no[209],
            ptr_sendbuf->cust_no[210],
            ptr_sendbuf->cust_no[211],
            ptr_sendbuf->cust_no[212],
            ptr_sendbuf->cust_no[213],
            ptr_sendbuf->cust_no[214],
            ptr_sendbuf->cust_no[215],
            ptr_sendbuf->cust_no[216],
            ptr_sendbuf->cust_no[217],
            ptr_sendbuf->cust_no[218],
            ptr_sendbuf->cust_no[219],
            ptr_sendbuf->cust_no[220],
            ptr_sendbuf->cust_no[221],
            ptr_sendbuf->cust_no[222],
            ptr_sendbuf->cust_no[223],
            ptr_sendbuf->cust_no[224],
            ptr_sendbuf->cust_no[225],
            ptr_sendbuf->cust_no[226],
            ptr_sendbuf->cust_no[227],
            ptr_sendbuf->cust_no[228],
            ptr_sendbuf->cust_no[229],
            ptr_sendbuf->cust_no[230],
            ptr_sendbuf->cust_no[231],
            ptr_sendbuf->cust_no[232],
            ptr_sendbuf->cust_no[233],
            ptr_sendbuf->cust_no[234],
            ptr_sendbuf->cust_no[235],
            ptr_sendbuf->cust_no[236],
            ptr_sendbuf->cust_no[237],
            ptr_sendbuf->cust_no[238],
            ptr_sendbuf->cust_no[239],
            ptr_sendbuf->cust_no[240],
            ptr_sendbuf->cust_no[241],
            ptr_sendbuf->cust_no[242],
            ptr_sendbuf->cust_no[243],
            ptr_sendbuf->cust_no[244],
            ptr_sendbuf->cust_no[245],
            ptr_sendbuf->cust_no[246],
            ptr_sendbuf->cust_no[247]);

	return(gbp->j);
}


int ft0002_ParceRecvStr(ft0002_st_recv *ptr_recvbuf, char *sz_recvbufcat, request_rec *r,
                                          struct global_struct *gbp)
{

        gbp->count = 0;
        ap_rprintf(r,"%s\n", xml_vers_message);

#ifdef XSL

        ap_rprintf(r,"<?xml-stylesheet type=\"text/xsl\" href=\"../xml-dtd/ft0002.xsl\"?>\n");

#endif
/*
                ap_rprintf(r,"<!DOCTYPE %s0002 PUBLIC \"-//%s//%s %s//%s\" \n", 
                                                        gbp->tag, originator, label, sg_version, language);

        ap_rprintf(r,"                     \"%s%s0002.dtd\">\n", url_tag, ft_tag);
*/
        ap_rprintf(r,"%s0002 %s\"ft0002\">\n", gbp->ft_btag, gbp->bitag);
        ap_rprintf(r,"%s\n", sga_message);
        ap_rprintf(r,"	%s>\n", gbp->mtag);


    memset(ptr_recvbuf->request_id,'\0', ft0002_REQ_ID_LEN+1);
    memcpy(ptr_recvbuf->request_id, sz_recvbufcat + gbp->count, ft0002_REQ_ID_LEN);
    ap_rprintf(r,"		<REQUEST_ID>%s</REQUEST_ID>\n", handle_special_chars(gbp,ptr_recvbuf->request_id));
    gbp->count += ft0002_REQ_ID_LEN;

    memset(ptr_recvbuf->record_id,'\0', ft0002_REC_ID_LEN+1);
    memcpy(ptr_recvbuf->record_id, sz_recvbufcat + gbp->count, ft0002_REC_ID_LEN );
    gbp->count += ft0002_REC_ID_LEN;

    memset(ptr_recvbuf->userid,'\0', ft0002_USER_ID_LEN+1);
    memcpy(ptr_recvbuf->userid, sz_recvbufcat + gbp->count, ft0002_USER_ID_LEN);
        ap_rprintf(r,"		<UID>%s</UID>\n", handle_special_chars(gbp,ptr_recvbuf->userid));
    gbp->count += ft0002_USER_ID_LEN;

    memset(ptr_recvbuf->success_flag,'\0', ft0002_SUCCESS_FLAG_LEN+1);
    memcpy(ptr_recvbuf->success_flag, sz_recvbufcat + gbp->count, ft0002_SUCCESS_FLAG_LEN);
        ap_rprintf(r,"		<SUCCESS_RESPONSE>%s</SUCCESS_RESPONSE>\n", handle_special_chars(gbp,ptr_recvbuf->success_flag));
    gbp->count += ft0002_SUCCESS_FLAG_LEN;

    memset(ptr_recvbuf->err_message,'\0', ft0002_ERR_LEN + 1);
    memcpy(ptr_recvbuf->err_message, sz_recvbufcat + gbp->count, ft0002_ERR_LEN);
        ap_rprintf(r,"		<MESSAGE>%s</MESSAGE>\n", handle_special_chars(gbp,ptr_recvbuf->err_message));
    gbp->count += ft0002_ERR_LEN;

    gbp->count += ft0002_SEND_FILLER_LEN;

    memset(ptr_recvbuf->cust_count,'\0', ft0002_CNT_LEN + 1);
    memcpy(ptr_recvbuf->cust_count, sz_recvbufcat + gbp->count, ft0002_CNT_LEN);
        ap_rprintf(r,"		<CUSTOMER_COUNT>%s</CUSTOMER_COUNT>\n", handle_special_chars(gbp,ptr_recvbuf->cust_count));
    gbp->count += ft0002_CNT_LEN;

    memset(ptr_recvbuf->page_no,'\0', ft0002_PAGE_NO_LEN + 1);
    memcpy(ptr_recvbuf->page_no, sz_recvbufcat + gbp->count, ft0002_PAGE_NO_LEN);
        ap_rprintf(r,"		<PAGE_NUM>%s</PAGE_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->page_no));
    gbp->count += ft0002_PAGE_NO_LEN;

	if ((gbp->ftsendbuf0002->search_type[0] == 'C') && (gbp->ftsendbuf0002->search_type[0] == 'L'))
	{
	    for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->cust_count); gbp->i++)
		{
			memset(ptr_recvbuf->customer_info[gbp->i].cust_num ,'\0', ft0002_CUST_NO_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].cust_num, sz_recvbufcat + gbp->count, ft0002_CUST_NO_LEN);
			gbp->count += ft0002_CUST_NO_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].fname ,'\0', ft0002_NAME_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].fname, sz_recvbufcat + gbp->count, ft0002_NAME_LEN);
			gbp->count += ft0002_NAME_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].lname ,'\0', ft0002_NAME_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].lname, sz_recvbufcat + gbp->count, ft0002_NAME_LEN);
			gbp->count += ft0002_NAME_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].minitial ,'\0', ft0002_MI_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].minitial, sz_recvbufcat + gbp->count, ft0002_MI_LEN);
			gbp->count += ft0002_MI_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].company ,'\0', ft0002_COMP_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].company, sz_recvbufcat + gbp->count, ft0002_COMP_LEN);
			gbp->count += ft0002_COMP_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].city ,'\0', ft0002_CITY_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].city, sz_recvbufcat + gbp->count, ft0002_CITY_LEN);
			gbp->count += ft0002_CITY_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].state ,'\0', ft0002_STATE_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].state, sz_recvbufcat + gbp->count, ft0002_STATE_LEN);
			gbp->count += ft0002_STATE_LEN;

			memset(ptr_recvbuf->customer_info[gbp->i].zip ,'\0', ft0002_ZIP_LEN + 1);
			memcpy(ptr_recvbuf->customer_info[gbp->i].zip, sz_recvbufcat + gbp->count, ft0002_ZIP_LEN);
			gbp->count += ft0002_ZIP_LEN;

		    ap_rprintf(r,"          <CUSTOMER_INFO>\n");
			ap_rprintf(r,"                  <CUSTOMER_NUM>%s</CUSTOMER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].cust_num));
			ap_rprintf(r,"                  <FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].fname));
			ap_rprintf(r,"                  <LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].lname));
			ap_rprintf(r,"                  <MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].minitial));
			ap_rprintf(r,"                  <COMPANY>%s</COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].company));
			ap_rprintf(r,"                  <CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].city));
			ap_rprintf(r,"                  <STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].state));
			ap_rprintf(r,"                  <ZIP>%s</ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->customer_info[gbp->i].zip));
		    ap_rprintf(r,"          </CUSTOMER_INFO>\n");

		}
	}
    else
	{
		if (gbp->ftsendbuf0002->list_mode[0] == 'N')
		{
			for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->cust_count); gbp->i++)
			{
				memset(ptr_recvbuf->ns_info[gbp->i].cust_num ,'\0', ft0002_CUST_NO_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].cust_num, sz_recvbufcat + gbp->count, ft0002_CUST_NO_LEN);
				gbp->count += ft0002_CUST_NO_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].fname ,'\0', ft0002_NAME_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].fname, sz_recvbufcat + gbp->count, ft0002_NAME_LEN);
				gbp->count += ft0002_NAME_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].lname ,'\0', ft0002_NAME_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].lname, sz_recvbufcat + gbp->count, ft0002_NAME_LEN);
				gbp->count += ft0002_NAME_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].minitial ,'\0', ft0002_MI_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].minitial, sz_recvbufcat + gbp->count, ft0002_MI_LEN);
				gbp->count += ft0002_MI_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].company ,'\0', ft0002_COMP_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].company, sz_recvbufcat + gbp->count, ft0002_COMP_LEN);
				gbp->count += ft0002_COMP_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].street ,'\0', ft0002_STREET_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].street, sz_recvbufcat + gbp->count, ft0002_STREET_LEN);
				gbp->count += ft0002_STREET_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].city ,'\0', ft0002_CITY_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].city, sz_recvbufcat + gbp->count, ft0002_CITY_LEN);
				gbp->count += ft0002_CITY_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].state ,'\0', ft0002_STATE_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].state, sz_recvbufcat + gbp->count, ft0002_STATE_LEN);
				gbp->count += ft0002_STATE_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].zip ,'\0', ft0002_ZIP_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].zip, sz_recvbufcat + gbp->count, ft0002_ZIP_LEN);
				gbp->count += ft0002_ZIP_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].type ,'\0', ft0002_TYPE_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].type, sz_recvbufcat + gbp->count, ft0002_TYPE_LEN);
				gbp->count += ft0002_TYPE_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].status ,'\0', ft0002_STATUS_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].status, sz_recvbufcat + gbp->count, ft0002_STATUS_LEN);
				gbp->count += ft0002_STATUS_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].action ,'\0', ft0002_ACTION_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].action, sz_recvbufcat + gbp->count, ft0002_ACTION_LEN);
				gbp->count += ft0002_ACTION_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].old_cust_flag ,'\0', ft0002_FLAG_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].old_cust_flag, sz_recvbufcat + gbp->count, ft0002_FLAG_LEN);
				gbp->count += ft0002_FLAG_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].chg_user ,'\0', ft0002_USER_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].chg_user, sz_recvbufcat + gbp->count, ft0002_USER_LEN);
				gbp->count += ft0002_USER_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].chg_date ,'\0', ft0002_DATE_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].chg_date, sz_recvbufcat + gbp->count, ft0002_DATE_LEN);
				gbp->count += ft0002_DATE_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].chg_time ,'\0', ft0002_TIME_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].chg_time, sz_recvbufcat + gbp->count, ft0002_TIME_LEN);
				gbp->count += ft0002_TIME_LEN;

				memset(ptr_recvbuf->ns_info[gbp->i].order_no ,'\0', ft0002_ORDER_LEN + 1);
				memcpy(ptr_recvbuf->ns_info[gbp->i].order_no, sz_recvbufcat + gbp->count, ft0002_ORDER_LEN);
				gbp->count += ft0002_ORDER_LEN;

			    ap_rprintf(r,"          <NS_INFO>\n");
    			ap_rprintf(r,"                  <CUSTOMER_NUM>%s</CUSTOMER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].cust_num));
    			ap_rprintf(r,"                  <FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].fname));
    			ap_rprintf(r,"                  <LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].lname));
    			ap_rprintf(r,"                  <MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].minitial));
    			ap_rprintf(r,"                  <COMPANY>%s</COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].company));
    			ap_rprintf(r,"                  <STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].street));
    			ap_rprintf(r,"                  <CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].city));
    			ap_rprintf(r,"                  <STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].state));
    			ap_rprintf(r,"                  <ZIP>%s</ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].zip));
    			ap_rprintf(r,"                  <TYPE>%s</TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].type));
    			ap_rprintf(r,"                  <STATUS>%s</STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].status));
    			ap_rprintf(r,"                  <ACTION>%s</ACTION>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].action));
    			ap_rprintf(r,"                  <OLD_CUSTOMER_FLAG>%s</OLD_CUSTOMER_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].old_cust_flag));
    			ap_rprintf(r,"                  <CHG_USER>%s</CHG_USER>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].chg_user));
    			ap_rprintf(r,"                  <CHG_DATE>%s</CHG_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].chg_date));
    			ap_rprintf(r,"                  <CHG_TIME>%s</CHG_TIME>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].chg_time));
    			ap_rprintf(r,"                  <ORDER_NUM>%s</ORDER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->ns_info[gbp->i].order_no));
			    ap_rprintf(r,"          </NS_INFO>\n");
			}
		}
		else if (gbp->ftsendbuf0002->list_mode[0] == 'D')
		{
			for( gbp->i = 0; gbp->i < atoi(ptr_recvbuf->cust_count); gbp->i++)
			{
				memset(ptr_recvbuf->detail_info[gbp->i].cust_num ,'\0', ft0002_CUST_NO_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].cust_num, sz_recvbufcat + gbp->count, ft0002_CUST_NO_LEN);
				gbp->count += ft0002_CUST_NO_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].fname ,'\0', ft0002_NAME_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].fname, sz_recvbufcat + gbp->count, ft0002_NAME_LEN);
				gbp->count += ft0002_NAME_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].lname ,'\0', ft0002_NAME_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].lname, sz_recvbufcat + gbp->count, ft0002_NAME_LEN);
				gbp->count += ft0002_NAME_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].minitial ,'\0', ft0002_MI_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].minitial, sz_recvbufcat + gbp->count, ft0002_MI_LEN);
				gbp->count += ft0002_MI_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].company ,'\0', ft0002_COMP_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].company, sz_recvbufcat + gbp->count, ft0002_COMP_LEN);
				gbp->count += ft0002_COMP_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].ref1 ,'\0', ft0002_REF_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].ref1, sz_recvbufcat + gbp->count, ft0002_REF_LEN);
				gbp->count += ft0002_REF_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].ref2 ,'\0', ft0002_REF_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].ref2, sz_recvbufcat + gbp->count, ft0002_REF_LEN);
				gbp->count += ft0002_REF_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].street ,'\0', ft0002_STREET_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].street, sz_recvbufcat + gbp->count, ft0002_STREET_LEN);
				gbp->count += ft0002_STREET_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].city ,'\0', ft0002_CITY_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].city, sz_recvbufcat + gbp->count, ft0002_CITY_LEN);
				gbp->count += ft0002_CITY_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].state ,'\0', ft0002_STATE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].state, sz_recvbufcat + gbp->count, ft0002_STATE_LEN);
				gbp->count += ft0002_STATE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].zip ,'\0', ft0002_ZIP_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].zip, sz_recvbufcat + gbp->count, ft0002_ZIP_LEN);
				gbp->count += ft0002_ZIP_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].country_code ,'\0', ft0002_COUNTRY_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].country_code, sz_recvbufcat + gbp->count, ft0002_COUNTRY_CODE_LEN);
				gbp->count += ft0002_COUNTRY_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].country ,'\0', ft0002_COUNTRY_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].country, sz_recvbufcat + gbp->count, ft0002_COUNTRY_LEN);
				gbp->count += ft0002_COUNTRY_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].day_phone ,'\0', ft0002_PHONE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].day_phone, sz_recvbufcat + gbp->count, ft0002_PHONE_LEN);
				gbp->count += ft0002_PHONE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].night_phone ,'\0', ft0002_PHONE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].night_phone, sz_recvbufcat + gbp->count, ft0002_PHONE_LEN);
				gbp->count += ft0002_PHONE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].fax_phone ,'\0', ft0002_PHONE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].fax_phone, sz_recvbufcat + gbp->count, ft0002_PHONE_LEN);
				gbp->count += ft0002_PHONE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].email ,'\0', ft0002_EMAIL_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].email, sz_recvbufcat + gbp->count, ft0002_EMAIL_LEN);
				gbp->count += ft0002_EMAIL_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].title_code ,'\0', ft0002_TITLE_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].title_code, sz_recvbufcat + gbp->count, ft0002_TITLE_CODE_LEN);
				gbp->count += ft0002_TITLE_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].title ,'\0', ft0002_TITLE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].title, sz_recvbufcat + gbp->count, ft0002_TITLE_LEN);
				gbp->count += ft0002_TITLE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].type_code ,'\0', ft0002_TYPE_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].type_code, sz_recvbufcat + gbp->count, ft0002_TYPE_CODE_LEN);
				gbp->count += ft0002_TYPE_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].type ,'\0', ft0002_TYPE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].type, sz_recvbufcat + gbp->count, ft0002_TYPE_LEN);
				gbp->count += ft0002_TYPE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].status_code ,'\0', ft0002_STATUS_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].status_code, sz_recvbufcat + gbp->count, ft0002_STATUS_CODE_LEN);
				gbp->count += ft0002_STATUS_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].status ,'\0', ft0002_STATUS_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].status, sz_recvbufcat + gbp->count, ft0002_STATUS_LEN);
				gbp->count += ft0002_STATUS_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].action_code ,'\0', ft0002_ACTION_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].action_code, sz_recvbufcat + gbp->count, ft0002_ACTION_CODE_LEN);
				gbp->count += ft0002_ACTION_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].action ,'\0', ft0002_ACTION_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].action, sz_recvbufcat + gbp->count, ft0002_ACTION_LEN);
				gbp->count += ft0002_ACTION_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].cust_worth ,'\0', ft0002_CUST_WORTH_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].cust_worth, sz_recvbufcat + gbp->count, ft0002_CUST_WORTH_LEN);
				gbp->count += ft0002_CUST_WORTH_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].last_date ,'\0', ft0002_DATE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].last_date, sz_recvbufcat + gbp->count, ft0002_DATE_LEN);
				gbp->count += ft0002_DATE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].total_order ,'\0', ft0002_TOT_ORDER_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].total_order, sz_recvbufcat + gbp->count, ft0002_TOT_ORDER_LEN);
				gbp->count += ft0002_TOT_ORDER_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].total_dol ,'\0', ft0002_TOT_DOL_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].total_dol, sz_recvbufcat + gbp->count, ft0002_TOT_DOL_LEN);
				gbp->count += ft0002_TOT_DOL_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].password ,'\0', ft0002_PASSWD_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].password, sz_recvbufcat + gbp->count, ft0002_PASSWD_LEN);
				gbp->count += ft0002_PASSWD_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].last_mail_source ,'\0', ft0002_SOURCE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].last_mail_source, sz_recvbufcat + gbp->count, ft0002_SOURCE_LEN);
				gbp->count += ft0002_SOURCE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].old_cust_flag ,'\0', ft0002_FLAG_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].old_cust_flag, sz_recvbufcat + gbp->count, ft0002_FLAG_LEN);
				gbp->count += ft0002_FLAG_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].credits ,'\0', ft0002_CREDIT_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].credits, sz_recvbufcat + gbp->count, ft0002_CREDIT_LEN);
				gbp->count += ft0002_CREDIT_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].no_promo_code ,'\0', ft0002_NO_PROMO_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].no_promo_code, sz_recvbufcat + gbp->count, ft0002_NO_PROMO_CODE_LEN);
				gbp->count += ft0002_NO_PROMO_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].no_promo ,'\0', ft0002_NO_PROMO_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].no_promo, sz_recvbufcat + gbp->count, ft0002_NO_PROMO_LEN);
				gbp->count += ft0002_NO_PROMO_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].no_rent_code ,'\0', ft0002_NO_RENT_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].no_rent_code, sz_recvbufcat + gbp->count, ft0002_NO_RENT_CODE_LEN);
				gbp->count += ft0002_NO_RENT_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].no_rent ,'\0', ft0002_NO_RENT_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].no_rent, sz_recvbufcat + gbp->count, ft0002_NO_RENT_LEN);
				gbp->count += ft0002_NO_RENT_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].terms_days ,'\0', ft0002_DAY_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].terms_days, sz_recvbufcat + gbp->count, ft0002_DAY_LEN);
				gbp->count += ft0002_DAY_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].terms_pct ,'\0', ft0002_PCT_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].terms_pct, sz_recvbufcat + gbp->count, ft0002_PCT_LEN);
				gbp->count += ft0002_PCT_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].cr_limit ,'\0', ft0002_CR_LIMIT_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].cr_limit, sz_recvbufcat + gbp->count, ft0002_CR_LIMIT_LEN);
				gbp->count += ft0002_CR_LIMIT_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].contract ,'\0', ft0002_CONTRACT_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].contract, sz_recvbufcat + gbp->count, ft0002_CONTRACT_LEN);
				gbp->count += ft0002_CONTRACT_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].cod_limit ,'\0', ft0002_COD_LIMIT_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].cod_limit, sz_recvbufcat + gbp->count, ft0002_COD_LIMIT_LEN);
				gbp->count += ft0002_COD_LIMIT_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].cust_edp ,'\0', ft0002_CUST_EDP_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].cust_edp, sz_recvbufcat + gbp->count, ft0002_CUST_EDP_LEN);
				gbp->count += ft0002_CUST_EDP_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].reg_flag ,'\0', ft0002_FLAG_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].reg_flag, sz_recvbufcat + gbp->count, ft0002_FLAG_LEN);
				gbp->count += ft0002_FLAG_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].first_date ,'\0', ft0002_DATE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].first_date, sz_recvbufcat + gbp->count, ft0002_DATE_LEN);
				gbp->count += ft0002_DATE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].old_cust_no ,'\0', ft0002_CUST_NO_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].old_cust_no, sz_recvbufcat + gbp->count, ft0002_CUST_NO_LEN);
				gbp->count += ft0002_CUST_NO_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].orig_source ,'\0', ft0002_SOURCE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].orig_source, sz_recvbufcat + gbp->count, ft0002_SOURCE_LEN);
				gbp->count += ft0002_SOURCE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].member_status ,'\0', ft0002_MEMBER_STATUS_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].member_status, sz_recvbufcat + gbp->count, ft0002_MEMBER_STATUS_LEN);
				gbp->count += ft0002_MEMBER_STATUS_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].gender_code ,'\0', ft0002_GENDER_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].gender_code, sz_recvbufcat + gbp->count, ft0002_GENDER_CODE_LEN);
				gbp->count += ft0002_GENDER_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].gender ,'\0', ft0002_GENDER_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].gender, sz_recvbufcat + gbp->count, ft0002_GENDER_LEN);
				gbp->count += ft0002_GENDER_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].lang_code ,'\0', ft0002_LANG_CODE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].lang_code, sz_recvbufcat + gbp->count, ft0002_LANG_CODE_LEN);
				gbp->count += ft0002_LANG_CODE_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].language ,'\0', ft0002_LANG_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].language, sz_recvbufcat + gbp->count, ft0002_LANG_LEN);
				gbp->count += ft0002_LANG_LEN; 

				memset(ptr_recvbuf->detail_info[gbp->i].ups_type ,'\0', ft0002_UPS_TYPE_LEN + 1);
				memcpy(ptr_recvbuf->detail_info[gbp->i].ups_type, sz_recvbufcat + gbp->count, ft0002_UPS_TYPE_LEN);
				gbp->count += ft0002_UPS_TYPE_LEN; 

			    ap_rprintf(r,"          <DETAIL_INFO>\n");
    			ap_rprintf(r,"                  <CUSTOMER_NUM>%s</CUSTOMER_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].cust_num));
    			ap_rprintf(r,"                  <FNAME>%s</FNAME>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].fname));
    			ap_rprintf(r,"                  <LNAME>%s</LNAME>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].lname));
    			ap_rprintf(r,"                  <MINITIAL>%s</MINITIAL>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].minitial));
    			ap_rprintf(r,"                  <COMPANY>%s</COMPANY>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].company));
    			ap_rprintf(r,"                  <REF1>%s</REF1>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].ref1));
    			ap_rprintf(r,"                  <REF2>%s</REF2>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].ref2));
    			ap_rprintf(r,"                  <STREET>%s</STREET>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].street));
    			ap_rprintf(r,"                  <CITY>%s</CITY>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].city));
    			ap_rprintf(r,"                  <STATE>%s</STATE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].state));
    			ap_rprintf(r,"                  <ZIP>%s</ZIP>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].zip));
    			ap_rprintf(r,"                  <COUNTRY_CODE>%s</COUNTRY_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].country_code));
    			ap_rprintf(r,"                  <COUNTRY>%s</COUNTRY>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].country));
    			ap_rprintf(r,"                  <DAY_PHONE>%s</DAY_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].day_phone));
    			ap_rprintf(r,"                  <NIGHT_PHONE>%s</NIGHT_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].night_phone));
    			ap_rprintf(r,"                  <FAX_PHONE>%s</FAX_PHONE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].fax_phone));
    			ap_rprintf(r,"                  <EMAIL>%s</EMAIL>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].email));
    			ap_rprintf(r,"                  <TITLE_CODE>%s</TITLE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].title_code));
    			ap_rprintf(r,"                  <TITLE>%s</TITLE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].title));
    			ap_rprintf(r,"                  <TYPE_CODE>%s</TYPE_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].type_code));
    			ap_rprintf(r,"                  <TYPE>%s</TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].type));
    			ap_rprintf(r,"                  <STATUS_CODE>%s</STATUS_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].status_code));
    			ap_rprintf(r,"                  <STATUS>%s</STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].status));
    			ap_rprintf(r,"                  <ACTION_CODE>%s</ACTION_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].action_code));
    			ap_rprintf(r,"                  <ACTION>%s</ACTION>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].action));
    			ap_rprintf(r,"                  <CUSTOMER_WORTH>%s</CUSTOMER_WORTH>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].cust_worth));
    			ap_rprintf(r,"                  <LAST_DATE>%s</LAST_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].last_date));
    			ap_rprintf(r,"                  <TOTAL_ORDER>%s</TOTAL_ORDER>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].total_order));
    			ap_rprintf(r,"                  <TOTAL_DOL>%s</TOTAL_DOL>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].total_dol));
    			ap_rprintf(r,"                  <PASSWORD>%s</PASSWORD>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].password));
    			ap_rprintf(r,"                  <LAST_MAIL_SOURCE>%s</LAST_MAIL_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].last_mail_source));
    			ap_rprintf(r,"                  <OLD_CUST_FLAG>%s</OLD_CUST_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].old_cust_flag));
    			ap_rprintf(r,"                  <CREDITS>%s</CREDITS>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].credits));
    			ap_rprintf(r,"                  <NO_PROMO_CODE>%s</NO_PROMO_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].no_promo_code));
    			ap_rprintf(r,"                  <NO_PROMO>%s</NO_PROMO>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].no_promo));
    			ap_rprintf(r,"                  <NO_RENT_CODE>%s</NO_RENT_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].no_rent_code));
    			ap_rprintf(r,"                  <NO_RENT>%s</NO_RENT>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].no_rent));
    			ap_rprintf(r,"                  <TERMS_DAYS>%s</TERMS_DAYS>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].terms_days));
    			ap_rprintf(r,"                  <TERMS_PCT>%s</TERMS_PCT>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].terms_pct));
    			ap_rprintf(r,"                  <CR_LIMIT>%s</CR_LIMIT>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].cr_limit));
    			ap_rprintf(r,"                  <CONTRACT>%s</CONTRACT>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].contract));
    			ap_rprintf(r,"                  <COD_LIMIT>%s</COD_LIMIT>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].cod_limit));
    			ap_rprintf(r,"                  <CUST_EDP>%s</CUST_EDP>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].cust_edp));
    			ap_rprintf(r,"                  <REG_FLAG>%s</REG_FLAG>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].reg_flag));
    			ap_rprintf(r,"                  <FIRST_DATE>%s</FIRST_DATE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].first_date));
    			ap_rprintf(r,"                  <OLD_CUST_NUM>%s</OLD_CUST_NUM>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].old_cust_no));
    			ap_rprintf(r,"                  <ORIG_SOURCE>%s</ORIG_SOURCE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].orig_source));
    			ap_rprintf(r,"                  <MEMBER_STATUS>%s</MEMBER_STATUS>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].member_status));
    			ap_rprintf(r,"                  <GENDER_CODE>%s</GENDER_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].gender_code));
    			ap_rprintf(r,"                  <GENDER>%s</GENDER>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].gender));
    			ap_rprintf(r,"                  <LANG_CODE>%s</LANG_CODE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].lang_code));
    			ap_rprintf(r,"                  <LANG>%s</LANG>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].language));
    			ap_rprintf(r,"                  <UPS_TYPE>%s</UPS_TYPE>\n", handle_special_chars(gbp,ptr_recvbuf->detail_info[gbp->i].ups_type));
			    ap_rprintf(r,"          </DETAIL_INFO>\n");
			}
		}

	}

    
        ap_rprintf(r,"	%s>\n", gbp->metag);
        ap_rprintf(r,"%s\n", pt_message);
        ap_rprintf(r,"	%s>\n", gbp->rstag);

        reparse_customer_data(r,gbp);

        ap_rprintf(r,"	%s>\n", gbp->rsetag);
        ap_rprintf(r,"%s0002>\n", gbp->ft_betag);


    return(0);
}








