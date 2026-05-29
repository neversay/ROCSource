#!/usr/bin/env python3
import sys
import os
import argparse
import json

# ==============================================================================
# CONSTANTS REGISTRY
# ==============================================================================

ACT_FLAGS = {
    1: "NPC",
    2: "SENTINEL",
    4: "SCAVENGER",
    32: "AGGRESSIVE",
    64: "STAY_AREA",
    128: "WIMPY",
    256: "PET",
    512: "TRAIN",
    1024: "PRACTICE",
    2048: "CRITICAL",
    4096: "HARD_DIS",
    8192: "HUNT",
    16384: "TWO_WEAPON",
    65536: "ARTIFACT",
    262144: "TACTICS",
    524288: "TRACK",
    1048576: "REGENERACY",
    2097152: "GAMBLE",
    4194304: "BANKER",
    8388608: "MOB_AI",
    16777216: "REFLEXION",
    33554432: "NO_PAR",
    67108864: "IGNORE_NO_MOB",
    268435456: "SMITH",
}

AFF_FLAGS = {
    1: "BLIND",
    2: "INVISIBLE",
    4: "DETECT_EVIL",
    8: "DETECT_INVIS",
    16: "DETECT_MAGIC",
    32: "DETECT_HIDDEN",
    64: "HOLD",
    128: "SANCTUARY",
    256: "FAERIE_FIRE",
    512: "INFRARED",
    1024: "CURSE",
    2048: "CHANGE_SEX",
    4096: "POISON",
    8192: "PROTECT",
    16384: "POLYMORPH",
    32768: "SNEAK",
    65536: "HIDE",
    131072: "SLEEP",
    262144: "CHARM",
    524288: "FLYING",
    1048576: "PASS_DOOR",
    2097152: "WATERWALK",
    4194304: "SUMMONED",
    8388608: "MUTE",
    16777216: "SEALMAGIC",
    134217728: "FLAMING",
    268435456: "PARALYZED",
    536870912: "CRAZY",
    1073741824: "WEAKEN",
    2147483648: "NIGHTSTALK",
    4294967296: "DARK_RAID",
    8589934592: "MANASHIELD",
    17179869184: "GILLS",
    34359738368: "TARDY",
    68719476736: "MYSTIC_MIRROR",
    137438953472: "MYSTIC_ARMOR",
    549755813888: "CONTINUE_CURE",
    1099511627776: "AFF_DISEASE",
    2199023255552: "AFF_FEAR",
}

P_FLAGS = {
    1: "NO_DM",
    2: "NO_LORE",
    4: "NO_BLIND",
    8: "NO_CHARM",
    16: "NO_CURSE",
    32: "NO_FAEFIRE",
    64: "NO_POISON",
    128: "NO_SLEEP",
    256: "HALF_MAGIC",
    512: "ANTI_MAGIC",
    1024: "REF_MAGIC",
    2048: "ALY_SAN",
    4096: "ALY_INVIS",
    8192: "ALY_HIDE",
    16384: "ALY_SNEAK",
    32768: "ALY_DI",
    65536: "ALY_DH",
    131072: "ALY_INF",
    262144: "ALY_BAR",
    524288: "NO_TARDY",
    1048576: "NO_MUTE",
    2097152: "NO_SEALMAGIC",
    4194304: "EXPERT",
    8388608: "NO_CRAZY",
    16777216: "MOBAI_LV2",
    33554432: "MOBAI_LV3",
    67108864: "NO_PARA",
    268435456: "SPELL_FURY",
    536870912: "NO_FEAR",
}

ROOM_FLAGS = {
    1: "DARK",
    4: "NO_MOB",
    8: "INDOORS",
    16: "UNDERGROUND",
    512: "PRIVATE",
    1024: "SAFE",
    2048: "SOLITARY",
    4096: "PET_SHOP",
    8192: "NO_RECALL",
    16384: "CONE_OF_SILENCE",
    32768: "GRAVE",
    65536: "SUMMON_BIT",
    131072: "AUCTION",
    262144: "SAVE",
    524288: "NO_TELEPORT_OUT",
    1048576: "NO_TELEPORT_IN",
    2097152: "PK",
}

SECTOR_TYPES = {
    0: "INSIDE",
    1: "CITY",
    2: "FIELD",
    3: "FOREST",
    4: "HILLS",
    5: "MOUNTAIN",
    6: "WATER_SWIM",
    7: "WATER_NOSWIM",
    8: "UNDERWATER",
    9: "AIR",
    10: "DESERT",
}

DOOR_FLAGS = {
    1: "ISDOOR",
    2: "CLOSED",
    4: "LOCKED",
    8: "BASHED",
    16: "BASHPROOF",
    32: "PICKPROOF",
    64: "PASSPROOF",
}

ITEM_TYPES = {
    1: "LIGHT",
    2: "SCROLL",
    3: "WAND",
    4: "STAFF",
    5: "WEAPON",
    8: "TREASURE",
    9: "ARMOR",
    10: "POTION",
    12: "FURNITURE",
    13: "TRASH",
    15: "CONTAINER",
    17: "DRINK_CON",
    18: "KEY",
    19: "FOOD",
    20: "MONEY",
    22: "BOAT",
    23: "CORPSE_NPC",
    24: "CORPSE_PC",
    25: "FOUNTAIN",
    26: "PILL",
    27: "ARROW",
    28: "DART",
}

EXTRA_FLAGS = {
    1: "GLOW",
    2: "HUM",
    4: "DARK",
    8: "LOCK",
    16: "EVIL",
    32: "INVIS",
    64: "MAGIC",
    128: "NODROP",
    256: "BLESS",
    512: "ANTI_GOOD",
    1024: "ANTI_EVIL",
    2048: "ANTI_NEUTRAL",
    4096: "NOREMOVE",
    8192: "INVENTORY",
    16384: "NOSAVE",
    32768: "VAMPIRE_BANE",
    65536: "HOLY",
    131072: "POISONED",
    262144: "REMOVE_NODROP",
    524288: "DONATION_PIT",
    1048576: "UNIQUE",
    2097152: "CLAN",
    4194304: "PERSONAL",
    16777216: "LORE",
    33554432: "NO_SAC",
}

WEAR_FLAGS = {
    1: "TAKE",
    2: "WEAR_FINGER",
    4: "WEAR_NECK",
    8: "WEAR_BODY",
    16: "WEAR_HEAD",
    32: "WEAR_LEGS",
    64: "WEAR_FEET",
    128: "WEAR_HANDS",
    256: "WEAR_ARMS",
    512: "WEAR_SHIELD",
    1024: "WEAR_ABOUT",
    2048: "WEAR_WAIST",
    4096: "WEAR_WRIST",
    8192: "WIELD",
    16384: "HOLD",
    32768: "TWO_HAND",
    65536: "ARROW",
}

APPLY_TYPES = {
    0: "NONE",
    1: "STR",
    2: "DEX",
    3: "INT",
    4: "WIS",
    5: "CON",
    6: "SEX",
    7: "CLASS",
    8: "LEVEL",
    9: "AGE",
    10: "HEIGHT",
    11: "WEIGHT",
    12: "MANA",
    13: "HIT",
    14: "MOVE",
    15: "GOLD",
    16: "EXP",
    17: "AC",
    18: "HITROLL",
    19: "DAMROLL",
    20: "SAVING_PARA",
    21: "SAVING_ROD",
    22: "SAVING_PETRI",
    23: "SAVING_BREATH",
    24: "SAVING_SPELL",
    27: "SAVING_FIRE",
    28: "SAVING_COLD",
    29: "SAVING_LIGHTNING",
    30: "SAVING_EARTH",
    31: "SAVING_WIND",
    35: "SAVING_ELEMENT",
    38: "RACE",
    39: "DODGE",
    40: "POWER_POTENTIAL",
    41: "REDUCE_DAMAGE",
    42: "REDUCE_MAGIC",
    43: "APPLY_ADAMAGE",
    44: "APPLY_AMAGIC",
    45: "APPLY_GPHYSICAL",
    46: "APPLY_GMAGIC",
}

HP_TRIGGERS = {
    1: "HP_WEAR",
    2: "HP_FIGHT",
    4: "HP_WEAPON",
    8: "HP_BLOCK",
    16: "HP_ARROW",
}

RESET_WEAR_LOCS = {
    -1: "CARRY",
    0: "LIGHT",
    1: "FINGER_L",
    2: "FINGER_R",
    3: "NECK_1",
    4: "NECK_2",
    5: "BODY",
    6: "HEAD",
    7: "LEGS",
    8: "FEET",
    9: "HANDS",
    10: "ARMS",
    11: "SHIELD",
    12: "ABOUT",
    13: "WAIST",
    14: "WRIST_L",
    15: "WRIST_R",
    16: "WIELD",
    17: "HOLD",
    18: "WIELD_2",
    19: "TWO_HAND",
    20: "ARROW",
    22: "FINGER_3",
    23: "FINGER_4",
}

MOB_CLASSES = {
    0: "NORMAL",
    1: "WARRIOR",
    2: "MAGE",
    4: "THIEF",
    8: "SWORD",
    16: "SHOT_ARROW",
    32: "BLOODY",
}

CLASS_RESTRICTIONS = {
    1: "MAGE",
    2: "CLERIC",
    4: "THIEF",
    8: "WARRIOR",
    16: "BARBARIAN",
    32: "WIZARD",
    64: "ALCHEMIST",
    128: "SAVANT",
    256: "BISHOP",
    512: "ASSASSIN",
    1024: "NINJA",
    2048: "KNIGHT",
    4096: "SWORDSMAN",
    8192: "MONK",
    16384: "SHAMAN",
    32768: "SORCERER",
    65536: "LORD",
    131072: "RANGER",
    262144: "WARLORD",
    524288: "MARTIALMAN",
}

