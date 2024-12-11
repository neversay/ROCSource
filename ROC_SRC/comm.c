/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael    *
 *  Chastain, Michael Quan, and Mitchell Tse.          *
 *                     *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.         *
 *                     *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and    *
 *  Mitchell Tse.                *
 *                     *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.     *
 *                     *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in    *
 *  'license.txt', as well as the Envy license in 'license.nvy'.     *
 *  In particular, you may not remove either of these copyright notices.   *
 *                     *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)      *
 *                     *
 *  Much time and thought has gone into this software and you are    *
 *  benefitting.  We hope that you share your changes too.  What goes    *
 *  around, comes around.              *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */
//*****************************************************************
//*  PROGRAM ID  : comm.c                                         *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         ­×¥¿­^¤å°T®§¡ÏÂ²µuªº´y­z¥X²{¶Ã½X¤§bug  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940619 Razgriz                                *
//*  PURPOSE     :         ­×¥¿¦]¿é¤J¿ù»~±K½X³y¦¨ª±®aÀÉ®×­«½Æ²Ö¿n *
//*              :         ¦b°O¾ÐÅé¤¤¡A¨Ï±o¦b¨Ï¥Î§ä´Mª««~ªº«ü¥O®É *
//*              :         µo¥Í­«½ÆÅã¥Üªº bug                     *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940802 Razgriz                                *
//*  PURPOSE     :         ­×¥¿¦]µn¤J½u¤W¤w¸g¦s¦bªº¸}¦â³y¦¨ª±®a   *
//*              :         ÀÉ®×­«½Æ²Ö¿n¦b°O¾ÐÅé¤¤¡A¨Ï±o¦b¨Ï¥Î§ä´M *
//*              :         ª««~ªº«ü¥O¥H¤Î obj prog ®Éµo¥Í­«½Æ     *
//*              :         Åã¥Ü©Î¼vÅTªº bug                       *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940825 Razgriz                                *
//*  PURPOSE     :         ­×§ï close_socket() ¦b¸Ó¨ç¼Æ«Å§i«e¥[¤J *
//*              :         fwrite_neweqbank ¸ò free_neweqbank ªº  *
//*              :         extern ¨ç¼Æ«Å§i, ¥H¤Î¦b close_socket() *
//*              :         ¤¤, ¥[¤J§PÂ_·íª±®aÂ_½u®É, ¦Û°ÊÃö³¬¨ä   *
//*              :         ¥´¶}ªº eqbank, ¥HÁ×§K¦³¤H§Q¥ÎÂ_½u¨Ó­«½Æ*
//*              :         ¶}±Ò eqbank ¨Ó copy eq ªº bug.         *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940912 jye                                    *
//*  PURPOSE     :         ¼W¥[Multi LoginÄµ§i°T®§                *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940923 Razgriz                                *
//*  PURPOSE     :         ­×§ïÅwªïµe­±ªº¹ê»Ú¤H¼Æ»P¨¤¦â¼ÆªºÅã¥Ü   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941003 jye                                    *
//*  PURPOSE     :         ­×§ïimm char ­«·s³s½u¤£Åãµø            *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0941009 jye                                    *
//*  PURPOSE     :         ­×§ïMulti Login¤§bug                   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         ·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ        *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 1050417 jye                                    *
//*  PURPOSE     :         ­×§ï¤£ÀË®ÖMulti Login                  *
//*****************************************************************
#define LINUX2
#define MAX_MULTI_LOGIN 8

#if defined( WIN32 )
char version_str[] = "$VER: EnvyMud 2.0 Windows 32 Bit Version";
/*
 * Provided by Mystro <http://www.cris.com/~Kendugas/mud.shtml>
 */
#endif

#if defined( AmigaTCP )
char version_str[] = "$VER: Merc/Diku Mud Envy2.0 AmiTCP Version";
/*
 * You must rename or delete the sc:sys/types.h, so the 
 * amitcp:netinclude/sys/types.h will be used instead.
 * Also include these assigns in your user-startup (After the SC assigns)
 *    assign lib: Amitcp:netlib add 
 *    assign include: Amitcp:netinclude add
 * If you haven't allready :)
 * Compilled with SasC 6.56 and AmiTCP 4.2
 * http://www.geocities.com/SiliconValley/1411
 * dkaupp@netcom.com (May be defunct soon)
 * or davek@megnet.net
 * 4-16-96
 */
#endif

#if !defined( WIN32 ) && !defined( AmigaTCP )
char version_str[] = "$VER: EnvyMud 2.0 *NIX";
#endif

#if defined( macintosh )
#include <types.h>
#else
#define FD_SETSIZE 1024
#include <sys/types.h>
#if !defined( WIN32 )
#include <sys/time.h>
/*#include <setjmp.h>*/
#if defined( unix )
#include <unistd.h>
#endif
#endif
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined( WIN32 )
#include <winsock.h>
#include <sys/timeb.h> /*for _ftime(), uses _timeb struct*/
#endif

#include "merc.h"
#include "ansi.h"


/*
 * Malloc debugging stuff.
 */
#if defined( sun )
#undef MALLOC_DEBUG
#endif

#if defined( MALLOC_DEBUG )
#include <malloc.h>
extern        int  malloc_debug      args( ( int  ) );
extern        int  malloc_verify        args( ( void ) );
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute( atomic ) in signal.h,
 *   I dance around it.
 */
#if defined( apollo )
#define __attribute( x )
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
#include <signal.h>
#endif

#if defined( apollo )
#undef __attribute
#endif



/*
 * Socket and TCP/IP stuff.
 */
#if    defined( macintosh )
const      char  echo_off_str      [] = { '\0' };
const      char  echo_on_str      [] = { '\0' };
const      char   go_ahead_str        [] = { '\0' };
#endif

#if    defined( unix ) || defined( AmigaTCP )
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const      char  echo_off_str      [] = { IAC, WILL, TELOPT_ECHO, '\0' };
const      char  echo_on_str      [] = { IAC, WONT, TELOPT_ECHO, '\0' };
const      char   go_ahead_str        [] = { IAC, GA, '\0' };
#endif

#ifndef _MAX_TRYTIMES_
#define _MAX_TRYTIMES_  (3)
#endif

/*
 * OS-dependent declarations.
 */
#if    defined( _AIX )
#include <sys/select.h>
int    accept    args( ( int s, struct sockaddr *addr, int *addrlen ) );
int    bind          args( ( int s, struct sockaddr *name, int namelen ) );
void      bzero    args( ( char *b, int length ) );
int    getpeername        args( ( int s, struct sockaddr *name, int *namelen ) );
int    getsockname        args( ( int s, struct sockaddr *name, int *namelen ) );
int    gettimeofday        args( ( struct timeval *tp, struct timezone *tzp ) );
int    listen    args( ( int s, int backlog ) );
int    setsockopt      args( ( int s, int level, int optname, void *optval,
             int optlen ) );
int    socket    args( ( int domain, int type, int protocol ) );
#endif

#if defined( irix )
void      bzero    args( ( char *b, int length ) );
int    read          args( ( int fd, char *buf, int nbyte ) );
int    write    args( ( int fd, char *buf, int nbyte ) );
int    close    args( ( int fd ) );
int    select    args( ( int width, fd_set *readfds, fd_set *writefds,
          fd_set *exceptfds, struct timeval *timeout ) );
#endif

#if    defined( __hpux )
int    accept    args( ( int s, void *addr, int *addrlen ) );
int    bind          args( ( int s, const void *addr, int addrlen ) );
void      bzero    args( ( char *b, int length ) );
int    getpeername        args( ( int s, void *addr, int *addrlen ) );
int    getsockname        args( ( int s, void *name, int *addrlen ) );
int    gettimeofday        args( ( struct timeval *tp, struct timezone *tzp ) );
int    listen    args( ( int s, int backlog ) );
int    setsockopt      args( ( int s, int level, int optname,
             const void *optval, int optlen ) );
int    socket    args( ( int domain, int type, int protocol ) );
#endif

#if defined( interactive )
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if    defined( macintosh )
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct        timeval
{
  time_t        tv_sec;
  time_t        tv_usec;
};
#if    !defined( isascii )
#define        isascii( c )      ( ( c ) < 0200 )
#endif
static        long        theKeys  [4];

int    gettimeofday        args( ( struct timeval *tp, void *tzp ) );
#endif

#if    defined( MIPS_OS )
extern        int    errno;
#endif

#if    defined( WIN32 )
const char echo_off_str[]  = { '\0' };
const char echo_on_str[]   = { '\0' };
const char go_ahead_str[]  = { '\0' };
void  gettimeofday  args( ( struct timeval *tp, void *tzp ) );
#endif

#if    defined( NeXT )
int    close    args( ( int fd ) );
int    fcntl    args( ( int fd, int cmd, int arg ) );
#if    !defined( htons )
u_short        htons        args( ( u_short hostshort ) );
#endif
#if    !defined( ntohl )
u_long        ntohl      args( ( u_long hostlong ) );
#endif
int    read          args( ( int fd, char *buf, int nbyte ) );
int    select    args( ( int width, fd_set *readfds, fd_set *writefds,
             fd_set *exceptfds, struct timeval *timeout ) );
int    write    args( ( int fd, char *buf, int nbyte ) );
#endif

#if    defined( sequent )
int    accept    args( ( int s, struct sockaddr *addr, int *addrlen ) );
int    bind          args( ( int s, struct sockaddr *name, int namelen ) );
int    close    args( ( int fd ) );
int    fcntl    args( ( int fd, int cmd, int arg ) );
int    getpeername        args( ( int s, struct sockaddr *name, int *namelen ) );
int    getsockname        args( ( int s, struct sockaddr *name, int *namelen ) );
int    gettimeofday        args( ( struct timeval *tp, struct timezone *tzp ) );
#if    !defined( htons )
u_short        htons        args( ( u_short hostshort ) );
#endif
int    listen    args( ( int s, int backlog ) );
#if    !defined( ntohl )
u_long        ntohl      args( ( u_long hostlong ) );
#endif
int    read          args( ( int fd, char *buf, int nbyte ) );
int    select    args( ( int width, fd_set *readfds, fd_set *writefds,
             fd_set *exceptfds, struct timeval *timeout ) );
int    setsockopt      args( ( int s, int level, int optname, caddr_t optval,
             int optlen ) );
int    socket    args( ( int domain, int type, int protocol ) );
int    write    args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well
 */

#if defined( sun )
int    accept    args( ( int s, struct sockaddr *addr, int *addrlen ) );
int    bind          args( ( int s, struct sockaddr *name, int namelen ) );
void      bzero    args( ( char *b, int length ) );
int    close    args( ( int fd ) );
int    getpeername        args( ( int s, struct sockaddr *name, int *namelen ) );
int    getsockname        args( ( int s, struct sockaddr *name, int *namelen ) );
#if !defined( SYSV )
int    gettimeofday        args( ( struct timeval *tp, struct timezone *tzp ) );
#endif
int    listen    args( ( int s, int backlog ) );
int    read          args( ( int fd, char *buf, int nbyte ) );
int    select    args( ( int width, fd_set *readfds, fd_set *writefds,
             fd_set *exceptfds, struct timeval *timeout ) );
#if defined( SYSV )
int setsockopt  args( ( int s, int level, int optname,
             const char *optval, int optlen ) );
#else
int    setsockopt      args( ( int s, int level, int optname, void *optval,
             int optlen ) );
#endif
int    socket    args( ( int domain, int type, int protocol ) );
int    write    args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined( ultrix )
int    accept    args( ( int s, struct sockaddr *addr, int *addrlen ) );
int    bind          args( ( int s, struct sockaddr *name, int namelen ) );
void      bzero    args( ( char *b, int length ) );
int    close    args( ( int fd ) );
int    getpeername        args( ( int s, struct sockaddr *name, int *namelen ) );
int    getsockname        args( ( int s, struct sockaddr *name, int *namelen ) );
int    gettimeofday        args( ( struct timeval *tp, struct timezone *tzp ) );
int    listen    args( ( int s, int backlog ) );
int    read          args( ( int fd, char *buf, int nbyte ) );
int    select    args( ( int width, fd_set *readfds, fd_set *writefds,
             fd_set *exceptfds, struct timeval *timeout ) );
int    setsockopt      args( ( int s, int level, int optname, void *optval,
             int optlen ) );
int    socket    args( ( int domain, int type, int protocol ) );
int    write    args( ( int fd, char *buf, int nbyte ) );
#endif


#if defined( sun )
int system    args( ( const char *string ) );
#endif




/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;      /* Free list for descriptors  */
DESCRIPTOR_DATA *   descriptor_list;      /* All open descriptors      */
DESCRIPTOR_DATA *   d_next;      /* Next descriptor in loop      */
FILE *        fpReserve;        /* Reserved file handle      */
bool      merc_down;      /* Shutdown       */
bool      wizlock;    /* Game is wizlocked        */
int       numlock = 0;  /* Game is numlocked at <level> */
char      str_boot_time [ MAX_INPUT_LENGTH ];
time_t      current_time;        /* Time of this pulse        */
time_t      boot_time;
time_t      down_time;
pid_t     pid;
u_short     port;
bool      fSeekPipe=TRUE;



/*
 * OS-dependent local functions.
 */
 
void check_login ( CHAR_DATA *ch); // add by jye 0940912
void abnormal_eq_check(CHAR_DATA *ch); // added by keelar
void abnormal_affect_check(CHAR_DATA *ch); // added by keelar
#if defined( macintosh )
void      game_loop_mac_msdos        args( ( void ) );
bool      read_from_descriptor  args( ( DESCRIPTOR_DATA *d ) );
bool      write_to_descriptor        args( ( int desc, char *txt, int length ) );
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
void      game_loop_unix      args( ( int control ) );
int    init_socket          args( ( u_short port ) );
void      new_descriptor      args( ( int control ) );
bool      read_from_descriptor  args( ( DESCRIPTOR_DATA *d ) );
bool      write_to_descriptor        args( ( int desc, char *txt, int length ) );
/*#if defined( WIN32 )
#define rfc931_name( desc, there )    ( "unknown" )
#else
#define FROM_UNKNOWN  "unknown" *//* name or address lookup failed *//*
#define RFC931_PORT 113   *//* Semi-well-known port *//*
#define TIMEOUT   1   *//* wait for at most 1 second *//*
static void   timeout          args( ( int sig ) );
char *    rfc931_name   args( ( int desc, struct sockaddr_in *there ) );
#endif*/
#endif




/*
 * Other local functions (OS-independent).
 */
bool      check_parse_name      args( ( char *name ) );
bool      check_reconnect      args( ( DESCRIPTOR_DATA *d, char *name,
               bool fConn ) );
bool      check_playing    args( ( DESCRIPTOR_DATA *d, char *name ) );
int    main            args( ( int argc, char **argv ) );
void      nanny      args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool      process_output      args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void      read_from_buffer      args( ( DESCRIPTOR_DATA *d ) );
void      stop_idling          args( ( CHAR_DATA *ch ) );
void  bust_a_prompt   args( ( DESCRIPTOR_DATA *d ) );
void      talk_channel  args( ( CHAR_DATA *ch, char *argument,
          int channel, const char *verb, const char *color) );
void  combat_info_message args( ( const char *format, CHAR_DATA *to, 
          CHAR_DATA *ch, CHAR_DATA *vch ) );

