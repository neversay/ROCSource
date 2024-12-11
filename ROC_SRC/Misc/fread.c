#include <stdio.h>
#include <ctype.h>
#include "merc.h"

char fread_letter( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace( c ) );

    return c;
}


char *fread_alpha_word( FILE *fp, int *status )
{
    static char	 word [ MAX_INPUT_LENGTH ];
	   char *pword;
	   char	 cEnd;

    *status = 0;

    do
    {
	cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"'  )
    {
	pword	= word;
    }
    else
    {
	word[0] = cEnd;
	pword	= word+1;
	cEnd	= ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	*pword = getc( fp );
	if ( cEnd == ' ' ? (isspace( *pword )|| *pword == '|') :( *pword == cEnd ) )
	{
	    if ( cEnd == ' '|| cEnd == '|' )
		ungetc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }

    bugf( "Fread_word: word too long[%s].", word );
	*status = 1;
    return NULL;
}


long long int fread_number( FILE *fp, int *status )
{
	char c;
	bool sign;
	long long int	 number;
	int	 stat;
	char * tmp;

	do
	{
		c = getc( fp );
	}
	while ( isspace( c ) );

	number = 0;

	sign   = FALSE;
	if ( c == '+' || c == '|')
	{
		c = getc( fp );
	}
	else if ( c == '-' )
	{
		sign = TRUE;
		c = getc( fp );
	}

	if ( !isdigit( c ) )
	{
		ungetc(c, fp);
		if( !isalpha(c ) ) {
			*status = 1;
			bug( "Fread_number: bad format 1.", 0 );
			bug( "	 If bad object, check for missing '~' in value[] fields.", 0 );
			return 0;
		}
		//tmp = fread_string(fp, &stat);
		tmp = fread_alpha_word(fp, &stat);
		number = symbol_lookup(tmp);

		if (number == -1){

			*status = 1;
			bugf( "Fread_number: bad format 2.[%s]", tmp );
			bug( "	 If bad object, check for missing '~' in value[] fields.", 0 );
			return 0;
		}

		if ( sign )
			number = 0 - number;

		c = getc(fp);
		if ( c == '|' )
			number += fread_number( fp, &stat );
		else 
			ungetc( c, fp );
		return number;
	}

	while ( isdigit(c) )
	{
		number = number * 10 + c - '0';
		c      = getc( fp );
	}

	if ( sign )
		number = 0 - number;

	if ( c == '|' )
		number += fread_number( fp, &stat );
	else if ( c != ' ' )
		ungetc( c, fp );

	return number;
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( c != '\n' && c != '\r' && c != EOF );

    do
    {
	c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetc( c, fp );
    return;
}



/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp, int *status )
{
    static char	 word [ MAX_INPUT_LENGTH ];
	   char *pword;
	   char	 cEnd;

    *status = 0;

    do
    {
	cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
	pword	= word;
    }
    else
    {
	word[0] = cEnd;
	pword	= word+1;
	cEnd	= ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	*pword = getc( fp );
	if ( cEnd == ' ' ? isspace( *pword ) : *pword == cEnd )
	{
	    if ( cEnd == ' ' )
		ungetc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }

    bug( "Fread_word: word too long.", 0 );
	*status = 1;
    return NULL;
}