RACE_RESTRICTIONS = {
    1: "HUMAN",
    2: "ELF",
    4: "DWARF",
    8: "ORC",
    16: "HOBBIT",
    32: "IMP",
    64: "GNOME",
    128: "DROW",
    256: "VAMPIRE",
    512: "BEHOLDER",
    1024: "LIZARDMAN",
    2048: "WEREWOLF",
    4096: "MUMMY",
}

ALLOW_BIT = 1048576
DENY_BIT = 2097152

# Helper mappings for bidirectionality
def int_to_flags(val, mapping_dict):
    flags = []
    for k in sorted(mapping_dict.keys()):
        if k != 0 and (val & k) == k:
            flags.append(mapping_dict[k])
    return flags

def flags_to_int(flags_list, mapping_dict):
    val = 0
    reverse_dict = {v: k for k, v in mapping_dict.items()}
    for f in flags_list:
        if f in reverse_dict:
            val |= reverse_dict[f]
        else:
            try:
                val |= int(f)
            except ValueError:
                pass
    return val

def parse_restrictions(val, mapping_dict):
    rest_type = "ALLOW"
    if val & DENY_BIT:
        rest_type = "DENY"
        bits = val & ~DENY_BIT
    elif val & ALLOW_BIT:
        rest_type = "ALLOW"
        bits = val & ~ALLOW_BIT
    else:
        rest_type = "ALLOW"
        bits = val
    
    items = []
    for k in sorted(mapping_dict.keys()):
        if (bits & k) == k:
            items.append(mapping_dict[k])
    return {"type": rest_type, "list": items}

def build_restrictions(rest_dict, mapping_dict):
    if not rest_dict:
        return 0
    rest_type = rest_dict.get("type", "ALLOW")
    items_list = rest_dict.get("list", [])
    
    bits = flags_to_int(items_list, mapping_dict)
    if rest_type == "DENY":
        return bits | DENY_BIT
    else:
        return bits | ALLOW_BIT

# ==============================================================================
# LEGACY AREA LEXER
# ==============================================================================

class AreaLexer:
    def __init__(self, content):
        self.content = content
        self.pos = 0
        self.length = len(content)

    def is_eof(self):
        return self.pos >= self.length

    def get_char(self):
        if self.pos >= self.length:
            return None
        c = self.content[self.pos]
        self.pos += 1
        return c

    def unget_char(self):
        if self.pos > 0:
            self.pos -= 1

    def fread_letter(self):
        while True:
            c = self.get_char()
            if c is None:
                return None
            if not c.isspace():
                return c

    def fread_to_eol(self):
        while True:
            c = self.get_char()
            if c is None:
                return
            if c in ('\n', '\r'):
                break
        while True:
            c = self.get_char()
            if c is None:
                return
            if c not in ('\n', '\r'):
                self.unget_char()
                break

    def fread_word(self):
        while True:
            c = self.get_char()
            if c is None:
                return ""
            if not c.isspace():
                break
        
        if c in ("'", '"'):
            quote = c
            word_chars = []
            while True:
                c = self.get_char()
                if c is None or c == quote:
                    break
                word_chars.append(c)
            return "".join(word_chars)
        else:
            word_chars = [c]
            while True:
                c = self.get_char()
                if c is None:
                    break
                if c.isspace():
                    self.unget_char()
                    break
                word_chars.append(c)
            return "".join(word_chars)

    def fread_string(self):
        while True:
            c = self.get_char()
            if c is None:
                return ""
            if not c.isspace():
                break
        
        if c == '~':
            return ""
        
        string_chars = [c]
        while True:
            c = self.get_char()
            if c is None or c == '~':
                break
            string_chars.append(c)
        
        raw_str = "".join(string_chars)
        normalized = raw_str.replace('\r\n', '\n').replace('\n\r', '\n').replace('\r', '\n')
        return normalized

    def fread_number(self):
        while True:
            c = self.get_char()
            if c is None:
                return 0
            if not c.isspace():
                break
        
        sign = 1
        if c == '+':
            c = self.get_char()
        elif c == '|':
            c = self.get_char()
        elif c == '-':
            sign = -1
            c = self.get_char()
            
        if not c.isdigit():
            self.unget_char()
            word = []
            while True:
                c = self.get_char()
                if c is None:
                    break
                if not (c.isalnum() or c in ('_', '-')):
                    self.unget_char()
                    break
                word.append(c)
            word_str = "".join(word)
            
            # Simple symbolic word fallback mapping (e.g. standard symbols if any)
            val = 0
            # If we need complex symbol lookup, we can implement it here.
            # But in elftown.are all numbers combined with | are actual digits, so val fallback is fine.
            val *= sign
            
            c = self.get_char()
            if c == '|':
                val += self.fread_number()
            else:
                self.unget_char()
            return val
        
        number = 0
        while c is not None and c.isdigit():
            number = number * 10 + int(c)
            c = self.get_char()
            
        number *= sign
        if c == '|':
            number += self.fread_number()
        else:
            self.unget_char()
        return number

# ==============================================================================
# DECOMPILER (Parser from .are to Dict)
# ==============================================================================

