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
//*****************************************************************
//*  PROGRAM ID  : act_wiz.c                                      *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940621 Razgriz                                *
//*  PURPOSE     :         修正 do_switch 加上判斷 victim 是否有  *
//*              :         PLR_HORSE 的 ACT bit，以避免在         *
//*              :         act_move.c 中會造成 crash 的 bug.      *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940704 Razgriz                                *
//*  PURPOSE     :         開放 disconnect 指令給 lv 大於 3 的玩家*
//*              :         使用，語法為：　　　　　　　　         *
//*              :         disconnect <id> <password>             *
//*              :         另外加上 qmark，限制密碼猜錯次數       *
//*              :         以避免有人故意利用此指令猜測玩家密碼   *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940831 Razgriz                                *
//*  PURPOSE     :         修改 disconnect 可以踢掉看不見的 char  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940916 Razgriz                                *
//*  PURPOSE     :         新增 do_forceo() 指令                  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940918 Razgriz                                *
//*  PURPOSE     :         修改do_oset(),新增obj_extra_value的檢查*
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940919 Razgriz                                *
//*  PURPOSE     :         修改do_users(),多了 -s 參數, 用來排序  *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950402 jye                                    *
//*  PURPOSE     :         修改do_mwheres(),多了-a參數查看所有ppl *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0950722 jye                                    *
//*  PURPOSE     :         新增EXP_PER_LEVEL ppl_class變數        *
//*****************************************************************
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#if defined(unix)
#include <unistd.h>
#endif
#endif
#include "merc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern bool IsSorGateMob(int nMobIndex);

char *flag_string(const struct flag_type *flag_table, long long int bits);
/* Conversion of Immortal powers to Immortal skills done by Thelonius */

// do_mtstat: Command: do_mtstat - Performs the 'mtstat' command.
// do_mtstat: 指令：do_mtstat - 執行 'mtstat' 指令。
void do_mtstat(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    OBJ_INDEX_DATA *mat;
    OBJ_HP *ohp;
    AFFECT_DATA *paf;
    OBJ_VRACE *ovr;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "mtstat"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("Mtstat what? \n\r", ch);
        return;
    }

    mat = material_lookup(arg);
    if (!mat) {
        send_to_char("No such material.\n\r", ch);
        return;
    }

    sprintf(buf, "Vnum: %d\n\rName: [%s]\n\rLevel: %d\n\r", mat->vnum,
            mat->name, mat->level);
    send_to_char(buf, ch);

    sprintf(buf, "Extra flags: %s\n\r",
            flag_string(extra_flags, mat->extra_flags));
    send_to_char(buf, ch);
    sprintf(buf, "Hitroll Mod: %d,  Damroll Mod: %d, Armor Class Mod: %d\n\r",
            mat->value[0], mat->value[1], mat->value[2]);
    send_to_char(buf, ch);
    sprintf(buf, "Durability: %d,  Weight Mod: %d%%,  Cost Mod: %d%%\n\r",
            mat->value[3], mat->weight, mat->cost);
    send_to_char(buf, ch);

    sprintf(buf, "Class: %d,  Race: %d\n\r", mat->class, mat->race);
    send_to_char(buf, ch);

    if ((paf = mat->affected)) {
        for (; paf; paf = paf->next) {
            sprintf(buf, "Affects %s by %d.\n\r",
                    affect_loc_name(paf->location), paf->modifier);
            send_to_char(buf, ch);
        }
    }

    if ((ohp = mat->hp)) {
        for (; ohp; ohp = ohp->next) {
            sprintf(buf, "HP: %s. percent: %d. level: %d. event: %d.\n\r",
                    ohp->hp_name, ohp->percent, ohp->level, ohp->event);
            send_to_char(buf, ch);
        }
    }

    if ((ovr = mat->vrace)) {
        sprintf(buf, "VRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
                race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p,
                ovr->dr_mod_q, ovr->dr_mod_p);
        send_to_char(buf, ch);
    }
}

// do_mtset: Command: do_mtset - Performs the 'mtset' command.
// do_mtset: 指令：do_mtset - 執行 'mtset' 指令。
void do_mtset(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    OBJ_INDEX_DATA *mat;
    char name[MAX_INPUT_LENGTH];
    char field[MAX_INPUT_LENGTH];
    char value[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "mtset"))
        return;

    argument = one_argument(argument, name);
    argument = one_argument(argument, field);
    argument = one_argument(argument, value);
    if (name[0] == '\0' || field[0] == '\0' || value[0] == '\0' ||
        !is_number(value)) {
        send_to_char("Mtset what? \n\r", ch);
        return;
    }

    mat = material_lookup(name);
    if (!mat) {
        send_to_char("No such material.\n\r", ch);
        return;
    }

    if (!str_cmp(field, "extra")) {
        mat->extra_flags = atoi(value);
    } else if (!str_cmp(field, "hr")) {
        mat->value[0] = atoi(value);
    } else if (!str_cmp(field, "dr")) {
        mat->value[1] = atoi(value);
    } else if (!str_cmp(field, "ac")) {
        mat->value[2] = atoi(value);
    } else if (!str_cmp(field, "dura")) {
        mat->value[3] = atoi(value);
    } else if (!str_cmp(field, "weight")) {
        mat->weight = atoi(value);
    } else if (!str_cmp(field, "cost")) {
        mat->cost = atoi(value);
    } else {
        send_to_char("mtstat <name> <field> <value>\n\r", ch);
        send_to_char("\n\rFields:\n\r extra hr dr ac dura weight cost\n\r", ch);
        return;
    }
    send_to_char("Ok.\n\r", ch);
}

// do_mtlist: Command: do_mtlist - Performs the 'mtlist' command.
// do_mtlist: 指令：do_mtlist - 執行 'mtlist' 指令。
void do_mtlist(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    OBJ_INDEX_DATA *mat;
    char buf[MAX_STRING_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "mtlist"))
        return;

    if (!material_list) {
        send_to_char("No materials.\n\r", ch);
        return;
    }

    send_to_char(" VNUM   材 質 名 稱\n\r", ch);

    for (mat = material_list; mat; mat = mat->next) {
        sprintf(buf, "[%5d] %s\n\r", mat->vnum, mat->short_descr);
        send_to_char(buf, ch);
    }
}

// do_unbackup: Command: do_unbackup - Performs the 'unbackup' command.
// do_unbackup: 指令：do_unbackup - 執行 'unbackup' 指令。
void do_unbackup(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int cmd;

    rch = get_char(ch);

    if (!authorized(rch, "unbackup"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("unbackup <id>\n\r", ch);
        return;
    }

    sprintf(buf, "mv %s%s/%s %s", PLAYER_DIR, initial(arg), capitalize(arg),
            "../player/restored/");
    cmd = system(buf);
    if (cmd) {
        sprintf(buf, "mv %s%s/%s.gz %s", PLAYER_DIR, initial(arg),
                capitalize(arg), "../player/restored/");
        cmd = system(buf);
        if (cmd) {
            send_to_char("No such player.\n\r", ch);
            return;
        }
    }
    sprintf(buf, "cp %s%s/%s %s%s/", BACKUP_DIR, initial(arg), capitalize(arg),
            PLAYER_DIR, initial(arg));
    cmd = system(buf);
    if (cmd) {
        sprintf(buf, "cp %s%s/%s.gz %s%s/", BACKUP_DIR, initial(arg),
                capitalize(arg), PLAYER_DIR, initial(arg));
        cmd = system(buf);

        if (cmd) {
            send_to_char("No backup for that player.\n\r", ch);
            return;
        }
    }
    send_to_char("Done.\n\r", ch);
    return;
}
// do_wizhelp: Command: do_wizhelp - Performs the 'wizhelp' command.
// do_wizhelp: 指令：do_wizhelp - 執行 'wizhelp' 指令。
void do_wizhelp(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    int cmd;
    int col;

    rch = get_char(ch);

    if (!authorized(rch, "wizhelp"))
        return;

    buf1[0] = '\0';
    col = 0;

    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++) {
        if (cmd_table[cmd].level < LEVEL_HERO ||
            str_infix(cmd_table[cmd].name, rch->pcdata->immskll))
            continue;

        sprintf(buf, "%-10s", cmd_table[cmd].name);
        strcat(buf1, buf);
        if (++col % 8 == 0)
            strcat(buf1, "\n\r");
    }

    if (col % 8 != 0)
        strcat(buf1, "\n\r");

    send_to_char(buf1, ch);
    return;
}

/*
// do_test: Command: do_test - Performs the 'test' command.
// do_test: 指令：do_test - 執行 'test' 指令。
void do_test( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    char       buf  [ MAX_STRING_LENGTH ];
    char       buf1 [ MAX_STRING_LENGTH * 2 ];
    int	       cmd;
    int	       col;
    extern     char * help_greeting [ MAX_GREETING ];
    extern     int num_greeting;
    int random_greeting = number_range(0, num_greeting - 1);

    rch = get_char( ch );
    send_to_char ("entering test", ch);

    if ( !authorized( rch, "test" ) )
        return;

    buf1[0] = '\0';
    sprintf(buf1, "Displaying Greeting [%d/%d]%s\n",
random_greeting,num_greeting, help_greeting[random_greeting]); col     = 0;

    send_to_char( buf1, ch );
    return;
}
*/

// do_bamfin: Command: do_bamfin - Performs the 'bamfin' command.
// do_bamfin: 指令：do_bamfin - 執行 'bamfin' 指令。
void do_bamfin(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "bamfin"))
        return;

    if (!IS_NPC(ch)) {
        if (longstring(ch, argument))
            return;

        smash_tilde(argument);
        free_string(ch->pcdata->bamfin);
        ch->pcdata->bamfin = str_dup(argument);
        send_to_char("Ok.\n\r", ch);
    }
    return;
}

// do_bamfout: Command: do_bamfout - Performs the 'bamfout' command.
// do_bamfout: 指令：do_bamfout - 執行 'bamfout' 指令。
void do_bamfout(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "bamfout"))
        return;

    if (!IS_NPC(ch)) {
        if (longstring(ch, argument))
            return;

        smash_tilde(argument);
        free_string(ch->pcdata->bamfout);
        ch->pcdata->bamfout = str_dup(argument);
        send_to_char("Ok.\n\r", ch);
    }
    return;
}

// do_deny: Command: do_deny - Performs the 'deny' command.
// do_deny: 指令：do_deny - 執行 'deny' 指令。
void do_deny(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    time_t t;

    rch = get_char(ch);

    if (!authorized(rch, "deny"))
        return;

    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg1);
    if (arg[0] == '\0' || arg[1] == '\0') {
        send_to_char("deny <角色> <時間>\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("不能 deny NPC.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("你失敗了. \n\r", ch);
        return;
    }

    if (!is_number(arg1)) {
        send_to_char("時間請用數字！\n\r", ch);
    }

    t = atoi(arg1) * 60;
    victim->pcdata->denied = time(NULL) + t;

    SET_BIT(victim->act, PLR_DENY);
    send_to_char("你被一股無形的力量所阻擋！\n\r", victim);
    send_to_char("OK.\n\r", ch);
    if (victim->level < 2)
        victim->level = 2;
    do_quit(victim, "");

    return;
}

/*
 * Modified by Razgriz	20050704
 * authorize PC to use disconnect with a password
 * and add a qmark to PC when he used wrong password
 * if PC use wrong password over 3 times continually
 * he will be forbidden to use disconnect for a period of mud time.
 */

// do_disconnect: Command: do_disconnect - Performs the 'disconnect' command.
// do_disconnect: 指令：do_disconnect - 執行 'disconnect' 指令。
void do_disconnect(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char arg[MAX_INPUT_LENGTH];
    bool fPC;            // Added by Razgriz	20050704
    QMARK *qmark = NULL; // Added by Razgriz	20050704

    rch = get_char(ch);

    /*  Mark by Razgriz	20050704
        if ( !authorized( rch, "disconnect" ) )
            return;
    */
    // Added by Razgriz	20050704
    if ((fPC = !authorized(rch, "disconnect")) == TRUE) {
        qmark = is_qmark(ch, "disconnect_fail");
        if (qmark && qmark->timer > 0) {
            send_to_char("你目前仍被禁止使用此指令！\n\r", ch);
            return;
        }
    }

    argument = one_argument(argument, arg); // Modified by Razgriz	20050704
    if (arg[0] == '\0') {
        send_to_char("Disconnect 誰?\n\r", ch);
        return;
    }

    // Added by Razgriz	20050704
    if (fPC && (argument[0] == '\0')) {
        send_to_char("你必須輸入你要 disconnect 的角色之密碼！\n\r", ch);
        return;
    }

    if (!(victim = get_char_world2(ch, arg))) // Modified by Razgriz	20050831
    {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (!victim->desc) {
        // Modified by Razgriz	20050704
        fPC ? act("$N 已經被 disconnect 囉.", ch, NULL, victim, TO_CHAR)
            : act("$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR);
        return;
    }

    // Added by Razgriz	20050704
    if (fPC && strcmp((char *)crypt(argument, victim->pcdata->pwd),
                      victim->pcdata->pwd)) {
        if (qmark) {
            if (qmark->v0 >= 3) {
                send_to_char("由於你連續輸入太多次錯誤密碼\n\r", ch);
                send_to_char("系統將禁止你使用此指令一段時間！！！\n\r", ch);
                do_mod_qmark(ch, "self disconnect_fail timer = 64");
                do_mod_qmark(ch, "self disconnect_fail v0    = 0");
                return;
            } else {
                send_to_char("你輸入的密碼是錯誤的喔！！\n\r", ch);
                do_mod_qmark(ch, "self disconnect_fail v0 + 1");
                return;
            }
        } else {
            send_to_char("你輸入的密碼是錯誤的喔！\n\r", ch);
            do_add_qmark(ch, "self disconnect_fail -1 1 0 0 0 1");
            return;
        }
    }

    // Added by Razgriz	20050704
    if (qmark != NULL) {
        do_del_qmark(ch, "self disconnect_fail");
        qmark = NULL;
    }

    for (d = descriptor_list; d; d = d->next) {
        if (d == victim->desc) {
            close_socket(d);
            send_to_char("Ok.\n\r", ch);
            return;
        }
    }

    bug("Do_disconnect: desc not found.", 0);
    send_to_char("Descriptor not found!\n\r", ch);
    return;
}

// by Amethyst
// do_check_bit: Command: do_check_bit - Performs the 'check_bit' command.
// do_check_bit: 指令：do_check_bit - 執行 'check_bit' 指令。
void do_check_bit(CHAR_DATA *ch, char *argument) {
    char buf1[MAX_INPUT_LENGTH];
    char buf0[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    unsigned long int flags;
    int total = 0;
    int found = 0;
    int k = 1;
    int i = 0;
    one_argument(argument, arg);

    if (arg[0] == '\0' || !is_number(arg)) {
        send_to_char("Syntax: checkbit <flags>.\n\r", ch);
        return;
    }

    flags = strtoul(arg, (char **)NULL, 10);

    buf1[0] = '\0';
    buf0[0] = '\0';
    for (i = 0; total < flags; i++) {
        k = 1;
        k = k << i;
        if (IS_SET(flags, k)) {
            found++;
            if (found == 1)
                sprintf(buf1, "%d", k);
            else
                sprintf(buf1, "|%d", k);
            total += k;
            strcat(buf0, buf1);
        }
    }
    strcat(buf0, "\n\r");
    send_to_char(buf0, ch);
    return;
}

// do_pardon: Command: do_pardon - Performs the 'pardon' command.
// do_pardon: 指令：do_pardon - 執行 'pardon' 指令。
void do_pardon(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "pardon"))
        return;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0') {
        send_to_char("使用方法: pardon <人物> <killer|thief>.\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg1))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("不能對 NPC 使用.\n\r", ch);
        return;
    }

    if (!str_cmp(arg2, "killer")) {
        if (IS_SET(victim->act, PLR_KILLER)) {
            REMOVE_BIT(victim->act, PLR_KILLER);
            send_to_char("Killer 頭銜已清除.\n\r", ch);
            send_to_char("你不再是 KILLER 了.\n\r", victim);
        }
        return;
    }

    if (!str_cmp(arg2, "thief")) {
        if (IS_SET(victim->act, PLR_THIEF)) {
            REMOVE_BIT(victim->act, PLR_THIEF);
            send_to_char("Thief 頭銜已清除.\n\r", ch);
            send_to_char("你不再是 THIEF 了.\n\r", victim);
        }
        return;
    }

    send_to_char("使用方法: pardon <人物> <killer|thief>.\n\r", ch);
    return;
}

// do_echo: Command: do_echo - Performs the 'echo' command.
// do_echo: 指令：do_echo - 執行 'echo' 指令。
void do_echo(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "echo"))
        return;

    if (argument[0] == '\0') {
        send_to_char("要 echo 什麼?\n\r", ch);
        return;
    }

    strcat(argument, "\n\r");
    send_to_all_char(argument);

    return;
}

