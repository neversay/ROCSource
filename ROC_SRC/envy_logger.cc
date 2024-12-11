#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define MAXLINE 4096
#define SIZ 500			   /* various buffers */

struct desc_data {
    int desc;
    char *name, *host, *remoteuser;
    desc_data *next;
}*desc_list=NULL, *pdesc=NULL;

extern int errno;
unsigned short auth_tcpport = 113;

char *auth_tcpuser(long unsigned int in, short unsigned int local, short unsigned int remote);
void log_string_now(const char*);

int main()
{
    int n, addr;
    int desc, remote, local, address;
    struct hostent *from;
    char *user;
    char name[MAXLINE];
    char line[MAXLINE];
    char *p=line;
    char outline[MAXLINE];
    char addr_buff[64];
    bool f=true;
		srand(time(0));
    while ( (n = read(STDIN_FILENO, p, MAXLINE-(p-line)-1)) > 0) {
	p[n]='\0';
	for (p=line;p<strchr(line, '\0');p=strchr(p, '\n')+1) {
	    if (strchr(p, '\n')==NULL) {
		memmove(line, p, strlen(p)+1);
		p=strchr(line, '\0');
		f=false;
		break;
	    }
	    if (p[0]=='|' && p[1]!='|') {
		switch(p[1]) {
		    case 'D':
			sscanf(p, "|D %d %d %d %d \n", &desc, &address, &remote, &local);
			pdesc=new desc_data;
			pdesc->next=desc_list;
			desc_list=pdesc;
			pdesc->desc=desc;
			pdesc->name=pdesc->host=pdesc->remoteuser=NULL;
			from=gethostbyaddr( (char*)&address,sizeof(addr),AF_INET);
			addr = ntohl(address);
			sprintf( addr_buff, "%d.%d.%d.%d",
			    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
			    ( addr >>  8 ) & 0xFF, ( addr	 ) & 0xFF );
			pdesc->host=new char[strlen(from?from->h_name:addr_buff)+1];
			strcpy(pdesc->host, from?from->h_name:addr_buff);
			user=auth_tcpuser(address, remote, local);
			pdesc->remoteuser=new char[strlen(user)+1];
			strcpy(pdesc->remoteuser, user);
//			log_string_now("Envy_log: Add pdesc here! => ");
//			log_string_now(pdesc->host);
//			log_string_now("\n");
			break;
		    case 'N':
			sscanf(p, "|N %d %s \n", &desc, name);
			for (pdesc=desc_list;pdesc!=NULL;pdesc=pdesc->next) {
			    if (pdesc->desc==desc)
				break;
			}
			if (pdesc==NULL) {
			    log_string_now("Envy_log: Child: Bad desc1.\n");
			    break;
			}
			pdesc->name=new char[strlen(name)+1];
			strcpy(pdesc->name, name);
			sprintf(outline, "|H %d %s %s \n", pdesc->desc, name, pdesc->host);
			write(STDOUT_FILENO, outline, strlen(outline));
			sprintf(outline, "|U %d %s %s \n", pdesc->desc, name, pdesc->remoteuser);
			write(STDOUT_FILENO, outline, strlen(outline));
			break;
		    case 'P':
			sscanf(p, "|P %d \n", &desc);
			for (pdesc=desc_list;pdesc!=NULL;pdesc=pdesc->next) {
			    if (pdesc->desc==desc)
				break;
			}
			if (pdesc==NULL) {
			    log_string_now("Envy_log: Child: Bad desc2.\n");
			    break;
			}
			break;
		    case 'Q': {
			sscanf(p, "|Q %d \n", &desc);
			desc_data *dprev=NULL;
			for (pdesc=desc_list;pdesc!=NULL;dprev=pdesc,pdesc=pdesc->next) {
			    if (pdesc->desc==desc)
				break;
			}
			if (pdesc==NULL) {
			    log_string_now("Envy_log: Child: Bad desc3.\n");
			    break;
			}
//			log_string_now("Envy_log: Del pdesc here! => ");
//			log_string_now(pdesc->host);
//			log_string_now("\n");
			delete pdesc->name;
			delete pdesc->host;
			delete pdesc->remoteuser;
			if (desc_list==pdesc)
			    desc_list=pdesc->next;
			else
			    dprev->next=pdesc->next;
			delete pdesc;
			break;
		    }
		    default:
//			if (p==NULL) {
//			    log_string_now("Child: Bad desc4.\n");
//			    break;
//			}
			log_string_now(p);
			break;
		}
	    }
	    else {
//		if (p==NULL) {
//		    log_string_now("Child: Bad desc5.\n");
//		    break;
//		}
		log_string_now(p);
	    }
	}
	if (f)
	    p=line;
	else
	    f=true;
    }
    return 0;
}