int main( int argc, char **argv )
{
    struct  timeval now_time;

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
    int control;
#endif

    /*
     * Memory debugging if needed.
     */
#if defined( MALLOC_DEBUG )
    malloc_debug( 2 );
#endif


    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    boot_time = current_time;
    strcpy( str_boot_time, ctime( &current_time ) );                       

    /*
     * Macintosh console initialization.
     */
#if defined( macintosh )
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( !( fpReserve = fopen( NULL_FILE, "r" ) ) )
    {
        perror( NULL_FILE );
        exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 4000;
    if ( argc > 1 )
    {
      if ( !is_number( argv[1] ) )
      {
          fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
          exit( 1 );
      }
      else if ( ( port = atoi( argv[1] ) ) <= 1024 )
      {
          fprintf( stderr, "Port number must be above 1024.\n" );
          exit( 1 );
      }
    }

    /*
     * Run the game.
     */
#if defined( macintosh )
    boot_db( );
    log_string( "EnvyMud is ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
    control = init_socket( port );
    if (pipe(fd0)<0||pipe(fd1)<0) {
  fprintf(stderr, "Error opening pipe.\n");
  exit(1);
    }
    if ((pid=fork())<0) {
  fprintf(stderr, "Error forking.\n");
  exit(1);
    }
    else if (pid>0) {
  close(fd0[0]);
  close(fd1[1]);
  fcntl(fd0[1], F_SETFL, O_NONBLOCK);
  fcntl(fd1[0], F_SETFL, O_NONBLOCK);
  boot_db( );
  sprintf( log_buf, "EnvyMud is ready to rock on port %d.", port );
  log_string( log_buf );
  game_loop_unix( control );
#if !defined( WIN32 )
  close( control );
  close(fd0[1]);
  close(fd1[0]);
#else
  closesocket( control );
  WSACleanup();
#endif
#endif

    /*
     * That's all, folks.
     */
  log_string( "Normal termination of game." );
  exit( 0 );
  return 0;
    }
    else {
  close(fd0[1]);
  close(fd1[0]);
  if (dup2(fd0[0], STDIN_FILENO)!=STDIN_FILENO) {
      fprintf(stderr, "Error duping2 STDIN.\n");
      close(fd0[0]);
      close(fd1[1]);
      return 1;
  }
  if (dup2(fd1[1], STDOUT_FILENO)!=STDOUT_FILENO) {
      fprintf(stderr, "Error duping2 STDOUT.\n");
      close(fd0[0]);
      close(fd1[1]);
      return 1;
  }
  if (execl("../src/envy_logger", "envy_resolver_logger", NULL)<0) {
      fprintf(stderr, "Error execing envy_logger.\n");
      close(fd0[0]);
      close(fd1[1]);
      return 1;
  }
    return 0;
    }
}



#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
int init_socket( u_short port )
{
    static struct sockaddr_in sa_zero;
     struct sockaddr_in sa;
      int       x        = 1; 
      int       fd;

#if !defined( WIN32 )
    system( "touch SHUTDOWN.TXT" );
    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
  perror( "Init_socket: socket" );
  exit( 1 );
    }
#else
    WORD wVersionRequested = MAKEWORD( 1, 1 );
    WSADATA wsaData;
    int err = WSAStartup( wVersionRequested, &wsaData ); 
    if ( err != 0 )
    {
  perror("No useable WINSOCK.DLL");
  exit(1);
    }

    if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
  perror( "Init_socket: socket" );
  exit( 1 );
    }
#endif

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof( x ) ) < 0 )
    {
  perror( "Init_socket: SO_REUSEADDR" );
#if !defined( WIN32 )
  close( fd );
#else
  closesocket( fd );
#endif
  exit( 1 );
    }

#if defined( SO_DONTLINGER ) && !defined( SYSV )
    {
  struct        linger      ld;

  ld.l_onoff  = 1;
  ld.l_linger = 1000;

  if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
  (char *) &ld, sizeof( ld ) ) < 0 )
  {
      perror( "Init_socket: SO_DONTLINGER" );
#if !defined( WIN32 )
      close( fd );
#else
      closesocket( fd );
#endif
      exit( 1 );
  }
    }
#endif

    sa        = sa_zero;
#if !defined( WIN32 )
    sa.sin_family   = AF_INET;
#else
    sa.sin_family   = PF_INET;
#endif
    sa.sin_port      = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
    {
  perror( "Init_socket: bind" );
#if !defined( WIN32 )
  close( fd );
#else
  closesocket( fd );
#endif
  exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
  perror( "Init_socket: listen" );
#if !defined( WIN32 )
  close( fd );
#else
  closesocket( fd );
#endif
  exit( 1 );
    }

#if !defined( WIN32 )
    system( "rm SHUTDOWN.TXT" );
#endif
    return fd;
}
#endif



#if defined( macintosh )
void game_loop_mac_msdos( void )
{
	static DESCRIPTOR_DATA dcon;
	struct timeval last_time;
	struct timeval now_time;

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;

	/*
	* New_descriptor analogue.
	*/
	dcon.descriptor = 0;
	dcon.character = NULL;
	dcon.connected = CON_GET_NAME;
	dcon.host = str_dup( "localhost" );
	dcon.outsize = 10000;
	dcon.outbuf = alloc_mem( dcon.outsize );
	dcon.showstr_head = str_dup( "" );
	dcon.showstr_point = 0;
	dcon.pEdit = NULL; /* OLC */
	dcon.pString = NULL; /* OLC */
	dcon.editor = 0; /* OLC */
	dcon.next = descriptor_list;
	descriptor_list = &dcon;

	/*
	* Send the greeting.
	*/
	{
		extern char * help_greeting;
		if ( help_greeting[0] == '.' )
			write_to_buffer( &dcon, help_greeting+1, 0 );
		else
			write_to_buffer( &dcon, help_greeting  , 0 );
	}

	/* Main loop */
	while ( !merc_down )
	{
		DESCRIPTOR_DATA *d;

		/*
		* Process input.
		*/
		for ( d = descriptor_list; d; d = d_next )
		{
			d_next    = d->next;
			d->fcommand        = FALSE;

			if ( d->character )
				d->character->timer = 0;
			if ( !read_from_descriptor( d ) )
			{
				if ( d->character )
					save_char_obj( d->character );
				d->outtop  = 0;
				close_socket( d );
				continue;
			}

			if ( d->character && d->character->wait > 0 )
			{
				--d->character->wait;
				continue;
			}

			read_from_buffer( d );
			if ( d->incomm[0] != '\0' )
			{
				d->fcommand = TRUE;
				stop_idling( d->character );

				/* OLC */
				if ( d->showstr_point )
					show_string( d, d->incomm );
				else
					if ( d->pString )
						string_add( d->character, d->incomm );
					else
						if ( d->connected == CON_PLAYING )
						{
							/* if ( d->showstr_point )
							show_string( d, d->incomm );
							else
							*/
							if ( !run_olc_editor( d ) )
								interpret( d->character, d->incomm );
						}
					else
						nanny( d, d->incomm );

				d->incomm[0] = '\0';
			}
		}



		/*
		* Autonomous game motion.
		*/
		update_handler( );



		/*
		* Output.
		*/
		for ( d = descriptor_list; d; d = d_next )
		{
			d_next = d->next;

			if ( ( d->fcommand || d->outtop > 0 ) )
			{
				if ( !process_output( d, TRUE ) )
				{
					if ( d->character )
						save_char_obj( d->character );
					d->outtop = 0;
					close_socket( d );
				}
			}
		}



		/*
		* Synchronize to a clock.
		* Busy wait (blargh).
		*/
		now_time = last_time;
		for ( ; ; )
		{
			int delta;

			if ( dcon.character )
				dcon.character->timer = 0;
			if ( !read_from_descriptor( &dcon ) )
			{
				if ( dcon.character )
					save_char_obj( d->character );
				dcon.outtop    = 0;
				close_socket( &dcon );
			}

			gettimeofday( &now_time, NULL );
			delta = ( now_time.tv_sec - last_time.tv_sec ) * 1000 * 1000 + ( now_time.tv_usec - last_time.tv_usec );
			if ( delta >= 1000000 / PULSE_PER_SECOND )
				break;
		}
		last_time = now_time;
		current_time = (time_t) last_time.tv_sec;
	}

	return;
}
#endif



#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
void game_loop_unix( int control )
{
  static struct timeval null_time;
  struct timeval last_time;

#if !defined( AmigaTCP ) && !defined( WIN32 )
  signal( SIGPIPE, SIG_IGN );
#endif

  gettimeofday( &last_time, NULL );
  current_time = (time_t) last_time.tv_sec;

  /* Main loop */
  while ( !merc_down )
  {
    DESCRIPTOR_DATA *d;
    fd_set     in_set;
    fd_set     out_set;
    fd_set     exc_set;
    int    maxdesc;

#if defined( MALLOC_DEBUG )
	if ( malloc_verify( ) != 1 )
		abort( );
#endif

    /*
     * Poll all active descriptors.
     */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc = control;
	for ( d = descriptor_list; d; d = d->next )
	{
		maxdesc = UMAX( (unsigned) maxdesc, d->descriptor );
		FD_SET( d->descriptor, &in_set  );
		FD_SET( d->descriptor, &out_set );
		FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
		perror( "Game_loop: select: poll" );
		exit( 1 );
	}

    /*
     * New connection?
     */
	if ( FD_ISSET( control, &in_set ) )
		new_descriptor( control );

    /*
     * Kick out the freaky folks.
     */
	for ( d = descriptor_list; d; d = d_next )
	{
		d_next = d->next; 
		if ( FD_ISSET( d->descriptor, &exc_set ) )
		{
			FD_CLR( d->descriptor, &in_set  );
			FD_CLR( d->descriptor, &out_set );
			if ( d->character )
				save_char_obj( d->character );
			d->outtop  = 0;
			close_socket( d );
		}
	}
    /*
     * Process input.
     */
	for ( d = descriptor_list; d; d = d_next )
	{
		d_next = d->next;
		d->fcommand = FALSE;
		if ( FD_ISSET( d->descriptor, &in_set ) )
		{
			if ( d->character )
				d->character->timer = 0;
			if ( !read_from_descriptor( d ) )
			{
				FD_CLR( d->descriptor, &out_set );
				if ( d->character )
					save_char_obj( d->character );
				d->outtop = 0;
				close_socket( d );
				continue;
			}
		}
		if (   d->character && d->character->pcdata
			&& d->character->pcdata->horse 
			&& d->character->pcdata->horse->wait
			&& d->character->pcdata->horse->wait > 0)
		--d->character->pcdata->horse->wait;

		if ( d->character && d->character->wait > 0 )
		{
			--d->character->wait;
			continue;
		}

		read_from_buffer( d );
		if ( d->incomm[0] != '\0' )
		{
			d->fcommand = TRUE;
			stop_idling( d->character );
			/* OLC */
			if ( d->showstr_point )
			{
				show_string( d, d->incomm );
			}
			else if ( d->pString )
			{
				string_add( d->character, d->incomm );
			}
			else if ( d->connected == CON_PLAYING )
			{
				/*
					if ( d->showstr_point )
					show_string( d, d->incomm );
					else
				*/
				if ( !run_olc_editor( d ) )
					interpret( d->character, d->incomm );
			}
			else
			{
				nanny( d, d->incomm );
			}
			d->incomm[0] = '\0';
		}
	}


    /*
     * Process input from child.
     */
    {
      /* stop processing input from child *
      int n;
      if ((n=read(fd1[0],read_pipe_ptr,MAX_STRING_LENGTH-(read_pipe_ptr-read_pipe_buf)-1))>0) {
        read_pipe_ptr[n]='\0';
        for (read_pipe_ptr=read_pipe_buf;read_pipe_ptr<strchr(read_pipe_buf, '\0');read_pipe_ptr=strchr(read_pipe_ptr, '\n')+1) {
          if (strchr(read_pipe_ptr, '\n')==NULL) {
            memmove(read_pipe_buf, read_pipe_ptr, strlen(read_pipe_ptr)+1);
            read_pipe_ptr=strchr(read_pipe_buf, '\0');
            fSeekPipe=FALSE;
            break;
          }
          if (read_pipe_ptr[0]=='|') {
            DESCRIPTOR_DATA *resolving=NULL;
            int __resolving = 0;
            char buf1[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];
            switch (read_pipe_ptr[1]) {
              case 'H':
                sscanf(read_pipe_ptr, "|H %d %s %s \n", 
                    &__resolving, buf1, buf2);
                resolving = (DESCRIPTOR_DATA *)__resolving;
                if (resolving->character 
                  && !str_cmp(buf1, resolving->character->name) 
                  && str_cmp(buf2, resolving->host)) {
                  BAN_DATA *pban;
                  free_string(resolving->host);
                  resolving->host=str_dup(buf2);
                  for ( pban = ban_list; pban; pban = pban->next )
                  {
                    if ( !str_infix( pban->name, resolving->host ) )
                    {
                      write_to_descriptor( resolving->descriptor,
                          "Your site has been banned from this Mud.\n\r", 0 );
                      close_socket( resolving );
                    }
                  }
                }
                break;
              case 'U':
                sscanf(read_pipe_ptr, "|U %d %s %s \n", 
                    &__resolving, buf1, buf2);
                resolving = (DESCRIPTOR_DATA *)__resolving;
                if (resolving->character && !str_cmp(buf1, resolving->character->name) && str_cmp(buf2, resolving->remoteuser)) {
                  free_string(resolving->remoteuser);
                  resolving->remoteuser=str_dup(buf2);
                }
                break;
              case 'E':
                log_string("Exit on child process error.");
                end_of_game();
                merc_down=TRUE;
                break;
              default:
                bug("Invalid argument from child: %s", (int)read_pipe_ptr);
            }
          }
          else
            bug("Invalid argument from child: %s", (int)read_pipe_buf);
        }
        if (fSeekPipe)
          read_pipe_ptr=read_pipe_buf;
        else
          fSeekPipe=TRUE;
      }
      else if (n==0) {
        fprintf(stderr, "Child has closed pipe; forking another one.\n");
        close(fd0[1]);
        close(fd1[0]);
        if (pipe(fd0)<0||pipe(fd1)<0) {
          fprintf(stderr, "Error opening pipe.\n");
        }
        else if ((pid=fork())<0) {
          fprintf(stderr, "Error forking.\n");
        }
        else if (pid>0) {
          close(fd0[0]);
          close(fd1[1]);
          fcntl(fd0[1], F_SETFL, O_NONBLOCK);
          fcntl(fd1[0], F_SETFL, O_NONBLOCK);
        }
        else {
          close(fd0[1]);
          close(fd1[0]);
          if (dup2(fd0[0], STDIN_FILENO)!=STDIN_FILENO) {
            fprintf(stderr, "Error duping2 STDIN.\n");
            close(fd0[0]);
            close(fd1[1]);
            exit(1);
          }
          if (dup2(fd1[1], STDOUT_FILENO)!=STDOUT_FILENO) {
            fprintf(stderr, "Error duping2 STDOUT.\n");
            close(fd0[0]);
            close(fd1[1]);
            exit(1);
          }
          if (execl("../src/envy_logger", "envy_resolver_loggger", NULL)<0) {
            fprintf(stderr, "Error execing envy_logger.\n");
            close(fd0[0]);
            close(fd1[1]);
            exit(1);
          }
        }
      }
      */
    }


    /*
     * Autonomous game motion.
     */
    update_handler( );



    /*
     * Output.
     */
	for ( d = descriptor_list; d; d = d_next )
	{
		d_next = d->next;

		if ( ( d->fcommand || d->outtop > 0 ) && FD_ISSET( d->descriptor, &out_set ) )
		{
			if ( !process_output( d, TRUE ) )
			{
				if ( d->character )
					save_char_obj( d->character );
				d->outtop      = 0;
				close_socket( d );
			}
		}
	}



    /*
     * Synchronize to a clock.
     * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
     * Careful here of signed versus unsigned arithmetic.
     */
#if !defined( WIN32 )
    {
      struct timeval now_time;
      long secDelta;
      long usecDelta;

      gettimeofday( &now_time, NULL );
      usecDelta      = ( (int) last_time.tv_usec )
        - ( (int)  now_time.tv_usec )
        + 1000000 / PULSE_PER_SECOND;
      secDelta      = ( (int) last_time.tv_sec )
        - ( (int)  now_time.tv_sec );
      while ( usecDelta < 0 )
      {
        usecDelta += 1000000;
        secDelta  -= 1;
      }

      while ( usecDelta >= 1000000 )
      {
        usecDelta -= 1000000;
        secDelta  += 1;
      }

      if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
      {
        struct timeval stall_time;

        stall_time.tv_usec = usecDelta;
        stall_time.tv_sec  = secDelta;
        if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
        {
          perror( "Game_loop: select: stall" );
          exit( 1 );
        }
      }
    }
#else
    {
      int times_up;
      int nappy_time;
      struct _timeb start_time;
      struct _timeb end_time;
      _ftime( &start_time );
      times_up = 0;

      while( times_up == 0 )
      {
        _ftime( &end_time );
        if ( ( nappy_time =
              (int) ( 1000 *
                  (double) ( ( end_time.time - start_time.time ) +
                         ( (double) ( end_time.millitm -
                              start_time.millitm ) /
                         1000.0 ) ) ) ) >=
            (double)( 1000 / PULSE_PER_SECOND ) )
          times_up = 1;
        else
        {
          Sleep( (int) ( (double) ( 1000 / PULSE_PER_SECOND ) -
                (double) nappy_time ) );
          times_up = 1;
        }
      }
    }
#endif

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;
  }

  return;
}
#endif