// do_recho: Command: do_recho - Performs the 'recho' command.
// do_recho: 指令：do_recho - 執行 'recho' 指令。
void do_recho(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    DESCRIPTOR_DATA *d;

    rch = get_char(ch);

    if (!authorized(rch, "recho"))
        return;

    if (argument[0] == '\0') {
        send_to_char("要 recho 什麼?\n\r", ch);
        return;
    }

    for (d = descriptor_list; d; d = d->next) {
        if (d->connected == CON_PLAYING &&
            d->character->in_room == ch->in_room) {
            send_to_char(argument, d->character);
            send_to_char("\n\r", d->character);
        }
    }

    return;
}

ROOM_INDEX_DATA *find_location(CHAR_DATA *ch, char *arg) {
    OBJ_DATA *obj;
    CHAR_DATA *victim;

    if (is_number(arg))
        return get_room_index(atoi(arg));

    if ((victim = get_char_world(ch, arg)))
        return victim->in_room;

    if ((obj = get_obj_world(ch, arg)))
        return obj->in_room;

    return NULL;
}

// do_transfer: Command: do_transfer - Performs the 'transfer' command.
// do_transfer: 指令：do_transfer - 執行 'transfer' 指令。
void do_transfer(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *location;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int dir = -1;
    EXIT_DATA *pexit;

    rch = get_char(ch);

    if (!authorized(rch, "transfer"))
        return;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0') {
        send_to_char("Transfer 誰(到那裡)?\n\r", ch);
        return;
    }

    if (!str_cmp(arg1, "all")) {
        for (d = descriptor_list; d; d = d->next) {
            if (d->connected == CON_PLAYING && d->character != ch &&
                d->character->in_room && can_see(ch, d->character)) {
                char buf[MAX_STRING_LENGTH];

                sprintf(buf, "%s %s", d->character->name, arg2);
                do_transfer(ch, buf);
            }
        }
        return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if (arg2[0] == '\0') {
        location = ch->in_room;
    } else {
        if (!str_prefix(arg2, "up")) {
            dir = 4;
        } else if (!str_prefix(arg2, "down")) {
            dir = 5;
        } else if (!str_prefix(arg2, "north")) {
            dir = 0;
        } else if (!str_prefix(arg2, "east")) {
            dir = 1;
        } else if (!str_prefix(arg2, "south")) {
            dir = 2;
        } else if (!str_prefix(arg2, "west")) {
            dir = 3;
        }

        if (dir == -1 && !(location = find_location(ch, arg2))) {
            send_to_char("沒有這個地點.\n\r", ch);
            return;
        }

        /* 移除 transfer private room 設定 2022/12/20
        if ( dir == -1 && room_is_private( ch , location ) )
        {
                send_to_char( "那裡是 private 的房間.\n\r", ch );
                return;
        } */

        if (dir != -1)
            if (!(pexit = ch->in_room->exit[dir]) ||
                !(location = pexit->to_room)) {
                //		send_to_char( "[33m哎呀，此路不通。[m\n\r", ch
                //);
                return;
            }
    }

    if (!(victim = get_char_world(ch, arg1))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
        return;
    }

    if (!victim->in_room) {
        send_to_char("They are in limbo.\n\r", ch);
        return;
    }

    if (get_trust(ch) < get_trust(victim)) {
        send_to_char("You can't transfer your superior.\n\r", ch);
        return;
    }

    if (victim->fighting)
        stop_fighting(victim, TRUE);
    act("天空中伸出一隻大手，把$n抓走了.", victim, NULL, NULL, TO_ROOM);
    char_from_room(victim);
    char_to_room(victim, location);
    act("天空中伸出一隻大手，把$n丟在這裡.", victim, NULL, NULL, TO_ROOM);
    if (ch != victim)
        act("$n移動了你的位置.", ch, NULL, victim, TO_VICT);
    do_look(victim, "auto");
    send_to_char("Ok.\n\r", ch);
}

// do_at: Command: do_at - Performs the 'at' command.
// do_at: 指令：do_at - 執行 'at' 指令。
void do_at(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *wch;
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "at"))
        return;

    argument = one_argument(argument, arg);

    if (arg[0] == '\0' || argument[0] == '\0') {
        send_to_char("At 那裡(做什麼)?\n\r", ch);
        return;
    }

    if (!(location = find_location(ch, arg))) {
        send_to_char("沒有這個地點.\n\r", ch);
        return;
    }

    /*    if ( room_is_private( location ) )
        {
            send_to_char( "That room is private right now.\n\r", ch );
            return;
        }*/

    original = ch->in_room;
    char_from_room(ch);
    char_to_room(ch, location);
    interpret(ch, argument);

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for (wch = char_list; wch; wch = wch->next) {
        if (wch == ch) {
            char_from_room(ch);
            char_to_room(ch, original);
            break;
        }
    }

    return;
}

// do_goto: Command: do_goto - Performs the 'goto' command.
// do_goto: 指令：do_goto - 執行 'goto' 指令。
void do_goto(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    ROOM_INDEX_DATA *location;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "goto"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("Goto 那裡?\n\r", ch);
        return;
    }

    if (!(location = find_location(ch, arg))) {
        send_to_char("沒有這個地點.\n\r", ch);
        return;
    }

    // if ( room_is_private( location ) )
    //{
    //	send_to_char( "That room is private right now.\n\r", ch );
    //	return;
    //  }

    if (ch->fighting)
        stop_fighting(ch, TRUE);
    if (!IS_SET(ch->act, PLR_WIZINVIS)) {
        act("$n $T", ch, NULL,
            (ch->pcdata && ch->pcdata->bamfout[0] != '\0')
                ? ch->pcdata->bamfout
                : "的身影突然從你面前消失",
            TO_ROOM);
    }

    char_from_room(ch);
    char_to_room(ch, location);

    if (!IS_SET(ch->act, PLR_WIZINVIS)) {
        act("$n $T", ch, NULL,
            (ch->pcdata && ch->pcdata->bamfin[0] != '\0') ? ch->pcdata->bamfin
                                                          : "突然出現在這裡",
            TO_ROOM);
    }

    do_look(ch, "auto");
    return;
}

