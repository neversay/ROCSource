// fight_const.h: Defines constants, stances, and modes for the combat and violence systems.
// fight_const.h: 定義戰鬥與暴力系統的相關常數、架式及戰鬥模式設定。
#ifndef __FIGHT_CONST_H__
#define __FIGHT_CONST_H__
/* English Attack type table */
static const char *attack_table[] =
    {
        "hit",
        "slice",
        "stab",
        "slash",
        "whip",
        "claw",
        "blast",
        "pound",
        "crush",
        "grep",
        "bite",
        "pierce",
        "suction",
        "chop",
        "shot",
        "dart",
        "ammo" // add at 2022/04/29
};

/* Chinese Attack type table */
static const char *cattack_table[] =
    {
        "奮力擊打",
        "銳利切割", // blade
        "威猛一戳", // lance
        "揮舞劈砍", // sword
        "灼熱一鞭", // whip
        "用力一抓", // claw
        "爆發噴擊", // magical damage
        "萬鈞力搥", // staff
        "狠狠碾壓", // hammer
        "犀利一剁", // axe
        "張口猛咬",
        "尖銳刺擊", // dagger
        "猛烈吸吮",
        "剁菜連斬",
        "射出箭矢",
        "擲出飛鏢",
        "槍械射擊" // gun add at 2022/04/29
};

struct dam_message_type {
    int   min_dmg;
    char *desc;
};

static const struct dam_message_type edmt[] = {
    {0,          "[1;37mcould not hurt[m"                       },
    {1,          "[1;36mscratches[m"                            },
    {2,          "[1;36mgrazes[m"                               },
    {4,          "[1;36mhits[m"                                 },
    {6,          "[1;36minjures[m"                              },
    {8,          "[1;36mwounds[m"                               },
    {15,         "[1;36mmauls[m"                                },
    {30,         "[1;36mdecimates[m"                            },
    {40,         "[1;35mdecimates[m"                            },
    {50,         "[1;35mdevastates[m"                           },
    {60,         "[1;35mmaims[m"                                },
    {70,         "[1;35mMUTILATES[m"                            },
    {80,         "[1;35mDISEMBOWELS[m"                          },
    {90,         "[1;35m* DISEMBOWELS *[m"                      },
    {100,        "[1;32m* EVISCERATES *[m"                      },
    {200,        "[1;32m** EVISCERATES **[m"                    },
    {300,        "[1;32m** MASSACRES **[m"                      },
    {400,        "[1;32m*** MASSACRES ***[m"                    },
    {500,        "[1;32m*** DEMOLISHES ***[m"                   },
    {600,        "[1;32m**** DEMOLISHES ****[m"                 },
    {700,        "[1;32m**** DESTROYS ****[m"                   },
    {800,        "[1;32m***** DESTROYS *****[m"                 },
    {900,        "[1;32mA-N-N-I-H-I-L-A-T-E-S[m"                },
    {1000,       "[1;33mA-N-N-I-H-I-L-A-T-E-S[m"                },
    {1500,       "[1;33mD-E-R-A-C-I-N-A-T-E-S[m"                },
    {2000,       "[1;33mE-R-A-D-I-C-A-T-E-S[m"                  },
    {2500,       "[1;31mE-R-A-D-I-C-A-T-E-S[m"                  },
    {3000,       "[1;31m! D..D-I-S-I-N-T-E-G-R-A-T-E-S ![m"     },
    {3500,       "[1;31m!! D..D-I-S-S-O-C-I-A-T-I-V-E-S !![m"   },
    {4000,       "[1;31m!!! COMMINUTES !!![m"                   },
    {5000,
     "\n\r\n\r\t[1;31mA - T - O - M - I - Z - E - S[m\n\r\n\r"  },
    {1073741824,
     "\n\r\n\r\t[1;31;5mA - T - O - M - I - Z - E - S[m\n\r\n\r"}
};

static const struct dam_message_type cedmt[] = {
    {0,          "[1;37m無法傷害[m"                                                                  },
    {1,          "[1;36m如微風輕拂[m"                                                               },
    {2,          "[1;36m輕輕擦過[m"                                                                  },
    {4,          "[1;36m幫忙搔癢[m"                                                                  },
    {6,          "[1;36m幫忙按摩[m"                                                                  },
    {8,          "[1;36m打到[m"                                                                        },
    {15,         "[1;36m擊中[m"                                                                        },
    {30,         "[1;36m嚇到[m"                                                                        },
    {40,         "[1;35m嚇到[m"                                                                        },
    {50,         "[1;35m稍感疼痛[m"                                                                  },
    {60,         "[1;35m有些擦傷[m"                                                                  },
    {70,         "[1;35m開始破皮[m"                                                                  },
    {80,         "[1;35m烏青瘀血[m"                                                                  },
    {90,         "[1;35m* 烏青瘀血 *[m"                                                              },
    {100,        "[1;32m* 疼痛不堪 *[m"                                                              },
    {200,        "[1;32m** 疼痛不堪 **[m"                                                            },
    {300,        "[1;32m** 尖聲哀叫 **[m"                                                            },
    {400,        "[1;32m*** 尖聲哀叫 ***[m"                                                          },
    {500,        "[1;32m*** 鮮血直流 ***[m"                                                          },
    {600,        "[1;32m**** 鮮血直流 ****[m"                                                        },
    {700,        "[1;32m**** 血流如注 ****[m"                                                        },
    {800,        "[1;32m***** 血流如注 *****[m"                                                      },
    {900,        "[1;32m猛噴鮮血，齒牙迸裂[m"                                                   },
    {1000,       "[1;33m猛噴鮮血，齒牙迸裂[m"                                                   },
    {1500,       "[1;33m臉孔扭曲，內臟出血[m"                                                   },
    {2000,       "[1;33m分筋錯骨，臟器碎裂[m"                                                   },
    {2500,       "[1;31m分筋錯骨，臟器碎裂[m"                                                   },
    {3000,       "[1;31m！筋-脈-寸-斷， 血-肉-模-糊！[m"                                      },
    {3500,       "[1;31m！！粉-身-碎-骨，魂-飛-魄-滅！！[m"                                 },
    {4000,
     "\n\r[1;31m！！！ 一  佛  升  天，二  佛  涅  盤 ！！！\n\r哇...[m"                 },
    {5000,
     "\n\r\n\r\t\t[1;31m化  成  齎  粉\n\r\t\t\t灰  飛  湮  滅[m\n\r\t\t\t\t\t我的天...\n\r"  },
    {1073741824,
     "\n\r\n\r\t\t[1;31;5m化  成  齎  粉\n\r\t\t\t灰  飛  湮  滅[m\n\r\t\t\t\t\t我的天...\n\r"}
};

#endif
