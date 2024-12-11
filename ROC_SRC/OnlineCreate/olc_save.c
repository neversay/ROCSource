/***************************************************************************
 *  File: olc_save.c							   *
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
/* OLC_SAVE.C
 * This takes care of saving all the .are information.
 * Notes:
 * -If a good syntax checker is used for setting vnum ranges of areas
 *  then it would become possible to just cycle through vnums instead
 *  of using the iHash stuff and checking that the room or reset or
 *  mob etc is part of that area.
 */

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
#include "olc.h"



/*****************************************************************************
 Name:		fix_string
 Purpose:	Returns a string without \r and ~.
 ****************************************************************************/
char *fix_string( const char *str )
{
    static char strfix[MAX_STRING_LENGTH];
    int i;
    int o;
    bool fChinese;
    fChinese=FALSE;

    if ( str == NULL )
	return '\0';

    for ( o = i = 0; str[i+o] != '\0'; i++ )
    {
	if ( fChinese ) {
	    if (str[i+o] == '\r')
		o++;
	    fChinese=FALSE;
	}
	else {
	    if (str[i+o] == '\r' || str[i+o] == '~')
		o++;
		fChinese = is_chinese_char( str[i+o], fChinese);
	    //if (str[i+o]>='\x80' && str[i+o]<='\xff')
		//fChinese=TRUE;
	}
	strfix[i] = str[i+o];
    }
    strfix[i] = '\0';
    return strfix;
}



/*****************************************************************************
 Name:		save_area_list
 Purpose:	Saves the listing of files to be loaded at startup.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area_list()
{
    FILE *fp;
    AREA_DATA *pArea;

    if ( ( fp = fopen( "AREA.LST", "w" ) ) == NULL )
    {
	bug( "Save_area_list: fopen", 0 );
	perror( "AREA.LST" );
    }
    else
    {
	/*
	 * Add any help files that need to be loaded at
	 * startup to this section.
	 */
	fprintf( fp, "help.are\n" );
	fprintf( fp, "olc.hlp\n" );

	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    fprintf( fp, "%s\n", pArea->filename );
	}

	fprintf( fp, "$\n" );
	fclose( fp );
    }

    return;
}



/*****************************************************************************
 Name:		save_mobiles
 Purpose:	Save #MOBILES secion of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_mobiles( FILE *fp, AREA_DATA *pArea )
{
    int vnum;
    MOB_INDEX_DATA *pMobIndex;

    fprintf( fp, "#MOBILES\n" );
    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pMobIndex = get_mob_index(vnum) ) )
	{
	    if ( pMobIndex->area == pArea )
	    {
		fprintf( fp, "#%d\n",		pMobIndex->vnum );
		fprintf( fp, "%s~\n",		pMobIndex->player_name );
		fprintf( fp, "%s~\n",		fix_string( pMobIndex->short_descr ) );
/*		  fprintf( fp, "%s~\n",		fix_string( pMobIndex->cname ) );*/
		fprintf( fp, "%s~\n",		fix_string( pMobIndex->long_descr ) );
		fprintf( fp, "%s~\n",		fix_string( pMobIndex->description ) );
		fprintf( fp, "%lld ",		pMobIndex->act );
		fprintf( fp, "%lld ",		pMobIndex->affected_by );
		fprintf( fp, "%d S\n",		pMobIndex->alignment );
		fprintf( fp, "%d ",		pMobIndex->level );
		fprintf( fp, "%d ",		pMobIndex->mod_hitplus );
		fprintf( fp, "%d ",		pMobIndex->mod_mana );
		fprintf( fp, "%dd%d+%d ",	pMobIndex->mod_hitroll, 

						pMobIndex->mod_damroll, 
						pMobIndex->mod_ac );
		fprintf( fp, "%dd%d+%d\n",	pMobIndex->mod_svs[0], 
						pMobIndex->dodge, 
						pMobIndex->mod_pp );
		fprintf( fp, "%d ",		pMobIndex->gold );
		fprintf( fp, "0\n0 " );