// do_rstat: Command: do_rstat - Performs the 'rstat' command.
// do_rstat: 指令：do_rstat - 執行 'rstat' 指令。
void do_rstat(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    ROOM_INDEX_DATA *location;
#ifdef HAS_ROOM_AFFECT
    R_AFFECT_DATA *paf;
#endif
    char buf[MAX_STRING_LENGTH * 2];
    char buf1[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int door;

    rch = get_char(ch);

    if (!authorized(rch, "rstat"))
        return;

    one_argument(argument, arg);
    location = (arg[0] == '\0') ? ch->in_room : find_location(ch, arg);
    if (!location) {
        send_to_char("沒有這個地點.\n\r", ch);
        return;
    }

    /* 移除 rstat private 設定 2022/12/20
    if ( ch->in_room != location && room_is_private( ch, location ) )
{
            send_to_char( "That room is private right now.\n\r", ch );
            return;
} */

    buf1[0] = '\0';

    sprintf(buf, "名稱: [%s]\n\r所屬區域: %s.\n\r", location->name,
            location->area->name);
    strcat(buf1, buf);

    sprintf(buf, "Vnum: %d.	Sector: %d.  Light: %d.\n\r", location->vnum,
            location->sector_type, location->light);
    strcat(buf1, buf);

    // room fall 2023/01/14
    if (location->fall != 0) {
        if (get_room_index(location->fall) == NULL)
            sprintf(buf, "Room Fall to: %s.\n\r", "Error: room vnum NOT exist");
        else
            sprintf(buf, "Room Fall to: %d.\n\r", location->fall);
    } else
        sprintf(buf, "Room Fall to: %s.\n\r", "(none)");
    strcat(buf1, buf);

    // room rembch 2021/11/05
    sprintf(buf, "Room Rember: %s.\n\r",
            location->rrembch ? location->rrembch->name : "(none)");
    strcat(buf1, buf);

    // 新增 room space, 值大於 0 才會影響 2022/12/20
    sprintf(buf, "Room Space: %d. %s\n\r", location->room_space,
            location->room_space > 0 ? "" HIR "USED" NOR "." : " ");
    strcat(buf1, buf);

    // 新增 room size, 值大於 0 才會影響 2022/12/20
    sprintf(buf, "Room Max Size: %d. %s\n\r", location->room_size,
            location->room_size > 0 ? "" HIR "USED" NOR "." : " ");
    strcat(buf1, buf);

    // room limited max char 2022/12/17
    if (IS_SET(location->room_flags, ROOM_PRIVATE)) {
        sprintf(buf, "Room Max Char: %d. " HIR "Limited Char" NOR " : %d.\n\r",
                location->max_char,
                location->max_char > 0 ? location->max_char : 2);
    } else {
        sprintf(buf, "Room Max Char: %d. Unlimited.\n\r", location->max_char);
    }
    strcat(buf1, buf);

    sprintf(buf, "Room flags: %s.\n\rDescription:\n\r%s",
            flag_string(room_flags, location->room_flags),
            location->description);
    strcat(buf1, buf);

    if (location->extra_descr) {
        EXTRA_DESCR_DATA *ed;

        strcat(buf1, "Extra description keywords: '");
        for (ed = location->extra_descr; ed; ed = ed->next) {
            strcat(buf1, ed->keyword);
            if (ed->next)
                strcat(buf1, " ");
        }
        strcat(buf1, "'.\n\r");
    }

    strcat(buf1, "房內生物:  ");

    /* Yes, we are reusing the variable rch.  - Kahn */
    for (rch = location->people; rch; rch = rch->next_in_room) {
        /* Heh.  Thanks to Zavod for this little security fix */
        if (can_see(ch, rch)) {
            one_argument(rch->name, buf);
            strcat(buf1, ", ");
            strcat(buf1, buf);
        }
    }

    strcat(buf1, ".\n\r\n\r房內物品:   ");
    for (obj = location->contents; obj; obj = obj->next_content) {
        one_argument(obj->name, buf);
        strcat(buf1, ", ");
        strcat(buf1, buf);
    }
    strcat(buf1, ".\n\r");

    for (door = 0; door <= 5; door++) {
        EXIT_DATA *pexit;

        if ((pexit = location->exit[door])) {
            sprintf(
                buf,
                "" HIB "%s" NOR
                " - 通往: %d. Key: %d.  Exit flags: %d. Exit size: %d %s\n\r",
                door == 0   ? "北"
                : door == 1 ? "東"
                : door == 2 ? "南"
                : door == 3 ? "西"
                : door == 4 ? "上"
                : door == 5 ? "下"
                            : "bug",
                pexit->to_room ? pexit->to_room->vnum : 0, pexit->key,
                pexit->exit_info,
                pexit->exit_size, // 2022/12/22
                pexit->exit_size > 0 ? "" HIR "Size Limit" NOR ""
                                     : " "); // 2022/12/22
            strcat(buf1, buf);
            sprintf(buf, "Keyword: %s.\n\r", pexit->keyword);
            strcat(buf1, buf);
            sprintf(buf, "Description: %s",
                    pexit->description[0] != '\0' ? pexit->description
                                                  : "(none).\n\r\n\r");
            strcat(buf1, buf);
        }
    }

    if (location->roomprogs) {
        sprintf(buf, "v0 = %d, v1 = %d, timer = %d, %d\n\r",
                location->roomprogs->v0, location->roomprogs->v1,
                location->roomprogs->timer[0], location->roomprogs->timer[1]);
        strcat(buf1, buf);
    }
#ifdef HAS_ROOM_AFFECT
    for (paf = location->affected; paf; paf = paf->next) {
        if (paf->deleted)
            continue;
        sprintf(buf, "Spell: '%s' modifier %d for %d hours with bits %s.\n\r",
                skill_table[(int)paf->type].name, paf->modifier, paf->duration,
                r_affect_bit_name(paf->bitvector));
        strcat(buf1, buf);
    }
#endif

    send_to_char(buf1, ch);
    return;
}

/*
// do_ostat: Command: do_ostat - Performs the 'ostat' command.
// do_ostat: 指令：do_ostat - 執行 'ostat' 指令。
void do_ostat( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA	*obj;
    CHAR_DATA	*rch;
    AFFECT_DATA *paf;
    char	 buf  [ MAX_STRING_LENGTH ];
    char	 buf1 [ MAX_STRING_LENGTH * 4];
    char	 arg  [ MAX_INPUT_LENGTH  ];
    int type= -1;

    BufEntry *ptr;
    char	*str;
    int i;

    rch = get_char( ch );

    if ( !authorized( rch, "ostat" ) )
        return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Ostat 什麼?\n\r", ch );

str = str_dup("start dumping");
ptr = (BufEntry *)( str - HEADER_SIZE );
for (i=0;i<100;i++)
{
 bug((char *)&ptr->buf[0],0);
 ptr = ptr->next;
}

        return;
    }

    buf1[0] = '\0';

    if ( !( obj = get_obj_world( ch, arg ) ) )
    {
        send_to_char( "沒有這樣物品.\n\r", ch);
        return;
    }

    sprintf( buf, "Name: [%s]\n\r",
            obj->name );
    strcat( buf1, buf );

    sprintf( buf, "Serial: [%d]\n\r", obj->serial);
    strcat(buf1, buf);


    if( obj->material ) {
        sprintf( buf, "Material: %s\n\r", obj->material->short_descr );
        strcat(buf1, buf);
    }

    sprintf( buf, "Vnum: %d.  Type: %s.\n\r",
            obj->pIndexData->vnum, item_type_name( obj ) );
    strcat( buf1, buf );

    sprintf( buf, "Short description: %s.\n\rLong description: %s\n\r",
            obj->short_descr, obj->description );
    strcat( buf1, buf );

    sprintf( buf, "Wear bits: %s.  Extra bits: %s.\n\r",
            wear_bit_name(obj->wear_flags), extra_bit_name( obj->extra_flags )
); strcat( buf1, buf );

    if( IS_SET(obj->extra_flags, ITEM_UNIQUE)){
        sprintf(buf, "Unique, Replica : %d.\n\r", obj->pIndexData->replica );
        strcat(buf1, buf);
    }

    sprintf( buf, "Number: %d/%d.  Weight: %d/%d.\n\r",
            1,		 get_obj_number( obj ),
            obj->weight, get_obj_weight( obj ) );
    strcat( buf1, buf );

    sprintf( buf, "Cost: %d.  Timer: %d.  Level: %d.  Class: %d.  Race: %d\n\r",
            obj->cost, obj->timer, obj->level, obj->class, obj->race );
    strcat( buf1, buf );

    sprintf( buf,
            "In room: %d.  In object: %s%s%s%s.	 Carried by: %s%s%s%s.
Wear_loc: %d.\n\r", !obj->in_room    ?	      0 : obj->in_room->vnum,
            !obj->in_obj     ?	     "" : obj->in_obj->short_descr,
            !obj->in_obj     ?	     "" : "(",
            !obj->in_obj     ? "（無）" : obj->in_obj->name,
            !obj->in_obj     ?	     "" : ")",
            !obj->carried_by ?	     "" : obj->carried_by->short_descr,
            !obj->carried_by ?	     "" : "(",
            !obj->carried_by ? "（無）" : obj->carried_by->name,
            !obj->carried_by ?	     "" : ")",
            obj->wear_loc );
    strcat( buf1, buf );

    sprintf( buf, "Values: %d %d %d %d.\n\r",
            obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
    strcat( buf1, buf );

    sprintf(buf, "Durability: %d/%d.\n\r",
            get_curr_dura(obj), get_max_dura(obj));
    strcat(buf1, buf);

    if ( obj->extra_descr || obj->pIndexData->extra_descr )
    {
        EXTRA_DESCR_DATA *ed;

        strcat( buf1, "Extra description keywords: '" );

        for ( ed = obj->extra_descr; ed; ed = ed->next )
        {
            strcat( buf1, ed->keyword );
            if ( ed->next )
                strcat( buf1, " " );
        }

        for ( ed = obj->pIndexData->extra_descr; ed; ed = ed->next )
        {
            strcat( buf1, ed->keyword );
            if ( ed->next )
                strcat( buf1, " " );
        }

        strcat( buf1, "'.\n\r" );
    }

    for ( paf = obj->affected; paf; paf = paf->next )
    {
        sprintf( buf, "Affects %s by %d.\n\r",
                affect_loc_name( paf->location ), paf->modifier );
        strcat( buf1, buf );
    }

    for ( paf = obj->pIndexData->affected; paf; paf = paf->next )
    {
        sprintf( buf, "Affects %s by %d.\n\r",
                affect_loc_name( paf->location ), paf->modifier );
        strcat( buf1, buf );
    }


    for ( paf = obj->material->affected; paf; paf = paf->next )
    {
        sprintf( buf, "MAffects %s by %d.\n\r",
                affect_loc_name( paf->location ), paf->modifier );
        strcat( buf1, buf );
    }

    if( obj->vrace ) {
        OBJ_VRACE * ovr ;
        for( ovr = obj->vrace ; ovr ; ovr = ovr->next )
        {
            sprintf( buf, "VRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
                    race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p,
                    ovr->dr_mod_q, ovr->dr_mod_p);
            strcat(buf1, buf);
        }
    }


    if( obj->material->vrace ) {
        OBJ_VRACE * ovr ;
        for( ovr = obj->material->vrace ; ovr ; ovr = ovr->next )
        {
            sprintf( buf, "MVRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
                    race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p,
                    ovr->dr_mod_q, ovr->dr_mod_p);
            strcat(buf1, buf);
        }
    }

    if( obj->item_type == ITEM_ARROW ) type = 14;
    else if( obj->item_type == ITEM_WEAPON) type = obj->value[3];
    if( type > -1 && type < MAX_TYPES ) {
        OBJ_VRACE * ovr ;
        for( ovr = (basic_types[type])->vrace ; ovr ; ovr = ovr->next )
        {
            sprintf( buf, "BVRACE: %s, hr %d/%d%%, dr %d/%d%%\n\r",
                    race_table[ovr->race].name, ovr->hr_mod_q, ovr->hr_mod_p,
                    ovr->dr_mod_q, ovr->dr_mod_p);
            strcat(buf1, buf);
        }
    }

    if ( obj->hp )
    {
        OBJ_HP * ohp = obj->hp;
        for( ; ohp; ohp = ohp->next ) {
            sprintf( buf, "HP: %s. percent: %d. level: %d. event: %d.\n\r",
                    ohp->hp_name, ohp->percent, ohp->level,
                    ohp->event );
        strcat( buf1, buf );
        }
    }

    if( obj->material->hp )
    {
        OBJ_HP * ohp = obj->material->hp;
        for( ; ohp; ohp = ohp->next ) {
            sprintf( buf, "MHP: %s. percent: %d. level: %d. event: %d.\n\r",
                    ohp->hp_name, ohp->percent, ohp->level,
                    ohp->event );
        strcat( buf1, buf );
        }
    }

    send_to_char( buf1, ch );
    return;
}
*/

// do_mstat: Command: do_mstat - Performs the 'mstat' command.
// do_mstat: 指令：do_mstat - 執行 'mstat' 指令。
void do_mstat(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim, *vch;
    QMARK *qmark;
    AFFECT_DATA *paf;
    MOB_WEAKNESS *weak;
    char buf[MAX_STRING_LENGTH * 7];
    char buf1[MAX_STRING_LENGTH * 7];
    char arg[MAX_INPUT_LENGTH];
    int count = 0;

    rch = get_char(ch);

    if (!authorized(rch, "mstat"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Mstat 誰?\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    buf1[0] = '\0';

    sprintf(buf, "Name: [%s]\n\r", victim->name);
    strcat(buf1, buf);

    sprintf(buf, "種族: %s\n\r", race_table[victim->race].name);
    strcat(buf1, buf);

    sprintf(buf, "Vnum: %d.  性別: %s.	位於: %d.\n\r",
            IS_NPC(victim) ? victim->pIndexData->vnum : 0,
            victim->sex == SEX_MALE     ? "男"
            : victim->sex == SEX_FEMALE ? "女"
                                        : "中性",
            !victim->in_room ? 0 : victim->in_room->vnum);
    strcat(buf1, buf);

    sprintf(buf,
            "Str: %d.  Int: %d.  Wis: %d.	 Dex: %d.  Con: %d.  Size: "
            "%d(mod %d)   Exp %d.\n\r",
            get_curr_str(victim), get_curr_int(victim), get_curr_wis(victim),
            get_curr_dex(victim), get_curr_con(victim),
            get_curr_size(victim), // 加入體型 size 2022/12/23
            victim->mod_size,      // 加入體型 size 2022/12/23
            victim->exp);
    strcat(buf1, buf);

    sprintf(buf, "Hp: %d/%d.  Mana: %d/%d.  Move: %d/%d.  Practices: %d.\n\r",
            victim->hit, victim->max_hit, victim->mana, victim->max_mana,
            victim->move, victim->max_move, victim->practice);
    strcat(buf1, buf);

    if (IS_NPC(victim)) {
        sprintf(buf, "MOB Skill level: %d\n\r", GetGeneralMobSklv(victim));
        strcat(buf1, buf);
    }

    sprintf(buf, "Lv: %d.  Class: %s(%d).  Align: %d.  AC: %d. DODGE: %d.\n\r",
            victim->level,
            IS_NPC(victim) ? "Mob" : class_table[victim->class].cname,
            victim->class, victim->alignment, GET_AC(victim),
            GET_DODGE(victim));
    strcat(buf1, buf);
    // 新增 sv poison 數值 2021/11/03
    sprintf(buf,
            "SVS: %d, VsFire: %d  VsCold: %d  VsWind: %d  \n\rvsEarth: %d "
            "VsBreath: %d VsLight: %d VsSaint: %d VsDark: %d VsPoison: %d \n\r",
            GET_SVS(victim), NEW_GET_SVS(victim, MAGIC_FIRE),
            NEW_GET_SVS(victim, MAGIC_COLD), NEW_GET_SVS(victim, MAGIC_WIND),
            NEW_GET_SVS(victim, MAGIC_EARTH), NEW_GET_SVS(victim, MAGIC_BREATH),
            NEW_GET_SVS(victim, MAGIC_LIGHTNING),
            NEW_GET_SVS(victim, MAGIC_SAINT), NEW_GET_SVS(victim, MAGIC_DARK),
            NEW_GET_SVS(victim, MAGIC_POISON));
    strcat(buf1, buf);

    sprintf(buf, "P.P: %d. Gold: %d. Exp: %d. SkillExp %d/%d/%d\n\r",
            victim->pp, victim->gold, victim->exp, get_eff_skill_exp(victim),
            get_skill_exp(victim), get_max_skill_exp(victim));
    strcat(buf1, buf);

    sprintf(buf, "Wait: %d.\n\r", victim->wait);
    strcat(buf1, buf);

    if (!IS_NPC(victim)) {
        sprintf(buf, "Respect: %d, ", victim->pcdata->respect);
        strcat(buf1, buf);
    }

    sprintf(buf, "Position: %d.  Wimpy: %d.\n\r", victim->position,
            victim->wimpy);
    strcat(buf1, buf);

    if (IS_NPC(victim)
        //|| victim->pcdata->learned[gsn_dual] != 0 )
        || get_skill_percent(victim, gsn_dual) != 0)
        strcat(buf1, "Primary Weapon ");
    sprintf(buf, "Hitroll: %d	Damroll: %d.\n\r",
            get_hitroll(victim, WEAR_WIELD), get_damroll(victim, WEAR_WIELD));
    strcat(buf1, buf);

    if (get_eq_char(victim, WEAR_WIELD_2)) {
        sprintf(buf, " Second Weapon Hitroll: %d  Damroll: %d.\n\r",
                get_hitroll(victim, WEAR_WIELD_2),
                get_damroll(victim, WEAR_WIELD_2));
        strcat(buf1, buf);
    }

    if (!IS_NPC(victim)) {
        sprintf(buf, "Page Lines: %d.\n\r", victim->pcdata->pagelen);
        strcat(buf1, buf);
    }

    if (!IS_NPC(victim)) {
        sprintf(buf, "Switched : %s.\n\r",
                (victim->pcdata->switched) ? "Yes" : "No");
        strcat(buf1, buf);
        sprintf(buf, "Ride : %d.  Horse : %s\n\r", victim->pcdata->ride,
                (victim->pcdata->horse ? victim->pcdata->horse->name : "N/A"));
        strcat(buf1, buf);
    }

    sprintf(buf, "Fighting: %s.  ",
            victim->fighting ? victim->fighting->name : "(none)");
    strcat(buf1, buf);
    sprintf(buf, "Hunting: %s.\n\r",
            victim->hunting ? victim->hunting->name : "(none)");
    strcat(buf1, buf);
    strcat(buf1, "Be hunted by:");
    if ((victim->hunted))
        for (vch = victim->hunted; vch; vch = vch->next_in_hunt) {
            sprintf(buf, " %s,", vch->name);
            strcat(buf1, buf);
        }
    else
        strcat(buf1, " <無>.");

    strcat(buf1, "\n\r");
    sprintf(buf, "Rember: %s.\n\r",
            victim->rembch ? victim->rembch->name : "(none)");
    strcat(buf1, buf);
    if (!IS_NPC(victim) && (victim->tracking)) {
        sprintf(buf, "Tracking: %s\n\r", victim->tracking->short_descr);
        strcat(buf1, buf);
    }

    if ((vch = victim->linking)) {
        sprintf(buf, "Linking with: %s.  ", vch->name);
        strcat(buf1, buf);
    } else
        strcat(buf1, "Linking with: <None>.\n\r");

    if ((victim->cooperator) && victim->cooperator != victim) {
        strcat(buf1, "Cooperated by :");
        for (vch = victim->cooperator; vch; vch = vch->next_in_coop) {
            sprintf(buf, " %s,", vch->name);
            strcat(buf1, buf);
        }
        strcat(buf1, ".  ");
    } else
        strcat(buf1, "Cooperated by : <None>.  ");

    if ((vch = victim->cooperating)) {
        sprintf(buf, "Cooperating: %s.\n\r", vch->name);
        strcat(buf1, buf);
    } else
        strcat(buf1, "Cooperating: <None>.\n\r");

    if ((victim->protector) && victim->protector != victim) {
        strcat(buf1, "Protected by :");
        for (vch = victim->protector; vch; vch = vch->next_in_protect) {
            sprintf(buf, " %s,", vch->name);
            strcat(buf1, buf);
        }
        strcat(buf1, ".  ");
    } else
        strcat(buf1, "Protected by : <None>.  ");

    if ((vch = victim->protecting)) {
        sprintf(buf, "Protecting: %s.  ", vch->name);
        strcat(buf1, buf);
    } else
        strcat(buf1, "Protecting: <None>.\n\r");

    if (!IS_NPC(victim)) {
        sprintf(buf,
                "Thirst: %d.  Full: %d.	 Drunk: %d.  Saving throw: %d.\n\r",
                victim->pcdata->condition[COND_THIRST],
                victim->pcdata->condition[COND_FULL],
                victim->pcdata->condition[COND_DRUNK], GET_SVS(victim));
        strcat(buf1, buf);
    }

    sprintf(buf,
            "Magic damage reduced by %d,  Physical damage reduced by %d.\n\r",
            victim->mdr, victim->pdr);
    strcat(buf1, buf);
    sprintf(buf,
            "Magic damage enhanced by %d,  Physical damage enhanced by %d.\n\r",
            victim->amdr, victim->apdr);
    strcat(buf1, buf);
    sprintf(buf, "Magic damage gate: %d,  Physical damage gate: %d.\n\r",
            victim->mad, victim->pad);
    strcat(buf1, buf);

    sprintf(buf, "Carry number: %d.  Carry weight: %d.\n\r",
            victim->carry_number, victim->carry_weight);
    strcat(buf1, buf);

    // 加入 stay_time 2023/01/15
    sprintf(
        buf,
        "Age: %d.  Played: %d.  Timer: %d.  Stay Time: %d. \n\rAct: %s.\n\r",
        get_age(victim), (int)victim->played, victim->timer,
        victim->stay_time, // 2023/01/15
        act_bit_name(victim->act));
    strcat(buf1, buf);

    sprintf(buf, "Master: %s.	Leader: %s.  \n\rAffected by: %s.\n\r",
            victim->master ? victim->master->name : "<無>",
            (victim->group && victim->group->leader)
                ? victim->group->leader->name
                : "<無>",
            affect_bit_name(victim->affected_by));
    strcat(buf1, buf);

    if (!IS_NPC(victim)) /* OLC */
    {
        sprintf(buf, "Security: %d.\n\r", victim->pcdata->security);
        strcat(buf1, buf);
    }

    sprintf(buf, "Short description: %s.\n\rLong  description: %s",
            victim->short_descr,
            victim->long_descr[0] != '\0' ? victim->long_descr : "<無>.\n\r");
    strcat(buf1, buf);

    sprintf(buf, "Special : %s\n\r", special_bit_name(victim->special));
    strcat(buf1, buf);

    if (!IS_NPC(victim)) {
        sprintf(buf, "Title: [%s]\n\r", victim->pcdata->title);
        strcat(buf1, buf);
    }
    if (victim->clan) {
        sprintf(buf, "Clan: %s\n\r", victim->clan->name);
        strcat(buf1, buf);
    }

    if (IS_NPC(victim) && victim->spec_fun != 0)
        strcat(buf1, "Mobile has spec fun.\n\r");

    send_to_char(buf1, ch);
    buf1[0] = '\0';
    if (IS_NPC(victim) && victim->jcl) {
        strcat(buf1, "Join:");
        strcat(buf1, victim->jcl);
        strcat(buf1, "\n\r");
    }

    if (IS_NPC(victim) && victim->psk) {
        strcat(buf1, "Skills:");
        strcat(buf1, victim->psk);
        strcat(buf1, "\n\r");
    }

    send_to_char(buf1, ch);
    buf1[0] = '\0';

    for (paf = victim->affected; paf; paf = paf->next) {
        if (paf->deleted)
            continue;
        ++count;
        sprintf(
            buf, "Spell: '%s' modifies %s by %d for %d hours with bits %s.\n\r",
            skill_table[(int)paf->type].name, affect_loc_name(paf->location),
            paf->modifier, paf->duration, affect_bit_name(paf->bitvector));
        strcat(buf1, buf);
        // if( count >= 50 ) { 2022/10/14
        if (count >= 80) {
            send_to_char(buf1, ch);
            count = 0;
            buf1[0] = '\0';
        }
    }

    if (victim->weakness) {
        for (weak = victim->weakness; weak; weak = weak->next) {
            ++count;
            sprintf(buf, "Weakness: [%s] percent %d%%, damage %d%%\n\r",
                    weak->name, weak->percent, weak->damage);
            strcat(buf1, buf);
            // if( count >= 50 ) { 2022/10/14
            if (count >= 80) {
                send_to_char(buf1, ch);
                count = 0;
                buf1[0] = '\0';
            }
        }
    } else
        strcat(buf1, "Weakness: (none)\n\r");

    send_to_char(buf1, ch);
    count = 0;
    buf1[0] = '\0';

    if (victim->qmark) {
        strcat(buf1, "\n\r身上 Qmark如下：\n\r");
        for (qmark = victim->qmark; qmark; qmark = qmark->next) {
            ++count;

            // 針對 pc qmark 加入 qmark death 及 save 顯示  2022/10/23

            if (IS_NPC(victim)) {
                sprintf(buf, "[%s] %d %d %d %d\n\r", qmark->mark_name,
                        qmark->timer, qmark->v0, qmark->v1, qmark->v2);
            } else {
                sprintf(buf, "[%s] %d %d %d %d  %s %s\n\r", qmark->mark_name,
                        qmark->timer, qmark->v0, qmark->v1, qmark->v2,
                        qmark->fDeath ? "" HIR "Death" NOR "" : "",
                        qmark->fSave ? "" : "" HIP "No_Save" NOR "");
            }
            strcat(buf1, buf);

            // if( count >= 50 ) { 2022/10/14
            if (count >= 80) {
                send_to_char(buf1, ch);
                count = 0;
                buf1[0] = '\0';
            }
        }
    }

    send_to_char(buf1, ch);
    return;
}

// do_mfind: Command: do_mfind - Performs the 'mfind' command.
// do_mfind: 指令：do_mfind - 執行 'mfind' 指令。
void do_mfind(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    MOB_INDEX_DATA *pMobIndex;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 5];
    char arg[MAX_INPUT_LENGTH];
    extern int top_mob_index;
    int vnum;
    int nMatch;
    int count = 0;
    bool fAll;
    bool found;

    rch = get_char(ch);

    if (!authorized(rch, "mfind"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("Mfind whom?\n\r", ch);
        return;
    }

    buf1[0] = '\0';
    fAll = !str_cmp(arg, "all");
    found = FALSE;
    nMatch = 0;

    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_mob_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for (vnum = 0; nMatch < top_mob_index; vnum++) {
        if (vnum > MAX_VNUM)
            break;
        if ((pMobIndex = get_mob_index(vnum))) {
            ++nMatch;
            if (fAll || is_name(arg, pMobIndex->player_name)) {
                found = TRUE;
                ++count;
                sprintf(buf, "[%5d] %s, <%s>\n\r", pMobIndex->vnum,
                        capitalize(pMobIndex->short_descr),
                        pMobIndex->player_name);
                //		if ( !fAll )
                //		    strcat( buf1, buf );
                //		else
                //		    send_to_char( buf, ch );
                strcat(buf1, buf);
                // if( count >= 50 ) { 2022/10/14
                if (count >= 80) {
                    count = 0;
                    send_to_char(buf1, ch);
                    buf1[0] = '\0';
                }
            }
        }
    }

    if (!found) {
        send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
        return;
    }

    send_to_char(buf1, ch);
    return;
}

// do_ofind: Command: do_ofind - Performs the 'ofind' command.
// do_ofind: 指令：do_ofind - 執行 'ofind' 指令。
void do_ofind(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    OBJ_INDEX_DATA *pObjIndex;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 6];
    char arg[MAX_INPUT_LENGTH];
    extern int top_obj_index;
    int vnum;
    int nMatch;
    int count = 0;
    bool fAll;
    bool found;

    rch = get_char(ch);

    if (!authorized(rch, "ofind"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("Ofind what?\n\r", ch);
        return;
    }

    buf1[0] = '\0';
    fAll = !str_cmp(arg, "all");
    found = FALSE;
    nMatch = 0;

    for (vnum = 0; nMatch < top_obj_index; vnum++) {
        if (vnum > MAX_VNUM)
            break;
        if ((pObjIndex = get_obj_index(vnum))) {
            nMatch++;
            if (fAll || is_name(arg, pObjIndex->name)) {
                found = TRUE;
                ++count;
                sprintf(buf, "[%5d] %s,<%s>\n\r", pObjIndex->vnum,
                        capitalize(pObjIndex->short_descr), pObjIndex->name);
                /*if ( !fAll )
                    strcat( buf1, buf );
                else
                    send_to_char( buf, ch );*/
                strcat(buf1, buf);
                // if( count >= 50 ) { 2022/10/14
                if (count >= 80) {
                    count = 0;
                    send_to_char(buf1, ch);
                    buf1[0] = '\0';
                }
            }
        }
    }

    if (!found) {
        send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
        return;
    }

    send_to_char(buf1, ch);
    return;
}

// do_mwhere: Command: do_mwhere - Performs the 'mwhere' command.
// do_mwhere: 指令：do_mwhere - 執行 'mwhere' 指令。
void do_mwhere(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *to;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 6];
    char arg[MAX_INPUT_LENGTH];
    bool found;
    int count = 0;

    rch = get_char(ch);

    if (!authorized(rch, "mwhere"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("Mwhere whom?\n\r", ch);
        return;
    }

    buf1[0] = '\0';
    found = FALSE;
    if (is_name(arg, "-a")) {
        for (d = descriptor_list; d; d = d->next) {
            to = (d->original) ? d->original : d->character;

            if (!to)
                continue;
            if (!to->in_room)
                continue;
            if (get_trust(to) == 987)
                continue;
            if (to->desc) {
                found = TRUE;
                sprintf(buf, "[%5d] ", to->level);
                strcat(buf1, buf);
                switch (to->position) {
                case POS_DEAD:
                    strcat(buf1, RED "它已死了" NOR);
                    break;
                case POS_MORTAL:
                    strcat(buf1, HIR "受致命傷" NOR);
                    break;
                case POS_INCAP:
                    strcat(buf1, HIB "失去行動" NOR);
                    break;
                case POS_STUNNED:
                    strcat(buf1, HIB "不省人事" NOR);
                    break;
                case POS_SLEEPING:
                    strcat(buf1, HIU "它睡著了" NOR);
                    break;
                case POS_RESTING:
                    strcat(buf1, HIG "它在休息" NOR);
                    break;
                case POS_STANDING:
                    strcat(buf1, HIB "它在站著" NOR);
                    break;
                case POS_FIGHTING:
                    strcat(buf1, HIY "在戰鬥中" NOR);
                    break;
                default:
                    break;
                }

                sprintf(buf, " %s 位在 [%5d] %s\n\r", to->short_descr,
                        to->in_room->vnum, to->in_room->name);
                strcat(buf1, buf);
            }
        }
    } else {
        for (victim = char_list; victim; victim = victim->next) {
            if (/*IS_NPC( victim ) && */
                victim->in_room && (is_name(arg, victim->name))) {
                ++count;
                found = TRUE;
                if (IS_NPC(victim)) {
                    sprintf(buf, "[%5d] %s, <%s> 位在 [%5d] %s\n\r",
                            victim->pIndexData->vnum, victim->short_descr,
                            victim->name, victim->in_room->vnum,
                            victim->in_room->name);
                    strcat(buf1, buf);
                } else {
                    sprintf(buf, "[%5d] %s, <%s> 位在 [%5d] %s\n\r",
                            victim->level, victim->short_descr, victim->name,
                            victim->in_room->vnum, victim->in_room->name);
                    strcat(buf1, buf);
                }
            }

            // if( count >= 50 ) { 2022/10/14
            if (count >= 80) {
                send_to_char(buf1, ch);
                buf1[0] = '\0';
                count = 0;
            }
        }
    }

    if (!found) {
        act("You didn't find any $T.", ch, NULL, arg, TO_CHAR);
        return;
    }

    send_to_char(buf1, ch);
    return;
}

// do_cras: Command: do_cras - Performs the 'cras' command.
// do_cras: 指令：do_cras - 執行 'cras' 指令。
void do_cras(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "crash"))
        return;

    send_to_char("If you want to make CRASH, spell it out.\n\r", ch);
    return;
}
// do_crash: Command: do_crash - Performs the 'crash' command.
// do_crash: 指令：do_crash - 執行 'crash' 指令。
void do_crash(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    sprintf(buf, "[make crash] --> %s used the crash command.", ch->name);
    log_string(buf);

    rch = get_char(ch);

    if (!authorized(rch, "crash"))
        return;

    send_to_char("還是別玩這種遊戲吧 ... \n\r", ch);

    return;
}

// do_reboo: Command: do_reboo - Performs the 'reboo' command.
// do_reboo: 指令：do_reboo - 執行 'reboo' 指令。
void do_reboo(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "reboot"))
        return;

    send_to_char("If you want to REBOOT, spell it out.\n\r", ch);
    return;
}