def decompile_area(content):
    lexer = AreaLexer(content)
    data = {
        "area": {},
        "helps": [],
        "materials": {},
        "mobprogs": [],
        "mobiles": {},
        "objects": {},
        "rooms": {},
        "resets": [],
        "shops": [],
        "specials": []
    }
    
    while not lexer.is_eof():
        letter = lexer.fread_letter()
        if letter is None:
            break
        if letter == '#':
            word = lexer.fread_word()
            if word == '$':
                break
            
            elif word == "AREADATA":
                while True:
                    key = lexer.fread_word()
                    if key == "End":
                        break
                    elif key == "Name":
                        data["area"]["name"] = lexer.fread_string()
                    elif key == "Identity":
                        data["area"]["identity"] = lexer.fread_string()
                    elif key == "Builders":
                        data["area"]["builders"] = lexer.fread_string()
                    elif key == "Translator":
                        data["area"]["translator"] = lexer.fread_string()
                    elif key == "Modifier":
                        data["area"]["modifier"] = lexer.fread_string()
                    elif key == "VNUMs":
                        data["area"]["vnum_start"] = lexer.fread_number()
                        data["area"]["vnum_end"] = lexer.fread_number()
                    elif key == "Security":
                        data["area"]["security"] = lexer.fread_number()
                    elif key == "Recall":
                        data["area"]["recall"] = lexer.fread_number()
                    elif key == "Death":
                        data["area"]["death"] = lexer.fread_number()
                    elif key == "F_toggle":
                        data["area"]["f_toggle"] = (lexer.fread_number() != 0)
            
            elif word == "HELPS":
                while True:
                    if lexer.is_eof():
                        break
                    level = lexer.fread_number()
                    keywords = lexer.fread_string()
                    if keywords.startswith("$"):
                        break
                    text = lexer.fread_string()
                    data["helps"].append({
                        "level": level,
                        "keywords": keywords,
                        "text": text
                    })
                    
            elif word == "MATERIAL":
                while True:
                    let = lexer.fread_letter()
                    if let != '#' or let is None:
                        break
                    vnum = lexer.fread_number()
                    if vnum == 0:
                        break
                    
                    mat = {
                        "name": lexer.fread_string(),
                        "cname": lexer.fread_string(),
                    }
                    lexer.fread_string() # unused
                    lexer.fread_string() # unused
                    mat["clan_level"] = lexer.fread_number()
                    mat["extra_flags"] = lexer.fread_number()
                    mat["clan_cost"] = lexer.fread_number()
                    
                    mat["hr_mod_p"] = lexer.fread_string()
                    mat["dr_mod_p"] = lexer.fread_string()
                    mat["ac_mod_p"] = lexer.fread_string()
                    mat["durability"] = lexer.fread_string()
                    
                    mat["weight_mod_p"] = lexer.fread_number()
                    mat["cost_mod_p"] = lexer.fread_number()
                    mat["cost_per_day"] = lexer.fread_number()
                    
                    mat["applies"] = []
                    mat["spells"] = []
                    mat["race_combat"] = []
                    mat["spell_buffs"] = []
                    
                    # Material letters loop
                    while True:
                        l = lexer.fread_letter()
                        if l is None:
                            break
                        if l == 'A':
                            mat["applies"].append({
                                "type": APPLY_TYPES.get(lexer.fread_number(), "NONE"),
                                "value": lexer.fread_number()
                            })
                        elif l == 'H':
                            mat["spells"].append({
                                "spell_slot": lexer.fread_number(),
                                "percent": lexer.fread_number(),
                                "level": lexer.fread_number(),
                                "time": lexer.fread_number()
                            })
                        elif l == 'L':
                            mat["level_limit"] = lexer.fread_number()
                        elif l == 'P':
                            mat["permit_classes"] = parse_restrictions(lexer.fread_number(), CLASS_RESTRICTIONS)
                        elif l == 'R':
                            mat["permit_races"] = parse_restrictions(lexer.fread_number(), RACE_RESTRICTIONS)
                        elif l == 'V':
                            mat["race_combat"].append({
                                "race": lexer.fread_string(),
                                "hr_mod_q": lexer.fread_number(),
                                "hr_mod_p": lexer.fread_number(),
                                "dr_mod_q": lexer.fread_number(),
                                "dr_mod_p": lexer.fread_number()
                            })
                        elif l == 'F':
                            mat["spell_buffs"].append(lexer.fread_word())
                        else:
                            lexer.unget_char()
                            break
                    data["materials"][vnum] = mat
                    
            elif word == "MOBILES":
                while True:
                    let = lexer.fread_letter()
                    if let != '#' or let is None:
                        break
                    vnum = lexer.fread_number()
                    if vnum == 0:
                        break
                    
                    mob = {
                        "keywords": lexer.fread_string(),
                        "short_description": lexer.fread_string(),
                        "long_description": lexer.fread_string(),
                        "description": lexer.fread_string(),
                    }
                    mob["act_flags"] = int_to_flags(lexer.fread_number(), ACT_FLAGS)
                    mob["affected_flags"] = int_to_flags(lexer.fread_number(), AFF_FLAGS)
                    mob["alignment"] = lexer.fread_number()
                    lexer.fread_letter() # S
                    mob["level"] = lexer.fread_number()
                    mob["mod_hitplus"] = lexer.fread_number()
                    mob["mod_mana"] = lexer.fread_number()
                    
                    # Read 'mod_hitroll d mod_damroll + mod_ac'
                    mob["mod_hitroll"] = lexer.fread_number()
                    lexer.fread_letter() # d
                    mob["mod_damroll"] = lexer.fread_number()
                    lexer.fread_letter() # +
                    mob["mod_ac"] = lexer.fread_number()
                    
                    # Read 'mod_svs d dodge + mod_pp'
                    mob["mod_svs"] = lexer.fread_number()
                    lexer.fread_letter() # d
                    mob["dodge"] = lexer.fread_number()
                    lexer.fread_letter() # +
                    mob["mod_pp"] = lexer.fread_number()
                    
                    mob["gold"] = lexer.fread_number()
                    lexer.fread_number() # xp (unused)
                    lexer.fread_number() # position (unused)
                    lexer.fread_number() # position (unused)
                    
                    sex_num = lexer.fread_number()
                    mob["sex"] = "female" if sex_num == 2 else ("male" if sex_num == 1 else "neutral")
                    
                    mob["weaknesses"] = []
                    mob["applies"] = []
                    mob["programs"] = []
                    mob["action_programs"] = []
                    
                    # Mobiles custom parameters letter loop
                    while True:
                        l = lexer.fread_letter()
                        if l is None:
                            break
                        if l == 'R':
                            mob["race"] = lexer.fread_string()
                        elif l == 'C':
                            mob["class"] = int_to_flags(lexer.fread_number(), MOB_CLASSES)
                        elif l == 'H':
                            mob["replica"] = lexer.fread_number()
                        elif l == 'P':
                            mob["spec_flags"] = int_to_flags(lexer.fread_number(), P_FLAGS)
                        elif l == 'W':
                            mob["weaknesses"].append({
                                "skill": lexer.fread_string(),
                                "percent": lexer.fread_number(),
                                "damage": lexer.fread_number()
                            })
                        elif l == 'S':
                            mob["applies"].append({
                                "type": APPLY_TYPES.get(lexer.fread_number(), "NONE"),
                                "degree": lexer.fread_number()
                            })
                        elif l == 'K':
                            mob["practice_skills"] = lexer.fread_string()
                        elif l == 'J':
                            mob["transfer_classes"] = lexer.fread_string()
                        elif l == 'A':
                            # Action daily routine
                            mob["action_programs"].append(lexer.fread_string())
                        elif l == '>':
                            # inline mobprog
                            prog_type = lexer.fread_word()
                            arglist = lexer.fread_string()
                            lexer.fread_to_eol()
                            comlist = lexer.fread_string()
                            lexer.fread_to_eol()
                            mob["programs"].append({
                                "trigger": prog_type,
                                "argument": arglist,
                                "script": comlist
                            })
                            # Program block terminator
                            next_let = lexer.fread_letter()
                            while next_let == '>':
                                prog_type = lexer.fread_word()
                                arglist = lexer.fread_string()
                                lexer.fread_to_eol()
                                comlist = lexer.fread_string()
                                lexer.fread_to_eol()
                                mob["programs"].append({
                                    "trigger": prog_type,
                                    "argument": arglist,
                                    "script": comlist
                                })
                                next_let = lexer.fread_letter()
                            if next_let != '|':
                                lexer.unget_char()
                        else:
                            lexer.unget_char()
                            break
                            
                    data["mobiles"][vnum] = mob

            elif word == "OBJECTS":
                while True:
                    let = lexer.fread_letter()
                    if let != '#' or let is None:
                        break
                    vnum = lexer.fread_number()
                    if vnum == 0:
                        break
                    
                    obj = {
                        "keywords": lexer.fread_string(),
                        "short_description": lexer.fread_string(),
                        "long_description": lexer.fread_string(),
                        "action_description": lexer.fread_string(),
                    }
                    obj["item_type"] = ITEM_TYPES.get(lexer.fread_number(), "TRASH")
                    obj["extra_flags"] = int_to_flags(lexer.fread_number(), EXTRA_FLAGS)
                    obj["wear_flags"] = int_to_flags(lexer.fread_number(), WEAR_FLAGS)
                    
                    obj["values"] = [
                        lexer.fread_string(),
                        lexer.fread_string(),
                        lexer.fread_string(),
                        lexer.fread_string()
                    ]
                    obj["weight"] = lexer.fread_number()
                    obj["cost"] = lexer.fread_number()
                    obj["level_limited"] = lexer.fread_number()
                    
                    obj["extra_descriptions"] = []
                    obj["applies"] = []
                    obj["hidden_powers"] = []
                    obj["race_combat"] = []
                    obj["spell_buffs"] = []
                    obj["programs"] = []
                    
                    # Object parameters letter loop
                    while True:
                        l = lexer.fread_letter()
                        if l is None:
                            break
                        if l == 'E':
                            obj["extra_descriptions"].append({
                                "keywords": lexer.fread_string(),
                                "description": lexer.fread_string()
                            })
                        elif l == 'A':
                            obj["applies"].append({
                                "type": APPLY_TYPES.get(lexer.fread_number(), "NONE"),
                                "value": lexer.fread_number()
                            })
                        elif l == 'H':
                            obj["hidden_powers"].append({
                                "spell": lexer.fread_string(),
                                "percent": lexer.fread_number(),
                                "level": lexer.fread_number(),
                                "event": HP_TRIGGERS.get(lexer.fread_number(), "HP_WEAR")
                            })
                        elif l == 'L':
                            obj["level"] = lexer.fread_number()
                        elif l == 'T':
                            obj["timer"] = lexer.fread_number()
                        elif l == 'C':
                            obj["cost"] = lexer.fread_number() # override
                        elif l == 'P':
                            obj["permit_classes"] = parse_restrictions(lexer.fread_number(), CLASS_RESTRICTIONS)
                        elif l == 'R':
                            obj["permit_races"] = parse_restrictions(lexer.fread_number(), RACE_RESTRICTIONS)
                        elif l == 'M':
                            m_type = lexer.fread_number()
                            msg_str = lexer.fread_string()
                            if m_type == 0:
                                obj["on_message"] = msg_str
                            elif m_type == 1:
                                obj["off_message"] = msg_str
                            elif m_type == 2:
                                obj["hit_message"] = msg_str
                        elif l == 'X':
                            obj["material"] = lexer.fread_string()
                        elif l == 'V':
                            obj["race_combat"].append({
                                "race": lexer.fread_string(),
                                "hr_mod_q": lexer.fread_number(),
                                "hr_mod_p": lexer.fread_number(),
                                "dr_mod_q": lexer.fread_number(),
                                "dr_mod_p": lexer.fread_number()
                            })
                        elif l == 'F':
                            # spell buffs
                            obj["spell_buffs"].append(lexer.fread_word())
                        elif l == 'U':
                            obj["replica"] = lexer.fread_number()
                        elif l == 'Z':
                            obj["souvenir"] = lexer.fread_number()
                        elif l == '>':
                            # inline objprog
                            prog_type = lexer.fread_word()
                            arglist = lexer.fread_string()
                            lexer.fread_to_eol()
                            comlist = lexer.fread_string()
                            lexer.fread_to_eol()
                            obj["programs"].append({
                                "trigger": prog_type,
                                "argument": arglist,
                                "script": comlist
                            })
                            next_let = lexer.fread_letter()
                            while next_let == '>':
                                prog_type = lexer.fread_word()
                                arglist = lexer.fread_string()
                                lexer.fread_to_eol()
                                comlist = lexer.fread_string()
                                lexer.fread_to_eol()
                                obj["programs"].append({
                                    "trigger": prog_type,
                                    "argument": arglist,
                                    "script": comlist
                                })
                                next_let = lexer.fread_letter()
                            if next_let != '|':
                                lexer.unget_char()
                        else:
                            lexer.unget_char()
                            break
                            
                    data["objects"][vnum] = obj

            elif word == "ROOMDATA":
                while True:
                    let = lexer.fread_letter()
                    if let != '#' or let is None:
                        break
                    vnum = lexer.fread_number()
                    if vnum == 0:
                        break
                    
                    room = {
                        "name": lexer.fread_string(),
                        "description": lexer.fread_string(),
                    }
                    lexer.fread_number() # unused area index
                    room["flags"] = int_to_flags(lexer.fread_number(), ROOM_FLAGS)
                    room["sector"] = SECTOR_TYPES.get(lexer.fread_number(), "INSIDE")
                    
                    room["exits"] = {}
                    room["extra_descriptions"] = []
                    room["programs"] = []
                    room["resets"] = {
                        "mobiles": [],
                        "objects": [],
                        "groups": []
                    }
                    
                    # Room custom parameters loop
                    while True:
                        l = lexer.fread_letter()
                        if l == 'S' or l == 's' or l is None:
                            break
                        elif l == 'F':
                            room["fall"] = lexer.fread_number()
                        elif l == 'D':
                            door_dir = lexer.fread_number()
                            dir_name = ["north", "east", "south", "west", "up", "down"][door_dir]
                            
                            exit_desc = lexer.fread_string()
                            exit_keyw = lexer.fread_string()
                            locks_num = lexer.fread_number()
                            key_vnum = lexer.fread_number()
                            to_room_vnum = lexer.fread_number()
                            
                            room["exits"][dir_name] = {
                                "description": exit_desc,
                                "keywords": exit_keyw,
                                "locks": int_to_flags(locks_num, DOOR_FLAGS),
                                "key": key_vnum,
                                "to_room": to_room_vnum
                            }
                        elif l == 'E':
                            room["extra_descriptions"].append({
                                "keywords": lexer.fread_string(),
                                "description": lexer.fread_string()
                            })
                        elif l == '>':
                            # inline roomprog
                            prog_type = lexer.fread_word()
                            arglist = lexer.fread_string()
                            lexer.fread_to_eol()
                            comlist = lexer.fread_string()
                            lexer.fread_to_eol()
                            room["programs"].append({
                                "trigger": prog_type,
                                "argument": arglist,
                                "script": comlist
                            })
                            next_let = lexer.fread_letter()
                            while next_let == '>':
                                prog_type = lexer.fread_word()
                                arglist = lexer.fread_string()
                                lexer.fread_to_eol()
                                comlist = lexer.fread_string()
                                lexer.fread_to_eol()
                                room["programs"].append({
                                    "trigger": prog_type,
                                    "argument": arglist,
                                    "script": comlist
                                })
                                next_let = lexer.fread_letter()
                            if next_let != '|':
                                lexer.unget_char()
                                
                    data["rooms"][vnum] = room

            elif word == "MOBPROGS":
                while True:
                    m_let = lexer.fread_letter()
                    if m_let == 'S' or m_let is None:
                        break
                    elif m_let == 'M':
                        mob_vnum = lexer.fread_number()
                        prog_file = lexer.fread_word()
                        data["mobprogs"].append({
                            "mob_vnum": mob_vnum,
                            "program_file": prog_file
                        })
                        
            elif word == "RESETS":
                while True:
                    r_let = lexer.fread_letter()
                    if r_let == 'S' or r_let is None:
                        break
                    elif r_let == '*':
                        comment = lexer.fread_string()
                        # ignore comment or store if needed
                        continue
                    
                    # Sequential reset parse
                    # M 0 mob limit room
                    # E 0 obj 0 wear_loc
                    # G 0 obj 0
                    # P 0 obj 0 container
                    # O 0 obj 0 room
                    # D 0 room exit state
                    # R 0 room exit_limit
                    # F follower room protect leader
                    if r_let in ('M', 'E', 'G', 'P', 'O', 'D', 'R', 'F'):
                        reset_entry = {"action": r_let}
                        if r_let == 'M':
                            lexer.fread_number() # 0
                            reset_entry["mob_vnum"] = lexer.fread_number()
                            reset_entry["limit"] = lexer.fread_number()
                            reset_entry["room_vnum"] = lexer.fread_number()
                        elif r_let == 'E':
                            lexer.fread_number() # 0
                            reset_entry["obj_vnum"] = lexer.fread_number()
                            lexer.fread_number() # 0
                            reset_entry["wear_loc"] = RESET_WEAR_LOCS.get(lexer.fread_number(), "CARRY")
                        elif r_let == 'G':
                            lexer.fread_number() # 0
                            reset_entry["obj_vnum"] = lexer.fread_number()
                            lexer.fread_number() # 0
                        elif r_let == 'P':
                            lexer.fread_number() # 0
                            reset_entry["obj_vnum"] = lexer.fread_number()
                            lexer.fread_number() # 0
                            reset_entry["container_vnum"] = lexer.fread_number()
                        elif r_let == 'O':
                            lexer.fread_number() # 0
                            reset_entry["obj_vnum"] = lexer.fread_number()
                            lexer.fread_number() # 0
                            reset_entry["room_vnum"] = lexer.fread_number()
                        elif r_let == 'D':
                            lexer.fread_number() # 0
                            reset_entry["room_vnum"] = lexer.fread_number()
                            door_dir = lexer.fread_number()
                            reset_entry["exit"] = ["north", "east", "south", "west", "up", "down"][door_dir]
                            reset_state = lexer.fread_number()
                            reset_entry["state"] = ["open", "closed", "locked"][reset_state]
                        elif r_let == 'R':
                            lexer.fread_number() # 0
                            reset_entry["room_vnum"] = lexer.fread_number()
                            reset_entry["exit_limit"] = lexer.fread_number()
                        elif r_let == 'F':
                            reset_entry["follower_vnum"] = lexer.fread_number()
                            reset_entry["room_vnum"] = lexer.fread_number()
                            reset_entry["protect"] = (lexer.fread_number() != 0)
                            reset_entry["leader_vnum"] = lexer.fread_number()
                        
                        data["resets"].append(reset_entry)

            elif word == "SHOPS":
                while True:
                    keeper = lexer.fread_number()
                    if keeper == 0:
                        break
                    shop_entry = {
                        "keeper": keeper,
                        "buy_types": [
                            ITEM_TYPES.get(lexer.fread_number(), "TRASH"),
                            ITEM_TYPES.get(lexer.fread_number(), "TRASH"),
                            ITEM_TYPES.get(lexer.fread_number(), "TRASH"),
                            ITEM_TYPES.get(lexer.fread_number(), "TRASH"),
                            ITEM_TYPES.get(lexer.fread_number(), "TRASH")
                        ],
                        "profit_buy": lexer.fread_number(),
                        "profit_sell": lexer.fread_number(),
                        "open_hour": lexer.fread_number(),
                        "close_hour": lexer.fread_number()
                    }
                    data["shops"].append(shop_entry)

            elif word == "SPECIALS":
                while True:
                    spec_let = lexer.fread_letter()
                    if spec_let == 'S' or spec_let is None:
                        break
                    elif spec_let == 'M':
                        mob_vnum = lexer.fread_number()
                        spec_fun = lexer.fread_word()
                        data["specials"].append({
                            "mob_vnum": mob_vnum,
                            "special_function": spec_fun
                        })

    # ==============================================================================
    # NESTING STATE RESOLVER
    # ==============================================================================

    # Nest Specials inside Mobiles
    for spec in data["specials"]:
        mob_vnum = spec["mob_vnum"]
        if mob_vnum in data["mobiles"]:
            data["mobiles"][mob_vnum]["special_function"] = spec["special_function"]

    # Nest Shops inside Mobiles
    for shop in data["shops"]:
        keeper = shop["keeper"]
        if keeper in data["mobiles"]:
            # remove 0/TRASH buy types from list
            clean_buy = [t for t in shop["buy_types"] if t != "TRASH"]
            data["mobiles"][keeper]["shop"] = {
                "buy_types": clean_buy,
                "profit_buy": shop["profit_buy"],
                "profit_sell": shop["profit_sell"],
                "open_hour": shop["open_hour"],
                "close_hour": shop["close_hour"]
            }

    # Nest referenced Mobprogs inside Mobiles
    for prog in data["mobprogs"]:
        mob_vnum = prog["mob_vnum"]
        if mob_vnum in data["mobiles"]:
            if "program_files" not in data["mobiles"][mob_vnum]:
                data["mobiles"][mob_vnum]["program_files"] = []
            data["mobiles"][mob_vnum]["program_files"].append(prog["program_file"])

    # Nest Resets inside Rooms
    last_mob = None
    last_obj = None

    for reset in data["resets"]:
        action = reset["action"]
        if action == 'M':
            room_vnum = reset["room_vnum"]
            if room_vnum in data["rooms"]:
                mob_reset = {
                    "vnum": reset["mob_vnum"],
                    "limit": reset["limit"],
                    "equipment": [],
                    "inventory": []
                }
                data["rooms"][room_vnum]["resets"]["mobiles"].append(mob_reset)
                last_mob = mob_reset
        elif action == 'E':
            if last_mob is not None:
                last_mob["equipment"].append({
                    "vnum": reset["obj_vnum"],
                    "wear_loc": reset["wear_loc"]
                })
        elif action == 'G':
            if last_mob is not None:
                last_mob["inventory"].append({
                    "vnum": reset["obj_vnum"]
                })
        elif action == 'O':
            room_vnum = reset["room_vnum"]
            if room_vnum in data["rooms"]:
                obj_reset = {
                    "vnum": reset["obj_vnum"],
                    "contains": []
                }
                data["rooms"][room_vnum]["resets"]["objects"].append(obj_reset)
                last_obj = obj_reset
        elif action == 'P':
            if last_obj is not None:
                last_obj["contains"].append({
                    "vnum": reset["obj_vnum"]
                })
        elif action == 'D':
            room_vnum = reset["room_vnum"]
            exit_dir = reset["exit"]
            if room_vnum in data["rooms"] and exit_dir in data["rooms"][room_vnum]["exits"]:
                data["rooms"][room_vnum]["exits"][exit_dir]["reset_state"] = reset["state"]
        elif action == 'R':
            room_vnum = reset["room_vnum"]
            if room_vnum in data["rooms"]:
                data["rooms"][room_vnum]["randomize_exits"] = reset["exit_limit"]
        elif action == 'F':
            room_vnum = reset["room_vnum"]
            if room_vnum in data["rooms"]:
                group_reset = {
                    "leader_vnum": reset["leader_vnum"],
                    "follower_vnum": reset["follower_vnum"],
                    "protect": reset["protect"]
                }
                data["rooms"][room_vnum]["resets"]["groups"].append(group_reset)

    return data