/*		fprintf( fp, "%s~ ",		race_table[pMobIndex->race].name );*/
		fprintf( fp, "%d ", POS_STANDING );
		fprintf( fp, "%d\n",  pMobIndex->sex );
		/*
		 * This is responsible for writing MOBprograms in_file.
		 * By Farix of Oz.
		 */
		if ( pMobIndex->psk )
			fprintf( fp, "K %s~\n", fix_string( pMobIndex->psk ) );
		if ( pMobIndex->jcl )
			fprintf( fp, "J %s~\n", fix_string( pMobIndex->jcl ) );
		if ( pMobIndex->race != 0 )
			fprintf( fp, "R %s~\n", race_table[pMobIndex->race].name );
		if ( pMobIndex->class != 0 )
			fprintf( fp, "C %d\n", pMobIndex->class );
		if ( pMobIndex->special != 0 )
			fprintf( fp, "P %d\n", pMobIndex->special );
		if ( pMobIndex->weakness ){
		    MOB_WEAKNESS *weak;
		    for( weak = pMobIndex->weakness; weak; weak = weak->next ){
			fprintf( fp, "W %s~ %d %d\n",
				weak->name, weak->percent, weak->damage);
		    }
		}
		if ( pMobIndex->mob_action )
		{
			MOB_ACTION* mob_action;
		    for ( mob_action = pMobIndex->mob_action; mob_action; mob_action = mob_action->next )
		    {
			fprintf( fp, "A%d\n", mob_action->start_time );
			fprintf( fp, "%s~\n", mob_action->com_list );
		    }
		    fprintf( fp, "|\n" );
		}
		if ( pMobIndex->mobprogs )
		{
			MPROG_DATA* mPtr;
		    for ( mPtr = pMobIndex->mobprogs; mPtr; mPtr = mPtr->next )
		    {
			fprintf( fp, ">%s ",	mprog_type_to_name(
						mPtr->type ) );
			fprintf( fp, "%s~\n",	mPtr->arglist );
			fprintf( fp, "%s~\n",	fix_string( mPtr->comlist ) );
		    }
		    fprintf( fp, "|\n" );
		}
	    }
	}
    }
    fprintf( fp, "#0\n\n\n\n" );
    return;
}



