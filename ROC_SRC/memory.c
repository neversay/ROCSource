
 /***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.			   *
 *									   *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and	   *
 *  Mitchell Tse.							   *
 *									   *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.	   *
 *									   *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in	   *
 *  'license.txt', as well as the Envy license in 'license.nvy'.	   *
 *  In particular, you may not remove either of these copyright notices.   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

#if defined( macintosh )
#include <types.h>
#else
#if defined( unix )
#include <unistd.h>
#endif
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"
#include "olc.h"

#if !defined( macintosh )
extern	int	_filbuf		args( (FILE *) );
#endif

#if !defined( ultrix ) && !defined( apollo )
#include <memory.h>
#endif



/*
 * Globals.
 */
extern char		str_empty		[ 1		     ];


/*
 * Memory management.
 * Increase MAX_STRING from 1500000 if you have too.
 * Tune the others only if you understand what you're doing.
 * MAX_STRING is now in ssm.c
 */
extern int		MAX_STRING;

#if defined( macintosh )
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	12

void *			rgFreeList		[ MAX_MEM_LIST	     ];
void *			rgErrList	;
const int		rgSizeList		[ MAX_MEM_LIST	     ]	=
{
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768, 65536-64
};
#else
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	16

void *			rgFreeList		[ MAX_MEM_LIST	     ]  =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
void *			rgErrList	=0;
const int		rgSizeList		[ MAX_MEM_LIST	     ]	=
{
    4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072 - 64
};
#endif

extern int		nAllocString;
extern int		sAllocString;
extern int		nOverFlowString;
extern int		sOverFlowString;
extern bool		Full;
int			nAllocPerm;
int			sAllocPerm;



/*
 * Semi-locals.
 */

void	init_string_space( void );
void	boot_done( );

struct mem_block{
    int size;
    int valid;
    char memory[1];
} * MB;


/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
int TOGGLE;
void *alloc_mem( int sMem )
{
	void *pMem;
	int iList;

	sMem += (int)MB->memory - (int)&(MB->size);

	for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
	{
		if ( sMem <= rgSizeList[iList] )
			break;
	}

	if ( iList == MAX_MEM_LIST )
	{
		bug( "Alloc_mem: size %d too large.", sMem );
		exit( 1 );
	}

	if ( !rgFreeList[iList] )
	{
		pMem = alloc_perm( rgSizeList[iList] );
	}
	else
	{
		pMem = rgFreeList[iList];
		rgFreeList[iList] = * ( (void **) rgFreeList[iList] );
		if( (int)pMem == 0x41a70d0a || !( (int)pMem & 0x80000000)  ) {
			//  bug("Alloc_mem: rgFreeList[%d] is invalid block.", iList);
			pMem = alloc_perm( rgSizeList[iList] );
			return (void*)pMem;
		}
	}

	MB = (struct mem_block *)pMem;
	MB->size = sMem;
	MB->valid = TRUE;
	return (void*)MB->memory;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem)
{
	int iList;
	int sMem;

	MB = (struct mem_block *)(pMem - ((int)MB->memory - (int)&(MB->size)));
	sMem = MB->size;
	pMem = (void*) MB;

	if ( !MB->valid )
	{
		//bug( "Free_mem: freed already:%d" ,MB->size);
		return;
		exit( 1 );
	}


	for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
	{
		if ( sMem <= rgSizeList[iList] )
			break;
	}

	if ( iList == MAX_MEM_LIST )
	{
		bug( "Free_mem: size %d too large.", sMem );
		bugf( "%s", MB->memory );
		//sprintf(NULL, "123");
		top_errmem ++;
		* ((void**)pMem) = rgErrList;
		rgErrList = pMem;
		return;
		exit( 1 );
	}

	* ( (void **) pMem ) = rgFreeList[iList];
	rgFreeList[iList]  = pMem;
	MB->valid = FALSE;

	nAllocPerm -= 1;
	sAllocPerm -= sMem;

	return;
}



/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( int sMem )
{
	void *pMem;
	static char *pMemPerm;
	static int	 iMemPerm;

	while ( sMem % sizeof( long ) != 0 )
		sMem++;
	if ( sMem > MAX_PERM_BLOCK )
	{
		bug( "Alloc_perm: %d too large.", sMem );
		exit( 1 );
	}

	if ( !pMemPerm || iMemPerm + sMem > MAX_PERM_BLOCK )
	{
		iMemPerm = 0;
		if ( !( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) )
		{
			perror( "Alloc_perm" );
			exit( 1 );
		}
	}

	pMem	= pMemPerm + iMemPerm;
	iMemPerm   += sMem;
	nAllocPerm += 1;
	sAllocPerm += sMem;
	return pMem;
}

