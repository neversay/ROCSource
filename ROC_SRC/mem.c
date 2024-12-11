/***************************************************************************
 *  File: mem.c								   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 *									   *
 *  This code was freely distributed with the The Isles 1.1 source code,   *
 *  and has been used here for OLC - OLC would not be what it is without   *
 *  all the previous coders who released their source code.		   *
 *									   *
 ***************************************************************************/



#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


/*
 * Globals
 */
extern		int			top_reset;
extern		int			top_area;
extern		int			top_exit;
extern		int			top_ed;
extern		int			top_room;
extern		int			top_callout;

AREA_DATA		*	area_free;
EXTRA_DESCR_DATA	*	extra_descr_free;
EXIT_DATA		*	exit_free;
ROOM_INDEX_DATA		*	room_index_free;
OBJ_INDEX_DATA		*	obj_index_free;
SHOP_DATA		*	shop_free;
MOB_INDEX_DATA		*	mob_index_free;
RESET_DATA		*	reset_free;
HELP_DATA		*	help_free;

HELP_DATA		*	help_last;


GROUP_DATA * new_group(void)
{
    GROUP_DATA *pGroup;

    if( !group_free ) {
        pGroup = alloc_mem(sizeof(*pGroup));
        pGroup->next = group_list;
        group_list = pGroup;
    }
    else {
        pGroup = group_free;
        group_free = group_free->next;
    }
    pGroup->leader = NULL;
    pGroup->members = NULL;

    return pGroup;
}

void free_group(GROUP_DATA *gd)
{
    GROUP_DATA *pNext;
    if( group_list == gd ) {
        group_list = gd->next;
    }
    else {
        for( pNext = group_list; pNext ; pNext = pNext->next ){
            if(pNext->next == gd ){
                pNext->next = gd->next;
            }
        }
    }
    free_string(gd->name);
    gd->leader = NULL;
    gd->members = NULL;
    gd->next = group_free;
    group_free = gd;
}

/*****************************************************************************
 Name:		new_reset_data
 Purpose:	Creates and clears a reset structure.
 ****************************************************************************/
RESET_DATA *new_reset_data( void )
{
    RESET_DATA *pReset;

    if ( !reset_free )
    {
	pReset		= alloc_mem( sizeof(*pReset) );
	top_reset++;
    }
    else
    {
	pReset		= reset_free;
	reset_free	= reset_free->next;
    }

    pReset->next	= NULL;
    pReset->command	= 'X';
    pReset->arg1	= 0;
    pReset->arg2	= 0;
    pReset->arg3	= 0;

    return pReset;
}

AUC_DATA *new_auction_data(  )
{
	AUC_DATA *pAuc = NULL;

	if( !auc_free ) {
	    pAuc = alloc_mem ( sizeof(*pAuc));
	}
	else {
	    pAuc = auc_free;
	    auc_free = auc_free->next;
	}
	pAuc->next = NULL;
	pAuc->owner = NULL;
	pAuc->buyer = NULL;
	pAuc->obj = NULL;
	pAuc->time = 0;
	pAuc->recall = 0;
	pAuc->price = 0;

	return pAuc;
}

void free_auction_data ( AUC_DATA *pAuc)
{
    pAuc->next = auc_free;
    auc_free = pAuc;
    return;
}


/*****************************************************************************
 Name:		free_reset_data
 Purpose:	Clears and deletes a reset structure.
 ****************************************************************************/
void free_reset_data( RESET_DATA *pReset )
{
    pReset->next	    = reset_free;
    reset_free		    = pReset;
    return;
}



/*****************************************************************************
 Name:		new_area
 Purpose:	Creates and clears a new area structure.
 ****************************************************************************/
AREA_DATA *new_area( void )
{
    AREA_DATA *pArea;
    char buf[MAX_INPUT_LENGTH];

    if ( !area_free )
    {
	pArea		= alloc_mem( sizeof(*pArea) );
	top_area++;
    }
    else
    {
	pArea		= area_free;
	area_free	= area_free->next;
    }

    pArea->next		= NULL;
    pArea->name		= str_dup( "New area" );
    pArea->recall	= ROOM_VNUM_TEMPLE;
    pArea->area_flags	= AREA_ADDED;
    pArea->security	= 1;
    pArea->builders	= str_dup( "None" );
    pArea->lvnum	= 0;
    pArea->uvnum	= 0;
    pArea->age		= 0;
    pArea->nplayer	= 0;
    pArea->identity	= str_dup("None");
    pArea->vnum		= top_area-1;		/* OLC 1.1b */
    sprintf( buf, "area%d.are", pArea->vnum );
    pArea->filename	= str_dup( buf );

    return pArea;
}



/*****************************************************************************
 Name:		free_area
 Purpose:	Clears and deletes an area structure.
 ****************************************************************************/
