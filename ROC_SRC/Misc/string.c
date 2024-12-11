/***************************************************************************
 *  File: string.c							   *
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
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
	bool fChinese;
	fChinese=FALSE;
	for ( ; *str != '\0'; str++ )
	{
		if ( *str == '~' && !fChinese )
			*str = '-';
		fChinese = is_chinese_char(*str, fChinese);
		//if ( fChinese )
		//    fChinese = FALSE;
		//else if ( *str >= '\x80' && *str <= '\xff' )
		//    fChinese = TRUE;
	}

	return;
}



/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{
	bool fChinese;
	fChinese=FALSE;
	if ( !astr )
	{
		bug( "Str_cmp: null astr.", 0 );
		return TRUE;
	}

	if ( !bstr )
	{
		bug( "Str_cmp: null bstr.", 0 );
		return TRUE;
	}

	for ( ; *astr || *bstr; astr++, bstr++ )
	{
		if ( fChinese ) {
			if ( *astr != *bstr )
				return TRUE;
			fChinese=FALSE;
		}
		else 
		{
			if ( LOWER( *astr ) != LOWER( *bstr ) )
				return TRUE;
			fChinese = is_chinese_char( *astr, fChinese);
			//if ( *astr >= '\x80' && *astr <= '\xff' )
			//	fChinese = TRUE;
		}
	}

	return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    bool fChinese;
    fChinese=FALSE;
	int i = 0;
	
    if ( !astr )
    {
	bug( "Str_prefix: null astr.", 0 );
	return TRUE;
    }

    if ( !bstr )
    {
	bug( "Str_prefix: null bstr.", 0 );
	return FALSE;
    }

    for ( ; *astr; astr++, bstr++, i++ )
    {
		if( i > 127 ) break;
	if ( fChinese ) {
	    if ( *astr != *bstr )
		return TRUE;
	    fChinese=FALSE;
	}
	else {
	    if ( LOWER( *astr ) != LOWER( *bstr ) )
		return TRUE;
		fChinese = is_chinese_char( *astr, fChinese);
	    //if ( *astr >= '\x80' && *astr <= '\xff' )
		//fChinese=TRUE;
	}
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    char c0;
    int	 sstr1;
    int	 sstr2;
    int	 ichar;

    if ( ( c0 = LOWER( astr[0] ) ) == '\0' )
	return FALSE;

    sstr1 = strlen( astr );
    sstr2 = strlen( bstr );

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
	if ( c0 == LOWER( bstr[ichar] ) && !str_prefix( astr, bstr + ichar ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen( astr );
    sstr2 = strlen( bstr );
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
	return FALSE;
    else
	return TRUE;
}



/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
	static char strcap [ MAX_STRING_LENGTH ];
	int	i;
	bool fChinese;
	fChinese=FALSE;

	for ( i = 0; str[i] != '\0'; i++ ) {
		if ( fChinese ) {
			strcap[i] = str[i];
			fChinese=FALSE;
		}
		else {
			strcap[i] = LOWER( str[i] );
			fChinese = is_chinese_char(str[i], fChinese);
			//if ( str[i] >= '\x80' && str[i] <= '\xff' )
			//fChinese=TRUE;
		}
	}
	strcap[i] = '\0';
	strcap[0] = UPPER( strcap[0] );
	return strcap;
}


/*****************************************************************************
 Name:		string_append
 Purpose:	Clears string and puts player into editing mode.
 Called by:	none
 ****************************************************************************/
void string_edit( CHAR_DATA *ch, char **pString )
{
    send_to_char( "-========-   字串編輯程式    -=========-\n\r", ch );
    send_to_char( "    在空白行輸入 .h 可獲得說明訊息\n\r", ch );
    send_to_char( "       在空白行輸入 @ 可結束編輯.\n\r", ch );
    send_to_char( "-=======================================-\n\r", ch );

    if ( *pString == NULL )
    {
	*pString = str_dup( "" );
    }
    else
    {
	**pString = '\0';
    }

    ch->desc->pString = pString;

    return;
}



/*****************************************************************************
 Name:		string_append
 Purpose:	Puts player into append mode for given string.
 Called by:	(many)olc_act.c
 ****************************************************************************/
