# Auto-converted from merc.h
import typing

# merc.h: The primary, core engine header containing all main structures, constants, and function prototypes.
# merc.h: 核心引擎主標頭檔，包含 MUD 所有主要資料結構、常數定義及函式原型宣告。
const = None

# Macro function: args(list)
def args(list):
    # C implementation: ()
    pass

# Macro function: DECLARE_DO_FUN(fun)
def DECLARE_DO_FUN(fun):
    # C implementation: void fun()
    pass

# Macro function: DECLARE_SPEC_FUN(fun)
def DECLARE_SPEC_FUN(fun):
    # C implementation: bool fun()
    pass

# Macro function: DECLARE_SPELL_FUN(fun)
def DECLARE_SPELL_FUN(fun):
    # C implementation: void fun()
    pass

# Macro function: DECLARE_GAME_FUN(fun)
def DECLARE_GAME_FUN(fun):
    # C implementation: void fun()
    pass

# Macro function: args(list)
def args(list):
    # C implementation: list
    pass

# Macro function: DECLARE_DO_FUN(fun)
def DECLARE_DO_FUN(fun):
    # C implementation: DO_FUN fun
    pass

# Macro function: DECLARE_DO_RFUN(fun)
def DECLARE_DO_RFUN(fun):
    # C implementation: DO_RFUN fun
    pass

# Macro function: DECLARE_DO_OFUN(fun)
def DECLARE_DO_OFUN(fun):
    # C implementation: DO_OFUN fun
    pass

# Macro function: DECLARE_SPEC_FUN(fun)
def DECLARE_SPEC_FUN(fun):
    # C implementation: SPEC_FUN fun
    pass

# Macro function: DECLARE_SPELL_FUN(fun)
def DECLARE_SPELL_FUN(fun):
    # C implementation: SPELL_FUN fun
    pass

# Macro function: DECLARE_GAME_FUN(fun)
def DECLARE_GAME_FUN(fun):
    # C implementation: GAME_FUN fun
    pass
LATEST_VERSION = 37
FALSE = 0
TRUE = 1
const = None
unix = None
# #define strdup	STRDUP_ERROR__USE_STR_DUP!
BV00 = (1 << 0)
BV01 = (1 << 1)
BV02 = (1 << 2)
BV03 = (1 << 3)
BV04 = (1 << 4)
BV05 = (1 << 5)
BV06 = (1 << 6)
BV07 = (1 << 7)
BV08 = (1 << 8)
BV09 = (1 << 9)
BV10 = (1 << 10)
BV11 = (1 << 11)
BV12 = (1 << 12)
BV13 = (1 << 13)
BV14 = (1 << 14)
BV15 = (1 << 15)
BV16 = (1 << 16)
BV17 = (1 << 17)
BV18 = (1 << 18)
BV19 = (1 << 19)
BV20 = (1 << 20)
BV21 = (1 << 21)
BV22 = (1 << 22)
BV23 = (1 << 23)
BV24 = (1 << 24)
BV25 = (1 << 25)
BV26 = (1 << 26)
BV27 = (1 << 27)
BV28 = (1 << 28)
BV29 = (1 << 29)
BV30 = (1 << 30)
# typedef struct affect_data AFFECT_DATA
# typedef struct area_data AREA_DATA
# typedef struct auction_data AUC_DATA
# typedef struct auction_data AUCTION_DATA
# typedef struct ban_data BAN_DATA
# typedef struct call_out_data CALL_OUT_DATA
# typedef struct char_data CHAR_DATA
# typedef struct clan_data CLAN_DATA
# typedef struct descriptor_data DESCRIPTOR_DATA
# typedef struct exit_data EXIT_DATA
# typedef struct extra_descr_data EXTRA_DESCR_DATA
# typedef struct group_data GROUP_DATA
# typedef struct help_data HELP_DATA
# typedef struct kill_data KILL_DATA
# typedef struct mob_action_data MOB_ACTION
# typedef struct mob_index_data MOB_INDEX_DATA
# typedef struct mob_prog_act_list MPROG_ACT_LIST
# typedef struct mob_prog_data MPROG_DATA
# typedef struct note_data NOTE_DATA
# typedef struct obj_data OBJ_DATA
# typedef struct obj_hidden_power OBJ_HP
# typedef struct obj_index_data OBJ_INDEX_DATA
# typedef struct obj_prog_data OPROG_DATA
# typedef struct obj_extra_value OBJ_EXTRA_VALUE
# typedef struct pc_data PC_DATA
# typedef struct quest_mark_data QMARK
# typedef struct qmark_name QMARK_STRING
# typedef struct rcall_out_data RCALL_OUT_DATA
# typedef struct reset_data RESET_DATA
# typedef struct room_index_data ROOM_INDEX_DATA
# typedef struct room_prog_data RPROG_DATA
# typedef struct shop_data SHOP_DATA
# typedef struct symbol_data SYMBOL_DATA
# typedef struct time_info_data TIME_INFO_DATA
# typedef struct top10_data TOP10_DATA
# typedef struct weather_data WEATHER_DATA
# typedef struct mob_weakness_data MOB_WEAKNESS
# typedef struct battle_array BATTLE_ARRAY
# typedef struct obj_vs_race OBJ_VRACE
# typedef struct area_skill_data ASKILL_DATA
# typedef struct bank_account_data BANK_DATA
# typedef struct clan_war_data CLAN_WAR_DATA
string: 'int' = 0
deflt: 'int' = 0

class key_data:
    """ C struct key_data """
    def __init__(self):
        self.key: 'str' = ""
        self.ptrs: 'None' = None

name: 'str' = ""

class ban_data:
    """ C struct ban_data """
    def __init__(self):
        self.next: 'BAN_DATA' = None

hour: 'int' = 0
day: 'int' = 0
month: 'int' = 0
year: 'int' = 0

class time_info_data:
    """ C struct time_info_data """
    def __init__(self):
        pass

mmhg: 'int' = 0
change: 'int' = 0
sky: 'int' = 0
sunlight: 'int' = 0

class weather_data:
    """ C struct weather_data """
    def __init__(self):
        pass

name: 'str' = ""
cname: 'str' = ""
race_abilities: 'int' = 0
size: 'int' = 0
str_mod: 'int' = 0
int_mod: 'int' = 0
wis_mod: 'int' = 0
dex_mod: 'int' = 0
con_mod: 'int' = 0
hp_gain: 'int' = 0
mana_gain: 'int' = 0
move_gain: 'int' = 0
thirst_mod: 'int' = 0
hunger_mod: 'int' = 0
nature_dodge: 'int' = 0
nature_ac: 'int' = 0
dmg_message: 'str' = ""
hate: 'str' = ""
glow: 'int' = 0
ghigh: 'int' = 0

class race_type:
    """ C struct race_type """
    def __init__(self):
        self.nature_svs: 'int' = 0

min_drunk_level: 'int' = 0
number_of_rep: 'int' = 0

class struckdrunk:
    """ C struct struckdrunk """
    def __init__(self):
        self.replacement: 'str' = None

host: 'str' = ""
remoteuser: 'str' = ""
connected: 'int' = 0
fcommand: 'bool' = False
repeat: 'int' = 0
showstr_head: 'str' = ""
showstr_point: 'str' = ""
outbuf: 'str' = ""
outsize: 'int' = 0
outtop: 'int' = 0
editor: 'int' = 0
trytimes: 'int' = 0

class descriptor_data:
    """ C struct descriptor_data """
    def __init__(self):
        self.next: 'DESCRIPTOR_DATA' = None
        self.snoop_by: 'DESCRIPTOR_DATA' = None
        self.character: 'CHAR_DATA' = None
        self.original: 'CHAR_DATA' = None
        self.inbuf: 'str' = ""
        self.incomm: 'str' = ""
        self.inlast: 'str' = ""
        self.pEdit: 'None' = None

tohit: 'int' = 0
todam: 'int' = 0
carry: 'int' = 0
wield: 'int' = 0
wield2: 'int' = 0
twohand: 'int' = 0

class str_app_type:
    """ C struct str_app_type """
    def __init__(self):
        pass

learn: 'int' = 0

class int_app_type:
    """ C struct int_app_type """
    def __init__(self):
        pass

practice: 'int' = 0
tohit: 'int' = 0

class wis_app_type:
    """ C struct wis_app_type """
    def __init__(self):
        pass

defensive: 'int' = 0
tohit: 'int' = 0

class dex_app_type:
    """ C struct dex_app_type """
    def __init__(self):
        pass

hitp: 'int' = 0
shock: 'int' = 0
armor: 'int' = 0

class con_app_type:
    """ C struct con_app_type """
    def __init__(self):
        pass

TO_ROOM = 0
TO_NOTVICT = 1
TO_VICT = 2
TO_CHAR = 3
TO_ROOM_IMM = "4 // 1050619 add TO_ROOM_IMM 只給room中的imm看"
level: 'int' = 0
keyword: 'str' = ""
text: 'str' = ""

class help_data:
    """ C struct help_data """
    def __init__(self):
        self.next: 'HELP_DATA' = None
        self.area: 'AREA_DATA' = None

keeper: 'int' = 0
profit_buy: 'int' = 0
profit_sell: 'int' = 0
open_hour: 'int' = 0
close_hour: 'int' = 0

class shop_data:
    """ C struct shop_data """
    def __init__(self):
        self.next: 'SHOP_DATA' = None
        self.buy_type: 'int' = 0

attr_prime: 'int' = 0
weapon: 'int' = 0
guild: 'int' = 0
skill_adept: 'int' = 0
thac0_00: 'int' = 0
thac0_42: 'int' = 0
hp_min: 'int' = 0
hp_max: 'int' = 0
fMana: 'bool' = False

class class_type:
    """ C struct class_type """
    def __init__(self):
        self.who_name: 'str' = ""
        self.name: 'str' = ""
        self.cname: 'str' = ""
        self.cwho_name: 'str' = ""
        self.bclass: 'int' = 0

sender: 'str' = ""
csender: 'str' = ""
date: 'str' = ""
to_list: 'str' = ""
subject: 'str' = ""
text: 'str' = ""

class note_data:
    """ C struct note_data """
    def __init__(self):
        self.next: 'NOTE_DATA' = None
        self.date_stamp: 'int' = 0

type: 'int' = 0
duration: 'int' = 0
location: 'int' = 0
modifier: 'int' = 0
deleted: 'bool' = False

class affect_data:
    """ C struct affect_data """
    def __init__(self):
        self.next: 'AFFECT_DATA' = None

type: 'int' = 0
duration: 'int' = 0
modifier: 'int' = 0
deleted: 'bool' = False

class r_affect_data:
    """ C struct r_affect_data """
    def __init__(self):
        self.next: 'R_AFFECT_DATA' = None
        self.next_in_list: 'R_AFFECT_DATA' = None
        self.prev_in_list: 'R_AFFECT_DATA' = None
        self.owner: 'ROOM_INDEX_DATA' = None

number: 'int' = 0
killed: 'int' = 0

class kill_data:
    """ C struct kill_data """
    def __init__(self):
        pass

mark_name: 'str' = ""
timer: 'int' = 0
v0: 'int' = 0
v1: 'int' = 0
v2: 'int' = 0
fDeath: 'bool' = False
fSave: 'bool' = False

class quest_mark_data:
    """ C struct quest_mark_data """
    def __init__(self):
        self.owner: 'CHAR_DATA' = None
        self.prev_in_list: 'QMARK' = None
        self.next_in_list: 'QMARK' = None
        self.prev: 'QMARK' = None
        self.next: 'QMARK' = None

name: 'str' = ""

class qmark_name:
    """ C struct qmark_name """
    def __init__(self):
        self.next: 'QMARK_STRING' = None

vnum: 'int' = 0
hp_name: 'str' = ""
percent: 'int' = 0
event: 'int' = 0
level: 'int' = 0
p_race: 'int' = 0
p_align: 'int' = 0
p_class: 'int' = 0
p_sex: 'int' = 0
l_race: 'int' = 0
l_class: 'int' = 0
l_sex: 'int' = 0
l_align: 'int' = 0
a_race: 'int' = 0
a_class: 'int' = 0
a_sex: 'int' = 0
a_align: 'int' = 0
n_sex: 'int' = 0
n_align: 'int' = 0
n_class: 'int' = 0
n_race: 'int' = 0

class obj_hidden_power:
    """ C struct obj_hidden_power """
    def __init__(self):
        self.next: 'OBJ_HP' = None

percent: 'int' = 0
damage: 'int' = 0
name: 'str' = ""

class mob_weakness_data:
    """ C struct mob_weakness_data """
    def __init__(self):
        self.next: 'MOB_WEAKNESS' = None

player_name: 'str' = ""
short_descr: 'str' = ""
long_descr: 'str' = ""
description: 'str' = ""
vnum: 'int' = 0
count: 'int' = 0
killed: 'int' = 0
sex: 'int' = 0
level: 'int' = 0
affected_by2: 'int' = 0
alignment: 'int' = 0
mod_str: 'int' = 0
mod_int: 'int' = 0
mod_wis: 'int' = 0
mod_dex: 'int' = 0
mod_con: 'int' = 0
mod_size: 'int' = 0
mod_hitplus: 'int' = 0
mod_mana: 'int' = 0
mod_hitroll: 'int' = 0
mod_damroll: 'int' = 0
mod_ac: 'int' = 0
pdr: 'int' = 0
mod_exp: 'int' = 0
mdr: 'int' = 0
pad: 'int' = 0
mad: 'int' = 0
apdr: 'int' = 0
amdr: 'int' = 0
dodge: 'int' = 0
mod_pp: 'int' = 0
gold: 'int' = 0
race: 'int' = 0
class_: 'int' = 0
special: 'int' = 0
replica: 'int' = 0
psk: 'str' = ""
jcl: 'str' = ""
progtypes: 'int' = 0
fAutoset: 'bool' = False

class mob_index_data:
    """ C struct mob_index_data """
    def __init__(self):
        self.next: 'MOB_INDEX_DATA' = None
        self.spec_fun: 'SPEC_FUN' = None
        self.pShop: 'SHOP_DATA' = None
        self.weakness: 'MOB_WEAKNESS' = None
        self.mod_svs: 'int' = 0
        self.mobprogs: 'MPROG_DATA' = None
        self.mob_action: 'MOB_ACTION' = None
        self.area: 'AREA_DATA' = None

str: 'str' = ""

class symbol_data:
    """ C struct symbol_data """
    def __init__(self):
        self.next: 'SYMBOL_DATA' = None

name: 'str' = ""

class group_data:
    """ C struct group_data """
    def __init__(self):
        self.leader: 'CHAR_DATA' = None
        self.members: 'CHAR_DATA' = None
        self.next: 'GROUP_DATA' = None

clan_channel: 'str' = ""
tell_channel: 'str' = ""
clan_join: 'str' = ""
psk: 'str' = ""
jcl: 'str' = ""
name: 'str' = ""
cname: 'str' = ""
short_descr: 'str' = ""
long_descr: 'str' = ""
description: 'str' = ""
stun_desc: 'str' = ""
# char *		attacked;
sex: 'int' = 0
class_: 'int' = 0
race: 'int' = 0
temp_race: 'int' = 0
level: 'int' = 0
trust: 'int' = 0
played: 'int' = 0
perm_str: 'int' = 0
perm_int: 'int' = 0
perm_wis: 'int' = 0
perm_dex: 'int' = 0
perm_con: 'int' = 0
mod_str: 'int' = 0
mod_int: 'int' = 0
mod_wis: 'int' = 0
mod_dex: 'int' = 0
mod_con: 'int' = 0
mod_size: 'int' = 0
stay_time: 'int' = 0
timer: 'int' = 0
wait: 'int' = 0
hit: 'int' = 0
max_hit: 'int' = 0
mana: 'int' = 0
max_mana: 'int' = 0
move: 'int' = 0
max_move: 'int' = 0
gold: 'int' = 0
bank: 'int' = 0
exp: 'int' = 0
affected_by2: 'int' = 0
summoned: 'int' = 0
charmed: 'int' = 0
position: 'int' = 0
practice: 'int' = 0
carry_weight: 'int' = 0
carry_number: 'int' = 0
alignment: 'int' = 0
hitroll: 'int' = 0
damroll: 'int' = 0
armor: 'int' = 0
dodge: 'int' = 0
pp: 'int' = 0
mdr: 'int' = 0
pdr: 'int' = 0
amdr: 'int' = 0
apdr: 'int' = 0
pad: 'int' = 0
mad: 'int' = 0
wimpy: 'int' = 0
deaf: 'int' = 0
special: 'int' = 0
deleted: 'bool' = False
ordered: 'bool' = False
forced: 'bool' = False
mpactnum: 'int' = 0
track_times: 'int' = 0

class char_data:
    """ C struct char_data """
    def __init__(self):
        self.next: 'CHAR_DATA' = None
        self.next_in_room: 'CHAR_DATA' = None
        self.master: 'CHAR_DATA' = None
        self.leader: 'CHAR_DATA' = None
        self.fighting: 'CHAR_DATA' = None
        self.hunting: 'CHAR_DATA' = None
        self.hunted: 'CHAR_DATA' = None
        self.next_in_hunt: 'CHAR_DATA' = None
        self.spec_target: 'CHAR_DATA' = None
        self.assassinating: 'CHAR_DATA' = None
        self.assassinated: 'CHAR_DATA' = None
        self.cooperating: 'CHAR_DATA' = None
        self.cooperator: 'CHAR_DATA' = None
        self.next_in_coop: 'CHAR_DATA' = None
        self.linking: 'CHAR_DATA' = None
        self.next_in_ass: 'CHAR_DATA' = None
        self.tracking: 'CHAR_DATA' = None
        self.tracked: 'CHAR_DATA' = None
        self.next_in_track: 'CHAR_DATA' = None
        self.protector: 'CHAR_DATA' = None
        self.next_in_protect: 'CHAR_DATA' = None
        self.protecting: 'CHAR_DATA' = None
        self.reply: 'CHAR_DATA' = None
        self.attacked: 'CHAR_DATA' = None
        self.temp_char: 'CHAR_DATA' = None
        self.rembch: 'CHAR_DATA' = None
        self.rembed: 'CHAR_DATA' = None
        self.next_in_rember: 'CHAR_DATA' = None
        self.next_in_group: 'CHAR_DATA' = None
        self.group: 'GROUP_DATA' = None
        self.rrembed: 'ROOM_INDEX_DATA' = None
        self.orembed: 'OBJ_DATA' = None
        self.spec_fun: 'SPEC_FUN' = None
        self.pIndexData: 'MOB_INDEX_DATA' = None
        self.desc: 'DESCRIPTOR_DATA' = None
        self.affected: 'AFFECT_DATA' = None
        self.pnote: 'NOTE_DATA' = None
        self.carrying: 'OBJ_DATA' = None
        self.eqbank: 'OBJ_DATA' = None
        self.neweqbank: 'BANK_DATA' = None
        self.in_room: 'ROOM_INDEX_DATA' = None
        self.was_in_room: 'ROOM_INDEX_DATA' = None
        self.pcdata: 'PC_DATA' = None
        self.qmark: 'QMARK' = None
        self.clan: 'CLAN_DATA' = None
        self.weakness: 'MOB_WEAKNESS' = None
        self.logon: 'int' = 0
        self.save_time: 'int' = 0
        self.last_note: 'int' = 0
        self.saving_throw: 'int' = 0
        self.mpact: 'MPROG_ACT_LIST' = None
        self.mob_action: 'MOB_ACTION' = None
        self.gater: 'CHAR_DATA' = None
        self.time_stamp: 'int' = 0

