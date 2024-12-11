/******************************************************************************
 *  SSM v2.2 (shared string manager)					      *
 *									      *
 *  Copyright(C) 1996 Melvin Smith (Fusion) for EnvyMUD 2.2		      *
 *									      *
 *  Due to alignment differences on 32 bit and 64 bit machines, memory	      *
 *  usage is now virtually identical to standard Merc on 32-bit		      *
 *  architecture, but slightly larger on 64-bit. Memory usage is still	      *
 *  smaller than SSM 2.0 or earlier. The manager now uses short ints for      *
 *  the count and size of chunks, so to increase MAX_STRING you must	      *
 *  increase MAX_CHUNKS instead. String have a max reference count of	      *
 *  +32766 and max size of CHUNK_SIZE (0xfff0). Fragmentation is also	      *
 *  handled more efficiently by marking failed chunks with -1 to temporarily  *
 *  disable them until a defrag_heap() recycles them. This helps when a	      *
 *  4 byte chunk is freed low in the heap, so string_dup() doesn't walk	      *
 *  the whole heap every time.						      *
 *									      *
 *  <msmith@falcon.mercer.peachnet.edu>					      *
 *									      *
 *  ROM2.4 modifications by Tom Adriaenssen (Jan 1996) -- Wreck		      *
 *									      *
 *  <tadriaen@zorro.ruca.ua.ac.be>					      *
 *									      *
 *  Removed ROM 2.4 modifications as Envy doesnt need *fread_string_eol -Kahn *
 *									      *
 *****************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : act_wiz.c                                      *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         修改do_ostat(), 加入顯示obj_extra_value*
//*****************************************************************
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

#if !defined( ultrix )
#include <memory.h>
#endif

#define intType	       short int
#define uintType       unsigned intType
#define intTypeSize  ( sizeof( intType ) )
#define addrType       void * 
#define addrTypeSize ( sizeof( addrType ) )
#define addrSizeMask ( sizeof( addrType ) - 1 )

#if !defined( macintosh )
extern	int	_filbuf		args( (FILE *) );
#endif

typedef struct BE BufEntry;

struct BE
{
    BufEntry *next;
    uintType  size;   /* size of the chunk (regardless of NULL CHAR) */ 
    intType   usage;  /* how many pointers to the string */ 
    char      buf[1]; /* chunk starts here */
};

/*
 * This is for the temporary hashing of strings at bootup to speedup
 * comparison/crunching of the string space. The temp_string_hash will
 * be freed after boot_done() is called.
 */
typedef struct TH TempHash;

struct TH
{
    TempHash *next;
    uintType  len;
    char     *str;
};

TempHash **temp_string_hash; 

/* These are the original Merc vars in db.c */
extern bool  fBootDb;
char	     str_empty[1];
char	    *string_space;
char	    *top_string;
long	     nAllocString;
long	     sAllocString;
long	     nOverFlowString;
long	     sOverFlowString;

int	     numFree;
bool	     Full;

char	     *str_dup	     ( const char * );
void	      free_string    ( char * );
char	     *fread_string   ( FILE *, int * );
void	      temp_hash_add  ( char * );
char	     *temp_hash_find ( const char * );
char	     *diw_fread_string   ( FILE *, int * );
/*
 * ssm_buf_head points to start of shared space,
 * ssm_buf_free points to next free block
 */ 
BufEntry *ssm_buf_head, *ssm_buf_free;

/* To allocate more memory increase MAX_CHUNKS in merc.h. */
#define CHUNK_SIZE   0xfff0		     /* DO NOT mess with this! */ 
long	MAX_STRING = MAX_CHUNKS * CHUNK_SIZE; // 9828000
int	HEADER_SIZE;
extern int TOGGLE;
/*
 * Not sure what is a good value for MAX_FREE 
 * If a dup fails str_dup will not defrag unless the number
 * of numFree >= MAX_FREE. numFree is NOT the current number of free blocks,
 * it is just a counter so defrag doesnt start dragging the game in the
 * case of a lot of failed dups.
 */
#define MAX_FREE     1000

