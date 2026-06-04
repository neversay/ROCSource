# Auto-converted from olc.h
import typing

# olc.h: Online Creation system editor states, prototypes, and building data structures.
# olc.h: 線上動態建設系統（OLC）之編輯器狀態、函式原型及地圖/怪物/物品建設資料結構。
VERSION = "ILAB Online Creation [Beta 1.1]"
AUTHOR = "	    By Jason(jdinkel@mines.colorado.edu)"
DATE = "	  (May. 15, 1995)"
CREDITS = "     Original by Surreality(cxw197@psu.edu) and Locke(locke@lm.com)\n\r     Modified by Amenda(amenda@wof.muds.net) and Zcecil(zcecil@wof.muds.net)"

# Macro function: DECLARE_OLC_FUN(fun)
def DECLARE_OLC_FUN(fun):
    # C implementation: OLC_FUN fun
    pass
ED_AREA = 1
ED_ROOM = 2
ED_OBJECT = 3
ED_MOBILE = 4
ED_MPROG = 5
def aedit(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  aedit args((CHAR_DATA * ch, char *argument)) """
    pass

def redit(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  redit args((CHAR_DATA * ch, char *argument)) """
    pass

def medit(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  medit args((CHAR_DATA * ch, char *argument)) """
    pass

def oedit(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  oedit args((CHAR_DATA * ch, char *argument)) """
    pass

def mpedit(ch: 'CHAR_DATA', argument: 'str') -> 'None':
    """ C function: void  mpedit args((CHAR_DATA * ch, char *argument)) """
    pass

MAX_MOB = 1
immonly: 'bool' = False

class olc_cmd_type:
    """ C struct olc_cmd_type """
    def __init__(self):
        self.olc_fun: 'OLC_FUN' = None


class editor_cmd_type:
    """ C struct editor_cmd_type """
    def __init__(self):
        self.do_fun: 'DO_FUN' = None

def get_vnum_area(vnum: 'int') -> 'AREA_DATA':
    """ C function: AREA_DATA * get_vnum_area args((int vnum)) """
    pass

def get_area_data(vnum: 'int') -> 'AREA_DATA':
    """ C function: AREA_DATA * get_area_data args((int vnum)) """
    pass

def show_commands(ch: 'CHAR_DATA', argument: 'str') -> 'bool':
    """ C function: bool  show_commands args((CHAR_DATA * ch, char *argument)) """
    pass

def show_help(ch: 'CHAR_DATA', argument: 'str') -> 'bool':
    """ C function: bool  show_help args((CHAR_DATA * ch, char *argument)) """
    pass

def edit_done(ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  edit_done args((CHAR_DATA * ch)) """
    pass

def show_version(ch: 'CHAR_DATA', argument: 'str') -> 'bool':
    """ C function: bool  show_version args((CHAR_DATA * ch, char *argument)) """
    pass

def N(arg0: 'aedit_show') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_show) """
    pass

def N(arg0: 'aedit_ftoggle') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_ftoggle) """
    pass

def N(arg0: 'aedit_create') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_create) """
    pass

def N(arg0: 'aedit_rlist') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_rlist) """
    pass

def N(arg0: 'aedit_mlist') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_mlist) """
    pass

def N(arg0: 'aedit_olist') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_olist) """
    pass

def N(arg0: 'aedit_save') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_save) """
    pass

def N(arg0: 'aedit_name') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_name) """
    pass

def N(arg0: 'aedit_identity') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_identity) """
    pass

def N(arg0: 'aedit_file') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_file) """
    pass

def N(arg0: 'aedit_age') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_age) """
    pass

def N(arg0: 'aedit_recall') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_recall) """
    pass

def N(arg0: 'aedit_reset') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_reset) """
    pass

def N(arg0: 'aedit_security') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_security) """
    pass

def N(arg0: 'aedit_builder') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_builder) """
    pass

def N(arg0: 'aedit_vnum') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_vnum) """
    pass

def N(arg0: 'aedit_lvnum') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_lvnum) """
    pass

def N(arg0: 'aedit_uvnum') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(aedit_uvnum) """
    pass

def N(arg0: 'redit_show') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_show) """
    pass

def N(arg0: 'redit_create') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_create) """
    pass

def N(arg0: 'redit_name') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_name) """
    pass

def N(arg0: 'redit_purge') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_purge) """
    pass

def N(arg0: 'redit_desc') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_desc) """
    pass

def N(arg0: 'redit_ed') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_ed) """
    pass

def N(arg0: 'redit_recall') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_recall) """
    pass

def N(arg0: 'redit_room') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_room) """
    pass

def N(arg0: 'redit_sector') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_sector) """
    pass

def N(arg0: 'redit_format') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_format) """
    pass

def N(arg0: 'redit_north') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_north) """
    pass

def N(arg0: 'redit_south') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_south) """
    pass

def N(arg0: 'redit_east') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_east) """
    pass

