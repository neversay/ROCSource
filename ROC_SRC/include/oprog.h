#ifndef __OBJ_PROG_H__
#define __OBJ_PROG_H__
extern int mprog_do_ifchck			args( ( char* ifchck, CHAR_DATA* mob,
											CHAR_DATA* actor, OBJ_DATA* obj, 
											void* vo,  CHAR_DATA* rndm, 
											CHAR_DATA* remb ) );
extern char *mprog_next_command		args( ( char* clist ) );
extern bool is_chinese_char(unsigned char c, bool flag);

/* obj program process, Keric 2005/2/16 */
char *	oprog_process_if	args( ( char* ifchck, char* com_list, 
				       OBJ_DATA *room, CHAR_DATA* actor,
                                       void* vo,
				       CHAR_DATA* rndm ) );
char * 	oextract_if_endif	args( ( char *com_list, OBJ_DATA *obj) );
void	oprog_process_cmnd	args( ( char* cmnd, OBJ_DATA *room, 
				       CHAR_DATA* actor, void* vo,CHAR_DATA* rndm ) );
void	oprog_translate		args( ( char ch, char* t, OBJ_DATA *room,
				       CHAR_DATA* actor, CHAR_DATA* rndm ) );


#endif