extern  int top_desc;
#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
void new_descriptor( int control )
{
  static DESCRIPTOR_DATA  d_zero;
  DESCRIPTOR_DATA *dnew, *dcount, *dcount2;	//Modified by Razgriz 20050923
  struct sockaddr_in      sock;
  /*    struct hostent     *from;*/
  BAN_DATA     *pban;
  char        buf [ MAX_STRING_LENGTH ];
  int         desc;
  int         size;
  int         addr;

  size = sizeof( sock );
  if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
  {
    perror( "New_descriptor: accept" );
    return;
  }

#if !defined( FNDELAY )
#if defined( __hpux )
#define FNDELAY O_NONBLOCK
#else
#define FNDELAY O_NDELAY
#endif
#endif

#if !defined( AmigaTCP ) && !defined( WIN32 )
  if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
  {
    perror( "New_descriptor: fcntl: FNDELAY" );
    return;
  }
#endif

  /*
   * Cons a new descriptor.
   */
  if ( !descriptor_free )
  {
    ++top_desc;
    dnew        = alloc_mem( sizeof( *dnew ) );
  }
  else
  {
    dnew        = descriptor_free;
    descriptor_free        = descriptor_free->next;
  }

  *dnew    = d_zero;
  dnew->descriptor      = desc;
  dnew->character = NULL;
  dnew->connected    = CON_GET_NAME;
  dnew->showstr_head  = str_dup( "" );
  dnew->showstr_point = 0;
  dnew->pEdit          = NULL;            /* OLC */
  dnew->pString = NULL;     /* OLC */
  dnew->editor  = 0;          /* OLC */
  dnew->outsize  = 2000;
  dnew->outbuf  = alloc_mem( dnew->outsize );
  dnew->trytimes  = 1;

  size = sizeof( sock );

  /*
   * Would be nice to use inet_ntoa here but it takes a struct arg,
   * which ain't very compatible between gcc and system libraries.
   */
  /*    dnew->remoteuser = str_dup( rfc931_name( (int)desc, (struct sockaddr_in*)&sock ) );*/
  dnew->remoteuser=str_dup("UNKNOWN");
  addr = ntohl( sock.sin_addr.s_addr );
  sprintf( buf, "%d.%d.%d.%d",
      ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
      ( addr >>  8 ) & 0xFF, ( addr     ) & 0xFF
       );
  dnew->host=str_dup(buf);
  sprintf(pipe_buf, "|D %d %d %d %d \n", (int)dnew, sock.sin_addr.s_addr, port, ntohs(sock.sin_port));
  write(fd0[1], pipe_buf, strlen(pipe_buf));
  //log_string("Envy_log: P1");
  sprintf(pipe_buf, "|P %d \n", (int)dnew);
  write(fd0[1], pipe_buf, strlen(pipe_buf));
  sprintf( log_buf, "Sock.sinaddr:  $U@%s", buf );
  log_string( log_buf );
  /*    from = gethostbyaddr( (char *) &sock.sin_addr,
      sizeof(sock.sin_addr), AF_INET );
      dnew->host = str_dup( from ? from->h_name : buf );*/


  /*
   * Swiftest: I added the following to ban sites.  I don't
   * endorse banning of sites, but Copper has few descriptors now
   * and some people from certain sites keep abusing access by
   * using automated 'autodialers' and leaving connections hanging.
   *
   * Furey: added suffix check by request of Nickel of HiddenWorlds.
   */
  for ( pban = ban_list; pban; pban = pban->next )
  {
    if ( !str_prefix( pban->name, dnew->host ) )
    {
      write_to_descriptor( desc,
          "Your site has been banned from this Mud.\n\r", 0 );
      sprintf(pipe_buf, "|Q %d \n", (int)dnew);
      write(fd0[1], pipe_buf, strlen(pipe_buf));
#if !defined( WIN32 )
      close( desc );
#else
      closesocket( desc );
#endif
      free_string( dnew->showstr_head );
      free_string( dnew->remoteuser );
      free_string( dnew->host );
      free_mem( dnew->outbuf/*, dnew->outsize */);
      dnew->next          = descriptor_free;
      descriptor_free    = dnew;
      return;
    }
  }

  /*
   * Init descriptor data.
   */
  dnew->next            = descriptor_list;
  descriptor_list      = dnew;

  /*
   * Send the greeting.
   */
  {
    extern char * help_greeting[ MAX_GREETING ];
    extern int num_greeting;
    int random_greeting;
    char __buffer[MAX_STRING_LENGTH];

    int current_ppl = 0, i;
    int real_ppl = 0;		//Added by Razgriz 20050923
    bool real_check;		//Added by Razgriz 20050923

    //Modified by Razgriz 20050923
    for ( dcount = descriptor_list, i=0; dcount; dcount = dcount->next, i++ )
    {
    	if ( dcount->connected != CON_PLAYING ) continue;
        current_ppl += 1;
        real_check = TRUE;
        for ( dcount2 = dcount->next; dcount2 ; dcount2 = dcount2->next )
        {
            if ( dcount2->connected != CON_PLAYING ) continue;
            if ( !str_cmp ( dcount2->host, dcount->host ) )
            {
                real_check = FALSE;
                break;
            }
        }
        if ( real_check )
            real_ppl++;
    }
    
    random_greeting = number_range( 0, num_greeting - 1 );

    if ( help_greeting[random_greeting][0] == '.' )
      write_to_buffer( dnew, help_greeting[random_greeting]+1, 0 );
    else
      write_to_buffer( dnew, help_greeting[random_greeting]  , 0 );

    //Modified by Razgriz 20050923
    sprintf( __buffer, "²{¦b¦³ %d ¦W«iªÌ ( %d ¦ì¨¤¦â ) ¥¿¦b½sÂ´¶Ç©_... \n\r", 
        (real_ppl    > 0) ? real_ppl    : 0, 
        (current_ppl > 0) ? current_ppl : 0 );
    write_to_buffer( dnew, "\n\r\t\t\t¢Ð¢Ð¢á¡G https://term.ptt2.cc \n\r", 0);
    write_to_buffer( dnew, "\t\t\t¬Ý  ªO¡G RealmOfChaos \n\r\n\r", 0);
    write_to_buffer( dnew, __buffer, 0);
    write_to_buffer( dnew, 
        "·Q­n¶i¤J ²V¨P¤Û¥@ ¥@¬É«_ÀIªº­^¶¯¦W¦r¬O¡H", 0);
  }

  return;
}
#endif

/*02/2/28, amenda, force quit immediately when close socket. to prevent bugs*/
void lost_link_quit( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj, *obj_next_content;
    CHAR_DATA *phorse = ch->pcdata->horse;

    phorse = ch->pcdata->horse;

  if( auction_list ){
      AUC_DATA *auction;
      for( auction = auction_list ; auction; auction = auction->next ){
    if( auction->owner == ch || auction->buyer == ch )
    {
        send_to_char(CYN"¤£¦æ¡I§AÁÙ¦b¶i¦æ©ç½æ©O¡C"NOR"\n\r", ch);
        return;
    }
      }
  }

    if ( ch->position == POS_FIGHTING )
    stop_fighting( ch, TRUE );

    if ( phorse && phorse->position == POS_FIGHTING )
    stop_fighting( phorse, TRUE );

    if ( ch->in_room->roomprogs && ( ch->in_room->roomprogs->type & ROOM_LEAVE )
  && !IS_SET( ch->act, PLR_WIZINVIS ) )
    rprog_leave_trigger( ch->in_room, ch, 7 );

    sprintf( log_buf, "%s close socket force quit.", ch->name );
    top10_check(ch);
    log_string( log_buf );

    /*
     * After extract_char the ch is no longer valid!
     */

    for ( obj = ch->carrying; obj; obj = obj_next_content )
    {
    obj_next_content = obj->next_content;
      if ( obj->deleted )
        continue;
    if ( IS_SET( obj->extra_flags, ITEM_NOSAVE )
      || ( ch->level < ( obj->level - 5 ) && ch->class < 5 ) )
    {
        unequip_char( ch, obj);
        obj_from_char( obj );
        obj_to_room( obj, ch->in_room );
    }
    }

    save_char_obj( ch );

  if ( phorse )
    extract_char( phorse, TRUE );
    extract_char( ch, TRUE );

    return;
}

//Added by Razgriz	20050825
extern void fwrite_neweqbank	args( ( CHAR_DATA * ch, FILE* fp ) );
extern void free_neweqbank	args( ( BANK_DATA * bank ) );

void close_socket( DESCRIPTOR_DATA *dclose )
{
	CHAR_DATA *ch;

	if ( dclose->outtop > 0 )
		process_output( dclose, FALSE );

	if ( dclose->snoop_by )
	{
		write_to_buffer( dclose->snoop_by, "Your victim has left the game.\n\r", 0 );
	}

	{
		DESCRIPTOR_DATA *d;

		for ( d = descriptor_list; d; d = d->next )
		{
			if ( d->snoop_by == dclose )
				d->snoop_by = NULL;
		}
	}

	if ( ( ch = dclose->character ) )
	{
		sprintf( log_buf, "Closing link to %s.", ch->name );
		log_string( log_buf );
		if ( dclose->connected == CON_PLAYING )
		{
			//1051003 modify by jye
			if (!IS_IMMORTAL(ch))
				act( "$n Â_½u¤F.", ch, NULL, NULL, TO_ROOM );

			//Added by Razgriz	20050825
			if( ch->neweqbank )
			{
				FILE * fp;
				char tmp[255];

				sprintf( tmp, "%s%s/%s", EQBANK_DIR, initial ( ch->neweqbank->name), capitalize (ch->neweqbank->name) );
				ch->neweqbank->opened = FALSE;
				if (  ( fp = fopen (tmp, "w") )  ) {
					fwrite_neweqbank (ch, fp);
					fflush( fp );
					fclose(fp);
					//send_to_char( "®L·ç´µ(Sharas)´À§A§â¥¼Ãö³¬ªº­Ü®wÃö¤W¤F¡C\n\r", ch );
					free_neweqbank (ch->neweqbank);
					ch->neweqbank = NULL;
				}else{
					bug ("eqbank: file open error at close_socket !!",0);
					//send_to_char("µo¥ÍÄY­«°ÝÃD¡A½Ð¦V¤j¯«¦^³ø¡C\n\r", ch);
				}
			}

			if( ch->desc && ch->desc->original && ch->desc->original->pcdata 
			&& ch->desc->original->pcdata->switched )
			{
				CHAR_DATA * och = ch->desc->original;

				if( IS_SET( ch->act, PLR_HORSE ) )
					do_ride(ch, "");
				else
					do_return( ch, "" );
				ch = och;
			}
			//    lost_link_quit( ch, "" );
			ch->desc = NULL;
		}
		else
			free_char( dclose->character );
	}

	if ( d_next == dclose )
	d_next = d_next->next;   

	if ( dclose == descriptor_list )
	{
		descriptor_list = descriptor_list->next;
	}
	else
	{
		DESCRIPTOR_DATA *d;

		for ( d = descriptor_list; d && d->next != dclose; d = d->next )
		;
		if ( d )
			d->next = dclose->next;
		else
			bug( "Close_socket: dclose not found.", 0 );
	}

	sprintf(pipe_buf, "|Q %d \n", (int)dclose);
	write(fd0[1], pipe_buf, strlen(pipe_buf));

#if !defined( WIN32 )
	close( dclose->descriptor );
#else
	closesocket( dclose->descriptor );
#endif
	free_string( dclose->showstr_head );
	free_string( dclose->host );
	free_string( dclose->remoteuser );

	/* RT socket leak fix */
	free_mem( dclose->outbuf/*, dclose->outsize */);

	dclose->next = descriptor_free;
	descriptor_free = dclose;
	//#if defined( macintosh )
	// exit( 1 );
	//#endif
	return;
}

void close_socket2( DESCRIPTOR_DATA *dclose )
{
	// CHAR_DATA *ch;

	if ( dclose->outtop > 0 )
		process_output( dclose, FALSE );

	if ( dclose->snoop_by )
	{
		write_to_buffer( dclose->snoop_by, "Your victim has left the game.\n\r", 0 );
	}

	{
		DESCRIPTOR_DATA *d;

		for ( d = descriptor_list; d; d = d->next )
		{
			if ( d->snoop_by == dclose )
				d->snoop_by = NULL;
		}
	}
	/*
	if ( ( ch = dclose->character ) )
	{
		sprintf( log_buf, "%s Reconnected.", ch->name );
		log_string( log_buf );
		if ( dclose->connected == CON_PLAYING )
		{
			//1051003 modify by jye
			if (!IS_IMMORTAL(ch))
				act( "$n Â_½u¤F.", ch, NULL, NULL, TO_ROOM );

			if( ch->desc && ch->desc->original && ch->desc->original->pcdata 
				&& ch->desc->original->pcdata->switched )
			{
				if( IS_SET(ch->act, PLR_HORSE ))
				{
					CHAR_DATA * och = ch->desc->original;
					do_ride(ch, "");
					ch = och;
				}
				else {
					ch->desc->character = ch->desc->original;
					ch->desc->original = NULL;
					ch->desc->character->pcdata->switched = FALSE;
					ch->desc->character->desc = ch->desc;
				}
			}
			ch->desc = NULL;
		}
		else
		{
			free_char( dclose->character );
		}
	}
	*/
	if ( d_next == dclose )
		d_next = d_next->next;   

	if ( dclose == descriptor_list )
	{
		descriptor_list = descriptor_list->next;
	}
	else
	{
		DESCRIPTOR_DATA *d;

		for ( d = descriptor_list; d && d->next != dclose; d = d->next )
			;
		if ( d )
			d->next = dclose->next;
		else
			bug( "Close_socket: dclose not found.", 0 );
	}

	sprintf(pipe_buf, "|Q %d \n", (int)dclose);
	write(fd0[1], pipe_buf, strlen(pipe_buf));

#if !defined( WIN32 )
	close( dclose->descriptor );
#else
	closesocket( dclose->descriptor );
#endif
	free_string( dclose->showstr_head );
	free_string( dclose->host );
	free_string( dclose->remoteuser );

	/* RT socket leak fix */
	free_mem( dclose->outbuf/*, dclose->outsize */);

	dclose->next  = descriptor_free;
	descriptor_free    = dclose;
#if defined( macintosh )
	exit( 1 );
#endif
	return;
}

bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
  return TRUE;

    /* Check for overflow. */
    iStart = strlen( d->inbuf );