// do_reboot: Command: do_reboot - Performs the 'reboot' command.
// do_reboot: 指令：do_reboot - 執行 'reboot' 指令。
void do_reboot(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;

    rch = get_char(ch);

    if (!authorized(rch, "reboot"))
        return;

    sprintf(buf, "Reboot by %s.", ch->name);
    do_echo(ch, buf);

    end_of_game();

    /*add by neversay on 2004/June/21 for startupSH script reboot correct.
    if mud crash or reboot, the SHUTDOWN.TXT will exist, then script reboot
    correct, if the SHUTDOWN.TXT isnot exist, the script will exit.*/
    //    system( "rm SHUTDOWN.TXT" );

    merc_down = TRUE;
    return;
}

// do_shutdow: Command: do_shutdow - Performs the 'shutdow' command.
// do_shutdow: 指令：do_shutdow - 執行 'shutdow' 指令。
void do_shutdow(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "shutdown"))
        return;

    send_to_char("If you want to SHUTDOWN, spell it out.\n\r", ch);
    return;
}

// do_shutdown: Command: do_shutdown - Performs the 'shutdown' command.
// do_shutdown: 指令：do_shutdown - 執行 'shutdown' 指令。
void do_shutdown(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;

    rch = get_char(ch);

    if (!authorized(rch, "shutdown"))
        return;

    sprintf(buf, "Shutdown by %s.", ch->name);
    append_file(ch, SHUTDOWN_FILE, buf);
    strcat(buf, "\n\r");
    do_echo(ch, buf);

    /*add by neversay on 2004/June/21 for startupSH script reboot correct.
    if mud crash or reboot, the SHUTDOWN.TXT will exist, then script reboot
    correct, if the SHUTDOWN.TXT isnot exist, the script will exit.*/
    system("rm SHUTDOWN.TXT");

    end_of_game();

    merc_down = TRUE;
    return;
}

// do_snoop: Command: do_snoop - Performs the 'snoop' command.
// do_snoop: 指令：do_snoop - 執行 'snoop' 指令。
void do_snoop(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "snoop"))
        return;

    if (get_trust(rch) <= 44)
        return;
    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Snoop whom?\n\r", ch);
        return;
    }

    // 加入 no-idle 的選項, 針對有在活動的所有角色作 snoop 2023/01/17
    if (!str_cmp(arg, "no-idle")) {
        // char buf [ MAX_INPUT_LENGTH ];
        CHAR_DATA *vch_next;
        for (victim = char_list; victim != NULL; victim = vch_next) {
            vch_next = victim->next;
            if (IS_NPC(victim))
                continue;
            if (victim == ch)
                continue;
            if (!victim->desc)
                continue;
            if (victim->desc->snoop_by)
                continue;
            if (get_trust(victim) >= get_trust(ch))
                continue;
            if (IS_SET(victim->act, PLR_IDLE))
                continue;
            // sprintf( buf , "%s", victim->name );
            do_snoop(ch, argument);
            // send_to_char( buf , ch );
        }
        return;
    } else if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (!victim->desc) {
        send_to_char("No descriptor to snoop.\n\r", ch);
        return;
    }

    if (victim == ch) {
        send_to_char("Cancelling all snoops.\n\r", ch);
        for (d = descriptor_list; d; d = d->next) {
            if (d->snoop_by == ch->desc)
                d->snoop_by = NULL;
        }
        return;
    }

    if (victim->desc->snoop_by) {
        send_to_char("Busy already.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("You failed.\n\r", ch);
        return;
    }

    if (ch->desc) {
        for (d = ch->desc->snoop_by; d; d = d->snoop_by) {
            if (d->character == victim || d->original == victim) {
                send_to_char("No snoop loops.\n\r", ch);
                return;
            }
        }
    }

    victim->desc->snoop_by = ch->desc;
    // 修改為 act 顯示 snoop 角色名 2023/01/19
    // send_to_char( " Ok.\n\r", ch );
    act("Snoop $N OK.", ch, NULL, victim, TO_CHAR);
    return;
}

// do_switch: Command: do_switch - Performs the 'switch' command.
// do_switch: 指令：do_switch - 執行 'switch' 指令。
void do_switch(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "switch"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Switch into whom?\n\r", ch);
        return;
    }

    if (!ch->desc)
        return;

    if (ch->desc->original) {
        send_to_char("You are already switched.\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (victim == ch) {
        send_to_char("Ok.\n\r", ch);
        return;
    }

    /*
     * Pointed out by Da Pub (What Mud)
     */
    if (!IS_NPC(victim) && !IS_SET(victim->act, PLR_HORSE)) {
        send_to_char("You cannot switch into a player!\n\r", ch);
        return;
    }

    // Added by Razgriz		20050621
    if (IS_SET(victim->act, PLR_HORSE)) {
        send_to_char("You cannot switch into a HORSE!\n\r", ch);
        return;
    }

    if (victim->desc) {
        send_to_char("Character in use.\n\r", ch);
        return;
    }

    ch->pcdata->switched = TRUE;
    ch->desc->character = victim;
    ch->desc->original = ch;
    victim->desc = ch->desc;
    ch->desc = NULL;
    send_to_char("Ok.\n\r", victim);
    return;
}

// do_return: Command: do_return - Performs the 'return' command.
// do_return: 指令：do_return - 執行 'return' 指令。
void do_return(CHAR_DATA *ch, char *argument) {
    if (!ch->desc)
        return;

    if (!ch->desc->original) {
        send_to_char("You aren't switched.\n\r", ch);
        return;
    }

    /* Note that we dont check for immortal ability to have return here.
     * We assume we will automatically allow immortals with switch to return.
     * Dont want to have our immortals stuck in a mobile's body do we?  :)
     * -Kahn */

    send_to_char("You return to your original body.\n\r", ch);
    ch->desc->original->pcdata->switched = FALSE;
    ch->desc->character = ch->desc->original;
    ch->desc->original = NULL;
    ch->desc->character->desc = ch->desc;
    ch->desc = NULL;
    return;
}

// do_mload: Command: do_mload - Performs the 'mload' command.
// do_mload: 指令：do_mload - 執行 'mload' 指令。
void do_mload(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    MOB_INDEX_DATA *pMobIndex;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "mload"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0' || !is_number(arg)) {
        send_to_char("Syntax: mload <vnum>.\n\r", ch);
        return;
    }

    if (!(pMobIndex = get_mob_index(atoi(arg)))) {
        send_to_char("No mob has that vnum.\n\r", ch);
        return;
    }

    victim = create_mobile(pMobIndex);
    char_to_room(victim, ch->in_room);
    mprog_repop_trigger(victim);
    send_to_char("Ok.\n\r", ch);
    act("$n 變出了 $N。", ch, NULL, victim, TO_ROOM);
    act("你 變出了 $N。", ch, NULL, victim, TO_CHAR);
    return;
}

// do_oload: Command: do_oload - Performs the 'oload' command.
// do_oload: 指令：do_oload - 執行 'oload' 指令。
void do_oload(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    OBJ_INDEX_DATA *pObjIndex;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int level;

    rch = get_char(ch);

    if (!authorized(rch, "oload"))
        return;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0' || !is_number(arg1)) {
        send_to_char("Syntax: oload <vnum> <level>.\n\r", ch);
        return;
    }

    if (arg2[0] == '\0' || get_trust(ch) < L_SEN) {
        level = get_trust(ch);
    } else {
        /*
         * New feature from Alander.
         */
        if (!is_number(arg2)) {
            send_to_char("Syntax: oload <vnum> <level>.\n\r", ch);
            return;
        }
        level = atoi(arg2);
        if (level < 0 || level > get_trust(ch)) {
            send_to_char("Limited to your trust level.\n\r", ch);
            return;
        }
    }

    if (!(pObjIndex = get_obj_index(atoi(arg1)))) {
        send_to_char("No object has that vnum.\n\r", ch);
        return;
    }

    obj = fcreate_object(pObjIndex, level);
    if (CAN_WEAR(obj, ITEM_TAKE)) {
        obj_to_char(obj, ch);
        act("$n 憑空一指，變出了 $p。", ch, obj, NULL, TO_ROOM);
    } else {
        obj_to_room(obj, ch->in_room);
        act("$n 憑空一指，變出了 $p。", ch, obj, NULL, TO_ROOM);
    }
    act("你憑空一指，變出了 $p。", ch, obj, NULL, TO_CHAR);
    send_to_char("Ok.\n\r", ch);
    oprog_repop_trigger(obj);
    return;
}