def N(arg0: 'redit_west') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_west) """
    pass

def N(arg0: 'redit_up') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_up) """
    pass

def N(arg0: 'redit_down') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_down) """
    pass

def N(arg0: 'redit_move') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_move) """
    pass

def N(arg0: 'redit_mreset') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_mreset) """
    pass

def N(arg0: 'redit_oreset') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_oreset) """
    pass

def N(arg0: 'redit_mlist') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_mlist) """
    pass

def N(arg0: 'redit_olist') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_olist) """
    pass

def N(arg0: 'redit_mshow') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_mshow) """
    pass

def N(arg0: 'redit_oshow') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(redit_oshow) """
    pass

def N(arg0: 'oedit_show') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_show) """
    pass

def N(arg0: 'oedit_create') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_create) """
    pass

def N(arg0: 'oedit_material') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_material) """
    pass

def N(arg0: 'oedit_oload') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_oload) """
    pass

def N(arg0: 'oedit_mtlist') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_mtlist) """
    pass

def N(arg0: 'oedit_name') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_name) """
    pass

def N(arg0: 'oedit_short') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_short) """
    pass

def N(arg0: 'oedit_long') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_long) """
    pass

def N(arg0: 'oedit_extra') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_extra) """
    pass

def N(arg0: 'oedit_wear') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_wear) """
    pass

def N(arg0: 'oedit_type') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_type) """
    pass

def N(arg0: 'oedit_level') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_level) """
    pass

def N(arg0: 'oedit_addaffect') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_addaffect) """
    pass

def N(arg0: 'oedit_delaffect') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_delaffect) """
    pass

def N(arg0: 'oedit_value0') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_value0) """
    pass

def N(arg0: 'oedit_value1') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_value1) """
    pass

def N(arg0: 'oedit_value2') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_value2) """
    pass

def N(arg0: 'oedit_value3') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_value3) """
    pass

def N(arg0: 'oedit_weight') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_weight) """
    pass

def N(arg0: 'oedit_cost') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_cost) """
    pass

def N(arg0: 'oedit_ed') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(oedit_ed) """
    pass

def N(arg0: 'medit_show') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_show) """
    pass

def N(arg0: 'medit_create') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_create) """
    pass

def N(arg0: 'medit_name') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_name) """
    pass

def N(arg0: 'medit_short') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_short) """
    pass

def N(arg0: 'medit_long') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_long) """
    pass

def N(arg0: 'medit_shop') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_shop) """
    pass

def N(arg0: 'medit_desc') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_desc) """
    pass

def N(arg0: 'medit_level') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_level) """
    pass

def N(arg0: 'medit_align') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_align) """
    pass

def N(arg0: 'medit_sex') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_sex) """
    pass

def N(arg0: 'medit_spec') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_spec) """
    pass

def N(arg0: 'medit_class') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_class) """
    pass

def N(arg0: 'medit_race') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(medit_race) """
    pass

def N(arg0: 'mpedit_show') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_show) """
    pass

def N(arg0: 'mpedit_add') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_add) """
    pass

def N(arg0: 'mpedit_delete') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_delete) """
    pass

def N(arg0: 'mpedit_create') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_create) """
    pass

def N(arg0: 'mpedit_copy') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_copy) """
    pass

def N(arg0: 'mpedit_trigger') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_trigger) """
    pass

def N(arg0: 'mpedit_program') -> 'DECLARE_OLC_FU':
    """ C function: DECLARE_OLC_FU N(mpedit_program) """
    pass

name: 'str' = ""
cclass: 'int' = 0
cost: 'int' = 0

class flag_cost:
    """ C struct flag_cost """
    def __init__(self):
        pass


# Macro function: IS_BUILDER(ch, Area)
def IS_BUILDER(ch, Area):
    # C implementation: ((ch->pcdata->security >= Area->security || strstr(Area->builders, ch->name) || strstr(Area->builders, "All")) && !IS_SWITCHED(ch))
    pass

# Macro function: IS_PPLBUILDER(ch, Area)
def IS_PPLBUILDER(ch, Area):
    # C implementation: (((ch)->clan ? (strstr(Area->builders, ch->clan->name) && !IS_SWITCHED(ch)) : FALSE))
    pass

# Macro function: TOGGLE_BIT(var, bit)
def TOGGLE_BIT(var, bit):
    # C implementation: ((var) ^= (bit))
    pass

# Macro function: EDIT_MOB(Ch, Mob)
def EDIT_MOB(Ch, Mob):
    # C implementation: (Mob = (MOB_INDEX_DATA *)Ch->desc->pEdit)
    pass

# Macro function: EDIT_OBJ(Ch, Obj)
def EDIT_OBJ(Ch, Obj):
    # C implementation: (Obj = (OBJ_INDEX_DATA *)Ch->desc->pEdit)
    pass

# Macro function: EDIT_ROOM(Ch, Room)
def EDIT_ROOM(Ch, Room):
    # C implementation: (Room = Ch->in_room)
    pass