/*****************************************************************************
 Name:		save_objects
 Purpose:	Save #OBJECTS section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_material( FILE *fp, AREA_DATA *pArea )
{
    OBJ_INDEX_DATA *pObjIndex;
    AFFECT_DATA *pAf;

    fprintf( fp, "#MATERIAL\n" );
    for( pObjIndex = material_list ; pObjIndex  ; pObjIndex = pObjIndex->next)
    {
	fprintf( fp, "#%d\n",	 pObjIndex->vnum );
	fprintf( fp, "%s~\n",	 pObjIndex->name );
	fprintf( fp, "%s~\n",	 fix_string( pObjIndex->short_descr ) );
	/*		  fprintf( fp, "%s~\n",	 fix_string( pObjIndex->cname ) );*/
	fprintf( fp, "%s~\n",	 fix_string( pObjIndex->description ) );
	fprintf( fp, "~\n" );
	fprintf( fp, "%d ",	 pObjIndex->item_type );
	fprintf( fp, "%d ",	 pObjIndex->extra_flags );
	fprintf( fp, "%d\n",	 pObjIndex->wear_flags);


	fprintf( fp, "%d~ %d~ %d~ %d~\n",
		pObjIndex->value[0],
		pObjIndex->value[1],
		pObjIndex->value[2],
		pObjIndex->value[3] );
	fprintf( fp, "%d ", pObjIndex->weight );
	fprintf( fp, "%d 0\n", pObjIndex->cost );


	for( pAf = pObjIndex->affected; pAf; pAf = pAf->next )
	{
	    if( pAf->location != 0 )
		fprintf( fp, "A\n%d %d\n",  pAf->location, pAf->modifier );
	    else
		fprintf( fp, "F %lld\n", pAf->bitvector);
	}
	if ( pObjIndex->hp )
	    fprintf( fp, "H %s~ %d %d %d\n", pObjIndex->hp->hp_name,
		    pObjIndex->hp->percent, pObjIndex->hp->level,
		    pObjIndex->hp->event );
	if ( pObjIndex->level > 1  )
	    fprintf( fp, "L %d\n", pObjIndex->level );
	if ( pObjIndex->class )
	    fprintf( fp, "P %d\n", pObjIndex->class );
	if ( pObjIndex->race )
	    fprintf( fp, "R %d\n", pObjIndex->race );
	if( pObjIndex->vrace ){
	    OBJ_VRACE *ovr;
	    for( ovr = pObjIndex->vrace; ovr; ovr = ovr->next ){
		fprintf(fp, "V %s~ %d %d %d %d\n",
			race_table[ovr->race].name,
			ovr->hr_mod_q,
			ovr->hr_mod_p,
			ovr->dr_mod_q,
			ovr->dr_mod_p);
	    }
	}
    }
    fprintf( fp, "#0\n\n\n\n" );
    return;
}
void save_objects( FILE *fp, AREA_DATA *pArea )
{
    int vnum;
    OBJ_INDEX_DATA *pObjIndex;
    AFFECT_DATA *pAf;
    EXTRA_DESCR_DATA *pEd;

    fprintf( fp, "#OBJECTS\n" );
    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pObjIndex = get_obj_index(vnum) ) )
	{
	    if ( pObjIndex->area == pArea )
	    {
		fprintf( fp, "#%d\n",	 pObjIndex->vnum );
		fprintf( fp, "%s~\n",	 pObjIndex->name );
		fprintf( fp, "%s~\n",	 fix_string( pObjIndex->short_descr ) );
/*		  fprintf( fp, "%s~\n",	 fix_string( pObjIndex->cname ) );*/
		fprintf( fp, "%s~\n",	 fix_string( pObjIndex->description ) );
		fprintf( fp, "~\n" );
		fprintf( fp, "%d ",	 pObjIndex->item_type );
		fprintf( fp, "%d ",	 pObjIndex->extra_flags );
		fprintf( fp, "%d\n",	 pObjIndex->wear_flags);

		switch ( pObjIndex->item_type )
		{
		default:
		fprintf( fp, "%d~ %d~ %d~ %d~\n",
			pObjIndex->value[0],
			pObjIndex->value[1],
			pObjIndex->value[2],
			pObjIndex->value[3] );
		    break;

		case ITEM_PILL:
		case ITEM_POTION:
		case ITEM_SCROLL:
		fprintf( fp, "%d~ %s~ %s~ %s~\n",
			pObjIndex->value[0],
			pObjIndex->value[1] != -1 ?
			      skill_table[pObjIndex->value[1]].name
			    : " ",
			pObjIndex->value[2] != -1 ?
			      skill_table[pObjIndex->value[2]].name
			    : " ",
			pObjIndex->value[3] != -1 ?
			      skill_table[pObjIndex->value[3]].name
			    : " " );
		    break;

		case ITEM_STAFF:
		case ITEM_WAND:
		fprintf( fp, "%d~ %d~ %d~ %s~\n",
			pObjIndex->value[0],
			pObjIndex->value[1],
			pObjIndex->value[2],
			pObjIndex->value[3] != -1 ?
			      skill_table[pObjIndex->value[3]].name
			    : " " );
		    break;
		}
		fprintf( fp, "%d ", pObjIndex->weight );
		fprintf( fp, "%d 0\n", pObjIndex->cost );

		for( pEd = pObjIndex->extra_descr; pEd; pEd = pEd->next )
		{
		fprintf( fp, "E\n%s~\n%s~\n", pEd->keyword,
					      fix_string( pEd->description ) );
		}

		for( pAf = pObjIndex->affected; pAf; pAf = pAf->next )
		{
		    if( pAf->location != 0 )
			fprintf( fp, "A\n%d %d\n",  pAf->location, pAf->modifier );
		    else
			fprintf( fp, "F %lld\n", pAf->bitvector);
		}
		if ( pObjIndex->hp )
			fprintf( fp, "H %s~ %d %d %d\n", pObjIndex->hp->hp_name,
				pObjIndex->hp->percent, pObjIndex->hp->level,
				pObjIndex->hp->event );
		if ( pObjIndex->level > 1  )
			fprintf( fp, "L %d\n", pObjIndex->level );
		if ( pObjIndex->timer )
			fprintf( fp, "T %d\n", pObjIndex->timer );
		if ( pObjIndex->cost )
			fprintf( fp, "C %d\n", pObjIndex->cost );
		if ( pObjIndex->class )
			fprintf( fp, "P %d\n", pObjIndex->class );
		if ( pObjIndex->race )
			fprintf( fp, "R %d\n", pObjIndex->race );
		if ( pObjIndex->on_message )
			fprintf( fp, "M 0 %s~\n", pObjIndex->on_message );
		if ( pObjIndex->off_message )
			fprintf( fp, "M 1 %s~\n", pObjIndex->off_message );
		if( pObjIndex->vrace ){
		    OBJ_VRACE *ovr;
		    for( ovr = pObjIndex->vrace; ovr; ovr = ovr->next ){
			fprintf(fp, "V %s~ %d %d %d %d\n",
				race_table[ovr->race].name,
			ovr->hr_mod_q,
			ovr->hr_mod_p,
			ovr->dr_mod_q,
			ovr->dr_mod_p);
		    }
		}
		if( str_cmp(pObjIndex->material->name, "Generic"))
			fprintf(fp, "X %s~\n", pObjIndex->material->name);
	    }
	}
    }
    fprintf( fp, "#0\n\n\n\n" );
    return;
}



