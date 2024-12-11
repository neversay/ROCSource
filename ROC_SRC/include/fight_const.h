#ifndef __FIGHT_CONST_H__
#define __FIGHT_CONST_H__
/* English Attack type table */
static const char * attack_table[] =
{
	"hit",
	"slice",
	"stab",
	"slash",
	"whip", 
	"claw",
	"blast",  
	"pound",   
	"crush", 
	"grep", 
	"bite",
	"pierce", 
	"suction", 
	"chop", 
	"shot", 
	"dart",
	"ammo"  // add at 2022/04/29
};

/* Chinese Attack type table */
static const char *cattack_table[] =
{
	"¾Ä¤OÀ»¥´",
	"¾U§Q¤Á³Î",  // blade
	"«Â²r¤@ÂW",  // lance  
	"´§»R¼A¬å",  // sword
	"¨`¼ö¤@Ã@",  // whip
	"¥Î¤O¤@§ì",  // claw
	"Ãzµo¼QÀ»",  // magical damage
	"¸U¶v¤OÝ´",  // staff
	"¬½¬½½VÀ£",  // hammer
	"µR§Q¤@¨ï",  // axe 
	"±i¤f²r«r",
	"¦y¾U¨ëÀ»",  //dagger
	"²r¯P§l§m", 
	"¨ïµæ³s±Ù", 
	"®g¥X½b¥Ú", 
	"ÂY¥X­¸Ãð",
	"ºj±ñ®gÀ»"   // gun add at 2022/04/29
};

struct dam_message_type 
{
    int min_dmg;
    char* desc;
};

static const struct dam_message_type edmt[] = {
	{   0,	    "[1;37mcould not hurt[m" },
	{   1,	    "[1;36mscratches[m"	     },
	{   2,	    "[1;36mgrazes[m"		 },
	{   4,	    "[1;36mhits[m"		     },
	{   6,	    "[1;36minjures[m"	     },
	{   8,	    "[1;36mwounds[m"		 },
	{   15,	    "[1;36mmauls[m"		     },
	{   30,	    "[1;36mdecimates[m"	     },
	{   40,	    "[1;35mdecimates[m"	     },
	{   50,	    "[1;35mdevastates[m"	 },
	{   60,	    "[1;35mmaims[m"		     },
	{   70,	    "[1;35mMUTILATES[m"	     },
	{   80,	    "[1;35mDISEMBOWELS[m"	 },
	{   90,    "[1;35m* DISEMBOWELS *[m"},
	{   100,	"[1;32m* EVISCERATES *[m"},
	{   200,	"[1;32m** EVISCERATES **[m"},
	{   300,	"[1;32m** MASSACRES **[m"},
	{   400, 	"[1;32m*** MASSACRES ***[m"},
	{   500,	"[1;32m*** DEMOLISHES ***[m"},
	{   600,	"[1;32m**** DEMOLISHES ****[m"},
	{   700,	"[1;32m**** DESTROYS ****[m"},
	{   800,	"[1;32m***** DESTROYS *****[m"},
	{   900,	"[1;32mA-N-N-I-H-I-L-A-T-E-S[m"},
	{   1000,	"[1;33mA-N-N-I-H-I-L-A-T-E-S[m"},
	{   1500,	"[1;33mD-E-R-A-C-I-N-A-T-E-S[m"},
	{   2000,	"[1;33mE-R-A-D-I-C-A-T-E-S[m"},
	{   2500,	"[1;31mE-R-A-D-I-C-A-T-E-S[m"},
	{   3000,	"[1;31m! D..D-I-S-I-N-T-E-G-R-A-T-E-S ![m"},
	{   3500,	"[1;31m!! D..D-I-S-S-O-C-I-A-T-I-V-E-S !![m" },
	{   4000,	"[1;31m!!! COMMINUTES !!![m" },
	{   5000 , 	
		"\n\r\n\r\t[1;31mA - T - O - M - I - Z - E - S[m\n\r\n\r" },
	{   1073741824, 
		"\n\r\n\r\t[1;31;5mA - T - O - M - I - Z - E - S[m\n\r\n\r" }
};

static const struct dam_message_type cedmt[]={
	{0 , "[1;37mµLªk¶Ë®`[m"	     },
	{1 , "[1;36m¦p·L­·»´©Ø[m"	     },
	{2 ,"[1;36m»´»´À¿¹L[m"		     },
	{4 ,"[1;36mÀ°¦£·kÄo[m"		     },
	{6 ,"[1;36mÀ°¦£«ö¼¯[m"	     },
	{8 ,  "[1;36m¥´¨ì[m"		     },
	{15 ,  "[1;36mÀ»¤¤[m"		     },
	{30 ,  "[1;36mÀ~¨ì[m"	     },
	{40 ,  "[1;35mÀ~¨ì[m"	     },
	{50 ,  "[1;35mµy·P¯kµh[m"	     },
	{60 ,  "[1;35m¦³¨ÇÀ¿¶Ë[m"		     },
	{70 ,  "[1;35m¶}©l¯}¥Ö[m"	     },
	{80 ,  "[1;35m¯Q«C·ï¦å[m"	     },
	{90 ,  "[1;35m* ¯Q«C·ï¦å *[m"	     },
	{100 ,  "[1;32m* ¯kµh¤£³ô *[m"	     },
	{200 ,  "[1;32m** ¯kµh¤£³ô **[m"	     },
	{300 ,  "[1;32m** ¦yÁn«s¥s **[m"	     },
	{400 ,  "[1;32m*** ¦yÁn«s¥s ***[m"	     },
	{500 ,  "[1;32m*** ÂA¦åª½¬y ***[m" },
	{600 ,  "[1;32m**** ÂA¦åª½¬y ****[m"  },
	{700 ,  "[1;32m**** ¦å¬y¦pª` ****[m"  },
	{800 ,  "[1;32m***** ¦å¬y¦pª` *****[m"  },
	{900 ,  "[1;32m²r¼QÂA¦å¡A¾¦¤ú°nµõ[m" },
	{1000 ,  "[1;33m²r¼QÂA¦å¡A¾¦¤ú°nµõ[m" },
	{1500 ,  "[1;33mÁy¤Õ§á¦±¡A¤ºÅ¦¥X¦å[m" },
	{2000 ,  "[1;33m¤Àµ¬¿ù°©¡AÅ¦¾¹¸Hµõ[m" },
	{2500 ,  "[1;31m¤Àµ¬¿ù°©¡AÅ¦¾¹¸Hµõ[m" },
	{3000 ,  "[1;31m¡Iµ¬-¯ß-¤o-Â_¡A ¦å-¦×-¼Ò-½k¡I[m" },
	{3500 ,  "[1;31m¡I¡I¯»-¨­-¸H-°©¡A»î-­¸-¾z-·À¡I¡I[m" },
	{4000 ,  
		"\n\r[1;31m¡I¡I¡I ¤@  ¦ò  ¤É  ¤Ñ¡A¤G  ¦ò  ¯I  ½L ¡I¡I¡I\n\r«z...[m" },
	{5000 , 			  
		"\n\r\n\r\t\t[1;31m¤Æ  ¦¨  öÒ  ¯»\n\r\t\t\t¦Ç  ­¸  ´ó  ·À[m\n\r\t\t\t\t\t§Úªº¤Ñ...\n\r" },
	{1073741824 , 
		"\n\r\n\r\t\t[1;31;5m¤Æ  ¦¨  öÒ  ¯»\n\r\t\t\t¦Ç  ­¸  ´ó  ·À[m\n\r\t\t\t\t\t§Úªº¤Ñ...\n\r" }
};

#endif