# ==============================================================================
# TOML BEAUTIFUL SERIALIZER (Dumper)
# ==============================================================================

def toml_escape(s):
    if s is None:
        return ""
    # We must escape backslashes first, then quotes, then control characters
    s = s.replace('\\', '\\\\').replace('"', '\\"')
    chars = []
    for c in s:
        o = ord(c)
        if o < 32 or o == 127:
            if c == '\n':
                chars.append('\\n')
            elif c == '\r':
                chars.append('\\r')
            elif c == '\t':
                chars.append('\\t')
            else:
                chars.append(f'\\u{o:04X}')
        else:
            chars.append(c)
    return "".join(chars)

def toml_multiline_escape(s):
    if s is None:
        return ""
    s = s.replace('\\', '\\\\').replace('"""', '\\"\\"\\"')
    chars = []
    for c in s:
        o = ord(c)
        if (o < 32 and c not in ('\n', '\r', '\t')) or o == 127:
            chars.append(f'\\u{o:04X}')
        else:
            chars.append(c)
    return "".join(chars)

def dump_toml_string(data):
    lines = []
    
    # 1. Area Metadata
    area = data.get("area", {})
    lines.append("[area]")
    for k in ["name", "identity", "builders", "translator", "modifier", "vnum_start", "vnum_end", "security", "recall", "death", "f_toggle"]:
        if k in area:
            val = area[k]
            if isinstance(val, str):
                lines.append(f'{k} = "{toml_escape(val)}"')
            elif isinstance(val, bool):
                lines.append(f'{k} = {str(val).lower()}')
            else:
                lines.append(f'{k} = {val}')
    lines.append("")
    
    # 2. Helps Section
    helps = data.get("helps", [])
    for h in helps:
        lines.append("[[helps]]")
        lines.append(f'level = {h.get("level", 0)}')
        lines.append(f'keywords = "{toml_escape(h.get("keywords", ""))}"')
        text = h.get("text", "")
        if "\n" in text:
            lines.append(f'text = """\n{toml_multiline_escape(text)}"""')
        else:
            lines.append(f'text = "{toml_escape(text)}"')
        lines.append("")
        
    # 3. Materials Section
    materials = data.get("materials", {})
    for vnum, mat in sorted(materials.items(), key=lambda x: int(x[0])):
        lines.append(f"[materials.{vnum}]")
        lines.append(f'name = "{toml_escape(mat.get("name", ""))}"')
        lines.append(f'cname = "{toml_escape(mat.get("cname", ""))}"')
        lines.append(f'clan_level = {mat.get("clan_level", 0)}')
        
        mat_extra = int_to_flags(mat.get("extra_flags", 0), EXTRA_FLAGS)
        lines.append(f'extra_flags = {json.dumps(mat_extra, ensure_ascii=False)}')
        lines.append(f'clan_cost = {mat.get("clan_cost", 0)}')
        lines.append(f'hr_mod_p = "{toml_escape(mat.get("hr_mod_p", "0"))}"')
        lines.append(f'dr_mod_p = "{toml_escape(mat.get("dr_mod_p", "0"))}"')
        lines.append(f'ac_mod_p = "{toml_escape(mat.get("ac_mod_p", "0"))}"')
        lines.append(f'durability = "{toml_escape(mat.get("durability", "100"))}"')
        lines.append(f'weight_mod_p = {mat.get("weight_mod_p", 100)}')
        lines.append(f'cost_mod_p = {mat.get("cost_mod_p", 100)}')
        lines.append(f'cost_per_day = {mat.get("cost_per_day", 0)}')
        
        if "level_limit" in mat:
            lines.append(f'level_limit = {mat["level_limit"]}')
        if "permit_classes" in mat:
            classes = mat["permit_classes"]
            lines.append(f'permit_classes = {{ type = "{classes["type"]}", list = {json.dumps(classes["list"], ensure_ascii=False)} }}')
        if "permit_races" in mat:
            races = mat["permit_races"]
            lines.append(f'permit_races = {{ type = "{races["type"]}", list = {json.dumps(races["list"], ensure_ascii=False)} }}')
            
        for app in mat.get("applies", []):
            lines.append(f'[[materials.{vnum}.applies]]')
            lines.append(f'type = "{app["type"]}"')
            lines.append(f'value = {app["value"]}')
            
        for sp in mat.get("spells", []):
            lines.append(f'[[materials.{vnum}.spells]]')
            lines.append(f'spell_slot = {sp["spell_slot"]}')
            lines.append(f'percent = {sp["percent"]}')
            lines.append(f'level = {sp["level"]}')
            lines.append(f'time = {sp["time"]}')
            
        for race_c in mat.get("race_combat", []):
            lines.append(f'[[materials.{vnum}.race_combat]]')
            lines.append(f'race = "{toml_escape(race_c["race"])}"')
            lines.append(f'hr_mod_q = {race_c["hr_mod_q"]}')
            lines.append(f'hr_mod_p = {race_c["hr_mod_p"]}')
            lines.append(f'dr_mod_q = {race_c["dr_mod_q"]}')
            lines.append(f'dr_mod_p = {race_c["dr_mod_p"]}')
            
        if mat.get("spell_buffs"):
            lines.append(f'spell_buffs = {json.dumps(mat["spell_buffs"], ensure_ascii=False)}')
        lines.append("")

    # 4. Mobiles Section
    mobiles = data.get("mobiles", {})
    for vnum, mob in sorted(mobiles.items(), key=lambda x: int(x[0])):
        lines.append(f"[mobiles.{vnum}]")
        lines.append(f'keywords = "{toml_escape(mob["keywords"])}"')
        lines.append(f'short_description = "{toml_escape(mob["short_description"])}"')
        lines.append(f'long_description = "{toml_escape(mob["long_description"])}"')
        
        desc = mob["description"]
        if "\n" in desc:
            lines.append(f'description = """\n{toml_multiline_escape(desc)}"""')
        else:
            lines.append(f'description = "{toml_escape(desc)}"')
            
        lines.append(f'act_flags = {json.dumps(mob["act_flags"], ensure_ascii=False)}')
        lines.append(f'affected_flags = {json.dumps(mob["affected_flags"], ensure_ascii=False)}')
        lines.append(f'alignment = {mob["alignment"]}')
        lines.append(f'level = {mob["level"]}')
        lines.append(f'mod_hitplus = {mob["mod_hitplus"]}')
        lines.append(f'mod_mana = {mob["mod_mana"]}')
        
        lines.append(f'mod_hitroll = {mob["mod_hitroll"]}')
        lines.append(f'mod_damroll = {mob["mod_damroll"]}')
        lines.append(f'mod_ac = {mob["mod_ac"]}')
        
        lines.append(f'mod_svs = {mob["mod_svs"]}')
        lines.append(f'dodge = {mob["dodge"]}')
        lines.append(f'mod_pp = {mob["mod_pp"]}')
        
        lines.append(f'gold = {mob["gold"]}')
        lines.append(f'sex = "{toml_escape(mob["sex"])}"')
        
        if "race" in mob:
            lines.append(f'race = "{toml_escape(mob["race"])}"')
        if "class" in mob:
            lines.append(f'class = {json.dumps(mob["class"], ensure_ascii=False)}')
        if "replica" in mob:
            lines.append(f'replica = {mob["replica"]}')
        if "spec_flags" in mob:
            lines.append(f'spec_flags = {json.dumps(mob["spec_flags"], ensure_ascii=False)}')
        if "special_function" in mob:
            lines.append(f'special_function = "{toml_escape(mob["special_function"])}"')
        if "practice_skills" in mob:
            lines.append(f'practice_skills = "{toml_escape(mob["practice_skills"])}"')
        if "transfer_classes" in mob:
            lines.append(f'transfer_classes = "{toml_escape(mob["transfer_classes"])}"')
        if "program_files" in mob:
            lines.append(f'program_files = {json.dumps(mob["program_files"], ensure_ascii=False)}')
            
        if mob.get("action_programs"):
            lines.append("        action_programs = [")
            for act_prog in mob["action_programs"]:
                if "\n" in act_prog:
                    lines.append(f'            """\n{toml_multiline_escape(act_prog)}""",')
                else:
                    lines.append(f'            "{toml_escape(act_prog)}",')
            lines.append("        ]")
            
        for weak in mob.get("weaknesses", []):
            lines.append(f"[[mobiles.{vnum}.weaknesses]]")
            lines.append(f'skill = "{toml_escape(weak["skill"])}"')
            lines.append(f'percent = {weak["percent"]}')
            lines.append(f'damage = {weak["damage"]}')
            
        for app in mob.get("applies", []):
            lines.append(f"[[mobiles.{vnum}.applies]]")
            lines.append(f'type = "{app["type"]}"')
            lines.append(f'degree = {app["degree"]}')
                
        for prog in mob.get("programs", []):
            lines.append(f"[[mobiles.{vnum}.programs]]")
            lines.append(f'trigger = "{toml_escape(prog["trigger"])}"')
            lines.append(f'argument = "{toml_escape(prog["argument"])}"')
            script = prog["script"]
            if "\n" in script:
                lines.append(f'script = """\n{toml_multiline_escape(script)}"""')
            else:
                lines.append(f'script = "{toml_escape(script)}"')
                
        if "shop" in mob:
            shop = mob["shop"]
            lines.append(f"[mobiles.{vnum}.shop]")
            lines.append(f'buy_types = {json.dumps(shop["buy_types"], ensure_ascii=False)}')
            lines.append(f'profit_buy = {shop["profit_buy"]}')
            lines.append(f'profit_sell = {shop["profit_sell"]}')
            lines.append(f'open_hour = {shop["open_hour"]}')
            lines.append(f'close_hour = {shop["close_hour"]}')
            
        lines.append("")

    # 5. Objects Section
    objects = data.get("objects", {})
    for vnum, obj in sorted(objects.items(), key=lambda x: int(x[0])):
        lines.append(f"[objects.{vnum}]")
        lines.append(f'keywords = "{toml_escape(obj["keywords"])}"')
        lines.append(f'short_description = "{toml_escape(obj["short_description"])}"')
        lines.append(f'long_description = "{toml_escape(obj["long_description"])}"')
        lines.append(f'action_description = "{toml_escape(obj["action_description"])}"')
        lines.append(f'item_type = "{toml_escape(obj["item_type"])}"')
        lines.append(f'extra_flags = {json.dumps(obj["extra_flags"], ensure_ascii=False)}')
        lines.append(f'wear_flags = {json.dumps(obj["wear_flags"], ensure_ascii=False)}')
        
        lines.append(f'values = {json.dumps(obj["values"], ensure_ascii=False)}')
        lines.append(f'weight = {obj["weight"]}')
        lines.append(f'cost = {obj["cost"]}')
        lines.append(f'level_limited = {obj["level_limited"]}')
        
        if "level" in obj:
            lines.append(f'level = {obj["level"]}')
        if "timer" in obj:
            lines.append(f'timer = {obj["timer"]}')
        if "permit_classes" in obj:
            classes = obj["permit_classes"]
            lines.append(f'permit_classes = {{ type = "{classes["type"]}", list = {json.dumps(classes["list"], ensure_ascii=False)} }}')
        if "permit_races" in obj:
            races = obj["permit_races"]
            lines.append(f'permit_races = {{ type = "{races["type"]}", list = {json.dumps(races["list"], ensure_ascii=False)} }}')
        if "material" in obj:
            lines.append(f'material = "{toml_escape(obj["material"])}"')
        if "replica" in obj:
            lines.append(f'replica = {obj["replica"]}')
        if "souvenir" in obj:
            lines.append(f'souvenir = {obj["souvenir"]}')
            
        if "on_message" in obj:
            lines.append(f'on_message = "{toml_escape(obj["on_message"])}"')
        if "off_message" in obj:
            lines.append(f'off_message = "{toml_escape(obj["off_message"])}"')
        if "hit_message" in obj:
            lines.append(f'hit_message = "{toml_escape(obj["hit_message"])}"')
            
        if obj.get("spell_buffs"):
            lines.append(f'spell_buffs = {json.dumps(obj["spell_buffs"], ensure_ascii=False)}')
            
        for ed in obj.get("extra_descriptions", []):
            lines.append(f"[[objects.{vnum}.extra_descriptions]]")
            lines.append(f'keywords = "{toml_escape(ed["keywords"])}"')
            desc = ed["description"]
            if "\n" in desc:
                lines.append(f'description = """\n{toml_multiline_escape(desc)}"""')
            else:
                lines.append(f'description = "{toml_escape(desc)}"')
                
        for app in obj.get("applies", []):
            lines.append(f"[[objects.{vnum}.applies]]")
            lines.append(f'type = "{app["type"]}"')
            lines.append(f'value = {app["value"]}')
            
        for hp in obj.get("hidden_powers", []):
            lines.append(f"[[objects.{vnum}.hidden_powers]]")
            lines.append(f'spell = "{toml_escape(hp["spell"])}"')
            lines.append(f'percent = {hp["percent"]}')
            lines.append(f'level = {hp["level"]}')
            lines.append(f'event = "{toml_escape(hp["event"])}"')
            
        for race_c in obj.get("race_combat", []):
            lines.append(f"[[objects.{vnum}.race_combat]]")
            lines.append(f'race = "{toml_escape(race_c["race"])}"')
            lines.append(f'hr_mod_q = {race_c["hr_mod_q"]}')
            lines.append(f'hr_mod_p = {race_c["hr_mod_p"]}')
            lines.append(f'dr_mod_q = {race_c["dr_mod_q"]}')
            lines.append(f'dr_mod_p = {race_c["dr_mod_p"]}')
            
        for prog in obj.get("programs", []):
            lines.append(f"[[objects.{vnum}.programs]]")
            lines.append(f'trigger = "{toml_escape(prog["trigger"])}"')
            lines.append(f'argument = "{toml_escape(prog["argument"])}"')
            script = prog["script"]
            if "\n" in script:
                lines.append(f'script = """\n{toml_multiline_escape(script)}"""')
            else:
                lines.append(f'script = "{toml_escape(script)}"')
                
        lines.append("")

    # 6. Rooms Section
    rooms = data.get("rooms", {})
    for vnum, room in sorted(rooms.items(), key=lambda x: int(x[0])):
        lines.append(f"[rooms.{vnum}]")
        lines.append(f'name = "{toml_escape(room["name"])}"')
        
        desc = room["description"]
        if "\n" in desc:
            lines.append(f'description = """\n{toml_multiline_escape(desc)}"""')
        else:
            lines.append(f'description = "{toml_escape(desc)}"')
            
        lines.append(f'flags = {json.dumps(room["flags"], ensure_ascii=False)}')
        lines.append(f'sector = "{toml_escape(room["sector"])}"')
        
        if "fall" in room:
            lines.append(f'fall = {room["fall"]}')
        if "randomize_exits" in room:
            rx = room["randomize_exits"]
            if isinstance(rx, int):
                lines.append(f'randomize_exits = {rx}')
            else:
                lines.append(f'randomize_exits = "{toml_escape(str(rx))}"')
            
        for ed in room.get("extra_descriptions", []):
            lines.append(f"[[rooms.{vnum}.extra_descriptions]]")
            lines.append(f'keywords = "{toml_escape(ed["keywords"])}"')
            desc = ed["description"]
            if "\n" in desc:
                lines.append(f'description = """\n{toml_multiline_escape(desc)}"""')
            else:
                lines.append(f'description = "{toml_escape(desc)}"')
                
        for dir_name, ex in sorted(room.get("exits", {}).items()):
            lines.append(f"[rooms.{vnum}.exits.{dir_name}]")
            lines.append(f'description = "{toml_escape(ex["description"])}"')
            lines.append(f'keywords = "{toml_escape(ex["keywords"])}"')
            lines.append(f'locks = {json.dumps(ex["locks"], ensure_ascii=False)}')
            lines.append(f'key = {ex["key"]}')
            lines.append(f'to_room = {ex["to_room"]}')
            if "reset_state" in ex:
                lines.append(f'reset_state = "{toml_escape(ex["reset_state"])}"')
                
        for prog in room.get("programs", []):
            lines.append(f"[[rooms.{vnum}.programs]]")
            lines.append(f'trigger = "{toml_escape(prog["trigger"])}"')
            lines.append(f'argument = "{toml_escape(prog["argument"])}"')
            script = prog["script"]
            if "\n" in script:
                lines.append(f'script = """\n{toml_multiline_escape(script)}"""')
            else:
                lines.append(f'script = "{toml_escape(script)}"')
                
        resets = room.get("resets", {})
        
        for mob_reset in resets.get("mobiles", []):
            lines.append(f"[[rooms.{vnum}.resets.mobiles]]")
            lines.append(f'vnum = {mob_reset["vnum"]}')
            lines.append(f'limit = {mob_reset["limit"]}')
            
            for eq in mob_reset.get("equipment", []):
                lines.append(f"[[rooms.{vnum}.resets.mobiles.equipment]]")
                lines.append(f'vnum = {eq["vnum"]}')
                lines.append(f'wear_loc = "{toml_escape(eq["wear_loc"])}"')
                
            for inv in mob_reset.get("inventory", []):
                lines.append(f"[[rooms.{vnum}.resets.mobiles.inventory]]")
                lines.append(f'vnum = {inv["vnum"]}')
                
        for obj_reset in resets.get("objects", []):
            lines.append(f"[[rooms.{vnum}.resets.objects]]")
            lines.append(f'vnum = {obj_reset["vnum"]}')
            
            for c_item in obj_reset.get("contains", []):
                lines.append(f"[[rooms.{vnum}.resets.objects.contains]]")
                lines.append(f'vnum = {c_item["vnum"]}')
                
        for gr in resets.get("groups", []):
            lines.append(f"[[rooms.{vnum}.resets.groups]]")
            lines.append(f'leader_vnum = {gr["leader_vnum"]}')
            lines.append(f'follower_vnum = {gr["follower_vnum"]}')
            lines.append(f'protect = {str(gr["protect"]).lower()}')
            
        lines.append("")

    return "\n".join(lines)