/* OLC 1.1b */
/*****************************************************************************
 Name:		save_rooms
 Purpose:	Save #ROOMDATA section of an area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_rooms( FILE *fp, AREA_DATA *pArea )
{
    ROOM_INDEX_DATA *pRoomIndex;
    EXTRA_DESCR_DATA *pEd;
    EXIT_DATA *pExit;
    int vnum;
    int door;

    fprintf( fp, "#ROOMDATA\n" );
    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pRoomIndex = get_room_index(vnum) ) )
	{
	    if ( pRoomIndex->area == pArea )
	    {
		fprintf( fp, "#%d\n", pRoomIndex->vnum );
		fprintf( fp, "%s~\n", pRoomIndex->name );
		fprintf( fp, "%s~\n", fix_string( pRoomIndex->description ) );
		fprintf( fp, "0 " );
		fprintf( fp, "%d ", (pRoomIndex->room_flags^pRoomIndex->area->f_toggle) );
		fprintf( fp, "%d\n", pRoomIndex->sector_type );

		for ( pEd = pRoomIndex->extra_descr; pEd; pEd = pEd->next )
		{
		    fprintf( fp, "E\n%s~\n%s~\n", pEd->keyword,
			fix_string( pEd->description ) );
		}

		for( door = 0; door < MAX_DIR; door++ )
		{
		    if ( ( pExit = pRoomIndex->exit[door] ) )
		    {
			fprintf( fp, "D%d\n", door );
			fprintf( fp, "%s~\n",
			    fix_string( pExit->description ) );
			fprintf( fp, "%s~\n", pExit->keyword );
			fprintf( fp, "%d %d %d\n", pExit->rs_flags,
			    pExit->key,
			    pExit->to_room ? pExit->to_room->vnum : 0 );
		    }
		}
		if ( pRoomIndex->roomprogs )
		{
			RPROG_DATA* mPtr;

	    		for ( mPtr = pRoomIndex->roomprogs; mPtr; mPtr = mPtr->next )
	    		{
				fprintf( fp, ">%s ",	rprog_type_to_name(
					mPtr->type ) );
				fprintf( fp, "%s~\n",	mPtr->arglist );
				fprintf( fp, "%s~\n",	fix_string( mPtr->comlist ) );
	    		}
	    		fprintf( fp, "|\n" );
		}
	    fprintf( fp, "S\n" );
    	    }
	}
    }
    fprintf( fp, "#0\n\n\n\n" );
    return;
}


/* OLC 1.1b */
/*****************************************************************************
 Name:		save_specials
 Purpose:	Save #SPECIALS section of area file.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_specials( FILE *fp, AREA_DATA *pArea )
{
    int vnum;
    MOB_INDEX_DATA *pMobIndex;
    
    fprintf( fp, "#SPECIALS\n" );

    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pMobIndex = get_mob_index(vnum) ) )
	{
	    if ( pMobIndex->area == pArea && pMobIndex->spec_fun )
	    {
		fprintf( fp, "M %d %s\n", pMobIndex->vnum,
					  spec_string( pMobIndex->spec_fun ) );
	    }
	}
    }

    fprintf( fp, "S\n\n\n\n" );
    return;
}


/* OLC 1.1b */
/*****************************************************************************
 Name:		vsave_specials
 Purpose:	Save #SPECIALS section of area file.
		New formating thanks to Rac.
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void vsave_specials( FILE *fp, AREA_DATA *pArea )
{
    int vnum;
    MOB_INDEX_DATA *pMobIndex;
    
    fprintf( fp, "#SPECIALS\n" );

    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pMobIndex = get_mob_index(vnum) ) )
	{
	    if ( pMobIndex->area == pArea && pMobIndex->spec_fun )
	    {
		fprintf( fp, "M %d %s \t; %s\n", pMobIndex->vnum,
						 spec_string( pMobIndex->spec_fun ),
						 pMobIndex->short_descr );
	    }
	}
    }

    fprintf( fp, "S\n\n\n\n" );
    return;
}



/* OLC 1.1b */
/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
		New format thanks to Rac.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_resets( FILE *fp, AREA_DATA *pArea )
{
    RESET_DATA *pReset;
    MOB_INDEX_DATA *pLastMob = NULL;
    OBJ_INDEX_DATA *pLastObj;
    ROOM_INDEX_DATA *pRoomIndex;
    char buf[MAX_STRING_LENGTH];
    int vnum;

    fprintf( fp, "#RESETS\n" );

    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pRoomIndex = get_room_index(vnum) ) )
	{
	    if ( pRoomIndex->area == pArea )
	    {
    for ( pReset = pRoomIndex->reset_first; pReset; pReset = pReset->next )
    {
	switch ( pReset->command )
	{
	default:
	    bug( "Save_resets: bad command %c.", pReset->command );
	    break;

	case 'M':
	    pLastMob = get_mob_index( pReset->arg1 );
	    fprintf( fp, "M 0 %d %d %d\n", 
		pReset->arg1,
		pReset->arg2,
		pReset->arg3 );
	    break;

	case 'O':
	    pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "O 0 %d 0 %d\n", 
		pReset->arg1,
		pReset->arg3 );
	    break;

	case 'P':
	    pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "P 0 %d 0 %d\n", 
		pReset->arg1,
		pReset->arg3  );
	    break;

	case 'G':
	    fprintf( fp, "G 0 %d 0\n", pReset->arg1 );
	    if ( !pLastMob )
	    {
		sprintf( buf,
		    "Save_resets: !NO_MOB! in [%s]", pArea->filename );
		bug( buf, 0 );
	    }
	    break;

	case 'E':
	    fprintf( fp, "E 0 %d 0 %d\n",
		pReset->arg1,
		pReset->arg3 );
	    if ( !pLastMob )
	    {
		sprintf( buf,
		    "Save_resets: !NO_MOB! in [%s]", pArea->filename );
		bug( buf, 0 );
	    }
	    break;

	case 'F':
	    fprintf( fp, "F 1 %d %d %d\n", 
		pReset->arg1,
		pReset->arg2,
		pReset->arg3 );
	    break;

	case 'D':
	    fprintf( fp, "D 0 %d %d %d\n",
		    pReset->arg1,
		    pReset->arg2,
		    pReset->arg3);
	    break;

	case 'R':
	    fprintf( fp, "R 0 %d %d\n", 
		pReset->arg1,
		pReset->arg2 );
	    break;
	}	/* End switch */
    }	/* End for pReset */
	    }	/* End if correct area */
	}	/* End if pRoomIndex */
    }	/* End for vnum */
    fprintf( fp, "S\n\n\n\n" );
    return;
}