pwd: 'str' = ""
bamfin: 'str' = ""
bamfout: 'str' = ""
immskll: 'str' = ""
title: 'str' = ""
prompt: 'str' = ""
clantitle: 'str' = ""
spouse: 'str' = ""
ride: 'int' = 0
rank: 'int' = 0
prime_attr: 'int' = 0
snd_attr: 'int' = 0
pagelen: 'int' = 0
respect: 'int' = 0
skexp_earned: 'int' = 0
skexp_used: 'int' = 0
skexp_earned_max: 'int' = 0
clanlevel: 'int' = 0
clanstatistics: 'int' = 0
clanexp: 'int' = 0
clanpk: 'int' = 0
pkwin: 'int' = 0
pklose: 'int' = 0
switched: 'bool' = False
autocombat: 'int' = 0
security: 'int' = 0
mprog_edit: 'int' = 0
version: 'int' = 0
merit: 'int' = 0
merit_spent: 'int' = 0

class pc_data:
    """ C struct pc_data """
    def __init__(self):
        self.next: 'PC_DATA' = None
        self.horse: 'CHAR_DATA' = None
        self.ba: 'BATTLE_ARRAY' = None
        self.condition: 'int' = 0
        self.learned: 'int' = 0
        self.denied: 'int' = 0
        self.freezed: 'int' = 0

LIQ_WATER = 0
LIQ_MAX = 30
liq_name: 'str' = ""
liq_ename: 'str' = ""
liq_color: 'str' = ""

class liq_type:
    """ C struct liq_type """
    def __init__(self):
        self.liq_affect: 'int' = 0

keyword: 'str' = ""
description: 'str' = ""
deleted: 'bool' = False

class extra_descr_data:
    """ C struct extra_descr_data """
    def __init__(self):
        self.next: 'EXTRA_DESCR_DATA' = None

# SPECIAL_AFFECT_DATA *			special_affected;
name: 'str' = ""
# char *cname;
short_descr: 'str' = ""
description: 'str' = ""
vnum: 'int' = 0
progtypes: 'int' = 0
item_type: 'int' = 0
extra_flags: 'int' = 0
wear_flags: 'int' = 0
count: 'int' = 0
replica: 'int' = 0
horse: 'int' = 0
weight: 'int' = 0
level: 'int' = 0
max_level: 'int' = 0
cost: 'int' = 0
timer: 'int' = 0
class_: 'int' = 0
hp_use_race: 'int' = 0
hp_use_class: 'int' = 0
hp_use_sex: 'int' = 0
hp_use_align: 'int' = 0
hp_affect_class: 'int' = 0
hp_affect_sex: 'int' = 0
hp_affect_align: 'int' = 0
souvenir: 'int' = 0
race: 'int' = 0
on_message: 'str' = ""
hit_message: 'str' = ""
off_message: 'str' = ""

class obj_index_data:
    """ C struct obj_index_data """
    def __init__(self):
        self.next: 'OBJ_INDEX_DATA' = None
        self.extra_descr: 'EXTRA_DESCR_DATA' = None
        self.affected: 'AFFECT_DATA' = None
        self.hp: 'OBJ_HP' = None
        self.objprogs: 'OPROG_DATA' = None
        self.value: 'int' = 0
        self.vrace: 'OBJ_VRACE' = None
        self.material: 'OBJ_INDEX_DATA' = None
        self.area: 'AREA_DATA' = None

type: 'int' = 0
duration: 'int' = 0
location: 'int' = 0
modifier: 'int' = 0
deleted: 'bool' = False

class special_affect_data:
    """ C struct special_affect_data """
    def __init__(self):
        self.next: 'AFFECT_DATA' = None

used: 'bool' = False

class obj_extra_value:
    """ C struct obj_extra_value """
    def __init__(self):
        self.v: 'int' = 0
        self.t: 'int' = 0

name: 'str' = ""
short_descr: 'str' = ""
description: 'str' = ""
in_eqbank: 'int' = 0
item_type: 'int' = 0
extra_flags: 'int' = 0
wear_flags: 'int' = 0
wear_loc: 'int' = 0
weight: 'int' = 0
cost: 'int' = 0
level: 'int' = 0
timer: 'int' = 0
class_: 'int' = 0
race: 'int' = 0
deleted: 'bool' = False
update_checked: 'bool' = False

class obj_data:
    """ C struct obj_data """
    def __init__(self):
        self.next: 'OBJ_DATA' = None
        self.next_content: 'OBJ_DATA' = None
        self.contains: 'OBJ_DATA' = None
        self.in_obj: 'OBJ_DATA' = None
        self.carried_by: 'CHAR_DATA' = None
        self.orembch: 'CHAR_DATA' = None
        self.next_in_orember: 'OBJ_DATA' = None
        self.extra_descr: 'EXTRA_DESCR_DATA' = None
        self.affected: 'AFFECT_DATA' = None
        self.pIndexData: 'OBJ_INDEX_DATA' = None
        self.in_room: 'ROOM_INDEX_DATA' = None
        self.hp: 'OBJ_HP' = None
        self.vrace: 'OBJ_VRACE' = None
        self.material: 'OBJ_INDEX_DATA' = None
        self.value: 'int' = 0
        self.extra_value: 'OBJ_EXTRA_VALUE' = None
        self.serial: 'int' = 0

vnum: 'int' = 0
exit_info: 'int' = 0
key: 'int' = 0
exit_size: 'int' = 0
keyword: 'str' = ""
description: 'str' = ""
rs_flags: 'int' = 0
orig_door: 'int' = 0

class exit_data:
    """ C struct exit_data """
    def __init__(self):
        self.to_room: 'ROOM_INDEX_DATA' = None
        self.next: 'EXIT_DATA' = None

command: 'str' = ""
arg1: 'int' = 0
arg2: 'int' = 0
arg3: 'int' = 0

class reset_data:
    """ C struct reset_data """
    def __init__(self):
        self.next: 'RESET_DATA' = None

name: 'str' = ""
identity: 'str' = ""
recall: 'int' = 0
death: 'int' = 0
age: 'int' = 0
nplayer: 'int' = 0
filename: 'str' = ""
builders: 'str' = ""
security: 'int' = 0
lvnum: 'int' = 0
uvnum: 'int' = 0
vnum: 'int' = 0
area_flags: 'int' = 0
translator: 'str' = ""
modifier: 'str' = ""
f_toggle: 'int' = 0
reborn: 'int' = 0

class area_data:
    """ C struct area_data """
    def __init__(self):
        self.next: 'AREA_DATA' = None
        self.clan: 'CLAN_DATA' = None
        self.weather: 'WEATHER_DATA' = None

name: 'str' = ""
description: 'str' = ""
vnum: 'int' = 0
fall: 'int' = 0
max_char: 'int' = 0
room_size: 'int' = 0
room_space: 'int' = 0
room_flags: 'int' = 0
light: 'int' = 0
sector_type: 'int' = 0
progtypes: 'int' = 0
temp_description: 'str' = ""

class room_index_data:
    """ C struct room_index_data """
    def __init__(self):
        self.next: 'ROOM_INDEX_DATA' = None
        self.people: 'CHAR_DATA' = None
        self.rrembch: 'CHAR_DATA' = None
        self.contents: 'OBJ_DATA' = None
        self.extra_descr: 'EXTRA_DESCR_DATA' = None
        self.area: 'AREA_DATA' = None
        self.exit: 'EXIT_DATA' = None
        self.next_in_rrember: 'ROOM_INDEX_DATA' = None
        self.affected: 'R_AFFECT_DATA' = None
        self.roomprogs: 'RPROG_DATA' = None
        self.reset_first: 'RESET_DATA' = None
        self.reset_last: 'RESET_DATA' = None

com_list: 'str' = ""
old_com_list: 'str' = ""
mode: 'int' = 0
start_time: 'int' = 0

class mob_action_data:
    """ C struct mob_action_data """
    def __init__(self):
        self.next_in_list: 'MOB_ACTION' = None
        self.prev_in_list: 'MOB_ACTION' = None
        self.next: 'MOB_ACTION' = None
        self.owner: 'CHAR_DATA' = None

TYPE_UNDEFINED = -1
TYPE_HIT = 1000
MSG_NORMAL = 0
MSG_BRIEF = 1
DMG_NORMAL = 0
DMG_ADJUST = 1
DT_NOCHANGE = 0
DT_CHANGE = 1
TAR_IGNORE = 0
TAR_CHAR_OFFENSIVE = 1
TAR_CHAR_DEFENSIVE = 2
TAR_CHAR_SELF = 3
TAR_OBJ_INV = 4
TAR_OBJ = 5
TAR_ARG = 6
name: 'str' = ""
cname: 'str' = ""
target: 'int' = 0
minimum_position: 'int' = 0
pgsn: 'int' = 0
min_mana: 'int' = 0
beats: 'int' = 0
noun_damage: 'str' = ""
msg_off: 'str' = ""
require_attr: 'int' = 0
# int		type;

class skill_type:
    """ C struct skill_type """
    def __init__(self):
        self.skill_level: 'int' = 0
        self.spell_fun: 'SPELL_FUN' = None
        self.apply_attr: 'int' = 0

id: 'str' = ""
value: 'int' = 0

class top10_data:
    """ C struct top10_data """
    def __init__(self):
        pass

ba11: 'str' = ""
ba12: 'str' = ""
ba13: 'str' = ""
ba21: 'str' = ""
ba22: 'str' = ""
ba23: 'str' = ""
ba31: 'str' = ""
ba32: 'str' = ""
ba33: 'str' = ""

class battle_array:
    """ C struct battle_array """
    def __init__(self):
        self.next: 'BATTLE_ARRAY' = None

fighting: 'int' = 0
clan_a_num: 'int' = 0
clan_b_num: 'int' = 0

class clan_war_data:
    """ C struct clan_war_data """
    def __init__(self):
        self.clan_a: 'CLAN_DATA' = None
        self.clan_b: 'CLAN_DATA' = None
        self.clan_a_member: 'CHAR_DATA' = None
        self.clan_b_member: 'CHAR_DATA' = None

ALLOW_BIT = 1048576
DENY_BIT = 2097152
NEW_BIT = 4194304
PRIME_STR = BV00
PRIME_DEX = BV01
PRIME_INT = BV02
PRIME_WIS = BV03
PRIME_CON = BV04
PRIME_ALL = (BV05 - 1)
SECOND_STR = BV05
SECOND_DEX = BV06
SECOND_INT = BV07
SECOND_WIS = BV08
SECOND_CON = BV09
SECOND_ALL = (BV10 - BV05)
ATTR_ALL = (BV10 - 1)
position: 'int' = 0
level: 'int' = 0
log: 'int' = 0
toPPL: 'bool' = False

class cmd_type:
    """ C struct cmd_type """
    def __init__(self):
        self.do_fun: 'DO_FUN' = None


class rcmd_type:
    """ C struct rcmd_type """
    def __init__(self):
        self.do_rfun: 'DO_RFUN' = None


class ocmd_type:
    """ C struct ocmd_type """
    def __init__(self):
        self.do_ofun: 'DO_OFUN' = None


class social_type:
    """ C struct social_type """
    def __init__(self):
        pass

buf: 'str' = ""

class mob_prog_act_list:
    """ C struct mob_prog_act_list """
    def __init__(self):
        self.next: 'MPROG_ACT_LIST' = None
        self.ch: 'CHAR_DATA' = None
        self.obj: 'OBJ_DATA' = None
        self.vo: 'None' = None

type: 'int' = 0
arglist: 'str' = ""
comlist: 'str' = ""

class mob_prog_data:
    """ C struct mob_prog_data """
    def __init__(self):
        self.next: 'MPROG_DATA' = None

MOBtrigger: 'bool' = False
type: 'int' = 0
arglist: 'str' = ""
comlist: 'str' = ""

class obj_prog_data:
    """ C struct obj_prog_data """
    def __init__(self):
        self.next: 'OPROG_DATA' = None
        self.next_in_list: 'OPROG_DATA' = None
        self.owner: 'OBJ_INDEX_DATA' = None

type: 'int' = 0
arglist: 'str' = ""
comlist: 'str' = ""
v0: 'int' = 0
v1: 'int' = 0

class room_prog_data:
    """ C struct room_prog_data """
    def __init__(self):
        self.next: 'RPROG_DATA' = None
        self.next_in_list: 'RPROG_DATA' = None
        self.owner: 'ROOM_INDEX_DATA' = None
        self.timer: 'int' = 0

spec_name: 'str' = ""

class spec_type:
    """ C struct spec_type """
    def __init__(self):
        self.spec_fun: 'SPEC_FUN' = None

name: 'str' = ""
settable: 'bool' = False

class flag_type:
    """ C struct flag_type """
    def __init__(self):
        pass

ERROR_PROG = -1
IN_FILE_PROG = 0
ACT_PROG = 1
SPEECH_PROG = 2
RAND_PROG = 4
FIGHT_PROG = 8
DEATH_PROG = 16
HITPRCNT_PROG = 32
ENTRY_PROG = 64
GREET_PROG = 128
ALL_GREET_PROG = 256
GIVE_PROG = 512
BRIBE_PROG = 1024
TALK_PROG = 2048
TICK_PROG = 4096
REPOP_PROG = 8192
DEFUN_PROG = 16384
HURT_PROG = 32768
KILL_PROG = 65536
ENTRY_GREET_PROG = "131072 // add at 2021/12/18"
ROOM_LEAVE = BV00
ROOM_ENTER = BV01
ROOM_RAND = BV02
ROOM_TIME = BV03
ROOM_COMMAND = BV04
ROOM_TRAP = BV05
ROOM_DEFUN = BV06
ROOM_MASK = BV07
OBJ_ACT = BV00
OBJ_BREAK = BV01
OBJ_RAND = BV02
OBJ_HIT = BV03
OBJ_REPOP = BV04
OBJ_BLOCK = BV05
OBJ_FIGHT = BV06
OBJ_DEFUN = BV07
OBJ_LOAD = BV08
OBJ_DISAPPEAR = BV09
OBJ_COMMAND = BV10
OBJ_TICK = BV11
OBJ_KILL = "BV12 // add at 2022/05/08"
AREA_NONE = 0
AREA_CHANGED = 1
AREA_ADDED = 2
AREA_LOADING = 4
AREA_VERBOSE = 8
AREA_BUILDING = 16
AREA_CLAN = 32
MAX_DIR = 6
NO_FLAG = "-99LL"
# typedef struct r_affect_data R_AFFECT_DATA
fLogAll: 'bool' = False
read_pipe_ptr: 'str' = ""
Reboot: 'bool' = False
top_affect: 'int' = 0
top_area: 'int' = 0
top_ed: 'int' = 0
top_exit: 'int' = 0
top_help: 'int' = 0
top_mob_index: 'int' = 0
top_obj_index: 'int' = 0
top_reset: 'int' = 0
top_room: 'int' = 0
top_shop: 'int' = 0
top_qmark: 'int' = 0
top_hiddenpower: 'int' = 0
top_errmem: 'int' = 0
top_vnum_mob: 'int' = 0
top_vnum_obj: 'int' = 0
top_vnum_room: 'int' = 0
MAX_PPL: 'int' = 0
MAX_TYPES = 15
def N(arg0: 'do_adore') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_adore) """
    pass

def N(arg0: 'do_repair') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_repair) """
    pass

def N(arg0: 'do_personal_recover') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_personal_recover) """
    pass

def N(arg0: 'do_unbackup') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_unbackup) """
    pass

def N(arg0: 'do_perform') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_perform) """
    pass

def N(arg0: 'do_skdemote') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_skdemote) """
    pass

def N(arg0: 'do_skadv') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_skadv) """
    pass

def N(arg0: 'do_advance') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_advance) """
    pass

def N(arg0: 'do_opstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_opstat) """
    pass

def N(arg0: 'do_eqbank') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_eqbank) """
    pass

def N(arg0: 'do_skilldrv') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_skilldrv) """
    pass

def N(arg0: 'do_mtlist') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mtlist) """
    pass

def N(arg0: 'do_mtstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mtstat) """
    pass

def N(arg0: 'do_mtset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mtset) """
    pass

def N(arg0: 'do_afk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_afk) """
    pass

def N(arg0: 'do_dice') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_dice) """
    pass

def N(arg0: 'do_allow') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_allow) """
    pass

def N(arg0: 'do_medi') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_medi) """
    pass

def N(arg0: 'do_meditate') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_meditate) """
    pass

def N(arg0: 'do_adv') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_adv) """
    pass

def N(arg0: 'do_answer') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_answer) """
    pass

def N(arg0: 'do_areas') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_areas) """
    pass

def N(arg0: 'do_at') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_at) """
    pass

def N(arg0: 'do_auction') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_auction) """
    pass

def N(arg0: 'do_auto') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_auto) """
    pass

def N(arg0: 'do_autocombat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_autocombat) """
    pass

def N(arg0: 'do_autoexit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_autoexit) """
    pass

def N(arg0: 'do_autoloot') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_autoloot) """
    pass

def N(arg0: 'do_autogold') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_autogold) """
    pass

def N(arg0: 'do_autosac') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_autosac) """
    pass

def N(arg0: 'do_battle_array') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_battle_array) """
    pass

def N(arg0: 'do_backstab') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_backstab) """
    pass

def N(arg0: 'do_bamfin') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bamfin) """
    pass

def N(arg0: 'do_bamfout') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bamfout) """
    pass

def N(arg0: 'do_bandage') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bandage) """
    pass

def N(arg0: 'do_ban') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ban) """
    pass

def N(arg0: 'do_bash') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bash) """
    pass

def N(arg0: 'do_beep') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_beep) """
    pass

def N(arg0: 'do_berserk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_berserk) """
    pass

def N(arg0: 'do_bet') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bet) """
    pass

def N(arg0: 'do_backup') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_backup) """
    pass