# Macro function: EDIT_AREA(Ch, Area)
def EDIT_AREA(Ch, Area):
    # C implementation: (Area = (AREA_DATA *)Ch->desc->pEdit)
    pass
ED = EXTRA_DESCR_DATA
def new_reset_data() -> 'RESET_DATA':
    """ C function: RESET_DATA * new_reset_data args((void)) """
    pass

def free_reset_data(pReset: 'RESET_DATA') -> 'None':
    """ C function: void  free_reset_data args((RESET_DATA * pReset)) """
    pass

def new_area() -> 'AREA_DATA':
    """ C function: AREA_DATA * new_area args((void)) """
    pass

def free_area(pArea: 'AREA_DATA') -> 'None':
    """ C function: void  free_area args((AREA_DATA * pArea)) """
    pass

def new_exit() -> 'EXIT_DATA':
    """ C function: EXIT_DATA * new_exit args((void)) """
    pass

def free_exit(pExit: 'EXIT_DATA') -> 'None':
    """ C function: void  free_exit args((EXIT_DATA * pExit)) """
    pass

def new_extra_descr() -> 'ED':
    """ C function: ED * new_extra_descr args((void)) """
    pass

def free_extra_descr(pExtra: 'ED') -> 'None':
    """ C function: void  free_extra_descr args((ED * pExtra)) """
    pass

def new_room_index() -> 'ROOM_INDEX_DATA':
    """ C function: ROOM_INDEX_DATA * new_room_index args((void)) """
    pass

def free_room_index(pRoom: 'ROOM_INDEX_DATA') -> 'None':
    """ C function: void  free_room_index args((ROOM_INDEX_DATA * pRoom)) """
    pass

def new_affect() -> 'AFFECT_DATA':
    """ C function: AFFECT_DATA * new_affect args((void)) """
    pass

def free_affect(pAf: 'AFFECT_DATA') -> 'None':
    """ C function: void  free_affect args((AFFECT_DATA * pAf)) """
    pass

def new_shop() -> 'SHOP_DATA':
    """ C function: SHOP_DATA * new_shop args((void)) """
    pass

def free_shop(pShop: 'SHOP_DATA') -> 'None':
    """ C function: void  free_shop args((SHOP_DATA * pShop)) """
    pass

def new_obj_index() -> 'OBJ_INDEX_DATA':
    """ C function: OBJ_INDEX_DATA * new_obj_index args((void)) """
    pass

def free_obj_index(pObj: 'OBJ_INDEX_DATA') -> 'None':
    """ C function: void  free_obj_index args((OBJ_INDEX_DATA * pObj)) """
    pass

def new_mob_index() -> 'MOB_INDEX_DATA':
    """ C function: MOB_INDEX_DATA * new_mob_index args((void)) """
    pass

def free_mob_index(pMob: 'MOB_INDEX_DATA') -> 'None':
    """ C function: void  free_mob_index args((MOB_INDEX_DATA * pMob)) """
    pass

def mprog_count(pMob: 'MOB_INDEX_DATA') -> 'int':
    """ C function: int  mprog_count args((MOB_INDEX_DATA * pMob)) """
    pass

def delete_mprog(ch: 'CHAR_DATA', pnum: 'int') -> 'None':
    """ C function: void  delete_mprog args((CHAR_DATA * ch, int pnum)) """
    pass

def check_cost(ch: 'CHAR_DATA', cost: 'int') -> 'bool':
    """ C function: bool  check_cost(CHAR_DATA *ch, int cost) """
    pass

def check_clco(fc: 'const struct flag_cost', arg: 'str', ch: 'CHAR_DATA') -> 'bool':
    """ C function: bool  check_clco(const struct flag_cost fc[], char *arg, CHAR_DATA *ch) """
    pass

def flag_cost(fc: 'const struct flag_cost', arg: 'str') -> 'int':
    """ C function: int   flag_cost(const struct flag_cost fc[], char *arg) """
    pass

def flag_class(fc: 'const struct flag_cost', arg: 'str') -> 'int':
    """ C function: int   flag_class(const struct flag_cost fc[], char *arg) """
    pass

COST_CREATE_ROOM = 1000000
COST_TWOWAY_EXIT = 100000
COST_DELETE_EXIT = 10000
COST_ONEWAY_EXIT = 50000
COST_REMOVE_KEY = 10000
COST_SET_KEY = 30000
COST_SET_DOORNAME = 10000
COST_SET_DOORDESC = 50000
COST_SET_CRECALL = 500000
COST_SET_ROOMED = 200000
COST_DEL_ROOMED = 10000
COST_CREATE_OBJ = 1500000
COST_OLOAD_OBJ = 10000
COST_SET_OBJNAME = 20000
COST_SET_OBJLONG = 100000
COST_SET_OBJED = 200000
COST_DEL_OBJED = 10000
COST_SET_OBJSHORT = 50000
COST_ADD_RESETOBJ = 500000
COST_CREATE_MOB = 500000