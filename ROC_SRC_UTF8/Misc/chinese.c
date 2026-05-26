
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.			   *
 *									   *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and	   *
 *  Mitchell Tse.							   *
 *									   *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.	   *
 *									   *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in	   *
 *  'license.txt', as well as the Envy license in 'license.nvy'.	   *
 *  In particular, you may not remove either of these copyright notices.   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define __ROC_CHINESE_C__
#include "ansi.h"
#include "merc.h"

// is_chinese_char: Checks if a byte is a valid Chinese character byte in Big5/UTF8.
// is_chinese_char: 檢查位元組是否為合法的 Big5 或 UTF-8 中文字元位元組。
bool is_chinese_char(unsigned char c, bool cFlag) {
    if (cFlag)
        return FALSE;

    if (c > '\x7F') // always true: && c <= '\xff' )
        return TRUE;

    return FALSE;
}

// is_brief: Checks if brief description mode should be used between characters.
// is_brief: 檢查在顯示角色時是否應對特定受害者使用簡短描述模式。
bool is_brief(CHAR_DATA *ch, CHAR_DATA *victim) {
    if (!ch)
        return (IS_NPC(victim) ? FALSE : IS_SET(ch->act, PLR_BRIEF));
    if (!victim) {
        if (IS_NPC(ch))
            return TRUE;
        else {
            return (IS_SET(ch->act, PLR_BRIEF));
        }
    }
    if (IS_NPC(ch)) {
        if (IS_NPC(victim))
            return TRUE;
        else {
            return (IS_SET(victim->act, PLR_BRIEF));
        }
    } else {
        return (IS_SET(ch->act, PLR_BRIEF));
    }

    return FALSE;
}
// is_chinese: Checks if a Chinese-specific translation or character display rule applies.
// is_chinese: 檢查是否對特定的受害者應用中文字元或語言顯示規則。
bool is_chinese(CHAR_DATA *ch, CHAR_DATA *victim) {
    if (!ch)
        return TRUE;
    if (!victim) {
        if (IS_NPC(ch))
            return TRUE;
        else {
            return (IS_SET(ch->act, PLR_CHINESE)) ? TRUE : FALSE;
        }
    }
    if (IS_NPC(ch)) {
        if (IS_NPC(victim))
            return TRUE;
        else {
            return (IS_SET(victim->act, PLR_CHINESE)) ? TRUE : FALSE;
        }
    } else {
        return (IS_SET(ch->act, PLR_CHINESE)) ? TRUE : FALSE;
    }

    return TRUE;
}

// strip_ansi: Strips ANSI color control escape codes from a source string into a destination buffer.
// strip_ansi: 將來源字串中的 ANSI 著色控制碼濾除，並輸出至目標緩衝區。
void strip_ansi(char *src, char *dup) {
    int i = 0;
    int j = 0;
    while (src[i] != '\0') {
        if (src[i] == '') {
            while (src[i] != 'm') {
                i++;
            }
            i++;
        } else {
            dup[j++] = src[i++];
        }
    }
    dup[j] = '\0';
}

/*
 * Return ascii name of an item type.
 */
char *c_item_type_name(OBJ_DATA *obj) {
    OBJ_DATA *in_obj;
    char      buf[MAX_STRING_LENGTH];

    switch (obj->item_type) {
    case ITEM_LIGHT:
        return "光源";
    case ITEM_SCROLL:
        return "卷軸";
    case ITEM_WAND:
        return "魔杖";
    case ITEM_STAFF:
        return "法杖";
    case ITEM_WEAPON:
        return "武器";
    case ITEM_TREASURE:
        return "寶藏";
    case ITEM_ARMOR:
        return "裝甲";
    case ITEM_POTION:
        return "藥劑";
    case ITEM_FURNITURE:
        return "家俱";
    case ITEM_TRASH:
        return "垃圾";
    case ITEM_CONTAINER:
        return "容器";
    case ITEM_DRINK_CON:
        return "飲水容器";
    case ITEM_KEY:
        return "鑰匙";
    case ITEM_FOOD:
        return "食物";
    case ITEM_MONEY:
        return "錢幣";
    case ITEM_BOAT:
        return "船隻";
    case ITEM_CORPSE_NPC:
        return "NPC屍體";
    case ITEM_CORPSE_PC:
        return "玩者屍體";
    case ITEM_FOUNTAIN:
        return "泉水";
    case ITEM_PILL:
        return "藥丸";
    case ITEM_ARROW:
        return "弓矢";
    case ITEM_DART:
        return "暗器";
    case ITEM_HORSE:
        return "坐騎";
    case ITEM_AMMO:
        return "彈藥"; // 2022/04/29
    }

    for (in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj)
        ;

    if (in_obj->carried_by)
        sprintf(buf, "Item_type_name: unknown type %d from %s owned by %s.",
                obj->item_type, obj->name, obj->carried_by->name);
    else
        sprintf(buf,
                "Item_type_name: unknown type %d from %s owned by <不明>.",
                obj->item_type, obj->name);

    bug(buf, 0);
    return "<不明>";
}

/*
 * Return ascii name of an affect location.
 */
char *c_affect_loc_name(int location) {
    switch (location) {
    case APPLY_NONE:
        return "無";
    case APPLY_STR:
        return "力量";
    case APPLY_DEX:
        return "敏捷";
    case APPLY_INT:
        return "智力";
    case APPLY_WIS:
        return "智慧";
    case APPLY_CON:
        return "體質";
    case APPLY_SEX:
        return "性別";
    case APPLY_CLASS:
        return "職業";
    case APPLY_LEVEL:
        return "級別";
    case APPLY_AGE:
        return "年齡";
    case APPLY_HEIGHT:
        return "體型";
    case APPLY_WEIGHT:
        return "負重";
    case APPLY_MANA:
        return "法力";
    case APPLY_HIT:
        return "生命力";
    case APPLY_MOVE:
        return "移動力";
    case APPLY_GOLD:
        return "金錢";
    case APPLY_EXP:
        return "經驗值";
    case APPLY_AC:
        return "防禦力";
    case APPLY_HITROLL:
        return "命中率";
    case APPLY_DAMROLL:
        return "傷害力";
    case APPLY_SAVING_PARA:
        return "麻痺回避率";
    case APPLY_SAVING_LIFE:
        return "生命回避率"; // 用於 mar 不死 2020/10/28
    case APPLY_SAVING_PETRI:
        return "石化回避率";
    case APPLY_SAVING_BREATH:
        return "龍息回避率";
    case APPLY_SAVING_SPELL:
        return "法術回避率";
    case APPLY_RACE:
        return "種族";
    case APPLY_POWER_POTENTIAL:
        return "魔法潛力";
    case APPLY_DODGE:
        return "閃避率";
    case APPLY_RDAMAGE:
        return "抗物理傷害";
    case APPLY_RMAGIC:
        return "抗魔法傷害";
    case APPLY_ADAMAGE:
        return "物理傷害力";
    case APPLY_AMAGIC:
        return "魔法傷害力";
    case APPLY_SAVING_ANCIENT:
        return "上古法術回避率";
    case APPLY_SAVING_SPIRIT:
        return "靈魂法術回避率";
    case APPLY_SAVING_FIRE:
        return "火炎法術回避率";
    case APPLY_SAVING_COLD:
        return "寒冷法術回避率";
    case APPLY_SAVING_LIGHTNING:
        return "閃電法術回避率";
    case APPLY_SAVING_EARTH:
        return "大地法術回避率";
    case APPLY_SAVING_WIND:
        return "風法術回避率";
    case APPLY_SAVING_SAINT:
        return "神聖法術回避率";
    case APPLY_SAVING_DARK:
        return "黑暗法術回避率";
    case APPLY_SAVING_POISON:
        return "劇毒傷害";
    case APPLY_SAVING_ELEMENT:
        return "元素回避率";
    case APPLY_SAVING_GOOD:
        return "善良法術回避率";
    case APPLY_SAVING_EVIL:
        return "邪惡法術回避率";
    case APPLY_DOB_SAVING_FLASH:
        return "光爆抵抗力";
    case APPLY_SAVING_ILLUSION:
        return "幻術回避率";
    case APPLY_ACTION_ABILITY:
        return "行動力";
    case APPLY_WEAPON_SPELL:
        return "法術屬性傷害"; // 武器額外追加法術威力 2021/09/19
    case APPLY_WEAPON_WIND:
        return "風屬性傷害"; // 武器額外追加風系威力
    case APPLY_WEAPON_FIRE:
        return "火屬性傷害"; // 武器額外追加火系威力
    case APPLY_WEAPON_COLD:
        return "水屬性傷害"; // 武器額外追加水系威力
    case APPLY_WEAPON_EARTH:
        return "地屬性傷害"; // 武器額外追加地系威力
    case APPLY_WEAPON_LIGHTNING:
        return "雷屬性傷害"; // 武器額外追加雷系威力
    case APPLY_WEAPON_SAINT:
        return "聖屬性傷害"; // 武器額外追加聖系威力
    case APPLY_WEAPON_DARK:
        return "暗屬性傷害"; // 武器額外追加暗系威力
    case APPLY_MAGIC_IMMUNE:
        return "魔法減免"; // 用於 mystic armor 2021/11/03
    case APPLY_TOXICITY_POISON:
        return "毒性強度"; // 用於 poison item 計算 2021/11/10
    case APPLY_DAMAGE_COUNT:
        return "累計傷害值"; // 用於 blood thirsty 計算 2022/02/21
    case APPLY_SHOOT_HITROLL:
        return "射擊命中率"; // 箭矢或槍彈等物品被射擊時的命中加成 2022/05/06
    case APPLY_SHOOT_DAMROLL:
        return "射擊傷害力"; // 箭矢或槍彈等物品被射擊時的傷害加成 2022/05/06
    case APPLY_SHOOT_ADAMAGE:
        return "射擊物理傷害力"; // 箭矢或槍彈等物品被射擊時的物理傷害加成 2022/05/06
    }

    bug("Affect_location_name: unknown location %d.", location);
    return "<不明>";
}