/* OLC 1.1b */
/*****************************************************************************
 Name:		save_resets
 Purpose:	Saves the #RESETS section of an area file.
		New format thanks to Rac.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void vsave_resets( FILE *fp, AREA_DATA *pArea )
{
    RESET_DATA *pReset;
    MOB_INDEX_DATA *pLastMob = NULL;
    OBJ_INDEX_DATA *pLastObj;
    ROOM_INDEX_DATA *pRoomIndex;
    char buf[MAX_STRING_LENGTH];
    int vnum;

    fprintf( fp, "#RESETS\n" );

    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pRoomIndex = get_room_index(vnum) ) )
	{
	    if ( pRoomIndex->area == pArea )
	    {
    for ( pReset = pRoomIndex->reset_first; pReset; pReset = pReset->next )
    {
	switch ( pReset->command )
	{
	default:
	    bug( "Save_resets: bad command %c.", pReset->command );
	    break;

	case 'M':
	    pLastMob = get_mob_index( pReset->arg1 );
	    fprintf( fp, "M 0 %d %2d %-5d \t; %s to %s\n", 
		pReset->arg1,
		pReset->arg2,
		pReset->arg3,
		pLastMob->short_descr,
		pRoomIndex->name );
	    break;

	case 'O':
	    pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "O 0 %d  0 %-5d \t; %s to %s\n", 
		pReset->arg1,
		pReset->arg3,
		capitalize(pLastObj->short_descr),
		pRoomIndex->name );
	    break;

	case 'P':
	    pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "P 0 %d  0 %-5d \t; %s inside %s\n", 
		pReset->arg1,
		pReset->arg3,
		capitalize(get_obj_index( pReset->arg1 )->short_descr),
		pLastObj ? pLastObj->short_descr : "!NO_OBJ!" );

	    if ( !pLastObj )	/* Thanks Rac! */
	    {
		sprintf( buf, "Save_resets: P with !NO_OBJ! in [%s]", pArea->filename );
		bug( buf, 0 );
	    }

	    break;

	case 'G':
	    pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( fp, "G 0 %d  0	 \t;   %s\n",
		pReset->arg1,
		capitalize(pLastObj->short_descr) );

	    if ( !pLastMob )
	    {
		sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->filename );
		bug( buf, 0 );
	    }
	    break;

	case 'E':
	    fprintf( fp, "E 0 %d  0 %-5d \t;   %s %s\n",
		pReset->arg1,
		pReset->arg3,
		capitalize(get_obj_index( pReset->arg1 )->short_descr),
		flag_string( wear_loc_strings, pReset->arg3 ) );
	    if ( !pLastMob )
	    {
		sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->filename );
		bug( buf, 0 );
	    }
	    break;

	case 'D':
	    break;

	case 'R':
	    fprintf( fp, "R 0 %d %2d	  \t; Randomize %s\n", 
		pReset->arg1,
		pReset->arg2,
		pRoomIndex->name );
	    break;
	}	/* End switch */
    }	/* End for pReset */
	    }	/* End if correct area */
	}	/* End if pRoomIndex */
    }	/* End for vnum */
    fprintf( fp, "S\n\n\n\n" );
    return;
}