void init_string_space()
{
    BufEntry *walk;
    int i;

    string_space = (char *)malloc( MAX_STRING );
    if( !string_space )
    {
	bug( "[SSM] Cant allocate shared string space.", 0 );
	exit(1);
    }

    top_string	 = string_space + MAX_STRING-1;
    ssm_buf_head = (BufEntry *)string_space;
    HEADER_SIZE	 = (int)( (char*)&ssm_buf_head->buf[0] - (char*)ssm_buf_head );

    walk = ssm_buf_head;
    for( i = 0; ;i++ )
    {
	walk->usage = 0;
	walk->size  = CHUNK_SIZE - HEADER_SIZE;
	if( i < MAX_CHUNKS - 1 )
	{
	    walk->next = (BufEntry *)( (char*)walk + CHUNK_SIZE );
	    walk       = walk->next;
	    continue;
	}

	walk->next = 0;
	break;
    }

    ssm_buf_free	= ssm_buf_head;
    temp_string_hash	= (TempHash **)calloc( sizeof(TempHash *),
					      MAX_KEY_HASH );
}

int defrag_heap()
{
    /*
     * Walk through the shared heap and merge adjacent free blocks.
     * Free blocks are merged in str_free if free->next is free but
     * if the block preceding free is free, it stays unmerged. I would
     * rather not have the heap as a DOUBLE linked list for 2 reasons...
     *	(1) Extra 4 bytes per struct uses more mem
     *	(2) Speed - don't want to bog down str_ functions with heap management
     * The "orphaned" blocks will eventually be either merged or reused.
     * The str_dup function will call defrag if it cant allocate a buf.
     */

    BufEntry *walk, *last_free, *next;
    int merges	 = 0;
    ssm_buf_free = 0;

    for( walk=ssm_buf_head,last_free=0; walk; walk = next )
      {
	next = walk->next;
	if( walk->usage > 0 )
	{
	    /* this block is in use so set last_free to NULL */
	    last_free = 0;
	    continue;
	}
	else if( !last_free )
	{
	    /* OK found a NEW free block, set last_free and move to next */
	    last_free = walk;
	    if( !ssm_buf_free )
		ssm_buf_free = walk; 

	    continue; 
	}
	else
	{
	    /* previous block free so merge walk into last_free and move on */
	    if( (long)last_free->size + (long)walk->size <= CHUNK_SIZE )
	    {
		merges++;
		last_free->size += walk->size + HEADER_SIZE;
		last_free->next	 = walk->next;
		last_free->usage = 0;
	    }
	    else
		last_free = walk;
	}
    }

    if( merges )
	bug( "[SSM] defrag_heap : made %d block merges.", merges );
    else
	bug( "[SSM] defrag_heap : resulted in 0 merges.", 0 ); 

    /* Start count over again */ 
    numFree = 0;
    return merges;
}


/*
 * Dup a string into shared space. If string exists, the usage count
 * gets incremented and the reference is returned. If the string does
 * not exist in heap, space is allocated and usage is 1.
 * This is a linked list first fit algorithm, so strings can be
 * freed. Upon bootup, there is a seperate hash table constructed in order
 * to do crunching, then the table is destroyed.
 */
char *new_str_dup( const char *str )
{
    static char *ret;
    int len;

    if ( !str )
        return NULL;

    len = strlen(str)+1;

    if (!((ret) = (char*) calloc ((len), sizeof(char))))         
    {                                                                   
        bug("malloc failure",0);                                       
        abort();                                                        
    }                 
    strcpy( ret, str );
    return ret;

}

