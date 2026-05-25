
/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,    *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                     *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael    *
 *  Chastain, Michael Quan, and Mitchell Tse.          *
 *                     *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.         *
 *                     *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and    *
 *  Mitchell Tse.                *
 *                     *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.     *
 *                     *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in    *
 *  'license.txt', as well as the Envy license in 'license.nvy'.     *
 *  In particular, you may not remove either of these copyright notices.   *
 *                     *
 *  Much time and thought has gone into this software and you are    *
 *  benefitting.  We hope that you share your changes too.  What goes    *
 *  around, comes around.              *
 ***************************************************************************/
//*****************************************************************
//*  PROGRAM ID  : clan.c                                         *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940613 jye                                    *
//*  PURPOSE     :         修改有指定clan_channel時               *
//*              :             看不到自己clan的頻道之bug          *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940617 jye                                    *
//*  PURPOSE     :         修改clan who別幫時本身lv4              *
//*              :             以上會看見exp gold之bug            *
//*****************************************************************
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include "ansi.h"
#include "merc.h"
#include "olc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * A clan structure
 */

extern const struct social_type social_table[];

CLAN_DATA *clan_free = NULL, *clan_list = NULL;

void free_clan_data(CLAN_DATA *pClan) {
    free_string(pClan->name);
    free_string(pClan->whoname);
    free_string(pClan->filename);
    free_string(pClan->leader);
    free_string(pClan->founder);
    free_string(pClan->desc);
    free_string(pClan->feud);
    free_string(pClan->ally);

    pClan->next = clan_free;
    clan_free   = pClan;
}

void do_clanrecall(CHAR_DATA *ch, char *argument);

char *bank_desc(long long int bank) {
    if (bank <= 1000000)
        return "貧困度日";
    else if (bank <= 10000000)
        return "普普通通";
    else if (bank <= 50000000)
        return "小有財力";
    else if (bank <= 100000000)
        return "財大勢大";
    else if (bank <= 300000000)
        return "富可敵國";
    else if (bank <= 600000000)
        return "雄霸一方";
    else if (bank <= 1000000000)
        return "比爾蓋天";
    else
        return "多到滿出來了。";
}

struct clan_class {
    char *name;
    int   exp;
};

const struct clan_class cc[] =
    {
        {"形單影隻", 10000     },
        {"三五好友", 20000     },
        {"同好會",    30000     },
        {"小型家族", 40000     },
        {"大型家族", 50000     },
        {"集會",       60000     },
        {"結社",       70000     },
        {"宗族",       90000     },
        {"氏族",       110000    },
        {"組織",       140000    },
        {"地下組織", 170000    },
        {"神秘組織", 210000    },
        {"堅實組織", 250000    },
        {"小山頭",    300000    },
        {"山頭",       360000    },
        {"大山頭",    430000    },
        {"協會",       510000    }, //  start jump
        {"聯合",       650000    },
        {"會社",       800000    },
        {"幫會",       970000    },
        {"強大幫會", 1200000   },
        {"第一大幫", 1400000   },
        {"強盛領地", 1800000   },
        {"勢如諸侯", 2200000   },
        {"勢如小國", 3000000   },
        {"勢如大國", 3800000   },
        {"邦國",       5000000   },
        {"強大邦國", 6600000   },
        {"各邦盟主", 8000000   },
        {"各邦共主", 10000000  },
        {"各國盟主", 12000000  },
        {"各國共主", 14000000  },
        {"各族盟主", 16000000  },
        {"各族共主", 18000000  },
        {"大陸盟主", 20000000  },
        {"大陸共主", 40000000  },
        {"世界盟主", 60000000  },
        {"世界共主", 100000000 },
        {"號令天下", 150000000 },
        {"莫敢不從", 220000000 },
        {"誰與爭鋒", 350000000 },
        {"混沌幻世", 1000000000},
        {"破表了",    2147480000},
        {"",             0         }
};

// clanact modified by jye 6.13.2005
void clanact(const char *format, CHAR_DATA *ch, const void *arg1,
             const void *arg2, int type) {
    OBJ_DATA          *obj1 = (OBJ_DATA *)arg1;
    DESCRIPTOR_DATA   *d;
    OBJ_DATA          *obj2 = (OBJ_DATA *)arg2;
    CHAR_DATA         *to;
    CHAR_DATA         *vch       = (CHAR_DATA *)arg2;
    static char *const he_she[]  = {"它", "他", "她"};
    static char *const him_her[] = {"它", "他", "她"};
    static char *const his_her[] = {"它的", "他的", "她的"};
    const char        *str;
    const char        *i;
    char              *point;
    char               buf[MAX_STRING_LENGTH];
    char               buf1[MAX_STRING_LENGTH];
    char               fname[MAX_INPUT_LENGTH];
    bool               fChinese;

    /*
     * Discard null and zero-length messages.
     */
    if (!format || format[0] == '\0')
        return;

    to = ch->in_room->people;
    if (type == TO_VICT) {
        if (!vch) {
            bug("Act: null vch with TO_VICT.", 0);
            sprintf(buf1, "Bad act string:  %s", format);
            bug(buf1, 0);
            return;
        }

        if (!vch->in_room) // amenda 6/2/01 for debug purpose
        {
            bug("clan.c, act:vch not in any room", 0);
            bug(vch->name, 0);
            return;
        }
        to = vch->in_room->people;
    }

    for (d = descriptor_list; d; d = d->next) {
        to = (d->original) ? d->original : d->character;

        if (!to)
            continue;
        // if( (to && ( to->clan != ch->clan && to->clan_channel != ch->clan->whoname
        //  && !str_cmp( to->clan_channel, "all" ) ) ) ) continue;
        if (to->clan != ch->clan && !to->clan_channel)
            continue;
        if (to->clan_channel) {
            if (to->clan_channel != ch->clan->whoname && to->clan != ch->clan) {
                if (str_cmp(to->clan_channel, "all"))
                    continue;
            }
        }

        sprintf(buf, "%s【%s】", HIY, ch->clan->whoname);
        point = buf;
        point += strlen(buf);
        /**point++ = '';
         *point++ = '[';
         *point++ = '1';
         *point++ = ';';
         *point++ = '3';
         *point++ = '3';
         *point++ = 'm';*/
        str      = format;
        fChinese = FALSE;
        while (*str != '\0') {
            if (*str != '$' || fChinese) {
                // if ( fChinese )
                //     fChinese=FALSE;
                // else
                fChinese = is_chinese_char(*str, fChinese);
                // if ( *str>='\x80' && *str<='\xff' )
                // fChinese=TRUE;
                *point++ = *str++;
                continue;
            }

            ++str;

            if (!arg2 && *str >= 'A' && *str <= 'Z') {
                bug("Act: missing arg2 for code %d.", *str);
                sprintf(buf1, "Bad act string:  %s", format);
                bug(buf1, 0);
                i = " <@@@> ";
            } else {
                switch (*str) {
                default:
                    bug("Act: bad code %d.", *str);
                    sprintf(buf1, "Bad act string:  %s", format);
                    bug(buf1, 0);
                    i = " <@@@> ";
                    break;
                /* Thx alex for 't' idea */
                case 't':
                    i = (char *)arg1;
                    break;
                case 'T':
                    i = (char *)arg2;
                    break;
                case 'n':
                    i = PERS(ch, to);
                    break;
                case 'N':
                    i = PERS(vch, to);
                    break;
                case 'e':
                    i = he_she[URANGE(0, ch->sex, 2)];
                    break;
                case 'E':
                    i = he_she[URANGE(0, vch->sex, 2)];
                    break;
                case 'm':
                    i = him_her[URANGE(0, ch->sex, 2)];
                    break;
                case 'M':
                    i = him_her[URANGE(0, vch->sex, 2)];
                    break;
                case 's':
                    i = his_her[URANGE(0, ch->sex, 2)];
                    break;
                case 'S':
                    i = his_her[URANGE(0, vch->sex, 2)];
                    break;

                case 'p':
                    i = can_see_obj(to, obj1)
                            ? (strcpy(name_buf, obj1->short_descr))
                            : "不明物品";
                    break;

                case 'P':
                    i = can_see_obj(to, obj2)
                            ? (strcpy(name_buf, obj2->short_descr))
                            : "不明物品";
                    break;

                case 'd':
                    if (!arg2 || ((char *)arg2)[0] == '\0') {
                        i = "門";
                    } else {
                        one_argument((char *)arg2, fname);
                        i = fname;
                    }
                    break;
                case '$':
                    i = "$";
                    break;
                }
            }

            ++str;
            while ((*point = *i) != '\0')
                ++point, ++i;

            *point++ = '';
            *point++ = '[';
            *point++ = '1';
            *point++ = ';';
            *point++ = '3';
            *point++ = '3';
            *point++ = 'm';
        }

        *point++ = '';
        *point++ = '[';
        *point++ = '0';
        *point++ = 'm';
        *point++ = '\n';
        *point++ = '\r';
        buf[0]   = UPPER(buf[0]);
        if (to->desc)
            write_to_buffer(to->desc, buf, point - buf);
        /*
        if (MOBtrigger)
            mprog_act_trigger( buf, to, ch, obj1, vch );
        */
    }

    // MOBtrigger = TRUE;
    return;
}
bool clan_social(CHAR_DATA *ch, char *command, char *argument) {
    CHAR_DATA *victim;
    char       arg[MAX_INPUT_LENGTH];
    int        cmd;
    bool       found;

    found = FALSE;
    for (cmd = 0; social_table[cmd].name[0] != '\0'; cmd++) {
        if (command[0] == social_table[cmd].name[0] && !str_prefix(command, social_table[cmd].name)) {
            found = TRUE;
            break;
        }
    }

    if (!found)
        return FALSE;

    if (!IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE)) {
        send_to_char("你無法使用任何表情指令！\n\r", ch);
        return TRUE;
    }

    switch (ch->position) {
    case POS_DEAD:
        send_to_char("靜靜躺著吧，你已經死了.\n\r", ch);
        return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
        send_to_char("你的狀況太糟了.\n\r", ch);
        return TRUE;

    case POS_STUNNED:
        send_to_char("你頭昏眼花.\n\r", ch);
        return TRUE;

    case POS_SLEEPING:
        /*
         * I just know this is the path to a 12" 'if' statement.  :(
         * But two players asked for it already!  -- Furey
         */
        if (!str_cmp(social_table[cmd].name, "snore"))
            break;
        send_to_char("你是在作夢嗎？\n\r", ch);
        return TRUE;
    }

    one_argument(argument, arg);
    victim = NULL;
    if (arg[0] == '\0') {
        // clanact( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
        clanact(social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM);
    } else if (!(victim = get_char_world(ch, arg))) {
        send_to_char("這裡沒有這個人.\n\r", ch);
    } else if (victim == ch) {
        // clanact( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
        clanact(social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM);
    } else if (!get_char_room(ch, arg) && can_see(ch, victim)) {
        if (!IS_NPC(victim)) {
            ROOM_INDEX_DATA *original;
            /*
            char      *ldbase      = "From far away, ";
            char       ldmsg [ MAX_STRING_LENGTH ];
            */

            original = ch->in_room;
            /*
            char_from_room( ch );
            char_to_room( ch, victim->in_room );
            */
            clanact(social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT);
            /*
            char_from_room( ch );
            char_to_room( ch, original );
            strcpy( ldmsg, ldbase );
            strcat( ldmsg, social_table[cmd].char_found );
            act( ldmsg,           ch, NULL, victim, TO_CHAR    );

            strcpy( ldmsg, ldbase );
            strcat( ldmsg, social_table[cmd].vict_found );
            act( ldmsg,           ch, NULL, victim, TO_VICT    );

            char_from_room( ch );
            char_to_room( ch, original );
            */
        } else {
            send_to_char("這裡沒有這個人.\n\r", ch);
        }
    } else {
        // clanact( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
        // clanact( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );
        clanact(social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT);
    }

    return TRUE;
}
int clan_members(CLAN_DATA *pClan) {
    int i, m = 0;
    for (i = 0; i < CLAN_LEVEL - 1; ++i)
        m += pClan->member[i];

    return m + 1;
}