void string_append( CHAR_DATA *ch, char **pString )
{
    send_to_char( "-=======- 字串編輯程式 - 附加 -========-\n\r", ch );
    send_to_char( "    在空白行輸入 .h 可獲得說明訊息\n\r", ch );
    send_to_char( "       在空白行輸入 @ 可結束編輯.\n\r", ch );
    send_to_char( "-=======================================-\n\r", ch );

    if ( *pString == NULL )
    {
	*pString = str_dup( "" );
    }
    send_to_char( *pString, ch );
    
    if ( *(*pString + strlen( *pString ) - 1) != '\r' )
    send_to_char( "\n\r", ch );

    ch->desc->pString = pString;

    return;
}



/*****************************************************************************
 Name:		string_replace
 Purpose:	Substitutes one string for another.
 Called by:	string_add(string.c) (aedit_builder)olc_act.c.
 ****************************************************************************/
char * string_replace( char * orig, char * old, char * new )
{
    char xbuf[MAX_STRING_LENGTH];
    int i;

    xbuf[0] = '\0';
    strcpy( xbuf, orig );
    if ( strstr( orig, old ) != NULL )
    {
	i = strlen( orig ) - strlen( strstr( orig, old ) );
	xbuf[i] = '\0';
	strcat( xbuf, new );
	strcat( xbuf, &orig[i+strlen( old )] );
	free_string( orig );
    }

    return str_dup( xbuf );
}


/* OLC 1.1b */
/*****************************************************************************
 Name:		string_add
 Purpose:	Interpreter for string editing.
 Called by:	game_loop_xxxx(comm.c).
 ****************************************************************************/
void string_add( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    /*
     * Thanks to James Seng
     */
    smash_tilde( argument );

    if ( *argument == '.' )
    {
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];
	char arg3 [MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg1 );
	argument = first_arg( argument, arg2, FALSE );
	argument = first_arg( argument, arg3, FALSE );

	if ( !str_cmp( arg1, ".c" ) )
	{
	    send_to_char( "字串清除完畢.\n\r", ch );
	    **ch->desc->pString = '\0';
	    return;
	}

	if ( !str_cmp( arg1, ".s" ) )
	{
	    send_to_char( "目前字串內容:\n\r", ch );
	    send_to_char( *ch->desc->pString, ch );
	    return;
	}

	if ( !str_cmp( arg1, ".r" ) )
	{
	    if ( arg2[0] == '\0' )
	    {
		send_to_char(
		    "格式:  .r \"舊字串\" \"新字串\"\n\r", ch );
		return;
	    }

	    *ch->desc->pString =
		string_replace( *ch->desc->pString, arg2, arg3 );
	    sprintf( buf, "'%s' 取代為 '%s'.\n\r", arg2, arg3 );
	    send_to_char( buf, ch );
	    return;
	}

	if ( !str_cmp( arg1, ".f" ) )
	{
	    *ch->desc->pString = format_string( *ch->desc->pString );
	    send_to_char( "字串格式化完成.\n\r", ch );
	    return;
	}
	
	if ( !str_cmp( arg1, ".h" ) )
	{
	    send_to_char( "說明 (在空白行使用):	  \n\r", ch );
	    send_to_char( ".r 'old' 'new'   - 字串替換\n\r", ch );
	    send_to_char( "		      (必須使用 '', \"\") \n\r", ch );
	    send_to_char( ".h		    - 列出說明\n\r", ch );
	    send_to_char( ".s		    - 列出目前字串\n\r", ch );
	    send_to_char( ".f		    - 字串重新格式化\n\r", ch );
	    send_to_char( ".c		    - 清除字串\n\r", ch );
	    send_to_char( "@		    - 結束編輯\n\r", ch );
	    return;
	}
	    

	send_to_char( "SEdit:  無此指令.\n\r", ch );
	return;
    }

    if ( *argument == '@' )
    {
	ch->desc->pString = NULL;
	return;
    }

    /*
     * Truncate strings to MAX_STRING_LENGTH.
     * --------------------------------------
     */
    if ( strlen( buf ) + strlen( argument ) >= ( MAX_STRING_LENGTH - 4 ) )
    {
	send_to_char( "字串過長.\n\r", ch );

	/* Force character out of editing mode. */
	ch->desc->pString = NULL;
	return;
    }

    strcpy( buf, *ch->desc->pString );
    strcat( buf, argument );
    strcat( buf, "\n\r" );
    free_string( *ch->desc->pString );
    *ch->desc->pString = str_dup( buf );
    return;
}