/*    while ( iStart>=3 && d->inbuf[iStart-1]=='\x01'
    && d->inbuf[iStart-2]=='\xfd' && d->inbuf[iStart-3]=='\xff' )
  iStart-=3; */
    if ( iStart >= sizeof( d->inbuf ) - 10 )
    {
//log_string("Envy_log: P2");
  sprintf( pipe_buf, "|P %d \n", (int)d );
  write( fd0[1], pipe_buf, strlen(pipe_buf) );
  sprintf( log_buf, "$U@$H input overflow!" );
  log_string( log_buf );
  write_to_descriptor( d->descriptor,
      "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
  return FALSE;
    }

    /* Snarf input. */
#if defined( macintosh )
    for ( ; ; )
    {
  int c;
  c = getc( stdin );
  if ( c == '\0' || c == EOF )
      break;
  putc( c, stdout );
  if ( c == '\r' )
      putc( '\n', stdout );
  d->inbuf[iStart++] = c;
  if ( iStart > sizeof( d->inbuf ) - 10 )
      break;
    }
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
    for ( ; ; )
    {
  int nRead;

#if !defined( WIN32 )
  nRead = read( d->descriptor, d->inbuf + iStart,
         sizeof( d->inbuf ) - 10 - iStart );
#else
  nRead = recv( d->descriptor, d->inbuf + iStart,
         sizeof( d->inbuf ) - 10 - iStart, 0 );
#endif
  if ( nRead > 0 )
  {
      iStart += nRead;
      if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
    break;
  }
  else if ( nRead == 0 )
  {
      log_string( "EOF encountered on read." );
      return FALSE;
  }
#if !defined( AmigaTCP ) && !defined( WIN32 )
  else if ( errno == EWOULDBLOCK || errno == EAGAIN )
      break;
#endif
#if defined( WIN32 )
  else if ( WSAGetLastError() == WSAEWOULDBLOCK || errno == EAGAIN )
      break;
#endif
  else
  {
      //perror( "Read_from_descriptor" );
      return FALSE;
  }
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
	int i;
	int j;
	int k;
	int l;

	/*
	* Hold horses if pending command already.
	*/
	if ( d->incomm[0] != '\0' )
		return;

	/*
	* Look for at least one new line.
	*/
	for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
	{
		if ( d->inbuf[i] == '\0' )
			return;
	}

	/*
	* Canonical input processing.
	*/
	for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
	{
		if ( k >= MAX_INPUT_LENGTH - 2 )
		{
			write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

			/* skip the rest of the line */
			for ( ; d->inbuf[i] != '\0'; i++ )
			{
				if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
					break;
			}
			d->inbuf[i]   = '\n';
			d->inbuf[i+1] = '\0';
			break;
		}

		if ( d->inbuf[i] == '\b' && k > 0 )
			--k;
		else if ( iscntrl( d->inbuf[i] ) )
		{
			if ( d->inbuf[i]=='\b' || d->inbuf[i]=='\n' || d->inbuf[i]=='\r' || d->inbuf[i]=='\t' || d->inbuf[i]=='\0' )
				d->incomm[k++]=d->inbuf[i];
			else if ( d->inbuf[i]=='\x1b' && d->inbuf[i+1]=='[')
			{
				for (l=i+2;isdigit(d->inbuf[l]) || d->inbuf[l]==';';l++);
					if ( d->inbuf[l]=='m' )
						d->incomm[k++]=d->inbuf[i];
			}
		}
		else /* if ( ( isascii( d->inbuf[i] ) && isprint( d->inbuf[i] ) )
		|| ( d->inbuf[i]>'\x80' && d->inbuf[i]<'\xff' )
		|| d->inbuf[i]=='\x1b' ) */
		d->incomm[k++] = d->inbuf[i];
	}

	/*
	* Finish off the line.
	*/
	if ( k == 0 )
		d->incomm[k++] = ' ';
	d->incomm[k] = '\0';

	/*
	* Deal with bozos with #repeat 1000 ...
	*/
	if ( k > 1 || d->incomm[0] == '!' )
	{
		if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
		{
			d->repeat = 0;
		}
		else
		{
			if ( ++d->repeat >= 20 )
			{
				//log_string("Envy_log: P3");
				sprintf( pipe_buf, "|P %d \n", (int)d);
				write(fd0[1], pipe_buf, strlen(pipe_buf));
				sprintf( log_buf, "$U@$H input spamming!" );
				log_string( log_buf );
				write_to_descriptor( d->descriptor, "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
				strcpy( d->incomm, "quit" );
			}
		}
	}

	/*
	* Do '!' substitution.
	*/
	if ( d->incomm[0] == '!' )
		strcpy( d->incomm, d->inlast );
	else
		strcpy( d->inlast, d->incomm );

	/*
	* Shift the input buffer.
	*/
	while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		i++;
	for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
		;
	return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )        /* OLC */
    {
      if ( d->showstr_point )
      {
          write_to_buffer( d,
        "[¿é¤J«ü¥O (c)Ä~Äò, (r)­«¦L¥»­¶, (b)¤W­¶, (h)»¡©ú, (q)Â÷¶}, ©Î«ö ENTER]:  ",  0 );
      }else if ( d->pString ){
          write_to_buffer( d, "> ", 2 );
      }else{
          CHAR_DATA *ch;

          ch = d->original ? d->original : d->character;
          if ( IS_SET( ch->act, PLR_BLANK ) )
              write_to_buffer( d, "\n\r", 2 );

          if ( IS_SET( ch->act, PLR_PROMPT  ) )
              bust_a_prompt( d );

          //if ( IS_SET( ch->act, PLR_TELNET_GA ) )
        //  write_to_buffer( d, go_ahead_str, 0 );
      }
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
        return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by )
    {
        write_to_buffer( d->snoop_by, "% ", 2 );
        write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
        d->outtop = 0;
        write_to_buffer( d, "... °T®§¦h¨ìÅý§A²´ªá¼º¶Ã @_@\n\r", 32 );
        write_to_descriptor( d->descriptor, d->outbuf, d->outtop );
        d->outtop = 0;

//        return FALSE;
        return TRUE;
    }
    else
    {
        d->outtop = 0;
        return TRUE;
    }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
	CHAR_DATA *ch;
	const char *str;
	const char *i;
	char *point;
	char buf  [ MAX_STRING_LENGTH ];
	char buf2 [ MAX_STRING_LENGTH ];
	char buf3 [ MAX_STRING_LENGTH ];

	/* Will always have a pc ch after this */
	ch = ( d->original ? d->original : d->character );
	if( !ch->pcdata->prompt || ch->pcdata->prompt[0] == '\0' )
	{
		send_to_char( "\n\r\n\r", ch );
		return;
	}

	point = buf;
	str = ch->pcdata->prompt;

	while( *str != '\0' )
	{
		if( *str != '%' )
		{
			*point++ = *str++;
			continue;
		}
		++str;
		switch( *str )
		{
			default :
				i = " ";
				break;
			case 'h' :
				// ²¾°£hp ¤j©ó 3/4 ®Éªº¦â½X¼vÅT, ¨Ã¹ï¨C­Ó¦â½X«e¥[¤J²×¤î¦â½X 2023/01/16
				if ( ch->hit > ch->max_hit * 3 / 4 )
					//sprintf( buf2, "[m" );
					sprintf( buf2, "" );
				else if ( ch->hit > ch->max_hit / 2 )
					sprintf( buf2, "[m[1;36m" );
				else if ( ch->hit > ch->max_hit / 4 )
					sprintf( buf2, "[m[1;33m" );
				else
					sprintf( buf2, "[m[1;31m" );
				sprintf( buf3, "%d[m", ch->hit );
				strcat( buf2, buf3 );
				i = buf2;
				break;
			case 'H' :
				sprintf( buf2, "%d", ch->max_hit );
				i = buf2;
				break;
			case 'm' :
				sprintf( buf2, "%d", ch->mana );
				i = buf2;
				break;
			case 'M' :
				sprintf( buf2, "%d", ch->max_mana );
				i = buf2;
				break;
			case 'v' :
				sprintf( buf2, "%d", ch->move ); 
				i = buf2;
				break;
			case 'V' :
				sprintf( buf2, "%d", ch->max_move );
				i = buf2;
				break;
			case 'x' :
				sprintf( buf2, "%d", ch->exp );
				i = buf2;
				break;
			case 'l' :
				sprintf( buf2, "%d", ch->level );
				i = buf2;
				break;
			case 'X' :
				//·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ 20060722 modify by jye
				sprintf( buf2, "%d",  EXP_PER_LEVEL( ch->level ,ch->class) * (ch->level + 1 ) - ch->exp );
				i = buf2;
				break;
			break;
			case 'g' :
				sprintf( buf2, "%d", ch->gold );
				i = buf2;
				break;
			case 'w' :
				if( ch->wait == 0 )
					sprintf( buf2, "" );
				else
					sprintf( buf2, "%d", ch->wait );
				i = buf2;
				break;
			case 'a' :
				if( ch->level > 10 )
					sprintf( buf2, "%d", ch->alignment );
				else
					sprintf( buf2, "%s", IS_GOOD( ch ) ? "µ½¨}"
					: IS_EVIL( ch ) ? "¨¸´c" : "¤¤¥ß" );
					i = buf2;
				break;
			case 'A' :
				sprintf( buf2, "%s", IS_GOOD( ch ) ? "µ½¨}" :
				IS_EVIL( ch) ? "¨¸´c" : "¤¤¥ß" );
				i = buf2;
				break;
			// °Ñ¼Æ b , B ¦Û°Ê¸É¦â align Åã¥Ü 2023/01/16
			case 'b' :
				if( ch->level > 10 ){
					if( ch->alignment > 799 )
						sprintf( buf2, "[m[1;37m" );
					else if( ch->alignment > 299 )
						sprintf( buf2, "[m[1;36m" );
					else if( ch->alignment > -300 )
						sprintf( buf2, "" );
					else if( ch->alignment > -600 )
						sprintf( buf2, "[m[1;33m" );
					else
						sprintf( buf2, "[m[1;31m" );
					sprintf( buf3, "%d[m", ch->alignment );
				}else{
					if( ch->alignment > 799 )
						sprintf( buf2, "[m[1;36m" );
					else if( ch->alignment > 599 )
						sprintf( buf2, "[m[1;37m" );
					else if( ch->alignment > 315 )
						sprintf( buf2, "[m[1;32m" );
					else if( ch->alignment > 284 )
						sprintf( buf2, "[m[36m" );
					else if( ch->alignment > -285 )
						sprintf( buf2, "" );
					else if( ch->alignment > -316 )
						sprintf( buf2, "[m[35m" );
					else if( ch->alignment > -600 )
						sprintf( buf2, "[m[1;35m" );
					else if( ch->alignment > -800 )
						sprintf( buf2, "[m[1;31m" );
					else
						sprintf( buf2, "[m[31m" );
					sprintf( buf3, "%s[m", IS_GOOD( ch ) ? "µ½¨}" : IS_EVIL( ch) ? "¨¸´c" : "¤¤¥ß" );
				}
				strcat( buf2, buf3 );
				i = buf2;
				break;
			case 'B' :
				if( ch->alignment > 799 )
					sprintf( buf2, "[m[1;36m" );
				else if( ch->alignment > 599 )
					sprintf( buf2, "[m[1;37m" );
				else if( ch->alignment > 315 )
					sprintf( buf2, "[m[1;32m" );
				else if( ch->alignment > 284 )
					sprintf( buf2, "[m[36m" );
				else if( ch->alignment > -285 )
					sprintf( buf2, "" );
				else if( ch->alignment > -316 )
					sprintf( buf2, "[m[35m" );
				else if( ch->alignment > -600 )
					sprintf( buf2, "[m[1;35m" );
				else if( ch->alignment > -800 )
					sprintf( buf2, "[m[1;31m" );
				else
					sprintf( buf2, "[m[31m" );
				sprintf( buf3, "%s[m", IS_GOOD( ch ) ? "µ½¨}" : IS_EVIL( ch) ? "¨¸´c" : "¤¤¥ß" );
				strcat( buf2, buf3 );
				i = buf2;
				break;
			case 'r' :
				if( ch->in_room )
					sprintf( buf2, "%s", ch->in_room->name );
				else
					sprintf( buf2, " " );
				i = buf2;
				break;
			case 'R' :
				if( IS_IMMORTAL( ch ) && ch->in_room )
					sprintf( buf2, "%d", ch->in_room->vnum );
				else
					sprintf( buf2, " " );
				i = buf2;
				break;
			case 'z' :
				if( IS_IMMORTAL( ch ) && ch->in_room )
					sprintf( buf2, "%s", ch->in_room->area->name );
				else
					sprintf( buf2, " " );
				i = buf2;
				break;
			case 'i' :  /* Invisible notification on prompt sent by Kaneda */
				sprintf( buf2, "%s|%s|%s", IS_AFFECTED( ch, AFF_INVISIBLE ) ?
				"Áô§Î" : "",
				IS_AFFECTED(ch, AFF_HIDE) ?
				"ÂÃ°Î" : "",
				IS_AFFECTED(ch, AFF_NIGHTSTALK) ?
				"·t©]" : "");
				i = buf2;
				break;
			case 't' :
					sprintf( buf2, "%d®É" ,time_info.hour%24 );
					i = buf2;
					break;

			case 'I' :
				if( IS_IMMORTAL( ch ) )
					sprintf( buf2, "(wizinv: %s)", IS_SET( ch->act, PLR_WIZINVIS ) ?
					"¶}" : "Ãö" );
				else
					sprintf( buf2, " " );
				i = buf2;
				break;
			case '%' :
				sprintf( buf2, "%%" );
				i = buf2;
				break;
			case 'c' :      /* OLC */
				i = olc_ed_name( ch );
				break;
			case 'C' :      /* OLC */
				i = olc_ed_vnum( ch );
				break;
			// ·s¼W d ªí¥Ü ¸Ë³Æªº½b¥Ú arrow ¼Æ¶q 2022/04/29
			case 'd' :
				if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
					if( get_eq_char( ch, WEAR_ARROW )->value[0] >= 1 )
						sprintf( buf2, "%d" , get_eq_char( ch, WEAR_ARROW )->value[0] );
					else if( get_eq_char( ch, WEAR_ARROW )->value[0] == -1 ) // µL­­¼Æ¶q 2023/01/05
						sprintf( buf2, "¡Û" );
					else
						sprintf( buf2, "0" );
				}else{
					sprintf( buf2, " " );
				}
				i = buf2;
				break;
			// ·s¼W D ªí¥Ü ¸Ë³Æªº¼uÃÄ ammo ¼Æ¶q 2022/04/29
			case 'D' :
				if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
					if( get_eq_char( ch, WEAR_AMMO )->value[0] >= 1 )
						sprintf( buf2, "%d" , get_eq_char( ch, WEAR_AMMO )->value[0] );
					else if( get_eq_char( ch, WEAR_AMMO )->value[0] == -1 ) // µL­­¼Æ¶q 2023/01/05
						sprintf( buf2, "¡Û" );
					else
						sprintf( buf2, "0" );
				}else{
					sprintf( buf2, " " );
				}
				i = buf2;
				break;
		}
		++str;
		while( ( *point = *i ) != '\0' )
			++point, ++i;      
	}
	write_to_buffer( d, buf, point - buf );

	if ( d->original )
	{
		ch = d->character;
		if( !ch->pcdata || !ch->pcdata->prompt || ch->pcdata->prompt[0] == '\0' )
		{
			send_to_char( "\n\r\n\r", ch );
			return;
		}
		send_to_char( "\n\r", ch );
		point = buf;
		str = ch->pcdata->prompt;

		while( *str != '\0' )
		{
			if( *str != '%' )
			{
				*point++ = *str++;
				continue;
			}
			++str;
			switch( *str )
			{
				default :
					i = " ";
					break;
				case 'h' :
					sprintf( buf2, "%d", ch->hit );
					i = buf2;
					break;
				case 'H' :
					sprintf( buf2, "%d", ch->max_hit );
					i = buf2;
					break;
				case 'm' :
					sprintf( buf2, "%d", ch->mana );
					i = buf2;
					break;
				case 'M' :
					sprintf( buf2, "%d", ch->max_mana );
					i = buf2;
					break;
				case 'v' :
					sprintf( buf2, "%d", ch->move ); 
					i = buf2;
					break;
				case 'V' :
					sprintf( buf2, "%d", ch->max_move );
					i = buf2;
					break;
				case 'x' :
					sprintf( buf2, "%d", ch->exp );
					i = buf2;
					break;
				case 'l' :
					sprintf( buf2, "%d", ch->level );
					i = buf2;
					break;
				case 'X' :
					//20060722 modify by jye
					//·s¼WEXP_PER_LEVEL ppl_classÅÜ¼Æ
					sprintf( buf2, "%d",  EXP_PER_LEVEL( ch->level,ch->class ) * (ch->level + 1 ) - ch->exp );
					i = buf2;
					break;
				case 'g' :
					sprintf( buf2, "%d", ch->gold );
					i = buf2;
					break;
				case 'w' :
					if( ch->wait == 0 )
						sprintf( buf2, "" );
					else
						sprintf( buf2, "%d", ch->wait );
					i = buf2;
					break;
				case 'a' :
					if( ch->level > 10 )
						sprintf( buf2, "%d", ch->alignment );
					else
						sprintf( buf2, "%s", IS_GOOD( ch ) ? "µ½¨}" : IS_EVIL( ch ) ? "¨¸´c" : "¤¤¥ß" );
					i = buf2;
					break;
				case 'A' :
					sprintf( buf2, "%s", IS_GOOD( ch ) ? "µ½¨}" : IS_EVIL( ch) ? "¨¸´c" : "¤¤¥ß" );
					i = buf2;
					break;
				case 'b' :
					if( ch->level > 10 )
						sprintf( buf2, "%d", ch->alignment );
					else
						sprintf( buf2, "%s", IS_GOOD( ch ) ? "µ½¨}" : IS_EVIL( ch ) ? "¨¸´c" : "¤¤¥ß" );
					i = buf2;
					break;
				case 'B' :
					sprintf( buf2, "%s", IS_GOOD( ch ) ? "µ½¨}" : IS_EVIL( ch) ? "¨¸´c" : "¤¤¥ß" );
					i = buf2;
					break;
				case 'r' :
					if( ch->in_room )
						sprintf( buf2, "%s", ch->in_room->name );
					else
						sprintf( buf2, " "              );
					i = buf2;
					break;
				case 'R' :
					if( IS_IMMORTAL( ch ) && ch->in_room )
						sprintf( buf2, "%d", ch->in_room->vnum );
					else
						sprintf( buf2, " "              );
					i = buf2;
					break;
				case 'z' :
					if( IS_IMMORTAL( ch ) && ch->in_room )
						sprintf( buf2, "%s", ch->in_room->area->name );
					else
						sprintf( buf2, " "              );
					i = buf2;
					break;
				case 'i' :  /* Invisible notification on prompt sent by Kaneda */
					sprintf( buf2, "%s", IS_AFFECTED( ch, AFF_INVISIBLE ) ? "Áô§Î" : "¥¼Áô§Î" );
					i = buf2;
					break;
				case 'I' :
					if( IS_IMMORTAL( ch ) )
						sprintf( buf2, "(wizinv: %s)", IS_SET( ch->act, PLR_WIZINVIS ) ? "¶}" : "Ãö" );
					else
						sprintf( buf2, " " );
					i = buf2;
					break;
				case '%' :
					sprintf( buf2, "%%" );
					i = buf2;
					break;
				case 'c' : /* OLC */
					i = olc_ed_name( ch );
					break;
				case 'C' : /* OLC */
					i = olc_ed_vnum( ch );
					break;
				// ·s¼W d ªí¥Ü ¸Ë³Æªº½b¥Ú arrow ¼Æ¶q 2022/04/29
				case 'd' :
					if( get_eq_char( ch, WEAR_ARROW ) && get_eq_char( ch, WEAR_ARROW )->item_type == ITEM_ARROW ){
						if( get_eq_char( ch, WEAR_ARROW )->value[0] >= 1 )
							sprintf( buf2, "%d" , get_eq_char( ch, WEAR_ARROW )->value[0] );
						else if( get_eq_char( ch, WEAR_ARROW )->value[0] == -1 ) // µL­­¼Æ¶q 2023/01/05
							sprintf( buf2, "¡Û" );
						else
							sprintf( buf2, "0" );
					}else{
						sprintf( buf2, " " );
					}
					i = buf2;
					break;
				// ·s¼W D ªí¥Ü ¸Ë³Æªº¼uÃÄ ammo ¼Æ¶q 2022/04/29
				case 'D' :
					if( get_eq_char( ch, WEAR_AMMO ) && get_eq_char( ch, WEAR_AMMO )->item_type == ITEM_AMMO ){
						if( get_eq_char( ch, WEAR_AMMO )->value[0] >= 1 )
							sprintf( buf2, "%d" , get_eq_char( ch, WEAR_AMMO )->value[0] );
						else if( get_eq_char( ch, WEAR_AMMO )->value[0] == -1 ) // µL­­¼Æ¶q 2023/01/05
							sprintf( buf2, "¡Û" );
						else
							sprintf( buf2, "0" );
					}else{
						sprintf( buf2, " " );
					}
					i = buf2;
					break;
			}
			++str;
			while( ( *point = *i ) != '\0' )
			++point, ++i;      
		}
		write_to_buffer( d, buf, point - buf );  
	}
	return;
}

