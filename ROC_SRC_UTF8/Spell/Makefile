CC		= gcc
OBJS	= magic.o spell_cure.o spell_assist.o spell_psichic.o\
		  spell_obj.o spell_aid.o spell_slash.o spell_breath.o\
		  spell_flame.o spell_lightning.o spell_ice.o\
		  spell_light.o spell_dark.o spell_gate.o

all: $(OBJS)

.c.o:
	$(CC) -c -g -O2 -Wall -I ../include/ $<
