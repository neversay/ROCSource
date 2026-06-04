# Auto-converted from skill.h
import typing

# skill.h: Structs for the central skill and spell lookup lists, categories, and progression parameters.
# skill.h: 中央技能與魔法查閱資料表之結構、分類及熟練度晉升相關參數定義。
def check_dodge(ch: 'CHAR_DATA', victim: 'CHAR_DATA', leveldiff: 'int', dexdiff: 'int') -> 'bool':
    """ C function: bool  check_dodge(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dexdiff) """
    pass

# extern bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff );
def check_parry(ch: 'CHAR_DATA', victim: 'CHAR_DATA', leveldiff: 'int', dt: 'int', wpn: 'int') -> 'bool':
    """ C function: bool  check_parry(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff, int dt, int wpn) """
    pass

def check_block(ch: 'CHAR_DATA', victim: 'CHAR_DATA', leveldiff: 'int') -> 'bool':
    """ C function: bool  check_block(CHAR_DATA *ch, CHAR_DATA *victim, int leveldiff) """
    pass

def check_tactics(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool  check_tactics(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def dam_message(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int') -> 'None':
    """ C function: void  dam_message(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn) """
    pass

def c_dam_message(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int') -> 'None':
    """ C function: void  c_dam_message(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn) """
    pass

def death_cry(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  death_cry(CHAR_DATA *ch) """
    pass

def group_gain(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  group_gain(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def xp_compute(gch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'int':
    """ C function: int   xp_compute(CHAR_DATA *gch, CHAR_DATA *victim) """
    pass

def is_wielding_poisoned(ch: 'CHAR_DATA', wpn: 'int') -> 'bool':
    """ C function: bool  is_wielding_poisoned(CHAR_DATA *ch, int wpn) """
    pass

def make_corpse(killer: 'CHAR_DATA', ch: 'CHAR_DATA') -> 'None':
    """ C function: void  make_corpse(CHAR_DATA *killer, CHAR_DATA *ch) """
    pass

def one_hit(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dt: 'int', wpn: 'int') -> 'int':
    """ C function: int   one_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn) """
    pass

# extern int 	mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int wpn );
def mone_hit(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dt_orig: 'int', wpn: 'int', msg_mode: 'int', dt_mode: 'int') -> 'int':
    """ C function: int  mone_hit(CHAR_DATA *ch, CHAR_DATA *victim, int *dt_orig, int wpn, int msg_mode, int dt_mode) """
    pass

# extern int 	brief_mone_hit( CHAR_DATA *ch, CHAR_DATA *victim, int* dt, int wpn );
def raw_kill(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool  raw_kill(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def set_fighting(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  set_fighting(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def disarm(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  disarm(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def trip(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  trip(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def check_race_special(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  check_race_special(CHAR_DATA *ch) """
    pass

def use_magical_item(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  use_magical_item(CHAR_DATA *ch) """
    pass

def track_hunt(ch: 'CHAR_DATA', victim: 'CHAR_DATA', was_in: 'ROOM_INDEX_DATA', door: 'int') -> 'None':
    """ C function: void  track_hunt(CHAR_DATA *ch, CHAR_DATA *victim, ROOM_INDEX_DATA *was_in, int door) """
    pass

def do_lead_assistance(ch: 'CHAR_DATA', protector: 'CHAR_DATA') -> 'None':
    """ C function: void  do_lead_assistance(CHAR_DATA *ch, CHAR_DATA *protector) """
    pass

def do_lead_assault(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  do_lead_assault(CHAR_DATA *ch, char *argument) """
    pass

def do_lead_ucharge(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  do_lead_ucharge(CHAR_DATA *ch, char *argument) """
    pass

def do_lead_ufire(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  do_lead_ufire(CHAR_DATA *ch, char *argument) """
    pass

def do_lead_hidemarch(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  do_lead_hidemarch(CHAR_DATA *ch, char *argument) """
    pass

def do_lead_envelopment(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  do_lead_envelopment(CHAR_DATA *ch, char *argument) """
    pass

def say_spell(ch: 'CHAR_DATA', sn: 'int') -> 'None':
    """ C function: void  say_spell args((CHAR_DATA * ch, int sn)) """
    pass

def set_fighting(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void            set_fighting(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def fall_effect(ch: 'CHAR_DATA') -> 'None':
    """ C function: void            fall_effect(CHAR_DATA *ch) """
    pass

def spell_flaming_armor_strike(sn: 'int', level: 'int', ch: 'CHAR_DATA', vo: 'None') -> 'None':
    """ C function: void            spell_flaming_armor_strike(int sn, int level, CHAR_DATA *ch, void *vo) """
    pass

def skill_lookup(name: 'const char') -> 'int':
    """ C function: int             skill_lookup(const char *name) """
    pass

def saves_spell(pp: 'int', victim: 'CHAR_DATA') -> 'int':
    """ C function: int             saves_spell(int pp, CHAR_DATA *victim) """
    pass

def new_saves_spell(pp: 'int', victim: 'CHAR_DATA', type: 'int') -> 'int':
    """ C function: int             new_saves_spell(int pp, CHAR_DATA *victim, int type) """
    pass

def old_saves_spell(pp: 'int', victim: 'CHAR_DATA', type: 'int') -> 'int':
    """ C function: int             old_saves_spell(int pp, CHAR_DATA *victim, int type) """
    pass

def new_saves_timer(pp: 'int', victim: 'CHAR_DATA', type: 'int') -> 'int':
    """ C function: int             new_saves_timer(int pp, CHAR_DATA *victim, int type) """
    pass

def int_effect(ch: 'CHAR_DATA') -> 'float':
    """ C function: double          int_effect(CHAR_DATA *ch) """
    pass

def wis_effect(ch: 'CHAR_DATA') -> 'float':
    """ C function: double          wis_effect(CHAR_DATA *ch) """
    pass

target_name: 'str' = ""
def weakness_percent(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int             weakness_percent(CHAR_DATA *ch, int sn) """
    pass

def weakness_damage(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int             weakness_damage(CHAR_DATA *ch, int sn) """
    pass

def check_fire_container(con: 'OBJ_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void            check_fire_container(OBJ_DATA *con, CHAR_DATA *victim) """
    pass