/* OLC 1.1b */
/*****************************************************************************
 Name:		save_shops
 Purpose:	Saves the #SHOPS section of an area file.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void save_shops( FILE *fp, AREA_DATA *pArea )
{
    SHOP_DATA *pShopIndex;
    MOB_INDEX_DATA *pMobIndex;
    int iTrade;
    int vnum;
    
    fprintf( fp, "#SHOPS\n" );

    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pMobIndex = get_mob_index(vnum) ) )
	{
	    if ( pMobIndex->area == pArea && pMobIndex->pShop )
	    {
		pShopIndex = pMobIndex->pShop;

		fprintf( fp, "%d ", pShopIndex->keeper );
		for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
		{
		    if ( pShopIndex->buy_type[iTrade] != 0 )
		    {
		       fprintf( fp, "%d ", pShopIndex->buy_type[iTrade] );
		    }
		    else
		       fprintf( fp, "0 ");
		}
		fprintf( fp, "%d %d ", pShopIndex->profit_buy, pShopIndex->profit_sell );
		fprintf( fp, "%d %d\n", pShopIndex->open_hour, pShopIndex->close_hour );
	    }
	}
    }

    fprintf( fp, "0\n\n\n\n" );
    return;
}



/* OLC 1.1b */
/*****************************************************************************
 Name:		vsave_shops
 Purpose:	Saves the #SHOPS section of an area file.
		New formating thanks to Rac.
 Called by:	save_area(olc_save.c)
 ****************************************************************************/