char *str_dup( const char *str )
{
	BufEntry *ptr;
	char     *str_new;
	int	      len;
	int	      rlen;
	//char buf[ MAX_STRING_LENGTH ];

	if( !str || !*str ) return &str_empty[0];

	if( str > string_space && str < top_string ){
		ptr = (BufEntry *)( str - HEADER_SIZE );
		if( ptr->usage <= 0 ){
			bug( "str_dup : invalid str", 0 );
			// bug( str, 0 );
      return &str_empty[0];
		}
		ptr->usage++;
		return (char *)str;
	}

/*
if (TOGGLE){
strcpy( buf, "dup:");
strcat( buf, str);
bug( buf, 0);
}*/
	rlen = len = (int)strlen( str ) + 1;

    /* 
     * Round up to machine dependant address size.
     * Don't remove this, because when the BufEntry struct is overlaid
     * the struct must be aligned correctly.
     */

	if( ( len + HEADER_SIZE ) & addrSizeMask ) 
		len += addrTypeSize - ( ( len + HEADER_SIZE ) & addrSizeMask );

	if( ssm_buf_free ){ 

RETRY:
		for( ptr = ssm_buf_free; ptr; ptr = ptr->next )
			if( ptr->usage == 0 && ptr->size >= len ) break;

		if( !ptr ){
			if( numFree >= MAX_FREE ){
				int merges;

				bug( "[SSM] Attempting to optimize shared string heap.", 0 );
				merges = defrag_heap();

				/* goto is fine because defrag will return 0 next time */
				if( merges ) goto RETRY;
			}	

			str_new = (char *)malloc( rlen );
			strcpy( str_new, str ); 
			sOverFlowString += rlen;
			nOverFlowString++;
			return str_new;
		}else if( ptr->size - len >= HEADER_SIZE ){
			/* If there is at least header size excess break it up */

			BufEntry *temp;

			/* WARNING! - DONT REMOVE THE CASTS BELOW! - Fusion */
			temp = (BufEntry*)( (char *)ptr + HEADER_SIZE + len );
			temp->size	 = ptr->size - ( len + HEADER_SIZE );
			temp->next	 = ptr->next;
			temp->usage	 = 0;
			ptr->size    = len;
			ptr->next  	 = temp;
			ptr->usage	 = 1;
			ssm_buf_free = temp;
		}else{
			ptr->usage = 1;
			if( ptr != ssm_buf_free && ssm_buf_free) 
				ssm_buf_free->usage--; /* buf_free was skipped */

			for( ssm_buf_free = ssm_buf_head; ssm_buf_free; ssm_buf_free = ssm_buf_free->next ){
				if( ssm_buf_free->usage == 0 ) break;
			}
		}

		str_new = (char *)&ptr->buf[0];
		strcpy( str_new, str ); 
		nAllocString++;
		sAllocString += ptr->size + HEADER_SIZE;
	}else{
		/* A one time toggle just for bugging purposes */
		if( !Full ){
			bug( "[SSM] The shared string heap is full!", 0 );
			Full = 1;
		}
		str_new = (char *)malloc( rlen );
		strcpy( str_new, str ); 
		sOverFlowString += rlen;
		nOverFlowString++;
	} 

	return str_new;
}

/*
 * If string is in shared space, decrement usage, if usage then is 0,
 * free the chunk and attempt to merge with next node. Other
 * strings are freed with standard free.
 * Never call free/delete externally on a shared string.
 */
void new_free_string( char * point )
{
    if (!(point))                                                       
    {                                                                   
        bug("freeing null pointer",0);
    }                                                                   
    else                                                                
    {                                                                   
        free((point));                                                  
            (point) = NULL;                                                 
    }               
}
void free_string( char *str )
{
    BufEntry *ptr;
    //char buf[ MAX_STRING_LENGTH ];

    if( !str || str == &str_empty[0] )
	return;
/*
if (TOGGLE){
strcpy( buf, "free:");
strcat( buf, str);
bug( buf, 0);
}*/
    if( str > string_space && str < top_string )
    {
	ptr = (BufEntry *)( str - HEADER_SIZE );

	if( --ptr->usage > 0 )
	    return;
	else if( ptr->usage < 0 )
	{
	    bug( "SSM:free_string: multiple free or invalid string.", 0 );
	    bug( (char*)&ptr->buf[0], 0 );
	    return;
	}
	numFree++;
	sAllocString -= ( ptr->size + HEADER_SIZE );
	nAllocString--;

	if( ssm_buf_free > ptr )
	    ssm_buf_free = ptr;

	if( fBootDb )
	{
	    TempHash *ptr;
	    TempHash *walk;
	    int ihash = strlen( str ) % MAX_KEY_HASH;

	    for( ptr = temp_string_hash[ ihash ]; ptr; ptr = ptr->next )
	    {
		if( ptr->str != str )
		    continue;
		else if( ptr == temp_string_hash[ ihash ] )
		    temp_string_hash[ ihash ] = ptr->next;
		else
		    for( walk = temp_string_hash[ ihash ]; walk;
			walk = walk->next )
		    {
			if( walk->next == ptr )
			{
			    walk->next = ptr->next;
			    break;
			}
		    }

		free( ptr );
		break; 
	    }
	}
	return;
    }

    sOverFlowString -= strlen( str ) + 1;
    nOverFlowString--;
    if ( sOverFlowString < 0 || nOverFlowString < 0 )
    {
	bug( "SSM:free_string: string free from outside SS space.", 0 );
	bug( str, 0 );
    }
    free( str );
}


