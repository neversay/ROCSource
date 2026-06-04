# Auto-converted from macros.h
import typing

# macros.h: Core utility and math macros (e.g. min, max, bit checks, name matching helpers).
# macros.h: 核心公用程式與數學運算巨集（如 min、max、位元檢查及名稱比對輔助等）。
__MACROS_H__ = None

# Macro function: UMIN(a, b)
def UMIN(a, b):
    # C implementation: ((a) < (b) ? (a) : (b))
    pass

# Macro function: UMAX(a, b)
def UMAX(a, b):
    # C implementation: ((a) > (b) ? (a) : (b))
    pass

# Macro function: URANGE(a, b, c)
def URANGE(a, b, c):
    # C implementation: ((b) < (a) ? (a)              \
    pass

# Macro function: LOWER(c)
def LOWER(c):
    # C implementation: ((c) >= 'A' && (c) <= 'Z' \
    pass

# Macro function: UPPER(c)
def UPPER(c):
    # C implementation: ((c) >= 'a' && (c) <= 'z' \
    pass

# Macro function: IS_SET(flag, bit)
def IS_SET(flag, bit):
    # C implementation: ((flag) ? (flag) & (bit) : 0)
    pass

# Macro function: SET_BIT(var, bit)
def SET_BIT(var, bit):
    # C implementation: ((var) |= (bit))
    pass

# Macro function: REMOVE_BIT(var, bit)
def REMOVE_BIT(var, bit):
    # C implementation: ((var) &= ~(bit))
    pass

# Macro function: NUM_BIT(var)
def NUM_BIT(var):
    # C implementation: (1 << var)
    pass
# #define R_NUM_BIT( var )		( var >> 1 )

# Macro function: IS_NPC(ch)
def IS_NPC(ch):
    # C implementation: (IS_SET((ch)->act, ACT_IS_NPC))
    pass

# Macro function: IS_IMMORTAL(ch)
def IS_IMMORTAL(ch):
    # C implementation: (get_trust(ch) >= LEVEL_IMMORTAL && \
    pass

# Macro function: IS_HERO(ch)
def IS_HERO(ch):
    # C implementation: (get_trust(ch) >= LEVEL_HERO)
    pass

# Macro function: IS_AFFECTED(ch, sn)
def IS_AFFECTED(ch, sn):
    # C implementation: (IS_SET((ch)->affected_by, (sn)))
    pass
# #define IS_AFFECTED2( ch, sn )	     ( IS_SET( ( ch )->affected_by2, ( sn ) ) )
GOOD_BASE = 300
EVIL_BASE = -300

# Macro function: IS_GOOD(ch)
def IS_GOOD(ch):
    # C implementation: (ch->alignment >= GOOD_BASE)
    pass

# Macro function: IS_EVIL(ch)
def IS_EVIL(ch):
    # C implementation: (ch->alignment <= EVIL_BASE)
    pass

# Macro function: IS_NEUTRAL(ch)
def IS_NEUTRAL(ch):
    # C implementation: (!IS_GOOD(ch) && !IS_EVIL(ch))
    pass

# Macro function: IS_AWAKE(ch)
def IS_AWAKE(ch):
    # C implementation: (ch->position > POS_SLEEPING)
    pass

# Macro function: IS_OUTSIDE(ch)
def IS_OUTSIDE(ch):
    # C implementation: ((ch)->in_room && !IS_SET(                       \
    pass
# #define WAIT_STATE( ch, pulse ) ( ( ch )->wait = UMAX( ( ch )->wait, (pulse)))

# Macro function: MANA_COST(ch, sn)
def MANA_COST(ch, sn):
    # C implementation: (skill_table[sn].min_mana)
    pass
# #define MANA_COST( ch, sn )	( IS_NPC( ch ) ? 0 : skill_table[sn].min_mana )
# #define MANA_COST( ch, sn )	( skill_table[(sn)].min_mana * (IS_NPC((ch)) ? 1 : (class_table[(ch)->class].fMana ? 1 : 2)))

# Macro function: IS_SWITCHED(ch)
def IS_SWITCHED(ch):
    # C implementation: (ch->pcdata->switched)
    pass

# Macro function: INT_DIFF(ch, victim)
def INT_DIFF(ch, victim):
    # C implementation: (get_curr_int(ch) - get_curr_int(victim))
    pass

# Macro function: WIS_DIFF(ch, victim)
def WIS_DIFF(ch, victim):
    # C implementation: (get_curr_wis(ch) - get_curr_wis(victim))
    pass

# Macro function: DEX_DIFF(ch, victim)
def DEX_DIFF(ch, victim):
    # C implementation: (get_curr_dex(ch) - get_curr_dex(victim))
    pass

# Macro function: STR_DIFF(ch, victim)
def STR_DIFF(ch, victim):
    # C implementation: (get_curr_str(ch) - get_curr_str(victim))
    pass

# Macro function: CON_DIFF(ch, victim)
def CON_DIFF(ch, victim):
    # C implementation: (get_curr_con(ch) - get_curr_con(victim))
    pass

# Macro function: CAN_WEAR(obj, part)
def CAN_WEAR(obj, part):
    # C implementation: (IS_SET((obj)->wear_flags, (part)))
    pass

# Macro function: IS_OBJ_STAT(obj, stat)
def IS_OBJ_STAT(obj, stat):
    # C implementation: (IS_SET((obj)->extra_flags, (stat)))
    pass

# Macro function: PERS(ch, looker)
def PERS(ch, looker):
    # C implementation: (can_see(looker, (ch)) ? strcat(strcpy(name_buf, (ch)->short_descr), "") : "不明人物 ")
    pass