# Generic Makefile.  Read 'port.txt' in the documentation
# directory for the Makefile that is right for you  - Kahn

CC      = gcc
#CC		= g++
CPLUS	= g++
PROF    = 
DEBUG   = -g 
NOCRYPT =
O_FLAGS = -O2
C_FLAGS = $(O_FLAGS) -lm -Wall $(DEBUG) $(PROF) $(NOCRYPT)
L_FLAGS = $(C_FLAGS) -lcrypt
I_FLAGS = $(C_FLAGS) -I./include/

# header files list.
INC		= ansi.h auc.h fight_const.h gsn.h memwatch.h\
		  merc.h olc.h oprog.h rprog.h skill.h update.h

# Objects list.
# Normal action objects.
O_ACT	= Act/act_comm.o Act/act_info.o Act/act_move.o\
		  Act/act_obj.o Act/act_wiz.o Act/act_social.o

# Room/Object/Mobile program objects.
O_RPROG	= RoomProg/room_prog.o RoomProg/room_commands.o
O_MPROG	= MobProg/mob_commands.o MobProg/mob_prog.o MobProg/mprog_load.o
O_OPROG	= ObjProg/obj_prog.o ObjProg/obj_commands.o

# Fight module's objects.
O_FIGHT = fight.o fight_check.o fight_damage.o fight_msg.o fight_hits.o

# Skill/Spell objects.
O_SKILL = Skill/skill.o Skill/skill_lead.o Skill/skill_defensive.o\
		  Skill/skill_slash.o
O_SPELL = Spell/magic.o Spell/spell_cure.o Spell/spell_assist.o\
		  Spell/spell_psichic.o Spell/spell_obj.o Spell/spell_aid.o\
		  Spell/spell_slash.o Spell/spell_breath.o Spell/spell_flame.o\
		  Spell/spell_lightning.o Spell/spell_ice.o\
		  Spell/spell_light.o Spell/spell_dark.o Spell/spell_gate.o

# OLC objects.
O_OLC	= OnlineCreate/olc.o OnlineCreate/olc_act.o OnlineCreate/olc_save.o

# Misc objects.
O_MISC	= Misc/string.o Misc/chinese.o Misc/fread.o Misc/condition.o

O_FILES = comm.o const.o db.o handler.o interp.o save.o ssm.o\
		  special.o update.o bit.o mem.o\
		  clan.o memory.o commands.o program.o\
		  affect.o

OBJS	= $(O_ACT) $(O_SKILL) $(O_SPELL) $(O_OLC) $(O_MISC)\
		  $(O_FIGHT) $(O_OPROG) $(O_RPROG) $(O_MPROG) $(O_FILES)

all: envy envy_logger

envy: act skill spell misc rprg mprg oprg olc $(O_FILES) $(O_FIGHT)
	rm -f envy2
#	purify -cache_dir=/tmp $(CC) $(L_FLAGS) -o envy $(O_FILES)
	$(CC) $(I_FLAGS) $(L_FLAGS) -o envy2 $(OBJS)
#	$(CC) $(L_FLAGS) -o envy.new $(O_FILES)

envy_logger: envy_logger.cc
	rm -f envy_logger
	$(CPLUS) $(I_FLAGS) -o envy_logger envy_logger.cc

.c.o:
	$(CC) -c $(I_FLAGS) $<

skill: 
	cd Skill; make all

spell:
	cd Spell; make all

act:
	cd Act; make all

misc:
	cd Misc; make all

rprg:
	cd RoomProg; make all

mprg:
	cd MobProg; make all

oprg:
	cd ObjProg; make all

olc:
	cd OnlineCreate; make all

clean:
	rm -f *.o envy envy2 envy_logger *.c~ *.h~ */*.o
	rm -f */*.c~ */*.h~ */*.o~

update:
	svn update --username mudadm --password "mrudoadmc"

install: all
