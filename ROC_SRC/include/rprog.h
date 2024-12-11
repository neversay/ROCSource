#ifndef __ROOM_PROG_H__
#define __ROOM_PROG_H__
extern int mprog_do_ifchck			args( ( char* ifchck, CHAR_DATA* mob,
											CHAR_DATA* actor, OBJ_DATA* obj, 
											void* vo,  CHAR_DATA* rndm, 
											CHAR_DATA* remb ) );
extern char *mprog_next_command		args( ( char* clist ) );
extern bool is_chinese_char(unsigned char c , bool flag);

/* Room prog process, Keric 2005/2/16  */
char *	rprog_process_if	args( ( char* ifchck, char* com_list, 
				       ROOM_INDEX_DATA *room, CHAR_DATA* actor,
				       CHAR_DATA* rndm ) );
void	rprog_translate		args( ( char ch, char* t, ROOM_INDEX_DATA *room,
				       CHAR_DATA* actor, CHAR_DATA* rndm ) );
void	rprog_process_cmnd	args( ( char* cmnd, ROOM_INDEX_DATA *room, 
				       CHAR_DATA* actor, CHAR_DATA* rndm ) );
void	rprog_driver		args( ( char* com_list, ROOM_INDEX_DATA *room,
				       CHAR_DATA* actor ) );
char * 	rextract_if_endif	args( ( char *com_list, ROOM_INDEX_DATA *room ) );


#endif
