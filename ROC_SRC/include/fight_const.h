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
	"�ĤO����",
	"�U�Q����",  // blade
	"�²r�@�W",  // lance  
	"���R�A��",  // sword
	"�`���@�@",  // whip
	"�ΤO�@��",  // claw
	"�z�o�Q��",  // magical damage
	"�U�v�Oݴ",  // staff
	"�����V��",  // hammer
	"�R�Q�@��",  // axe 
	"�i�f�r�r",
	"�y�U����",  //dagger
	"�r�P�l�m", 
	"���s��", 
	"�g�X�b��", 
	"�Y�X����",
	"�j��g��"   // gun add at 2022/04/29
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
	{0 , "[1;37m�L�k�ˮ`[m"	     },
	{1 , "[1;36m�p�L������[m"	     },
	{2 ,"[1;36m�������L[m"		     },
	{4 ,"[1;36m�����k�o[m"		     },
	{6 ,"[1;36m��������[m"	     },
	{8 ,  "[1;36m����[m"		     },
	{15 ,  "[1;36m����[m"		     },
	{30 ,  "[1;36m�~��[m"	     },
	{40 ,  "[1;35m�~��[m"	     },
	{50 ,  "[1;35m�y�P�k�h[m"	     },
	{60 ,  "[1;35m��������[m"		     },
	{70 ,  "[1;35m�}�l�}��[m"	     },
	{80 ,  "[1;35m�Q�C���[m"	     },
	{90 ,  "[1;35m* �Q�C��� *[m"	     },
	{100 ,  "[1;32m* �k�h���� *[m"	     },
	{200 ,  "[1;32m** �k�h���� **[m"	     },
	{300 ,  "[1;32m** �y�n�s�s **[m"	     },
	{400 ,  "[1;32m*** �y�n�s�s ***[m"	     },
	{500 ,  "[1;32m*** �A�媽�y ***[m" },
	{600 ,  "[1;32m**** �A�媽�y ****[m"  },
	{700 ,  "[1;32m**** ��y�p�` ****[m"  },
	{800 ,  "[1;32m***** ��y�p�` *****[m"  },
	{900 ,  "[1;32m�r�Q�A��A�����n��[m" },
	{1000 ,  "[1;33m�r�Q�A��A�����n��[m" },
	{1500 ,  "[1;33m�y�էᦱ�A��Ŧ�X��[m" },
	{2000 ,  "[1;33m���������AŦ���H��[m" },
	{2500 ,  "[1;31m���������AŦ���H��[m" },
	{3000 ,  "[1;31m�I��-��-�o-�_�A ��-��-��-�k�I[m" },
	{3500 ,  "[1;31m�I�I��-��-�H-���A��-��-�z-���I�I[m" },
	{4000 ,  
		"\n\r[1;31m�I�I�I �@  ��  ��  �ѡA�G  ��  �I  �L �I�I�I\n\r�z...[m" },
	{5000 , 			  
		"\n\r\n\r\t\t[1;31m��  ��  ��  ��\n\r\t\t\t��  ��  ��  ��[m\n\r\t\t\t\t\t�ڪ���...\n\r" },
	{1073741824 , 
		"\n\r\n\r\t\t[1;31;5m��  ��  ��  ��\n\r\t\t\t��  ��  ��  ��[m\n\r\t\t\t\t\t�ڪ���...\n\r" }
};

#endif