void free_area( AREA_DATA *pArea )
{
    free_string( pArea->name );
    free_string( pArea->filename );
    free_string( pArea->builders );

    pArea->next		=   area_free->next;
    area_free		=   pArea;
    return;
}



EXIT_DATA *new_exit( void )
{
    EXIT_DATA *pExit;

    if ( !exit_free )
    {
	pExit		=   alloc_mem( sizeof(*pExit) );
	top_exit++;
    }
    else
    {
	pExit		=   exit_free;
	exit_free	=   exit_free->next;
    }

    pExit->to_room	=   NULL;
    pExit->next		=   NULL;
    pExit->vnum		=   0;
    pExit->exit_info	=   0;
    pExit->key		=   0;
    pExit->keyword	=   &str_empty[0];;
    pExit->description	=   &str_empty[0];;
    pExit->rs_flags	=   0;

    return pExit;
}



void free_exit( EXIT_DATA *pExit )
{
    free_string( pExit->keyword );
    free_string( pExit->description );

    pExit->next		=   exit_free;
    exit_free		=   pExit;
    return;
}



EXTRA_DESCR_DATA *new_extra_descr( void )
{
    EXTRA_DESCR_DATA *pExtra;

    if ( !extra_descr_free )
    {
	pExtra		    =	alloc_mem( sizeof(*pExtra) );
	top_ed++;
    }
    else
    {
	pExtra		    =	extra_descr_free;
	extra_descr_free    =	extra_descr_free->next;
    }

    pExtra->keyword	    =	NULL;
    pExtra->description	    =	NULL;
    pExtra->next	    =	NULL;

    return pExtra;
}



void free_extra_descr( EXTRA_DESCR_DATA *pExtra )
{
    free_string( pExtra->keyword );
    free_string( pExtra->description );

    pExtra->next	=   extra_descr_free;
    extra_descr_free	=   pExtra;
    return;
}



ROOM_INDEX_DATA *new_room_index( void )
{
    ROOM_INDEX_DATA *pRoom;
    int door;

    if ( !room_index_free )
    {
	pRoom		=   alloc_mem( sizeof(*pRoom) );
	top_room++;
    }
    else
    {
	pRoom		=   room_index_free;
	room_index_free =   room_index_free->next;
    }

    pRoom->next		    =	NULL;
    pRoom->people	    =	NULL;
    pRoom->contents	    =	NULL;
    pRoom->extra_descr	    =	NULL;
    pRoom->area		    =	NULL;

    for ( door=0; door < MAX_DIR; door++ )
	pRoom->exit[door]   =	NULL;

    pRoom->name		    =	&str_empty[0];
    pRoom->description	    =	&str_empty[0];
    pRoom->vnum		    =	0;
    pRoom->room_flags	    =	0;
    pRoom->light	    =	0;
    pRoom->sector_type	    =	0;
    pRoom->rrembch	    =	NULL;
    pRoom->roomprogs	    =	NULL;
    pRoom->progtypes	    =	0;
    pRoom->reset_first	    =	NULL;
    pRoom->reset_last	    =	NULL;
#ifdef HAS_ROOM_AFFECT
    pRoom->affected	    =	NULL;
    pRoom->affected_by	    =	0;
#endif
    return pRoom;
    
}



void free_room_index( ROOM_INDEX_DATA *pRoom )
{
    int door;
    EXTRA_DESCR_DATA *pExtra;
    RESET_DATA *pReset;

    free_string( pRoom->name );
    free_string( pRoom->description );

    for ( door = 0; door < MAX_DIR; door++ )
    {
	if ( pRoom->exit[door] )
	    free_exit( pRoom->exit[door] );
    }

    for ( pExtra = pRoom->extra_descr; pExtra; pExtra = pExtra->next )
    {
	free_extra_descr( pExtra );
    }

    for ( pReset = pRoom->reset_first; pReset; pReset = pReset->next )
    {
	free_reset_data( pReset );
    }

    pRoom->next	    =	room_index_free;
    room_index_free =	pRoom;
    return;
}

OBJ_HP * new_hiddenpower(void)
{
    OBJ_HP * pHP;
    if( obj_hp_free ) {
	pHP = obj_hp_free;
	obj_hp_free = pHP->next;
    }
    else {
	pHP = alloc_mem( sizeof(*pHP));
	top_hiddenpower ++;
    }
    pHP->next = NULL;
    pHP->vnum = -1;
    pHP->hp_name = &str_empty[0];
    pHP->percent = 0;
    pHP->event = 0;
    pHP->level = 0;
    return pHP;
    /*
    OBJ_HP *	next;
    int		vnum;
    char  *     hp_name;
    int		percent;
    int		event;
    int		level;
     **/
}