/*
 * Read and allocate space for a string from a file.
 * This replaces db.c fread_string
 * This is modified version of Furey's fread_string from Merc
 */
char *fread_string( FILE *fp, int *status )
{
    char buf[ MAX_STRING_LENGTH*12 ];
    char *ptr = buf;
    char  c;
    bool fChinese;
    fChinese = FALSE;

    *status = 0;

    do { c = getc( fp ); }
    while( isspace( c ) );

    if( ( *ptr++ = c ) == '~' )
	return &str_empty[0];
    else 
		fChinese = is_chinese_char(c, fChinese);
	//if ( c >= '\x80' && c <= '\xff' )
	//fChinese = TRUE;

    for ( ;; )
    {
		c = getc ( fp );
		switch ( *ptr = c /* getc( fp ) */ )
		{
			default:
				ptr++;
				break;

			case EOF:
				bug( "Fread_string: EOF", 0 );
				*status = 1;
				return NULL;
				break;

			case '\n':
				ptr++;
				*ptr++ = '\r';
				break;

			case '\r':
				break;

			case '~':
				if ( !fChinese ) {
					*ptr = '\0';
					if( fBootDb )
					{ 
						ptr = temp_hash_find( buf ); 
						if( ptr )
							return str_dup( ptr ); 

						ptr = str_dup( buf );
						temp_hash_add( ptr );
						return ptr;
					}

					return str_dup( buf );
				}
				else {
					ptr++;
					break;
				}
		}

		fChinese = is_chinese_char( c, fChinese);
		//if ( fChinese == TRUE )
		//    fChinese = FALSE;
		//else if ( c >= '\x80' && c <= '\xff' )
		//    fChinese = TRUE;
	}
}

char *diw_fread_string( FILE *fp, int *status )
{
    char buf[ MAX_STRING_LENGTH*4 ];
    char *ptr = buf;
    char  c;
    bool fChinese;
    fChinese = FALSE;

    *status = 0;

	do { c = getc( fp ); }
	while( isspace( c ) );

	if( ( *ptr++ = c ) == '~' )
		return &str_empty[0];
	else //if ( c >= '\x80' && c <= '\xff' )
		fChinese = is_chinese_char( c, fChinese);

	for ( ;; )
    {
	c = getc ( fp );
	switch ( *ptr = c /* getc( fp ) */ )
	{
	    default:
		ptr++;
		break;

	    case EOF:
		bug( "Fread_string: EOF", 0 );
		*status = 1;
		return NULL;
		break;

	    case '\n':
		    *ptr = '\0';
		    if( fBootDb )
		    { 
			ptr = temp_hash_find( buf ); 
			if( ptr )
			    return str_dup( ptr ); 
		    
			ptr = str_dup( buf );
			temp_hash_add( ptr );
			return ptr;
		    }
		    return str_dup( buf );


	    case 32:
		    *ptr = '\0';
		    if( fBootDb )
		    { 
			ptr = temp_hash_find( buf ); 
			if( ptr )
			    return str_dup( ptr ); 
		    
			ptr = str_dup( buf );
			temp_hash_add( ptr );
			return ptr;
		    }
		    return str_dup( buf );

	    case '\r':
		break;

	    case '~':
		if ( !fChinese ) {
		    *ptr = '\0';
		    if( fBootDb )
		    { 
			ptr = temp_hash_find( buf ); 
			if( ptr )
			    return str_dup( ptr ); 
		    
			ptr = str_dup( buf );
			temp_hash_add( ptr );
			return ptr;
		    }

		    return str_dup( buf );
		}
		else {
		    ptr++;
		    break;
		}
	}
	fChinese = is_chinese_char( c, fChinese);
	/*if ( fChinese == TRUE )
	    fChinese = FALSE;
	else if ( c >= '\x80' && c <= '\xff' )
	    fChinese = TRUE;*/
    }
}