static char ruser[SIZ];
static char realbuf[SIZ];
static char *buf;
static char READ_ERROR[] = "READ_ERROR";
static char SEND_ERROR[] = "SEND_ERROR";
static char BAD_STRING[] = "BAD_STRING";
static char REMOTE_LOCAL[] = "REMOTE_LOCAL";
static char UNKNOWN[] = "UNKNOWN";

/* This was taken from public domain source that I managed to dredge up,
 * I changed a little of it, but I can't claim that I wrote it all.  I don't
 * know who the original author is, so I can't give him/her credit. */

char *
auth_tcpuser(long unsigned int in, short unsigned int local, short unsigned int remote)
{
    struct sockaddr_in sa;
    int s;
    int buflen;
    int w;
    int saveerrno;
    char ch;
    unsigned short rlocal;
    unsigned short rremote;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	return 0;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(auth_tcpport);
    sa.sin_addr.s_addr = in;
    if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
	saveerrno = errno;
	(void) close(s);
	errno = saveerrno;
	return(UNKNOWN);
    }
    buf = realbuf;
    (void) sprintf(buf, "%u , %u\r\n", (unsigned int) remote, (unsigned int) local);
    /* note the reversed order, the example in the RFC is misleading */
    buflen = strlen(buf);
    while ((w = write(s, buf, buflen)) < buflen)
	if (w == -1) {		  /* should we worry about 0 as well? */
	    saveerrno = errno;
	    (void) close(s);
	    errno = saveerrno;
	    return(SEND_ERROR);
	} else {
	    buf += w;
	    buflen -= w;
	}
    buf = realbuf;
    while ((w = read(s, &ch, 1)) == 1) {
	*buf = ch;
	if ((ch != ' ') && (ch != '\t') && (ch != '\r'))
	    ++buf;
	if ((buf - realbuf == sizeof(realbuf) - 1) || (ch == '\n'))
	    break;
    }
    if (w == -1) {
	saveerrno = errno;
	(void) close(s);
	errno = saveerrno;
	return(READ_ERROR);
    }
    *buf = '\0';
    bzero(ruser,SIZ); /* can do a memset if you need to */
    if (sscanf(realbuf, "%hd,%hd: USERID :%*[^:]:%499s", &rremote, &rlocal, ruser) < 3) {
	(void) close(s);
	errno = EOF;
	/* makes sense, right? well, not when USERID failed to match ERROR
	 * but there's no good error to return in that case */
	return(BAD_STRING);
    }
    if ((remote != rremote) || (local != rlocal)) {
	(void) close(s);
	errno = EOF;
	return(REMOTE_LOCAL);
    }
    (void) close(s);
    return ruser;
}

static char dollar_sign[] = "$";
static char empty_string[] = "";
void log_string_now( const char *str ) {
    char buf[MAXLINE], *ptr=buf;
    char *i;
    bool fChinese=false;
    while (*str!='\n') {
	if (fChinese) {
	    *ptr++=*str++;
	    fChinese=false;
	}
	else {
	    if (*str=='$') {
		str++;
		switch(*str) {
		    case 'H':
			if (pdesc)
			i=pdesc->host;
			else
			i=empty_string;
			break;
		    case 'U':
			if (pdesc)
			i=pdesc->remoteuser;
			else
			i=empty_string;
			break;
		    case '$':
			i=dollar_sign;
			break;
		    default:
			i=empty_string;
		}
		strcpy(ptr, i);
		str++;
		ptr+=strlen(i);
	    }
	    else {
		if ((unsigned char)(*str) >= 128)
		    fChinese=true;
		*ptr++=*str++;
	    }
	}
    }
    *ptr++='\n';
    *ptr='\0';
    fprintf(stderr, buf);
    fflush(stderr);
    return;
}