#ifdef HAS_ROOM_AFFECT
R_AFFECT_DATA *new_r_affect( void )
{
    R_AFFECT_DATA *pAf;

    if ( !r_affect_free )
    {
	pAf		=   alloc_mem( sizeof(*pAf) );
	top_affect++;
    }
    else
    {
	pAf		=   r_affect_free;
	r_affect_free	=   r_affect_free->next;
    }

    pAf->next	    =	NULL;
    pAf->next_in_list = NULL;
    pAf->prev_in_list = NULL;
    pAf->owner = NULL;
    pAf->type	    =	0;
    pAf->duration   =	0;
    pAf->bitvector  =	0;
    pAf->modifier = 0;
    pAf->deleted = FALSE;

    return pAf;
}



void free_r_affect( R_AFFECT_DATA* pAf )
{
    pAf->next		= r_affect_free;
    r_affect_free		= pAf;
    return;
}
#endif
AFFECT_DATA *new_affect( void )
{
    AFFECT_DATA *pAf;

    if ( !affect_free )
    {
	pAf		=   alloc_mem( sizeof(*pAf) );
	top_affect++;
    }
    else
    {
	pAf		=   affect_free;
	affect_free	=   affect_free->next;
    }

    pAf->next	    =	NULL;
    pAf->location   =	-1;
    pAf->modifier   =	-1;
    pAf->type	    =	0;
    pAf->duration   =	0;
    pAf->bitvector  =	0;
    pAf->deleted = FALSE;

    return pAf;
}



void free_affect( AFFECT_DATA* pAf )
{
    pAf->next		= affect_free;
    affect_free		= pAf;
    return;
}



SHOP_DATA *new_shop( void )
{
    SHOP_DATA *pShop;
    int buy;

    if ( !shop_free )
    {
	pShop		=   alloc_mem( sizeof(*pShop) );
	top_shop++;
    }
    else
    {
	pShop		=   shop_free;
	shop_free	=   shop_free->next;
    }

    pShop->next		=   NULL;
    pShop->keeper	=   0;

    for ( buy=0; buy<MAX_TRADE; buy++ )
	pShop->buy_type[buy]	=   0;

    pShop->profit_buy	=   100;
    pShop->profit_sell	=   100;
    pShop->open_hour	=   0;
    pShop->close_hour	=   23;

    return pShop;
}



void free_shop( SHOP_DATA *pShop )
{
    pShop->next = shop_free;
    shop_free	= pShop;
    return;
}

OBJ_VRACE *new_obj_vrace(void)
{
    OBJ_VRACE *pVrace;

    pVrace = alloc_mem( sizeof( *pVrace ));

    pVrace->next = NULL;
    pVrace->race = -1;
    pVrace->hr_mod_q = 0;
    pVrace->hr_mod_p = 100;
    pVrace->dr_mod_q = 0;
    pVrace->dr_mod_q = 100;
    return pVrace;
}

OBJ_INDEX_DATA *new_obj_index( void )
{
    OBJ_INDEX_DATA *pObj;
    int value;

    if ( !obj_index_free )
    {
	pObj	       =   alloc_mem( sizeof(*pObj) );
	++top_obj_index;
    }
    else
    {
	pObj		=   obj_index_free;
	obj_index_free	=   obj_index_free->next;
    }

    pObj->next		=   NULL;
    pObj->extra_descr	=   NULL;
    pObj->affected	=   NULL;
    pObj->area		=   NULL;
    pObj->name		=   str_dup( "no name" );
    pObj->short_descr	=   str_dup( "(no short description)" );
    pObj->description	=   str_dup( "(no description)" );
    pObj->vnum		=   0;
    pObj->item_type	=   ITEM_TRASH;
    pObj->extra_flags	=   0;
    pObj->wear_flags	=   0;
    pObj->max_level	=   MAX_LEVEL;
    pObj->count		=   0;
    pObj->weight	=   0;
    pObj->cost		=   0;
    pObj->level		=   1;
    pObj->vrace		= NULL;
    pObj->material		= NULL;
    pObj->hp		= NULL;
    for ( value=0; value<4; value++ )
	pObj->value[value]  =	0;

    return pObj;
}



void free_obj_index( OBJ_INDEX_DATA *pObj )
{
    EXTRA_DESCR_DATA *pExtra;
    AFFECT_DATA *pAf;

    free_string( pObj->name );
    free_string( pObj->short_descr );
    free_string( pObj->description );

    for ( pAf = pObj->affected; pAf; pAf = pAf->next )
    {
	free_affect( pAf );
    }

    for ( pExtra = pObj->extra_descr; pExtra; pExtra = pExtra->next )
    {
	free_extra_descr( pExtra );
    }
    
    pObj->next		    = obj_index_free;
    obj_index_free	    = pObj;
    return;
}