def N(arg0: 'do_blank') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_blank) """
    pass

def N(arg0: 'do_brandish') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_brandish) """
    pass

def N(arg0: 'do_brief') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_brief) """
    pass

def N(arg0: 'do_bug') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bug) """
    pass

def N(arg0: 'do_buy') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_buy) """
    pass

def N(arg0: 'do_cast') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_cast) """
    pass

def N(arg0: 'delayed_do_cast') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(delayed_do_cast) """
    pass

def N(arg0: 'do_cast_delay') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_cast_delay) """
    pass

def N(arg0: 'do_chameleon') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_chameleon) """
    pass

def N(arg0: 'do_channels') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_channels) """
    pass

def N(arg0: 'do_charge') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_charge) """
    pass

def N(arg0: 'do_chat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_chat) """
    pass

def N(arg0: 'do_circle') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_circle) """
    pass

def N(arg0: 'do_circular') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_circular) """
    pass

def N(arg0: 'do_clan') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_clan) """
    pass

def N(arg0: 'do_clanchat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_clanchat) """
    pass

def N(arg0: 'do_close') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_close) """
    pass

def N(arg0: 'do_cname') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_cname) """
    pass

def N(arg0: 'do_hname') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_hname) """
    pass

def N(arg0: 'do_hsummon') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_hsummon) """
    pass

def N(arg0: 'do_combine') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_combine) """
    pass

def N(arg0: 'do_commands') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_commands) """
    pass

def N(arg0: 'do_compare') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_compare) """
    pass

def N(arg0: 'do_config') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_config) """
    pass

def N(arg0: 'do_consent') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_consent) """
    pass

def N(arg0: 'do_consider') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_consider) """
    pass

def N(arg0: 'do_credits') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_credits) """
    pass

def N(arg0: 'do_deny') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_deny) """
    pass

def N(arg0: 'do_description') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_description) """
    pass

def N(arg0: 'do_disarm') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_disarm) """
    pass

def N(arg0: 'do_barehand') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_barehand) """
    pass

def N(arg0: 'do_disconnect') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_disconnect) """
    pass

def N(arg0: 'do_donate') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_donate) """
    pass

def N(arg0: 'do_down') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_down) """
    pass

def N(arg0: 'do_drink') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_drink) """
    pass

def N(arg0: 'do_drop') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_drop) """
    pass

def N(arg0: 'do_destro') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_destro) """
    pass

def N(arg0: 'do_destroy') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_destroy) """
    pass

def N(arg0: 'do_earth') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_earth) """
    pass

def N(arg0: 'do_east') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_east) """
    pass

def N(arg0: 'do_eat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_eat) """
    pass

def N(arg0: 'do_echo') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_echo) """
    pass

def N(arg0: 'do_elemental') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_elemental) """
    pass

def N(arg0: 'do_emote') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_emote) """
    pass

def N(arg0: 'do_equipment') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_equipment) """
    pass

def N(arg0: 'do_examine') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_examine) """
    pass

def N(arg0: 'do_exits') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_exits) """
    pass

def N(arg0: 'do_fee') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_fee) """
    pass

def N(arg0: 'do_feed') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_feed) """
    pass

def N(arg0: 'do_fill') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_fill) """
    pass

def N(arg0: 'do_flaming') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_flaming) """
    pass

def N(arg0: 'do_storm') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_storm) """
    pass

def N(arg0: 'do_stone_form') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_stone_form) """
    pass

def N(arg0: 'do_sprinkle') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sprinkle) """
    pass

def N(arg0: 'do_icecore') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_icecore) """
    pass

def N(arg0: 'do_fulgurant') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_fulgurant) """
    pass

def N(arg0: 'do_flee') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_flee) """
    pass

def N(arg0: 'do_follow') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_follow) """
    pass

def N(arg0: 'do_force') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_force) """
    pass

def N(arg0: 'do_forceo') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_forceo) """
    pass

def N(arg0: 'do_forcer') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_forcer) """
    pass

def N(arg0: 'do_addexp') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_addexp) """
    pass

def N(arg0: 'do_freeze') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_freeze) """
    pass

def N(arg0: 'do_get') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_get) """
    pass

def N(arg0: 'do_get_silence') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_get_silence) """
    pass

def N(arg0: 'do_give') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_give) """
    pass

def N(arg0: 'do_goto') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_goto) """
    pass

def N(arg0: 'do_group') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_group) """
    pass

def N(arg0: 'do_gtell') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_gtell) """
    pass

def N(arg0: 'do_heighten') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_heighten) """
    pass

def N(arg0: 'do_help') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_help) """
    pass

def N(arg0: 'do_hide') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_hide) """
    pass

def N(arg0: 'do_holylight') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_holylight) """
    pass

def N(arg0: 'do_idea') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_idea) """
    pass

def N(arg0: 'do_immtalk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_immtalk) """
    pass

def N(arg0: 'do_check_bit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_check_bit) """
    pass

def N(arg0: 'do_herotalk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_herotalk) """
    pass

def N(arg0: 'do_imtlset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_imtlset) """
    pass

def N(arg0: 'do_inventory') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_inventory) """
    pass

def N(arg0: 'do_invis') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_invis) """
    pass

def N(arg0: 'do_kick') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_kick) """
    pass

def N(arg0: 'do_hunt') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_hunt) """
    pass

def N(arg0: 'do_ass') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ass) """
    pass

def N(arg0: 'do_scan') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_scan) """
    pass

def N(arg0: 'do_frenzy') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_frenzy) """
    pass

def N(arg0: 'do_bashhead') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_bashhead) """
    pass

def N(arg0: 'do_abstraction') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_abstraction) """
    pass

def N(arg0: 'do_ammo') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ammo) """
    pass

def N(arg0: 'do_power') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_power) """
    pass

def N(arg0: 'do_reflexion') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_reflexion) """
    pass

def N(arg0: 'do_combat_howl') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_combat_howl) """
    pass

def N(arg0: 'do_track') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_track) """
    pass

def N(arg0: 'do_invoke') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_invoke) """
    pass

def N(arg0: 'do_darkraid') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_darkraid) """
    pass

def N(arg0: 'do_stare') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_stare) """
    pass

def N(arg0: 'do_deposit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_deposit) """
    pass

def N(arg0: 'do_withdraw') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_withdraw) """
    pass

def N(arg0: 'do_store') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_store) """
    pass

def N(arg0: 'do_receive') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_receive) """
    pass

def N(arg0: 'do_scout') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_scout) """
    pass

def N(arg0: 'do_kill') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_kill) """
    pass

def N(arg0: 'do_throw') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_throw) """
    pass

def N(arg0: 'do_lead') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_lead) """
    pass

def N(arg0: 'do_list') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_list) """
    pass

def N(arg0: 'do_lock') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_lock) """
    pass

def N(arg0: 'do_log') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_log) """
    pass

def N(arg0: 'do_look') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_look) """
    pass

def N(arg0: 'do_memory') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_memory) """
    pass

def N(arg0: 'do_meteoric_arrow') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_meteoric_arrow) """
    pass

def N(arg0: 'do_rainfall_arrow') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rainfall_arrow) """
    pass

def N(arg0: 'do_mfind') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mfind) """
    pass

def N(arg0: 'do_mload') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mload) """
    pass

def N(arg0: 'do_mset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mset) """
    pass

def N(arg0: 'do_mstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mstat) """
    pass

def N(arg0: 'do_cras') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_cras) """
    pass

def N(arg0: 'do_crash') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_crash) """
    pass

def N(arg0: 'do_idlequit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_idlequit) """
    pass

def N(arg0: 'do_mwhere') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mwhere) """
    pass

def N(arg0: 'do_murder') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_murder) """
    pass

def N(arg0: 'do_music') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_music) """
    pass

def N(arg0: 'do_newloc') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_newloc) """
    pass

def N(arg0: 'do_newlock') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_newlock) """
    pass

def N(arg0: 'do_noemote') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_noemote) """
    pass

def N(arg0: 'do_north') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_north) """
    pass

def N(arg0: 'do_note') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_note) """
    pass

def N(arg0: 'do_notell') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_notell) """
    pass

def N(arg0: 'do_numloc') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_numloc) """
    pass

def N(arg0: 'do_numlock') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_numlock) """
    pass

def N(arg0: 'do_ofind') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ofind) """
    pass

def N(arg0: 'do_oload') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_oload) """
    pass

def N(arg0: 'do_open') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_open) """
    pass

def N(arg0: 'do_order') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_order) """
    pass

def N(arg0: 'do_oset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_oset) """
    pass

def N(arg0: 'do_ostat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ostat) """
    pass

def N(arg0: 'do_owhere') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_owhere) """
    pass

def N(arg0: 'do_pagelen') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_pagelen) """
    pass

def N(arg0: 'do_pardon') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_pardon) """
    pass

def N(arg0: 'do_password') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_password) """
    pass

def N(arg0: 'do_peace') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_peace) """
    pass

def N(arg0: 'do_pick') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_pick) """
    pass

def N(arg0: 'do_poison_weapon') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_poison_weapon) """
    pass

def N(arg0: 'do_pose') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_pose) """
    pass

def N(arg0: 'do_practice') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_practice) """
    pass

def N(arg0: 'do_cooperate') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_cooperate) """
    pass

def N(arg0: 'do_protect') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_protect) """
    pass

def N(arg0: 'do_join') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_join) """
    pass

def N(arg0: 'do_prompt') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_prompt) """
    pass

def N(arg0: 'do_purge') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_purge) """
    pass

def N(arg0: 'do_put') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_put) """
    pass

def N(arg0: 'do_quaff') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_quaff) """
    pass

def N(arg0: 'do_question') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_question) """
    pass

def N(arg0: 'do_qui') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_qui) """
    pass

def N(arg0: 'do_quit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_quit) """
    pass

def N(arg0: 'do_reboo') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_reboo) """
    pass

def N(arg0: 'do_reboot') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_reboot) """
    pass

def N(arg0: 'do_recall') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_recall) """
    pass

def N(arg0: 'do_recho') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_recho) """
    pass

def N(arg0: 'do_recite') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_recite) """
    pass

def N(arg0: 'do_register') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_register) """
    pass

def N(arg0: 'do_remove') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_remove) """
    pass

def N(arg0: 'do_rent') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rent) """
    pass

def N(arg0: 'do_reply') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_reply) """
    pass

def N(arg0: 'do_report') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_report) """
    pass

def N(arg0: 'do_rescue') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rescue) """
    pass

def N(arg0: 'do_change') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_change) """
    pass

def N(arg0: 'do_rest') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rest) """
    pass

def N(arg0: 'do_restore') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_restore) """
    pass

def N(arg0: 'do_return') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_return) """
    pass

def N(arg0: 'do_ride') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ride) """
    pass

def N(arg0: 'do_rset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rset) """
    pass

def N(arg0: 'do_rstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rstat) """
    pass

def N(arg0: 'do_sacrifice') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sacrifice) """
    pass

def N(arg0: 'do_save') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_save) """
    pass

def N(arg0: 'do_silentsave') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_silentsave) """
    pass

def N(arg0: 'do_say') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_say) """
    pass

def N(arg0: 'do_score') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_score) """
    pass

def N(arg0: 'do_elescore') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_elescore) """
    pass

def N(arg0: 'do_sell') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sell) """
    pass

def N(arg0: 'do_shadow') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_shadow) """
    pass

def N(arg0: 'do_shout') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_shout) """
    pass

def N(arg0: 'do_shutdow') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_shutdow) """
    pass

def N(arg0: 'do_shutdown') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_shutdown) """
    pass

def N(arg0: 'do_silence') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_silence) """
    pass

def N(arg0: 'do_sla') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sla) """
    pass

def N(arg0: 'do_slay') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_slay) """
    pass

def N(arg0: 'do_sleep') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sleep) """
    pass

def N(arg0: 'do_slist') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_slist) """
    pass

def N(arg0: 'do_slookup') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_slookup) """
    pass

def N(arg0: 'do_snare') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_snare) """
    pass

def N(arg0: 'do_sneak') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sneak) """
    pass

def N(arg0: 'do_nightstalk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_nightstalk) """
    pass

def N(arg0: 'do_snoop') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_snoop) """
    pass

def N(arg0: 'do_socials') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_socials) """
    pass

def N(arg0: 'do_south') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_south) """
    pass

def N(arg0: 'do_spells') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_spells) """
    pass

def N(arg0: 'do_split') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_split) """
    pass

def N(arg0: 'do_sset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sset) """
    pass

def N(arg0: 'do_sstime') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sstime) """
    pass

def N(arg0: 'do_sstim') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sstim) """
    pass

def N(arg0: 'do_stake') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_stake) """
    pass

def N(arg0: 'do_stand') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_stand) """
    pass

def N(arg0: 'do_steal') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_steal) """
    pass

def N(arg0: 'do_sun') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sun) """
    pass

def N(arg0: 'do_suicid') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_suicid) """
    pass

def N(arg0: 'do_suicide') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_suicide) """
    pass

def N(arg0: 'do_hsuicid') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_hsuicid) """
    pass

def N(arg0: 'do_hsuicide') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_hsuicide) """
    pass

def N(arg0: 'do_switch') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_switch) """
    pass

def N(arg0: 'do_tell') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_tell) """
    pass

def N(arg0: 'do_test') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_test) """
    pass

def N(arg0: 'do_time') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_time) """
    pass

def N(arg0: 'do_title') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_title) """
    pass

def N(arg0: 'do_top10') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_top10) """
    pass

def N(arg0: 'do_train') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_train) """
    pass

def N(arg0: 'do_transfer') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_transfer) """
    pass

def N(arg0: 'do_trust') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_trust) """
    pass

def N(arg0: 'do_typo') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_typo) """
    pass

def N(arg0: 'do_twin') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_twin) """
    pass

def N(arg0: 'do_unlock') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_unlock) """
    pass

def N(arg0: 'do_untangle') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_untangle) """
    pass

def N(arg0: 'do_up') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_up) """
    pass

def N(arg0: 'do_users') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_users) """
    pass

def N(arg0: 'do_value') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_value) """
    pass

def N(arg0: 'do_visible') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_visible) """
    pass

def N(arg0: 'do_disperse') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_disperse) """
    pass

def N(arg0: 'do_wager') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wager) """
    pass

def N(arg0: 'do_wake') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wake) """
    pass

def N(arg0: 'do_wear') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wear) """
    pass

def N(arg0: 'do_weather') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_weather) """
    pass

def N(arg0: 'do_sector') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_sector) """
    pass

def N(arg0: 'do_west') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_west) """
    pass

def N(arg0: 'do_where') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_where) """
    pass

def N(arg0: 'do_who') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_who) """
    pass

def N(arg0: 'do_whois') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_whois) """
    pass

def N(arg0: 'do_wimpy') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wimpy) """
    pass

def N(arg0: 'do_wizhelp') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wizhelp) """
    pass

def N(arg0: 'do_wizify') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wizify) """
    pass

def N(arg0: 'do_wizlist') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wizlist) """
    pass

def N(arg0: 'do_wizlock') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_wizlock) """
    pass

def N(arg0: 'do_yell') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_yell) """
    pass

def N(arg0: 'do_zap') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_zap) """
    pass

def N(arg0: 'do_mpget') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpget) """
    pass

def N(arg0: 'do_mpasound') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpasound) """
    pass

def N(arg0: 'do_mplog') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mplog) """
    pass

def N(arg0: 'do_mpat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpat) """
    pass

def N(arg0: 'do_mpecho') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpecho) """
    pass

def N(arg0: 'do_mpechoaround') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpechoaround) """
    pass

def N(arg0: 'do_mpechoat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpechoat) """
    pass

def N(arg0: 'do_mpnonecho') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpnonecho) """
    pass

def N(arg0: 'do_mpnonechoaround') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpnonechoaround) """
    pass

def N(arg0: 'do_mpnonechoat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpnonechoat) """
    pass

def N(arg0: 'do_mpechotoall') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpechotoall) """
    pass

def N(arg0: 'do_mpforce') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpforce) """
    pass

def N(arg0: 'do_mpforceo') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpforceo) """
    pass

def N(arg0: 'do_mpforcer') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpforcer) """
    pass

def N(arg0: 'do_mpgoto') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpgoto) """
    pass

def N(arg0: 'do_mpgreward') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpgreward) """
    pass

def N(arg0: 'do_mpalign') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpalign) """
    pass

def N(arg0: 'do_mpskillexp') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpskillexp) """
    pass

def N(arg0: 'do_mpmerit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmerit) """
    pass

def N(arg0: 'do_mpjunk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpjunk) """
    pass

def N(arg0: 'do_mpkill') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpkill) """
    pass

def N(arg0: 'do_mpmake_horse') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmake_horse) """
    pass

def N(arg0: 'do_mpmload') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmload) """
    pass

def N(arg0: 'do_mpdesc') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpdesc) """
    pass

def N(arg0: 'do_mpmset') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmset) """
    pass

def N(arg0: 'do_mpnowait') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpnowait) """
    pass

def N(arg0: 'do_mpoload') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpoload) """
    pass

def N(arg0: 'do_mppurge') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mppurge) """
    pass

def N(arg0: 'do_mpstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpstat) """
    pass

def N(arg0: 'do_mastat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mastat) """
    pass

def N(arg0: 'do_mpset_obj_affect') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpset_obj_affect) """
    pass

def N(arg0: 'do_mpremove_obj_affect') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpremove_obj_affect) """
    pass

def N(arg0: 'do_mpmodify_obj_affect') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmodify_obj_affect) """
    pass

def N(arg0: 'do_mpcall') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpcall) """
    pass

def N(arg0: 'do_mpcallout') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpcallout) """
    pass

def N(arg0: 'do_rpstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rpstat) """
    pass

def N(arg0: 'do_opstat') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_opstat) """
    pass

def N(arg0: 'do_mptransfer') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mptransfer) """
    pass

def N(arg0: 'do_mpdef_rembch') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpdef_rembch) """
    pass

def N(arg0: 'do_mpadd_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpadd_qmark) """
    pass