void vsave_shops( FILE *fp, AREA_DATA *pArea )
{
    SHOP_DATA *pShopIndex;
    MOB_INDEX_DATA *pMobIndex;
    int iTrade;
    int vnum;
    
    fprintf( fp, "#SHOPS\n" );

    for( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
    {
	if( ( pMobIndex = get_mob_index(vnum) ) )
	{
	    if ( pMobIndex->area == pArea && pMobIndex->pShop )
	    {
		pShopIndex = pMobIndex->pShop;

		fprintf( fp, "%d ", pShopIndex->keeper );
		for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
		{
		    if ( pShopIndex->buy_type[iTrade] != 0 )
		    {
		       fprintf( fp, "%2d ", pShopIndex->buy_type[iTrade] );
		    }
		    else
		       fprintf( fp, " 0 ");
		}
		fprintf( fp, "%3d %3d ", pShopIndex->profit_buy, pShopIndex->profit_sell );
		fprintf( fp, "%2d %2d ", pShopIndex->open_hour, pShopIndex->close_hour );
		fprintf( fp, "\t; %s\n", get_mob_index( pShopIndex->keeper )->short_descr );
	    }
	}
    }

    fprintf( fp, "0\n\n\n\n" );
    return;
}



/*****************************************************************************
 Name:		save_helps
 Purpose:	Save #HELPS section of an area file.
 Written by:	Walker <nkrendel@evans.Denver.Colorado.EDU>
 Called by:	save_area(olc_save.c).
 ****************************************************************************/
void save_helps( FILE *fp, AREA_DATA *pArea )
{
    HELP_DATA *pHelp;
    bool found = FALSE;

    for( pHelp = help_first; pHelp; pHelp = pHelp->next )
    {
	if( pHelp->area && pHelp->area == pArea )
	{
	    if( !found )
	    {
		fprintf( fp, "#HELPS\n\n" );
		found = TRUE;
	    }
	    fprintf( fp, "%d %s~\n%s~\n", 
		pHelp->level,
		all_capitalize( pHelp->keyword ), 
		fix_string( pHelp->text ) );
	}
    }

    if( found )
	fprintf( fp, "\n0 $~\n\n" );

    return;
}

void save_limbo( AREA_DATA *pArea )
{
    FILE *fp;

    fclose( fpReserve );
    if ( !( fp = fopen( pArea->filename, "w" ) ) )
    {
	bug( "Open_area: fopen", 0 );
	perror( pArea->filename );
    }



    fprintf( fp, "#AREADATA\n" );
    fprintf( fp, "Name	      %s~\n",	     pArea->name );
    fprintf( fp, "Identity    %s~\n",	     pArea->identity ? pArea->identity : "None" );
    fprintf( fp, "Builders    %s~\n",	     fix_string( pArea->builders ? pArea->builders : "None" ) );
    fprintf( fp, "Translator  %s~\n",	     fix_string( pArea->translator ? pArea->translator : "None" ) );
    fprintf( fp, "Modifier    %s~\n",	     fix_string( pArea->modifier ? pArea->modifier : "None" ) );
    fprintf( fp, "VNUMs	      %d %d\n",	     pArea->lvnum, pArea->uvnum );
    fprintf( fp, "Security    %d\n",	     pArea->security );
    fprintf( fp, "Recall      %d\n",	     pArea->recall );
    fprintf( fp, "F_toggle    %d\n",	     pArea->f_toggle );
    fprintf( fp, "Death	    %d\n",	    pArea->death);
    fprintf( fp, "End\n\n\n\n" );
	save_helps( fp, pArea );				/* OLC 1.1b */
	save_material(fp, pArea);
	save_mobiles( fp, pArea );
	save_objects( fp, pArea );
	save_rooms( fp, pArea );
	if ( IS_SET(pArea->area_flags, AREA_VERBOSE) )	/* OLC 1.1b */
	{
	    vsave_specials( fp, pArea );
	    vsave_resets( fp, pArea );
	    vsave_shops( fp, pArea );
	}
	else
	{
	    save_specials( fp, pArea );
	    save_resets( fp, pArea );
	    save_shops( fp, pArea );
	}

	fprintf( fp, "#$\n" );
    fclose( fp );

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


/*****************************************************************************
 Name:		save_area
 Purpose:	Save an area, note that this format is new.
 Called by:	do_asave(olc_save.c).
 ****************************************************************************/
void save_area( AREA_DATA *pArea )
{
    FILE *fp;

    if( !str_cmp("limbo", pArea->identity )){
	save_limbo( pArea );
	return;
    }
    fclose( fpReserve );
    if ( !( fp = fopen( pArea->filename, "w" ) ) )
    {
	bug( "Open_area: fopen", 0 );
	perror( pArea->filename );
    }



    fprintf( fp, "#AREADATA\n" );
    fprintf( fp, "Name	      %s~\n",	     pArea->name );
    fprintf( fp, "Identity    %s~\n",	     pArea->identity ? pArea->identity : "None" );
    fprintf( fp, "Builders    %s~\n",	     fix_string( pArea->builders ? pArea->builders : "None" ) );
    fprintf( fp, "Translator  %s~\n",	     fix_string( pArea->translator ? pArea->translator : "None" ) );
    fprintf( fp, "Modifier    %s~\n",	     fix_string( pArea->modifier ? pArea->modifier : "None" ) );
    fprintf( fp, "VNUMs	      %d %d\n",	     pArea->lvnum, pArea->uvnum );
    fprintf( fp, "Security    %d\n",	     pArea->security );
    fprintf( fp, "Recall      %d\n",	     pArea->recall );
    fprintf( fp, "F_toggle    %d\n",	     pArea->f_toggle );
    fprintf( fp, "End\n\n\n\n" );
#if 0
    if( !str_cmp(pArea->identity, "limbo")){
	bug("Saving limbo.are",0);
	save_helps( fp, pArea );				/* OLC 1.1b */
	save_material(fp, pArea);
	save_mobiles( fp, pArea );
	save_objects( fp, pArea );
	save_rooms( fp, pArea );
	if ( IS_SET(pArea->area_flags, AREA_VERBOSE) )	/* OLC 1.1b */
	{
	    vsave_specials( fp, pArea );
	    vsave_resets( fp, pArea );
	    vsave_shops( fp, pArea );
	}
	else
	{
	    save_specials( fp, pArea );
	    save_resets( fp, pArea );
	    save_shops( fp, pArea );
	}

	fprintf( fp, "#$\n" );
    fclose( fp );

    fpReserve = fopen( NULL_FILE, "r" );
    return;
    }
#endif

    save_helps( fp, pArea );				/* OLC 1.1b */
    save_mobiles( fp, pArea );
    save_objects( fp, pArea );
    save_rooms( fp, pArea );
    if ( IS_SET(pArea->area_flags, AREA_VERBOSE) )	/* OLC 1.1b */
    {
	vsave_specials( fp, pArea );
	vsave_resets( fp, pArea );
	vsave_shops( fp, pArea );
    }
    else
    {
	save_specials( fp, pArea );
	save_resets( fp, pArea );
	save_shops( fp, pArea );
    }

    fprintf( fp, "#$\n" );
    fclose( fp );

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


/* OLC 1.1b */
/*****************************************************************************
 Name:		do_asave
 Purpose:	Entry point for saving area data.
 Called by:	interpreter(interp.c)
 ****************************************************************************/
void do_asave( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    AREA_DATA *pArea;
    int value;

    if ( !ch )	     /* Do an autosave */
    {
	send_to_all_char( "一股和風自東方吹來。\n\r" );
	save_area_list();
	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    if ( !str_cmp(pArea->identity , "school" ))
	    	continue;
	    if( !str_cmp(pArea->identity, "limbo"))
		continue;

	    if( !IS_SET(pArea->area_flags, AREA_CHANGED)
	    &&  !IS_SET(pArea->area_flags, AREA_ADDED)) continue;
	    save_area( pArea );
	    REMOVE_BIT( pArea->area_flags, AREA_CHANGED | AREA_ADDED );
	}
	send_to_all_char( "風過之後一切又回復平靜。\n\r" );
	return;
    }

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
    send_to_char( "Syntax:\n\r", ch );
    send_to_char( "  asave <vnum>    - saves a particular area\n\r",	 ch );
    send_to_char( "  asave list	     - saves the AREA.LST file\n\r",	 ch );
    send_to_char( "  asave area	     - saves the area being edited\n\r", ch );
    send_to_char( "  asave changed   - saves all changed zones\n\r",	 ch );
    send_to_char( "  asave world     - saves the world! (db dump)\n\r",	 ch );
    send_to_char( "  asave ^ verbose - saves in verbose mode\n\r", ch );
    send_to_char( "\n\r", ch );
	return;
    }

    /* Snarf the value (which need not be numeric). */
    value = atoi( arg1 );

    /* Save the area of given vnum. */
    /* ---------------------------- */

    if ( !( pArea = get_area_data( value ) ) && is_number( arg1 ) )
    {
	send_to_char( "That area does not exist.\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	if ( !IS_BUILDER( ch, pArea ) && ch->level < MAX_LEVEL )
	{
	    send_to_char( "You are not a builder for this area.\n\r", ch );
	    return;
	}

	save_area_list();
	if ( !str_cmp( "verbose", argument ) )
	    SET_BIT( pArea->area_flags, AREA_VERBOSE );
	save_area( pArea );
	REMOVE_BIT( pArea->area_flags, AREA_VERBOSE );
	return;
    }

    /* Save the world, only authorized areas. */
    /* -------------------------------------- */

    if ( !str_cmp( "world", arg1 ) )
    {
	send_to_char("asave world is disabled by now.\n\r", ch);
	return;
	save_area_list();
	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    /* Builder must be assigned this area. */
	    if ( !IS_BUILDER( ch, pArea ) )
		continue;	  

	    if ( !str_cmp( "verbose", argument ) )
		SET_BIT( pArea->area_flags, AREA_VERBOSE );
	    save_area( pArea );
	    REMOVE_BIT( pArea->area_flags, AREA_CHANGED | AREA_ADDED | AREA_VERBOSE );
	}
	send_to_char( "You saved the world.\n\r", ch );
	send_to_all_char( "Database saved.\n\r" );
	return;
    }

    /* Save changed areas, only authorized areas. */
    /* ------------------------------------------ */

    if ( !str_cmp( "changed", arg1 ) )
    {
	char buf[MAX_INPUT_LENGTH];

	save_area_list();

	send_to_char( "Saved zones:\n\r", ch );
	sprintf( buf, "None.\n\r" );

	for( pArea = area_first; pArea; pArea = pArea->next )
	{
	    /* Builder must be assigned this area. */
	    if ( !IS_BUILDER( ch, pArea ) )
		continue;

	    /* Save changed areas. */
	    if ( IS_SET(pArea->area_flags, AREA_CHANGED)
	      || IS_SET(pArea->area_flags, AREA_ADDED) )
	    {
		if ( !str_cmp( "verbose", argument ) )
		    SET_BIT( pArea->area_flags, AREA_VERBOSE );
		save_area( pArea );
		REMOVE_BIT( pArea->area_flags, AREA_CHANGED | AREA_ADDED | AREA_VERBOSE );
		sprintf( buf, "%24s - '%s'\n\r", pArea->name, pArea->filename );
		send_to_char( buf, ch );
	    }
	}
	if ( !str_cmp( buf, "None.\n\r" ) )
	    send_to_char( buf, ch );
	return;
    }

    /* Save the AREA.LST file. */
    /* ----------------------- */
    if ( !str_cmp( arg1, "list" ) )
    {
	save_area_list();
	return;
    }

    /* Save area being edited, if authorized. */
    /* -------------------------------------- */
    if ( !str_cmp( arg1, "area" ) )
    {	
	/* Find the area to save. */
	switch (ch->desc->editor)
	{
	    case ED_AREA:
		pArea = (AREA_DATA *)ch->desc->pEdit;
		break;
	    case ED_ROOM:
		pArea = ch->in_room->area;
		break;
	    case ED_OBJECT:
		pArea = ( (OBJ_INDEX_DATA *)ch->desc->pEdit )->area;
		break;
	    case ED_MOBILE:
		pArea = ( (MOB_INDEX_DATA *)ch->desc->pEdit )->area;
		break;
	    default:
		pArea = ch->in_room->area;
		break;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "You are not a builder for this area.\n\r", ch );
	    return;
	}

	save_area_list();
	if ( !str_cmp( "verbose", argument ) )
	    SET_BIT( pArea->area_flags, AREA_VERBOSE );
	save_area( pArea );
	REMOVE_BIT( pArea->area_flags, AREA_CHANGED | AREA_ADDED | AREA_VERBOSE );
	send_to_char( "Area saved.\n\r", ch );
	return;
    }

    /* Show correct syntax. */
    /* -------------------- */
    do_asave( ch, "" );
    return;
}



bool aedit_save( CHAR_DATA *ch, char * arg)
{
    AREA_DATA *pArea;

    EDIT_AREA(ch, pArea);
    SET_BIT(pArea->area_flags, AREA_VERBOSE);
    save_area(pArea);
    REMOVE_BIT( pArea->area_flags, AREA_CHANGED | AREA_ADDED | AREA_VERBOSE );
    send_to_char("區域存檔完畢。\n\r", ch);
    return FALSE;
}