/*
 * Read string into user supplied buffer.
 * Modified version of Furey's fread_string
 */
void temp_fread_string( FILE *fp, char *buf )
{
    char *ptr = buf;
    char  c;
    bool  fChinese;
    fChinese=FALSE;

    do { c = getc( fp ); }
    while ( isspace( c ) );

    if ( ( *ptr++ = c ) == '~' )
    {	
	*buf = '\0';
	return;
    }
    else //if ( c >= '\x80' && c <= '\xff' )
	fChinese = is_chinese_char(c, fChinese);

    for ( ;; )
    {
	c=getc( fp );
	switch ( *ptr = c /* getc( fp ) */ )
	{
	    default:
		ptr++;
		break;

	    case EOF:
		bug( "Fread_string: EOF", 0 );
		exit( 1 );
		break;

	    case '\n':
		ptr++;
		*ptr++ = '\r';
		break;

	    case '\r':
		break;

	    case '~':
		if ( fChinese ) {
		    ptr++;
		    break;
		}
		else {
		    *ptr = '\0';
		    return;
		}
	}
	fChinese = is_chinese_char(c, fChinese);
	//if ( fChinese )
	//    fChinese=FALSE;
	//else if ( c>='\x80' && c<='\xff' )
	//    fChinese=TRUE;
    }
}


/* Lookup the string in the boot-time hash table. */
char *temp_hash_find( const char *str )
{
    TempHash *ptr;
    int	      len;
    int	      ihash;

    if( !fBootDb || !*str )
	return 0;

    len	  = strlen( str );
    ihash = len % MAX_KEY_HASH;

    for( ptr = temp_string_hash[ ihash ]; ptr; ptr = ptr->next )
    {
	if( *ptr->str != *str )
	    continue;
	else if( strcmp( ptr->str, str ) )
	    continue;
	else return ptr->str;
    }

    return 0;
}


/*
 * Add a reference in the temporary hash table.
 * String is still in the linked list structure but
 * reference is kept here for quick lookup at boot time;
 */
void temp_hash_add( char *str )
{
    TempHash *add;
    int	     len;
    int	     ihash;

    if( !fBootDb || !*str || ( str <= string_space && str >= top_string ) )
	return;

    len	  = strlen( str );
    ihash = len % MAX_KEY_HASH;
    add	  = (TempHash *)malloc( sizeof( TempHash ) );
    add->next = temp_string_hash[ ihash ];
    temp_string_hash[ ihash ] = add;
    add->len = len;
    add->str = str;
}


/* Free the temp boot string hash table */
void boot_done( void )
{
    TempHash *ptr, *next;
    int	      ihash;

    for( ihash = 0; ihash < MAX_KEY_HASH; ihash++ )
    {
	for( ptr = temp_string_hash[ ihash ]; ptr; ptr = next )
	{
	    next = ptr->next;
	    free( ptr );
	}
    }

    free( temp_string_hash );
    temp_string_hash = 0; /* Bug check in case someone accesses later */
}