/*
 * Return ascii name of an affect bit vector.
 */
char *c_affect_bit_name(long long int vector) {
    static char buf[1024];

    buf[0] = '\0';
    if (vector & AFF_BLIND)
        strcat(buf, " 盲目");
    if (vector & AFF_INVISIBLE)
        strcat(buf, " 隱形");
    if (vector & AFF_DETECT_EVIL)
        strcat(buf, " 偵測邪惡");
    if (vector & AFF_DETECT_INVIS)
        strcat(buf, " 偵測隱形");
    if (vector & AFF_DETECT_MAGIC)
        strcat(buf, " 偵測魔法");
    if (vector & AFF_DETECT_HIDDEN)
        strcat(buf, " 偵測藏匿");
    if (vector & AFF_HOLD)
        strcat(buf, " HOLD");
    if (vector & AFF_SANCTUARY)
        strcat(buf, " 聖光");
    if (vector & AFF_FAERIE_FIRE)
        strcat(buf, " 天使之火");
    if (vector & AFF_INFRARED)
        strcat(buf, " 夜視");
    if (vector & AFF_CURSE)
        strcat(buf, " 詛咒");
    if (vector & AFF_TARDY)
        strcat(buf, " 遲緩");
    if (vector & AFF_CHANGE_SEX)
        strcat(buf, " 性別變更");
    if (vector & AFF_POISON)
        strcat(buf, " 中毒");
    if (vector & AFF_PROTECT)
        strcat(buf, " 天使加護");
    if (vector & AFF_POLYMORPH)
        strcat(buf, " 種族變更");
    if (vector & AFF_SLEEP)
        strcat(buf, " 沉睡");
    if (vector & AFF_SNEAK)
        strcat(buf, " 潛行");
    if (vector & AFF_HIDE)
        strcat(buf, " 藏匿");
    if (vector & AFF_CHARM)
        strcat(buf, " 迷失");
    if (vector & AFF_FLYING)
        strcat(buf, " 飛行");
    if (vector & AFF_WATERWALK)
        strcat(buf, " 水面行走");
    if (vector & AFF_PASS_DOOR)
        strcat(buf, " 半透明");
    if (vector & AFF_MUTE)
        strcat(buf, " 沉默");
    if (vector & AFF_SEAL_MAGIC)
        strcat(buf, " 封咒");
    // if ( vector & AFF_GILLS )        strcat( buf, " gills" );
    if (vector & AFF_VAMP_BITE)
        strcat(buf, " 吸血鬼之吻");
    if (vector & AFF_GHOUL)
        strcat(buf, " 不死族詛咒"); // 2022/01/16
    if (vector & AFF_FLAMING)
        strcat(buf, " 炎壁");
    if (vector & AFF_PARALYZED)
        strcat(buf, " 麻痺");
    if (vector & AFF_CRAZY)
        strcat(buf, " 瘋狂");
    if (vector & AFF_WEAKEN)
        strcat(buf, " 衰弱");
    if (vector & AFF_NIGHTSTALK)
        strcat(buf, " 暗夜匿蹤");
    if (vector & AFF_DISEASE)
        strcat(buf, " 生病");
    if (vector & AFF_FEAR)
        strcat(buf, " 恐懼");
    return (buf[0] != '\0') ? buf + 1 : "無";
}

/*
 * Return ascii name of extra flags vector.
 */
char *c_extra_bit_name(int extra_flags) {
    static char buf[1024];

    buf[0] = '\0';
    if (extra_flags & ITEM_GLOW)
        strcat(buf, " 發光");
    if (extra_flags & ITEM_HUM)
        strcat(buf, " 低鳴");
    if (extra_flags & ITEM_DARK)
        strcat(buf, " 闇黑");
    if (extra_flags & ITEM_LOCK)
        strcat(buf, " 上鎖");
    if (extra_flags & ITEM_EVIL)
        strcat(buf, " 邪惡");
    if (extra_flags & ITEM_INVIS)
        strcat(buf, " 隱形");
    if (extra_flags & ITEM_MAGIC)
        strcat(buf, " 魔力");
    if (extra_flags & ITEM_NODROP)
        strcat(buf, " 無法丟棄");
    if (extra_flags & ITEM_BLESS)
        strcat(buf, " 祝福");
    if (extra_flags & ITEM_ANTI_GOOD)
        strcat(buf, " 反善良");
    if (extra_flags & ITEM_ANTI_EVIL)
        strcat(buf, " 反邪惡");
    if (extra_flags & ITEM_ANTI_NEUTRAL)
        strcat(buf, " 反中立");
    if (extra_flags & ITEM_NOREMOVE)
        strcat(buf, " 無法移除");
    if (extra_flags & ITEM_INVENTORY)
        strcat(buf, " 物品");
    if (extra_flags & ITEM_POISONED)
        strcat(buf, " 劇毒");
    if (extra_flags & ITEM_VAMPIRE_BANE)
        strcat(buf, " BANE");
    if (extra_flags & ITEM_HOLY)
        strcat(buf, " 神聖");
    if (extra_flags & ITEM_NOSAVE)
        strcat(buf, " 無法儲存");
    if (extra_flags & ITEM_UNIQUE)
        strcat(buf, " 獨一無二");
    if (extra_flags & ITEM_LORE)
        strcat(buf, " 單一");
    if (extra_flags & ITEM_NOSAC)
        strcat(buf, " 無法奉獻");
    if (extra_flags & ITEM_PERSONAL)
        strcat(buf, " 個人專用");
    if (extra_flags & ITEM_RECOVER)
        strcat(buf, " 毀損修復");
    if (extra_flags & ITEM_NO_MALE)
        strcat(buf, " 男性禁用");
    if (extra_flags & ITEM_NO_FEMALE)
        strcat(buf, " 女性禁用");
    if (extra_flags & ITEM_NO_NEUTRAL)
        strcat(buf, " 中性禁用");
    if (extra_flags & ITEM_SPECIAL)
        strcat(buf, " 職業限制");
    if (extra_flags & ITEM_SPELL)
        strcat(buf, " 附魔"); // ITEM_SPELL add at 2021/09/19
                              //	if ( extra_flags & ITEM_MALE) strcat( buf, "男性專用"		);
                              //	if ( extra_flags & ITEM_FEMALE) strcat( buf, "女性專用"		);
                              //	if ( extra_flags & ITEM_NEUTRAL) strcat( buf, "中性專用"	);
    return (buf[0] != '\0') ? buf + 1 : "無";
}