CLAN_DATA *new_clan_data(void) {
    CLAN_DATA *pClan = NULL;

    if (!clan_free) {
        pClan = alloc_mem(sizeof(*pClan));

    } else {
        pClan     = clan_free;
        clan_free = pClan->next;
    }

    pClan->dirty    = 0;
    pClan->filename = NULL;

    return pClan;
}

void clan_update(void) {
    FILE      *fp;
    CLAN_DATA *cn;
    int        i;
    // char buf [MAX_STRING_LENGTH];

    fclose(fpReserve);

    for (cn = clan_list; cn; cn = cn->next) {
        // sprintf(buf, "%s%s", CLAN_DIR, cn->filename );
        if (!(fp = fopen(cn->filename, "w"))) {
            bug("Clan_update:  fopen of CLAN_FILE failed", 0);
            return;
        }
        fprintf(fp, "#CLAN\n");
        fprintf(fp, "Nm  %s~\n", cn->name);
        fprintf(fp, "Wnm %s~\n", cn->whoname);
        fprintf(fp, "Fndr  %s~\n", cn->founder);
        fprintf(fp, "Ldr %s~\n", cn->leader);
        fprintf(fp, "Desc  %s~\n", cn->desc ? cn->desc : "");
        fprintf(fp, "Bank  %lld\n", cn->bank);
        fprintf(fp, "Mmbr      %d", cn->member[0]);
        for (i = 1; i < CLAN_LEVEL - 1; ++i)
            fprintf(fp, " %d", cn->member[i]);
        fprintf(fp, "\n");
        fprintf(fp, "CRecl %d\n", cn->crecall);
        fprintf(fp, "Ally  %s~\n", cn->ally);
        fprintf(fp, "Feud   %s~\n", cn->feud);
        fprintf(fp, "War   %d %d\n", cn->warwin, cn->warlose);
        fprintf(fp, "CScore  %d\n", cn->score);
        fprintf(fp, "CClass  %d\n", cn->cclass);
        fprintf(fp, "HIRE  %d\n", cn->hire);
        fprintf(fp, "Construct %d\n", cn->construct);
        fprintf(fp, "CObj  %d", cn->cobj[0]);
        for (i = 1; i < CLAN_LEVEL; ++i)
            fprintf(fp, " %d", cn->cobj[i]);
        fprintf(fp, "\n");
        fprintf(fp, "Killed  %d", cn->kill[0]);
        for (i = 1; i < CLAN_KILL; ++i)
            fprintf(fp, " %d", cn->kill[i]);
        fprintf(fp, "\n");
        fprintf(fp, "End\n");
        fprintf(fp, "#End\n");
        fclose(fp);
    }

    if (!(fp = fopen(CLAN_FILE, "w"))) {
        bug("Clan_update:  fopen of CLAN_FILE failed", 0);
        return;
    }
    for (cn = clan_list; cn; cn = cn->next) {
        fprintf(fp, "%s\n", cn->filename);
    }
    fprintf(fp, "$~\n");
    fclose(fp);
    fpReserve = fopen(NULL_FILE, "r");
    return;
}

/* code original from Greed Mud */
bool fread_clan(CLAN_DATA *pClan, FILE *fp) {
    const char *word;
    bool        fMatch = FALSE;
    int         stat;

    for (;;) {
        word   = feof(fp) ? "End" : fread_word(fp, &stat);
        fMatch = FALSE;

        switch (UPPER(word[0])) {
        case '*':
            fMatch = TRUE;
            fread_to_eol(fp);
            break;

        case 'A':
            if (!str_cmp(word, "Ally")) {
                fMatch = TRUE;
                if (pClan->ally)
                    free_string(pClan->ally);
                pClan->ally = fread_string(fp, &stat);
            }
            break;

        case 'B':
            if (!str_cmp(word, "Bank")) {
                fMatch      = TRUE;
                pClan->bank = fread_number(fp, &stat);
            }
            break;

        case 'C':
            if (!str_cmp(word, "CRecl")) {
                pClan->crecall = fread_number(fp, &stat);
                fMatch         = TRUE;
            } else if (!str_cmp(word, "CScore")) {
                fMatch       = TRUE;
                pClan->score = fread_number(fp, &stat);
            } else if (!str_cmp(word, "CClass")) {
                fMatch        = TRUE;
                pClan->cclass = fread_number(fp, &stat);
            } else if (!str_cmp(word, "CObj")) {
                int i;
                for (i = 0; i < CLAN_LEVEL; ++i) {
                    pClan->cobj[i] = fread_number(fp, &stat);
                    fMatch         = TRUE;
                }
            } else if (!str_cmp(word, "Construct")) {
                pClan->construct = fread_number(fp, &stat);
                fMatch           = TRUE;
            }
            break;

        case 'D':
            if (!str_cmp(word, "Desc")) {
                fMatch = TRUE;
                if (pClan->desc)
                    free_string(pClan->desc);
                pClan->desc = fread_string(fp, &stat);
            }
            break;

        case 'E':
            if (!str_cmp(word, "End"))
                return TRUE;
            break;

        case 'F':
            if (!str_cmp(word, "Feud")) {
                fMatch = TRUE;
                if (pClan->feud)
                    free_string(pClan->feud);
                pClan->feud = fread_string(fp, &stat);
            } else if (!str_cmp(word, "Fndr")) {
                fMatch = TRUE;
                if (pClan->founder)
                    free_string(pClan->founder);
                pClan->founder = fread_string(fp, &stat);
            }
            break;

        case 'H':
            if (!str_cmp(word, "Hire")) {
                fMatch      = TRUE;
                pClan->hire = fread_number(fp, &stat);
            }
            break;

        case 'K':
            if (!str_cmp(word, "Killed")) {
                int i;
                for (i = 0; i < CLAN_KILL; ++i) {
                    pClan->kill[i] = fread_number(fp, &stat);
                    fMatch         = TRUE;
                }
            }
            break;

        case 'L':
            if (!str_cmp(word, "Ldr")) {
                if (pClan->leader)
                    free_string(pClan->leader);
                pClan->leader = fread_string(fp, &stat);
                fMatch        = TRUE;
            }
            break;

        case 'M':
            if (!str_cmp(word, "Mmbr")) {
                int i;
                fMatch = TRUE;
                for (i = 0; i < CLAN_LEVEL - 1; ++i) {
                    pClan->member[i] = fread_number(fp, &stat);
                }
            }
            break;

        case 'N':
            if (!str_cmp(word, "Nm")) {
                if (pClan->name)
                    free_string(pClan->name);
                pClan->name = fread_string(fp, &stat);
                // bug( pClan->name, 0 );
                fMatch = TRUE;
            }
            break;

        case 'W':
            if (!str_cmp(word, "War")) {
                pClan->warwin  = fread_number(fp, &stat);
                pClan->warlose = fread_number(fp, &stat);
                fMatch         = TRUE;
            } else if (!str_cmp(word, "Wnm")) {
                fMatch = TRUE;
                if (pClan->whoname)
                    free_string(pClan->whoname);
                pClan->whoname = fread_string(fp, &stat);
            }
            break;
        }

        if (!fMatch) {
            bugf("fread_clan: unknown key [%s].", word);
        }
    }
    return FALSE;
}