/*
 * Append onto an output buffer.
 */

void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
        length = strlen( txt );

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
        d->outbuf[0]      = '\n';
        d->outbuf[1]      = '\r';
        d->outtop  = 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
        char *outbuf;
        //if ( d->outsize >= 32000 )  2020/10/21 10 ­¿
		if ( d->outsize >= 320000 )
        {
            /* empty buffer */
            d->outtop = 0;
            bugf( "Buffer overflow. Closing (%s).",
                ( d->character ? d->character->name : "???" ) );
//      write_to_descriptor( d->descriptor, "\n\r*** BUFFER OVERFLOW!!! ***\n\r", 0 );
            close_socket( d );
            return;
        }
        outbuf      = alloc_mem( 2 * d->outsize );
//  ++top_desc;
        strncpy( outbuf, d->outbuf, d->outtop );
        free_mem( d->outbuf/*, d->outsize */);
        d->outbuf   = outbuf;
        d->outsize *= 2;
    }

    /*
     * Copy.  Modifications sent in by Zavod.
     */
    strncpy( d->outbuf + d->outtop, txt, length );
    d->outtop += length;
    return;
}
/*
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    *
     * Find length in case caller didn't.
     *
    if ( length <= 0 )
  length = strlen( txt );

    *
     * Initial \n\r if needed.
     *
    if ( d->outtop == 0 && !d->fcommand )
    {
  d->outbuf[0]      = '\n';
  d->outbuf[1]      = '\r';
  d->outtop  = 2;
    }

    *
     * Expand the buffer as needed.
     *
    while ( d->outtop + length >= d->outsize )
    {
  char *outbuf;

  outbuf      = alloc_mem( 2 * d->outsize );
  strncpy( outbuf, d->outbuf, d->outtop );
  free_mem( d->outbuf*, d->outsize* );
  d->outbuf   = outbuf;
  d->outsize *= 2;
    }

    *
     * Copy.  Modifications sent in by Zavod.
     *
    strncpy( d->outbuf + d->outtop, txt, length );
    d->outtop += length;
    return;
}
*/

/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined( macintosh )
    if ( desc == 0 ) desc = 1;
#endif

    if ( length <= 0 ) length = strlen( txt );

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
        nBlock = UMIN( length - iStart, 4096 );
#if !defined( WIN32 )
        if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
#else
        if ( ( nWrite = send( desc, txt + iStart, nBlock , 0) ) < 0 )