void do_ostat( CHAR_DATA *ch, char *argument )
{
	OBJ_DATA *obj;
	CHAR_DATA *rch;
	AFFECT_DATA *paf;
	char buf [ MAX_STRING_LENGTH ];
	char buf1 [ MAX_STRING_LENGTH * 4];
	char arg [ MAX_INPUT_LENGTH ];
	int type= -1;
	//int count = 0;

	//BufEntry *ptr;
	//char	*str;
	//int i;

	rch = get_char( ch );
	/* remove at 2020/05/31 縮減 bug log
	if( ch->pcdata )
	bug("ch->ride = %d", ch->pcdata->ride);
	if( ch->pcdata->horse && ch->pcdata->horse->pcdata )
	bug("horse->ride = %d", ch->pcdata->horse->pcdata->ride);
	*/
	if ( !authorized( rch, "ostat" ) )
		return;

	one_argument( argument, arg );

	if ( arg[0] == '\0' )
	{
		send_to_char( "Ostat 什麼?\n\r", ch );
		/*
			if (TOGGLE){
			str = str_dup("start dumping");
			ptr = (BufEntry *)( str - HEADER_SIZE );
			for (i=0;i<100;i++)
			{
			if (ptr->usage == 1)
			 bug((char *)&ptr->buf[0],0);
			 ptr = ptr->next;
			}
			}
		*/
		TOGGLE = !TOGGLE;
		return;
	}

	buf1[0] = '\0';

	if ( !( obj = get_obj_world( ch, arg ) ) )
	{
		send_to_char( "沒有這樣物品.\n\r", ch);
		return;
	}

	sprintf( buf, "Name: [%s]\n\r", obj->name );
	strcat( buf1, buf );

	sprintf( buf, "Serial: [%ld]\n\r", (long)obj->serial);
	strcat(buf1, buf);


	if( obj->material ) {
		sprintf( buf, "Material: %s\n\r", obj->material->short_descr );
		strcat(buf1, buf);
	}

	sprintf( buf, "Vnum: %d.  Type: %s.\n\r", obj->pIndexData->vnum, item_type_name( obj ) );
	strcat( buf1, buf );

	sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r", obj->short_descr, obj->description );
	strcat( buf1, buf );

	sprintf( buf, "Wear bits: %s.  Extra bits: %s.\n\r", wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags ) );
	strcat( buf1, buf );

	if( IS_SET(obj->extra_flags, ITEM_UNIQUE)){
		sprintf(buf, "Unique, Replica : %d.\n\r", obj->pIndexData->replica );
		strcat(buf1, buf);
	}

	sprintf( buf, "Number: %d/%d.  Weight: %d/%d.\n\r", 1, get_obj_number( obj ), obj->weight, get_obj_weight( obj ) );
	strcat( buf1, buf );

	sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.  Class: %d.  Race: %d\n\r", obj->cost, obj->timer, obj->level, obj->class, obj->race );
	strcat( buf1, buf );

	sprintf( buf,
				"In room: %d.  In object: %s%s%s%s.	 Carried by: %s%s%s%s.	Wear_loc: %d.\n\r",
					!obj->in_room    ?	      0 : obj->in_room->vnum,
					!obj->in_obj     ?	     "" : obj->in_obj->short_descr,
					!obj->in_obj     ?	     "" : "(",
					!obj->in_obj     ? "（無）" : obj->in_obj->name,
					!obj->in_obj     ?	     "" : ")",
					!obj->carried_by ?	     "" : obj->carried_by->short_descr,
					!obj->carried_by ?	     "" : "(",
					!obj->carried_by ? "（無）" : obj->carried_by->name,
					!obj->carried_by ?	     "" : ")",
					obj->wear_loc );
	strcat( buf1, buf );

	sprintf( buf, "Values: %d %d %d %d %d.\n\r", obj->value[0], obj->value[1], obj->value[2], obj->value[3],obj->value[4] );
	strcat( buf1, buf );

	// 增加 obj rember 2022/11/10
	if ( obj->orembch )
		sprintf ( buf, "Obj Rember: %s.\n\r", obj->orembch->name );
	else
		sprintf ( buf, "Obj Rember: (none).\n\r");
	strcat( buf1, buf );


	/* Added by Razgriz 20050918
	 * Display obj_extra_value
	 */
	if ( ( obj->extra_value ).used ) sprintf ( buf, "ExValue: %d %d %d %d.\n\r",
												  ( obj->extra_value ).v[0],
												  ( obj->extra_value ).v[1],
												  ( obj->extra_value ).t[0],
												  ( obj->extra_value ).t[1] );
	else
		sprintf ( buf, "ExValue: (Unused).\n\r");
	strcat( buf1, buf );

	sprintf(buf, "Durability: %d/%d.\n\r", get_curr_dura(obj), get_max_dura(obj));
	strcat(buf1, buf);

	if ( obj->extra_descr || obj->pIndexData->extra_descr )
	{
		EXTRA_DESCR_DATA *ed;

		strcat( buf1, "Extra description keywords: '" );

		for ( ed = obj->extra_descr; ed; ed = ed->next )
		{
			strcat( buf1, ed->keyword );
			if ( ed->next )
				strcat( buf1, " " );
		}

		for ( ed = obj->pIndexData->extra_descr; ed; ed = ed->next )
		{
			strcat( buf1, ed->keyword );
			if ( ed->next )
				strcat( buf1, " " );
		}

		strcat( buf1, "'.\n\r" );
	}

	send_to_char(buf1, ch);
	buf1[0] = '\0';


	for ( paf = obj->affected; paf; paf = paf->next )
	{
		// affects has timer , 2021/09/29
		if ( paf->duration >= 0  )
			sprintf( buf, "Affects %s by %d. timer: %d . \n\r", affect_loc_name( paf->location ), paf->modifier, paf->duration);
		else
			sprintf( buf, "Affects %s by %d.\n\r", affect_loc_name( paf->location ), paf->modifier );
		//sprintf( buf, "Affects %s by %d.\n\r",
		//affect_loc_name( paf->location ), paf->modifier );
		strcat( buf1, buf );
	}

	for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
	{
		// affects has timer , 2021/09/29
		if ( paf->duration >= 0  )
			sprintf( buf, "Affects %s by %d. timer: %d . \n\r", affect_loc_name( paf->location ), paf->modifier, paf->duration);
		else
			sprintf( buf, "Affects %s by %d.\n\r", affect_loc_name( paf->location ), paf->modifier );
		//sprintf( buf, "Affects %s by %d.\n\r",
		//affect_loc_name( paf->location ), paf->modifier );
		strcat( buf1, buf );
	}




	for ( paf = obj->material->affected; paf; paf = paf->next )
	{
		// affects has timer , 2021/09/29
		if ( paf->duration >= 0 )
			sprintf( buf, "MAffects %s by %d. timer: %d . \n\r", affect_loc_name( paf->location ), paf->modifier, paf->duration);
		else
			sprintf( buf, "MAffects %s by %d.\n\r", affect_loc_name( paf->location ), paf->modifier );
		//sprintf( buf, "MAffects %s by %d.\n\r",
		//affect_loc_name( paf->location ), paf->modifier );
		strcat( buf1, buf );
	}
	send_to_char(buf1, ch);
	buf1[0] = '\0';

	if( obj->vrace ) {
		OBJ_VRACE * ovr ;
		for( ovr = obj->vrace ; ovr ; ovr = ovr->next )
		{
			sprintf( buf, "VRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
							race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p, ovr->dr_mod_q, ovr->dr_mod_p);
			strcat(buf1, buf);
		}
	}


	if( obj->material->vrace ) {
		OBJ_VRACE * ovr ;
		for( ovr = obj->material->vrace ; ovr ; ovr = ovr->next )
		{
			sprintf( buf, "MVRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
							race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p, ovr->dr_mod_q, ovr->dr_mod_p);
			strcat(buf1, buf);
		}
	}

	if( obj->item_type == ITEM_ARROW )
		type = 14;
	else if( obj->item_type == ITEM_WEAPON)
		type = obj->value[3];

	if( type > -1 && type < MAX_TYPES ) {
		OBJ_VRACE * ovr ;
		for( ovr = (basic_types[type])->vrace ; ovr ; ovr = ovr->next )
		{
			sprintf( buf, "BVRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
							race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p, ovr->dr_mod_q, ovr->dr_mod_p);
			strcat(buf1, buf);
		}
	}

	send_to_char(buf1, ch);
	buf1[0] = '\0';
	if ( obj->hp )
	{
		OBJ_HP * ohp = obj->hp;
		for( ; ohp; ohp = ohp->next ) {
			sprintf( buf, "HP: %s. percent: %d. level: %d. event: %d.\n\r",
							ohp->hp_name, ohp->percent, ohp->level, ohp->event );
		strcat( buf1, buf );
		}
	}

	if( obj->material->hp ) 
	{
		OBJ_HP * ohp = obj->material->hp;
		for( ; ohp; ohp = ohp->next ) {
			sprintf( buf, "MHP: %s. percent: %d. level: %d. event: %d.\n\r",
							ohp->hp_name, ohp->percent, ohp->level, ohp->event );
		strcat( buf1, buf );
		}
	}

	send_to_char( buf1, ch );
	return;
}