void old_load_clan(void) {
    CLAN_DATA *cn;
    FILE      *fp;

    if (!(fp = fopen("CLAN.TXT", "r")))
        return;

    for (;;) {
        char letter;
        int  stat;

        do {
            letter = getc(fp);
            if (feof(fp)) {
                fclose(fp);
                return;
            }
        } while (isspace(letter));
        ungetc(letter, fp);

        if (clan_free == NULL) {
            cn = alloc_mem(sizeof(*cn));
        } else {
            cn        = clan_free;
            clan_free = clan_free->next;
        }

        cn->name      = fread_string(fp, &stat);
        cn->founder   = fread_string(fp, &stat);
        cn->leader    = fread_string(fp, &stat);
        cn->desc      = fread_string(fp, &stat);
        cn->bank      = fread_number(fp, &stat);
        cn->member[0] = fread_number(fp, &stat);
        if (fread_letter(fp) != '|') {
            bug("clan_load error", 0);
            return;
        }

        cn->next  = clan_list;
        clan_list = cn;
    }
}

bool load_clan_file(char *fn) {
    FILE      *fp;
    CLAN_DATA *pClan = NULL;
    char       clanfile[MAX_STRING_LENGTH];
    int        stat;

    sprintf(clanfile, "%s", fn);
    if (!(fp = fopen(clanfile, "r"))) {
        bugf("clan file not found : %s", clanfile);
        return FALSE;
    }

    pClan           = new_clan_data();
    pClan->filename = str_dup(clanfile);

    for (; !feof(fp);) {
        char *word;
        int   letter;

        letter = fread_letter(fp);

        if (letter == '*') {
            fread_to_eol(fp);
            continue;
        }
        if (letter != '#') {
            bug("Load_clan_file: # not found.", 0);
            free_clan_data(pClan);
            break;
        }

        word = fread_word(fp, &stat);
        if (!word) {
            bug("load_clan_file, no word!", 0);
            return FALSE;
        }

        if (!str_cmp(word, "CLAN")) {
            fread_clan(pClan, fp);

            pClan->next = clan_list;
            clan_list   = pClan;

            break;
        } else if (!str_cmp(word, "END"))
            break;
        else {
            bugf("Load_clan_file: bad section: %s.", word);
            free_clan_data(pClan);
            break;
        }
    }
    fclose(fp);

    return TRUE;
}

void load_clans(void) {
    FILE *fp;
    char  fn[MAX_INPUT_LENGTH];
    char *fname;
    int   stat;

    /*if ( !( fp = fopen( "CLAN.TXT", "r" ) ) )
  return;

  old_load_clan();
  return;*/

    if (!(fp = fopen(CLAN_LIST, "r"))) {
        bug("clan list not found!", 0);
        return;
    }
    for (;;) {
        if (feof(fp))
            break;
        fname = fread_word(fp, &stat);
        strcpy(fn, fname);

        if (fn[0] == '$')
            break;
        bugf("Clan [%s] loaded.", fn);

        if (!load_clan_file(fn)) {
            bugf("cannot load clan file : %s", fn);
            return;
        }
    }
    fclose(fp);

    clan_war = alloc_mem(sizeof(*clan_war));
    return;
}

void clan_title(CHAR_DATA *ch, char *title) {
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch)) {
        bug("Set_title: NPC.", 0);
        return;
    }

    buf[0] = '\0';

    strcpy(buf, title);

    free_string(ch->pcdata->clantitle);
    ch->pcdata->clantitle = str_dup(buf);
    return;
}

void do_clanchatemote(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];
    sprintf(buf, "chat* %s", argument);
    do_clan(ch, buf);
    return;
}

void do_clanchat(CHAR_DATA *ch, char *argument) {
    char buf[MAX_INPUT_LENGTH];
    sprintf(buf, "chat %s", argument);
    do_clan(ch, buf);
    return;
}