// do_purge: Command: do_purge - Performs the 'purge' command.
// do_purge: 指令：do_purge - 執行 'purge' 指令。
void do_purge(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "purge"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        /* 'purge' */
        OBJ_DATA *obj_next;
        CHAR_DATA *vnext;

        for (victim = ch->in_room->people; victim; victim = vnext) {
            vnext = victim->next_in_room;
            if (victim->deleted)
                continue;

            if (IS_NPC(victim) && victim != ch &&
                !IS_SET(victim->act, ACT_HORSE)) {
                if (victim->pIndexData != NULL &&
                    IsSorGateMob(victim->pIndexData->vnum)) {
                    if (victim->master != NULL && !IS_NPC(victim->master)) {
                    } else {
                        extract_char(victim, TRUE);
                    }
                } else {
                    extract_char(victim, TRUE);
                }
            }
        }

        for (obj = ch->in_room->contents; obj; obj = obj_next) {
            obj_next = obj->next_content;
            if (obj->deleted)
                continue;
            extract_obj(obj);
        }

        send_to_char("Ok.\n\r", ch);
        act("$n purges the room!", ch, NULL, NULL, TO_ROOM);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (!IS_NPC(victim)) {
        send_to_char("Not on PC's.\n\r", ch);
        return;
    }

    act("$n purges $N.", ch, NULL, victim, TO_NOTVICT);
    extract_char(victim, TRUE);
    return;
}

// do_advance: Command: do_advance - Performs the 'advance' command.
// do_advance: 指令：do_advance - 執行 'advance' 指令。
void do_advance(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int level;
    int iLevel;

    rch = get_char(ch);

    if (!authorized(rch, "advance"))
        return;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0' || !is_number(arg2)) {
        send_to_char("Syntax: advance <char> <level>.\n\r", ch);
        return;
    }

    if (!(victim = get_char_room(ch, arg1))) {
        send_to_char("That player is not here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }
    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("Not on superior or peer.\n\r", ch);
    }

    level = atoi(arg2);

    if (level < 1 || level > MAX_LEVEL) {
        char buf[MAX_STRING_LENGTH];

        sprintf(buf, "Advance within range 1 to %d.\n\r", MAX_LEVEL);
        send_to_char(buf, ch);
        return;
    }

    if (level >= get_trust(ch)) {
        send_to_char("Limited to your trust level.\n\r", ch);
        return;
    }

    /*
     * Lower level:
     *	 Reset to level 1.
     *	 Then raise again.
     *	 Currently, an imp can lower another imp.
     *	 -- Swiftest
     */
    if (level <= victim->level) {
        int sn;

        send_to_char("Lowering a player's level!\n\r", ch);
        send_to_char("**** 你感到全身的力量逐漸離你遠去 ****\n\r", victim);
        victim->level = 1;
        // 20060722 modify by jye
        // 新增EXP_PER_LEVEL ppl_class變數
        victim->exp = EXP_PER_LEVEL(1, ch->class);
        victim->max_hit = 10;
        victim->max_mana = 100;
        victim->max_move = 100;
        for (sn = 0; sn < MAX_SKILL; sn++)
            victim->pcdata->learned[sn] = 0;
        victim->practice = 0;
        victim->hit = victim->max_hit;
        victim->mana = victim->max_mana;
        victim->move = victim->max_move;
        advance_level(victim);
    } else {
        send_to_char("Raising a player's level!\n\r", ch);
        send_to_char("**** 你渾身充滿力量，快活地呼號著 ****\n\r", victim);
    }

    for (iLevel = victim->level; iLevel < level; iLevel++) {
        send_to_char("You raise a level!!  ", victim);
        victim->level += 1;
        advance_level(victim);
    }
    // 20060722 modify by jye
    // 新增EXP_PER_LEVEL ppl_class變數
    victim->exp = EXP_PER_LEVEL(victim->level - 1, victim->class) *
                  UMAX(1, victim->level);
    //    victim->trust = 0;
    if (victim->trust <= victim->level)
        victim->trust = 0;
    return;
}

// do_trust: Command: do_trust - Performs the 'trust' command.
// do_trust: 指令：do_trust - 執行 'trust' 指令。
void do_trust(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int level;

    rch = get_char(ch);

    if (!authorized(rch, "trust"))
        return;

    argument = one_argument(argument, arg1);
    one_argument(argument, arg2);

    if (arg1[0] == '\0' || arg2[0] == '\0' || !is_number(arg2)) {
        send_to_char("Syntax: trust <char> <level>.\n\r", ch);
        return;
    }

    if (!(victim = get_char_room(ch, arg1))) {
        send_to_char("That player is not here.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("Not on superior or peer.\n\r", ch);
    }

    level = atoi(arg2);

    if (level < 1 || level > MAX_LEVEL) {
        char buf[MAX_STRING_LENGTH];

        sprintf(buf, "Trust within range 1 to %d.\n\r", MAX_LEVEL);
        send_to_char(buf, ch);
        return;
    }

    if (level > get_trust(ch)) {
        send_to_char("Limited to your trust.\n\r", ch);
        return;
    }

    victim->trust = level;
    return;
}

// do_restore: Command: do_restore - Performs the 'restore' command.
// do_restore: 指令：do_restore - 執行 'restore' 指令。
void do_restore(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "restore"))
        return;

    one_argument(argument, arg);
    if (get_trust(ch) < L_JUN || arg[0] == '\0') {
        victim = ch;
        victim->hit = victim->max_hit;
        victim->mana = victim->max_mana;
        victim->move = victim->max_move;
        update_pos(victim);
        act("$n has restored you.", ch, NULL, victim, TO_VICT);
        return;
    }

    /* Restore All feature coded by Katrina */
    if (!str_cmp(arg, "all")) {
        for (victim = char_list; victim; victim = victim->next) {
            if (victim->deleted)
                continue;
            victim->hit = victim->max_hit;
            victim->mana = victim->max_mana;
            victim->move = victim->max_move;
            update_pos(victim);
            act("$n has restored you.", ch, NULL, victim, TO_VICT);
        }
        send_to_char("Aww...how sweet :)...Done.\n\r", ch);
    } else {
        if (!(victim = get_char_world(ch, arg))) {
            send_to_char("They aren't here.\n\r", ch);
            return;
        }

        victim->hit = victim->max_hit;
        victim->mana = victim->max_mana;
        victim->move = victim->max_move;
        update_pos(victim);
        act("$n has restored you.", ch, NULL, victim, TO_VICT);
        send_to_char("Ok.\n\r", ch);
    }

    return;
}

// do_freeze: Command: do_freeze - Performs the 'freeze' command.
// do_freeze: 指令：do_freeze - 執行 'freeze' 指令。
void do_freeze(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    time_t t;

    rch = get_char(ch);

    if (!authorized(rch, "freeze"))
        return;

    argument = one_argument(argument, arg);
    argument = one_argument(argument, arg1);

    if (arg[0] == '\0' || arg[1] == '\0') {
        send_to_char("freeze char time\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim) || IS_SET(victim->act, PLR_HORSE)) {
        send_to_char("Not on NPC's or horse's.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("You failed.\n\r", ch);
        return;
    }

    if (!is_number(arg1)) {
        send_to_char("時間請用數字。\n\r", ch);
        return;
    }

    t = atoi(arg1) * 60;
    if (t == 0) {
        victim->pcdata->freezed = 0;
        REMOVE_BIT(victim->act, PLR_FREEZE);
        send_to_char("FREEZE removed.\n\r", ch);
        send_to_char("You can play again.\n\r", victim);
    } else {
        victim->pcdata->freezed = time(NULL) + t;
        SET_BIT(victim->act, PLR_FREEZE);
        send_to_char("FREEZE set.\n\r", ch);
        send_to_char("You can't do ANYthing!\n\r", victim);
    }

    save_char_obj(victim);

    return;
}

// do_log: Command: do_log - Performs the 'log' command.
// do_log: 指令：do_log - 執行 'log' 指令。
void do_log(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "log"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Log whom?\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "all")) {
        if (fLogAll) {
            fLogAll = FALSE;
            send_to_char("Log ALL off.\n\r", ch);
        } else {
            fLogAll = TRUE;
            send_to_char("Log ALL on.\n\r", ch);
        }
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if (IS_SET(victim->act, PLR_LOG)) {
        REMOVE_BIT(victim->act, PLR_LOG);
        send_to_char("LOG removed.\n\r", ch);
    } else {
        SET_BIT(victim->act, PLR_LOG);
        send_to_char("LOG set.\n\r", ch);
    }

    return;
}

// do_noemote: Command: do_noemote - Performs the 'noemote' command.
// do_noemote: 指令：do_noemote - 執行 'noemote' 指令。
void do_noemote(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "noemote"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Noemote whom?\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("You failed.\n\r", ch);
        return;
    }

    if (IS_SET(victim->act, PLR_NO_EMOTE)) {
        REMOVE_BIT(victim->act, PLR_NO_EMOTE);
        send_to_char("NO_EMOTE removed.\n\r", ch);
        send_to_char("You can emote again.\n\r", victim);
    } else {
        SET_BIT(victim->act, PLR_NO_EMOTE);
        send_to_char("You can't emote!\n\r", victim);
        send_to_char("NO_EMOTE set.\n\r", ch);
    }

    return;
}

// do_notell: Command: do_notell - Performs the 'notell' command.
// do_notell: 指令：do_notell - 執行 'notell' 指令。
void do_notell(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "notell"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Notell whom?", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("You failed.\n\r", ch);
        return;
    }

    if (IS_SET(victim->act, PLR_NO_TELL)) {
        REMOVE_BIT(victim->act, PLR_NO_TELL);
        send_to_char("NO_TELL removed.\n\r", ch);
        send_to_char("You can tell again.\n\r", victim);
    } else {
        SET_BIT(victim->act, PLR_NO_TELL);
        send_to_char("NO_TELL set.\n\r", ch);
        send_to_char("You can't tell!\n\r", victim);
    }

    return;
}

// do_silence: Command: do_silence - Performs the 'silence' command.
// do_silence: 指令：do_silence - 執行 'silence' 指令。
void do_silence(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "silence"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Silence whom?\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    if (get_trust(victim) >= get_trust(ch)) {
        send_to_char("You failed.\n\r", ch);
        return;
    }

    if (IS_SET(victim->act, PLR_SILENCE)) {
        REMOVE_BIT(victim->act, PLR_SILENCE);
        send_to_char("You can use channels again.\n\r", victim);
        send_to_char("SILENCE removed.\n\r", ch);
    } else {
        SET_BIT(victim->act, PLR_SILENCE);
        send_to_char("You can't use channels!\n\r", victim);
        send_to_char("SILENCE set.\n\r", ch);
    }

    return;
}

// do_peace: Command: do_peace - Performs the 'peace' command.
// do_peace: 指令：do_peace - 執行 'peace' 指令。
void do_peace(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "peace"))
        return;

    /* Yes, we are reusing rch.	 -Kahn */
    for (rch = ch->in_room->people; rch; rch = rch->next_in_room) {
        if (rch->fighting)
            stop_fighting(rch, TRUE);
    }

    send_to_char("Ok.\n\r", ch);
    return;
}

BAN_DATA *ban_free;
BAN_DATA *ban_list;

// do_ban: Command: do_ban - Performs the 'ban' command.
// do_ban: 指令：do_ban - 執行 'ban' 指令。
void do_ban(CHAR_DATA *ch, char *argument) {
    BAN_DATA *pban;
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];

    if (IS_NPC(ch))
        return;

    rch = get_char(ch);

    if (!authorized(rch, "ban"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        strcpy(buf, "Banned sites:\n\r");
        for (pban = ban_list; pban; pban = pban->next) {
            strcat(buf, pban->name);
            strcat(buf, "\n\r");
        }
        send_to_char(buf, ch);
        return;
    }

    for (pban = ban_list; pban; pban = pban->next) {
        if (!str_cmp(arg, pban->name)) {
            send_to_char("That site is already banned!\n\r", ch);
            return;
        }
    }

    if (!ban_free) {
        pban = alloc_mem(sizeof(*pban));
    } else {
        pban = ban_free;
        ban_free = ban_free->next;
    }

    pban->name = str_dup(arg);
    pban->next = ban_list;
    ban_list = pban;
    send_to_char("Ok.\n\r", ch);
    ban_update();
    return;
}

// do_allow: Command: do_allow - Performs the 'allow' command.
// do_allow: 指令：do_allow - 執行 'allow' 指令。
void do_allow(CHAR_DATA *ch, char *argument) {
    BAN_DATA *prev;
    BAN_DATA *curr;
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "allow"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Remove which site from the ban list?\n\r", ch);
        return;
    }

    prev = NULL;
    for (curr = ban_list; curr; prev = curr, curr = curr->next) {
        if (!str_cmp(arg, curr->name)) {
            if (!prev)
                ban_list = ban_list->next;
            else
                prev->next = curr->next;

            free_string(curr->name);
            curr->next = ban_free;
            ban_free = curr;
            send_to_char("Ok.\n\r", ch);
            ban_update();
            return;
        }
    }

    send_to_char("Site is not banned.\n\r", ch);
    return;
}

// do_wizlock: Command: do_wizlock - Performs the 'wizlock' command.
// do_wizlock: 指令：do_wizlock - 執行 'wizlock' 指令。
void do_wizlock(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    extern bool wizlock;

    rch = get_char(ch);

    if (!authorized(rch, "wizlock"))
        return;

    wizlock = !wizlock;

    if (wizlock)
        send_to_char("Game wizlocked.\n\r", ch);
    else
        send_to_char("Game un-wizlocked.\n\r", ch);

    return;
}