def N(arg0: 'do_mpadd_all_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpadd_all_qmark) """
    pass

def N(arg0: 'do_add_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_add_qmark) """
    pass

def N(arg0: 'do_del_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_del_qmark) """
    pass

def N(arg0: 'do_mod_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mod_qmark) """
    pass

def N(arg0: 'do_mpdel_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpdel_qmark) """
    pass

def N(arg0: 'do_mpdel_all_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpdel_all_qmark) """
    pass

def N(arg0: 'do_mpmod_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmod_qmark) """
    pass

def N(arg0: 'do_mpmod_all_qmark') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmod_all_qmark) """
    pass

def N(arg0: 'do_talk') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_talk) """
    pass

def N(arg0: 'do_mpgold') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpgold) """
    pass

def N(arg0: 'do_mpexp') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpexp) """
    pass

def N(arg0: 'do_mppractice') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mppractice) """
    pass

def N(arg0: 'do_mpcexp') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpcexp) """
    pass

def N(arg0: 'do_mpresp') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpresp) """
    pass

def N(arg0: 'do_mpgroup') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpgroup) """
    pass

def N(arg0: 'do_rumor') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_rumor) """
    pass

def N(arg0: 'do_mpdis_magic') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpdis_magic) """
    pass

# DECLARE_DO_FUN( do_mpstun		);
def N(arg0: 'do_mpteleport') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpteleport) """
    pass

def N(arg0: 'do_mptrip') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mptrip) """
    pass

def N(arg0: 'do_mpmodify') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpmodify) """
    pass

def N(arg0: 'do_self_skill') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_self_skill) """
    pass

def N(arg0: 'do_northeast') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_northeast) """
    pass

def N(arg0: 'do_northwest') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_northwest) """
    pass

def N(arg0: 'do_southeast') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_southeast) """
    pass

def N(arg0: 'do_southwest') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_southwest) """
    pass

def N(arg0: 'do_rrecho') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rrecho) """
    pass

def N(arg0: 'do_rechoat') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rechoat) """
    pass

def N(arg0: 'do_rnonechoaround') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rnonechoaround) """
    pass

def N(arg0: 'do_rnonechoat') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rnonechoat) """
    pass

def N(arg0: 'do_rechoaround') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rechoaround) """
    pass

def N(arg0: 'do_rechotoall') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rechotoall) """
    pass

def N(arg0: 'do_rechotoarea') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rechotoarea) """
    pass

def N(arg0: 'do_modrvt') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_modrvt) """
    pass

def N(arg0: 'do_modhmv') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_modhmv) """
    pass

def N(arg0: 'do_modhmvgroup') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_modhmvgroup) """
    pass

def N(arg0: 'do_rat') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rat) """
    pass

def N(arg0: 'do_rcall') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rcall) """
    pass

def N(arg0: 'do_rcast') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rcast) """
    pass

def N(arg0: 'do_def_rembch') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_def_rembch) """
    pass

def N(arg0: 'do_roload') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_roload) """
    pass

def N(arg0: 'do_rmload') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rmload) """
    pass

def N(arg0: 'do_add_affect') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_add_affect) """
    pass

def N(arg0: 'do_del_affect') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_del_affect) """
    pass

def N(arg0: 'do_rforce') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rforce) """
    pass

def N(arg0: 'do_rtransfer') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rtransfer) """
    pass

def N(arg0: 'do_rtransfergroup') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rtransfergroup) """
    pass

def N(arg0: 'do_rwtransfer') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rwtransfer) """
    pass

def N(arg0: 'do_rwtransfergroup') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rwtransfergroup) """
    pass

def N(arg0: 'do_rpurge') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpurge) """
    pass

def N(arg0: 'do_rlog') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rlog) """
    pass

def N(arg0: 'do_radd_qmark') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_radd_qmark) """
    pass

def N(arg0: 'do_rpexp') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpexp) """
    pass

def N(arg0: 'do_rpgold') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpgold) """
    pass

def N(arg0: 'do_rpresp') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpresp) """
    pass

def N(arg0: 'do_rpalign') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpalign) """
    pass

def N(arg0: 'do_rpskillexp') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpskillexp) """
    pass

def N(arg0: 'do_rpmerit') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpmerit) """
    pass

def N(arg0: 'do_rppractice') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rppractice) """
    pass

def N(arg0: 'do_rgreward') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rgreward) """
    pass

def N(arg0: 'do_radd_all_qmark') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_radd_all_qmark) """
    pass

def N(arg0: 'do_rdel_qmark') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rdel_qmark) """
    pass

def N(arg0: 'do_rdel_all_qmark') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rdel_all_qmark) """
    pass

def N(arg0: 'do_rmod_qmark') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rmod_qmark) """
    pass

def N(arg0: 'do_rmod_all_qmark') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rmod_all_qmark) """
    pass

def N(arg0: 'do_rpmakedoor') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpmakedoor) """
    pass

def N(arg0: 'do_rpdeldoor') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpdeldoor) """
    pass

def N(arg0: 'do_rpsetdoor') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpsetdoor) """
    pass

def N(arg0: 'do_rcallout') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rcallout) """
    pass

def N(arg0: 'do_set_room') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_set_room) """
    pass

def N(arg0: 'do_rpstun') -> 'DECLARE_DO_RFU':
    """ C function: DECLARE_DO_RFU N(do_rpstun) """
    pass

def N(arg0: 'do_ooset') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_ooset) """
    pass

def N(arg0: 'do_ooecho') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_ooecho) """
    pass

def N(arg0: 'do_oechotoall') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oechotoall) """
    pass

def N(arg0: 'do_oechoat') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oechoat) """
    pass

def N(arg0: 'do_oechoaround') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oechoaround) """
    pass

def N(arg0: 'do_ononechoat') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_ononechoat) """
    pass

def N(arg0: 'do_ononechoaround') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_ononechoaround) """
    pass

def N(arg0: 'do_oteleport') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oteleport) """
    pass

def N(arg0: 'do_ogoto') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_ogoto) """
    pass

def N(arg0: 'do_oforce') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oforce) """
    pass

def N(arg0: 'do_omodhmv') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_omodhmv) """
    pass

def N(arg0: 'do_oforcer') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oforcer) """
    pass

def N(arg0: 'do_oadd_affect') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_oadd_affect) """
    pass

def N(arg0: 'do_odel_affect') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_odel_affect) """
    pass

def N(arg0: 'do_odef_rembch') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_odef_rembch) """
    pass

def N(arg0: 'do_ocast') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_ocast) """
    pass

def N(arg0: 'do_opurge') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_opurge) """
    pass

def N(arg0: 'do_opset_obj_affect') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_opset_obj_affect) """
    pass

def N(arg0: 'do_opremove_obj_affect') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_opremove_obj_affect) """
    pass

def N(arg0: 'do_opmodify_obj_affect') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_opmodify_obj_affect) """
    pass

def N(arg0: 'do_opmodvt') -> 'DECLARE_DO_OFU':
    """ C function: DECLARE_DO_OFU N(do_opmodvt) """
    pass

def N(arg0: 'do_aedit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_aedit) """
    pass

def N(arg0: 'do_redit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_redit) """
    pass

def N(arg0: 'do_ppl_redit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_ppl_redit) """
    pass

def N(arg0: 'do_oedit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_oedit) """
    pass

def N(arg0: 'do_medit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_medit) """
    pass

def N(arg0: 'do_asave') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_asave) """
    pass

def N(arg0: 'do_alist') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_alist) """
    pass

def N(arg0: 'do_resets') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_resets) """
    pass

def N(arg0: 'do_mpedit') -> 'DECLARE_DO_FU':
    """ C function: DECLARE_DO_FU N(do_mpedit) """
    pass

def N(arg0: 'spell_null') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_null) """
    pass

def N(arg0: 'spell_fire_wall') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fire_wall) """
    pass

def N(arg0: 'spell_meteor_shower') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_meteor_shower) """
    pass

def N(arg0: 'spell_ice_comet') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ice_comet) """
    pass

def N(arg0: 'spell_magic_arrow') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_magic_arrow) """
    pass

def N(arg0: 'spell_mental_empower') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mental_empower) """
    pass

def N(arg0: 'spell_resurrection') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_resurrection) """
    pass

def N(arg0: 'spell_mark') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mark) """
    pass

def N(arg0: 'spell_recall') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_recall) """
    pass

def N(arg0: 'spell_evacuate') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_evacuate) """
    pass

def N(arg0: 'spell_earth_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_earth_slash) """
    pass

def N(arg0: 'spell_mana_recharge') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mana_recharge) """
    pass

def N(arg0: 'spell_double_recharge') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_double_recharge) """
    pass

def N(arg0: 'spell_full_recharge') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_full_recharge) """
    pass

def N(arg0: 'spell_flaming_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flaming_slash) """
    pass

def N(arg0: 'spell_flaming_slash2') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flaming_slash2) """
    pass

def N(arg0: 'spell_storm_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_storm_slash) """
    pass

def N(arg0: 'spell_icecore_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_icecore_slash) """
    pass

def N(arg0: 'spell_fulgurant_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fulgurant_slash) """
    pass

def N(arg0: 'spell_elemental_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_elemental_slash) """
    pass

def N(arg0: 'spell_sun_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sun_slash) """
    pass

def N(arg0: 'spell_sprinkle_slash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sprinkle_slash) """
    pass

def N(arg0: 'spell_calm') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_calm) """
    pass

def N(arg0: 'spell_lifeshare') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lifeshare) """
    pass

def N(arg0: 'spell_mystic_mirror') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mystic_mirror) """
    pass

def N(arg0: 'spell_mystic_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mystic_armor) """
    pass

def N(arg0: 'spell_virtual_swords') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_virtual_swords) """
    pass

def N(arg0: 'spell_sober_up') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sober_up) """
    pass

def N(arg0: 'spell_acid_blast') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_acid_blast) """
    pass

def N(arg0: 'spell_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_armor) """
    pass

def N(arg0: 'spell_bless') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_bless) """
    pass

def N(arg0: 'spell_wizard_rage') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_wizard_rage) """
    pass

def N(arg0: 'spell_hasten') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_hasten) """
    pass

def N(arg0: 'spell_powerup') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_powerup) """
    pass

def N(arg0: 'spell_pfire') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_pfire) """
    pass

def N(arg0: 'spell_pcold') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_pcold) """
    pass

def N(arg0: 'spell_pwind') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_pwind) """
    pass

def N(arg0: 'spell_pearth') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_pearth) """
    pass

def N(arg0: 'spell_plightning') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_plightning) """
    pass

def N(arg0: 'spell_day_of_prot') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_day_of_prot) """
    pass

def N(arg0: 'spell_day_of_power') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_day_of_power) """
    pass

def N(arg0: 'spell_crushing_hand') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_crushing_hand) """
    pass

def N(arg0: 'spell_song_of_cleanse') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_song_of_cleanse) """
    pass

def N(arg0: 'spell_day_of_brightness') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_day_of_brightness) """
    pass

def N(arg0: 'spell_blindness') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_blindness) """
    pass

def N(arg0: 'spell_paralyze') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_paralyze) """
    pass

def N(arg0: 'spell_demontremor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_demontremor) """
    pass

def N(arg0: 'spell_powerball') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_powerball) """
    pass

def N(arg0: 'spell_starfire') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_starfire) """
    pass

def N(arg0: 'spell_whirlwind') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_whirlwind) """
    pass

def N(arg0: 'spell_grave_robber') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_grave_robber) """
    pass

def N(arg0: 'spell_full_refresh') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_full_refresh) """
    pass

def N(arg0: 'spell_seal_magic') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_seal_magic) """
    pass

def N(arg0: 'spell_group_full_refresh') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_group_full_refresh) """
    pass

def N(arg0: 'spell_group_double_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_group_double_heal) """
    pass

def N(arg0: 'spell_vivification') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_vivification) """
    pass

def N(arg0: 'spell_soulsteal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_soulsteal) """
    pass

def N(arg0: 'spell_lifesteal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lifesteal) """
    pass

def N(arg0: 'spell_flaming_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flaming_armor) """
    pass

def N(arg0: 'spell_mana_shield') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mana_shield) """
    pass

def N(arg0: 'spell_armageddon') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_armageddon) """
    pass

def N(arg0: 'spell_wizard_rage') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_wizard_rage) """
    pass

def N(arg0: 'spell_remove_nodrop') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_remove_nodrop) """
    pass

def N(arg0: 'spell_extra_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_extra_heal) """
    pass

def N(arg0: 'spell_gextra_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_gextra_heal) """
    pass

def N(arg0: 'spell_divine_light') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_divine_light) """
    pass

def N(arg0: 'spell_glacier_strike') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_glacier_strike) """
    pass

def N(arg0: 'spell_clay_golem') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_clay_golem) """
    pass

def N(arg0: 'spell_ogre_power') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ogre_power) """
    pass

def N(arg0: 'spell_endless_sight') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_endless_sight) """
    pass

def N(arg0: 'spell_water_spirit') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_water_spirit) """
    pass

def N(arg0: 'spell_greendragon_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_greendragon_breath) """
    pass

def N(arg0: 'spell_fire_elemental') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fire_elemental) """
    pass

def N(arg0: 'spell_double_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_double_heal) """
    pass

def N(arg0: 'spell_group_cure_blindness') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_group_cure_blindness) """
    pass

def N(arg0: 'spell_wind_screaming') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_wind_screaming) """
    pass

def N(arg0: 'spell_angry_fire_storm') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_angry_fire_storm) """
    pass

def N(arg0: 'spell_sun_flash') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sun_flash) """
    pass

def N(arg0: 'spell_green_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_green_breath) """
    pass

def N(arg0: 'spell_burning_hands') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_burning_hands) """
    pass

def N(arg0: 'spell_breathe_water') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_breathe_water) """
    pass

def N(arg0: 'spell_call_lightning') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_call_lightning) """
    pass

def N(arg0: 'spell_cause_critical') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cause_critical) """
    pass

def N(arg0: 'spell_cause_light') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cause_light) """
    pass

def N(arg0: 'spell_cause_serious') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cause_serious) """
    pass

def N(arg0: 'spell_change_sex') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_change_sex) """
    pass

def N(arg0: 'spell_charm_person') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_charm_person) """
    pass

def N(arg0: 'spell_chill_touch') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_chill_touch) """
    pass

def N(arg0: 'spell_colour_spray') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_colour_spray) """
    pass

def N(arg0: 'spell_cone_of_silence') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cone_of_silence) """
    pass

def N(arg0: 'spell_continual_light') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_continual_light) """
    pass

def N(arg0: 'spell_control_weather') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_control_weather) """
    pass

def N(arg0: 'spell_create_food') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_create_food) """
    pass

def N(arg0: 'spell_create_spring') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_create_spring) """
    pass

def N(arg0: 'spell_create_water') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_create_water) """
    pass

def N(arg0: 'spell_cure_blindness') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cure_blindness) """
    pass

def N(arg0: 'spell_cure_critical') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cure_critical) """
    pass

def N(arg0: 'spell_cure_light') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cure_light) """
    pass

def N(arg0: 'spell_cure_poison') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cure_poison) """
    pass

def N(arg0: 'spell_cure_serious') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cure_serious) """
    pass

def N(arg0: 'spell_curse') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_curse) """
    pass

def N(arg0: 'spell_destroy_cursed') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_destroy_cursed) """
    pass

def N(arg0: 'spell_detect_evil') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_detect_evil) """
    pass

def N(arg0: 'spell_detect_hidden') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_detect_hidden) """
    pass

def N(arg0: 'spell_detect_invis') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_detect_invis) """
    pass

def N(arg0: 'spell_detect_magic') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_detect_magic) """
    pass

def N(arg0: 'spell_detect_poison') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_detect_poison) """
    pass

def N(arg0: 'spell_dispel_evil') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_dispel_evil) """
    pass

def N(arg0: 'spell_dispel_magic') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_dispel_magic) """
    pass

def N(arg0: 'spell_earthquake') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_earthquake) """
    pass

def N(arg0: 'spell_enchant_weapon') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_enchant_weapon) """
    pass

def N(arg0: 'spell_enchant_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_enchant_armor) """
    pass

def N(arg0: 'spell_energy_drain') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_energy_drain) """
    pass

def N(arg0: 'spell_exorcise') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_exorcise) """
    pass

def N(arg0: 'spell_faerie_fire') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_faerie_fire) """
    pass

def N(arg0: 'spell_faerie_fog') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_faerie_fog) """
    pass

def N(arg0: 'spell_fireball') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fireball) """
    pass

def N(arg0: 'spell_flamestrike') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flamestrike) """
    pass

def N(arg0: 'spell_flaming') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flaming) """
    pass

def N(arg0: 'spell_storm') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_storm) """
    pass

def N(arg0: 'spell_sprinkle') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sprinkle) """
    pass

def N(arg0: 'spell_fly') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fly) """
    pass

def N(arg0: 'spell_gate') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_gate) """
    pass

def N(arg0: 'spell_general_purpose') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_general_purpose) """
    pass

def N(arg0: 'spell_giant_strength') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_giant_strength) """
    pass

def N(arg0: 'spell_harm') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_harm) """
    pass

def N(arg0: 'spell_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_heal) """
    pass

def N(arg0: 'spell_cont_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cont_heal) """
    pass

def N(arg0: 'spell_high_explosive') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_high_explosive) """
    pass

def N(arg0: 'spell_identify') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_identify) """
    pass

def N(arg0: 'spell_infravision') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_infravision) """
    pass

def N(arg0: 'spell_invis') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_invis) """
    pass

def N(arg0: 'spell_know_alignment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_know_alignment) """
    pass

def N(arg0: 'spell_lightning_bolt') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lightning_bolt) """
    pass

def N(arg0: 'spell_locate_object') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_locate_object) """
    pass

def N(arg0: 'spell_magic_missile') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_magic_missile) """
    pass

def N(arg0: 'spell_mass_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mass_heal) """
    pass

def N(arg0: 'spell_mass_invis') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mass_invis) """
    pass

def N(arg0: 'spell_mute') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mute) """
    pass

def N(arg0: 'spell_pass_door') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_pass_door) """
    pass

def N(arg0: 'spell_poison') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_poison) """
    pass

def N(arg0: 'spell_poison_explosion') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_poison_explosion) """
    pass

def N(arg0: 'spell_polymorph_other') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_polymorph_other) """
    pass

def N(arg0: 'spell_protection') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_protection) """
    pass

def N(arg0: 'spell_recharge_item') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_recharge_item) """
    pass

def N(arg0: 'spell_refresh') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_refresh) """
    pass

def N(arg0: 'spell_double_refresh') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_double_refresh) """
    pass

def N(arg0: 'spell_remove_alignment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_remove_alignment) """
    pass

def N(arg0: 'spell_remove_curse') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_remove_curse) """
    pass

def N(arg0: 'spell_remove_silence') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_remove_silence) """
    pass

def N(arg0: 'spell_sanctuary') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sanctuary) """
    pass

def N(arg0: 'spell_shocking_grasp') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_shocking_grasp) """
    pass

def N(arg0: 'spell_shield') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_shield) """
    pass

def N(arg0: 'spell_sleep') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sleep) """
    pass

def N(arg0: 'spell_stone_skin') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_stone_skin) """
    pass

def N(arg0: 'spell_summon') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_summon) """
    pass

def N(arg0: 'spell_tardy') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_tardy) """
    pass

def N(arg0: 'spell_teleport') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_teleport) """
    pass