#endif
            { perror( "Write_to_descriptor_Test" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
  CHAR_DATA *ch;//, *horse;
  NOTE_DATA *pnote;
  CHAR_DATA *check;
  DESCRIPTOR_DATA *check1;
  char      *pwdnew;
  char      *classname;
  char      *p;
  char       buf [ MAX_STRING_LENGTH ];
  char       buf1 [ MAX_STRING_LENGTH ];
  int        iClass;
  int        iRace;
  int        lines;
  int        notes;
  bool       fOld;

  while ( isspace( *argument ) || !isascii ( *argument ) )
    argument++;

  /* This is here so we wont get warnings.  ch = NULL anyways - Kahn */
  ch    = d->character;

  switch ( d->connected )
  {

    default:
      bug( "Nanny: bad d->connected %d.", d->connected );
      close_socket( d );
      return;

    case CON_GET_NAME:
      if ( argument[0] == '\0' )
      {
        if( ++d->trytimes > _MAX_TRYTIMES_ ) {
          //write_to_buffer(d, "±K½X¿é¤J¥¢±Ñ¤Ó¦h¦¸¡I\n\r", 0);  2024/01/29 ±Ô­z¿ù»~
		  write_to_buffer(d, "¿é¤J¨¤¦â¦WºÙ¥¢±Ñ¤Ó¦h¦¸¡I\n\r", 0);
          close_socket(d);
        }else
          write_to_buffer(d, "½Ð­«·s¿é¤J¨¤¦â¦WºÙ¡G", 0);

        return;
      }

      argument[0] = UPPER( argument[0] );

      if ( !check_parse_name( argument ) )
      {
        write_to_buffer( d, "¨º¬O¤£¦Xªkªº¦W¦r¡A½Ð­«·s¿é¤J\n\r±zªº¦W¦r(­^¤å)¡G", 0 );
        return;
      }
      sprintf( log_buf, "Pre-name check: %s", argument );

      log_string( log_buf );
      fOld = load_char_obj( d, argument );
      ch   = d->character;

      if ( IS_SET( ch->act, PLR_DENY ) )
      {
        if( IS_NPC(ch) || time(NULL) <= ch->pcdata->denied ){
          //log_string("Envy_log: P4");
          sprintf( pipe_buf, "|P %d \n", (int)d );
          write( fd0[1], pipe_buf, strlen(pipe_buf) );
          sprintf( log_buf, "Denying access to %s, $U@$H.", argument );
          log_string( log_buf );
          write_to_buffer( d, "±z³Q¦C¦b¶Â¦W³æ·í¤¤\n\r", 0 );
          close_socket( d );
        }
        else REMOVE_BIT(ch->act, PLR_DENY);
        return;
      }

		// ¨¾¤î login µe­±¤¤ ­«¦W³y¦¨ crash 2024/01/29
		if ( check_playing( d, ch->name ) )
          return;

      for( check1 = descriptor_list;check1;check1 = check1->next)
      {
        //    if ( d == check1 || !check1->character || check1->character->deleted || IS_NPC(check1->character))
        //      continue;
        if ( d == check1 )
          continue;
        if ( (!check1->character 
              || check1->character->deleted 
              || IS_NPC(check1->character)) &&
            (!check1->original || !check1->original->name ) )
          continue;


        if (!str_cmp( d->character->name, check1->character->name) 
            && (!IS_IMMORTAL( check1->character )
              || ( check1->original 
                && !str_cmp(check1->original->name, d->character->name))) )
          if (check1->character->desc) 
          {
            write_to_buffer( d, "²{¦b½u¤W¦³³o­Ó¤Hªº¦s¦b¡C\n\r½Ð­«·s¿é¤J¦W¦r¡G", 0 );
            d->connected = CON_GET_NAME;
            extract_char( ch, TRUE );					//Added by Razgriz   20050802
            if( ++d->trytimes > _MAX_TRYTIMES_ ) close_socket(d);
            return;
          }
      }

      for( check = char_list;check;check = check->next)
      {
        if ( check->deleted 
            || IS_NPC( check ) 
            || IS_SET(check->act, PLR_HORSE))
          continue;
      if (!str_cmp( d->character->name, check->name) 
            && !IS_IMMORTAL( check ))
          if (check->desc) 
          {
            write_to_buffer( d, "²{¦b½u¤W¦³³o­Ó¤Hªº¦s¦b¡C\n\r½Ð­«·s¿é¤J¦W¦r¡G", 0 );
            d->connected = CON_GET_NAME;
            extract_char( ch, TRUE );					//Added by Razgriz   20050802
            if( ++d->trytimes > _MAX_TRYTIMES_ ) close_socket(d);
            return;
          }

      }

      if ( check_reconnect( d, argument, FALSE ) )
      {
        fOld = TRUE;
      }
      else
      {
        /* Must be immortal with wizbit in order to get in */
        if ( wizlock
            //&& !IS_HERO( ch )
            &&  !IS_SET( ch->act, PLR_WIZBIT ) 
            && get_trust(ch) < L_APP)
        {
          write_to_buffer( d, "¥»¹CÀ¸¥Ø«e¥u¤¹³\\§Å®vµn¤J¡C\n\r", 0 );
          close_socket( d );
          return;
        }
        if ( ch->level <= numlock
            && !IS_SET( ch->act, PLR_WIZBIT )
            && get_trust(ch) < L_APP
            && numlock != 0 )
        {
          write_to_buffer( d,
              "The game is locked to your level character.\n\r\n\r",
              0 );
          if ( ch->level == 0 )
          {
            write_to_buffer( d,
                "New characters are now temporarily in email ",
                0 );
            write_to_buffer( d, "registration mode.\n\r\n\r", 0 );
            write_to_buffer( d,
                "Please email <implementor addr here> to ", 0 );
            write_to_buffer( d, "register your character.\n\r\n\r",
                0 );
            write_to_buffer( d,
                "One email address per character please.\n\r", 0 );
            write_to_buffer( d, "Thank you, EnvyMud Staff.\n\r\n\r",
                0 );
          }
          close_socket( d ) ;
          return;
        }
      }

      if ( /*!IS_SET( ch->act, PLR_REPLACE ) &&*/ ch->level < L_APP )
        if ( check_playing( d, ch->name ) )
          return;

      if ( fOld )
      {
        /* Old player */
        write_to_buffer( d, "±K½X¡G", 0 );
        write_to_buffer( d, echo_off_str, 0 );
        d->connected = CON_GET_OLD_PASSWORD;
        return;
      }
      else
      {
        /* New player */
        sprintf( buf, "±z¨S¿é¤J¿ù§a¡H¬O %s ¶Ü(Y/N)? ", argument );
        write_to_buffer( d, buf, 0 );
        d->connected = CON_CONFIRM_NEW_NAME;
        return;
      }
      break;

    case CON_GET_OLD_PASSWORD:
#if defined( unix ) || defined( AmigaTCP )
      write_to_buffer( d, "\n\r", 2 );
#endif

      if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
      {
        write_to_buffer( d, "±K½X¿ù»~¡C\n\r½Ð­«·s¿é¤J±zªº¦W¦r¡G", 0 );
        extract_char( ch, TRUE );					//Added by Razgriz   20050619
        d->connected = CON_GET_NAME;
        if( ++d->trytimes > _MAX_TRYTIMES_ ) close_socket( d );
        return;
      }

      write_to_buffer( d, echo_on_str, 0 );

      if ( check_reconnect( d, ch->name, TRUE ) )
        return;

      if ( check_playing( d, ch->name) )
        return;

      //log_string("Envy_log: P5");
      sprintf( pipe_buf, "|P %d \n", (int)d );
      write( fd0[1], pipe_buf, strlen(pipe_buf) );
      sprintf( log_buf, "%s, $U@$H has connected.", ch->name );
      log_string( log_buf );
      lines = ch->pcdata->pagelen;
      ch->pcdata->pagelen = 40;
      if ( IS_IMMORTAL( ch ) )
        do_help( ch, "imotd" );
      do_help( ch, "motd" );
      ch->pcdata->pagelen = lines;
      d->connected = CON_READ_MOTD;
      //  ch->next  = char_list;
      //  char_list  = ch;
      break;

    case CON_CONFIRM_NEW_NAME:
      switch ( *argument )
      {
        case 'y': case 'Y':
          set_cname( ch, ch->name );
          sprintf( buf, "³o¬O¤@­Ó·sªº¨¤¦â¡C\n\r½Ð¬° %s ¿é¤J¤@­Ó±K½X¡G %s",
              ch->name, echo_off_str );
          write_to_buffer( d, buf, 0 );
          d->connected = CON_GET_NEW_PASSWORD;
          break;

        case 'n': case 'N':
          write_to_buffer( d, "¦n§a¡A¨º»ò¦A¿é¤J¤@¦¸¡G", 0 );
          free_char( d->character );
          d->character = NULL;
          d->connected = CON_GET_NAME;
          break;

        default:
          write_to_buffer( d, "½Ð¿é¤J yes ©Î no¡H", 0 );
          break;
      }
      break;

    case CON_GET_NEW_PASSWORD:
#if defined( unix ) || defined( AmigaTCP )
      write_to_buffer( d, "\n\r", 2 );
#endif

      if ( strlen( argument ) < 5 )
      {
        write_to_buffer( d,
            "±K½X¦Ü¤Ö­n¦³¤­­Ó­^¤å¦r¥À©Î¼Æ¦r\n\r½Ð­«·s¿é¤J±K½X¡G ",
            0 );
        return;
      }

      pwdnew = crypt( argument, ch->name );
      for ( p = pwdnew; *p != '\0'; p++ )
      {
        if ( *p == '~' )
        {
          write_to_buffer( d,
              "±K½X¤¤¦³¤£¦Xªk¦r¤¸¡A½Ð­«·s¿é¤J.\n\rPassword: ",
              0 );
          return;
        }
      }

      free_string( ch->pcdata->pwd );
      ch->pcdata->pwd        = str_dup( pwdnew );
      write_to_buffer( d, "½Ð¦A¿é¤J¤@¦¸±K½X¡A¥H½T©w±z¨S°O¿ù¡G", 0 );
      d->connected = CON_CONFIRM_NEW_PASSWORD;
      break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined( unix ) || defined( AmigaTCP )
      write_to_buffer( d, "\n\r", 2 );
#endif

      if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
      {
        write_to_buffer( d, "«e«á¨â¦¸±K½X¤£²Å\n\r½Ð­«·s¿é¤J±K½X¡G ",
            0 );
        d->connected = CON_GET_NEW_PASSWORD;
        return;
      }

      write_to_buffer( d, echo_on_str, 0 );
      write_to_buffer( d, "\n\r½Ð«ö Return ÁäÄ~Äò¡G\n\r", 0 );
      d->connected = CON_DISPLAY_RACE;
      break;

    case CON_DISPLAY_RACE:
      strcpy( buf, "½Ð¿ï¾Ü¤@­ÓºØ±Ú¡G\n\r\n\r" );
      for ( iRace = 0; iRace < MAX_RACE; iRace++ )
      {
        if ( !IS_SET( race_table[ iRace ].race_abilities, RACE_PC_AVAIL ) )
          continue;
        //      if ( iRace > 0 )
        //    strcat( buf, " " );
        sprintf( buf1, "  %s(%s)\n\r", race_table[ iRace ].cname, race_table[ iRace ].name );
        strcat( buf, buf1 );
      }
      strcat( buf, "\n\r¡i½Ð¿é¤JºØ±Ú­^¤å¦WºÙ¡j¡G" );
      write_to_buffer( d, buf, 0 );
      d->connected = CON_GET_NEW_RACE;
      break;

    case CON_GET_NEW_RACE:
      for ( iRace = 0; iRace < MAX_RACE; iRace++ )
        if ( !str_prefix( argument, race_table[iRace].name )
            && IS_SET( race_table[ iRace ].race_abilities,
              RACE_PC_AVAIL ) )
        {
          ch->race = race_lookup( race_table[iRace].name );
          break;
        }

      if ( iRace == MAX_RACE )
      {
        write_to_buffer( d,
            "¨º¨Ã¤£¬O¤@­ÓºØ±Ú¡C\n\r½Ð¿é¤JºØ±Ú­^¤å¦WºÙ¡H ", 0 );
        return;
      }

      write_to_buffer( d, "\n\r", 0 );
      do_help( ch, race_table[ch->race].name );
      write_to_buffer( d, "±z½T©w­n¿ï¾Ü³o­ÓºØ±Ú¡H(Y/N)  ", 0 );
      d->connected = CON_CONFIRM_NEW_RACE;
      break;

    case CON_CONFIRM_NEW_RACE:
      switch ( argument[0] )
      {
        case 'y': case 'Y': break;
        default:
                  write_to_buffer( d, "\n\r½Ð«ö Return ÁäÄ~Äò¡G\n\r", 0 );
                  d->connected = CON_DISPLAY_RACE;
                  return;
      }

      write_to_buffer( d, "\n\r±zªº©Ê§O¬O¡H(M ¬°¨k©Ê¡AF ¬°¤k©Ê¡AN ¬°¤¤©Ê)? ", 0 );
      d->connected = CON_GET_NEW_SEX;
      break;

    case CON_GET_NEW_SEX:
      switch ( argument[0] )
      {
        case 'm': case 'M': ch->sex = SEX_MALE;    break;
        case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
        case 'n': case 'N': ch->sex = SEX_NEUTRAL; break;
        default:
                  write_to_buffer( d, "¤£¦Xªkªº©Ê§O¡A½Ð¿é¤J M, F ©Î N\n\r±zªº©Ê§O¬O? ", 0 );
                  return;
      }

      d->connected = CON_DISPLAY_CLASS;
      write_to_buffer( d, "\n\r½Ð«ö Return ÁäÄ~Äò¡G\n\r", 0 );
      break;

    case CON_DISPLAY_CLASS:
      strcpy( buf, "½Ð¿ï¾Ü±zªºÂ¾·~¡G\n\r\n\r" );
      for ( iClass = 0; iClass < 5; iClass++ )
      {
        //      if ( iClass > 0 )
        //    strcat( buf, " " );
        sprintf( buf1, "  %s(%s)\n\r", class_table[iClass].cname, class_table[iClass].who_name );
        strcat( buf, buf1 );
      }
      strcat( buf, "\n\r¡i½Ð¿é¤JÂ¾·~­^¤å¦WºÙ¡j¡G" );
      write_to_buffer( d, buf, 0 );
      d->connected = CON_GET_NEW_CLASS;
      break;

    case CON_GET_NEW_CLASS:
      for ( iClass = 0; iClass < 5; iClass++ )
      {
        if ( !str_prefix( argument, class_table[iClass].who_name ) )
        {
          ch->class = iClass;
          break;
        }
      }

      if ( iClass == 5 )
      {
        write_to_buffer( d,
            "¨º¨Ã¤£¬O¤@­ÓÂ¾·~¡C\n\r±zªºÂ¾·~¬O¡H ", 0 );
        return;
      }

      write_to_buffer( d, "\n\r", 0 );

      /* define the classname for helps. */
      switch ( ch->class )
      {
        default: classname = "";       break;
        case 0:  classname = "MAGE";       break;
        case 1:  classname = "CLERIC";     break;
        case 2:  classname = "THIEF";      break;
        case 3:  classname = "WARRIOR";    break;
        case 4:  classname = "BARBARIAN"; break;
      }
      if ( classname != "" )
        do_help( ch, classname );
      else
        bug( "Nanny CON_GET_NEW_CLASS:  ch->class (%d) not valid",
            ch->class );

      write_to_buffer( d, "±z½T©w­n¿ï¾Ü³o­ÓÂ¾·~¶Ü¡H(Y/N)", 0 );
      d->connected = CON_CONFIRM_CLASS;
      break;

    case CON_CONFIRM_CLASS:
      switch ( argument[0] )
      {
        case 'y': case 'Y': break;
        default:
                  write_to_buffer( d, "\n\r½Ð«ö Return ÁäÄ~Äò¡G\n\r", 0 );
                  d->connected = CON_DISPLAY_CLASS;
                  return;
      }
      //log_string("Envy_log: P6");
      sprintf( pipe_buf, "|P %d \n", (int)d );
      write(fd0[1], pipe_buf, strlen(pipe_buf));
      sprintf( log_buf, "%s, $U@$H new player.", ch->name );
      log_string( log_buf );
      write_to_buffer( d, "\n\r", 2 );
      ch->pcdata->pagelen = 40;
      //  ch->dodge += race_table[ ch->race ].nature_dodge;
      //  ch->armor += race_table[ ch->race ].nature_ac;
      do_help( ch, "motd" );
      d->connected = CON_READ_MOTD;
      break;

    case CON_READ_MOTD:
      ch->next  = char_list;
      char_list  = ch;
      d->connected      = CON_PLAYING;

      send_to_char( "\n\r"HIP"Åwªï¨Ó¨ì³o­Ó¯«©_ªº¤Û·Q¥@¬É¡A§Æ±æ±z¦b³o¸Ìªº«_ÀI¬G¨Æ¥Ã»·....¶Ç»w..."NOR"\n\r\n\r", ch );

      if ( ch->level == 0 )
      {
        OBJ_DATA *obj;

        switch ( class_table[ch->class].attr_prime )
        {
          case APPLY_STR: ch->perm_str = 16; break;
          case APPLY_INT: ch->perm_int = 16; break;
          case APPLY_WIS: ch->perm_wis = 16; break;
          case APPLY_DEX: ch->perm_dex = 16; break;
          case APPLY_CON: ch->perm_con = 16; break;
        }

        switch ( ch->class )
        {
          case MAGE :   ch->pcdata->prime_attr = APPLY_INT;
                  ch->pcdata->snd_attr = APPLY_NONE;
                  break;
          case CLERIC :   ch->pcdata->prime_attr = APPLY_WIS;
                  ch->pcdata->snd_attr = APPLY_NONE;
                  break;
          case THIEF :  ch->pcdata->prime_attr = APPLY_DEX;
                  ch->pcdata->snd_attr = APPLY_NONE;
                  break;
          case WARRIOR :  ch->pcdata->prime_attr = APPLY_STR;
                  ch->pcdata->snd_attr = APPLY_NONE;
                  break;
          case BARBARIAN :ch->pcdata->prime_attr = APPLY_CON;
                  ch->pcdata->snd_attr = APPLY_NONE;
                  break;
        }
        ch->level      = 1;
        ch->exp    = 1000;
        ch->gold  = 10000 + number_fuzzy( 3 )
          * number_fuzzy( 4 ) * number_fuzzy( 5 ) * 10;
        ch->hit    = ch->max_hit;
        ch->mana      = ch->max_mana;
        ch->move      = ch->max_move;
        sprintf( buf, "- %s",
            c_title_table [ch->class] [ch->level]
            [ch->sex == SEX_FEMALE ? 1 : 0] );
        set_title( ch, buf );
        free_string( ch->pcdata->prompt );
        ch->pcdata->prompt = str_dup( "[m<¥Í©R[1;32m%h[mÅ]ªk[1;34m%m[m²¾°Ê[1;35m%v[m> " );

        obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_BANNER ), 0 );
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_LIGHT );

        obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_SHIELD ), 0 );
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_SHIELD );

        obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_BOOK ), 0 );
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_HOLD );

        if ( !IS_SET( race_table[ ch->race ].race_abilities, RACE_BEHOLDER ) )
        {
          obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_VEST   ), 0 );
          obj_to_char( obj, ch );
          equip_char( ch, obj, WEAR_BODY );

          obj = create_object( 
              get_obj_index( class_table[ch->class].weapon ),
              0 );
          obj_to_char( obj, ch );
          equip_char( ch, obj, WEAR_WIELD );
        }

        char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
      }
      else if ( ch->in_room )
      {
        char_to_room( ch, ch->in_room );
      }
      else if ( IS_IMMORTAL( ch ) )
      {
        char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
      }
      else
      {
        char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
      }

      /*mary by coconet to end horse
        if ( IS_SET( ch->pcdata->ride, 1 ) )
        {
        sprintf( log_buf, "Load %s's horse.", ch->name );
        horse = load_horse_obj( d, ch->name );
        if ( !horse || horse->long_descr[0] == '\0' )
        {
        send_to_char( "§Aªº°¨¤£¨£¤F¡I\n\r", ch );
        ch->pcdata->ride = 0;
        ch->pcdata->horse = NULL;
        if ( horse )
        free_char( horse );
        }
        else
        {
        char_to_room( horse, ch->in_room );
        horse->next = char_list;
        char_list  = horse;
        add_follower( horse, ch );
        horse->leader = ch;
        ch->pcdata->horse = horse;
        horse->pcdata->horse = ch;
        }
        }
        */
      //Deduct ppl gold for charm boss bug, Amenda
      /*
         if ( ch->gold + ch->bank > 10000000 )
         {
         bug( "deduct player gold!", 0 );
         bug( ch->name, 0 );
         bug( "gold + bank = %d", ch->gold + ch->bank );
         ch->gold = 1000000;
         ch->bank = 0;  
         }
         */
      if ( !IS_SET( ch->act, PLR_WIZINVIS ) )
          //&& !IS_AFFECTED( ch, AFF_INVISIBLE ) )  ²¾°£ inv µn¤J¤£Åã¥Ü±Ô­z 2022/01/16
        act( "$n ³s½u¶i¤J³o­Ó¥@¬É.", ch, NULL, NULL, TO_ROOM );
      talk_channel( ch, "123", CHANNEL_SYSTEM, "«Å¥¬",HIU );
      check_login(ch); //add by jye 0940912
      abnormal_eq_check(ch);
      //abnormal_affect_check(ch);  remove at 20/05/12
      top10_check(ch);

      do_look( ch, "auto" );
      /* check for new notes */
      notes = 0;

      for ( pnote = note_list; pnote; pnote = pnote->next )
        if ( is_note_to( ch, pnote ) && str_cmp( ch->name, pnote->sender )
            && pnote->date_stamp > ch->last_note )
          notes++;

      if ( notes == 1 )
        send_to_char( "\n\r§A¦³¤@«Ê·sªº note¡C\n\r", ch );
      else
        if ( notes > 1 )
        {
          sprintf( buf, "\n\r§A¦³ %d «Ê·sªº note ¦bµ¥§A¡C\n\r",
              notes );
          send_to_char( buf, ch );
        }

      break;
  }

  return;
}

void abnormal_affect_check(CHAR_DATA *ch)
{
  AFFECT_DATA *paf, *temp_paf;
  bool has_been_removed = FALSE;
  for ( paf = ch->affected; paf; paf = paf->next ) {
    switch (paf->location) {
      case APPLY_AC:
        if (paf->modifier < -1000) {
          affect_remove(ch, paf);
          has_been_removed = TRUE;
        }
        continue;
      case APPLY_DODGE:
        if (paf->modifier < -50) {
          affect_remove(ch, paf);
          has_been_removed = TRUE;
        }
        continue;
      case APPLY_SAVING_BREATH:
      case APPLY_SAVING_SPELL:
      case APPLY_SAVING_ANCIENT:
      case APPLY_SAVING_SPIRIT:
      case APPLY_SAVING_FIRE:
      case APPLY_SAVING_COLD:
      case APPLY_SAVING_LIGHTNING:
      case APPLY_SAVING_EARTH:
      case APPLY_SAVING_WIND:
      case APPLY_SAVING_SAINT:
      case APPLY_SAVING_DARK:
      case APPLY_SAVING_POISON:
      case APPLY_SAVING_ELEMENT:
      case APPLY_SAVING_GOOD:
      case APPLY_SAVING_EVIL:
        if (paf->modifier < -30) {
          affect_remove(ch, paf);
          has_been_removed = TRUE;
        }
        continue;
      case APPLY_HIT:
      case APPLY_MANA:
      case APPLY_MOVE:
        if (paf->modifier > 500) {
          affect_remove(ch, paf);
          has_been_removed = TRUE;
        }
        continue;
      default:
        if (paf->modifier > 100) {
          if (paf->type != gsn_adore) {
            affect_remove(ch, paf);
            has_been_removed = TRUE;
          }
        }
        continue;
    }
  }
  if (has_been_removed) {
    send_to_char("\n\r§A¨­¤W©Ç²§ªºªk³NÂ÷§A¦Ó¥h\n\r\n\r", ch);
  }
}