char *const c_title_table[MAX_CLASS][MAX_LEVEL + 1][2] =
    {
        {{"Man", "Woman"},

         {"魔法學徒", "魔法學徒"},
         {"魔法學徒", "魔法學徒"},
         {"魔法學習者", "魔法學習者"},
         {"魔法研究者", "魔法研究者"},
         {"魔法靈媒", "魔法靈媒"},

         {"魔法抄寫員", "魔法抄寫員"},
         {"先知", "女先知"},
         {"智者", "智者"},
         {"幻術士", "幻術士"},
         {"咒術士", "咒術士"},

         {"降術士", "降術士"},
         {"魔導士", "魔導士"},
         {"召喚士", "召喚士"},
         {"魔術師", "魔術師"},
         {"創造者", "創造者"},

         {"賢者", "賢者"},
         {"法師", "女法師"},
         {"巫師", "女巫"},
         {"大法師", "大法師"},
         {"魔法師", "魔法師"},

         {"魔法長老", "魔法長老"},
         {"高階魔法長老", "高階魔法長老"},
         {"大魔法師", "大魔法師"},
         {"巨像製造者", "巨像製造者"},
         {"巨像製造大師", "巨像製造大師"},

         {
             "魔石製造者",
             "魔石製造者",
         },
         {
             "魔藥製造者",
             "魔藥製造者",
         },
         {
             "卷軸製造者",
             "卷軸製造者",
         },
         {
             "法杖製造者",
             "法杖製造者",
         },
         {
             "權杖製造者",
             "權杖製造者",
         },

         {"惡魔召喚士", "惡魔召喚士"},
         {"惡魔召喚師", "惡魔召喚師"},
         {"魅龍術士", "魅龍術士"},
         {"魅龍術師", "魅龍術師"},
         {"魔法大師", "魔法大師"},

         {"魔法大師", "魔法大師"},
         {"魔法大師", "魔法大師"},
         {"魔法大師", "魔法大師"},
         {"法師英雄", "女法師英雄"},
         {"法師英雄", "女法師英雄"},

         {"法師英雄", "女法師英雄"},
         {"法師英雄", "女法師英雄"},
         {"魔法天使", "魔法天使"},
         {"魔法之神", "魔法之神"},

         {"魔法大神", "魔法大神"},
         {"造物主", "創世女神"},
         {"造物主", "創世女神"}},

        {{"Man", "Woman"},

         {"信徒", "女信徒"},
         {"侍從", "侍從"},
         {"侍僧", "侍僧"},
         {"新進教徒", "新進教徒"},
         {"傳教士", "傳教士"},

         {"教士長", "教士長"},
         {"執事", "女執事"},
         {"司教", "司教"},
         {"祭司", "女祭司"},
         {"牧師", "牧師"},

         {"參事", "參事"},
         {"參事長", "參事長"},
         {"輔祭", "輔祭"},
         {"修士", "修女"},
         {"醫者", "女醫者"},

         {"高階牧師", "高階牧師"},
         {"闡道者", "闡道者"},
         {"主教", "主教"},
         {"大主教", "大主教"},
         {"總主教", "總主教"},

         {"主教長老", "主教長老"},
         {"大主教長老", "大主教長老"},
         {"總主教長老", "總主教長老"},
         {"除魔者", "除魔者"},
         {"除魔聖者", "除魔聖者"},

         {"海階樞機主教", "海階樞機主教"},
         {"大地階樞機主教", "大地階樞機主教"},
         {"風階樞機主教", "風階樞機主教"},
         {"乙太階樞機主教", "乙太階樞機主教"},
         {"天位樞機主教", "天位樞機主教"},

         {"不朽聖者", "不朽聖者"},
         {"神聖化身", "神聖化身"},
         {"神聖化身", "神聖化身"},
         {"天神降世", "天神降世"},
         {"神聖魔法大師", "神聖魔法大師"},

         {"神聖魔法大師", "神聖魔法大師"},
         {"神聖魔法大師", "神聖魔法大師"},
         {"神聖魔法大師", "神聖魔法大師"},
         {"牧師英雄", "牧師女英雄"},
         {"牧師英雄", "牧師女英雄"},

         {"牧師英雄", "牧師女英雄"},
         {"牧師英雄", "牧師女英雄"},
         {"Angel", "Angel"},
         {"Deity", "Deity"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"毛賊", "毛賊"},
         {"攔路盜", "攔路盜"},
         {"小偷", "女小偷"},
         {"扒手", "扒手"},
         {"盜賊", "盜賊"},

         {"竊賊", "竊賊"},
         {"高級扒手", "高級扒手"},
         {"強盜", "強盜"},
         {"騙子", "女騙子"},
         {"流氓", "流氓"},
         {"惡棍", "惡棍"},

         {"盜匪", "盜匪"},
         {"馬賊", "馬賊"},
         {"夜行盜", "夜行盜"},
         {"妙賊", "妙賊"},

         {"Knifer", "Knifer"},
         {"Quick-Blade", "Quick-Blade"},
         {"兇手", "兇手"},
         {"山賊", "山賊"},
         {"大盜", "大盜"},

         {"間諜", "間諜"},
         {"老手間諜", "老手間諜"},
         {"間諜大師", "間諜大師"},
         {"刺客", "刺客"},
         {"高級刺客", "高級刺客"},

         {"窺視專家", "窺視專家"},
         {"竊聽專家", "竊聽專家"},
         {"洞察專家", "洞察專家"},
         {"Master of Taste", "Mistress of Taste"},
         {"Master of Touch", "Mistress of Touch"},

         {"犯罪高手", "犯罪高手"},
         {"犯罪專家", "犯罪專家"},
         {"犯罪大師", "犯罪大師"},
         {"犯罪之王", "犯罪之后"},
         {"黑街教父", "黑街教母"},
         {"黑街教父", "黑街教母"},
         {"黑街教父", "黑街教母"},
         {"黑街教父", "黑街教母"},

         {"Thief Hero", "Thief Heroine"},
         {"Thief Hero", "Thief Heroine"},
         {"Thief Hero", "Thief Heroine"},
         {"Thief Hero", "Thief Heroine"},
         {"Angel of Death", "Angel of Death"},
         {"Deity of Assassins", "Deity of Assassins"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"劍術見習生", "劍術見習生"},
         {"新兵", "新兵"},
         {"步卒", "步卒"},
         {"戰士", "女戰士"},
         {"士兵", "女兵"},

         {"武士", "女武士"},
         {"老兵", "老兵"},
         {"劍手", "劍手"},
         {"擊劍者", "擊劍者"},
         {"鬥士", "女鬥士"},

         {"英雄", "女英雄"},
         {"勇士", "女勇士"},
         {"狂劍士", "狂劍士"},
         {"傭兵", "女傭兵"},
         {"劍師", "女劍師"},

         {"副首領", "副首領"},
         {"首領", "女首領"},
         {"龍騎兵", "龍騎兵"},
         {"龍騎將", "龍騎將"},
         {"正騎士", "女騎士"},

         {"上位騎士", "上位騎士"},
         {"騎士長", "騎士長"},
         {"聖殿騎士", "聖殿騎士"},
         {"聖殿禁衛", "聖殿禁衛"},
         {"除魔者", "除魔者"},

         {"上位除魔者", "上位除魔者"},
         {"屠龍者", "屠龍者"},
         {"屠龍勇者", "屠龍勇者"},
         {"戰將", "戰將"},
         {"御封戰將", "御封戰將"},

         {"雷鳴男爵", "雷鳴女爵"},
         {"風暴子爵", "風暴女子爵"},
         {"狂風伯爵", "狂風女伯爵"},
         {"颶風侯爵", "颶風女侯爵"},
         {"流星公爵", "流星女公爵"},

         {"東荒沙王爵", "東荒沙女王爵"},
         {"西海濤王爵", "西海濤女王爵"},
         {"北峻嶺王爵", "北峻嶺女王爵"},
         {"南煙林王爵", "南煙林女王爵"},
         //{ "Knight of the Black Rose",	"Baroness of Meteors"		},

         //	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
         //{ "Knight of the White Rose",	"Baroness of Meteors"		},
         {"英雄戰士", "英雄女戰士"},
         {"英雄戰士", "英雄女戰士"},
         {"英雄戰士", "英雄女戰士"},
         /*{ "Knight of the Silver Moon",	"Baroness of Meteors"		},
         { "Knight of the Golden Sun",	"Baroness of Meteors"		},
         { "Squire of the Realm",	"Baroness of Meteors"		},
         
         { "Herald of the Realm",	"Baroness of Meteors"		},
         { "Knight of the Realm",	"Baroness of Meteors"		},
         { "Lord of the Realm",		"Baroness of Meteors"		},
         { "Highlord of the Realm",	"Baroness of Meteors"		},
         { "Knight Hero",		"Knight Heroine"		},
         */
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},
         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"劍術見習生", "劍術見習生"},
         {"新兵", "新兵"},
         {"步卒", "步卒"},
         {"戰士", "女戰士"},
         {"士兵", "女兵"},

         {"武士", "女武士"},
         {"老兵", "老兵"},
         {"劍手", "劍手"},
         {"擊劍者", "擊劍者"},
         {"鬥士", "女鬥士"},

         {"英雄", "女英雄"},
         {"勇士", "女勇士"},
         {"狂劍士", "狂劍士"},
         {"傭兵", "女傭兵"},
         {"劍師", "女劍師"},

         {"副首領", "副首領"},
         {"首領", "女首領"},
         {"龍騎兵", "龍騎兵"},
         {"龍騎將", "龍騎將"},
         {"正騎士", "女騎士"},

         {"上位騎士", "上位騎士"},
         {"騎士長", "騎士長"},
         {"聖殿騎士", "聖殿騎士"},
         {"聖殿禁衛", "聖殿禁衛"},
         {"除魔者", "除魔者"},

         {"上位除魔者", "上位除魔者"},
         {"屠龍者", "屠龍者"},
         {"屠龍勇者", "屠龍勇者"},
         {"戰將", "戰將"},
         {"御封戰將", "御封戰將"},

         {"雷鳴男爵", "雷鳴女爵"},
         {"風暴子爵", "風暴女子爵"},
         {"狂風伯爵", "狂風女伯爵"},
         {"颶風侯爵", "颶風女侯爵"},
         {"流星公爵", "流星女公爵"},

         {"東荒沙王爵", "東荒沙女王爵"},
         {"西海濤王爵", "西海濤女王爵"},
         {"北峻嶺王爵", "北峻嶺女王爵"},
         {"南煙林王爵", "南煙林女王爵"},
         //{ "Knight of the Black Rose",	"Baroness of Meteors"		},

         //	{ "Knight of the Red Rose",	"Baroness of Meteors"		},
         //{ "Knight of the White Rose",	"Baroness of Meteors"		},
         {"英雄蠻俠", "英雄女蠻俠"},
         {"英雄蠻俠", "英雄女蠻俠"},
         {"英雄蠻俠", "英雄女蠻俠"},
         /*{ "Knight of the Silver Moon",	"Baroness of Meteors"		},
         { "Knight of the Golden Sun",	"Baroness of Meteors"		},
         { "Squire of the Realm",	"Baroness of Meteors"		},
         
         { "Herald of the Realm",	"Baroness of Meteors"		},
         { "Knight of the Realm",	"Baroness of Meteors"		},
         { "Lord of the Realm",		"Baroness of Meteors"		},
         { "Highlord of the Realm",	"Baroness of Meteors"		},
         { "Knight Hero",		"Knight Heroine"		},
         */
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},
         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {
             "Psychic",
             "Psychic",
         },
         {
             "Medium",
             "Medium",
         },
         {
             "Gypsy",
             "Gypsy",
         },
         {
             "Meditator",
             "Meditator",
         },
         {
             "Mind Prober",
             "Mind Prober",
         },

         {
             "Soul Searcher",
             "Soul Searcher",
         },
         {
             "Astral Voyager",
             "Astral Voyager",
         },
         {
             "Seeker",
             "Seeker",
         },
         {
             "Empath",
             "Empath",
         },
         {"Mind Reader", "Mind Reader"},

         {
             "Telepath",
             "Telepath",
         },
         {
             "Mental Adept",
             "Mental Adept",
         },
         {
             "Spoonbender",
             "Spoonbender",
         },
         {
             "Perceptive",
             "Perceptive",
         },
         {
             "Clever",
             "Clever",
         },

         {
             "Wise",
             "Wise",
         },
         {
             "Genius",
             "Genius",
         },
         {
             "Oracle",
             "Oracle",
         },
         {
             "Soothsayer",
             "Soothsayer",
         },
         {
             "Truthteller",
             "Truthteller",
         },

         {
             "Sage",
             "Sage",
         },
         {
             "Master Psychic",
             "Mistress Psychic",
         },
         {
             "Master Meditator",
             "Mistress Meditator",
         },
         {
             "Master Empath",
             "Mistress Empath",
         },
         {
             "Master Clairvoyant",
             "Mistress Clairvoyant",
         },

         {
             "Master Mind Reader",
             "Mistress Mind Reader",
         },
         {
             "Master Telepath",
             "Mistress Telepath",
         },
         {
             "Master Spoonbender",
             "Mistress Spoonbender",
         },
         {
             "Grand Master Psychic",
             "Grand Mistress Psychic",
         },
         {
             "Grand Master Meditator",
             "Grand Mistress Meditator",
         },

         {
             "Grand Master Empath",
             "Grand Mistress Empath",
         },
         {
             "Grand Master Clairvoyant",
             "Grand Mistress Clairvoyant",
         },
         {
             "Grand Master Mind Reader",
             "Grand Mistress Mind Reader",
         },
         {
             "Grand Master Telepath",
             "Grand Mistress Telepath",
         },
         {
             "Grand Master Spoonbender",
             "Grand Mistress Spoonbender",
         },

         {
             "大法師",
             "女大法師",
         },
         {
             "天才大法師",
             "梅黛拉繼者",
         },
         {
             "巫師",
             "女巫師",
         },
         {
             "大巫師",
             "女大巫師",
         },
         {
             "戰巫",
             "女戰巫",
         },
         {
             "帝國戰巫",
             "梅法拉繼者",
         },
         {
             "萬物引導者",
             "萬物引導者",
         },

         {"Psionicist Angel", "Psionicist Angel"},
         {"Deity of Psionics", "Deity of Psionics"},

         {"Supreme Psionicst", "Supreme Psionicist"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {
             "Psychic",
             "Psychic",
         },
         {
             "Medium",
             "Medium",
         },
         {
             "Gypsy",
             "Gypsy",
         },
         {
             "Meditator",
             "Meditator",
         },
         {
             "Mind Prober",
             "Mind Prober",
         },

         {
             "Soul Searcher",
             "Soul Searcher",
         },
         {
             "Astral Voyager",
             "Astral Voyager",
         },
         {
             "Seeker",
             "Seeker",
         },
         {
             "Empath",
             "Empath",
         },
         {"Mind Reader", "Mind Reader"},

         {
             "Telepath",
             "Telepath",
         },
         {
             "Mental Adept",
             "Mental Adept",
         },
         {
             "Spoonbender",
             "Spoonbender",
         },
         {
             "Perceptive",
             "Perceptive",
         },
         {
             "Clever",
             "Clever",
         },

         {
             "Wise",
             "Wise",
         },
         {
             "Genius",
             "Genius",
         },
         {
             "Oracle",
             "Oracle",
         },
         {
             "Soothsayer",
             "Soothsayer",
         },
         {
             "Truthteller",
             "Truthteller",
         },

         {
             "Sage",
             "Sage",
         },
         {
             "Master Psychic",
             "Mistress Psychic",
         },
         {
             "Master Meditator",
             "Mistress Meditator",
         },
         {
             "Master Empath",
             "Mistress Empath",
         },
         {
             "Master Clairvoyant",
             "Mistress Clairvoyant",
         },

         {
             "Master Mind Reader",
             "Mistress Mind Reader",
         },
         {
             "Master Telepath",
             "Mistress Telepath",
         },
         {
             "Master Spoonbender",
             "Mistress Spoonbender",
         },
         {
             "Grand Master Psychic",
             "Grand Mistress Psychic",
         },
         {
             "Grand Master Meditator",
             "Grand Mistress Meditator",
         },

         {
             "Grand Master Empath",
             "Grand Mistress Empath",
         },
         {
             "Grand Master Clairvoyant",
             "Grand Mistress Clairvoyant",
         },
         {
             "Grand Master Mind Reader",
             "Grand Mistress Mind Reader",
         },
         {
             "Grand Master Telepath",
             "Grand Mistress Telepath",
         },
         {
             "Grand Master Spoonbender",
             "Grand Mistress Spoonbender",
         },

         {
             "Grand Master Spoonbender",
             "Grand Mistress Spoonbender",
         },
         {
             "Grand Master Spoonbender",
             "Grand Mistress Spoonbender",
         },
         {
             "Alchemist Hero",
             "Alchemist Herione",
         },
         {
             "Alchemist Hero",
             "Alchemist Herione",
         },
         {
             "Alchemist Hero",
             "Alchemist Herione",
         },
         {
             "Alchemist Hero",
             "Alchemist Herione",
         },
         {
             "創造者",
             "創造者",
         },
         {"Psionicist Angel", "Psionicist Angel"},
         {"Deity of Psionics", "Deity of Psionics"},

         {"Supreme Psionicst", "Supreme Psionicist"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"男人", "女人"},

         {"愚民", "愚民"},
         {"侍從", "侍從"},
         {"沙彌", "女沙彌"},
         {"新兵", "女兵"},
         {"傳教士", "女傳教士"},

         {"高手", "女高手"},
         {"副司祭", "女副司祭"},
         {"代理牧師", "代理牧師"},
         {"牧師", "女牧師"},
         {"住持", "女住持"},

         {"教會參事", "教會參事"},
         {"神殿副主祭", "神殿女副主祭"},
         {"助理神父", "助理神母"},
         {"黑衣修士", "紅衣修女"},
         {"醫者", "女醫者"},

         {"隨軍牧師", "隨軍女牧師"},
         {"解道者", "女解道者"},
         {"主教", "主教"},
         {"高階主教", "女高階主教"},
         {"大主教", "女大主教"},

         {"紅衣大主教", "紅衣女大主教"},
         {"教主", "女教主"},
         {"大教主", "女大教主"},
         {"惡魔殺手", "惡魔殺手"},
         {"大惡魔殺手", "大惡魔殺手"},

         {"海洋樞機主教", "海洋樞機主教"},
         {"大地樞機主教", "大地樞機主教"},
         {"大氣樞機主教", "大氣樞機主教"},
         {"以太樞機主教", "以太樞機主教"},
         {"天堂樞機主教", "天堂樞機主教"},

         {"不朽者", "不朽者"},
         {"純淨不朽者", "純淨不朽者"},
         {"聖者", "聖者"},
         {"造物聖者", "造物聖者"},
         {"純淨聖者", "純淨聖者"},

         {"光榮聖者", "光榮聖者"},
         {"光明聖者", "光明聖者"},
         {"大光明聖者", "大光明聖者"},
         {"星輝之賢者", "星輝之賢者"},
         {"月映之賢者", "月映之賢者"},
         {"日明之賢者", "日明之賢者"},
         {"救世聖主", "救世聖主"},
         {"天使", "天使"},
         {"Deity", "Deity"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"男人", "女人"},

         {"愚民", "愚民"},
         {"侍從", "侍從"},
         {"沙彌", "女沙彌"},
         {"新兵", "女兵"},
         {"傳教士", "女傳教士"},

         {"高手", "女高手"},
         {"副司祭", "女副司祭"},
         {"代理牧師", "代理牧師"},
         {"牧師", "女牧師"},
         {"住持", "女住持"},

         {"教會參事", "教會參事"},
         {"神殿副主祭", "神殿女副主祭"},
         {"助理神父", "助理神母"},
         {"黑衣修士", "紅衣修女"},
         {"醫者", "女醫者"},

         {"隨軍牧師", "隨軍女牧師"},
         {"解道者", "女解道者"},
         {"主教", "主教"},
         {"高階主教", "女高階主教"},
         {"大主教", "女大主教"},

         {"紅衣大主教", "紅衣女大主教"},
         {"教主", "女教主"},
         {"大教主", "女大教主"},
         {"惡魔殺手", "惡魔殺手"},
         {"大惡魔殺手", "大惡魔殺手"},

         {"海洋樞機主教", "海洋樞機主教"},
         {"大地樞機主教", "大地樞機主教"},
         {"大氣樞機主教", "大氣樞機主教"},
         {"以太樞機主教", "以太樞機主教"},
         {"天堂樞機主教", "天堂樞機主教"},

         {"不朽者", "不朽者"},
         {"純淨不朽者", "純淨不朽者"},
         {"聖者", "聖者"},
         {"造物聖者", "造物聖者"},
         {"純淨聖者", "純淨聖者"},

         {"光榮聖者", "光榮聖者"},
         {"光明聖者", "光明聖者"},
         {"大光明聖者", "大光明聖者"},
         {"太天位樞機主教", "太天位樞機主教"},
         {"教宗", "教宗"},
         {"副教皇", "副教皇"},
         {"教皇", "女教皇"},
         {"Angel", "Angel"},
         {"Deity", "Deity"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Pilferer", "Pilferess"},
         {"Footpad", "Footpad"},
         {"Filcher", "Filcheress"},
         {"Pick-Pocket", "Pick-Pocket"},
         {"Sneak", "Sneak"},

         {"Pincher", "Pincheress"},
         {"Cut-Purse", "Cut-Purse"},
         {"Snatcher", "Snatcheress"},
         {"Sharper", "Sharpress"},
         {"Rogue", "Rogue"},

         {"Robber", "Robber"},
         {"Magsman", "Magswoman"},
         {"Highwayman", "Highwaywoman"},
         {"Burglar", "Burglaress"},
         {"Thief", "Thief"},

         {"Knifer", "Knifer"},
         {"Quick-Blade", "Quick-Blade"},
         {"Killer", "Murderess"},
         {"Brigand", "Brigand"},
         {"Cut-Throat", "Cut-Throat"},

         {"Spy", "Spy"},
         {"Grand Spy", "Grand Spy"},
         {"Master Spy", "Master Spy"},
         {"Assassin", "Assassin"},
         {"Greater Assassin", "Greater Assassin"},

         {"Master of Vision", "Mistress of Vision"},
         {"Master of Hearing", "Mistress of Hearing"},
         {"Master of Smell", "Mistress of Smell"},
         {"Master of Taste", "Mistress of Taste"},
         {"Master of Touch", "Mistress of Touch"},

         {"Crime Lord", "Crime Mistress"},
         {"Infamous Crime Lord", "Infamous Crime Mistress"},
         {"Greater Crime Lord", "Greater Crime Mistress"},
         {"Master Crime Lord", "Master Crime Mistress"},
         {"Godfather", "Godmother"},

         {"Godfather", "Godmother"},
         {"Godfather", "Godmother"},
         {"Master Assassin", "Master Assassin "},
         {"Master Assassin", "Master Assassin "},
         {"Assassin Hero", "Assassin Heroine"},
         {"Assassin Hero", "Assassin Heroine"},
         {"幻世殺神", "幻世殺神"},
         {"Angel of Death", "Angel of Death"},
         {"Deity of Assassins", "Deity of Assassins"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Pilferer", "Pilferess"},
         {"Footpad", "Footpad"},
         {"Filcher", "Filcheress"},
         {"Pick-Pocket", "Pick-Pocket"},
         {"Sneak", "Sneak"},

         {"Pincher", "Pincheress"},
         {"Cut-Purse", "Cut-Purse"},
         {"Snatcher", "Snatcheress"},
         {"Sharper", "Sharpress"},
         {"Rogue", "Rogue"},

         {"Robber", "Robber"},
         {"Magsman", "Magswoman"},
         {"Highwayman", "Highwaywoman"},
         {"Burglar", "Burglaress"},
         {"Thief", "Thief"},

         {"Knifer", "Knifer"},
         {"Quick-Blade", "Quick-Blade"},
         {"Killer", "Murderess"},
         {"Brigand", "Brigand"},
         {"Cut-Throat", "Cut-Throat"},

         {"Spy", "Spy"},
         {"Grand Spy", "Grand Spy"},
         {"Master Spy", "Master Spy"},
         {"Assassin", "Assassin"},
         {"Greater Assassin", "Greater Assassin"},

         {"Master of Vision", "Mistress of Vision"},
         {"Master of Hearing", "Mistress of Hearing"},
         {"Master of Smell", "Mistress of Smell"},
         {"Master of Taste", "Mistress of Taste"},
         {"Master of Touch", "Mistress of Touch"},

         {"Crime Lord", "Crime Mistress"},
         {"Infamous Crime Lord", "Infamous Crime Mistress"},
         {"Greater Crime Lord", "Greater Crime Mistress"},
         {"Master Crime Lord", "Master Crime Mistress"},
         {"Godfather", "Godmother"},

         {"Godfather", "Godmother"},
         {"Godfather", "Godmother"},
         {"Ninja Master", "Ninja Mistress"},
         {"Ninja Master", "Ninja Mistress"},
         {"Ninja Master", "Ninja Mistress"},
         {"Ninja Master", "Ninja Mistress"},
         {"Lord of Shadow", "Lady of Shadow"},
         {"Angel of Death", "Angel of Death"},
         {"Deity of Assassins", "Deity of Assassins"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Swordpupil", "Swordpupil"},
         {"Recruit", "Recruit"},
         {"Sentry", "Sentress"},
         {"Fighter", "Fighter"},
         {"Soldier", "Soldier"},

         {"Warrior", "Warrior"},
         {"Veteran", "Veteran"},
         {"Swordsman", "Swordswoman"},
         {"Fencer", "Fenceress"},
         {"Combatant", "Combatess"},

         {"Hero", "Heroine"},
         {"Myrmidon", "Myrmidon"},
         {"Swashbuckler", "Swashbuckleress"},
         {"Mercenary", "Mercenaress"},
         {"Swordmaster", "Swordmistress"},

         {"Lieutenant", "Lieutenant"},
         {"Champion", "Lady Champion"},
         {"Dragoon", "Lady Dragoon"},
         {"Cavalier", "Lady Cavalier"},
         {"Knight", "Lady Knight"},

         {"Grand Knight", "Grand Knight"},
         {"Master Knight", "Master Knight"},
         {"Paladin", "Paladin"},
         {"Grand Paladin", "Grand Paladin"},
         {"Demon Slayer", "Demon Slayer"},

         {"Greater Demon Slayer", "Greater Demon Slayer"},
         {"Dragon Slayer", "Dragon Slayer"},
         {"Greater Dragon Slayer", "Greater Dragon Slayer"},
         {"Underlord", "Underlord"},
         {"Overlord", "Overlord"},

         {"Baron of Thunder", "Baroness of Thunder"},
         {"Baron of Storms", "Baroness of Storms"},
         {"Baron of Tornadoes", "Baroness of Tornadoes"},
         {"Baron of Hurricanes", "Baroness of Hurricanes"},
         {"Baron of Meteors", "Baroness of Meteors"},

         {"魔鎧騎士", "飛雪騎士"},
         {"神盾騎士", "若蘭騎士"},
         {"聖劍騎士", "玫瑰騎士"},
         {"皇冠騎士", "薔薇騎士"},
         {"混沌騎士", "混沌騎士"},
         {"天位混沌騎士", "天位混沌騎士"},
         {"太天位混沌騎士", "太天位混沌騎士"},
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},

         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Swordpupil", "Swordpupil"},
         {"Recruit", "Recruit"},
         {"Sentry", "Sentress"},
         {"Fighter", "Fighter"},
         {"Soldier", "Soldier"},

         {"Warrior", "Warrior"},
         {"Veteran", "Veteran"},
         {"Swordsman", "Swordswoman"},
         {"Fencer", "Fenceress"},
         {"Combatant", "Combatess"},

         {"Hero", "Heroine"},
         {"Myrmidon", "Myrmidon"},
         {"Swashbuckler", "Swashbuckleress"},
         {"Mercenary", "Mercenaress"},
         {"Swordmaster", "Swordmistress"},

         {"Lieutenant", "Lieutenant"},
         {"Champion", "Lady Champion"},
         {"Dragoon", "Lady Dragoon"},
         {"Cavalier", "Lady Cavalier"},
         {"Knight", "Lady Knight"},

         {"Grand Knight", "Grand Knight"},
         {"Master Knight", "Master Knight"},
         {"Paladin", "Paladin"},
         {"Grand Paladin", "Grand Paladin"},
         {"Demon Slayer", "Demon Slayer"},

         {"Greater Demon Slayer", "Greater Demon Slayer"},
         {"Dragon Slayer", "Dragon Slayer"},
         {"Greater Dragon Slayer", "Greater Dragon Slayer"},
         {"Underlord", "Underlord"},
         {"Overlord", "Overlord"},

         {"Baron of Thunder", "Baroness of Thunder"},
         {"Baron of Storms", "Baroness of Storms"},
         {"Baron of Tornadoes", "Baroness of Tornadoes"},
         {"Baron of Hurricanes", "Baroness of Hurricanes"},
         {"Baron of Meteors", "Baroness of Meteors"},

         {"大地劍士", "大地劍士"},
         {"熾炎劍士", "熾炎劍士"},
         {"霸皇劍士", "霸皇劍士"},
         {"傲風劍士", "傲風劍士"},
         {"劍聖", "女劍聖"},
         {"天位劍聖", "天位女劍聖"},
         {"太天位劍聖", "太天位女劍聖"},
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},

         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"沙彌", "沙彌"},
         {"Recruit", "Recruit"},
         {"Sentry", "Sentress"},
         {"Fighter", "Fighter"},
         {"Soldier", "Soldier"},

         {"Warrior", "Warrior"},
         {"Veteran", "Veteran"},
         {"Swordsman", "Swordswoman"},
         {"Fencer", "Fenceress"},
         {"Combatant", "Combatess"},

         {"Hero", "Heroine"},
         {"Myrmidon", "Myrmidon"},
         {"Swashbuckler", "Swashbuckleress"},
         {"Mercenary", "Mercenaress"},
         {"Swordmaster", "Swordmistress"},

         {"Lieutenant", "Lieutenant"},
         {"Champion", "Lady Champion"},
         {"Dragoon", "Lady Dragoon"},
         {"Cavalier", "Lady Cavalier"},
         {"Knight", "Lady Knight"},

         {"Grand Knight", "Grand Knight"},
         {"Master Knight", "Master Knight"},
         {"Paladin", "Paladin"},
         {"Grand Paladin", "Grand Paladin"},
         {"Demon Slayer", "Demon Slayer"},

         {"Greater Demon Slayer", "Greater Demon Slayer"},
         {"Dragon Slayer", "Dragon Slayer"},
         {"Greater Dragon Slayer", "Greater Dragon Slayer"},
         {"Underlord", "Underlord"},
         {"Overlord", "Overlord"},

         {"Baron of Thunder", "Baroness of Thunder"},
         {"Baron of Storms", "Baroness of Storms"},
         {"Baron of Tornadoes", "Baroness of Tornadoes"},
         {"Baron of Hurricanes", "Baroness of Hurricanes"},
         {"Baron of Meteors", "Baroness of Meteors"},

         {"藏經閣護法", "藏經閣護法"},
         {"達摩堂首座", "達摩堂首座"},
         {"戒律院首座", "戒律院首座"},
         {"羅漢堂首座", "羅漢堂首座"},
         {"伏虎羅漢", "伏虎羅漢"},
         {"降龍羅漢", "降龍羅漢"},
         {"大阿羅漢", "大阿羅漢"},
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},

         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Swordpupil", "Swordpupil"},
         {"Recruit", "Recruit"},
         {"Sentry", "Sentress"},
         {"Fighter", "Fighter"},
         {"Soldier", "Soldier"},

         {"Warrior", "Warrior"},
         {"Veteran", "Veteran"},
         {"Swordsman", "Swordswoman"},
         {"Fencer", "Fenceress"},
         {"Combatant", "Combatess"},

         {"Hero", "Heroine"},
         {"Myrmidon", "Myrmidon"},
         {"Swashbuckler", "Swashbuckleress"},
         {"Mercenary", "Mercenaress"},
         {"Swordmaster", "Swordmistress"},

         {"Lieutenant", "Lieutenant"},
         {"Champion", "Lady Champion"},
         {"Dragoon", "Lady Dragoon"},
         {"Cavalier", "Lady Cavalier"},
         {"Knight", "Lady Knight"},

         {"Grand Knight", "Grand Knight"},
         {"Master Knight", "Master Knight"},
         {"Paladin", "Paladin"},
         {"Grand Paladin", "Grand Paladin"},
         {"Demon Slayer", "Demon Slayer"},

         {"Greater Demon Slayer", "Greater Demon Slayer"},
         {"Dragon Slayer", "Dragon Slayer"},
         {"Greater Dragon Slayer", "Greater Dragon Slayer"},
         {"Underlord", "Underlord"},
         {"Overlord", "Overlord"},

         {"Baron of Thunder", "Baroness of Thunder"},
         {"Baron of Storms", "Baroness of Storms"},
         {"Baron of Tornadoes", "Baroness of Tornadoes"},
         {"颶風伯爵", "颶風女爵"},
         {"彗星伯爵", "彗星女爵"},

         {"東荒原伯爵", "東荒原女爵"},
         {"西冰山伯爵", "西冰山女爵"},
         {"北大漠伯爵", "北大漠女爵"},
         {"南大洋伯爵", "南大洋女爵"},
         {"Shaman Hero", "Shaman Heroine"},
         {"Shaman Hero", "Shaman Heroine"},
         {"Shaman Hero", "Shaman Heroine"},
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},

         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Apprentice of Magic", "Apprentice of Magic"},
         {"Spell Student", "Spell Student"},
         {"Scholar of Magic", "Scholar of Magic"},
         {"Delver in Spells", "Delveress in Spells"},
         {"Medium of Magic", "Medium of Magic"},

         {"Scribe of Magic", "Scribess of Magic"},
         {"Seer", "Seeress"},
         {"Sage", "Sage"},
         {"Illusionist", "Illusionist"},
         {"Abjurer", "Abjuress"},

         {"Invoker", "Invoker"},
         {"Enchanter", "Enchantress"},
         {"Conjurer", "Conjuress"},
         {"Magician", "Witch"},
         {"Creator", "Creator"},

         {"Savant", "Savant"},
         {"Magus", "Craftess"},
         {"Wizard", "Wizard"},
         {"Warlock", "War Witch"},
         {"Sorcerer", "Sorceress"},

         {"Elder Sorcerer", "Elder Sorceress"},
         {"Grand Sorcerer", "Grand Sorceress"},
         {"Great Sorcerer", "Great Sorceress"},
         {"Golem Maker", "Golem Maker"},
         {"Greater Golem Maker", "Greater Golem Maker"},

         {
             "Maker of Stones",
             "Maker of Stones",
         },
         {
             "Maker of Potions",
             "Maker of Potions",
         },
         {
             "Maker of Scrolls",
             "Maker of Scrolls",
         },
         {
             "Maker of Wands",
             "Maker of Wands",
         },
         {
             "Maker of Staves",
             "Maker of Staves",
         },

         {"Demon Summoner", "Demon Summoner"},
         {"Greater Demon Summoner", "Greater Demon Summoner"},
         {"Dragon Charmer", "Dragon Charmer"},
         {"Greater Dragon Charmer", "Greater Dragon Charmer"},
         {"Master of all Magic", "Master of all Magic"},

         {"Master of all Magic", "Master of all Magic"},
         {"Master of all Magic", "Master of all Magic"},
         {"Summoner Hero", "Summoner Heroine"},
         {"Summoner Hero", "Summoner Heroine"},
         {"Summoner Hero", "Summoner Heroine"},
         {"Summoner Hero", "Summoner Heroine"},
         {"Summoner Hero", "Summoner Heroine"},
         {"Angel of Magic", "Angel of Magic"},
         {"Deity of Magic", "Deity of Magic"},

         {"Supremity of Magic", "Supremity of Magic"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Believer", "Believer"},
         {"Attendant", "Attendant"},
         {"Acolyte", "Acolyte"},
         {"Novice", "Novice"},
         {"Missionary", "Missionary"},

         {"Adept", "Adept"},
         {"Deacon", "Deaconess"},
         {"Vicar", "Vicaress"},
         {"Priest", "Priestess"},
         {"Minister", "Lady Minister"},

         {"Canon", "Canon"},
         {"Levite", "Levitess"},
         {"Curate", "Curess"},
         {"Monk", "Nun"},
         {"Healer", "Healess"},

         {"Chaplain", "Chaplain"},
         {"Expositor", "Expositress"},
         {"Bishop", "Bishop"},
         {"Arch Bishop", "Arch Lady of the Church"},
         {"Patriarch", "Matriarch"},

         {"Elder Patriarch", "Elder Matriarch"},
         {"Grand Patriarch", "Grand Matriarch"},
         {"Great Patriarch", "Great Matriarch"},
         {"Demon Killer", "Demon Killer"},
         {"Greater Demon Killer", "Greater Demon Killer"},

         {"Cardinal of the Sea", "Cardinal of the Sea"},
         {"Cardinal of the Earth", "Cardinal of the Earth"},
         {"Cardinal of the Air", "Cardinal of the Air"},
         {"Cardinal of the Ether", "Cardinal of the Ether"},
         {"Cardinal of the Heavens", "Cardinal of the Heavens"},

         {"Avatar of an Immortal", "Avatar of an Immortal"},
         {"Avatar of a Deity", "Avatar of a Deity"},
         {"Avatar of a Supremity", "Avatar of a Supremity"},
         {"Avatar of an Implementor", "Avatar of an Implementor"},
         {"Master of all Divinity", "Mistress of all Divinity"},

         {"Master of all Divinity", "Mistress of all Divinity"},
         {"Master of all Divinity", "Mistress of all Divinity"},
         {"Champion of the Realm", "Champion of the Realm"},
         {"Lord of the Realm", "Lady of the Realm"},
         {"King of the Realm", "Queen of the Realm"},
         {"Guadian of the Realm", "Guadian of the Realm"},
         {"Angel", "Angel"},
         {"Deity", "Deity"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Pilferer", "Pilferess"},
         {"Footpad", "Footpad"},
         {"Filcher", "Filcheress"},
         {"Pick-Pocket", "Pick-Pocket"},
         {"Sneak", "Sneak"},

         {"Pincher", "Pincheress"},
         {"Cut-Purse", "Cut-Purse"},
         {"Snatcher", "Snatcheress"},
         {"Sharper", "Sharpress"},
         {"Rogue", "Rogue"},

         {"Robber", "Robber"},
         {"Magsman", "Magswoman"},
         {"Highwayman", "Highwaywoman"},
         {"Burglar", "Burglaress"},
         {"Thief", "Thief"},

         {"Knifer", "Knifer"},
         {"Quick-Blade", "Quick-Blade"},
         {"Killer", "Murderess"},
         {"Brigand", "Brigand"},
         {"Cut-Throat", "Cut-Throat"},

         {"Spy", "Spy"},
         {"Grand Spy", "Grand Spy"},
         {"Master Spy", "Master Spy"},
         {"Assassin", "Assassin"},
         {"Greater Assassin", "Greater Assassin"},

         {"Master of Vision", "Mistress of Vision"},
         {"Master of Hearing", "Mistress of Hearing"},
         {"Master of Smell", "Mistress of Smell"},
         {"Master of Taste", "Mistress of Taste"},
         {"Master of Touch", "Mistress of Touch"},

         {"Crime Lord", "Crime Mistress"},
         {"Infamous Crime Lord", "Infamous Crime Mistress"},
         {"Greater Crime Lord", "Greater Crime Mistress"},
         {"Master Crime Lord", "Master Crime Mistress"},
         {"Godfather", "Godmother"},

         {"Godfather", "Godmother"},
         {"Godfather", "Godmother"},
         {"Ranger Hero", "Ranger Heroine"},
         {"Ranger Hero", "Ranger Heroine"},
         {"Ranger Hero", "Ranger Heroine"},
         {"Ranger Hero", "Ranger Heroine"},
         {"Ranger Hero", "Ranger Heroine"},
         {"Angel of Death", "Angel of Death"},
         {"Deity of Assassins", "Deity of Assassins"},

         {"Supreme Master", "Supreme Mistress"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Swordpupil", "Swordpupil"},
         {"Recruit", "Recruit"},
         {"Sentry", "Sentress"},
         {"Fighter", "Fighter"},
         {"Soldier", "Soldier"},

         {"Warrior", "Warrior"},
         {"Veteran", "Veteran"},
         {"Swordsman", "Swordswoman"},
         {"Fencer", "Fenceress"},
         {"Combatant", "Combatess"},

         {"Hero", "Heroine"},
         {"Myrmidon", "Myrmidon"},
         {"Swashbuckler", "Swashbuckleress"},
         {"Mercenary", "Mercenaress"},
         {"Swordmaster", "Swordmistress"},

         {"Lieutenant", "Lieutenant"},
         {"Champion", "Lady Champion"},
         {"Dragoon", "Lady Dragoon"},
         {"Cavalier", "Lady Cavalier"},
         {"Knight", "Lady Knight"},

         {"Grand Knight", "Grand Knight"},
         {"Master Knight", "Master Knight"},
         {"Paladin", "Paladin"},
         {"Grand Paladin", "Grand Paladin"},
         {"Demon Slayer", "Demon Slayer"},

         {"Greater Demon Slayer", "Greater Demon Slayer"},
         {"Dragon Slayer", "Dragon Slayer"},
         {"Greater Dragon Slayer", "Greater Dragon Slayer"},
         {"Underlord", "Underlord"},
         {"Overlord", "Overlord"},

         {"Baron of Thunder", "Baroness of Thunder"},
         {"Baron of Storms", "Baroness of Storms"},
         {"Baron of Tornadoes", "Baroness of Tornadoes"},
         {"颶風伯爵", "颶風女爵"},
         {"彗星伯爵", "彗星女爵"},

         {"東荒原伯爵", "東荒原女爵"},
         {"西冰山伯爵", "西冰山女爵"},
         {"北大漠伯爵", "北大漠女爵"},
         {"南大洋伯爵", "南大洋女爵"},
         {"戰神", "女戰神"},
         {"戰神", "女戰神"},
         {"戰神", "女戰神"},
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},

         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}},

        {{"Man", "Woman"},

         {"Swordpupil", "Swordpupil"},
         {"Recruit", "Recruit"},
         {"Sentry", "Sentress"},
         {"Fighter", "Fighter"},
         {"Soldier", "Soldier"},

         {"Warrior", "Warrior"},
         {"Veteran", "Veteran"},
         {"Swordsman", "Swordswoman"},
         {"Fencer", "Fenceress"},
         {"Combatant", "Combatess"},

         {"Hero", "Heroine"},
         {"Myrmidon", "Myrmidon"},
         {"Swashbuckler", "Swashbuckleress"},
         {"Mercenary", "Mercenaress"},
         {"Swordmaster", "Swordmistress"},

         {"Lieutenant", "Lieutenant"},
         {"Champion", "Lady Champion"},
         {"Dragoon", "Lady Dragoon"},
         {"Cavalier", "Lady Cavalier"},
         {"Knight", "Lady Knight"},

         {"Grand Knight", "Grand Knight"},
         {"Master Knight", "Master Knight"},
         {"Paladin", "Paladin"},
         {"Grand Paladin", "Grand Paladin"},
         {"Demon Slayer", "Demon Slayer"},

         {"Greater Demon Slayer", "Greater Demon Slayer"},
         {"Dragon Slayer", "Dragon Slayer"},
         {"Greater Dragon Slayer", "Greater Dragon Slayer"},
         {"Underlord", "Underlord"},
         {"Overlord", "Overlord"},

         {"Baron of Thunder", "Baroness of Thunder"},
         {"Baron of Storms", "Baroness of Storms"},
         {"Baron of Tornadoes", "Baroness of Tornadoes"},
         {"颶風伯爵", "颶風女爵"},
         {"彗星伯爵", "彗星女爵"},

         {"東荒原伯爵", "東荒原女爵"},
         {"西冰山伯爵", "西冰山女爵"},
         {"北大漠伯爵", "北大漠女爵"},
         {"南大洋伯爵", "南大洋女爵"},

         {"武神", "女武神"},
         {"武神", "女武神"},
         {"武神", "女武神"},
         {"Angel of War", "Angel of War"},
         {"Deity of War", "Deity of War"},

         {"Supreme Master of War", "Supreme Mistress of War"},
         {"造物主", "創世女神"},
         {"Implementor", "Implementress"}}
};