void do_clan(CHAR_DATA *ch, char *argument) {
    DESCRIPTOR_DATA *d      = descriptor_list;
    CHAR_DATA       *victim = NULL;
    CHAR_DATA       *vch;
    CLAN_DATA       *cn    = clan_list;
    CLAN_DATA       *vclan = NULL;
    char             arg[MAX_INPUT_LENGTH];
    char             buf[MAX_STRING_LENGTH * 2];
    char             buf1[MAX_STRING_LENGTH * 6];
    char             no_ansi[MAX_INPUT_LENGTH];
    char             clan_name[40];
    int              level   = 0;
    int              members = 0;
    int              num     = 0;
    CHAR_DATA       *wch[500];
    char const *class[500];
    int i    = 0;
    int rnum = 0;
    int mnum = 0;
    int onum = 0;

    if (IS_NPC(ch) || (ch->desc && ch->desc->original))
        return;

    argument = one_argument(argument, arg);
    if (arg[0] == '\0')
        strcpy(arg, "who");

    if (!str_cmp(arg, "setup")) {
        char arg2[MAX_INPUT_LENGTH];
        char arg3[MAX_INPUT_LENGTH];
        argument = one_argument(argument, arg2);
        argument = one_argument(argument, arg3);

        if (!ch->in_room || ch->in_room->vnum != 11000) {
            send_to_char("目前幫會的創造需要先經由Imm 同意喔！\n\r", ch);
            return;
        }
        if (arg2[0] == '\0' || arg3[0] == '\0') {
            send_to_char("格式： clan setup 完整幫名 簡稱（二到四字）\n\r", ch);
            return;
        }
        for (cn = clan_list; cn; cn = cn->next)
            if (!str_cmp(cn->founder, ch->name)) {
                send_to_char("你已經創過幫了！\n\r", ch);
                return;
            }
        if (ch->bank < 8000000) {
            send_to_char("要成立一個幫會是很花錢的！你的存款不夠\n\r", ch);
            return;
        }
        if (ch->pcdata->respect < 2 || ch->level < 38) {
            send_to_char("無名小子也想開宗立派？\n\r", ch);
            return;
        }
        if (ch->clan) {
            send_to_char("你已加入別的幫派。\n\r", ch);
            return;
        }
        if (strlen(arg2) > 31 || strlen(arg3) > 11) {
            send_to_char(YEL "幫派的名字太長了，請取一個短一點的。" NOR "\n\r", ch);
            return;
        }
        for (cn = clan_list; cn; cn = cn->next)
            if (!str_cmp(cn->name, arg2) || !str_cmp(cn->whoname, arg3)) {
                send_to_char("對不起，這個幫派已經存在，請另取名稱。\n\r", ch);
                return;
            }

        ch->bank -= 8000000;
        ch->pcdata->respect += 5;
        ch->pcdata->clanlevel = 5;
        clan_title(ch, "幫主");
        send_to_char("因為開宗立派，你的威望增加了！", ch);
        if (clan_free) {
            cn        = clan_free;
            clan_free = clan_free->next;
        } else
            cn = new_clan_data();
        smash_tilde(arg2);
        smash_tilde(arg3);
        strcpy(buf, arg2);
        cn->name = str_dup(buf);
        strcpy(buf, arg3);
        cn->whoname = str_dup(buf);
        strcpy(buf, ch->name);
        cn->leader  = str_dup(buf);
        cn->founder = str_dup(buf);
        sprintf(buf, "%s%s.clan", CLAN_DIR, ch->name);
        cn->filename  = str_dup(buf);
        cn->bank      = 1000000;
        cn->member[0] = 0;
        cn->member[1] = 0;
        cn->member[2] = 0;
        cn->member[3] = 0;
        cn->member[4] = 1;
        cn->next      = clan_list;
        clan_list     = cn;

        ch->clan = cn;

        sprintf(buf, "\n\r" HIY "一股新的勢力" NOR " %s, " HIY "由英明的" NOR " %s " HIY "所領導，慢慢崛起了！" NOR "\n\r",
                cn->name, ch->name);
        send_to_all_char(buf);
        clan_update();
        do_save(ch, "");
        return;
    } else if (!str_cmp(arg, "transfer")) {
        if (IS_NPC(ch) || !ch->clan) {
            return;
        }
        one_argument(argument, arg);
        if (arg[0] == '\0' || !(victim = get_char_room(ch, arg)) || IS_NPC(victim)) {
            send_to_char("你要傳位給誰？\n\r", ch);
            return;
        }
        // strcpy(clan_name, ch->clan->name);

        if (!victim->clan || victim->clan != ch->clan) {
            send_to_char("不能傳位給幫外之人！\n\r", ch);
            return;
        }
        if (victim->level < 38) {
            send_to_char("他的能力不足以接任幫主一職。\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel < 5) {
            send_to_char("別鬧了，你根本不是幫主。\n\r", ch);
            return;
        }
        if (ch == victim) {
            send_to_char("這樣有什麼意義？\n\r", ch);
            return;
        }
        free_string(ch->clan->leader);
        ch->clan->leader = str_dup(victim->name);
        ch->clan->score -= ch->clan->cclass * 5000;
        ch->pcdata->clanlevel     = 4;
        victim->pcdata->clanlevel = 5;
        ch->pcdata->respect -= 2;
        victim->pcdata->respect += 2;
        clan_title(ch, "前幫主");
        clan_title(victim, "幫主");

        sprintf(buf, "\n\r%s%s(%s)%s宣佈：本人將%s %s %s幫主之位傳給 %s%s %s！%s\n\r\n\r",
                HIY,
                ch->cname,
                ch->name,
                HIY,
                NOR,
                ch->clan->name,
                HIY,
                NOR,
                victim->name,
                HIY,
                NOR);
        send_to_all_char(buf);
        send_to_char("你的威望下降了！\n\r", ch);
        send_to_char("肩負起一幫重任，你的威望上升了！\n\r", victim);

    } else if (!str_cmp(arg, "join")) {
        if (ch->clan) {
            send_to_char("你已經加入別的幫派了。\n\r", ch);
            return;
        }
        one_argument(argument, arg);
        if (!str_cmp(arg, "cancel")) {
            if (!ch->clan_join) {
                send_to_char("你到現在為止還沒有加入幫派的念頭。\n\r", ch);
                return;
            }
            sprintf(buf, "你打消加入%s的念頭！\n\r", ch->clan_join);
            send_to_char(buf, ch);
            /*sprintf(buf, "%s【%s公告】%s%s打消加入貴幫的念頭！%s\n\r",
                HIY,
                ch->clan_join,
                ch->short_descr,
                HIY,
                NOR);
                for ( d = descriptor_list; d; d = d->next )
                {
                  if( d->connected != CON_PLAYING ) continue;
                  victim = (d->character) ? d->character : d->original;
                  if( !victim ) continue;
                  if( victim->clan->whoname != ch->clan_join && !victim->clan_channel ) continue;
                  if( victim->clan_channel )
                  {
                    if ( victim->clan_channel != ch->clan_join )
                    {
                      if ( str_cmp( victim->clan_channel, "all" ) ) continue;
                    }
                  }
                    send_to_char(buf, victim);
                }*/
            ch->clan_join = NULL;
            return;
        }
        sprintf(clan_name, "%s", arg);
        for (cn = clan_list; cn; cn = cn->next) {
            if (!str_cmp(clan_name, cn->leader)) {
                ch->clan_join = cn->whoname;
                sprintf(buf, "你希望加入的幫派是：%s！\n\r", ch->clan_join);
                send_to_char(buf, ch);
                /*sprintf(buf, "%s【%s公告】%s%s希望加入貴幫，請幫中元老招收！%s\n\r",
                  HIY,
                  ch->clan_join,
                  ch->short_descr,
                  HIY,
                  NOR);
                  for ( d = descriptor_list; d; d = d->next )
                  {
                    if( d->connected != CON_PLAYING ) continue;
                    victim = (d->character) ? d->character : d->original;
                    if( !victim ) continue;
                    if( victim->clan->whoname != ch->clan_join && !victim->clan_channel ) continue;
                    if( victim->clan_channel )
                    {
                      if ( victim->clan_channel != ch->clan_join )
                      {
                        if ( str_cmp( victim->clan_channel, "all" ) ) continue;
                      }
                    }
                      send_to_char(buf, victim);
                  }*/
                break;
            }
        }
        if (!ch->clan_join) {
            send_to_char("對不起，找不到你要找的幫派。\n\r", ch);
            return;
        }
        return;
    } else if (!str_cmp(arg, "recall")) {
        do_clanrecall(ch, "");
        return;
    } else if (!str_cmp(arg, "goto")) {
        ROOM_INDEX_DATA *pRoom;
        if (!ch->clan)
            return;
        if (ch->pcdata->clanlevel < 3) {
            send_to_char("你在幫中的地位不夠。\n\r", ch);
            return;
        }
        one_argument(argument, arg);
        if (!(pRoom = get_room_index(atoi(arg)))) {
            send_to_char("沒有這個地方。\n\r", ch);
            return;
        }
        if (!IS_PPLBUILDER(ch, pRoom->area) || !IS_PPLBUILDER(ch, ch->in_room->area)) {
            send_to_char("你無法移動到那裡去。\n\r", ch);
            return;
        }
        act("$n的身影突然從你面前消失。\n\r", ch, NULL, NULL, TO_ROOM);
        char_from_room(ch);
        char_to_room(ch, pRoom);
        act("$n突然出現在這裡。\n\r", ch, NULL, NULL, TO_ROOM);
        do_look(ch, "auto");
        return;
    } else if (!str_cmp(arg, "chat")) {
        if (!(ch->clan))
            return;
        one_argument(argument, arg);
        if (arg[0] == '\0') {
            send_to_char("你要說什麼？\n\r", ch);
            return;
        }
        if (strlen(arg) > 100) {
            send_to_char("輸入過長.\n\r", ch);
            return;
        }
        // strcpy(clan_name, ch->clan->name);
        sprintf(buf, "%s【%s】%s%s閒聊著「%s」%s\n\r",
                HIY,
                ch->clan->whoname,
                ch->short_descr,
                // ch->name,
                HIY,
                argument,
                NOR);
        strcpy(buf1, "[*****]  CC: ");
        strcat(buf1, buf);
        log_string(buf1);

        for (d = descriptor_list; d; d = d->next) {
            if (d->connected != CON_PLAYING)
                continue;
            //      victim = (d->original) ? d->original : d->character;
            victim = (d->character) ? d->character : d->original;
            if (!victim)
                continue; //&& !( victim->clan && victim->clan_channel ) ) continue;
            if (victim->clan != ch->clan && !victim->clan_channel)
                continue;
            if (victim->clan_channel) {
                if (victim->clan_channel != ch->clan->whoname && victim->clan != ch->clan) {
                    if (str_cmp(victim->clan_channel, "all"))
                        continue;
                }
            }
            send_to_char(buf, victim);
        }
        return;
    } else if (!str_cmp(arg, "chat*")) {
        if (!(ch->clan))
            return;
        argument = one_argument(argument, arg);
        if (!arg || arg[0] == '\0')
            return;

        //    sprintf(buf,"[%s][%s]\n\r", command, argument);
        // send_to_char(buf, ch);
        clan_social(ch, arg, argument);
        return;
    } else if (!str_cmp(arg, "accept")) {
        if (!ch->clan)
            return;
        one_argument(argument, arg);
        if (arg[0] == '\0' || !(victim = get_char_room(ch, arg)) || IS_NPC(ch)) {
            send_to_char("這個人不在這裡啊？\n\r", ch);
            return;
        }
        if (victim->clan) {
            send_to_char("很可惜，你慢了一步，他(她)已加入其他幫派了。\n\r", ch);
            return;
        }
        if (!victim->clan_join) {
            send_to_char("很可惜，他(她)還沒有選擇要加入的幫派。\n\r", ch);
            return;
        }
        if (ch->clan->whoname != victim->clan_join) {
            send_to_char("很可惜，他(她)沒有加入你幫派的打算。\n\r", ch);
            return;
        }
        if (IS_NPC(victim) || ch->pcdata->clanlevel < 3) {
            send_to_char("你的份量不夠。\n\r", ch);
            return;
        }
        if (victim->level < 10) {
            send_to_char("你的幫派不會想要這種小鬼的。\n\r", ch);
            return;
        }
        if (victim->gold < 30000) {
            send_to_char("他的身上沒有帶足夠的金幣。\n\r", ch);
            send_to_char("你的身上沒有足夠的金幣繳交會費。\n\r", victim);
            return;
        }
        if (ch->clan->bank < 100000) {
            send_to_char("幫中的帳戶沒有足夠的金幣繳交吸收費。\n\r", ch);
            send_to_char("這個幫派太窮了，養不起你。\n\r", victim);
            return;
        }
        victim->gold -= 30000;
        ch->clan->bank -= 100000;
        if (victim->level <= 20)
            ch->clan->score += victim->level * 10;
        else
            ch->clan->score += UMIN(victim->level * (victim->level + victim->level / 3), 1000);
        ch->clan->member[0] += 1;
        victim->pcdata->respect += 2;
        victim->pcdata->clanstatistics = 0;
        victim->pcdata->clanexp        = 0;
        victim->pcdata->clanlevel      = 0;
        sprintf(buf, "因為加入 %s, 你的威望上升了！\n\r",
                ch->clan->name);
        send_to_char(buf, victim);
        victim->clan = ch->clan;
        clan_title(victim, "幫眾");
        sprintf(buf, "\n\r" HIY "%s(%s)" HIY "由%s" HIY "引領加入%s" HIY "，使得%s" HIY "聲勢大振！" NOR "\n\r\n\r",
                victim->cname, victim->name, ch->cname, ch->clan->name, ch->clan->name);
        send_to_all_char(buf);
    } else if (!str_cmp(arg, "retrieve")) {
        if (!ch->clan)
            return;
        one_argument(argument, arg);
        if (arg[0] == '\0' || !(victim = get_char_world(ch, arg)) || ch == victim || IS_NPC(victim)) {
            send_to_char("這個人不在這裡啊？\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel < 3) {
            send_to_char("你沒有足夠的權力！\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel <= victim->pcdata->clanlevel) {
            send_to_char("你不能驅逐你的上司！\n\r", ch);
            return;
        }
        if (ch->clan != victim->clan) {
            send_to_char("你們不是同一個幫會的。\n\r", ch);
            return;
        }
        if (ch->clan->bank < 200000) {
            send_to_char("幫派的帳戶沒有足夠的金幣付遣散費。\n\r", ch);
            return;
        }
        if (!victim->clan || IS_NPC(victim)) {
            send_to_char("他(她)根本沒有加入任何幫派，何退之有？\n\r", ch);
            return;
        }
        ch->clan->bank -= 200000;
        ch->clan->score -= 1000;
        victim->pcdata->respect -= 3;
        sprintf(buf, "因為被踢出 %s, $s威望下降了！\n\r",
                victim->clan->name);
        send_to_char(buf, victim);
        victim->clan = NULL;
        ch->clan->member[0] -= 1;
        sprintf(buf, "\n\r%s" HIY "清理門戶，將" NOR "%s" HIY "由" NOR "%s" HIY "開除！" NOR "\n\r\n\r",
                ch->cname, victim->cname, ch->clan->name);
        send_to_all_char(buf);
    } else if (!str_cmp(arg, "leave")) {
        if (!ch->clan)
            return;
        if (ch->pcdata->clanlevel > 4) {
            send_to_char("世上豈有幫主退幫之理？\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel > 2) {
            send_to_char("你身居要職，不能任意退幫。\n\r", ch);
            return;
        }
        if (ch->gold < 100000) {
            send_to_char("你的身上沒有足夠的金幣支付退會費。\n\r", ch);
            return;
        }
        if (!ch->clan) {
            send_to_char("你根本沒有加入任何幫派，何退之有？\n\r", ch);
            return;
        }
        ch->gold -= 100000;
        ch->pcdata->respect -= 2;
        sprintf(buf, "因為退出 %s, 你的威望下降了！\n\r",
                ch->clan->name);
        send_to_char(buf, ch);
        sprintf(buf, "\n\r" HIY "道不同不相為謀，" NOR "%s(%s)" HIY "毅然地退出" NOR "%s" HIY "！" NOR "\n\r\n\r",
                ch->cname, ch->name, ch->clan->name);
        send_to_all_char(buf);
        ch->clan->score -= 1000;
        ch->clan->member[0] -= 1;
        ch->clan = NULL;
    } else if (!str_cmp(arg, "all")) {
        int clv;
        int lv;
        int count = 0;
        sprintf(buf1, "[ " HIC "線上所有組織成員列表" NOR " ]\n\r");
        strcat(buf1, "========================================================================" NOR "\n\r");

        for (clv = 5; clv >= 0; --clv) {
            for (lv = MAX_LEVEL; lv > 0; --lv) {
                for (d = descriptor_list; d; d = d->next) {
                    if (d->connected != CON_PLAYING)
                        continue;
                    victim = (d->original) ? d->original : d->character;
                    if (victim->level != lv)
                        continue;
                    if (!can_see(ch, victim))
                        continue;
                    if (!victim->clan)
                        continue;
                    if (victim->pcdata->clanlevel != clv)
                        continue;
                    ++count;
                    sprintf(buf, "【" HIW "%8s %1d %2d" NOR " 】 " HIG "%s" NOR "  -  %s(" HIY "%s" NOR ") %s \n\r",
                            victim->clan->whoname,
                            victim->pcdata->clanlevel,
                            victim->level,
                            //    class_table[ victim->class ].cwho_name,
                            victim->pcdata->clantitle,
                            victim->cname,
                            victim->name,
                            victim->pcdata->title);
                    strcat(buf1, buf);
                    if (count >= 30) {
                        send_to_char(buf1, ch);
                        buf1[0] = '\0';
                        count   = 0;
                    }
                }
            }
        }
        strcat(buf1, "========================================================================" NOR "\n\r");
        send_to_char(buf1, ch);
        return;
    } else if (!str_cmp(arg, "who")) {
        one_argument(argument, arg);
        if (arg[0] == '\0') {
            if (!ch->clan) {
                send_to_char("對不起，你並沒有加入任何幫派。", ch);
                return;
            }
            // strcpy(clan_name, ch->clan->name);
            vclan = ch->clan;
        } else {
            // strcpy(clan_name, arg);
            sprintf(clan_name, "%s", arg);
            // vclan = ch->clan;
            for (cn = clan_list; cn; cn = cn->next) {
                if (!str_cmp(clan_name, cn->leader)) {
                    vclan = cn;
                    break;
                }
            }
            if (!vclan) {
                send_to_char("對不起，找不到你要找的幫派。", ch);
                return;
            }
        }
        sprintf(buf, "[ " HIC "%s幫員列表" NOR " ]", vclan->whoname);
        send_to_char(buf, ch);
        if (ch->pcdata->clanlevel > 0 && ch->clan == vclan) {
            sprintf(buf, " 目前幫費：" HIY "%lld" NOR ", 發展等級：" HIY "%2d" NOR ", 戰役：" HIB "%d 勝" NOR " " HIR "%d 敗" NOR "\n\r",
                    vclan->bank, vclan->cclass, vclan->warwin, vclan->warlose);
            send_to_char(buf, ch);
        } else {
            send_to_char("\n\r", ch);
        }
        sprintf(buf, "========================================================================" NOR "\n\r");
        send_to_char(buf, ch);
        for (d = descriptor_list; d; d = d->next) {
            if (d->connected != CON_PLAYING)
                continue;
            victim = (d->original) ? d->original : d->character;
            if (!can_see(ch, victim))
                continue;
            if (victim->clan
                //&& !str_cmp( clan_name, victim->clan->name))
                && victim->clan == vclan) {
                wch[members]   = victim;
                class[members] = class_table[wch[members]->class].cwho_name;
                ++members;
            }
        }
        for (level = 5; level >= 0; --level) {
            for (i = 0; i < members; ++i) {
                if (wch[i]->pcdata->clanlevel == level) {
                    cn = wch[i]->clan;
                    if ((ch->pcdata->clanlevel > 3 && ch->clan == vclan) || IS_IMMORTAL(ch)) {
                        strip_ansi(wch[i]->name, no_ansi);
                        sprintf(buf, "【" HIW "%1d %2d" NOR " " HIW "%6s" NOR " 】 %s - %s(" HIY "%s" NOR ")  (" HIY "%d, %d" NOR ")\n\r",
                                wch[i]->pcdata->clanlevel,
                                wch[i]->level,
                                class[i],
                                wch[i]->pcdata->clantitle,
                                wch[i]->cname,
                                wch[i]->name,
                                //  wch[i]->pcdata->title,
                                wch[i]->pcdata->clanexp,
                                wch[i]->pcdata->clanstatistics);
                    } else {
                        strip_ansi(wch[i]->name, no_ansi);
                        sprintf(buf, "【" HIW "%1d %2d" NOR " " HIW "%6s" NOR " 】 %s - %s(" HIY "%s" NOR ") %s\n\r",
                                wch[i]->pcdata->clanlevel,
                                wch[i]->level,
                                class[i],
                                wch[i]->pcdata->clantitle,
                                wch[i]->cname,
                                wch[i]->name,
                                wch[i]->pcdata->title);
                        // sprintf( buf, "%20s%20s%22s %d\n\r",
                        //       cn->name, d->character->pcdata->clantitle,
                        //       d->character->cname, d->character->pcdata->clanstatistics );
                    }
                    send_to_char(buf, ch);
                }
            }
        }
        sprintf(buf, "%s有 " HIW "%d" NOR " 名幫眾正在線上。\n\r",
                "========================================================================" NOR "\n\r",
                members);
        send_to_char(buf, ch);
        return;
    } else if (!str_cmp(arg, "withdraw")) {
        CHAR_DATA *vch;

        if (!ch->clan) {
            send_to_char("你並沒有加入任何幫派。\n\r", ch);
            return;
        }

        if (ch->pcdata->clanlevel < 3) {
            send_to_char("你的份量不夠。", ch);
            return;
        }

        for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
            if (IS_SET(vch->act, ACT_BANKER))
                break;

        one_argument(argument, arg);
        if (is_number(arg))
            num = atoi(arg);
        else {
            send_to_char("請明確說出一個數字。\n\r", ch);
            return;
        }

        if (num < 1) {
            send_to_char("請至少領一元。\n\r", ch);
            return;
        }
        if (num > 2000000000) {
            send_to_char("不能領出這麼多錢。\n\r", ch);
            return;
        }
        num += num / 100;

        if (vch) {
            if (num < ch->clan->bank) {
                ch->gold += num;
                ch->clan->bank -= (num);
                ch->pcdata->clanstatistics -= num;
                act("$N向你一鞠躬說：這是您的金幣，請點收。謝謝您的光臨。",
                    ch, NULL, vch, TO_CHAR);
                act("$N向$n一鞠躬說：這是您的金幣，請點收。謝謝您的光臨。",
                    ch, NULL, vch, TO_ROOM);
                sprintf(buf, "chat 本人自幫庫中提領了 %d 枚金幣。", num);
                do_clan(ch, buf);
            } else {
                send_to_char("你以為你加入的是'大陸連鎖銀行幫'嗎？\n\r", ch);
                return;
            }
        } else {
            send_to_char("你找不到人幫你提錢。\n\r", ch);
            return;
        }
    } else if (!str_cmp(arg, "deposit")) {
        CHAR_DATA *vch;

        if (!ch->clan) {
            send_to_char("你並沒有加入任何幫派。\n\r", ch);
            return;
        }

        for (vch = ch->in_room->people; vch; vch = vch->next_in_room)
            if (IS_SET(vch->act, ACT_BANKER))
                break;

        one_argument(argument, arg);
        if (is_number(arg))
            num = atoi(arg);
        else {
            send_to_char("請明確說出一個數字。\n\r", ch);
            return;
        }
        if (num < 1) {
            send_to_char("請至少存一元。\n\r", ch);
            return;
        }
        if (num > 2000000000) {
            send_to_char("不能存入這麼多錢。\n\r", ch);
            return;
        }

        if (vch) {
            if (num < ch->gold) {
                ch->gold -= num;
                ch->clan->bank += num;
                ch->pcdata->clanstatistics += num;
                act("$N向你一鞠躬說：謝謝您的光臨，大陸連鎖銀行永遠竭誠為您服務。",
                    ch, NULL, vch, TO_CHAR);
                act("$N向$n一鞠躬說：謝謝您的光臨，大陸連鎖銀行永遠竭誠為您服務。",
                    ch, NULL, vch, TO_ROOM);
            } else {
                send_to_char("喔！你身上沒那麼多錢。\n\r", ch);
                return;
            }
        } else {
            send_to_char("你找不到人幫你存錢。\n\r", ch);
            return;
        }
    } else if (!str_cmp(arg, "level")) {
        if (!ch->clan)
            return;
        argument = one_argument(argument, arg);
        if (arg[0] == '\0' || !(victim = get_char_room(ch, arg)) || ch == victim || IS_NPC(victim)) {
            send_to_char("這個人不在這裡啊？\n\r", ch);
            return;
        }
        one_argument(argument, arg);
        if (!is_number(arg) || (level = atoi(arg)) > 4 || level < 0) {
            send_to_char("等級必須是 0-4 的數字。\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel < 4) {
            send_to_char("你在幫中的地位不夠。\n\r", ch);
            return;
        }
        if (victim->clan != ch->clan || IS_NPC(victim)) {
            send_to_char("你幹嘛管到別幫的家務事？。\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel <= victim->pcdata->clanlevel || ch->pcdata->clanlevel <= level) {
            send_to_char("你沒有這麼大的權限。\n\r", ch);
            return;
        }
        if (ch->clan->bank < level * level * 10000) {
            send_to_char("你的幫派在銀行中沒有足夠的存款。", ch);
            return;
        }
        if (level > victim->pcdata->clanlevel) {
            send_to_char("你在幫派裡的地位升級了！\n\r", victim);
            send_to_char("你擢昇了他(她)在幫派裡的地位！\n\r", ch);
        } else {
            send_to_char("你在幫派裡的地位降低了！\n\r", victim);
            send_to_char("你調降他(她)在幫派裡的地位！\n\r", ch);
        }
        ch->clan->member[victim->pcdata->clanlevel] -= 1;
        ch->clan->member[level] += 1;
        victim->pcdata->clanlevel = level;
        ch->clan->bank -= level * level * 10000;
    } else if (!str_cmp(arg, "title")) {
        if (!ch->clan)
            return;
        argument = one_argument(argument, arg);
        if (ch->pcdata->clanlevel == 0) {
            send_to_char("你的份量不夠。\n\r", ch);
            return;
        }
        if (arg[0] == '\0' || !(victim = get_char_room(ch, arg)) || IS_NPC(victim)) {
            send_to_char("這個人不在這裡啊？\n\r", ch);
            return;
        }
        if (!ch->clan || !victim->clan) {
            send_to_char("好歹先入幫吧？", ch);
            return;
        }
        if (victim != ch && victim->pcdata->clanlevel >= ch->pcdata->clanlevel) {
            send_to_char("你的份量不夠。\n\r", ch);
            return;
        }
        if (ch->clan != victim->clan) {
            send_to_char("你管其他幫會的家務事幹什麼？\n\r", ch);
            return;
        }
        one_argument(argument, arg);
        if (argument[0] == '\0') {
            send_to_char("[33m要把稱號改成什麼？[m\n\r", ch);
            return;
        }

        if (strlen(arg) > 21) {
            arg[21] = '\0';
            send_to_char(YEL "稱號太長了，請想一個短一點的，響亮一點的。" NOR "\n\r", ch);
        }

        smash_tilde(arg);
        clan_title(victim, arg);
        if (victim != ch) {
            sprintf(buf, "chat 本人宣布：封 %s 為 %s！", victim->short_descr, arg);
            do_clan(ch, buf);
        }
        send_to_char("[36mOK.[m\n\r", ch);
    } else if (!str_cmp(arg, "status")) {
        send_to_char("現有幫派之資料：\n\r", ch);
        send_to_char(
            //   1234567890123456789012345678901234567890123456789012345678901234567890
            "[     幫     名      ][ 創 始 人  ][現 任 幫 主][幫眾][ 發展狀況 ][   幫費   ]\n\r", ch);
        for (cn = clan_list; cn; cn = cn->next) {
            if (IS_IMMORTAL(ch) || (ch->clan == cn && ch->pcdata->clanlevel > 3)) {
                sprintf(buf, " %-20s %12s %12s  %4d%10s(%2d) %lld\n\r",
                        cn->name, cn->founder, cn->leader, clan_members(cn), cc[cn->cclass].name, cn->cclass, cn->bank);
                send_to_char(buf, ch);
            } else {
                sprintf(buf, " %-20s %12s %12s  %4d %10s  %s\n\r",
                        cn->name, cn->founder, cn->leader, clan_members(cn), cc[cn->cclass].name, bank_desc(cn->bank));
                send_to_char(buf, ch);
            }
        }
        if (clan_war->fighting == 2) {
            sprintf(buf, "%s 和 %s 的戰役還在進行中！\n\r",
                    clan_war->clan_a->name, clan_war->clan_b->name);
            send_to_char(buf, ch);
            sprintf(buf, "%s 方面仍然屹立在戰場的人員有：", clan_war->clan_a->name);
            for (i = 0; i < 10; i++)
                if (clan_war->clan_a_member[i]) {
                    sprintf(buf1, "%s, ", clan_war->clan_a_member[i]->name);
                    strcat(buf, buf1);
                }
            strcat(buf, "\n\r");
            send_to_char(buf, ch);
            sprintf(buf, "%s 方面仍然屹立在戰場的人員有：", clan_war->clan_b->name);
            for (i = 0; i < 10; i++)
                if (clan_war->clan_b_member[i]) {
                    sprintf(buf1, "%s, ", clan_war->clan_b_member[i]->name);
                    strcat(buf, buf1);
                }
            strcat(buf, "\n\r");
            send_to_char(buf, ch);
        }
        return;
    } else if (!str_cmp(arg, "+")) {
        if (!ch->clan || ch->pcdata->clanlevel < 4) {
            send_to_char("你的份量不夠。\n\r", ch);
            return;
        }
        if (ch->clan->desc)
            strcpy(buf, ch->clan->desc);
        if (strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 200) {
            send_to_char("[33m幫派描述太長了。[m\n\r", ch);
            return;
        }
        smash_tilde(argument);
        strcat(buf, argument);
        strcat(buf, "\n\r");
        if (ch->clan->desc)
            free_string(ch->clan->desc);
        ch->clan->desc = str_dup(buf);
    } else if (!str_cmp(arg, "clear")) {
        if (!ch->clan || ch->pcdata->clanlevel < 4) {
            send_to_char("你的份量不夠。\n\r", ch);
            return;
        }
        if (ch->clan->desc)
            free_string(ch->clan->desc);
        ch->clan->desc = NULL;
    } else if (!str_cmp(arg, "history")) {
        one_argument(argument, arg);
        for (cn = clan_list; cn; cn = cn->next)
            if (!str_cmp(cn->leader, arg))
                break;
        if (!cn)
            send_to_char("找不到這個幫派。\n\r", ch);
        else
            send_to_char(cn->desc, ch);
        return;
    } else if (!str_cmp(arg, "upgrade")) {
        if (!ch->clan || ch->pcdata->clanlevel < 4) {
            send_to_char("你沒那個份量。\n\r", ch);
            return;
        }
        if (ch->clan->bank < 1000000) {
            send_to_char("組織的財力不夠喔！\n\r", ch);
            return;
        }
        for (i = 0; *(cc[i].name); i++)
            if (ch->clan->score < cc[i].exp)
                break;
        if (ch->clan->cclass < i) {
            ch->clan->cclass++;
            ch->clan->bank -= 1000000;
            sprintf(buf, "\n\r" HIY "經過眾人長久的努力，" NOR "%s" HIY "的勢力更進一步地擴張了！" NOR "\n\r",
                    ch->clan->name);
            send_to_all_char(buf);
            sprintf(buf, "%s" HIY "現在已是一個" NOR " %s " HIY "的組織！" NOR "\n\r", ch->clan->name, cc[ch->clan->cclass].name);
            send_to_all_char(buf);
            send_to_char("恭喜你！\n\r", ch);
        } else {
            sprintf(buf, "你的組織還需要 %d 點發展點數才能升級。\n\r", cc[i].exp - ch->clan->score);
            send_to_char(buf, ch);
            return;
        }
    } else if (!str_cmp(arg, "dispatch")) {
        CHAR_DATA *mch, *lch;
        argument = one_argument(argument, arg);
        if (!(mch = get_char_room(ch, arg)) || !(lch = get_char_room(ch, argument))) {
            send_to_char("用法：clan dispatch 生物名 領導者名\n\r", ch);
            return;
        }
        if (!ch->clan || ch->pcdata->clanlevel < 3 || mch->clan != ch->clan || lch->clan != ch->clan || !IS_NPC(mch)) {
            send_to_char("你無法這麼做。\n\r", ch);
            return;
        }
        if (mch->level > lch->level + 5) {
            send_to_char("對不起，二者等級差距太大。\n\r", ch);
            return;
        }
        mch->master = lch;
        act("$n 將視 $N 為主人！", mch, NULL, lch, TO_ROOM);
        send_to_char("沒問題。\n\r", ch);
        return;
    } else if (!str_cmp(arg, "clanpk")) {
        if (ch->pcdata->clanlevel < 3) {
            send_to_char("你在幫內的份量不夠，無法向別的幫會宣戰。\n\r", ch);
            return;
        }
        if (ch->clan == clan_war->clan_a || ch->clan == clan_war->clan_b) {
            send_to_char("你也太好戰了吧？\n\r", ch);
            return;
        }
        if (clan_war->fighting == 2) {
            sprintf("%s 和 %s 的戰役還在進行中，請稍候。\n\r", clan_war->clan_a->name,
                    clan_war->clan_b->name);
            send_to_char(buf, ch);
            return;
        }
        if (!IS_SET(ch->act, PLR_REGISTER)) {
            send_to_char("你沒有加入 PK！\n\r", ch);
            return;
        }
        if (ch->clan->bank < 500000) {
            send_to_char("你的幫會太窮了，至少要有五十萬金幣才能完成備戰準備。\n\r", ch);
            return;
        }
        ch->clan->bank -= 500000;
        if (clan_war->fighting == 0) {
            clan_war->fighting = 1;
            clan_war->clan_a   = ch->clan;
            for (vch = ch->in_room->people; vch; vch = vch->next_in_room) {
                if (!IS_NPC(vch) && vch->desc && IS_SET(vch->act, PLR_REGISTER) && vch->clan == ch->clan) {
                    clan_war->clan_a_member[clan_war->clan_a_num] = vch;
                    vch->pcdata->clanpk                           = 1;
                    act("$n 已經加入這場戰役！", vch, NULL, NULL, TO_ROOM);
                    send_to_char("你加入了這場戰役！\n\r", vch);
                    clan_war->clan_a_num++;
                    if (clan_war->clan_a_num == 10)
                        break;
                }
            }
        } else {
            clan_war->fighting = 2;
            clan_war->clan_b   = ch->clan;
            for (vch = ch->in_room->people; vch; vch = vch->next_in_room) {
                if (!IS_NPC(vch) && vch->desc && IS_SET(vch->act, PLR_REGISTER) && vch->clan == ch->clan) {
                    clan_war->clan_b_member[clan_war->clan_b_num] = vch;
                    vch->pcdata->clanpk                           = 1;
                    act("$n 已經加入這場戰役！", vch, NULL, NULL, TO_ROOM);
                    send_to_char("你加入了這場戰役！\n\r", vch);
                    clan_war->clan_b_num++;
                    if (clan_war->clan_b_num == 10)
                        break;
                }
            }
            sprintf(buf, "" HIY "混沌幻世兩大強權 -- " NOR "%s " HIY "和" NOR " %s " HIY "爆發激烈戰鬥！" NOR "\n\r",
                    clan_war->clan_a->name, clan_war->clan_b->name);
            send_to_all_char(buf);
            sprintf(buf, "%s 方面主要參戰人員有：", clan_war->clan_a->name);
            for (i = 0; i < 10; i++)
                if (clan_war->clan_a_member[i]) {
                    sprintf(buf1, "%s, ", clan_war->clan_a_member[i]->name);
                    strcat(buf, buf1);
                }
            strcat(buf, "\n\r");
            send_to_all_char(buf);
            sprintf(buf, "%s 方面主要參戰人員有：", clan_war->clan_b->name);
            for (i = 0; i < 10; i++)
                if (clan_war->clan_b_member[i]) {
                    sprintf(buf1, "%s, ", clan_war->clan_b_member[i]->name);
                    strcat(buf, buf1);
                }
            strcat(buf, "\n\r");
            send_to_all_char(buf);
        }
    } else if (!str_cmp(arg, "cancelpk")) {
        if (clan_war->fighting < 2) {
            send_to_char("根本就還沒開打哩！\n\r", ch);
            return;
        }
        if (ch->pcdata->clanlevel < 3) {
            send_to_char("你在幫內的份量不夠謀和。\n\r", ch);
            return;
        }
        if (ch->clan == clan_war->clan_a) {
            sprintf(buf, "戰役結束，%s 向 %s 繳交賠款，以求十年生聚教訓！\n\r",
                    clan_war->clan_a->name, clan_war->clan_b->name);
            send_to_all_char(buf);
            clan_war->clan_b->bank += 1000000;
            for (i = 0; i < 10; i++) {
                if (clan_war->clan_a_member[i]) {
                    clan_war->clan_a_member[i]->pcdata->clanpk = 0;
                    clan_war->clan_a_member[i]                 = NULL;
                }
                if (clan_war->clan_b_member[i]) {
                    clan_war->clan_b_member[i]->pcdata->clanpk = 0;
                    clan_war->clan_b_member[i]                 = NULL;
                }
            }
            clan_war->fighting   = 0;
            clan_war->clan_a     = NULL;
            clan_war->clan_b     = NULL;
            clan_war->clan_a_num = 0;
            clan_war->clan_b_num = 0;
        } else {
            sprintf(buf, "戰役結束，%s 向 %s 繳交賠款，以求十年生聚教訓！\n\r",
                    clan_war->clan_b->name, clan_war->clan_a->name);
            send_to_all_char(buf);
            clan_war->clan_a->bank += 1000000;
            for (i = 0; i < 10; i++) {
                if (clan_war->clan_a_member[i]) {
                    clan_war->clan_a_member[i]->pcdata->clanpk = 0;
                    clan_war->clan_a_member[i]                 = NULL;
                }
                if (clan_war->clan_b_member[i]) {
                    clan_war->clan_b_member[i]->pcdata->clanpk = 0;
                    clan_war->clan_b_member[i]                 = NULL;
                }
            }
            clan_war->fighting   = 0;
            clan_war->clan_a     = NULL;
            clan_war->clan_b     = NULL;
            clan_war->clan_a_num = 0;
            clan_war->clan_b_num = 0;
        }
    } else if (!str_cmp(arg, "score")) {
        one_argument(argument, arg);
        cn = ch->clan;
        if (arg[0] == '\0' && !cn) {
            send_to_char("請用 clan score <幫主id> 來查詢。\n\r", ch);
            return;
        } else {
            for (cn = clan_list; cn; cn = cn->next)
                if (!str_cmp(arg, cn->leader))
                    break;
        }
        if (!cn) {
            send_to_char("沒這個幫派啊？\n\r", ch);
            return;
        }
        if (IS_IMMORTAL(ch) || (ch->pcdata->clanlevel > 2 && ch->clan == cn)) {
            AREA_DATA *pArea;
            int        vnum;
            for (pArea = area_first; pArea; pArea = pArea->next)
                if (pArea->clan == cn) {
                    for (vnum = pArea->lvnum; vnum <= pArea->uvnum; ++vnum) {
                        if (get_room_index(vnum))
                            rnum++;
                        if (get_mob_index(vnum))
                            mnum++;
                        if (get_obj_index(vnum))
                            onum++;
                    }
                    break;
                }
        }
        sprintf(buf, "\n\r********組織目前發展概況為********\n\r");
        send_to_char(buf, ch);
        sprintf(buf, "組織總人數：%d\n\r", clan_members(cn));
        send_to_char(buf, ch);
        sprintf(buf, "總經費：%lld\n\r", cn->bank);
        send_to_char(buf, ch);
        sprintf(buf, "組織等級：%d, %s\n\r", cn->cclass, cc[cn->cclass].name);
        send_to_char(buf, ch);
        sprintf(buf, "累積發展點數/升級所需點數：%d/%d\n\r", cn->score, cc[cn->cclass].exp - cn->score);
        send_to_char(buf, ch);
        sprintf(buf, "雇用點數：%d\n\r", cn->hire);
        send_to_char(buf, ch);
        sprintf(buf, "製造點數：%d\n\r", cn->construct);
        send_to_char(buf, ch);
        sprintf(buf, "領地大小：%d\n\r", rnum);
        send_to_char(buf, ch);
        sprintf(buf, "生物總數：%d\n\r", mnum);
        send_to_char(buf, ch);
        sprintf(buf, "物品總數：%d\n\r", onum);
        send_to_char(buf, ch);
        sprintf(buf, "盟友：\n\r");
        send_to_char(buf, ch);
        sprintf(buf, "戰役記錄：%d 勝 %d 敗\n\r", cn->warwin, cn->warlose);
        send_to_char(buf, ch);
        return;
    }

    /*
        else
        {
      return;
      if( !ch->clan ) return;
      if( argument[0] == '\0' ) {
          send_to_char("你要說什麼？\n\r", ch);
          return;
      }
      sprintf(buf, "%s【%s】%s(%s)%s閒聊著「%s」%s\n\r",
        HIY,
        ch->clan->name,
        ch->cname,
        ch->name,
        HIY,
        argument,
        NOR);
      for ( d = descriptor_list; d; d = d->next )
      {
          victim = (d->original) ? d->original : d->character;
          if( !victim ) continue;
          if( victim && !victim->clan ) continue;
          //if( !str_cmp( clan_name, victim->clan->name)){
          if( victim && victim->clan == ch->clan ){
        send_to_char(buf, victim);
          }
      }
      return;
        }
    */
    clan_update();
    if (ch)
        do_save(ch, "");
    if (victim && victim != ch)
        do_save(victim, "");
}

void do_clanrecall(CHAR_DATA *ch, char *argument) {
    CHAR_DATA       *victim, *horse;
    ROOM_INDEX_DATA *location;
    char             buf[MAX_STRING_LENGTH];
    int              place;

    if (!ch->in_room || !ch->clan)
        return;

    if (auction_list) {
        AUC_DATA *auction;
        for (auction = auction_list; auction; auction = auction->next) {
            if (ch == auction->buyer || ch == auction->owner) {
                send_to_char("你正在進行拍賣，不能祈求傳送。\n\r", ch);
                return;
            }
        }
    }

    act("[36m$n祈求傳送！[m", ch, NULL, NULL, TO_ROOM);

    if (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) || IS_AFFECTED(ch, AFF_CURSE)) {
        send_to_char("[33m神遺棄了你。[m\n\r", ch);
        return;
    }

    place = ch->clan->crecall;
    if (!(location = get_room_index(place))) {
        send_to_char("[33m你完完全全地迷路了。[m\n\r", ch);
        return;
    }

    if (!(ch->in_room->area->recall == location->area->recall)) {
        send_to_char("你距離幫會太遠了喔。\n\r", ch);
        return;
    }

    if (ch->in_room == location)
        return;

    if ((victim = ch->fighting)) {
        int lose;

        if (number_bits(1) == 0) {
            WAIT_STATE(ch, 4);
            lose = (ch->desc) ? 12 : 25;
            gain_exp(ch, 0 - lose);
            sprintf(buf, "[33m你沒能成功！你失去了" HIC "%d" CYN "點經驗值。[m\n\r", lose);
            send_to_char(buf, ch);
            return;
        }

        lose = (ch->desc) ? 25 : 50;
        gain_exp(ch, 0 - lose);
        sprintf(buf, "[36m你從戰鬥中傳送撤回！你失去了" HIC "%d" CYN "點經驗值。[m\n\r", lose);
        send_to_char(buf, ch);
        stop_fighting(ch, TRUE);
    }

    ch->move /= 2;

    if (ch->in_room->roomprogs && (ch->in_room->progtypes & ROOM_LEAVE) && !IS_SET(ch->act, PLR_WIZINVIS))
        rprog_leave_trigger(ch->in_room, ch, 6);

    act("[36m$n消失了。[m", ch, NULL, NULL, TO_ROOM);
    char_from_room(ch);
    char_to_room(ch, location);
    act("[36m$n忽然在這裡出現。[m", ch, NULL, NULL, TO_ROOM);
    do_look(ch, "auto");
    /*
      if ( IS_SET( ch->act, PLR_HORSE ) && ch->desc )
      {
        horse = ch->pcdata->horse;
        stop_fighting( horse, TRUE );
          if ( horse->in_room->roomprogs && ( horse->in_room->progtypes & ROOM_LEAVE )
        && !IS_SET( horse->act, PLR_WIZINVIS ) )
        rprog_leave_trigger( horse->in_room, horse, 6 );

          act( "[36m$n消失了。[m", horse, NULL, NULL, TO_ROOM );
          char_from_room( horse );
          char_to_room( horse, location );
          act( "[36m$n忽然在這裡出現。[m", horse, NULL, NULL, TO_ROOM );
      }
    */

    if (ch->pcdata && ch->pcdata->horse && ch->pcdata->horse) {
        if (ch->in_room != (horse = ch->pcdata->horse)->in_room) {
            char_from_room(horse);
            char_to_room(horse, location);
            send_to_char("你的戰馬回到了你的身邊。\n\r", victim);
        }
    }

    dm_effect(ch);

    return;
}