def N(arg0: 'spell_turn_undead') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_turn_undead) """
    pass

def N(arg0: 'spell_ventriloquate') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ventriloquate) """
    pass

def N(arg0: 'spell_weaken') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_weaken) """
    pass

def N(arg0: 'spell_word_of_recall') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_word_of_recall) """
    pass

def N(arg0: 'spell_acid_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_acid_breath) """
    pass

def N(arg0: 'spell_fire_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fire_breath) """
    pass

def N(arg0: 'spell_frost_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_frost_breath) """
    pass

def N(arg0: 'spell_gas_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_gas_breath) """
    pass

def N(arg0: 'spell_lightning_breath') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lightning_breath) """
    pass

def N(arg0: 'spell_vampiric_bite') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_vampiric_bite) """
    pass

def N(arg0: 'spell_repair_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_repair_armor) """
    pass

def N(arg0: 'spell_holy_crown') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_holy_crown) """
    pass

def N(arg0: 'spell_mass_turn') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mass_turn) """
    pass

def N(arg0: 'spell_disease') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_disease) """
    pass

def N(arg0: 'spell_cure_disease') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cure_disease) """
    pass

def N(arg0: 'spell_fear') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fear) """
    pass

def N(arg0: 'spell_dispel_good') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_dispel_good) """
    pass

def N(arg0: 'spell_electrostatic_bolt') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_electrostatic_bolt) """
    pass

def N(arg0: 'spell_sand_tide') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_sand_tide) """
    pass

def N(arg0: 'spell_banshee_wail') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_banshee_wail) """
    pass

def N(arg0: 'spell_succubus_kiss') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_succubus_kiss) """
    pass

def N(arg0: 'spell_will_fortress') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_will_fortress) """
    pass

def N(arg0: 'spell_mind_blast') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mind_blast) """
    pass

def N(arg0: 'spell_disenchant') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_disenchant) """
    pass

def N(arg0: 'spell_ghosts_celebration') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ghosts_celebration) """
    pass

def N(arg0: 'spell_chain_lightning') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_chain_lightning) """
    pass

def N(arg0: 'spell_finger_of_death') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_finger_of_death) """
    pass

def N(arg0: 'spell_apocalypse') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_apocalypse) """
    pass

def N(arg0: 'spell_dark_containment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_dark_containment) """
    pass

def N(arg0: 'spell_phlogistic_verse') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_phlogistic_verse) """
    pass

def N(arg0: 'spell_wail_of_tempest') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_wail_of_tempest) """
    pass

def N(arg0: 'spell_fulgurant_dance') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fulgurant_dance) """
    pass

def N(arg0: 'spell_salvation') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_salvation) """
    pass

def N(arg0: 'spell_agglomerate_descant') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_agglomerate_descant) """
    pass

def N(arg0: 'spell_earth_bind') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_earth_bind) """
    pass

def N(arg0: 'spell_holy_judgement') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_holy_judgement) """
    pass

def N(arg0: 'spell_hell_southencross') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_hell_southencross) """
    pass

def N(arg0: 'spell_song_of_firestorm') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_song_of_firestorm) """
    pass

def N(arg0: 'spell_lore_of_frost') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lore_of_frost) """
    pass

def N(arg0: 'spell_soul_of_magma') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_soul_of_magma) """
    pass

def N(arg0: 'spell_fang_of_slime') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_fang_of_slime) """
    pass

def N(arg0: 'spell_magic_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_magic_enchantment) """
    pass

def N(arg0: 'spell_wind_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_wind_enchantment) """
    pass

def N(arg0: 'spell_flame_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flame_enchantment) """
    pass

def N(arg0: 'spell_water_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_water_enchantment) """
    pass

def N(arg0: 'spell_earth_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_earth_enchantment) """
    pass

def N(arg0: 'spell_lightning_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lightning_enchantment) """
    pass

def N(arg0: 'spell_saint_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_saint_enchantment) """
    pass

def N(arg0: 'spell_dark_enchantment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_dark_enchantment) """
    pass

# added by jye 0940711
# Sor New Skill
def N(arg0: 'spell_flame_elemental') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flame_elemental) """
    pass

def N(arg0: 'spell_water_elemental') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_water_elemental) """
    pass

def N(arg0: 'spell_earth_elemental') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_earth_elemental) """
    pass

def N(arg0: 'spell_wind_elemental') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_wind_elemental) """
    pass

def N(arg0: 'spell_lightning_elemental') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lightning_elemental) """
    pass

def N(arg0: 'spell_adrenaline_control') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_adrenaline_control) """
    pass

def N(arg0: 'spell_agitation') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_agitation) """
    pass

def N(arg0: 'spell_aura_sight') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_aura_sight) """
    pass

def N(arg0: 'spell_awe') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_awe) """
    pass

def N(arg0: 'spell_ballistic_attack') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ballistic_attack) """
    pass

def N(arg0: 'spell_biofeedback') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_biofeedback) """
    pass

def N(arg0: 'spell_cell_adjustment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_cell_adjustment) """
    pass

def N(arg0: 'spell_combat_mind') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_combat_mind) """
    pass

def N(arg0: 'spell_full_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_full_heal) """
    pass

def N(arg0: 'spell_group_full_heal') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_group_full_heal) """
    pass

def N(arg0: 'spell_control_flames') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_control_flames) """
    pass

def N(arg0: 'spell_create_sound') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_create_sound) """
    pass

def N(arg0: 'spell_death_field') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_death_field) """
    pass

def N(arg0: 'spell_detonate') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_detonate) """
    pass

def N(arg0: 'spell_disintegrate') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_disintegrate) """
    pass

def N(arg0: 'spell_displacement') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_displacement) """
    pass

def N(arg0: 'spell_domination') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_domination) """
    pass

def N(arg0: 'spell_ectoplasmic_form') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ectoplasmic_form) """
    pass

def N(arg0: 'spell_ego_whip') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ego_whip) """
    pass

def N(arg0: 'spell_energy_containment') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_energy_containment) """
    pass

def N(arg0: 'spell_enhance_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_enhance_armor) """
    pass

def N(arg0: 'spell_enhanced_strength') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_enhanced_strength) """
    pass

def N(arg0: 'spell_flesh_armor') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_flesh_armor) """
    pass

def N(arg0: 'spell_inertial_barrier') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_inertial_barrier) """
    pass

def N(arg0: 'spell_inflict_pain') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_inflict_pain) """
    pass

def N(arg0: 'spell_lend_health') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lend_health) """
    pass

def N(arg0: 'spell_levitation') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_levitation) """
    pass

def N(arg0: 'spell_barrier') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_barrier) """
    pass

def N(arg0: 'spell_mind_thrust') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_mind_thrust) """
    pass

def N(arg0: 'spell_project_force') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_project_force) """
    pass

def N(arg0: 'spell_psionic_blast') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_psionic_blast) """
    pass

def N(arg0: 'spell_psychic_crush') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_psychic_crush) """
    pass

def N(arg0: 'spell_psychic_drain') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_psychic_drain) """
    pass

def N(arg0: 'spell_psychic_healing') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_psychic_healing) """
    pass

def N(arg0: 'spell_share_strength') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_share_strength) """
    pass

def N(arg0: 'spell_thought_shield') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_thought_shield) """
    pass

def N(arg0: 'spell_ultrablast') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_ultrablast) """
    pass

def N(arg0: 'spell_lore') -> 'DECLARE_SPELL_FU':
    """ C function: DECLARE_SPELL_FU N(spell_lore) """
    pass

def N(arg0: 'spec_cast_water') -> 'DECLARE_SPEC_FU':
    """ C function: DECLARE_SPEC_FU N(spec_cast_water) """
    pass