// do_slookup: Command: do_slookup - Performs the 'slookup' command.
// do_slookup: 指令：do_slookup - 執行 'slookup' 指令。
void do_slookup(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 3];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    rch = get_char(ch);

    if (!authorized(rch, "slookup"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0') {
        send_to_char("Slookup what?\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "all")) {
        buf1[0] = '\0';
        for (sn = 0; sn < MAX_SKILL; sn++) {
            if (!skill_table[sn].name)
                break;
            sprintf(buf, "Sn: %4d \tSkill/spell: [%s]\n\r", sn,
                    skill_table[sn].name);
            strcat(buf1, buf);
        }
        send_to_char(buf1, ch);
    } else {
        if (is_number(arg)) {
            sn = atoi(arg);
            if (sn >= 0 && sn < MAX_SKILL && skill_table[sn].name) {
                sprintf(buf, "Sn: %4d \tSkill/spell: [%s]\n\r", sn,
                        skill_table[sn].name);
                send_to_char(buf, ch);
                return;
            }
        }

        if ((sn = skill_lookup(arg)) < 0) {
            send_to_char("No such skill or spell.\n\r", ch);
            return;
        }

        sprintf(buf, "Sn: %4d \tSkill/spell: [%s]\n\r", sn,
                skill_table[sn].name);
        send_to_char(buf, ch);
    }

    return;
}

// do_sset: Command: do_sset - Performs the 'sset' command.
// do_sset: 指令：do_sset - 執行 'sset' 指令。
void do_sset(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    int value, value2;
    int sn;
    bool fAll;

    rch = get_char(ch);

    if (!authorized(rch, "sset"))
        return;

    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    one_argument(argument, arg3);

    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
        send_to_char("Syntax: sset <victim> <skill> <value>\n\r", ch);
        send_to_char("or:     sset <victim> all     <value>\n\r", ch);
        send_to_char("Skill being any skill or spell.\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg1))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    if (get_trust(ch) < L_SEN) {
        victim = ch;
    }

    if (ch->level <= victim->level && ch != victim) {
        send_to_char("You may not sset your peer nor your superior.\n\r", ch);
        return;
    }

    fAll = !str_cmp(arg2, "all");
    sn = 0;
    if (!fAll && (sn = skill_lookup(arg2)) < 0) {
        send_to_char("No such skill or spell.\n\r", ch);
        return;
    }

    /*
     * Snarf the value.
     */
    if (!is_number(arg3)) {
        send_to_char("Value must be numeric.\n\r", ch);
        return;
    }

    value = atoi(arg3);
    if (value < 0) //|| value > 100 )
    {
        send_to_char("Value range is 0 to 100.\n\r", ch);
        return;
    }

    if (fAll) {
        if (get_trust(ch) < L_DIR) {
            send_to_char("Only Directors may sset all.\n\r", ch);
            return;
        }
        for (sn = 0; sn < MAX_SKILL; sn++) {
            if (skill_table[sn].name) {
                if ((skill_eff_percent(victim, sn) > 0) &&
                    (get_trust(victim) < LEVEL_IMMORTAL)) {
                    //		if (
                    ////skill_table[sn].skill_level[victim->class] <= LEVEL_HERO
                    //+ 6 )
                    //                    skill_req_level(victim, sn) <=
                    //                    LEVEL_HERO + 6 )
                    value2 = value % 1000;
                    value -= value2;
                    value += skill_eff_percent(victim, sn);
                    victim->pcdata->learned[sn] = value;
                    //		else
                    //		    if (
                    ////skill_table[sn].skill_level[victim->class]
                    //                          skill_req_level( victim, sn )
                    //		<= get_trust( victim ) )
                    //		victim->pcdata->learned[sn] = 1;
                } else
                    victim->pcdata->learned[sn] = value;
            }
        }
    } else {
        victim->pcdata->learned[sn] = value;
    }

    return;
}

// do_mset: Command: do_mset - Performs the 'mset' command.
// do_mset: 指令：do_mset - 執行 'mset' 指令。
void do_mset(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    int value;
    int max;

    rch = get_char(ch);

    if (!authorized(rch, "mset"))
        return;

    smash_tilde(argument);
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    strcpy(arg3, argument);

    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
        send_to_char("Syntax: mset <victim> <field>  <value>\n\r", ch);
        send_to_char("or:     mset <victim> <string> <value>\n\r", ch);
        send_to_char("\n\r", ch);
        send_to_char("Field being one of:\n\r", ch);
        send_to_char("	 str int wis dex con class sex race level\n\r", ch);
        send_to_char("	 gold hp mana move practice align\n\r", ch);
        send_to_char("	 thirst drunk full security respect special\n\r",
                     ch);                                          /* OLC */
        send_to_char("	 hitroll damroll armor dodge pp\n\r", ch); /* OLC */
        send_to_char("	 merit merit_spent size \n\r", ch);        /* OLC */
        send_to_char("\n\r", ch);
        send_to_char("String being one of:\n\r", ch);
        send_to_char("	 name cname short long title spec\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(ch, arg1))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = is_number(arg3) ? atoi(arg3) : -1;

    /*
     * Set something.
     */
    if (!str_cmp(arg2, "playd")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }
        victim->played = value;
        return;
    }
    if (!str_cmp(arg2, "size")) {
        victim->mod_size = value;
        return;
    }
    if (!str_cmp(arg2, "str")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        max = 25;

        if (value < 3 || value > max) {
            sprintf(buf, "Strength range is 3 to %d.\n\r", max);
            send_to_char(buf, ch);
            return;
        }

        victim->perm_str = value;
        return;
    }

    if (!str_cmp(arg2, "int")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        max = 25;

        if (value < 3 || value > max) {
            sprintf(buf, "Intelligence range is 3 to %d.\n\r", max);
            send_to_char(buf, ch);
            return;
        }

        victim->perm_int = value;
        return;
    }

    if (!str_cmp(arg2, "wis")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        max = 25;
        if (value < 3 || value > max) {
            sprintf(buf, "Wisdom range is 3 to %d.\n\r", max);
            send_to_char(buf, ch);
            return;
        }

        victim->perm_wis = value;
        return;
    }

    if (!str_cmp(arg2, "dex")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        max = 25;

        if (value < 3 || value > max) {
            sprintf(buf, "Dexterity range is 3 to %d.\n\r", max);
            send_to_char(buf, ch);
            return;
        }

        victim->perm_dex = value;
        return;
    }

    if (!str_cmp(arg2, "con")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        max = 25;

        if (value < 3 || value > max) {
            sprintf(buf, "Constitution range is 3 to %d.\n\r", max);
            send_to_char(buf, ch);
            return;
        }

        victim->perm_con = value;
        return;
    }

    if (!str_cmp(arg2, "class")) {
        if (value < 0 || value >= MAX_CLASS) {
            char buf[MAX_STRING_LENGTH];

            sprintf(buf, "Class range is 0 to %d.\n", MAX_CLASS - 1);
            send_to_char(buf, ch);
            return;
        }
        victim->class = value;
        return;
    }

    if (!str_cmp(arg2, "sex")) {
        if (IS_AFFECTED(victim, AFF_CHANGE_SEX)) {
            send_to_char("This person is affect by change sex.\n\r", ch);
            send_to_char("Try again later.\n\r", ch);
            return;
        }

        if (value < 0 || value > 2) {
            send_to_char("Sex range is 0 to 2.\n\r", ch);
            return;
        }

        victim->sex = value;

        return;
    }

    if (!str_cmp(arg2, "race")) {
        OBJ_DATA *wield;
        OBJ_DATA *wield2;
        int race;

        if (IS_AFFECTED(victim, AFF_POLYMORPH)) {
            send_to_char("This person is affected by polymorph other.\n\r", ch);
            send_to_char("Try again later.\n\r", ch);
            return;
        }

        race = race_lookup(arg3);

        if (race < 0) {
            send_to_char("Invalid race.\n\r", ch);
            return;
        }

        if (!IS_SET(race_table[race].race_abilities, RACE_PC_AVAIL) &&
            get_trust(ch) < L_SEN) {
            send_to_char("You may not set a race not available to PC's.\n\r",
                         ch);
            return;
        }

        victim->race = race;

        if ((wield = get_eq_char(victim, WEAR_WIELD)) &&
            !IS_SET(race_table[victim->race].race_abilities,
                    RACE_WEAPON_WIELD)) {
            act("You drop $p.", victim, wield, NULL, TO_CHAR);
            act("$n drops $p.", victim, wield, NULL, TO_ROOM);
            obj_from_char(wield);
            // modified by jye 0950310
            if (IS_SET(wield->extra_flags, ITEM_PERSONAL) || IS_NPC(victim))
                obj_to_char(wield, victim);
            else
                obj_to_room(wield, victim->in_room);
        }

        if ((wield2 = get_eq_char(victim, WEAR_WIELD_2)) &&
            !IS_SET(race_table[victim->race].race_abilities,
                    RACE_WEAPON_WIELD)) {
            act("You drop $p.", victim, wield2, NULL, TO_CHAR);
            act("$n drops $p.", victim, wield2, NULL, TO_ROOM);
            obj_from_char(wield2);
            // modified by jye 0950310
            if (IS_SET(wield2->extra_flags, ITEM_PERSONAL) || IS_NPC(victim))
                obj_to_char(wield2, victim);
            else
                obj_to_room(wield2, victim->in_room);
        }

        if ((wield2 = get_eq_char(victim, WEAR_TWO_HAND)) &&
            !IS_SET(race_table[victim->race].race_abilities,
                    RACE_WEAPON_WIELD)) {
            act("You drop $p.", victim, wield2, NULL, TO_CHAR);
            act("$n drops $p.", victim, wield2, NULL, TO_ROOM);
            obj_from_char(wield2);
            // modified by jye 0950310
            if (IS_SET(wield2->extra_flags, ITEM_PERSONAL) || IS_NPC(victim))
                obj_to_char(wield2, victim);
            else
                obj_to_room(wield2, victim->in_room);
        }
        return;
    }

    if (!str_cmp(arg2, "level")) {
        if (!IS_NPC(victim)) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        if (value < 1 || value > 300) {
            send_to_char("Level range is 1 to 300.\n\r", ch);
            return;
        }
        victim->level = value;
        return;
    }

    if (!str_cmp(arg2, "gold")) {
        victim->gold = value;
        return;
    }

    if (!str_cmp(arg2, "hp")) {
        if (value < 1 || value > 100000) {
            send_to_char("Hp range is 1 to 100,000 hit points.\n\r", ch);
            return;
        }
        if (victim->fighting && value < 0) {
            send_to_char("You cannot set a fighting person's hp below 0.\n\r",
                         ch);
            return;
        }
        victim->max_hit = value;
        return;
    }

    if (!str_cmp(arg2, "mana")) {
        if (value < 1 || value > 100000) {
            send_to_char("Mana range is 1 to 100,000 mana points.\n\r", ch);
            return;
        }
        victim->max_mana = value;
        return;
    }

    if (!str_cmp(arg2, "move")) {
        if (value < 1 || value > 100000) {
            send_to_char("Move range is 1 to 100,000 move points.\n\r", ch);
            return;
        }
        victim->max_move = value;
        return;
    }

    if (!str_cmp(arg2, "practice")) {
        if (value < 0 || value > 1000) {
            send_to_char("Practice range is 0 to 1000 sessions.\n\r", ch);
            return;
        }
        victim->practice = value;
        return;
    }

    if (!str_cmp(arg2, "merit")) {
        if (value < 0) {
            send_to_char("merit should be positive\n\r", ch);
            return;
        }

        gain_merit(victim, value - victim->pcdata->merit);

        return;
    }

    if (!str_cmp(arg2, "merit_spent")) {
        if (value < 0) {
            send_to_char("merit spent should be positive\n\r", ch);
            return;
        }
        if (value > victim->pcdata->merit)
            value = victim->pcdata->merit;
        victim->pcdata->merit_spent = value;

        return;
    }

    if (!str_cmp(arg2, "align")) {
        if (value < -1000 || value > 1000) {
            send_to_char("Alignment range is -1000 to 1000.\n\r", ch);
            return;
        }
        victim->alignment = value;
        return;
    }

    if (!str_cmp(arg2, "respect")) {

        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        if (value < -1000 || value > 1000) {
            send_to_char("Respect range is -1000 to 1000.\n\r", ch);
            return;
        }
        victim->pcdata->respect = value;
        return;
    }

    if (!str_cmp(arg2, "damroll")) {

        if (!IS_NPC(victim) && get_trust(ch) < L_SEN) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        victim->damroll = value;
        return;
    }
    if (!str_cmp(arg2, "armor")) {

        if (!IS_NPC(victim) && get_trust(ch) < L_SEN) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        victim->armor = value;
        return;
    }
    if (!str_cmp(arg2, "dodge")) {

        if (!IS_NPC(victim) && get_trust(ch) < L_SEN) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        victim->dodge = value;
        return;
    }
    if (!str_cmp(arg2, "pp")) {

        if (!IS_NPC(victim) && get_trust(ch) < L_SEN) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        victim->pp = value;
        return;
    }
    if (!str_cmp(arg2, "hitroll")) {

        if (!IS_NPC(victim) && get_trust(ch) < L_SEN) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        victim->hitroll = value;
        return;
    }
    if (!str_cmp(arg2, "special")) {

        if (!IS_NPC(victim)) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        victim->special = value;
        return;
    }

    if (!str_cmp(arg2, "thirst")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        if ((value < 0 || value > 100) && get_trust(victim) < LEVEL_IMMORTAL) {
            send_to_char("Thirst range is 0 to 100.\n\r", ch);
            return;
        } else if (value < -1 || value > 100) {
            send_to_char("Thirst range is -1 to 100.\n\r", ch);
            return;
        }

        victim->pcdata->condition[COND_THIRST] = value;
        return;
    }

    if (!str_cmp(arg2, "drunk")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        if (value < 0 || value > 100) {
            send_to_char("Drunk range is 0 to 100.\n\r", ch);
            return;
        }

        victim->pcdata->condition[COND_DRUNK] = value;
        return;
    }

    if (!str_cmp(arg2, "full")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        if ((value < 0 || value > 100) && get_trust(victim) < LEVEL_IMMORTAL) {
            send_to_char("Full range is 0 to 100.\n\r", ch);
            return;
        } else if (value < -1 || value > 100) {
            send_to_char("Full range is -1 to 100.\n\r", ch);
            return;
        }

        victim->pcdata->condition[COND_FULL] = value;
        return;
    }

    if (!str_cmp(arg2, "name")) {
        if (!IS_NPC(victim)) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        if (longstring(ch, arg3))
            return;

        free_string(victim->name);
        victim->name = str_dup(arg3);
        return;
    }

    if (!str_cmp(arg2, "short")) {
        if (longstring(ch, arg3))
            return;

        free_string(victim->short_descr);
        victim->short_descr = str_dup(arg3);
        return;
    }

    if (!str_cmp(arg2, "long")) {
        if (longstring(ch, arg3))
            return;

        free_string(victim->long_descr);
        strcat(arg3, "\n\r");
        victim->long_descr = str_dup(arg3);
        return;
    }

    if (!str_cmp(arg2, "title")) {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        set_title(victim, arg3);
        return;
    }

    if (!str_cmp(arg2, "cname")) {
        set_cname(victim, arg3);
        return;
    }

    if (!str_cmp(arg2, "spec")) {
        if (!IS_NPC(victim)) {
            send_to_char("Not on PC's.\n\r", ch);
            return;
        }

        if ((victim->spec_fun = spec_lookup(arg3)) == 0) {
            send_to_char("No such spec fun.\n\r", ch);
            return;
        }

        return;
    }

    if (!str_cmp(arg2, "security")) /* OLC */
    {
        if (IS_NPC(victim)) {
            send_to_char("Not on NPC's.\n\r", ch);
            return;
        }

        if (value > ch->pcdata->security || value < 0) {
            if (ch->pcdata->security != 0) {
                sprintf(buf, "Valid security is 0-%d.\n\r",
                        ch->pcdata->security);
                send_to_char(buf, ch);
            } else {
                send_to_char("Valid security is 0 only.\n\r", ch);
            }
            return;
        }
        victim->pcdata->security = value;
        return;
    }

    /*
     * Generate usage message.
     */
    do_mset(ch, "");
    return;
}

// do_oset: Command: do_oset - Performs the 'oset' command.
// do_oset: 指令：do_oset - 執行 'oset' 指令。
void do_oset(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    int value;

    rch = get_char(ch);

    if (!authorized(rch, "oset"))
        return;

    smash_tilde(argument);
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    strcpy(arg3, argument);

    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
        send_to_char("Syntax: oset <object> <field>  <value>\n\r", ch);
        send_to_char("or:     oset <object> <string> <value>\n\r", ch);
        send_to_char("\n\r", ch);
        send_to_char("Field being one of:\n\r", ch);
        send_to_char("	value0 value1 value2 value3\n\r", ch);
        send_to_char("	exused exvalue0 exvalue1 extimer0 extimer1\n\r", ch);
        send_to_char("	extra wear level weight cost timer\n\r", ch);
        send_to_char("\n\r", ch);
        send_to_char("String being one of:\n\r", ch);
        send_to_char("	name short long ed \n\r", ch);
        return;
    }

    if (!(obj = get_obj_world(ch, arg1))) {
        send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);
        return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi(arg3);

    /*
     * Set something.
     */
    if (!str_cmp(arg2, "value0") || !str_cmp(arg2, "v0")) {
        obj->value[0] = value;
        return;
    }

    if (!str_cmp(arg2, "value1") || !str_cmp(arg2, "v1")) {
        obj->value[1] = value;
        return;
    }

    if (!str_cmp(arg2, "value2") || !str_cmp(arg2, "v2")) {
        obj->value[2] = value;
        return;
    }

    if (!str_cmp(arg2, "value3") || !str_cmp(arg2, "v3")) {
        obj->value[3] = value;
        return;
    }

    /* Added by Razgriz
     * set obj_extra_value
     */
    if (!str_cmp(arg2, "exused")) {
        if (value > 0)
            (obj->extra_value).used = TRUE;
        else
            (obj->extra_value).used = FALSE;
        return;
    }

    if (!str_cmp(arg2, "exvalue0") || !str_cmp(arg2, "ev0")) {
        (obj->extra_value).used = TRUE;
        (obj->extra_value).v[0] = value;
        return;
    }

    if (!str_cmp(arg2, "exvalue1") || !str_cmp(arg2, "ev1")) {
        (obj->extra_value).used = TRUE;
        (obj->extra_value).v[1] = value;
        return;
    }

    if (!str_cmp(arg2, "extimer0") || !str_cmp(arg2, "et0")) {
        (obj->extra_value).used = TRUE;
        (obj->extra_value).t[0] = value;
        return;
    }

    if (!str_cmp(arg2, "extimer1") || !str_cmp(arg2, "et1")) {
        (obj->extra_value).used = TRUE;
        (obj->extra_value).t[1] = value;
        return;
    }
    /* set obj_extra_value end */

    if (!str_cmp(arg2, "extra")) {
        obj->extra_flags = value;
        return;
    }

    if (!str_cmp(arg2, "add_extra")) {
        obj->extra_flags = obj->extra_flags + value;
        return;
    }

    if (!str_cmp(arg2, "remove_extra")) {
        if (obj->extra_flags >= value)
            obj->extra_flags = obj->extra_flags - value;
        return;
    }

    if (!str_cmp(arg2, "wear")) {
        obj->wear_flags = value;
        return;
    }

    if (!str_cmp(arg2, "level")) {
        obj->level = value;
        return;
    }

    if (!str_cmp(arg2, "weight")) {
        if (obj->carried_by != NULL && !IS_NPC(obj->carried_by)) {
            send_to_char(
                "You may not modify an item's weight while on a PC.\n\r", ch);
            return;
        }
        obj->weight = value;
        return;
    }

    if (!str_cmp(arg2, "cost")) {
        obj->cost = value;
        return;
    }

    if (!str_cmp(arg2, "timer")) {
        obj->timer = value;
        return;
    }

    if (!str_cmp(arg2, "name")) {
        if (longstring(ch, arg3))
            return;

        free_string(obj->name);
        obj->name = str_dup(arg3);
        return;
    }

    if (!str_cmp(arg2, "short")) {
        if (longstring(ch, arg3))
            return;

        free_string(obj->short_descr);
        obj->short_descr = str_dup(arg3);
        return;
    }

    if (!str_cmp(arg2, "long")) {
        if (longstring(ch, arg3))
            return;

        free_string(obj->description);
        obj->description = str_dup(arg3);
        return;
    }

    // 移除 ed 2022/11/29
    /*if ( !str_cmp( arg2, "ed" ) )
    {
            EXTRA_DESCR_DATA *ed;

            argument = one_argument( argument, arg3 );
            argument = one_argument( argument, arg4 );
            if ( arg4[0] == '\0' )
            {
                    send_to_char( "Syntax: oset <object> ed <keyword>
    <string>\n\r", ch ); return;
            }

            ed = new_extra_descr();

            ed->keyword = str_dup( arg3 );
            ed->description = str_dup( argument );
            ed->deleted = FALSE;
            ed->next = obj->extra_descr;
            obj->extra_descr = ed;
            return;
    }*/

    /*
     * Generate usage message.
     */
    do_oset(ch, "");
    return;
}