void abnormal_eq_check(CHAR_DATA *ch)
{
  if (IS_NPC(ch)) return;
  if (ch->level >= 43) return;

  OBJ_DATA *obj = NULL;
  AFFECT_DATA *paf = NULL;
  int affect_count = 0;
  int wear_loc = 0;

  for( wear_loc = 0; wear_loc <= 20; wear_loc++ ) {
    obj = get_eq_char(ch, wear_loc);
    if (obj == NULL) continue;
    
    affect_count = 0;
    for (affect_count = 0, paf = obj->affected;
         paf;
         paf = paf->next, affect_count++ ){
    }

    if (affect_count > 10) {
      send_to_char("§AµLªk­t²ü¸Ë³Æ¤WªºÅ]¤O¡I\n\r", ch);
      act( "$p §â§A¼u¶}¡A±¼¸¨¦b¦a.", ch, obj, NULL, TO_CHAR );
      act( "$p ±q $n ¨­¤W¼u¸¨¦b¦a.",  ch, obj, NULL, TO_ROOM );
      obj_from_char( obj );
      obj_to_room( obj, ch->in_room );
    }
  }
}

//add by jye 0940912
void check_login( CHAR_DATA *ch)
{
  char       buf [ BUF_STRING_LENGTH * 5];
  DESCRIPTOR_DATA *d;
  DESCRIPTOR_DATA *ch_d;
  CHAR_DATA    *to;
  int        intCnt;
  
  //modify by jye 1050417
  return;
  
  if( !ch->desc)
      return;
      
  ch_d = ch->desc;
  
  intCnt = 1;
  sprintf( buf , HIR"System Warning! Multi Login " );
  
  for( d = descriptor_list; d; d = d->next )
  {
    to = (d->original) ? d->original : d->character;
    
    if( !to ) 
        continue;
        
    if( d->connected != CON_PLAYING )
        continue;

    if( IS_IMMORTAL( to ) || IS_IMMORTAL( ch ) )
        continue;
        
    if( !str_cmp( ch->name , to->name )) 
        continue;
    
    if( !str_cmp( ch_d->host , d->host ))
    {
        strcat(buf, " ");
        strcat(buf, to->name);
        intCnt++;
    }
  }
  
  // the maximum allowable multi
  if( intCnt > MAX_MULTI_LOGIN)
  {
      strcat(buf, " ");
      strcat(buf, ch->name);
      strcat(buf, NOR);
      strcat(buf, "\n\r");
      send_to_all_char( buf );
  }
      
  return;
}

/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto imm immortal self someone none . .. ./ ../ /" ) )
  return FALSE;

    /*
     * Obsenities
     */
    if ( is_name( name, "damn fuck screw shit ass asshole bitch bastard gay lesbian pussy fart vagina penis" ) )
  return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen( name ) <  3 )
  return FALSE;

#if defined( macintosh ) || defined( unix ) || defined( AmigaTCP )
    if ( strlen( name ) > 12 )
  return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
  char *pc;
  bool fIll;

  fIll = TRUE;
  for ( pc = name; *pc != '\0'; pc++ )
  {
      if ( !isalpha( *pc ) )
    return FALSE;
      if ( LOWER( *pc ) != 'i' && LOWER( *pc ) != 'l' )
    fIll = FALSE;
  }

  if ( fIll )
      return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
  extern MOB_INDEX_DATA *mob_index_hash [ MAX_KEY_HASH ];
         MOB_INDEX_DATA *pMobIndex;
         int         iHash;

  for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
  {
      for ( pMobIndex  = mob_index_hash[iHash];
      pMobIndex;
      pMobIndex  = pMobIndex->next )
      {
    if ( is_name( name, pMobIndex->player_name ) )
        return FALSE;
      }
  }
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
	CHAR_DATA *ch, *och;

	for ( ch = char_list; ch; ch = ch->next )
	{
		if ( ch->deleted || IS_NPC( ch ) || IS_SET(ch->act, PLR_HORSE))
			continue;

		if ( ( !fConn || !ch->desc ) && !str_cmp( d->character->name, ch->name))
		{
			if ( fConn == FALSE )
			{
				free_string( d->character->pcdata->pwd );
				d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
			}
			else
			{
				/*
					ch->desc->original->pcdata->switched  = FALSE;
					ch->desc->character = horse;
					ch->desc->original = NULL;
					horse->desc = ch->desc;
					ch->desc = NULL;
					REMOVE_BIT( horse->pcdata->ride, 2 );
					add_follower( ch, horse );
					horse->master = NULL;
					ch->leader = horse;
					horse->leader = NULL;
					act( "$nÃtÅx¦aÂ½¨­¸õ¤U¥Lªº§¤ÃM¡C", horse, NULL, NULL, TO_ROOM );
				*/
				/*
					if( IS_SET(ch->act, PLR_HORSE ) && ch->desc)
					{
						do_ride(ch, "");
						return check_reconnect(d, name, fConn);
					}
					else if( ch->pcdata->switched )
					{
						if( !ch->desc ) {
							bug("[check_reconnect] No desc.",0);
							continue;
						}
					}
				*/
				/*
				 *  ¦³¨âºØ¨ú¥N¡A¤@¬OÂ_½u¨ú¥N(ch->desc == NULL)¡A¤G¬O¨SÂ_½uµw¨ú¥N(ch->desc != NULL)
				 *  ¦pªG¬OÂ_½u¨ú¥N¡AÂ_½u®É·|¦Û°Ê¦³¤U°¨ªº°Ê§@¡C
				 *  ¦pªG¬Oµw¨ú¥N¡A¨S¦³¤U°¨¡A¦ý§Ú­Ì¥i¥H check ch->pcdata->ride == 2 ±oª¾¥¿¦bÃM°¨¡C
				 *  amenda
				 */
				if ( IS_SET( ch->pcdata->ride, 2 ) )
					do_ride( ch->pcdata->horse, "" );      //±j­¢¤U°¨¡AÅý¨Æ±¡³æ¯Â¤Æ¡C
				if ( ch->pcdata->switched )                //²{¦b¦³µw¨ú¥N¡A¥²¶· check imm switch ªº±¡ªp

				for( och = char_list; och; och = och->next )
				{
					if ( och->desc && och->desc->original == ch )
						do_return( och, "" );
				}

				if ( ch->desc )
					close_socket2( ch->desc );
				free_char( d->character );

				d->character = ch;
				ch->desc = d;

				ch->timer = 0;
				send_to_char( "­«·s³s½u§¹²¦.\n\r", ch );

				if (!IS_IMMORTAL( ch ) )                      //0941003 by jye imm char­«·s³s½u¤£Åãµø
					act( "$n ­«·s¦^¨ì³o­Ó¥@¬É.", ch, NULL, NULL, TO_ROOM );

				//log_string("Envy_log: P7");
				sprintf( pipe_buf, "|P %d \n", (int)d );
				write(fd0[1], pipe_buf, strlen(pipe_buf));
				sprintf( log_buf, "%s, $U@$H reconnected.", ch->name );
				log_string( log_buf );
				d->connected = CON_PLAYING;
			}
			return TRUE;
		}
	}

	return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
  if ( dold != d
      && dold->character
      && dold->connected != CON_GET_NAME
      //&& dold->connected != CON_GET_OLD_PASSWORD
      && !str_cmp( name, dold->original
      ? dold->original->name : dold->character->name )
      && !dold->character->deleted )
  {
      write_to_buffer( d, "²{¦b½u¤W¦³³o­Ó¤Hªº¦s¦b¡C\n\r½Ð­«·s¿é¤J¦W¦r¡G", 0 );
      d->connected = CON_GET_NAME;
      if ( d->character )
      {
    free_char( d->character );
    d->character = NULL;
      }
      return TRUE;
  }
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
	if (   !ch
		|| !ch->desc
		||  ch->desc->connected != CON_PLAYING
		|| !ch->was_in_room
		||  ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

	ch->timer = 0;
	char_from_room( ch );
	char_to_room( ch, ch->was_in_room );
	ch->was_in_room = NULL;

	if (!IS_IMMORTAL(ch)) //1051003 modify by jye
		act( "$n ¤S¦^¨ì¤F³o­Ó¥@¬É.", ch, NULL, NULL, TO_ROOM );
	return;
}

/*
 * Write to all in the room.
 */
void send_to_room( const char *txt, ROOM_INDEX_DATA *room )
{
    DESCRIPTOR_DATA *d;
    
    for ( d = descriptor_list; d; d = d->next )
  if ( d->character != NULL )
      if ( d->character->in_room == room )
    act( txt, d->character, NULL, NULL, TO_CHAR );
}

/*
 * Write to all characters.
 */
void send_to_all_char( const char *text )
{
    DESCRIPTOR_DATA *d;

    if ( !text )
  return;
    for ( d = descriptor_list; d; d = d->next )
  if ( d->connected == CON_PLAYING )
      send_to_char( text, d->character );
    return;
}

/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    char       buf [ MAX_STRING_LENGTH*15 ];
    int len ;

    if ( !txt || !ch ||!ch->desc )
        return;
    len = strlen(txt);

    /*
     * Bypass the paging procedure if the text output is small
     * Saves process time.
     */
    if ( ch->desc->showstr_point )
    {
        int diff = (int)( ch->desc->showstr_point - ch->desc->showstr_head);
        strcpy( buf, ch->desc->showstr_head );
        if( strlen(buf) + len >= 50000 ) {
            return;
        }
        strncat( buf, txt, 50000 - len );
        free_string( ch->desc->showstr_head );
        ch->desc->showstr_head = str_dup( buf );
        ch->desc->showstr_point = ch->desc->showstr_head + diff;

    }
    else if ( len  < 300 )
        //    if ( strlen( txt ) < 300 )
        write_to_buffer( ch->desc, txt, strlen( txt ) );
    else if( len < 50000 )
    {
        free_string( ch->desc->showstr_head );
        ch->desc->showstr_head  = str_dup( txt );
        ch->desc->showstr_point = ch->desc->showstr_head;
        show_string( ch->desc, "" );
    }

    return;
}

 /* The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom
    for porting this SillyMud code for MERC 2.0 and laying down the groundwork.
    Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which
    the improvements to the pager was modeled from.  - Kahn */
 /* 12/1/94 Fixed bounds and overflow bugs in pager thanks to BoneCrusher
    of EnvyMud Staff - Kahn */

/*
void show_string( struct descriptor_data *d, char *input )
{
    register char *scan;
       char  buffer[ MAX_STRING_LENGTH*6 ];
       char  buf   [ MAX_INPUT_LENGTH    ];
       int   line      = 0;
       int   toggle    = 0;
       int   pagelines = 20;

    one_argument( input, buf );

    switch( UPPER( buf[0] ) )
    {
    case '\0':
    case 'C': */ /* show next page of text */ /*
  break;

    case 'R': */ /* refresh current page of text */ /*
  toggle = 1;
  break;

    case 'B': */ /* scroll back a page of text */ /*
  toggle = 2;
  break;

    default: */ /*otherwise, stop the text viewing */ /*
  if ( d->showstr_head )
  {
      free_string( d->showstr_head );
      d->showstr_head = str_dup( "" );
  }
  d->showstr_point = 0;
  return;

    }

    if ( d->original )
  pagelines = d->original->pcdata->pagelen;
    else
  pagelines = d->character->pcdata->pagelen;

    if ( toggle )
    {
  if ( d->showstr_point == d->showstr_head )
      return;
  if ( toggle == 1 )
      line = -1;
  do
  {
      if ( *d->showstr_point == '\n' )
        if ( ( line++ ) == ( pagelines * toggle ) )
    break;
      d->showstr_point--;
  } while( d->showstr_point != d->showstr_head );
    }
    
    line    = 0;
    *buffer = 0;
    scan    = buffer;
    if ( *d->showstr_point )
    {
  do
  {
      *scan = *d->showstr_point;
      if ( *scan == '\n' )
        if ( ( line++ ) == pagelines )
    {
      scan++;
      break;
    }
      scan++;
      d->showstr_point++;
      if( *d->showstr_point == 0 )
        break;
  } while( 1 );
    }

    */ /* On advice by Scott Mobley and others */ /*
    *scan++ = '\n';
    *scan++ = '\r';

    *scan = 0;

    write_to_buffer( d, buffer, strlen( buffer ) );
    if ( *d->showstr_point == 0 )
    {
      free_string( d->showstr_head );
      d->showstr_head  = str_dup( "" );
      d->showstr_point = 0;
    }

    return;
}
*/

/* OLC, new pager for editing long descriptions. */
/* ========================================================================= */
/* - The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom for porting  */
/*   this SillyMud code for MERC 2.0 and laying down the groundwork.       */
/* - Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which the improvements*/
/*   to the pager was modeled from.  - Kahn            */
/* - Safer, allows very large pagelen now, and allows to page while switched */
/*   Zavod of jcowan.reslife.okstate.edu 4000.             */
/* ========================================================================= */

void show_string( DESCRIPTOR_DATA *d, char *input )
{
    char         *start, *end;
    char    arg[MAX_INPUT_LENGTH];
    int     lines = 0, pagelen;

    /* Set the page length */
    /* ------------------- */

    pagelen = d->original ? d->original->pcdata->pagelen
        : d->character->pcdata->pagelen;

    /* Check for the command entered */
    /* ----------------------------- */

    one_argument( input, arg );

    switch( UPPER( *arg ) )
    {
  /* Show the next page */

  case '\0':
  case 'C': lines = 0;
      break;
  
  /* Scroll back a page */

  case 'B': lines = -2 * pagelen;
      break;

  /* Help for show page */

  case 'H': write_to_buffer( d, "B     - Scroll back one page.\n\r", 0 );
      write_to_buffer( d, "C     - Continue scrolling.\n\r", 0 );
      write_to_buffer( d, "H     - This help menu.\n\r", 0 );
      write_to_buffer( d, "R     - Refresh the current page.\n\r",
           0 );
      write_to_buffer( d, "Enter - Continue Scrolling.\n\r", 0 );
      return;

  /* refresh the current page */

  case 'R': lines = -1 - pagelen;
      break;

  /* stop viewing */

  default:  free_string( d->showstr_head );
      d->showstr_head  = NULL;
      d->showstr_point = NULL;
      return;
    }

    /* do any backing up necessary to find the starting point */
    /* ------------------------------------------------------ */

    if ( lines < 0 )
    {
  for( start= d->showstr_point; start > d->showstr_head && lines < 0;
       start-- )
      if ( *start == '\r' )
    lines++;
    }
    else
  start = d->showstr_point;

    /* Find the ending point based on the page length */
    /* ---------------------------------------------- */

    lines  = 0;

    for ( end= start; *end && lines < pagelen; end++ )
  if ( *end == '\r' )
      lines++;

    d->showstr_point = end;

    if ( end - start )
  write_to_buffer( d, start, end - start );

    /* See if this is the end (or near the end) of the string */
    /* ------------------------------------------------------ */

    for ( ; isspace( *end ); end++ );

    if ( !*end )
    {
  free_string( d->showstr_head );
  d->showstr_head  = NULL;
  d->showstr_point = NULL;
    }

    return;
}