def N(arg0: 'spec_cast_fire') -> 'DECLARE_SPEC_FU':
    """ C function: DECLARE_SPEC_FU N(spec_cast_fire) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

AmigaTCP = None
def atoi(string: 'const char') -> 'int':
    """ C function: int  atoi args((const char *string)) """
    pass

def calloc(nelem: 'unsigned', size: 'size_t') -> 'None':
    """ C function: void * calloc args((unsigned nelem, size_t size)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

NOCRYPT = None
def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def fclose(stream: 'FILE') -> 'int':
    """ C function: int  fclose args((FILE * stream)) """
    pass

def fprintf(stream: 'FILE', format: 'const char', arg2: '...') -> 'int':
    """ C function: int  fprintf args((FILE * stream, const char *format, ...)) """
    pass

def fread(ptr: 'None', size: 'int', n: 'int', stream: 'FILE') -> 'int':
    """ C function: int  fread args((void *ptr, int size, int n, FILE *stream)) """
    pass

def fseek(stream: 'FILE', offset: 'int', ptrname: 'int') -> 'int':
    """ C function: int  fseek args((FILE * stream, long offset, int ptrname)) """
    pass

def perror(s: 'const char') -> 'None':
    """ C function: void  perror args((const char *s)) """
    pass

def ungetc(c: 'int', stream: 'FILE') -> 'int':
    """ C function: int  ungetc args((int c, FILE *stream)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def fclose(stream: 'FILE') -> 'int':
    """ C function: int  fclose args((FILE * stream)) """
    pass

def fprintf(stream: 'FILE', format: 'const char', arg2: '...') -> 'int':
    """ C function: int  fprintf args((FILE * stream, const char *format, ...)) """
    pass

def fread(ptr: 'None', size: 'size_t', nitems: 'size_t', stream: 'FILE') -> 'size_t':
    """ C function: size_t  fread args((void *ptr, size_t size, size_t nitems, FILE *stream)) """
    pass

def fseek(stream: 'FILE', offset: 'int', ptrname: 'int') -> 'int':
    """ C function: int  fseek args((FILE * stream, long offset, int ptrname)) """
    pass

def perror(s: 'const char') -> 'None':
    """ C function: void  perror args((const char *s)) """
    pass

def ungetc(c: 'int', stream: 'FILE') -> 'int':
    """ C function: int  ungetc args((int c, FILE *stream)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def crypt(key: 'const char', salt: 'const char') -> 'str':
    """ C function: char * crypt args((const char *key, const char *salt)) """
    pass

def system(string: 'const char') -> 'int':
    """ C function: int  system args((const char *string)) """
    pass

def read(fd: 'int', buf: 'None', nbyte: 'size_t') -> 'ssize_t':
    """ C function: ssize_t  read args((int fd, void *buf, size_t nbyte)) """
    pass

def write(fd: 'int', buf: 'const void', nbyte: 'size_t') -> 'ssize_t':
    """ C function: ssize_t  write args((int fd, const void *buf, size_t nbyte)) """
    pass

def close(fd: 'int') -> 'int':
    """ C function: int  close args((int fd)) """
    pass


# Macro function: crypt(s1, s2)
def crypt(s1, s2):
    # C implementation: (s1)
    pass
CD = CHAR_DATA
MID = MOB_INDEX_DATA
OD = OBJ_DATA
AD = AFFECT_DATA
ED = EXTRA_DESCR_DATA
OID = OBJ_INDEX_DATA
RID = ROOM_INDEX_DATA
SF = SPEC_FUN
def add_member(ch: 'CHAR_DATA', member: 'CHAR_DATA') -> 'None':
    """ C function: void  add_member args((CHAR_DATA * ch, CHAR_DATA *member)) """
    pass

def clear_member(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  clear_member args((CHAR_DATA * ch)) """
    pass

def remove_member(member: 'CHAR_DATA') -> 'None':
    """ C function: void  remove_member args((CHAR_DATA * member)) """
    pass

def add_follower(ch: 'CHAR_DATA', master: 'CHAR_DATA') -> 'None':
    """ C function: void  add_follower args((CHAR_DATA * ch, CHAR_DATA *master)) """
    pass

def stop_follower(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_follower args((CHAR_DATA * ch)) """
    pass

def die_follower(ch: 'CHAR_DATA', name: 'str') -> 'None':
    """ C function: void  die_follower args((CHAR_DATA * ch, char *name)) """
    pass

def add_protector(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  add_protector args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def stop_protecting(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_protecting args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def add_cooperator(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  add_cooperator args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def stop_cooperating(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_cooperating args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def stop_linking(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_linking args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def add_linking(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  add_linking args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def add_rember(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  add_rember args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def stop_rember(ch: 'CHAR_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_rember args((CHAR_DATA * ch, CHAR_DATA *pch)) """
    pass

def add_rrember(ch: 'ROOM_INDEX_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  add_rrember args((ROOM_INDEX_DATA * ch, CHAR_DATA *pch)) """
    pass

def stop_rrember(ch: 'ROOM_INDEX_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_rrember args((ROOM_INDEX_DATA * ch, CHAR_DATA *pch)) """
    pass

def add_orember(ch: 'OBJ_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  add_orember args((OBJ_DATA * ch, CHAR_DATA *pch)) """
    pass

def stop_orember(ch: 'OBJ_DATA', pch: 'CHAR_DATA') -> 'None':
    """ C function: void  stop_orember args((OBJ_DATA * ch, CHAR_DATA *pch)) """
    pass

def remove_protector(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  remove_protector args((CHAR_DATA * ch)) """
    pass

def remove_cooperator(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  remove_cooperator args((CHAR_DATA * ch)) """
    pass

def remove_linking(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  remove_linking args((CHAR_DATA * ch)) """
    pass

def remove_rember(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  remove_rember args((CHAR_DATA * ch)) """
    pass

def remove_rrember(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  remove_rrember args((CHAR_DATA * ch)) """
    pass

def is_same_group(ach: 'CHAR_DATA', bch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  is_same_group args((CHAR_DATA * ach, CHAR_DATA *bch)) """
    pass

def is_note_to(ch: 'CHAR_DATA', pnote: 'NOTE_DATA') -> 'bool':
    """ C function: bool  is_note_to args((CHAR_DATA * ch, NOTE_DATA *pnote)) """
    pass

def skill_exp_level(exp: 'int') -> 'int':
    """ C function: int  skill_exp_level args((int exp)) """
    pass

def set_title(ch: 'CHAR_DATA', title: 'str') -> 'None':
    """ C function: void  set_title args((CHAR_DATA * ch, char *title)) """
    pass

def set_cname(ch: 'CHAR_DATA', cname: 'str') -> 'None':
    """ C function: void  set_cname args((CHAR_DATA * ch, char *cname)) """
    pass

def check_blind(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  check_blind args((CHAR_DATA * ch)) """
    pass

def scan_print(ch: 'CHAR_DATA', pexit: 'EXIT_DATA', door: 'int', level: 'int') -> 'None':
    """ C function: void  scan_print args((CHAR_DATA * ch, EXIT_DATA *pexit, int door, int level)) """
    pass

def scout(ch: 'CHAR_DATA', pexit: 'EXIT_DATA', to_room: 'ROOM_INDEX_DATA', sklv: 'int') -> 'None':
    """ C function: void  scout args((CHAR_DATA * ch, EXIT_DATA *pexit, ROOM_INDEX_DATA *to_room, int sklv)) """
    pass

def top10_check(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  top10_check args((CHAR_DATA * ch)) """
    pass

def move_char(ch: 'CHAR_DATA', door: 'int') -> 'None':
    """ C function: void  move_char args((CHAR_DATA * ch, int door)) """
    pass

def remove_obj(ch: 'CHAR_DATA', iWear: 'int', fReplace: 'bool') -> 'bool':
    """ C function: bool  remove_obj args((CHAR_DATA * ch, int iWear, bool fReplace)) """
    pass

def do_autoget(ch: 'CHAR_DATA', argument: 'str') -> 'int':
    """ C function: int  do_autoget(CHAR_DATA *ch, char *argument) """
    pass

def find_location(ch: 'CHAR_DATA', arg: 'str') -> 'ROOM_INDEX_DATA':
    """ C function: ROOM_INDEX_DATA * find_location args((CHAR_DATA * ch, char *arg)) """
    pass

def load_clans() -> 'None':
    """ C function: void  load_clans args((void)) """
    pass

def clan_update() -> 'None':
    """ C function: void  clan_update args((void)) """
    pass

def c_item_type_name(obj: 'OBJ_DATA') -> 'str':
    """ C function: char * c_item_type_name args((OBJ_DATA * obj)) """
    pass

def c_affect_loc_name(location: 'int') -> 'str':
    """ C function: char * c_affect_loc_name args((int location)) """
    pass

def c_affect_bit_name(vector: 'long long int') -> 'str':
    """ C function: char * c_affect_bit_name args((long long int vector)) """
    pass

def c_extra_bit_name(extra_flags: 'int') -> 'str':
    """ C function: char * c_extra_bit_name args((int extra_flags)) """
    pass

def strip_ansi(src: 'str', dup: 'str') -> 'None':
    """ C function: void  strip_ansi args((char *src, char *dup)) """
    pass

def close_socket(dclose: 'DESCRIPTOR_DATA') -> 'None':
    """ C function: void  close_socket args((DESCRIPTOR_DATA * dclose)) """
    pass

def write_to_buffer(d: 'DESCRIPTOR_DATA', txt: 'const char', length: 'int') -> 'None':
    """ C function: void  write_to_buffer args((DESCRIPTOR_DATA * d, const char *txt, int length)) """
    pass

def send_to_room(txt: 'const char', room: 'ROOM_INDEX_DATA') -> 'None':
    """ C function: void  send_to_room args((const char *txt, ROOM_INDEX_DATA *room)) """
    pass

def send_to_all_char(text: 'const char') -> 'None':
    """ C function: void  send_to_all_char args((const char *text)) """
    pass

def send_to_char(txt: 'const char', ch: 'CHAR_DATA') -> 'None':
    """ C function: void  send_to_char args((const char *txt, CHAR_DATA *ch)) """
    pass

def show_string(d: 'DESCRIPTOR_DATA', input: 'str') -> 'None':
    """ C function: void  show_string args((DESCRIPTOR_DATA * d, char *input)) """
    pass

def act(format: 'const char', ch: 'CHAR_DATA', arg1: 'const void', arg2: 'const void', type: 'int') -> 'None':
    """ C function: void  act args((const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type)) """
    pass

def check_parse_name(name: 'str') -> 'bool':
    """ C function: bool  check_parse_name args((char *name)) """
    pass

def boot_db() -> 'None':
    """ C function: void  boot_db args((void)) """
    pass

def interpolate1(level1: 'int', value1: 'int', level2: 'int', value2: 'int', level: 'int') -> 'int':
    """ C function: int           interpolate1(int level1, int value1, int level2, int value2, int level) """
    pass

def interpolate(level: 'int', value_00: 'int', value_42: 'int') -> 'int':
    """ C function: int           interpolate(int level, int value_00, int value_42) """
    pass

# long long int	atoll(char*);
def area_update() -> 'None':
    """ C function: void  area_update args((void)) """
    pass

def new_character(player: 'bool') -> 'CD':
    """ C function: CD * new_character args((bool player)) """
    pass

def new_affect() -> 'AD':
    """ C function: AD * new_affect args((void)) """
    pass

def new_call_out() -> 'CALL_OUT_DATA':
    """ C function: CALL_OUT_DATA * new_call_out args((void)) """
    pass

def new_rcall_out() -> 'RCALL_OUT_DATA':
    """ C function: RCALL_OUT_DATA * new_rcall_out args((void)) """
    pass

def free_call_out(rm: 'CALL_OUT_DATA') -> 'None':
    """ C function: void  free_call_out args((CALL_OUT_DATA * rm)) """
    pass

def free_rcall_out(rm: 'RCALL_OUT_DATA') -> 'None':
    """ C function: void  free_rcall_out args((RCALL_OUT_DATA * rm)) """
    pass

def new_r_affect() -> 'R_AFFECT_DATA':
    """ C function: R_AFFECT_DATA * new_r_affect args((void)) """
    pass

def new_object() -> 'OD':
    """ C function: OD * new_object args((void)) """
    pass

def new_extra_descr() -> 'ED':
    """ C function: ED * new_extra_descr args((void)) """
    pass

def create_mobile(pMobIndex: 'MOB_INDEX_DATA') -> 'CD':
    """ C function: CD * create_mobile args((MOB_INDEX_DATA * pMobIndex)) """
    pass

def create_object(pObjIndex: 'OBJ_INDEX_DATA', level: 'int') -> 'OD':
    """ C function: OD * create_object args((OBJ_INDEX_DATA * pObjIndex, int level)) """
    pass

def fcreate_object(pObjIndex: 'OBJ_INDEX_DATA', level: 'int') -> 'OD':
    """ C function: OD * fcreate_object args((OBJ_INDEX_DATA * pObjIndex, int level)) """
    pass

def clear_char(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  clear_char args((CHAR_DATA * ch)) """
    pass

def free_char(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  free_char args((CHAR_DATA * ch)) """
    pass

def get_extra_descr(name: 'const char', ed: 'EXTRA_DESCR_DATA') -> 'str':
    """ C function: char * get_extra_descr args((const char *name, EXTRA_DESCR_DATA *ed)) """
    pass

def get_pfxextra_descr(name: 'const char', ed: 'EXTRA_DESCR_DATA') -> 'str':
    """ C function: char * get_pfxextra_descr args((const char *name, EXTRA_DESCR_DATA *ed)) """
    pass

def get_mob_index(vnum: 'int') -> 'MID':
    """ C function: MID * get_mob_index args((int vnum)) """
    pass

def get_obj_index(vnum: 'int') -> 'OID':
    """ C function: OID * get_obj_index args((int vnum)) """
    pass

def get_room_index(vnum: 'int') -> 'RID':
    """ C function: RID * get_room_index args((int vnum)) """
    pass

def fread_letter(fp: 'FILE') -> 'str':
    """ C function: char  fread_letter args((FILE * fp)) """
    pass

def fread_string(fp: 'FILE', status: 'int') -> 'str':
    """ C function: char * fread_string args((FILE * fp, int *status)) """
    pass

def diw_fread_string(fp: 'FILE', status: 'int') -> 'str':
    """ C function: char * diw_fread_string args((FILE * fp, int *status)) """
    pass

def fread_to_eol(fp: 'FILE') -> 'None':
    """ C function: void  fread_to_eol args((FILE * fp)) """
    pass

def fread_word(fp: 'FILE', status: 'int') -> 'str':
    """ C function: char * fread_word args((FILE * fp, int *status)) """
    pass

def fread_alpha_word(fp: 'FILE', status: 'int') -> 'str':
    """ C function: char * fread_alpha_word args((FILE * fp, int *status)) """
    pass

def alloc_mem(sMem: 'int') -> 'None':
    """ C function: void * alloc_mem args((int sMem)) """
    pass

def alloc_perm(sMem: 'int') -> 'None':
    """ C function: void * alloc_perm args((int sMem)) """
    pass

def free_mem(pMem: 'None') -> 'None':
    """ C function: void  free_mem args((void *pMem)) """
    pass

def str_dup(str: 'const char') -> 'str':
    """ C function: char * str_dup args((const char *str)) """
    pass

def free_string(pstr: 'str') -> 'None':
    """ C function: void  free_string args((char *pstr)) """
    pass

def number_fuzzy(number: 'int') -> 'int':
    """ C function: int  number_fuzzy args((int number)) """
    pass

def number_range(from_: 'int', to: 'int') -> 'int':
    """ C function: int  number_range args((int from, int to)) """
    pass

def number_percent() -> 'int':
    """ C function: int  number_percent args((void)) """
    pass

def number_door() -> 'int':
    """ C function: int  number_door args((void)) """
    pass

def number_bits(width: 'int') -> 'int':
    """ C function: int  number_bits args((int width)) """
    pass

def number_mm() -> 'int':
    """ C function: int  number_mm args((void)) """
    pass

def dice(number: 'int', size: 'int') -> 'int':
    """ C function: int  dice args((int number, int size)) """
    pass

def interpolate(level: 'int', value_00: 'int', value_32: 'int') -> 'int':
    """ C function: int  interpolate args((int level, int value_00, int value_32)) """
    pass

def smash_tilde(str: 'str') -> 'None':
    """ C function: void  smash_tilde args((char *str)) """
    pass

def str_cmp(astr: 'const char', bstr: 'const char') -> 'bool':
    """ C function: bool  str_cmp args((const char *astr, const char *bstr)) """
    pass

def ip_str_cmp(astr: 'const char', bstr: 'const char') -> 'bool':
    """ C function: bool  ip_str_cmp args((const char *astr, const char *bstr)) """
    pass

def str_prefix(astr: 'const char', bstr: 'const char') -> 'bool':
    """ C function: bool  str_prefix args((const char *astr, const char *bstr)) """
    pass

def str_infix(astr: 'const char', bstr: 'const char') -> 'bool':
    """ C function: bool  str_infix args((const char *astr, const char *bstr)) """
    pass

def str_suffix(astr: 'const char', bstr: 'const char') -> 'bool':
    """ C function: bool  str_suffix args((const char *astr, const char *bstr)) """
    pass

def capitalize(str: 'const char') -> 'str':
    """ C function: char * capitalize args((const char *str)) """
    pass

def append_file(ch: 'CHAR_DATA', file: 'str', str: 'str') -> 'None':
    """ C function: void  append_file args((CHAR_DATA * ch, char *file, char *str)) """
    pass

def bug(str: 'const char', param: 'int') -> 'None':
    """ C function: void  bug args((const char *str, int param)) """
    pass

def bugf(fmt: 'str', arg1: '...') -> 'None':
    """ C function: void                        bugf(char *fmt, ...) """
    pass

def logf_envy(fmt: 'str', arg1: '...') -> 'None':
    """ C function: void                        logf_envy(char *fmt, ...) """
    pass

def log_string(str: 'const char') -> 'None':
    """ C function: void  log_string args((const char *str)) """
    pass

def tail_chain() -> 'None':
    """ C function: void  tail_chain args((void)) """
    pass

def string_edit(ch: 'CHAR_DATA', pString: 'str') -> 'None':
    """ C function: void  string_edit args((CHAR_DATA * ch, char **pString)) """
    pass

def string_append(ch: 'CHAR_DATA', pString: 'str') -> 'None':
    """ C function: void  string_append args((CHAR_DATA * ch, char **pString)) """
    pass

def string_replace(orig: 'str', old: 'str', new: 'str') -> 'str':
    """ C function: char * string_replace args((char *orig, char *old, char *new)) """
    pass

def string_add(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  string_add args((CHAR_DATA * ch, char *argument)) """
    pass

def format_string(oldstring: 'str') -> 'str':
    """ C function: char * format_string args((char *oldstring )) """
    pass

def first_arg(argument: 'str', arg_first: 'str', fCase: 'bool') -> 'str':
    """ C function: char * first_arg args((char *argument, char *arg_first, bool fCase)) """
    pass

def string_unpad(argument: 'str') -> 'str':
    """ C function: char * string_unpad args((char *argument)) """
    pass

def string_proper(argument: 'str') -> 'str':
    """ C function: char * string_proper args((char *argument)) """
    pass

def reset_area(pArea: 'AREA_DATA') -> 'None':
    """ C function: void  reset_area args((AREA_DATA * pArea)) """
    pass

def reset_room(pRoom: 'ROOM_INDEX_DATA') -> 'None':
    """ C function: void  reset_room args((ROOM_INDEX_DATA * pRoom)) """
    pass

def top10_load() -> 'None':
    """ C function: void  top10_load args((void)) """
    pass

def top10_save() -> 'None':
    """ C function: void  top10_save args((void)) """
    pass

def raw_kill(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool               raw_kill(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def set_fighting(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  set_fighting args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def self_damage(ch: 'CHAR_DATA', victim: 'CHAR_DATA', miss_times: 'int', no_dam_times: 'int', dam_times: 'int', times: 'int', dam: 'int', skill_name: 'str') -> 'None':
    """ C function: void                  self_damage(CHAR_DATA *ch, CHAR_DATA *victim, int miss_times, int no_dam_times, int dam_times, int times, int dam, char *skill_name) """
    pass

def disarm(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  disarm args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def trip(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  trip args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def check_mob_ai(mob: 'CHAR_DATA') -> 'CHAR_DATA':
    """ C function: CHAR_DATA           * check_mob_ai(CHAR_DATA *mob) """
    pass

def trigger_obj_hp(obj: 'OBJ_DATA', sn: 'int', level: 'int', ch: 'CHAR_DATA', vo: 'None') -> 'None':
    """ C function: void                  trigger_obj_hp(OBJ_DATA *obj, int sn, int level, CHAR_DATA *ch, void *vo) """
    pass

def violence_update() -> 'None':
    """ C function: void  violence_update args((void)) """
    pass

def multi_hit(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dt: 'int') -> 'None':
    """ C function: void  multi_hit args((CHAR_DATA * ch, CHAR_DATA *victim, int dt)) """
    pass

# int brief__damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
# int brief_damage  args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
# void _damage args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int wpn ) );
def damage(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int', msg_mode: 'int', dmg_mode: 'int') -> 'int':
    """ C function: int  damage args((CHAR_DATA * ch, CHAR_DATA *victim, int dam, int dt, int wpn, int msg_mode, int dmg_mode)) """
    pass

def magic__damage(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int') -> 'None':
    """ C function: void  magic__damage args((CHAR_DATA * ch, CHAR_DATA *victim, int dam, int dt, int wpn)) """
    pass

def magic_damage(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int') -> 'int':
    """ C function: int  magic_damage args((CHAR_DATA * ch, CHAR_DATA *victim, int dam, int dt, int wpn)) """
    pass

def is_real_npc(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  is_real_npc args((CHAR_DATA * ch)) """
    pass

def stop_fighting(ch: 'CHAR_DATA', fBoth: 'bool') -> 'None':
    """ C function: void  stop_fighting args((CHAR_DATA * ch, bool fBoth)) """
    pass

def update_pos(victim: 'CHAR_DATA') -> 'None':
    """ C function: void  update_pos args((CHAR_DATA * victim)) """
    pass

def check_killer(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'None':
    """ C function: void  check_killer args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def is_safe(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool  is_safe args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def licensed(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  licensed args((CHAR_DATA * ch)) """
    pass

def registered(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool  registered args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def death_cry(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  death_cry args((CHAR_DATA * ch)) """
    pass

def check_asshunt(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  check_asshunt args((CHAR_DATA * ch)) """
    pass

def brief_dam_message(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int', total: 'int', hit: 'int') -> 'None':
    """ C function: void  brief_dam_message args((CHAR_DATA * ch, CHAR_DATA *victim, int dam, int dt, int wpn, int total, int hit)) """
    pass

def dam_message(ch: 'CHAR_DATA', victim: 'CHAR_DATA', dam: 'int', dt: 'int', wpn: 'int') -> 'None':
    """ C function: void  dam_message args((CHAR_DATA * ch, CHAR_DATA *victim, int dam, int dt, int wpn)) """
    pass

def get_obj_wear2(ch: 'CHAR_DATA', argument: 'str') -> 'OBJ_DATA':
    """ C function: OBJ_DATA * get_obj_wear2(CHAR_DATA *ch, char *argument) """
    pass

def check_skill(ch: 'CHAR_DATA', sn: 'int', chance: 'int') -> 'bool':
    """ C function: bool       check_skill(CHAR_DATA *ch, int sn, int chance) """
    pass

def use_skill_exp(ch: 'CHAR_DATA', value: 'int') -> 'int':
    """ C function: int        use_skill_exp(CHAR_DATA *ch, int value) """
    pass

def regain_skill_exp(ch: 'CHAR_DATA', xp: 'int') -> 'None':
    """ C function: void       regain_skill_exp(CHAR_DATA *ch, int xp) """
    pass

def skill_difficulty(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        skill_difficulty(CHAR_DATA *ch, int sn) """
    pass

def check_int(i: 'int') -> 'bool':
    """ C function: bool       check_int(int i) """
    pass

def consented(ch: 'CHAR_DATA', corpse: 'OBJ_DATA') -> 'bool':
    """ C function: bool       consented(CHAR_DATA *ch, OBJ_DATA *corpse) """
    pass

def IS_HORSE(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool       IS_HORSE(CHAR_DATA *ch) """
    pass

def GET_AC(ch: 'CHAR_DATA') -> 'int':
    """ C function: int        GET_AC(CHAR_DATA *ch) """
    pass

def get_skill_exp(ch: 'CHAR_DATA') -> 'int':
    """ C function: int        get_skill_exp(CHAR_DATA *ch) """
    pass

def get_skill_level(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        get_skill_level(CHAR_DATA *ch, int sn) """
    pass

def skill_difficuty(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        skill_difficuty(CHAR_DATA *ch, int sn) """
    pass

def skill_max_percent(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        skill_max_percent(CHAR_DATA *ch, int sn) """
    pass

def skill_req_level(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        skill_req_level(CHAR_DATA *ch, int sn) """
    pass

def skill_preq_level(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        skill_preq_level(CHAR_DATA *ch, int sn) """
    pass

def get_skill_percent(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int        get_skill_percent(CHAR_DATA *ch, int sn) """
    pass

def gain_skill_level(ch: 'CHAR_DATA', sn: 'int', value: 'int') -> 'int':
    """ C function: int        gain_skill_level(CHAR_DATA *ch, int sn, int value) """
    pass

def gain_skill_percent(ch: 'CHAR_DATA', sn: 'int', value: 'int') -> 'int':
    """ C function: int        gain_skill_percent(CHAR_DATA *ch, int sn, int value) """
    pass

def WAIT_STATE(ch: 'CHAR_DATA', wait: 'int') -> 'None':
    """ C function: void       WAIT_STATE(CHAR_DATA *ch, int wait) """
    pass

def WAIT_MAGIC_STATE(ch: 'CHAR_DATA', wait: 'int') -> 'None':
    """ C function: void       WAIT_MAGIC_STATE(CHAR_DATA *ch, int wait) """
    pass

def add_call_out(call: 'CALL_OUT_DATA') -> 'None':
    """ C function: void       add_call_out(CALL_OUT_DATA *call) """
    pass

def is_qmark(ch: 'CHAR_DATA', qmark: 'str') -> 'QMARK':
    """ C function: QMARK    * is_qmark(CHAR_DATA *ch, char *qmark) """
    pass

def is_prefix_qmark(ch: 'CHAR_DATA', qmark: 'str') -> 'QMARK':
    """ C function: QMARK    * is_prefix_qmark(CHAR_DATA *ch, char *qmark) """
    pass

def is_flying(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool       is_flying(CHAR_DATA *ch) """
    pass

def room_clanlv(pROOM: 'ROOM_INDEX_DATA') -> 'int':
    """ C function: int        room_clanlv(ROOM_INDEX_DATA *pROOM) """
    pass

def is_chinese(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool       is_chinese(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def is_brief(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool       is_brief(CHAR_DATA *ch, CHAR_DATA *victim) """
    pass

def is_chinese_char(c: 'unsigned char', flag: 'bool') -> 'bool':
    """ C function: bool  is_chinese_char(unsigned char c, bool flag) """
    pass

def is_chinese_char(c: 'unsigned char', flag: 'bool') -> 'bool':
    """ C function: bool  is_chinese_char(unsigned char c, bool flag) """
    pass

def waste_dura(obj: 'OBJ_DATA') -> 'None':
    """ C function: void                   waste_dura(OBJ_DATA *obj) """
    pass

def waste_duraz(obj: 'OBJ_DATA') -> 'None':
    """ C function: void                   waste_duraz(OBJ_DATA *obj) """
    pass

def has_player(name: 'str') -> 'bool':
    """ C function: bool                   has_player(char *name) """
    pass

def repair_dura(obj: 'OBJ_DATA', value: 'int') -> 'None':
    """ C function: void                   repair_dura(OBJ_DATA *obj, int value) """
    pass

def meet_req_prime(ch: 'CHAR_DATA', sn: 'int', attr: 'int') -> 'bool':
    """ C function: bool  meet_req_prime args((CHAR_DATA * ch, int sn, int attr)) """
    pass

def meet_req_second(ch: 'CHAR_DATA', sn: 'int', attr: 'int') -> 'bool':
    """ C function: bool  meet_req_second args((CHAR_DATA * ch, int sn, int attr)) """
    pass

def GET_DODGE(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  GET_DODGE args((CHAR_DATA * ch)) """
    pass

def is_broken(obj: 'OBJ_DATA') -> 'bool':
    """ C function: bool                   is_broken(OBJ_DATA *obj) """
    pass

def indestructible(obj: 'OBJ_DATA') -> 'bool':
    """ C function: bool                   indestructible(OBJ_DATA *obj) """
    pass

def GET_SVS(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  GET_SVS args((CHAR_DATA * ch)) """
    pass

def NEW_GET_SVS(ch: 'CHAR_DATA', type: 'int') -> 'int':
    """ C function: int  NEW_GET_SVS args((CHAR_DATA * ch, int type)) """
    pass

def get_trust(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_trust args((CHAR_DATA * ch)) """
    pass

def get_age(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_age args((CHAR_DATA * ch)) """
    pass

def get_curr_dura(obj: 'OBJ_DATA') -> 'int':
    """ C function: int  get_curr_dura args((OBJ_DATA * obj)) """
    pass

def get_max_dura(obj: 'OBJ_DATA') -> 'int':
    """ C function: int  get_max_dura args((OBJ_DATA * obj)) """
    pass

def get_max_skill_exp(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_max_skill_exp args((CHAR_DATA * ch)) """
    pass

def get_eff_skill_exp(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_eff_skill_exp args((CHAR_DATA * ch)) """
    pass

def gain_skill_exp(ch: 'CHAR_DATA', xp: 'int') -> 'None':
    """ C function: void  gain_skill_exp args((CHAR_DATA * ch, int xp)) """
    pass

def get_curr_str(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_curr_str args((CHAR_DATA * ch)) """
    pass

def get_curr_int(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_curr_int args((CHAR_DATA * ch)) """
    pass

def get_curr_wis(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_curr_wis args((CHAR_DATA * ch)) """
    pass

def get_curr_dex(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_curr_dex args((CHAR_DATA * ch)) """
    pass

def get_curr_con(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_curr_con args((CHAR_DATA * ch)) """
    pass

def get_curr_size(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_curr_size args((CHAR_DATA * ch)) """
    pass

def get_eff_str(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_eff_str args((CHAR_DATA * ch)) """
    pass

def get_eff_int(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_eff_int args((CHAR_DATA * ch)) """
    pass

def get_eff_wis(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_eff_wis args((CHAR_DATA * ch)) """
    pass

def get_eff_dex(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_eff_dex args((CHAR_DATA * ch)) """
    pass

def get_eff_con(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_eff_con args((CHAR_DATA * ch)) """
    pass

def get_max_str(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_max_str args((CHAR_DATA * ch)) """
    pass

def get_max_int(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_max_int args((CHAR_DATA * ch)) """
    pass

def get_max_wis(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_max_wis args((CHAR_DATA * ch)) """
    pass

def get_max_dex(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_max_dex args((CHAR_DATA * ch)) """
    pass

def get_max_con(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  get_max_con args((CHAR_DATA * ch)) """
    pass

def get_hitroll(ch: 'CHAR_DATA', wpn: 'int') -> 'int':
    """ C function: int  get_hitroll args((CHAR_DATA * ch, int wpn)) """
    pass

def get_damroll(ch: 'CHAR_DATA', wpn: 'int') -> 'int':
    """ C function: int  get_damroll args((CHAR_DATA * ch, int wpn)) """
    pass

def get_vrace_hrq(victim: 'CHAR_DATA', wpn: 'OBJ_DATA') -> 'int':
    """ C function: int  get_vrace_hrq args((CHAR_DATA * victim, OBJ_DATA *wpn)) """
    pass

def get_vrace_hrp(victim: 'CHAR_DATA', wpn: 'OBJ_DATA') -> 'int':
    """ C function: int  get_vrace_hrp args((CHAR_DATA * victim, OBJ_DATA *wpn)) """
    pass

def get_vrace_drq(victim: 'CHAR_DATA', wpn: 'OBJ_DATA') -> 'int':
    """ C function: int  get_vrace_drq args((CHAR_DATA * victim, OBJ_DATA *wpn)) """
    pass

def get_vrace_drp(victim: 'CHAR_DATA', wpn: 'OBJ_DATA') -> 'int':
    """ C function: int  get_vrace_drp args((CHAR_DATA * victim, OBJ_DATA *wpn)) """
    pass

def can_carry_n(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  can_carry_n args((CHAR_DATA * ch)) """
    pass

def can_carry_w(ch: 'CHAR_DATA') -> 'int':
    """ C function: int  can_carry_w args((CHAR_DATA * ch)) """
    pass

def is_name(str: 'const char', namelist: 'str') -> 'bool':
    """ C function: bool  is_name args((const char *str, char *namelist)) """
    pass

def is_pfxname(str: 'const char', namelist: 'str') -> 'bool':
    """ C function: bool  is_pfxname args((const char *str, char *namelist)) """
    pass

def affect_to_room(ch: 'ROOM_INDEX_DATA', paf: 'R_AFFECT_DATA') -> 'None':
    """ C function: void  affect_to_room args((ROOM_INDEX_DATA * ch, R_AFFECT_DATA *paf)) """
    pass

def r_affect_remove(ch: 'ROOM_INDEX_DATA', paf: 'R_AFFECT_DATA') -> 'None':
    """ C function: void  r_affect_remove args((ROOM_INDEX_DATA * ch, R_AFFECT_DATA *paf)) """
    pass

def r_affect_strip(ch: 'ROOM_INDEX_DATA', sn: 'int') -> 'None':
    """ C function: void  r_affect_strip args((ROOM_INDEX_DATA * ch, int sn)) """
    pass

def r_is_affected(ch: 'ROOM_INDEX_DATA', sn: 'int') -> 'bool':
    """ C function: bool  r_is_affected args((ROOM_INDEX_DATA * ch, int sn)) """
    pass

def r_affect_join(ch: 'ROOM_INDEX_DATA', paf: 'R_AFFECT_DATA') -> 'None':
    """ C function: void  r_affect_join args((ROOM_INDEX_DATA * ch, R_AFFECT_DATA *paf)) """
    pass

def affect_to_char(ch: 'CHAR_DATA', paf: 'AFFECT_DATA') -> 'None':
    """ C function: void  affect_to_char args((CHAR_DATA * ch, AFFECT_DATA *paf)) """
    pass

def affect_remove(ch: 'CHAR_DATA', paf: 'AFFECT_DATA') -> 'None':
    """ C function: void  affect_remove args((CHAR_DATA * ch, AFFECT_DATA *paf)) """
    pass

def affect_strip(ch: 'CHAR_DATA', sn: 'int') -> 'None':
    """ C function: void  affect_strip args((CHAR_DATA * ch, int sn)) """
    pass

def is_affected(ch: 'CHAR_DATA', sn: 'int') -> 'bool':
    """ C function: bool  is_affected args((CHAR_DATA * ch, int sn)) """
    pass

def affected_value(ch: 'CHAR_DATA', sn: 'int', apply_type: 'int') -> 'int':
    """ C function: int  affected_value args((CHAR_DATA * ch, int sn, int apply_type)) """
    pass

def affect_join(ch: 'CHAR_DATA', paf: 'AFFECT_DATA') -> 'None':
    """ C function: void  affect_join args((CHAR_DATA * ch, AFFECT_DATA *paf)) """
    pass

def char_from_room(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  char_from_room args((CHAR_DATA * ch)) """
    pass

def char_from_room2(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  char_from_room2 args((CHAR_DATA * ch)) """
    pass

def char_to_room(ch: 'CHAR_DATA', pRoomIndex: 'ROOM_INDEX_DATA') -> 'None':
    """ C function: void  char_to_room args((CHAR_DATA * ch, ROOM_INDEX_DATA *pRoomIndex)) """
    pass

def obj_to_char(obj: 'OBJ_DATA', ch: 'CHAR_DATA') -> 'None':
    """ C function: void  obj_to_char args((OBJ_DATA * obj, CHAR_DATA *ch)) """
    pass

def obj_from_char(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  obj_from_char args((OBJ_DATA * obj)) """
    pass

def apply_ac(obj: 'OBJ_DATA', iWear: 'int') -> 'int':
    """ C function: int  apply_ac args((OBJ_DATA * obj, int iWear)) """
    pass

def get_eq_char(ch: 'CHAR_DATA', iWear: 'int') -> 'OD':
    """ C function: OD * get_eq_char args((CHAR_DATA * ch, int iWear)) """
    pass

def equip_char(ch: 'CHAR_DATA', obj: 'OBJ_DATA', iWear: 'int') -> 'None':
    """ C function: void  equip_char args((CHAR_DATA * ch, OBJ_DATA *obj, int iWear)) """
    pass

def unequip_char(ch: 'CHAR_DATA', obj: 'OBJ_DATA') -> 'None':
    """ C function: void  unequip_char args((CHAR_DATA * ch, OBJ_DATA *obj)) """
    pass

def equip_char2(ch: 'CHAR_DATA', obj: 'OBJ_DATA', iWear: 'int') -> 'None':
    """ C function: void  equip_char2 args((CHAR_DATA * ch, OBJ_DATA *obj, int iWear)) """
    pass

def unequip_char2(ch: 'CHAR_DATA', obj: 'OBJ_DATA') -> 'None':
    """ C function: void  unequip_char2 args((CHAR_DATA * ch, OBJ_DATA *obj)) """
    pass

def count_obj_list(obj: 'OBJ_INDEX_DATA', list: 'OBJ_DATA') -> 'int':
    """ C function: int  count_obj_list args((OBJ_INDEX_DATA * obj, OBJ_DATA *list)) """
    pass

def obj_from_room(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  obj_from_room args((OBJ_DATA * obj)) """
    pass

def obj_to_room(obj: 'OBJ_DATA', pRoomIndex: 'ROOM_INDEX_DATA') -> 'None':
    """ C function: void  obj_to_room args((OBJ_DATA * obj, ROOM_INDEX_DATA *pRoomIndex)) """
    pass

def obj_to_obj(obj: 'OBJ_DATA', obj_to: 'OBJ_DATA') -> 'None':
    """ C function: void  obj_to_obj args((OBJ_DATA * obj, OBJ_DATA *obj_to)) """
    pass

def obj_from_obj(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  obj_from_obj args((OBJ_DATA * obj)) """
    pass

def extract_obj(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  extract_obj args((OBJ_DATA * obj)) """
    pass

def extract_char(ch: 'CHAR_DATA', fPull: 'bool') -> 'None':
    """ C function: void  extract_char args((CHAR_DATA * ch, bool fPull)) """
    pass

def get_char_room(ch: 'CHAR_DATA', argument: 'str') -> 'CD':
    """ C function: CD * get_char_room args((CHAR_DATA * ch, char *argument)) """
    pass

def get_char_world(ch: 'CHAR_DATA', argument: 'str') -> 'CD':
    """ C function: CD * get_char_world args((CHAR_DATA * ch, char *argument)) """
    pass

def get_char_world2(ch: 'CHAR_DATA', argument: 'str') -> 'CD':
    """ C function: CD * get_char_world2 args((CHAR_DATA * ch, char *argument)) """
    pass

def get_obj_type(pObjIndexData: 'OBJ_INDEX_DATA') -> 'OD':
    """ C function: OD * get_obj_type args((OBJ_INDEX_DATA * pObjIndexData)) """
    pass

def get_obj_list(ch: 'CHAR_DATA', argument: 'str', list: 'OBJ_DATA') -> 'OD':
    """ C function: OD * get_obj_list args((CHAR_DATA * ch, char *argument, OBJ_DATA *list)) """
    pass

def get_obj_carry(ch: 'CHAR_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_obj_carry args((CHAR_DATA * ch, char *argument)) """
    pass

def get_obj_wear(ch: 'CHAR_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_obj_wear args((CHAR_DATA * ch, char *argument)) """
    pass

def get_char_obj(ch: 'CHAR_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_char_obj args((CHAR_DATA * ch, char *argument)) """
    pass

def get_obj_here(ch: 'CHAR_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_obj_here args((CHAR_DATA * ch, char *argument)) """
    pass

def get_room_obj(pRoomIndex: 'ROOM_INDEX_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_room_obj args((ROOM_INDEX_DATA * pRoomIndex, char *argument)) """
    pass

def get_obj_world(ch: 'CHAR_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_obj_world args((CHAR_DATA * ch, char *argument)) """
    pass

def get_obj_index_vnum(vnum: 'int') -> 'OD':
    """ C function: OD * get_obj_index_vnum args((int vnum)) """
    pass

def count_obj_index_vnum(vnum: 'int') -> 'int':
    """ C function: int  count_obj_index_vnum args((int vnum)) """
    pass

def check_using_wp(ch: 'CHAR_DATA', dt: 'int', wield: 'int') -> 'OD':
    """ C function: OD * check_using_wp args((CHAR_DATA * ch, int dt, int wield)) """
    pass

def check_using_wp2(ch: 'CHAR_DATA', dt: 'int') -> 'OD':
    """ C function: OD * check_using_wp2 args((CHAR_DATA * ch, int dt)) """
    pass

def create_money(amount: 'int') -> 'OD':
    """ C function: OD * create_money args((int amount)) """
    pass

def get_obj_number(obj: 'OBJ_DATA') -> 'int':
    """ C function: int  get_obj_number args((OBJ_DATA * obj)) """
    pass

def get_obj_weight(obj: 'OBJ_DATA') -> 'int':
    """ C function: int  get_obj_weight args((OBJ_DATA * obj)) """
    pass

def room_is_dark(pRoomIndex: 'ROOM_INDEX_DATA') -> 'bool':
    """ C function: bool  room_is_dark args((ROOM_INDEX_DATA * pRoomIndex)) """
    pass

# bool room_is_private args( ( ROOM_INDEX_DATA *pRoomIndex ) );
def room_is_private(ch: 'CHAR_DATA', pRoomIndex: 'ROOM_INDEX_DATA') -> 'bool':
    """ C function: bool  room_is_private args((CHAR_DATA * ch, ROOM_INDEX_DATA *pRoomIndex)) """
    pass

def can_see(ch: 'CHAR_DATA', victim: 'CHAR_DATA') -> 'bool':
    """ C function: bool  can_see args((CHAR_DATA * ch, CHAR_DATA *victim)) """
    pass

def can_see_obj(ch: 'CHAR_DATA', obj: 'OBJ_DATA') -> 'bool':
    """ C function: bool  can_see_obj args((CHAR_DATA * ch, OBJ_DATA *obj)) """
    pass

def can_drop_obj(ch: 'CHAR_DATA', obj: 'OBJ_DATA') -> 'bool':
    """ C function: bool  can_drop_obj args((CHAR_DATA * ch, OBJ_DATA *obj)) """
    pass

def item_type_name(obj: 'OBJ_DATA') -> 'str':
    """ C function: char * item_type_name args((OBJ_DATA * obj)) """
    pass

def affect_loc_name(location: 'int') -> 'str':
    """ C function: char * affect_loc_name args((int location)) """
    pass

def affect_bit_name(vector: 'long long int') -> 'str':
    """ C function: char * affect_bit_name args((long long int vector)) """
    pass

def r_affect_bit_name(vector: 'long long int') -> 'str':
    """ C function: char * r_affect_bit_name args((long long int vector)) """
    pass

def wear_bit_name(vector: 'int') -> 'str':
    """ C function: char * wear_bit_name args((int vector)) """
    pass

def act_bit_name(vector: 'long long int') -> 'str':
    """ C function: char * act_bit_name args((long long int vector)) """
    pass

def special_bit_name(vector: 'int') -> 'str':
    """ C function: char * special_bit_name args((int vector)) """
    pass

def extra_bit_name(extra_flags: 'int') -> 'str':
    """ C function: char * extra_bit_name args((int extra_flags)) """
    pass

def get_char(ch: 'CHAR_DATA') -> 'CD':
    """ C function: CD * get_char args((CHAR_DATA * ch)) """
    pass

def longstring(ch: 'CHAR_DATA', argument: 'str') -> 'bool':
    """ C function: bool  longstring args((CHAR_DATA * ch, char *argument)) """
    pass

def authorized(ch: 'CHAR_DATA', skllnm: 'str') -> 'bool':
    """ C function: bool  authorized args((CHAR_DATA * ch, char *skllnm)) """
    pass

def end_of_game() -> 'None':
    """ C function: void  end_of_game args((void)) """
    pass

def race_lookup(race: 'const char') -> 'int':
    """ C function: int  race_lookup args((const char *race)) """
    pass

def material_lookup(race: 'const char') -> 'OBJ_INDEX_DATA':
    """ C function: OBJ_INDEX_DATA * material_lookup args((const char *race)) """
    pass

def affect_lookup(race: 'const char') -> 'int':
    """ C function: int  affect_lookup args((const char *race)) """
    pass

def check_exit(ch: 'CHAR_DATA', check: 'int') -> 'int':
    """ C function: int  check_exit args((CHAR_DATA * ch, int check)) """
    pass

def interpret(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  interpret args((CHAR_DATA * ch, char *argument)) """
    pass

def rinterpret(room: 'ROOM_INDEX_DATA', actor: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  rinterpret args((ROOM_INDEX_DATA * room, CHAR_DATA *actor, char *argument)) """
    pass

def ointerpret(room: 'OBJ_DATA', actor: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  ointerpret args((OBJ_DATA * room, CHAR_DATA *actor, char *argument)) """
    pass

def is_number(arg: 'str') -> 'bool':
    """ C function: bool  is_number args((char *arg)) """
    pass

def number_argument(argument: 'str', arg: 'str') -> 'int':
    """ C function: int  number_argument args((char *argument, char *arg)) """
    pass

def one_argument(argument: 'str', arg_first: 'str') -> 'str':
    """ C function: char * one_argument args((char *argument, char *arg_first)) """
    pass

def IS_SWITCHED(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  IS_SWITCHED args((CHAR_DATA * ch)) """
    pass

def check_social(ch: 'CHAR_DATA', command: 'str', argument: 'str') -> 'bool':
    """ C function: bool  check_social args((CHAR_DATA * ch, char *command, char *argument)) """
    pass

def free_affect(pAf: 'AFFECT_DATA') -> 'None':
    """ C function: void                            free_affect(AFFECT_DATA *pAf) """
    pass

def new_qmark() -> 'QMARK':
    """ C function: QMARK * new_qmark args((void)) """
    pass

def new_group() -> 'GROUP_DATA':
    """ C function: GROUP_DATA * new_group args((void)) """
    pass

def free_group(g: 'GROUP_DATA') -> 'None':
    """ C function: void                            free_group(GROUP_DATA *g) """
    pass

def new_qmark_string() -> 'QMARK_STRING':
    """ C function: QMARK_STRING * new_qmark_string args((void)) """
    pass

def qmark_lookup(str: 'const char') -> 'str':
    """ C function: char * qmark_lookup args((const char *str)) """
    pass

def new_hiddenpower() -> 'OBJ_HP':
    """ C function: OBJ_HP * new_hiddenpower args((void)) """
    pass

def new_obj_vrace() -> 'OBJ_VRACE':
    """ C function: OBJ_VRACE * new_obj_vrace args((void)) """
    pass

def new_auction_data() -> 'AUC_DATA':
    """ C function: AUC_DATA                      * new_auction_data(void) """
    pass

def free_auction_data(pAuc: 'AUC_DATA') -> 'None':
    """ C function: void                            free_auction_data(AUC_DATA *pAuc) """
    pass

def free_r_affect(pAf: 'R_AFFECT_DATA') -> 'None':
    """ C function: void  free_r_affect(R_AFFECT_DATA *pAf) """
    pass

def new_saves_spell(pp: 'int', victim: 'CHAR_DATA', type: 'int') -> 'int':
    """ C function: int                   new_saves_spell(int pp, CHAR_DATA *victim, int type) """
    pass

def skill_lookup(name: 'const char') -> 'int':
    """ C function: int  skill_lookup args((const char *name)) """
    pass

def weakness_percent(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int  weakness_percent args((CHAR_DATA * ch, int sn)) """
    pass

def weakness_damage(ch: 'CHAR_DATA', sn: 'int') -> 'int':
    """ C function: int  weakness_damage args((CHAR_DATA * ch, int sn)) """
    pass

def saves_spell(level: 'int', victim: 'CHAR_DATA') -> 'int':
    """ C function: int  saves_spell args((int level, CHAR_DATA *victim)) """
    pass

def old_saves_spell(pp: 'int', victim: 'CHAR_DATA', type: 'int') -> 'int':
    """ C function: int                   old_saves_spell(int pp, CHAR_DATA *victim, int type) """
    pass

def dm_effect(ch: 'CHAR_DATA') -> 'None':
    """ C function: void                  dm_effect(CHAR_DATA *ch) """
    pass

def fall_effect(ch: 'CHAR_DATA') -> 'None':
    """ C function: void                  fall_effect(CHAR_DATA *ch) """
    pass

def do_autopower(ch: 'CHAR_DATA', arg: 'str') -> 'bool':
    """ C function: bool                    do_autopower(CHAR_DATA *ch, char *arg) """
    pass

def obj_cast_spell(sn: 'int', level: 'int', ch: 'CHAR_DATA', victim: 'CHAR_DATA', obj: 'OBJ_DATA') -> 'None':
    """ C function: void  obj_cast_spell args((int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)) """
    pass

def room_cast_spell(sn: 'int', level: 'int', victim: 'CHAR_DATA', ch: 'CHAR_DATA', obj: 'ROOM_INDEX_DATA') -> 'None':
    """ C function: void  room_cast_spell args((int sn, int level, CHAR_DATA *victim, CHAR_DATA *ch, ROOM_INDEX_DATA *obj)) """
    pass

def get_powergem_carry(ch: 'CHAR_DATA', argument: 'str') -> 'OD':
    """ C function: OD * get_powergem_carry args((CHAR_DATA * ch, char *argument)) """
    pass

def strstr(s1: 'const char', s2: 'const char') -> 'str':
    """ C function: char * strstr args((const char *s1, const char *s2)) """
    pass

def check_arrival(ch: 'CHAR_DATA', f: 'bool') -> 'None':
    """ C function: void                       check_arrival(CHAR_DATA *ch, bool f) """
    pass

def mprog_wordlist_check(arg: 'str', mob: 'CHAR_DATA', actor: 'CHAR_DATA', object: 'OBJ_DATA', vo: 'None', type: 'int') -> 'None':
    """ C function: void  mprog_wordlist_check args((char *arg, CHAR_DATA *mob, CHAR_DATA *actor, OBJ_DATA *object, void *vo, int type)) """
    pass

def mprog_percent_check(mob: 'CHAR_DATA', actor: 'CHAR_DATA', object: 'OBJ_DATA', vo: 'None', type: 'int') -> 'None':
    """ C function: void  mprog_percent_check args((CHAR_DATA * mob, CHAR_DATA *actor, OBJ_DATA *object, void *vo, int type)) """
    pass

def oprog_percent_check(object: 'OBJ_DATA', actor: 'CHAR_DATA', victim: 'CHAR_DATA', vo: 'None', type: 'int') -> 'None':
    """ C function: void  oprog_percent_check args((OBJ_DATA * object, CHAR_DATA *actor, CHAR_DATA *victim, void *vo, int type)) """
    pass

# void	    mprog_act_trigger	     args ( ( char* buf, CHAR_DATA* mob,
# CHAR_DATA* ch, OBJ_DATA* obj,
# void* vo ) );
# void mprog_act_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob );
def mprog_act_trigger(txt: 'str', ch: 'CHAR_DATA', mob: 'CHAR_DATA') -> 'bool':
    """ C function: bool  mprog_act_trigger(char *txt, CHAR_DATA *ch, CHAR_DATA *mob) """
    pass

def oprog_act_trigger(txt: 'str', ch: 'CHAR_DATA', mob: 'OBJ_DATA') -> 'None':
    """ C function: void  oprog_act_trigger(char *txt, CHAR_DATA *ch, OBJ_DATA *mob) """
    pass

def mprog_bribe_trigger(mob: 'CHAR_DATA', ch: 'CHAR_DATA', amount: 'int') -> 'None':
    """ C function: void  mprog_bribe_trigger args((CHAR_DATA * mob, CHAR_DATA *ch, int amount)) """
    pass

def mprog_entry_trigger(mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_entry_trigger args((CHAR_DATA * mob)) """
    pass

def mprog_entry_greet_trigger(mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_entry_greet_trigger args((CHAR_DATA * mob)) """
    pass

def mprog_give_trigger(mob: 'CHAR_DATA', ch: 'CHAR_DATA', obj: 'OBJ_DATA') -> 'None':
    """ C function: void  mprog_give_trigger args((CHAR_DATA * mob, CHAR_DATA *ch, OBJ_DATA *obj)) """
    pass

def mprog_greet_trigger(mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_greet_trigger args((CHAR_DATA * mob)) """
    pass

def mprog_fight_trigger(mob: 'CHAR_DATA', ch: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_fight_trigger args((CHAR_DATA * mob, CHAR_DATA *ch)) """
    pass

def mprog_hurt_trigger(mob: 'CHAR_DATA', ch: 'CHAR_DATA', dt: 'int') -> 'None':
    """ C function: void                            mprog_hurt_trigger(CHAR_DATA *mob, CHAR_DATA *ch, int dt) """
    pass

def mprog_hitprcnt_trigger(mob: 'CHAR_DATA', ch: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_hitprcnt_trigger args((CHAR_DATA * mob, CHAR_DATA *ch)) """
    pass

def mprog_death_trigger(actor: 'CHAR_DATA', mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_death_trigger args((CHAR_DATA * actor, CHAR_DATA *mob)) """
    pass

def mprog_random_trigger(mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_random_trigger args((CHAR_DATA * mob)) """
    pass

def oprog_rand_trigger(obj: 'OBJ_DATA') -> 'None':
    """ C function: void                            oprog_rand_trigger(OBJ_DATA *obj) """
    pass

def oprog_tick_trigger(obj: 'OBJ_DATA') -> 'None':
    """ C function: void                            oprog_tick_trigger(OBJ_DATA *obj) """
    pass

def mprog_tick_trigger(mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_tick_trigger args((CHAR_DATA * mob)) """
    pass

def mprog_kill_trigger(mob: 'CHAR_DATA', ch: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_kill_trigger args((CHAR_DATA * mob, CHAR_DATA *ch)) """
    pass

def mprog_repop_trigger(mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_repop_trigger args((CHAR_DATA * mob)) """
    pass

def oprog_repop_trigger(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  oprog_repop_trigger args((OBJ_DATA * obj)) """
    pass

def oprog_fight_trigger(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  oprog_fight_trigger args((OBJ_DATA * obj)) """
    pass

def oprog_kill_trigger(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  oprog_kill_trigger args((OBJ_DATA * obj)) """
    pass

def oprog_cmd_trigger(obj: 'OBJ_DATA', ch: 'CHAR_DATA', txt: 'str') -> 'bool':
    """ C function: bool                            oprog_cmd_trigger(OBJ_DATA *obj, CHAR_DATA *ch, char *txt) """
    pass

def oprog_disappear_trigger(obj: 'OBJ_DATA') -> 'None':
    """ C function: void  oprog_disappear_trigger args((OBJ_DATA * obj)) """
    pass

def mprog_speech_trigger(txt: 'str', mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_speech_trigger args((char *txt, CHAR_DATA *mob)) """
    pass

def mprog_talk_trigger(txt: 'str', ch: 'CHAR_DATA', mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_talk_trigger args((char *txt, CHAR_DATA *ch, CHAR_DATA *mob)) """
    pass

def mprog_defun_trigger(txt: 'str', ch: 'CHAR_DATA', mob: 'CHAR_DATA') -> 'None':
    """ C function: void  mprog_defun_trigger args((char *txt, CHAR_DATA *ch, CHAR_DATA *mob)) """
    pass

def rprog_entry_trigger(room: 'ROOM_INDEX_DATA', ch: 'CHAR_DATA', door: 'int') -> 'None':
    """ C function: void  rprog_entry_trigger args((ROOM_INDEX_DATA * room, CHAR_DATA *ch, int door)) """
    pass

def rprog_trap_trigger(room: 'ROOM_INDEX_DATA', ch: 'CHAR_DATA', door: 'int') -> 'None':
    """ C function: void  rprog_trap_trigger args((ROOM_INDEX_DATA * room, CHAR_DATA *ch, int door)) """
    pass

def rprog_leave_trigger(room: 'ROOM_INDEX_DATA', ch: 'CHAR_DATA', door: 'int') -> 'None':
    """ C function: void  rprog_leave_trigger args((ROOM_INDEX_DATA * room, CHAR_DATA *ch, int door)) """
    pass

def rprog_rand_trigger(room: 'RPROG_DATA') -> 'None':
    """ C function: void  rprog_rand_trigger args((RPROG_DATA * room)) """
    pass

def rprog_time_trigger(room: 'RPROG_DATA') -> 'None':
    """ C function: void  rprog_time_trigger args((RPROG_DATA * room)) """
    pass

def rprog_cmd_trigger(room: 'ROOM_INDEX_DATA', ch: 'CHAR_DATA', txt: 'str') -> 'bool':
    """ C function: bool        rprog_cmd_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *txt) """
    pass

def rprog_mask_trigger(room: 'ROOM_INDEX_DATA', ch: 'CHAR_DATA', txt: 'str') -> 'bool':
    """ C function: bool        rprog_mask_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *txt) """
    pass

def rprog_defun_trigger(room: 'ROOM_INDEX_DATA', ch: 'CHAR_DATA', txt: 'str') -> 'None':
    """ C function: void        rprog_defun_trigger(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *txt) """
    pass

def rget_char_room(room: 'ROOM_INDEX_DATA', argument: 'str') -> 'CHAR_DATA':
    """ C function: CHAR_DATA * rget_char_room(ROOM_INDEX_DATA *room, char *argument) """
    pass

def set_qmark(arg: 'str') -> 'QMARK':
    """ C function: QMARK * set_qmark args((char *arg)) """
    pass

def del_qmark(qmark: 'QMARK') -> 'None':
    """ C function: void  del_qmark args((QMARK * qmark)) """
    pass

def room_find_char(room: 'ROOM_INDEX_DATA', name: 'str') -> 'CHAR_DATA':
    """ C function: CHAR_DATA * room_find_char args((ROOM_INDEX_DATA * room, char *name)) """
    pass

def world_find_char(room: 'ROOM_INDEX_DATA', name: 'str') -> 'CHAR_DATA':
    """ C function: CHAR_DATA * world_find_char args((ROOM_INDEX_DATA * room, char *name)) """
    pass

def do_mpgroup(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  do_mpgroup args((CHAR_DATA * ch, char *argument)) """
    pass

def check_qmark(target: 'CHAR_DATA', arg: 'str') -> 'bool':
    """ C function: bool  check_qmark args((CHAR_DATA * target, char *arg)) """
    pass

def obj_find(pobj: 'OBJ_DATA', ch: 'CHAR_DATA', argument: 'str') -> 'OBJ_DATA':
    """ C function: OBJ_DATA * obj_find args((OBJ_DATA * pobj, CHAR_DATA *ch, char *argument)) """
    pass

def save_char_obj(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  save_char_obj args((CHAR_DATA * ch)) """
    pass

def load_char_obj(d: 'DESCRIPTOR_DATA', name: 'str') -> 'bool':
    """ C function: bool  load_char_obj args((DESCRIPTOR_DATA * d, char *name)) """
    pass

def load_horse_obj(d: 'DESCRIPTOR_DATA', name: 'str') -> 'CHAR_DATA':
    """ C function: CHAR_DATA * load_horse_obj args((DESCRIPTOR_DATA * d, char *name)) """
    pass

def save_horse_obj(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  save_horse_obj args((CHAR_DATA * ch)) """
    pass

def initial(str: 'const char') -> 'str':
    """ C function: char * initial args((const char *str)) """
    pass

def fwrite_bank_obj(ch: 'CHAR_DATA', obj: 'OBJ_DATA', fp: 'FILE', iNest: 'int') -> 'None':
    """ C function: void  fwrite_bank_obj args((CHAR_DATA * ch, OBJ_DATA *obj, FILE *fp, int iNest)) """
    pass

def fread_bank_obj(ch: 'CHAR_DATA', fp: 'FILE') -> 'int':
    """ C function: int  fread_bank_obj args((CHAR_DATA * ch, FILE *fp)) """
    pass

def fwrite_newbank_obj(ch: 'CHAR_DATA', obj: 'OBJ_DATA', fp: 'FILE', iNest: 'int') -> 'None':
    """ C function: void  fwrite_newbank_obj args((CHAR_DATA * ch, OBJ_DATA *obj, FILE *fp, int iNest)) """
    pass

def fread_newbank_obj(ch: 'BANK_DATA', fp: 'FILE') -> 'int':
    """ C function: int  fread_newbank_obj args((BANK_DATA * ch, FILE *fp)) """
    pass

def spec_lookup(name: 'const char') -> 'SF':
    """ C function: SF * spec_lookup args((const char *name)) """
    pass

def spec_string(fun: 'SPEC_FUN') -> 'str':
    """ C function: char * spec_string args((SPEC_FUN * fun)) """
    pass

def advance_level(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  advance_level args((CHAR_DATA * ch)) """
    pass

def demote_level(ch: 'CHAR_DATA') -> 'None':
    """ C function: void  demote_level args((CHAR_DATA * ch)) """
    pass

def gain_exp(ch: 'CHAR_DATA', gain: 'int') -> 'None':
    """ C function: void  gain_exp args((CHAR_DATA * ch, int gain)) """
    pass

def gain_condition(ch: 'CHAR_DATA', iCond: 'int', value: 'int') -> 'None':
    """ C function: void  gain_condition args((CHAR_DATA * ch, int iCond, int value)) """
    pass

def update_handler() -> 'None':
    """ C function: void  update_handler args((void)) """
    pass

def ban_update() -> 'None':
    """ C function: void  ban_update args((void)) """
    pass

def run_olc_editor(d: 'DESCRIPTOR_DATA') -> 'bool':
    """ C function: bool  run_olc_editor args((DESCRIPTOR_DATA * d)) """
    pass

def olc_ed_name(ch: 'CHAR_DATA') -> 'str':
    """ C function: char * olc_ed_name args((CHAR_DATA * ch)) """
    pass

def olc_ed_vnum(ch: 'CHAR_DATA') -> 'str':
    """ C function: char * olc_ed_vnum args((CHAR_DATA * ch)) """
    pass

def string_edit(ch: 'CHAR_DATA', pString: 'str') -> 'None':
    """ C function: void  string_edit args((CHAR_DATA * ch, char **pString)) """
    pass

def string_append(ch: 'CHAR_DATA', pString: 'str') -> 'None':
    """ C function: void  string_append args((CHAR_DATA * ch, char **pString)) """
    pass

def string_replace(orig: 'str', old: 'str', new: 'str') -> 'str':
    """ C function: char * string_replace args((char *orig, char *old, char *new)) """
    pass

def string_add(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  string_add args((CHAR_DATA * ch, char *argument)) """
    pass

def format_string(oldstring: 'str') -> 'str':
    """ C function: char * format_string args((char *oldstring )) """
    pass

def first_arg(argument: 'str', arg_first: 'str', fCase: 'bool') -> 'str':
    """ C function: char * first_arg args((char *argument, char *arg_first, bool fCase)) """
    pass

def string_unpad(argument: 'str') -> 'str':
    """ C function: char * string_unpad args((char *argument)) """
    pass

def string_proper(argument: 'str') -> 'str':
    """ C function: char * string_proper args((char *argument)) """
    pass

def all_capitalize(str: 'const char') -> 'str':
    """ C function: char * all_capitalize args((const char *str)) """
    pass

def mprog_type_to_name(type: 'int') -> 'str':
    """ C function: char * mprog_type_to_name args((int type)) """
    pass

def rprog_type_to_name(type: 'int') -> 'str':
    """ C function: char * rprog_type_to_name args((int type)) """
    pass

CLAN_LEVEL = 5
CLAN_KILL = 20
name: 'str' = ""
whoname: 'str' = ""
filename: 'str' = ""
leader: 'str' = ""
founder: 'str' = ""
desc: 'str' = ""
feud: 'str' = ""
ally: 'str' = ""
dirty: 'int' = 0
score: 'int' = 0
cclass: 'int' = 0
hire: 'int' = 0
construct: 'int' = 0
crecall: 'int' = 0

class clan_data:
    """ C struct clan_data """
    def __init__(self):
        self.next: 'CLAN_DATA' = None
        self.member: 'int' = 0
        self.cobj: 'int' = 0
        self.kill: 'int' = 0

cmd: 'str' = ""
timer: 'int' = 0

class call_out_data:
    """ C struct call_out_data """
    def __init__(self):
        self.owner: 'CHAR_DATA' = None
        self.next: 'CALL_OUT_DATA' = None

cmd: 'str' = ""
timer: 'int' = 0

class rcall_out_data:
    """ C struct rcall_out_data """
    def __init__(self):
        self.owner: 'ROOM_INDEX_DATA' = None
        self.next: 'RCALL_OUT_DATA' = None

price: 'int' = 0
time: 'int' = 0
recall: 'int' = 0

class auction_data:
    """ C struct auction_data """
    def __init__(self):
        self.next: 'AUC_DATA' = None
        self.owner: 'CHAR_DATA' = None
        self.buyer: 'CHAR_DATA' = None
        self.obj: 'OBJ_DATA' = None

name: 'str' = ""
password: 'str' = ""
amount: 'int' = 0
recall: 'int' = 0
opened: 'bool' = False

class bank_account_data:
    """ C struct bank_account_data """
    def __init__(self):
        self.next: 'BANK_DATA' = None
        self.objs: 'OBJ_DATA' = None
        self.obj_list: 'OBJ_DATA' = None

target: 'int' = 0
name: 'str' = ""
cname: 'str' = ""
msg_to_vict: 'str' = ""
msg_to_room: 'str' = ""
msg_to_char: 'str' = ""

class area_skill_data:
    """ C struct area_skill_data """
    def __init__(self):
        self.next: 'ASKILL_DATA' = None

race: 'int' = 0
hr_mod_q: 'int' = 0
hr_mod_p: 'int' = 0
dr_mod_q: 'int' = 0
dr_mod_p: 'int' = 0

class obj_vs_race:
    """ C struct obj_vs_race """
    def __init__(self):
        self.next: 'OBJ_VRACE' = None