// do_rset: Command: do_rset - Performs the 'rset' command.
// do_rset: 指令：do_rset - 執行 'rset' 指令。
void do_rset(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *person;
    ROOM_INDEX_DATA *location;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    int value;

    rch = get_char(ch);

    if (!authorized(rch, "rset"))
        return;

    smash_tilde(argument);
    argument = one_argument(argument, arg1);
    argument = one_argument(argument, arg2);
    strcpy(arg3, argument);

    if (arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0') {
        send_to_char("Syntax: rset <location> <field> value\n\r", ch);
        send_to_char("\n\r", ch);
        send_to_char("Field being one of:\n\r", ch);
        send_to_char("	 flags sector\n\r", ch);
        return;
    }

    if (!(location = find_location(ch, arg1))) {
        send_to_char("No such location.\n\r", ch);
        return;
    }

    /*
     * Snarf the value.
     */
    if (!is_number(arg3)) {
        send_to_char("Value must be numeric.\n\r", ch);
        return;
    }
    value = atoi(arg3);

    for (person = location->people; person; person = person->next_in_room) {
        if (IS_NPC(person))
            continue;
        if (person != ch && person->level >= ch->level) {
            send_to_char("Your superior is in this room, no rsetting now.\n\r",
                         ch);
            return;
        }
    }

    /*
     * Set something.
     */
    if (!str_cmp(arg2, "flags")) {
        location->room_flags = value;
        return;
    }

    if (!str_cmp(arg2, "sector")) {
        location->sector_type = value;
        return;
    }

    /*
     * Generate usage message.
     */
    do_rset(ch, "");
    return;
}

/*
//modified by Chris with sort by remoteuser at 05/09/17.
// do_users: Command: do_users - Performs the 'users' command.
// do_users: 指令：do_users - 執行 'users' 指令。
void do_users( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA	    *rch;
    DESCRIPTOR_DATA *d;
    DESCRIPTOR_DATA *dtemp;
    DESCRIPTOR_DATA *dlist[500];
    CHAR_DATA *vch = NULL;
    char	    buf	 [ MAX_STRING_LENGTH * 8];
    char	    buf2 [ MAX_STRING_LENGTH ];
    char            arg [MAX_INPUT_LENGTH];
    int		    i,j,count;

    rch = get_char( ch );

    if ( !authorized( rch, "users" ) )
        return;

    one_argument(argument, arg);
    if( arg[0] != '\0' ) {
        vch = get_char_world( ch, arg );
        if( !vch ) {
            send_to_char("沒有這個玩家.\n\r", ch);
            return;
        }
        if( ! (d = vch->desc) ) {
            send_to_char("它不是玩家角色，或是沒有 descriptor.\n\r", ch);
            return;
        }
        sprintf( buf , "[%3d %2d] %s %s@%s\n\r",
                d->descriptor,
                d->connected,
                d->original	 ? d->original->name  :
                d->character ? d->character->name : "(none)",
                d->remoteuser, d->host );
        send_to_char(buf, ch);
        return;
    }

    count	= 0;
    buf[0]	= '\0';
    buf2[0]	= '\0';


    for ( d = descriptor_list; d; d = d->next )
    {
        if ( d->character && can_see( ch, d->character ) )
        {
            dlist[count]=d;
            count++;
        }
    }

    for (i=0 ;i < count; i++ )
    {
        for (j = i+1 ; j< count; j++)
        {
           if ( !ip_str_cmp( dlist[i]->host, dlist[j]->host) )
           {
              dtemp=dlist[i+1];
              dlist[i+1]=dlist[j];
              dlist[j]=dtemp;
              i++;
           }
        }
    }

    for( i = 0 ; i < count ; )
    {
        d=dlist[i];
        sprintf( buf + strlen( buf ), "%-42s", d->host);
        do
        {
            if( !d->original && !d->character)
            {
                sprintf( buf + strlen( buf ), " (none)");
            }
            else
            {
                sprintf( buf + strlen( buf ), " %2d %-12s",
                         d->original  ? d->original->level :
d->character->level, d->original  ? d->original->name  : d->character->name );
            }
            i++;
            d=dlist[i];
        }
        while( i < count && !ip_str_cmp(dlist[i]->host, dlist[i-1]->host) );

        sprintf( buf + strlen( buf ), "\n\r" );

    }

    sprintf( buf2, "%d user%s\n\r", count, count == 1 ? "" : "s" );
    strcat( buf, buf2 );
    send_to_char( buf, ch );
    return;
}
*/

// do_users: Command: do_users - Performs the 'users' command.
// do_users: 指令：do_users - 執行 'users' 指令。
void do_users(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    DESCRIPTOR_DATA *d;
    DESCRIPTOR_DATA **sort, *dtmp;
    CHAR_DATA *vch = NULL;
    char buf[MAX_STRING_LENGTH * 8];
    char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int count;
    int i, j, min, maxdesc = 0;

    rch = get_char(ch);

    if (!authorized(rch, "users"))
        return;

    one_argument(argument, arg);
    if (arg[0] != '\0' && str_cmp(arg, "-s")) {
        vch = get_char_world(ch, arg);
        if (!vch) {
            send_to_char("沒有這個玩家.\n\r", ch);
            return;
        }
        if (!(d = vch->desc)) {
            send_to_char("它不是玩家角色，或是沒有 descriptor.\n\r", ch);
            return;
        }
        sprintf(buf, "[%3d %2d] %s %s@%s\n\r", d->descriptor, d->connected,
                d->original    ? d->original->name
                : d->character ? d->character->name
                               : "(none)",
                d->remoteuser, d->host);
        send_to_char(buf, ch);
        return;
    }
    // Added by Razgriz 20050919
    else if (!str_cmp(arg, "-s")) {
        for (d = descriptor_list; d; d = d->next)
            maxdesc++;
        sort = (DESCRIPTOR_DATA **)malloc(maxdesc * sizeof(DESCRIPTOR_DATA *));
        for (i = 0, d = descriptor_list; i < maxdesc; i++, d = d->next)
            sort[i] = d;
        for (i = 0; i < maxdesc; i++) {
            min = i;
            for (j = i + 1; j < maxdesc; j++) {
                if (strcmp(sort[j]->host, sort[min]->host) <= 0)
                    min = j;
            }
            dtmp = sort[min];
            sort[min] = sort[i];
            sort[i] = dtmp;
        }
        count = 0;
        buf[0] = '\0';
        buf2[0] = '\0';
        for (i = 0; i < maxdesc; i++) {
            if (sort[i]->character && can_see(ch, sort[i]->character)) {
                count++;
                sprintf(buf + strlen(buf), "[%3d %2d] %s %s@%s\n\r",
                        sort[i]->descriptor, sort[i]->connected,
                        sort[i]->original    ? sort[i]->original->name
                        : sort[i]->character ? sort[i]->character->name
                                             : "(none)",
                        sort[i]->remoteuser, sort[i]->host);
            }
        }
        sprintf(buf2, "%d user%s\n\r", count, count == 1 ? "" : "s");
        strcat(buf, buf2);
        send_to_char(buf, ch);
        free(sort);
        return;
    }

    count = 0;
    buf[0] = '\0';
    buf2[0] = '\0';
    for (d = descriptor_list; d; d = d->next) {
        if (d->character && can_see(ch, d->character)) {
            count++;
            sprintf(buf + strlen(buf), "[%3d %2d] %s %s@%s\n\r", d->descriptor,
                    d->connected,
                    d->original    ? d->original->name
                    : d->character ? d->character->name
                                   : "(none)",
                    d->remoteuser, d->host);
        }
    }

    sprintf(buf2, "%d user%s\n\r", count, count == 1 ? "" : "s");
    strcat(buf, buf2);
    send_to_char(buf, ch);
    return;
}

// do_forcer: Command: do_forcer - Performs the 'forcer' command.
// do_forcer: 指令：do_forcer - 執行 'forcer' 指令。
void do_forcer(CHAR_DATA *ch, char *argument) {
    ROOM_INDEX_DATA *room = NULL;
    char arg[MAX_INPUT_LENGTH];
    // int	       trust;
    // int	       cmd;
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "forcer"))
        return;

    argument = one_argument(argument, arg);

    if (arg[0] == '\0' || argument[0] == '\0') {
        send_to_char("Force which room to do what?\n\r", ch);
        return;
    }

    if (!str_cmp(arg, "here"))
        room = ch->in_room;
    else if (!is_number(arg)) {
        send_to_char("Force which room to do what?\n\r", ch);
        return;
    } else {
        room = get_room_index(atoi(arg));
        if (!room) {
            send_to_char("No such location.\n\r", ch);
            ;
            return;
        }
    }
    /*
     * Look for command in command table.
     */

    rinterpret(room, ch, argument);

    send_to_char("Ok.\n\r", ch);
    return;
}

/* Added by Razgriz 20050916
 * force object do something
 */
// do_forceo: Command: do_forceo - Performs the 'forceo' command.
// do_forceo: 指令：do_forceo - 執行 'forceo' 指令。
void do_forceo(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj = NULL;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;

    rch = get_char(ch);
    if (!authorized(rch, "forceo"))
        return;

    argument = one_argument(argument, arg);

    if (arg[0] == '\0' || argument[0] == '\0') {
        send_to_char("Force which object to do what?\n\r", ch);
        return;
    }

    obj = get_obj_here(ch, arg);

    if (!obj) {
        send_to_char("No such object in here.\n\r", ch);
        return;
    }

    /*
     * Look for command in command table.
     */

    ointerpret(obj, ch, argument);

    send_to_char("Ok.\n\r", ch);
    return;
}

// do_addexp: Command: do_addexp - Performs the 'addexp' command.
// do_addexp: 指令：do_addexp - 執行 'addexp' 指令。
void do_addexp(CHAR_DATA *ch, char *argument) {
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *target;
    int value;
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "addexp"))
        return;

    argument = one_argument(argument, arg);

    if ((target = get_char_world(ch, arg))) {
        one_argument(argument, arg);
        if (is_number(arg)) {
            value = atoi(arg);
            if (value > 0)
                sprintf(arg, "你得到 %d 點經驗。\n\r", value);
            else {
                if (target->exp < abs(value))
                    return;
                sprintf(arg, "你失去 %d 點經驗。\n\r", abs(value));
            }
            send_to_char(arg, target);
            gain_exp(target, value);
            return;
        }
    } else {
        send_to_char("They are not here.\n\r", ch);
        return;
    }
    send_to_char("addexp [victim] [exp]\n\r", ch);
    return;
}

/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
// do_force: Command: do_force - Performs the 'force' command.
// do_force: 指令：do_force - 執行 'force' 指令。
void do_force(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    char arg[MAX_INPUT_LENGTH];
    int trust;
    int cmd;

    rch = get_char(ch);

    if (!authorized(rch, "force"))
        return;

    argument = one_argument(argument, arg);

    if (arg[0] == '\0' || argument[0] == '\0') {
        send_to_char("Force whom to do what?\n\r", ch);
        return;
    }

    /*
     * Look for command in command table.
     */
    trust = get_trust(ch);
    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++) {
        if (argument[0] == cmd_table[cmd].name[0] &&
            !str_prefix(argument, cmd_table[cmd].name) &&
            (cmd_table[cmd].level > trust)) {
            send_to_char("You can't even do that yourself!\n\r", ch);
            return;
        }
    }

    if (!str_cmp(arg, "all")) {
        CHAR_DATA *vch;

        for (vch = char_list; vch; vch = vch->next) {
            if (vch->deleted)
                continue;

            if (!IS_NPC(vch) && get_trust(vch) < get_trust(ch)) {
                act("$n forces you to '$t'.", ch, argument, vch, TO_VICT);
                interpret(vch, argument);
            }
        }
    } else {
        CHAR_DATA *victim;

        if (!(victim = get_char_world(ch, arg))) {
            send_to_char("They aren't here.\n\r", ch);
            return;
        }

        if (victim == ch) {
            send_to_char("Aye aye, right away!\n\r", ch);
            return;
        }

        if (get_trust(victim) >= get_trust(ch)) {
            send_to_char("Do it yourself!\n\r", ch);
            return;
        }

        act("$n forces you to '$t'.", ch, argument, victim, TO_VICT);
        interpret(victim, argument);
    }

    send_to_char("Ok.\n\r", ch);
    return;
}

/*
 * New routines by Dionysos.
 */