/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1,
          const void *arg2, int type )
{
     OBJ_DATA    *obj1  = (OBJ_DATA  *) arg1;
     OBJ_DATA    *obj2  = (OBJ_DATA  *) arg2;
     CHAR_DATA     *to;
     CHAR_DATA     *vch   = (CHAR_DATA *) arg2;
     static char *    const  he_she  [ ] = { "¥¦",  "¥L",  "¦o" };
     static char *    const  him_her [ ] = { "¥¦",  "¥L",  "¦o" };
     static char *    const  his_her [ ] = { "¥¦ªº","¥Lªº","¦oªº" };
     const  char      *str;
     const  char      *i;
     char      *point;
     char       buf     [ MAX_STRING_LENGTH ];
     char       buf1    [ MAX_STRING_LENGTH ];
     char       fname   [ MAX_INPUT_LENGTH  ];
     bool       fChinese;

    /*
     * Discard null and zero-length messages.
     */
    if ( !format || *format == '\0' ) return;

    if( ch->deleted ) {
        return;
    }

    if( ! ch->in_room ) {
        bug(" Act: no ch->in_room !",0);
        return;
    }
    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( !vch )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            sprintf( buf1, "Bad act string:  %s", format );
            bug( buf1, 0 );
            return;
        }
		if ( !vch->in_room )//amenda 6/2/01 for debug purpose
        {
			// ²¾°£¤£¥²­nªº bug log 2022/11/14
            //bug( "comm.c, act:vch not in any room", 0 );
            //bug( vch->name, 0 );
            return; 
        }
        to = vch->in_room->people;
    }
    
    for ( ; to; to = to->next_in_room )
    {
        if ( ( to->deleted )
            || ( !to->desc && IS_NPC( to ) && !(to->pIndexData->progtypes & ACT_PROG ))
            || !IS_AWAKE( to ) )
            continue;
            
        //1050619 add TO_ROOM_IMM ¥uµ¹room¤¤ªºimm¬Ý
        if ( type == TO_ROOM_IMM && !IS_IMMORTAL(to))
            continue;

        if ( type == TO_CHAR  && to != ch )
            continue;
        if ( type == TO_VICT  && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM  && to == ch )
            continue;
        else if ( type == TO_ROOM && !IS_NPC(to) && to->position == POS_FIGHTING && !IS_SET( to->act, PLR_COMBAT))
            continue;

        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
        else if ( type == TO_NOTVICT && !IS_NPC(to) && to->position == POS_FIGHTING && !IS_SET( to->act, PLR_COMBAT))
            continue;

        point      = buf;
        str    = format;
        fChinese=FALSE;
        while ( *str != '\0' )
        {
            if ( *str != '$' || fChinese )
            {
                fChinese = is_chinese_char( *str, fChinese );
                //if ( fChinese )
                //    fChinese=FALSE;
                //else if ( *str>='\x80' && *str<='\xff' )
                //    fChinese=TRUE;
                *point++ = *str++;
                continue;
            }
            ++str;

            if ( !arg2 && *str >= 'A' && *str <= 'Z' )
            {
                bug( "Act: missing arg2 for code %d.", *str );
                sprintf( buf1, "Bad act string:  %s", format );
                bug( buf1, 0 );
                i = " <@@@> ";
            }
            else
            {
                switch ( *str )
                {
                default:  bug( "comm.c Act: bad code %d.", *str );
                    sprintf( buf1, "comm.c, Bad act string:  %s", format );
                    bug( buf1, 0 );
                    i = " <@@@> ";        break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;            break;
                case 'T': i = (char *) arg2;              break;
                case 'n': i = PERS( ch,  to  );              break;
                case 'N': i = PERS( vch, to  );              break;
                case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];    break;
                case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];    break;
                case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];    break;
                case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];    break;
                case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];    break;
                case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];    break;

                case 'p':
                    i = can_see_obj( to, obj1 )
                      ? ( strcpy(name_buf, obj1->short_descr) )
                      : "¤£©úª««~";
                    break;

                case 'P':
                    i = can_see_obj( to, obj2 )
                      ? ( strcpy(name_buf, obj2->short_descr) )
                      : "¤£©úª««~";
                    break;

                case 'd':
                    if ( !arg2 || ( (char *) arg2 )[0] == '\0' )
                    {
                        i = "ªù";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                case '$':
                    i = "$";
                    break;
                }
            }
        
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }

        *point++ = '\n';
        *point++ = '\r';
        *point = '\0';  //Bug-Fix from ED
        buf[0]   = UPPER( buf[0] );
        if ( to->desc )
            send_to_char( buf, to );
    }

    MOBtrigger = TRUE;
    return;
}

void act_nonline( const char *format, CHAR_DATA *ch, const void *arg1,
          const void *arg2, int type )
{
     OBJ_DATA    *obj1  = (OBJ_DATA  *) arg1;
     OBJ_DATA    *obj2  = (OBJ_DATA  *) arg2;
     CHAR_DATA     *to;
     CHAR_DATA     *vch   = (CHAR_DATA *) arg2;
     static char *    const  he_she  [ ] = { "¥¦",  "¥L",  "¦o" };
     static char *    const  him_her [ ] = { "¥¦",  "¥L",  "¦o" };
     static char *    const  his_her [ ] = { "¥¦ªº","¥Lªº","¦oªº" };
     const  char      *str;
     const  char      *i;
     char      *point;
     char       buf     [ MAX_STRING_LENGTH ];
     char       buf1    [ MAX_STRING_LENGTH ];
     char       fname   [ MAX_INPUT_LENGTH  ];
     bool       fChinese;

    /*
     * Discard null and zero-length messages.
     */
    if ( !format || *format == '\0' ) return;

    if( ch->deleted ) {
        return;
    }

    if( ! ch->in_room ) {
        bug(" Act: no ch->in_room !",0);
        return;
    }
    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( !vch )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            sprintf( buf1, "Bad act string:  %s", format );
            bug( buf1, 0 );
            return;
        }
        if ( !vch->in_room )//amenda 6/2/01 for debug purpose
        {
            bug( "comm.c, act_nonline :vch not in any room", 0 );
            bug( vch->name, 0 );
            return; 
        }
        to = vch->in_room->people;
    }
    
    for ( ; to; to = to->next_in_room )
    {
        if ( ( to->deleted )
            || ( !to->desc && IS_NPC( to ) && !(to->pIndexData->progtypes & ACT_PROG ))
            || !IS_AWAKE( to ) )
            continue;
            
        //1050619 add TO_ROOM_IMM ¥uµ¹room¤¤ªºimm¬Ý
        if ( type == TO_ROOM_IMM && !IS_IMMORTAL(to))
            continue;

        if ( type == TO_CHAR  && to != ch )
            continue;
        if ( type == TO_VICT  && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM  && to == ch )
            continue;
        else if ( type == TO_ROOM && !IS_NPC(to) && to->position == POS_FIGHTING && !IS_SET( to->act, PLR_COMBAT))
            continue;

        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
        else if ( type == TO_NOTVICT && !IS_NPC(to) && to->position == POS_FIGHTING && !IS_SET( to->act, PLR_COMBAT))
            continue;

        point      = buf;
        str    = format;
        fChinese=FALSE;
        while ( *str != '\0' )
        {
            if ( *str != '$' || fChinese )
            {
                fChinese = is_chinese_char( *str, fChinese );
                //if ( fChinese )
                //    fChinese=FALSE;
                //else if ( *str>='\x80' && *str<='\xff' )
                //    fChinese=TRUE;
                *point++ = *str++;
                continue;
            }
            ++str;

            if ( !arg2 && *str >= 'A' && *str <= 'Z' )
            {
                bug( "Act: missing arg2 for code %d.", *str );
                sprintf( buf1, "Bad act string:  %s", format );
                bug( buf1, 0 );
                i = " <@@@> ";
            }
            else
            {
                switch ( *str )
                {
                default:  bug( "comm.c Act: bad code %d.", *str );
                    sprintf( buf1, "comm.c, Bad act string:  %s", format );
                    bug( buf1, 0 );
                    i = " <@@@> ";        break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;            break;
                case 'T': i = (char *) arg2;              break;
                case 'n': i = PERS( ch,  to  );              break;
                case 'N': i = PERS( vch, to  );              break;
                case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];    break;
                case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];    break;
                case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];    break;
                case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];    break;
                case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];    break;
                case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];    break;

                case 'p':
                    i = can_see_obj( to, obj1 )
                      ? ( strcpy(name_buf, obj1->short_descr) )
                      : "¤£©úª««~";
                    break;

                case 'P':
                    i = can_see_obj( to, obj2 )
                      ? ( strcpy(name_buf, obj2->short_descr) )
                      : "¤£©úª««~";
                    break;

                case 'd':
                    if ( !arg2 || ( (char *) arg2 )[0] == '\0' )
                    {
                        i = "ªù";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                case '$':
                    i = "$";
                    break;
                }
            }
        
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }

        *point = '\0';  //Bug-Fix from ED
        buf[0]   = UPPER( buf[0] );
        if ( to->desc )
            send_to_char( buf, to );
    }

    MOBtrigger = TRUE;
    return;
}

/*
 * The alternative interface of void act, using for
 * combat information. It is effenciency and clear than
 * origin act function.
 */
void combat_info( const char *format, CHAR_DATA *ch, const void *arg, int type )
{
     CHAR_DATA     *to;
     CHAR_DATA     *vch   = (CHAR_DATA *) arg;
     char       buf1    [ MAX_STRING_LENGTH ];


    /*
     * Discard null and zero-length messages.
     */
    if ( !format || *format == '\0' )
        return;

    if( ch->deleted ) {
        return;
    }

    if( ! ch->in_room ) {
        bug(" Combat_info: no ch->in_room !",0);
        return;
    }
    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( !vch )
        {
            bug( "Combat_info: null vch with TO_VICT.", 0 );
            sprintf( buf1, "Bad combat_info string:  %s", format );
            bug( buf1, 0 );
            return;
        }
        if ( !vch->in_room )//amenda 6/2/01 for debug purpose
        {
            //bug( "comm.c, combat_info:vch not in any room", 0 );
            //bug( vch->name, 0 );
            return;
        }
        to = vch->in_room->people;
    }
    switch(type){
      case TO_CHAR:
        combat_info_message(format, ch, ch, vch);
        break;
      case TO_VICT:
        combat_info_message(format, vch, ch, vch);
        break;
      case TO_ROOM:
        for(;to;to = to->next_in_room ){
            if ( ( to->deleted ) || ( !to->desc && IS_NPC( to ) &&
                !(to->pIndexData->progtypes & ACT_PROG )) || !IS_AWAKE( to ) )
                continue;
            if(to != ch && (IS_NPC(to) || IS_SET(to->act, PLR_COMBAT)))
                combat_info_message(format, to, ch, vch);
        }
        break;
      case TO_NOTVICT:
        for(;to;to = to->next_in_room ){
            if ( ( to->deleted ) || ( !to->desc && IS_NPC( to ) &&
                !(to->pIndexData->progtypes & ACT_PROG )) || !IS_AWAKE( to ) )
                continue;
            if(to != ch && to != vch && (IS_NPC(to) || IS_SET(to->act, PLR_COMBAT)))
                combat_info_message(format, to, ch, vch);
        }
        break;
      default:
        bug( "Combat_info: bad type", 0 );
    }

    MOBtrigger = TRUE;
    return;
}

// combat_info_message modified by jye 6.13.2005
void combat_info_message(const char *format, CHAR_DATA *to, CHAR_DATA *ch, CHAR_DATA *vch)
{
  //static char *    const  he_she  [ ] = { "¥¦",  "¥L",  "¦o" };
  static char *    const  him_her [ ] = { "¥¦",  "¥L",  "¦o" };
  //static char *    const  his_her [ ] = { "¥¦ªº","¥Lªº","¦oªº" };

  char buf[ MAX_STRING_LENGTH ];
  const char *str = format, *i;
  char *point = buf;
  bool fChinese = FALSE;
  char             buf1    [ MAX_STRING_LENGTH ];

    if ( !format || *format == '\0' )
		return;

    if( ch->deleted ) 
		return;
	
	/* Dirty Hack for solving combat_info bad formatted string 
	 * By Keric. 
	 */
	if( ch->position == POS_MORTAL )
		return;
  
  while( *str != '\0' )
  {
     if( *str != '$' || fChinese )
     {
         /*if ( fChinese )
           fChinese=FALSE;
           else if ( *str>='\x80' && *str<='\xff' )
           fChinese=TRUE;*/
         fChinese = is_chinese_char( *str, fChinese);
         *point++ = *str++;
         continue;
     }
     ++str;

     if( !vch && *str >= 'A' && *str <= 'Z' )
     {
         bug( "Combat_info_message: missing vch for code %d.", *str );
         sprintf( buf1, "Bad combat_info_message string:   %s", format );
         bug( buf1, 0 );
         i = " <@@@> ";
     }
     else
     {
         switch ( *str )
         {
           default:  bug( "comm.c combat_info_message: bad code %d.", *str );
             sprintf( buf1, "comm.c, Bad combat_info_message string:  %s", format );
             bug( buf1, 0 );
             i = " <@@@> ";           break;
             /* Thx alex for 't' idea */
             //case 't': i = (char *) arg1;       break;
             //case 'T': i = (char *) arg2;       break;
           case 'n': i = PERS( ch,   to  );       break;
           case 'N': i = PERS( vch, to  );        break;
           //case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];   break;
           //case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];   break;
           case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];   break;
           case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];   break;
           //case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];   break;
           //case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];   break;
     
           case '$': i = "$";           break;
         }
     }

     ++str;
     
     while( ( *point = *i ) != '\0' )
            ++point, ++i;
  }

  *point++ = '\n';
  *point++ = '\r';
  *point = '\0';  //Bug-Fix from ED
  buf[0]   = UPPER( buf[0] );
  if( to->desc )
      send_to_char( buf, to );

}



/*
 * Macintosh support functions.
 */
#if defined( macintosh )
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

/*
 * Windows 95 and Windows NT support functions
 */
#if defined( WIN32 )
void gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
  }
#endif

/*#if defined( unix ) || defined( AmigaTCP )
static jmp_buf timebuf;

*//* timeout - handle timeouts *//*
static void timeout( int sig )
{
    longjmp( timebuf, sig );
}

*//* rfc931_name - return remote user name *//*
char *rfc931_name( int desc, struct sockaddr_in *there)
{
    struct sockaddr_in   here;    *//* local link information *//*
    struct sockaddr_in   sin;   *//* for talking to RFC931 daemon *//*
    int      length;
    int      s;
    unsigned     remote;
    unsigned     local;
    static char    user[256];   *//* XXX *//*
    FILE    *fp;
    char    *cp;
    char    *result = FROM_UNKNOWN;

    *//* Find out local address and port number of stdin. *//*

    length = sizeof(here);
    if ( getsockname( desc, (struct sockaddr *) & here, &length ) == -1)
  return result;

    *//*
     * The socket that will be used for user name lookups should be bound to
     * the same local IP address as stdin. This will automagically happen on
     * hosts that have only one IP network interface. When the local host has
     * more than one IP network interface we must do an explicit bind() call.
     *//*

    if ( ( s = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
  return result;

    sin = here;
    sin.sin_port = 0;
    if ( bind( s, (struct sockaddr *)&sin, sizeof(sin) ) < 0 )
  return result;

    *//* Set up timer so we won't get stuck. *//*

    signal( SIGALRM, timeout );
    if ( setjmp(timebuf) )
    {
  close( s );       *//* not: fclose(fp) *//*
  return result;
    }
    alarm( TIMEOUT );

    *//* Connect to the RFC931 daemon. *//*
    sin = *there;
    sin.sin_port = htons( RFC931_PORT );
    if (connect(s, (struct sockaddr *) &sin, sizeof(sin) ) == -1
  || ( fp = fdopen( s, "w+" ) ) == 0 )
    {
  close( s );
  alarm( 0 );
  return result;
    }

    *//* Query the RFC 931 server. Would 13-byte writes ever be broken up? *//*
    fprintf( fp, "%u,%u\r\n", ntohs(there->sin_port), ntohs(here.sin_port) );
    fflush( fp );

    *//* Read response. Kill stdio buffer or we may read back our own query. *//*
    setbuf( fp, (char *) 0 );
    if ( fscanf( fp, "%u , %u : USERID :%*[^:]:%255s", &remote, &local, user ) == 3
  && ferror(fp) == 0
  && feof(fp) == 0
  && ntohs(there->sin_port) == remote
  && ntohs(here.sin_port) == local)
    {
  *//* Strip trailing carriage return. *//*
  if ( ( cp = strchr( user, '\r' ) ) )
      *cp = 0;
  result = user;
    }
    alarm( 0 );
    fclose( fp );

    return result;
}
#endif*/