# ==============================================================================
# COMPILER (Generates legacy .are file from loaded TOML Dict)
# ==============================================================================

def to_legacy_string(str_val):
    # normalize newline characters and append ~ character suffix
    if not str_val:
        return "~"
    return f"{str_val}~"

def compile_toml_to_are(data):
    lines = []
    
    # 1. #AREADATA
    area = data.get("area", {})
    lines.append("#AREADATA")
    if "name" in area:
        lines.append(f"Name        {to_legacy_string(area['name'])}")
    if "identity" in area:
        lines.append(f"Identity    {to_legacy_string(area['identity'])}")
    if "builders" in area:
        lines.append(f"Builders    {to_legacy_string(area['builders'])}")
    if "translator" in area:
        lines.append(f"Translator  {to_legacy_string(area['translator'])}")
    if "modifier" in area:
        lines.append(f"Modifier    {to_legacy_string(area['modifier'])}")
    if "vnum_start" in area and "vnum_end" in area:
        lines.append(f"VNUMs       {area['vnum_start']} {area['vnum_end']}")
    if "security" in area:
        lines.append(f"Security    {area['security']}")
    if "recall" in area:
        lines.append(f"Recall      {area['recall']}")
    if "death" in area:
        lines.append(f"Death       {area['death']}")
    if "f_toggle" in area:
        lines.append(f"F_toggle    {1 if area['f_toggle'] else 0}")
    lines.append("End")
    lines.append("")
    lines.append("")
    lines.append("")
    
    # 2. #HELPS
    helps = data.get("helps", [])
    if helps:
        lines.append("#HELPS")
        for h in helps:
            lines.append(f"{h.get('level', 0)} {to_legacy_string(h.get('keywords', ''))}")
            lines.append(to_legacy_string(h.get("text", "")))
        lines.append("0 $~")
        lines.append("")
        lines.append("")
        
    # 3. #MATERIAL
    materials = data.get("materials", {})
    if materials:
        lines.append("#MATERIAL")
        for vnum, mat in sorted(materials.items(), key=lambda x: int(x[0])):
            lines.append(f"#{vnum}")
            lines.append(to_legacy_string(mat.get("name", "")))
            lines.append(to_legacy_string(mat.get("cname", "")))
            lines.append("~") # unused
            lines.append("~") # unused
            
            extra_bits = flags_to_int(mat.get("extra_flags", []), EXTRA_FLAGS)
            lines.append(f"{mat.get('clan_level', 0)} {extra_bits} {mat.get('clan_cost', 0)}")
            
            lines.append(f"{to_legacy_string(mat.get('hr_mod_p', '0'))} {to_legacy_string(mat.get('dr_mod_p', '0'))} {to_legacy_string(mat.get('ac_mod_p', '0'))} {to_legacy_string(mat.get('durability', '100'))}")
            lines.append(f"{mat.get('weight_mod_p', 100)} {mat.get('cost_mod_p', 100)} {mat.get('cost_per_day', 0)}")
            
            for app in mat.get("applies", []):
                rev_app_types = {v: k for k, v in APPLY_TYPES.items()}
                app_num = rev_app_types.get(app["type"], 0)
                lines.append(f"A {app_num} {app['value']}")
                
            for sp in mat.get("spells", []):
                lines.append(f"H {sp['spell_slot']} {sp['percent']} {sp['level']} {sp['time']}")
                
            if "level_limit" in mat:
                lines.append(f"L {mat['level_limit']}")
            if "permit_classes" in mat:
                class_val = build_restrictions(mat["permit_classes"], CLASS_RESTRICTIONS)
                lines.append(f"P {class_val}")
            if "permit_races" in mat:
                race_val = build_restrictions(mat["permit_races"], RACE_RESTRICTIONS)
                lines.append(f"R {race_val}")
                
            for race_c in mat.get("race_combat", []):
                lines.append(f"V {to_legacy_string(race_c['race'])} {race_c['hr_mod_q']} {race_c['hr_mod_p']} {race_c['dr_mod_q']} {race_c['dr_mod_p']}")
                
            for spell_b in mat.get("spell_buffs", []):
                lines.append(f"F {spell_b}")
                
        lines.append("#0")
        lines.append("")
        lines.append("")
        
    # 4. #MOBILES
    mobiles = data.get("mobiles", {})
    if mobiles:
        lines.append("#MOBILES")
        for vnum, mob in sorted(mobiles.items(), key=lambda x: int(x[0])):
            lines.append(f"#{vnum}")
            lines.append(to_legacy_string(mob["keywords"]))
            lines.append(to_legacy_string(mob["short_description"]))
            lines.append(to_legacy_string(mob["long_description"]))
            lines.append(to_legacy_string(mob["description"]))
            
            act_bits = flags_to_int(mob["act_flags"], ACT_FLAGS)
            affected_bits = flags_to_int(mob["affected_flags"], AFF_FLAGS)
            lines.append(f"{act_bits} {affected_bits} {mob['alignment']} S")
            
            # level, mod_hitplus, mod_mana, combat_stats, saving_stats, dodge, mod_pp
            # elftown.are format: 36 1000 0 0d0+0 0d0+0
            lines.append(f"{mob['level']} {mob['mod_hitplus']} {mob['mod_mana']} {mob['mod_hitroll']}d{mob['mod_damroll']}+{mob['mod_ac']} {mob['mod_svs']}d{mob['dodge']}+{mob['mod_pp']}")
            
            lines.append(f"{mob['gold']} 0") # gold, xp/unused
            
            sex_num = 2 if mob["sex"] == "female" else (1 if mob["sex"] == "male" else 0)
            lines.append(f"0 8 {sex_num}") # unused position, unused position, sex
            
            if "race" in mob:
                lines.append(f"R {to_legacy_string(mob['race'])}")
            if "class" in mob:
                class_bits = flags_to_int(mob["class"], MOB_CLASSES)
                lines.append(f"C {class_bits}")
            if "replica" in mob:
                lines.append(f"H {mob['replica']}")
            if "spec_flags" in mob:
                spec_bits = flags_to_int(mob["spec_flags"], P_FLAGS)
                lines.append(f"P {spec_bits}")
                
            for weak in mob.get("weaknesses", []):
                lines.append(f"W {to_legacy_string(weak['skill'])} {weak['percent']} {weak['damage']}")
                
            for app in mob.get("applies", []):
                rev_app_types = {v: k for k, v in APPLY_TYPES.items()}
                app_num = rev_app_types.get(app["type"], 0)
                lines.append(f"S {app_num} {app['degree']}")
                
            if "practice_skills" in mob:
                lines.append(f"K {to_legacy_string(mob['practice_skills'])}")
            if "transfer_classes" in mob:
                lines.append(f"J {to_legacy_string(mob['transfer_classes'])}")
                
            for act_prog in mob.get("action_programs", []):
                lines.append(f"A {to_legacy_string(act_prog)}")
                
            # Embedded inline MobProgs
            if mob.get("programs"):
                for prog in mob["programs"]:
                    lines.append(f">{prog['trigger']} {to_legacy_string(prog['argument'])}")
                    lines.append(to_legacy_string(prog["script"]))
                lines.append("|")
                
        lines.append("#0")
        lines.append("")
        lines.append("")
        
    # 5. #OBJECTS
    objects = data.get("objects", {})
    if objects:
        lines.append("#OBJECTS")
        for vnum, obj in sorted(objects.items(), key=lambda x: int(x[0])):
            lines.append(f"#{vnum}")
            lines.append(to_legacy_string(obj["keywords"]))
            lines.append(to_legacy_string(obj["short_description"]))
            lines.append(to_legacy_string(obj["long_description"]))
            lines.append(to_legacy_string(obj["action_description"]))
            
            rev_item_types = {v: k for k, v in ITEM_TYPES.items()}
            item_num = rev_item_types.get(obj["item_type"], 13) # fallback TRASH
            extra_bits = flags_to_int(obj["extra_flags"], EXTRA_FLAGS)
            wear_bits = flags_to_int(obj["wear_flags"], WEAR_FLAGS)
            lines.append(f"{item_num} {extra_bits} {wear_bits}")
            
            # Object value[0-3] strings with tildes
            vals = [to_legacy_string(v) for v in obj["values"]]
            lines.append(" ".join(vals))
            
            lines.append(f"{obj['weight']} {obj['cost']} {obj['level_limited']}")
            
            for ed in obj.get("extra_descriptions", []):
                lines.append(f"E\n{to_legacy_string(ed['keywords'])}")
                lines.append(to_legacy_string(ed["description"]))
                
            for app in obj.get("applies", []):
                rev_app_types = {v: k for k, v in APPLY_TYPES.items()}
                app_num = rev_app_types.get(app["type"], 0)
                lines.append(f"A\n{app_num} {app['value']}")
                
            for hp in obj.get("hidden_powers", []):
                rev_hp_triggers = {v: k for k, v in HP_TRIGGERS.items()}
                trig_num = rev_hp_triggers.get(hp["event"], 1)
                lines.append(f"H {to_legacy_string(hp['spell'])} {hp['percent']} {hp['level']} {trig_num}")
                
            if "level" in obj:
                lines.append(f"L {obj['level']}")
            if "timer" in obj:
                lines.append(f"T {obj['timer']}")
            if "cost" in obj:
                lines.append(f"C {obj['cost']}")
            if "permit_classes" in obj:
                class_val = build_restrictions(obj["permit_classes"], CLASS_RESTRICTIONS)
                lines.append(f"P {class_val}")
            if "permit_races" in obj:
                race_val = build_restrictions(obj["permit_races"], RACE_RESTRICTIONS)
                lines.append(f"R {race_val}")
                
            if "on_message" in obj:
                lines.append(f"M 0 {to_legacy_string(obj['on_message'])}")
            if "off_message" in obj:
                lines.append(f"M 1 {to_legacy_string(obj['off_message'])}")
            if "hit_message" in obj:
                lines.append(f"M 2 {to_legacy_string(obj['hit_message'])}")
                
            if "material" in obj:
                lines.append(f"X {to_legacy_string(obj['material'])}")
                
            for race_c in obj.get("race_combat", []):
                lines.append(f"V {to_legacy_string(race_c['race'])} {race_c['hr_mod_q']} {race_c['hr_mod_p']} {race_c['dr_mod_q']} {race_c['dr_mod_p']}")
                
            for spell_b in obj.get("spell_buffs", []):
                lines.append(f"F {spell_b}")
                
            if "replica" in obj:
                lines.append(f"U {obj['replica']}")
            if "souvenir" in obj:
                lines.append(f"Z {obj['souvenir']}")
                
            # Embedded inline ObjProgs
            if obj.get("programs"):
                for prog in obj["programs"]:
                    lines.append(f">{prog['trigger']} {to_legacy_string(prog['argument'])}")
                    lines.append(to_legacy_string(prog["script"]))
                lines.append("|")
                
        lines.append("#0")
        lines.append("")
        lines.append("")
        
    # 6. #ROOMDATA
    rooms = data.get("rooms", {})
    if rooms:
        lines.append("#ROOMDATA")
        for vnum, room in sorted(rooms.items(), key=lambda x: int(x[0])):
            lines.append(f"#{vnum}")
            lines.append(to_legacy_string(room["name"]))
            lines.append(to_legacy_string(room["description"]))
            
            room_bits = flags_to_int(room["flags"], ROOM_FLAGS)
            rev_sectors = {v: k for k, v in SECTOR_TYPES.items()}
            sector_num = rev_sectors.get(room["sector"], 0)
            lines.append(f"0 {room_bits} {sector_num}")
            
            if "fall" in room:
                lines.append(f"F {room['fall']}")
                
            for dir_name, ex in sorted(room.get("exits", {}).items()):
                dir_num = ["north", "east", "south", "west", "up", "down"].index(dir_name)
                lines.append(f"D{dir_num}")
                lines.append(to_legacy_string(ex["description"]))
                lines.append(to_legacy_string(ex["keywords"]))
                
                lock_bits = flags_to_int(ex["locks"], DOOR_FLAGS)
                lines.append(f"{lock_bits} {ex['key']} {ex['to_room']}")
                
            for ed in room.get("extra_descriptions", []):
                lines.append(f"E\n{to_legacy_string(ed['keywords'])}")
                lines.append(to_legacy_string(ed["description"]))
                
            # Embedded inline RoomProgs
            if room.get("programs"):
                for prog in room["programs"]:
                    lines.append(f">{prog['trigger']} {to_legacy_string(prog['argument'])}")
                    lines.append(to_legacy_string(prog["script"]))
                lines.append("|")
                
            lines.append("S")
            
        lines.append("#0")
        lines.append("")
        lines.append("")

    # 7. #MOBPROGS (referenced external files)
    # Reassemble external mobprogs from mobiles
    external_mobprogs = []
    for vnum, mob in sorted(mobiles.items(), key=lambda x: int(x[0])):
        for prog_file in mob.get("program_files", []):
            external_mobprogs.append((vnum, prog_file))
            
    if external_mobprogs:
        lines.append("#MOBPROGS")
        for vnum, prog_file in external_mobprogs:
            lines.append(f"M {vnum} {prog_file}")
        lines.append("S")
        lines.append("")
        lines.append("")

    # 8. #SPECIALS
    # Reassemble specials from mobiles
    specials = []
    for vnum, mob in sorted(mobiles.items(), key=lambda x: int(x[0])):
        if "special_function" in mob:
            specials.append((vnum, mob["special_function"]))
            
    if specials:
        lines.append("#SPECIALS")
        for vnum, spec_fun in specials:
            lines.append(f"M {vnum} {spec_fun}")
        lines.append("S")
        lines.append("")
        lines.append("")

    # 9. #RESETS
    # Reassemble resets from nested rooms resets
    resets_lines = []
    
    # We output resets in room VNUM order
    for room_vnum, room in sorted(rooms.items(), key=lambda x: int(x[0])):
        
        # 1. Door resets (D resets)
        for dir_name, ex in sorted(room.get("exits", {}).items()):
            if "reset_state" in ex:
                dir_num = ["north", "east", "south", "west", "up", "down"].index(dir_name)
                state_num = ["open", "closed", "locked"].index(ex["reset_state"])
                resets_lines.append(f"D 0 {room_vnum} {dir_num} {state_num}")
                
        # 2. Randomize exits reset (R resets)
        if "randomize_exits" in room:
            rx = room["randomize_exits"]
            if isinstance(rx, int):
                dir_num = rx
            else:
                dirs = ["north", "east", "south", "west", "up", "down"]
                dir_num = dirs.index(rx) if rx in dirs else 6
            resets_lines.append(f"R 0 {room_vnum} {dir_num}")
            
        # 3. Object resets (O & P resets)
        resets = room.get("resets", {})
        for obj_reset in resets.get("objects", []):
            obj_vnum = obj_reset["vnum"]
            resets_lines.append(f"O 0 {obj_vnum} 0 {room_vnum}")
            for c_item in obj_reset.get("contains", []):
                resets_lines.append(f"P 0 {c_item['vnum']} 0 {obj_vnum}")
                
        # 4. Mob resets (M, E, G resets)
        for mob_reset in resets.get("mobiles", []):
            mob_vnum = mob_reset["vnum"]
            limit = mob_reset["limit"]
            resets_lines.append(f"M 0 {mob_vnum} {limit} {room_vnum}")
            
            # equipment
            for eq in mob_reset.get("equipment", []):
                rev_reset_wear_locs = {v: k for k, v in RESET_WEAR_LOCS.items()}
                wear_num = rev_reset_wear_locs.get(eq["wear_loc"], -1)
                resets_lines.append(f"E 0 {eq['vnum']} 0 {wear_num}")
                
            # inventory
            for inv in mob_reset.get("inventory", []):
                resets_lines.append(f"G 0 {inv['vnum']} 0")
                
        # 5. Group follows resets (F resets)
        for gr in resets.get("groups", []):
            prot_val = 1 if gr["protect"] else 0
            resets_lines.append(f"F {gr['follower_vnum']} {room_vnum} {prot_val} {gr['leader_vnum']}")
            
    if resets_lines:
        lines.append("#RESETS")
        for r_line in resets_lines:
            lines.append(r_line)
        lines.append("S")
        lines.append("")
        lines.append("")

    # 10. #SHOPS
    # Reassemble shops from mobiles
    shops = []
    for vnum, mob in sorted(mobiles.items(), key=lambda x: int(x[0])):
        if "shop" in mob:
            shop = mob["shop"]
            shops.append((vnum, shop))
            
    if shops:
        lines.append("#SHOPS")
        for keeper, shop in shops:
            buy_types_nums = []
            rev_item_types = {v: k for k, v in ITEM_TYPES.items()}
            for t in shop["buy_types"][:5]:
                buy_types_nums.append(rev_item_types.get(t, 0))
            # Pad with 0 up to 5 elements
            while len(buy_types_nums) < 5:
                buy_types_nums.append(0)
            
            buy_str = " ".join(str(n) for n in buy_types_nums)
            lines.append(f"{keeper} {buy_str} {shop['profit_buy']} {shop['profit_sell']} {shop['open_hour']} {shop['close_hour']}")
        lines.append("0")
        lines.append("")
        lines.append("")

    # 11. End of File terminator
    lines.append("#$")
    lines.append("")
    
    return "\n".join(lines)