QMARK_STRING * new_qmark_string ( void )
{
    QMARK_STRING *q;
    q = alloc_mem( sizeof( *q ) );
    q->name = NULL;
    q->next = NULL;
    return q;
}

QMARK * new_qmark( void )
{
    QMARK *pQmark;
    if( qmark_free ) {
	pQmark = qmark_free;
	qmark_free = pQmark->next_in_list;
    }
    else {
	pQmark = alloc_mem( sizeof(*pQmark));
	++top_qmark ;
    }
    pQmark->owner = NULL;
    pQmark->prev_in_list = NULL;
    pQmark->next_in_list = NULL;
    pQmark->prev = NULL;
    pQmark->next= NULL;
    pQmark->mark_name = NULL;
    pQmark->timer = 0;
    pQmark->v0 = 0;
    pQmark->v1 = 0;
    pQmark->v2 = 0;
    pQmark->fDeath = TRUE;
    pQmark->fSave = FALSE;
    
    return pQmark;

    /*
    CHAR_DATA * owner;
    QMARK *     prev_in_list;
    QMARK *     next_in_list;
    QMARK *     prev;
    QMARK *     next;
    char  *     mark_name;
    int		timer;
    int		v0;
    int		v1;
    int		v2;
    bool	fDeath;
    bool	fSave;
     */
}

MOB_INDEX_DATA *new_mob_index( void )
{
	MOB_INDEX_DATA *pMob;
	int i;

	if ( !mob_index_free )
	{
		pMob = alloc_mem( sizeof(*pMob) );
		top_mob_index++;
	}
	else
	{
		pMob = mob_index_free;
		mob_index_free = mob_index_free->next;
	}

	for(i=0; i< MAX_SAVING; ++i)
		pMob->mod_svs[i] = 0;
	pMob->mod_str  = 0;
	pMob->mod_dex  = 0;
	pMob->mod_int  = 0;
	pMob->mod_wis  = 0;
	pMob->mod_con  = 0;
	pMob->mod_size = 0; // 體型 2022/12/23

	pMob->next			=   NULL;
	pMob->spec_fun		=   NULL;
	pMob->pShop			=   NULL;
	pMob->area			=   NULL;
	pMob->player_name	=   str_dup( "no name" );
	pMob->short_descr	=   str_dup( "(no short description)" );
	pMob->long_descr	=   str_dup( "(no long description)\n\r" );
	pMob->description	=   &str_empty[0];
	pMob->vnum			=   0;
	pMob->count			=   0;
	pMob->killed		=   0;
	pMob->sex			=   0;
	pMob->mod_exp		=   0;
	pMob->level			=   0;
	pMob->act			=   ACT_IS_NPC;
	pMob->affected_by	=   0;
	pMob->alignment		=   0;
	pMob->mod_hitplus	=   0;
	pMob->mod_mana		=   0;
	pMob->mod_hitroll	=   0;
	pMob->mod_damroll	=   0;
	pMob->mod_ac		=   0;
	pMob->dodge			=   0;
	pMob->mod_pp		=   0;
	pMob->gold			=   0;
	pMob->race			=   0;
	pMob->class			=   0;
	pMob->special		=   0;
	pMob->psk			=   NULL;
	pMob->jcl			=   NULL;
	pMob->mobprogs		=   NULL;
	pMob->progtypes		=   0;
	pMob->mob_action	=   NULL;
	pMob->area			=   NULL;

	return pMob;
}


RCALL_OUT_DATA * new_rcall_out()
{
    RCALL_OUT_DATA *cod;
    if( rcall_out_free ) {
		cod = rcall_out_free;
		rcall_out_free = cod->next;
		cod->next = NULL;
		cod->timer = 0;
    }else {
		++top_callout;
		cod = alloc_mem( sizeof( *cod));
		cod->next = NULL;
		cod->owner = NULL;
		cod->cmd = NULL;
		cod->timer = 0;
    }
    return cod;
}

CALL_OUT_DATA * new_call_out()
{
    CALL_OUT_DATA *cod;
    if( call_out_free ) {
		cod = call_out_free;
		call_out_free = cod->next;
		cod->next = NULL;
		cod->timer = 0;
    }else {
		++top_callout;
		cod = alloc_mem( sizeof( *cod));
		cod->next = NULL;
		cod->owner = NULL;
		cod->cmd = NULL;
		cod->timer = 0;
    }
    return cod;
}


void free_mob_index( MOB_INDEX_DATA *pMob )
{
    free_string( pMob->player_name );
    free_string( pMob->short_descr );
    free_string( pMob->long_descr );
    free_string( pMob->description );

    free_shop( pMob->pShop );

    pMob->next     =  mob_index_free;
    mob_index_free =  pMob;
    return;
}