// do_invis: Command: do_invis - Performs the 'invis' command.
// do_invis: 指令：do_invis - 執行 'invis' 指令。
void do_invis(CHAR_DATA *ch, char *argument) {
    int level;
    char arg[MAX_INPUT_LENGTH];
    if (IS_NPC(ch))
        return;

    if (!authorized(ch, "wizinvis"))
        return;

    one_argument(argument, arg);
    if (arg[0] == '\0')
        level = get_trust(ch);
    else
        level = URANGE(1, atoi(arg), get_trust(ch));

    if (IS_SET(ch->act, PLR_WIZINVIS)) {
        QMARK *qm;
        if (!(qm = is_qmark(ch, "wizinvis")) || arg[0] == '\0' || level < 0) {
            REMOVE_BIT(ch->act, PLR_WIZINVIS);
            do_del_qmark(ch, "self wizinvis");
            send_to_char("You slowly fade back into existence.\n\r", ch);
            act("$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM);
        } else {
            qm->v0 = level;
            send_to_char("You changed your wizinvis level.\n\r", ch);
        }
    } else {
        SET_BIT(ch->act, PLR_WIZINVIS);
        sprintf(arg, " self wizinvis -1 %d 0 0 0 1", level);
        do_add_qmark(ch, arg);
        send_to_char("You slowly vanish into thin air.\n\r", ch);
        act("$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM);
    }

    return;
}

// do_holylight: Command: do_holylight - Performs the 'holylight' command.
// do_holylight: 指令：do_holylight - 執行 'holylight' 指令。
void do_holylight(CHAR_DATA *ch, char *argument) {
    if (IS_NPC(ch))
        return;

    if (!authorized(ch, "holylight"))
        return;

    if (IS_SET(ch->act, PLR_HOLYLIGHT)) {
        REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("Holy light mode off.\n\r", ch);
    } else {
        SET_BIT(ch->act, PLR_HOLYLIGHT);
        send_to_char("Holy light mode on.\n\r", ch);
    }

    return;
}

/* Wizify and Wizbit sent in by M. B. King */

// do_wizify: Command: do_wizify - Performs the 'wizify' command.
// do_wizify: 指令：do_wizify - 執行 'wizify' 指令。
void do_wizify(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "wizify"))
        return;

    one_argument(argument, arg1);

    if (arg1[0] == '\0') {
        send_to_char("Syntax: wizify <name>\n\r", ch);
        return;
    }
    if (!(victim = get_char_world(ch, arg1))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }
    if (IS_NPC(victim)) {
        send_to_char("Not on mobs.\n\r", ch);
        return;
    }

    if (!IS_SET(victim->act, PLR_WIZBIT)) {
        SET_BIT(victim->act, PLR_WIZBIT);
        act("$N wizified.", ch, NULL, victim, TO_CHAR);
        act("$n has wizified you!", ch, NULL, victim, TO_VICT);
    } else {
        REMOVE_BIT(victim->act, PLR_WIZBIT);
        act("$N dewizzed.", ch, NULL, victim, TO_CHAR);
        act("$n has dewizzed you!", ch, NULL, victim, TO_VICT);
    }

    do_save(victim, "");
    return;
}

/* Idea from Talen of Vego's do_where command */

// do_owhere: Command: do_owhere - Performs the 'owhere' command.
// do_owhere: 指令：do_owhere - 執行 'owhere' 指令。
void do_owhere(CHAR_DATA *ch, char *argument) {
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH * 8];
    char arg[MAX_INPUT_LENGTH];
    int count = 0;
    bool found = FALSE;
    int obj_counter = 0;

    rch = get_char(ch);

    if (!authorized(rch, "owhere"))
        return;

    one_argument(argument, arg);

    if (arg[0] == '\0') {
        send_to_char("Syntax:	owhere <object>.\n\r", ch);
        return;
    } else {
        buf1[0] = '\0';
        for (obj = object_list; obj; obj = obj->next) {
            if (!can_see_obj(ch, obj) || !is_name(arg, obj->name))
                continue;
            if (obj->deleted)
                continue;

            found = TRUE;

            for (in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj)
                ;

            ++obj_counter;
            if (in_obj->carried_by) {
                if (!can_see(ch, in_obj->carried_by))
                    continue;
                ++count;
                sprintf(buf, "[%2d] %s, <%s> carried by %s at [%4d].\n\r",
                        obj_counter, obj->short_descr, obj->name,
                        PERS(in_obj->carried_by, ch),
                        in_obj->carried_by->in_room->vnum);
            } else {
                ++count;
                sprintf(buf, "[%2d] %s, <%s> in %s at [%4d].\n\r", obj_counter,
                        obj->short_descr, obj->name,
                        (!in_obj->in_room) ? "某處" : in_obj->in_room->name,
                        (!in_obj->in_room) ? 0 : in_obj->in_room->vnum);
            }

            buf[0] = UPPER(buf[0]);
            strcat(buf1, buf);
            // if( count >= 50 ) { // 2022/10/14
            if (count >= 80) {
                send_to_char(buf1, ch);
                buf1[0] = '\0';
                count = 0;
            }
        }

        send_to_char(buf1, ch);
    }

    if (!found)
        send_to_char("Nothing like that in hell, earth, or heaven.\n\r", ch);

    return;
}

void do_numlock(CHAR_DATA *ch, char *argument) /*By Globi*/
{
    CHAR_DATA *rch;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    extern int numlock;
    int temp;

    rch = get_char(ch);

    if (!authorized(rch, "numlock"))
        return;

    one_argument(argument, arg1);

    temp = atoi(arg1);

    if (arg1[0] == '\0') /* Prints out the current value */
    {
        sprintf(buf, "Current numlock setting is:  %d.\n\r", numlock);
        send_to_char(buf, ch);
        return;
    }

    if ((temp < 0) || (temp > LEVEL_HERO)) {
        sprintf(buf, "Level must be between 0 and %d.\n\r", LEVEL_HERO);
        send_to_char(buf, ch);
        return;
    }

    numlock = temp; /* Only set numlock if arg supplied and within range */

    if (numlock != 0) {
        sprintf(buf, "Game numlocked to levels %d and below.\n\r", numlock);
        send_to_char(buf, ch);
    } else
        send_to_char("Game now open to all levels.\n\r", ch);

    return;
}

// do_numloc: Command: do_numloc - Performs the 'numloc' command.
// do_numloc: 指令：do_numloc - 執行 'numloc' 指令。
void do_numloc(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "numlock"))
        return;
    send_to_char("if you want to NUMLOCK, spell it out.\n\r", ch);
}

// do_newloc: Command: do_newloc - Performs the 'newloc' command.
// do_newloc: 指令：do_newloc - 執行 'newloc' 指令。
void do_newloc(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;

    rch = get_char(ch);

    if (!authorized(rch, "newlock"))
        return;
    send_to_char("if you want to NEWLOCK, spell it out.\n\r", ch);
}

// do_newlock: Command: do_newlock - Performs the 'newlock' command.
// do_newlock: 指令：do_newlock - 執行 'newlock' 指令。
void do_newlock(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    extern int numlock;
    char buf[MAX_STRING_LENGTH];

    rch = get_char(ch);

    if (!authorized(rch, "newlock"))
        return;

    if (numlock != 0 && get_trust(ch) < L_SEN) {
        send_to_char("You may not change the current numlock setting\n\r", ch);
        sprintf(buf, "Game numlocked to levels %d and below.\n\r", numlock);
        send_to_char(buf, ch);
        return;
    }

    if (numlock != 0) {
        sprintf(buf, "Game numlocked to levels %d and below.\n\r", numlock);
        send_to_char(buf, ch);
        send_to_char("Changing to: ", ch);
    }

    numlock = 1;
    send_to_char("Game locked to new characters.\n\r", ch);
    return;
}

// do_sstim: Command: do_sstim - Performs the 'sstim' command.
// do_sstim: 指令：do_sstim - 執行 'sstim' 指令。
void do_sstim(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch = get_char(ch);

    if (!authorized(rch, "sstime"))
        return;

    send_to_char("if you want to SSTIME, spell it out.\n\r", ch);
}

// do_sstime: Command: do_sstime - Performs the 'sstime' command.
// do_sstime: 指令：do_sstime - 執行 'sstime' 指令。
void do_sstime(CHAR_DATA *ch, char *argument) {
    CHAR_DATA *rch;
    extern time_t down_time;
    extern time_t warning1;
    extern time_t warning0;
    extern time_t warning2;
    extern bool Reboot;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    int number;

    rch = get_char(ch);

    if (!authorized(rch, "sstime"))
        return;

    smash_tilde(argument);
    argument = one_argument(argument, arg1);

    if (!str_cmp(arg1, "reboot")) {
        Reboot = !Reboot;
        sprintf(buf, "Reboot is %s.\n\r", Reboot ? "on" : "off");
        send_to_char(buf, ch);
        return;
    }

    number = atoi(arg1);

    if (arg1[0] == '\0' || !is_number(arg1) || number < 0) {
        send_to_char("Syntax: sstime <value>/reboot\n\r", ch);
        send_to_char("\n\r", ch);
        send_to_char("Value is number of minutes till reboot/shutdown.", ch);
        send_to_char("	 Or 0 to turn off.\n\r", ch);
        send_to_char("Reboot will toggle reboot on or off.\n\r", ch);
        send_to_char("\n\r", ch);
        if (down_time > 0) {
            sprintf(buf, "1st warning:	 %d minutes (%d seconds).\n\r",
                    UMAX(((int)warning1 - (int)current_time) / 60, 0),
                    UMAX(((int)warning1 - (int)current_time), 0));
            send_to_char(buf, ch);
            sprintf(buf, "2nd warning:	 %d minutes (%d seconds).\n\r",
                    UMAX(((int)warning2 - (int)current_time) / 60, 0),
                    UMAX(((int)warning2 - (int)current_time), 0));
            send_to_char(buf, ch);
            sprintf(buf, "%s%d minutes (%d seconds).\n\r",
                    Reboot ? "Reboot:	    " : "Shutdown:     ",
                    UMAX(((int)down_time - (int)current_time) / 60, 0),
                    UMAX(((int)down_time - (int)current_time), 0));
            send_to_char(buf, ch);
        } else
            send_to_char("Automatic reboot/shutdown:  off.\n\r", ch);
        return;
    }

    /* Set something */

    if (number > 0) {
        down_time = current_time + (number * 60);
        if (number < 6) {
            warning2 = down_time - 150;
            warning1 = warning2 - 75;
            warning0 = warning1 - 300;
        } else {
            warning2 = down_time - 150;
            warning1 = warning2 - 150;
            warning0 = warning1 - 1800;
        }
        sprintf(buf, "%s will be in %d minutes (%d seconds).\n\r",
                Reboot ? "Reboot" : "Shutdown",
                ((int)down_time - (int)current_time) / 60,
                ((int)down_time - (int)current_time));
        send_to_char(buf, ch);
        sprintf(buf, "0th Warning will be in %d minutes (%d seconds).\n\r",
                ((int)warning0 - (int)current_time) / 60,
                ((int)warning0 - (int)current_time));
        send_to_char(buf, ch);
        sprintf(buf, "1st Warning will be in %d minutes (%d seconds).\n\r",
                ((int)warning1 - (int)current_time) / 60,
                ((int)warning1 - (int)current_time));
        send_to_char(buf, ch);
        sprintf(buf, "2nd Warning will be in %d minutes (%d seconds).\n\r",
                ((int)warning2 - (int)current_time) / 60,
                ((int)warning2 - (int)current_time));
        send_to_char(buf, ch);
    } else {
        down_time = 0;
        sprintf(buf, "Auto%s is now off.\n\r", Reboot ? "reboot" : "shutdown");
        send_to_char(buf, ch);
    }

    return;
}

/*
 * Modifications contributed by
 * Canth (phule@xs4all.nl)
 * Maniac (v942346@si.hhs.nl)
 * Vego (v942429@si.hhs.nl)
 */
// do_imtlset: Command: do_imtlset - Performs the 'imtlset' command.
// do_imtlset: 指令：do_imtlset - 執行 'imtlset' 指令。
void do_imtlset(CHAR_DATA *ch, char *argument) {

    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    bool fAll = FALSE;
    int cmd;
    int col = 0;

    rch = get_char(ch);

    if (!authorized(rch, "imtlset"))
        return;

    argument = one_argument(argument, arg1);

    if (arg1[0] == '\0') {
        send_to_char("Syntax: imtlset <victim> +|- <immortal skill>\n\r", ch);
        send_to_char("or:     imtlset <victim> +|- all\n\r", ch);
        send_to_char("or:     imtlset <victim>\n\r", ch);
        return;
    }

    if (!(victim = get_char_world(rch, arg1))) {
        send_to_char("They aren't here.\n\r", ch);
        return;
    }

    if (IS_NPC(victim)) {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    if (get_trust(rch) <= get_trust(victim) && rch != victim) {
        send_to_char("You may not imtlset your peer nor your superior.\n\r",
                     ch);
        return;
    }

    if (argument[0] == '+' || argument[0] == '-') {
        buf[0] = '\0';
        smash_tilde(argument);

        argument = one_argument(argument, arg1);

        if (!str_cmp(argument, "all")) {
            if (get_trust(ch) < L_DIR) {
                send_to_char("Only Directors can imtlset +|- all\n\r", ch);
                return;
            }
            fAll = TRUE;
        }

        if (arg1[0] == '+') {
            if (!fAll) {
                if (victim->pcdata->immskll)
                    sprintf(buf, "%s", victim->pcdata->immskll);

                if (is_name(argument, victim->pcdata->immskll)) {
                    send_to_char("That skill has already been set.\n\r", ch);
                    return;
                }
            }

            for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++) {
                if (cmd_table[cmd].level > get_trust(rch))
                    continue;
                if (fAll) {
                    if (cmd_table[cmd].level <= victim->level &&
                        cmd_table[cmd].level >= LEVEL_HERO &&
                        str_infix(cmd_table[cmd].name, "reboo sla shutdow :")) {
                        strcat(buf, cmd_table[cmd].name);
                        strcat(buf, " ");
                    }
                } else /* Add only one skill */
                {
                    if (!str_cmp(argument, cmd_table[cmd].name))
                        break;
                }
            }

            if (!fAll) {
                if (cmd_table[cmd].name[0] == '\0' ||
                    is_name(argument, "reboo sla shutdow :")) {
                    send_to_char("That is not an immskill.\n\r", ch);
                    return;
                }

                strcat(buf, argument);
                strcat(buf, " "); /* This line is really not needed but makes
                                     pfile look nice - Kahn */
            }
        } else /* arg1[0] == '-' */
        {
            if (fAll) {
                free_string(victim->pcdata->immskll);
                victim->pcdata->immskll = str_dup("");
                send_to_char("All Immskills have been deleted.\n\r", ch);
                return;
            } else /* Remove one skill */
            {
                char *buf2;
                char buf3[MAX_STRING_LENGTH];
                char arg3[MAX_INPUT_LENGTH];
                char arg2[MAX_INPUT_LENGTH];

                argument = one_argument(argument, arg2);

                sprintf(buf3, "%s", victim->pcdata->immskll);
                buf2 = buf3;

                if (!is_name(arg2, victim->pcdata->immskll)) {
                    send_to_char("That Immskill is not set.\n\r", ch);
                    return;
                }

                while (buf2[0] != '\0') {
                    buf2 = one_argument(buf2, arg3);
                    if (str_cmp(arg3, arg2)) {
                        strcat(buf, arg3);
                        strcat(buf, " ");
                    }
                }
            }
        }

        free_string(victim->pcdata->immskll);
        victim->pcdata->immskll = str_dup(buf);
    }

    sprintf(buf, "Immortal skills set for %s:\n\r", victim->name);
    send_to_char(buf, ch);
    buf1[0] = '\0';
    for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++) {
        if (cmd_table[cmd].level < LEVEL_HERO ||
            !is_name(cmd_table[cmd].name, victim->pcdata->immskll))
            continue;

        sprintf(buf, "%-10s", cmd_table[cmd].name);
        strcat(buf1, buf);
        if (++col % 8 == 0)
            strcat(buf1, "\n\r");
    }

    if (col % 8 != 0)
        strcat(buf1, "\n\r");
    send_to_char(buf1, ch);

    return;
}