/*
 *  Thanks to Kalgen for the new procedure (no more bug!)
 *  Original wordwrap() written by Surreality.
 */
/*****************************************************************************
 Name:		format_string
 Purpose:	Special string formating and word-wrapping.
 Called by:	string_add(string.c) (many)olc_act.c
 ****************************************************************************/
char *format_string( char *oldstring /*, bool fSpace */)
{
  char xbuf[MAX_STRING_LENGTH];
  char xbuf2[MAX_STRING_LENGTH];
  char *rdesc;
  int i=0;
  bool cap=TRUE;
  bool fChinese;
  fChinese=FALSE;
  
  xbuf[0]=xbuf2[0]=0;
  
  i=0;
  
  if ( strlen(oldstring) >= (MAX_STRING_LENGTH - 4) )	/* OLC 1.1b */
  {
     bug( "String to format_string() longer than MAX_STRING_LENGTH.", 0 );
     return (oldstring);
  }

  for (rdesc = oldstring; *rdesc; rdesc++)
  {
    if (*rdesc=='\n')
    {
      if (xbuf[i-1] != ' ')
      {
	xbuf[i]=' ';
	i++;
      }
    }
    else if (*rdesc=='\r') ;
    else if (*rdesc==' ')
    {
      if (xbuf[i-1] != ' ')
      {
	xbuf[i]=' ';
	i++;
      }
    }
    else if (*rdesc==')')
    {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
	  (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!'))
      {
	xbuf[i-2]=*rdesc;
	xbuf[i-1]=' ';
	xbuf[i]=' ';
	i++;
      }
      else
      {
	xbuf[i]=*rdesc;
	i++;
      }
    }
    else if (*rdesc=='.' || *rdesc=='?' || *rdesc=='!') {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
	  (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!')) {
	xbuf[i-2]=*rdesc;
	if (*(rdesc+1) != '\"')
	{
	  xbuf[i-1]=' ';
	  xbuf[i]=' ';
	  i++;
	}
	else
	{
	  xbuf[i-1]='\"';
	  xbuf[i]=' ';
	  xbuf[i+1]=' ';
	  i+=2;
	  rdesc++;
	}
      }
      else
      {
	xbuf[i]=*rdesc;
	if (*(rdesc+1) != '\"')
	{
	  xbuf[i+1]=' ';
	  xbuf[i+2]=' ';
	  i += 3;
	}
	else
	{
	  xbuf[i+1]='\"';
	  xbuf[i+2]=' ';
	  xbuf[i+3]=' ';
	  i += 4;
	  rdesc++;
	}
      }
      cap = TRUE;
    }
    else
    {
      xbuf[i]=*rdesc;
      if ( cap )
	{
	  cap = FALSE;
	  if ( !fChinese )
	    xbuf[i] = UPPER( xbuf[i] );
	}
      i++;
    }
    //if ( fChinese )
    //  fChinese=FALSE;
    //else
	fChinese = is_chinese_char( *rdesc, fChinese );
		/*if ( *rdesc >= '\x80' && *rdesc <= '\xff' )
      fChinese=TRUE;*/
  }
  xbuf[i]=0;
  strcpy(xbuf2,xbuf);
  
  rdesc=xbuf2;
  
  xbuf[0]=0;
  
  for ( ; ; )
  {
    for (i=0; i<77; i++)
    {
      if (!*(rdesc+i)) break;
    }
    if (i<77)
    {
      break;
    }
    for (i=(xbuf[0]?76:73) ; i ; i--)
    {
      if (*(rdesc+i)==' ') break;
    }
    if (i)
    {
      *(rdesc+i)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"\n\r");
      rdesc += i+1;
      while (*rdesc == ' ') rdesc++;
    }
    else
    {
      bug ("No spaces", 0);
      *(rdesc+75)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"-\n\r");
      rdesc += 76;
    }
  }
  while (*(rdesc+i) && (*(rdesc+i)==' '||
			*(rdesc+i)=='\n'||
			*(rdesc+i)=='\r'))
    i--;
  *(rdesc+i+1)=0;
  strcat(xbuf,rdesc);
  if (xbuf[strlen(xbuf)-2] != '\n')
    strcat(xbuf,"\n\r");

  free_string(oldstring);
  return(str_dup(xbuf));
}