# ==============================================================================
# CLI INTERFACE
# ==============================================================================

def main():
    parser = argparse.ArgumentParser(description="ROC MUD Area File (.are) <-> TOML Converter")
    subparsers = parser.add_subparsers(dest="command", required=True)
    
    # Decompile Command
    decompile_parser = subparsers.add_parser("decompile", help="Decompile .are to .toml")
    decompile_parser.add_argument("input_file", help="Path to legacy .are file")
    decompile_parser.add_argument("output_file", help="Path to output .toml file")
    
    # Compile Command
    compile_parser = subparsers.add_parser("compile", help="Compile .toml back to .are")
    compile_parser.add_argument("input_file", help="Path to source .toml file")
    compile_parser.add_argument("output_file", help="Path to output compiled .are file")
    
    args = parser.parse_args()
    
    if args.command == "decompile":
        print(f"Reading legacy area file: {args.input_file}")
        with open(args.input_file, "r", encoding="utf-8") as f:
            content = f.read()
        
        print("Parsing and decompiling...")
        data = decompile_area(content)
        
        print(f"Serializing to TOML and writing to: {args.output_file}")
        toml_content = dump_toml_string(data)
        with open(args.output_file, "w", encoding="utf-8") as f:
            f.write(toml_content)
        print("De-compilation completed successfully.")
        
    elif args.command == "compile":
        print(f"Reading TOML area file: {args.input_file}")
        # To avoid external dependencies we import standard tomllib built into Python 3.11+
        import tomllib
        with open(args.input_file, "rb") as f:
            data = tomllib.load(f)
            
        print("Compiling and generating legacy area file...")
        are_content = compile_toml_to_are(data)
        
        print(f"Writing legacy area format to: {args.output_file}")
        with open(args.output_file, "w", encoding="utf-8", newline="\n") as f:
            f.write(are_content)
        print("Compilation completed successfully.")

if __name__ == "__main__":
    main()