/*
 * Used above in string_add.  Because this function does not
 * modify case if fCase is FALSE and because it understands
 * parenthesis, it would probably make a nice replacement
 * for one_argument.
 */
/*****************************************************************************
 Name:		first_arg
 Purpose:	Pick off one argument from a string and return the rest.
		Understands quates, parenthesis (barring ) ('s) and
		percentages.
 Called by:	string_add(string.c)
 ****************************************************************************/
char *first_arg( char *argument, char *arg_first, bool fCase )
{
    char cEnd;
    bool fChinese;
    fChinese=FALSE;

    while ( *argument == ' ' )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"'
      || *argument == '%'  || *argument == '(' )
    {
	if ( *argument == '(' )
	{
	    cEnd = ')';
	    argument++;
	}
	else cEnd = *argument++;
    }

    while ( *argument != '\0' )
    {
		if ( *argument == cEnd )
		{
			argument++;
			break;
		}
		if ( fCase ) {
			if ( fChinese ) {
				*arg_first = *argument;
				fChinese=FALSE;
			}
			else 
			{
				*arg_first = LOWER(*argument);
				//if ( *argument >= '\x80' && *argument<= '\xff' )
				//    fChinese=TRUE;
				fChinese = is_chinese_char(*argument, fChinese);
			}
		}
		else 
			*arg_first = *argument;
		arg_first++;
		argument++;
	}
    *arg_first = '\0';

	while ( *argument == ' ' )
		argument++;

    return argument;
}




/*
 * Used in olc_act.c for aedit_builders.
 */
char * string_unpad( char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char *s;

    s = argument;

    while ( *s == ' ' )
	s++;

    strcpy( buf, s );
    s = buf;

    if ( *s != '\0' )
    {
	while ( *s != '\0' )
	    s++;
	s--;

	while( *s == ' ' )
	    s--;
	s++;
	*s = '\0';
    }

    free_string( argument );
    return str_dup( buf );
}



/*
 * Same as capitalize but changes the pointer's data.
 * Used in olc_act.c in aedit_builder.
 */
char * string_proper( char * argument )
{
    char *s;

    s = argument;

    while ( *s != '\0' )
    {
	if ( *s != ' ' )
	{
	*s = UPPER(*s);
	    while ( *s != ' ' && *s != '\0' )
		s++;
	}
	else
	{
	    s++;
	}
    }

    return argument;
}


/*
 * Returns an all-caps string.		OLC 1.1b
 */
char* all_capitalize( const char *str )
{
    static char strcap [ MAX_STRING_LENGTH ];
	   int	i;
	   bool fChinese;
	   fChinese=FALSE;
    for ( i = 0; str[i] != '\0'; i++ ) {
	if ( fChinese ) {
	    strcap[i] = str[i];
	    fChinese=FALSE;
	}
	else {
	    strcap[i] = UPPER( str[i] );
	    fChinese = is_chinese_char( str[i], fChinese);
		//if ( str[i] >= '\x80' && str[i] <= '\xff' )
		//fChinese=TRUE;
	}
    }
    strcap[i] = '\0';
    return strcap;
}


/* By Chris at 05/09/17
 * Compare hosts/IPs of strings.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool ip_str_cmp( const char *astr, const char *bstr )
{
	int count=0;
	
	if ( !astr )
	{
		bug( "IP_str_cmp: null astr.", 0 );
		return TRUE;
	}

	if ( !bstr )
	{
		bug( "IP_str_cmp: null bstr.", 0 );
		return TRUE;
	}

	for ( ; *astr || *bstr; astr++, bstr++ )
	{
	    if(    ( *astr == '.' || *astr == '-' || *astr == ' ')
	        && ( *bstr == '.' || *bstr == '-' || *bstr == ' ') )
	    {
	        count++;
	        if(count < 4)  continue;
	        else           break;
	    }

	    if ( *astr  !=  *bstr  )
                return TRUE;
	}

	return FALSE;
}

