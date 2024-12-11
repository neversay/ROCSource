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
//*  PROGRAM ID  : act_social.c                                   *
//*  LANGUAGE    : c                                              *
//*---------------------------------------------------------------*
//*  MODIFY  BY  : 0940618 Razgriz                                *
//*  PURPOSE     :         修正 check_social()                    *
//*              :         在戰鬥中對別的房間的玩家做 social      *
//*              :         導致 mob 單方面解除戰鬥                *
//*****************************************************************
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "ansi.h"


bool	
check_social	args( ( CHAR_DATA *ch, char *command, char *argument ) );


/* New socials contributed by Katrina and Binky */
const	struct	social_type	social_table [] =
{
      {
	"?",
	"你露出疑惑的表情。",
	"$n露出疑惑的表情。",
	"你對$N露出疑惑的表情。",
	"$n對$N露出疑惑的表情。",
	"$n對你露出疑惑的表情。",
	"",
	""
    },
      {
	  "--",
	  "你露出\"-_-\"的表情。",
	  "$n露出\"-_-\"的表情。",
	  "你對$N露出\"-_-\"的表情。",
	  "$n對$N露出\"-_-\"的表情。",
	  "$n對你露出\"-_-\"的表情。",
	  "你對自己露出\"-_-\"的表情。",
	  "$n對自己露出\"-_-\"的表情。"
      },

 {
	"accuse",
	"Accuse 誰？",
	"",
	"你不懷好意的盯著$N瞧。",
	"$n不懷好意的盯著$N瞧。",
	"$n不懷好意的盯著你瞧。",
	"你感到良心不安。",
	"$n看起來良心不安。"
    },

    {
	"ack",
	"你露出一付「那會按呢」的表情。",
	"$n露出一付「那會按呢」的表情。",
	"",
	"",
	"",
	"",
	""
    },

    {
	"addoil",
	"你對所有人大喊：「加油！」",
	"$n對所有人大喊：「加油！」",
	"你向$N大喊：「加油！」",
	"$n向$N大喊：「加油！」",
	"$n對你大喊：「加油！」",
	"你替自己加油打氣。",
	"$n對自己說：「加油！」"
    },

    {
	"agree",
	"你完全同意。",
	"$n完全同意。",
	"你完全同意$N的看法。",
	"$n完全同意$N的看法。",
	"$n完全同意你的看法。",
	"你對自己的看法感到很自豪。",
	"$n對$e自己的看法感到很自豪。"
    },
    { // by chinko@ROC
        "aluba",
            "你要阿魯巴誰？",
        //"$n正虎視眈眈的尋找獵物中。",
        "$n口中喃喃念著：「阿魯巴～我要阿魯巴～」一邊四處尋找對象。",
        "你把$N拖去阿魯巴, 讓他感覺生不如死！",
        "$n正殘暴的對$N施展傳說中的單人單腳阿魯巴。啊！別再踩了～",
        "$n把$N拖過去, 單腳猛往他的跨下踩, $N這下糟了！",
        "你瘋了嗎？",
        "$n正在對自己做自助式阿魯巴！這？"
    },

    {
	"angry",
	"你露出一付很生氣的表情。",
	"$n露出一付很生氣的表情。",
	"你生氣地瞪著$N。",
	"$n生氣地瞪著$N。",
	"$n生氣地瞪著你。",
	"你對自己的愚蠢行為感到很生氣。",
	"$n對自己的愚蠢行為感到很生氣。"
    },

    {
	"applaud",
	"你興奮地鼓掌.",
	"$n興奮地鼓掌.",
	"你對著$N鼓掌喝采！",
	"$n對著$N鼓掌喝采.",
	"$n對你鼓掌喝采，你一定是做了什麼了不起的事。",
	"對自己鼓掌？你很了不起嗎？",
	"$n對著$e自己鼓掌，好像以為自己很了不起。"
    },

    {
	"bark",
	"你叫了一聲「汪」！",
	"$n學狗叫了一聲「汪」。",
	"你對$N大聲咆哮。",
	"$n對$N大聲咆哮。",
	"$n對你大聲咆哮。",
	"你對著自己學狗叫，「汪」「汪」！",
	"$n對著$e自己學狗叫。"
    },

    {
	"beer",
	"你拿起一杯冰啤酒，仰頭一飲而盡，哇…好爽！",
	"$n拿起一杯冰啤酒，抬頭一口氣喝乾，臉上露出滿足的表情。",
	"你恭敬地替$N倒了一杯清涼的冰啤酒。",
	"$n恭敬地替$N倒了一杯冰涼的啤酒。",
	"$n恭敬地替你倒了一杯冰啤酒。",
	"你替自己倒了一杯酒。",
	"$n替$e自己倒了一杯酒。"
    },

    {
	"beg",
	"你可憐兮兮的叫道：「好心的帥哥、美女～請施捨我幾塊錢吧～」",
	"$n可憐兮兮的叫道：「好心的帥哥、美女～請施捨我幾塊錢吧～」",
	"你對著$N可憐兮兮的叫道：「好心的帥哥、美女～請施捨我幾塊錢吧～」",
	"$n對著$N可憐兮兮的叫道：「好心的帥哥、美女～請施捨我幾塊錢吧～」",
	"$n對著你可憐兮兮的叫道：「好心的帥哥、美女～施捨我幾塊錢吧～」",
	"你絕望的大叫：「神啊～為什麼我的命這麼苦啊～」",
	"$n絕望的大叫：「神啊～為什麼我的命這麼苦啊～」"
    },
    {
	"bf",  /* for rimu */
	"你望空大喊：「神啊～賜我一個美少年吧～」",
	"$n望空大喊：「神啊～賜我一個美少年吧～」",
	"你生氣地對$N喊道：「小子，不淮動我的美少年！」",
	"$n生氣地對$N喊道：「小子，不淮動我的美少年！」",
	"$n生氣地對你喊道：「小子，不淮動我的美少年！」",
	"你生氣地喊道：「那一個傢伙敢動我的美少年！」",
	"$n生氣地喊道：「那一個傢伙敢動我的美少年！」"
    },

    {
        "stool",
        "你坐在折凳上面隱藏自己的殺機。",
        "雖然$n露出一副壞蛋臉坐在折凳上，可是你卻感覺不到任何殺機。",
        "你一把抓起屁股下的折凳，朝著$N的大臉猛力打下去！",
        "$n突然抓起屁股下的折凳朝$N的臉上打去，這一招讓你不禁讚嘆「好折凳！」",
        "$n突然抓起屁股下的折凳朝你臉上打下來，ＮＯ～～～",
        "$n拿著折凳狂笑道：「連警察都拿我沒輒啊！」",
        "$n拿著折凳狂笑道：「連警察都拿我沒輒！」哇，果然是七大武器之首！",
}
,
{
           "zc",
           "你在空中快速的比劃了幾下，口裡喃喃念著：「Zc退散！BG不要來！」",
           "$n在空中快速的比劃了幾下，口裡喃喃念著：「Zc退散！BG不要來！」",
           "你向$N喃喃念道：「願Zc破骰與你同在」",
           "$n向$N喃喃念道：「願Zc破骰與你同在」",
           "$n對你咒道：「願Zc破骰與你同在」，你覺得一定會BG",
           "你試著讓自己遠離Zc的破骰，但成效似乎不怎麼樣。",
           "$n試著讓$m遠離Zc的詛咒，可是看來......"
       },
 {
          "masterAsia",
          "你用最大力氣宣揚東方不敗流",
          "$n放聲大喝：「流派，東方不敗乃王者之風！」",
          "你對著$N大喝：「你這個笨蛋弟子！」",
          "$n對著$N大喝：「你這個笨蛋弟子！」",
          "$n大聲以師父的身份的斥責你，讓你一頭霧水",
          "你相信自己的東方不敗流是最強的",
          "$n對著自己說：「我是東方不敗！我是最強的！」"
      },
 {
           "richman",
           "你用力在背包裡翻找，拿出了一副大富翁，吆喝著大家來玩。",
           "$n不知道從那麼變出來一副大富翁，吵著：「來玩嘛！來玩嘛！」",
           "你向$N說著：「地便宜賣你，豁免權給我就好啦！」",
           "$n向$N偷偷的進行私下交易，慘了，又要虧本了。",
           "$n對你咬耳朵：「我要豁免，其他人怎麼死我不管！」",
           "你心裡想著：「糟！走到強盜那就要破產了。」",
           "你看著$n臉上寫滿了憂慮，嘻嘻，又有一家要倒了。"
       },
{
                "boring",
                "你向天吶喊道：「天啊！怎麼這麼無聊啊！！」",
                "$n向天大喊：「好無聊啊！！」",
                "你懶洋洋地對$N說：「很無聊耶...」",
                "$n懶洋洋地對$N說：「很無聊耶...」",
                "$n懶洋洋地對你說：「很無聊耶...」",
                "你因為太無聊沒有事情做，雙眼無神的盯著前方。",
                "$n因為太無聊沒有事情做，雙眼無神的坐在那邊。"
        },
{
          "shining",
          "你的眼睛突然冒出了異樣的光芒！",
          "$n的眼神突然出現了異樣的光芒！彷彿發現了什麼！",
          "你發現了$N的秘密了！嘿嘿嘿嘿！",
          "$n似乎發現了什麼，詭異地盯著$N瞧個不停",
          "$n閃亮的雙眼盯著你直直看，讓你渾身不自在！",
          "你發現了一些不為人知的祕密！",
          "$n似乎發現了什麼祕密！喔喔喔喔！"
      },
 {
  "wc",
  "你不好意思地對大家說：「對不起，我去上個洗手間！」。",
  "$n不好意思地對大家說：「對不起，我去上個洗手間！」，然後急急忙忙的跑開了。",
  "你尷尬地對$N說：「上個廁所！等我一下喔。」",
  "$n尷尬地對$N說：「上個廁所！等我一下喔。」",
  "$n尷尬地對你說：「上個廁所！等我一下喔。」",
  "",
  "",
      },
 {
          "phone",
          "你不好意思地對大家說：「接個電話！」。",
          "$n大叫說，電話響了，我去接電話！",
          "你拍謝地對$N說：「接個電話！請稍後片刻。」",
          "$n拍謝地對$N說：「接個電話！請稍後片刻。」",
          "$n拍謝地對你說：「接個電話！請稍後片刻。」",
          "你脫下鞋子，把鞋子當電話說：「喂！阿榮嗎？」",
          "$n脫下鞋子，把鞋子當電話說：「喂！阿榮嗎？」",
      },
 {
      "candle",
      "你點燃了一根紅色蠟燭，心中燃起了一絲邪惡的念頭。",
      "$n點燃一根紅色蠟燭，你看到那紅色蠟油不斷的滴落，還是趕緊閃遠一點的好。",
      "你將蠟燭油滴在$N身上，令他發出「愉快」的叫聲！",
      "$n將蠟燭油滴在$N身上，令$N哇哇大叫，哇噢～～",
      "$n將滾燙的蠟燭油滴在你身上，救、救人喔！",
      "你將蠟燭油滴在自己身上，好燙燙燙燙燙......",
      "$n居然在自己身上滴蠟燭油？$s興趣還真是奇特啊。",
      },
{
      "charm",
      "你想要找誰來成為你的戰利品呢？",
      "$n正不斷的四處張望，似乎在尋找下手的目標，小心別被$e看上了。",
      "經過一陣誘惑，$N終於成了你的戰利品了。",
      "$N禁不起$e的誘惑，終於敗倒在$n的石榴裙下。",
      "你禁不起$n的誘惑，決定要永遠伴隨他左右了。",
      "你沈浸在那鏡中美麗的形影，久久不能自拔。",
      "你看到$n陶醉在那鏡子前面，彷彿已經失了魂似的。",                              
},
 {
      "pardon",
      "你在胸前劃個十字，喃喃說道：願上帝能寬恕你們這些罪人～阿門～",
      "$n在胸前劃個十字，喃喃說道：願上帝能寬恕你們這些罪人～阿門～",
      "你一手拍著$N的肩膀，一手在胸前劃個十字對$N說道：上帝會原諒你的～阿門～",
      "$n一手拍在$N的肩膀，一手在胸前劃個十字對$N說道：上帝會原諒你的～阿門～",
      "$n一手拍在你的肩膀，一手在胸前劃個十字對你說道：上帝會原諒你的～阿門～",
      "你在胸前劃個十字，大聲地說道：神啊～請寬恕我吧～阿門～",
      "$n在胸前劃個十字，大聲地說道：神啊～請寬恕我吧～阿門～",
      },
{
          "meow",
          "你像隻可愛的小貓咪一般咪咪地叫, 看起來十分令人想抱在懷中好好撫摸。",
          "$n像隻可愛的小貓咪一般咪咪地叫, 看起來十分令人想抱在懷中好好撫摸。",
          "你像隻可愛的小貓咪, 在$N身旁不停地撒嬌, 希望博得$N的同情。",
          "$n像隻可愛的小貓咪, 在$N身旁不停地撒嬌, 希望博得$N的同情。",
          "$n像隻可愛的小貓咪, 在你身旁不停地撒嬌, 希望博得你的同情。",
          "你像隻可愛的招財貓在那揮著貓爪。",
          "$n像隻可愛的招財貓在那揮著貓爪。"
      },
{
          "newbie",
          "你對著大家說：「我是新手～請大家多多照顧我喔～」",
          "$n對大家鞠個躬說道：「我是新手，今後還請大家多多照顧我喔～」",
          "你向$N說：「我是新手，今後還請你多多照顧～ *^_^*」",
          "$n向$N說：「我是新手，今後還請你多多照顧～ *^_^*」",
          "$n向你說：「我是新手，今後還請你多多照顧～ *^_^*」",
          "你對著自己加油說道：「我一定會是個討人喜歡的新手的～」",
          "你看到$n那樣的有禮貌，讓你不禁想好好的照顧$e。",
          },
{
	"bingo",
	"你高興的大叫：「Ｂｉｎｇｏ～」",
	"$n高興的大叫：「Ｂｉｎｇｏ～」",
	"你興奮地對$N說：「恭喜你中獎了！」",
	"$n興奮地對$N說：「恭喜你中獎了！」",
	"$n興奮地對你說：「恭喜你中獎了！」",
	"",
	""
    },

    {
	"bite",
	"Bite 誰？",
	"",
	"你張開血盆大口，往$N狠狠地咬了一口！",
	"$n張開血盆大口，往$N狠狠地咬了一口。",
	"$n張開血盆大口，往你狠狠地咬了一口！",
	"你餓得發慌，於是咬了自己一口。",
	"$n咬了自己一口，是不是餓壞了？"
    },
    {
        "blatant",
        "你痛苦的摀住眼大叫：「閃光！是閃光啊！」",
        "$n痛苦的摀住眼大叫：「閃光！是閃光啊！」",
        "你痛苦的對著$N說：「好刺眼的光芒啊！」",
        "$n痛苦的對著$N說：「太刺眼了！我瞎了！」",
        "$n痛苦的對著你說：「好刺眼的光芒啊！」",
        "你戴著墨鏡，哀傷的說著：「可魯，我的可魯呢？」",
        "$n戴著墨鏡哀傷的尋找自己的導盲犬。"

    },
    { // by chinko@ROC

        "bless",
            "嗯。。希望世界和平呀～",
            "$n好像變成了神一樣，看他的眼神，是多麼的和藹呀！",
            "給你一點點的祝福，祝你能事事順心。",
            "$n在$N的頭上，灑下一輪光圈，好像是在為他祝福似的！",
            "$n的真心祝福你能一切順利呦！",
            "Oh！God Bless Me～",
                "$n對於自己的未來懷著滿心的期許\ 。"
    },

    {
	"blush",
	"你的臉害羞的紅了起來！",
	"$n的臉害羞的紅了起來！",
	"你一見到$N，馬上臉紅著害羞跑開了。",
	"$n一見到$N，馬上臉紅著害羞跑開了。",
	"$n一見到你，馬上臉紅著害羞跑開了。",
	"你對自己的行為感到羞恥。",
	"$n對$e的行為感到羞恥。"
    },

    {
	"bounce",
	"你高興地蹦蹦跳跳。",
	"$n像個小孩子一樣地蹦蹦跳跳。",
	"你高興地跳到$N的身上。",
	"$n高興地跳到$N的身上。",
	"$n高興地跳到你的身上。",
	"",
	""
    },

    {
	"bow",
	"你優雅的欠身鞠躬。",
	"$n優雅的欠身鞠躬。",
	"你向$N深深一鞠躬。",
	"$n向$N深深一鞠躬。",
	"$n對你深深一鞠躬。",
	"",
	""
    },
/*
    {
	"burp",
	"You burp loudly.",
	"$n burps loudly.",
	"You burp loudly to $M in response.",
	"$n burps loudly in response to $N's remark.",
	"$n burps loudly in response to your remark.",
	"You burp at yourself.",
	"$n burps at $mself.  What a sick sight."
    },
*/
    {
	"bye",
	"你對大家揮手說再見！",
	"$n對大家揮手說再見！",
	"你向$N說道：「走好，不送了。」",
	"$n向$N說道：「走好，不送了。」",
	"$n對你說道：「走好，不送了。」",
	"",
	""
    },

    {
	"cackle",
	"你很酷地仰起頭，尖聲地狂笑起來！",
	"$n很酷地仰起頭，尖聲地狂笑起來！",
	"你對$N尖聲狂笑。",
	"$n對$N尖聲狂笑。",
	"$n對你尖聲狂笑著。 你最好離$m遠一點。",
	"你像發了瘋般地對自己尖聲狂笑。",
	"$n對著自己尖聲狂笑，看起來好像不太對勁。 "
    },

    {
	"chuckle",
	"你輕輕地笑了一聲。",
	"$n 輕輕地笑了一聲。",
	"你對著$N輕輕地笑了一聲。 ",
	"$n對著$N輕輕地笑了一聲。",
	"$n對著你輕輕地笑了一聲。",
	"",
	""
    },

    {
	"clap",
	"你興奮地鼓掌.",
	"$n興奮地鼓掌.",
	"你對$N鼓掌喝采！",
	"$n對$N鼓掌喝采.",
	"$n對你鼓掌喝采，你一定是做了什麼了不起的事。",
	"對自己鼓掌？你很了不起嗎？",
	"$n對著$e自己鼓掌，好像以為自己很了不起。"
/*	"You clap your hands together.",
	"$n shows $s approval by clapping $s hands together.",
	"You clap at $S performance.",
	"$n claps at $N's performance.",
	"$n claps at your performance.",
	"You clap at your own performance.",
	"$n claps at $s own performance." */
    },
/*
    {
	"comb",
	"You comb your hair - perfect.",
	"$n combs $s hair, how dashing!",
	"You patiently untangle $N's hair - what a mess!",
	"$n tries patiently to untangle $N's hair.",
	"$n pulls your hair in an attempt to comb it.",
	"You pull your hair, but it will not be combed.",
	"$n tries to comb $s tangled hair."
    },
*/
    {
	"cold",
	"你感到一絲寒意。",
	"$n突然被凍得動彈不得！",
	"你突然全身僵硬，好像被$N的冷笑話凍住了。",
	"$N的冷笑話凍得$n"HIR"！全身僵硬，四肢麻木！"NOR,
	"$N的冷笑話凍得$n"HIR"！全身僵硬，四肢麻木！"NOR,
	"$n的冷笑話凍得你"HIR"！全身僵硬，四肢麻木！"NOR,
	"$n的冷笑話凍得$e自己"HIR"！全身僵硬，四肢麻木！"NOR
    },
    {
	"comfort",
	"comfort 誰？",
	"",
	"你細心地安慰著$N。",
	"$n細心地安慰著$N。",
	"$n細心地安慰著你。",
	"",
	""
    },
    
    {
	"cong",
	"你大叫：「可喜可樂，可喜可樂～」",
	"$n大叫：「可喜可樂，可喜可樂～」",
	"你對著$N大叫：「恭喜發財，紅包一個來！」",
	"$n對著$N大叫：「恭喜發財，紅包一個來！」",
	"$n對著你大叫：「恭喜發財，紅包一個來！」",
	"",
	""
    },

    {
	"cook",
	"你拿出爐子、鍋子，看起來要弄一鍋美味的料理。",
	"$n拿出爐子、鍋子，看起來要弄一鍋美味的料理。",
	"你架起火爐，放上一鍋湯，把$N丟了進去，還在旁邊跳著食人族的舞蹈。",
	"$n架起火爐，放上一鍋湯，把$N丟了進去，還在旁邊跳著食人族的舞蹈。",
	"$n架起火爐，放上一鍋湯，把你丟了進去，還在旁邊跳著食人族的舞蹈。",
	"你熱了一鍋油，然後走了進去，把自己炸成金黃色，看起來又香又脆。",
	"$n熱了一鍋油，然後走了進去，把$e炸成金黃色，看起來又香又脆。"
    },

    {
	"cringe",
	"你害怕的直發抖！",
	"$n害怕的直發抖。",
	"你害怕$N而發抖。",
	"$n害怕$N而發抖。",
	"$n對著你直發抖，看起來好像很怕你。",
	"你覺得自己長得很可怕嗎？",
	""
    },

    {
	"cry",
	"你放聲大哭，哇～～～",
	"$n放聲大哭。",
	"你靠在$N的肩膀上，放聲大哭。",
	"$n靠在$N的肩膀上，放聲大哭。",
	"$n靠在你的肩膀上，放聲大哭。",
	"你不停地哭哭啼啼。",
	"$n不停地哭哭啼啼。"
    },

    {
	"cuddle",
	"cuddle 誰？",
	"",
	"你熱情地擁抱著$N。",
	"$n熱情地擁抱著$N。",
	"$n熱情地擁抱著你。",
	"你雙手環抱，沉醉在自己的世界裡。",
	"$n雙手環抱，沉醉在自己的世界裡。"
    },

    {
	"curse",
	"你罵道：「#*X^%?*&!」",
	"$n罵道：「 #*X^%?*&!」",
	"你對著$N破口大罵！.",
	"$n對著$N破口大罵。",
	"$n對著你破口大罵，你一定做了什麼令他不爽的事。",
	"",
	""
    },

    {
	"curtsey",
	"你對在場的所有人福了一福。",
	"$n對所有人福了一福。",
	"你對$N躬身福了一福。",
	"$n對$N躬身福了一福。",
	"$n對你躬身福了一福。",
	"",
	""
    },

    {
	"dance",
	"你高興的跳起舞來！",
	"$n高興的跳起舞來！",
	"你拉著$N的手，高興的跳起舞來！",
	"$n拉著$N的手，高興的跳起舞來！",
	"$n拉著你的手，高興的跳起舞來！",
	"",
	""
    },

    {	
	"die",
	"你死了。",
	"$n死了。",
	"你吐出一口鮮血，死在$N的懷裡。",
	"$n吐出一口鮮血，死在$N的懷裡。",
	"$n吐出一口鮮血，死在你的懷裡。",
	"你囂張地說道：「來呀，來殺我呀～」",
	"$n囂張地說道：「來呀，來殺我呀～」"
    },

    {
	"disagree",
	"你完全不同意。",
	"$n完全不同意。",
	"你完全不同意$N的看法。",
	"$n完全不同意$N的看法。",
	"$n完全不同意你的看法。",
	"你認為自己的看法完全錯誤。",
	"$n認為$e自己的看法完全錯誤。"
    },
    {
        "dive",
        "你找了個地洞鑽了進去。",
        "$n找了個地洞鑽了進去。",
        "你鑽進$N的懷裡撒嬌。",
        "$n鑽進$N的懷裡撒嬌。",
        "$n鑽進你的懷裡撒嬌。",
        "你想盡辦法把自己的頭埋進土裡。",
        "$n在地上挖了個洞，然後把自己的頭埋進去。"
    },
    {
        "domon",
        "你熱血沸騰地說：「我的手中正火紅燃燒著!!」",
        "$n熱血地說：「我的手中正火紅燃燒著!!」",
        "你向$N熱血地說：「看我的 GOD~~~ FINGER~~~ !!」, $N被擊中後開始全身燃燒著。",
        "$n向$N熱血地說：「看我的 GOD~~~ FINGER~~~ !!」, $N被擊中後開始全身燃燒著。",
        "$n對你熱血地說：「看我的 GOD~~~ FINGER~~~ !!」, 你被擊中後開始全身燃燒著。",
        "你手中結著印, 全身散發著金色光芒, 進入明鏡止水的狀態。",
        "$n全身散發著強烈的金色光芒。"
    },


    /*
     * This one's for Baka, Penn, and Onethumb!
     */
    {
	"drool",
	"你一個不小心，口水滴了滿地。",
	"$n一個不小心，口水滴了滿地。",
	"你盯著$N瞧個不停，口水都流了出來。",
	"$n盯著$N瞧個不停，口水都流了出來。",
	"$n盯著你瞧個不停，口水都流了出來。",
	"你一邊傻笑，口水一邊流了滿身。",
	"$n一邊傻笑，口水一邊流了$m滿身。",
    },
    {
	"drunk",
	"你喝得醉醺醺的，挺個大肚子，打了個嗝，「呃～～」。",
	"$n喝得醉醺醺的，挺個大肚子，「呃」的一聲打了個嗝。",
	"你用關心的語氣對$N說：「孩子，你醉了。」",
	"$n用關心的語氣對$N說：「孩子，你醉了。」",
	"$n用關心的語氣對你說：「孩子，你醉了。」",
	"",
	""
    },
    { //by sword@ROC
        "duel",
        "你大聲的怒吼︰「是誰！？有種就出來決一生死！」",
        "$n大聲的怒吼：「是誰！？有種就出來決一生死！」",
        "你憤而向$N下了戰書︰「今日不是你死就是我亡，咱們決鬥場見！」",
        "$n憤而向$N下了戰書︰「今日不是你死就是我亡，咱們決鬥場見！」",
        "$n憤而向你下了戰書︰「今日不是你死就是我亡，咱們決鬥場見！」",
        "你挑釁的叫喊︰「沒有人敢跟我決鬥嘛！？」",
        "$n挑釁的叫喊︰「沒有人敢跟我決鬥嘛！？」。"
    },


    {
	"duh",
	"你大叫一聲：「啊！我怎麼沒想到！」",
	"$n大叫一聲：「啊！我怎麼沒想到！」",
	"","","","",""
    },

    {
	"dunno",
	"你搖搖頭，說：「我不知道。」",
	"$n搖搖頭，說：「我不知道。」",
	"你對$N說：「你說什麼？我不清楚哦。」",
	"$n對$N說：「你說什麼？我不清楚哦。」",
	"$n對你說：「你說什麼？我不清楚哦。」",
	"你高聲唱著：「啦～啦～啦～我什麼都不知道。」",
	"$n高聲唱著：「啦～啦～啦～我什麼都不知道。」"
    },

    {
	"enter",
	"該死！你的 ENTER 鍵卡住了！\n\r\n\r\n\r",
	"$n低著頭說：「對不起，我的 ENTER 卡住了。」",
	"你苦口婆心地勸$N：「該換個 ENTER 了。」",
	"$n苦口婆心地勸$N：「該換個 ENTER 了。」",
	"$n苦口婆心地勸你：「該換個 ENTER 了。」",
	"你像發瘋似地拔掉自己鍵盤上的 ENTER。",
	"$n像瘋子似地把自己鍵盤上的 ENTER 拔掉，還一邊狂笑：「你再卡啊～再卡啊！」"
	    
    },
      {
          "envy",
          "你眼中流露出羨慕的眼神！",
          "$n的眼裡流露出羨慕的眼神！",
          "你羨慕地盯著$N瞧個不停",
          "$n羨慕地盯著$N瞧個不停",
          "$n羨慕地盯著你直直瞧，讓你感覺毛骨聳然！",
          "你自豪地說著：怎樣！羨慕吧！",
          "$n自豪地說著：怎樣！羨慕吧！嘿嘿嘿！"
      },

    {
        "eyebeam",
        "你的眼睛射出怪異的光線朝四周掃射。",
        "$n的眼睛突然射出怪異的光線朝四周胡亂掃射！",
        "你用眼睛射線把$N變成一座雕像。",
        "$n的眼睛突然射出怪異的光線，把$N變成一座雕像！",
        "你被$n的眼睛射線變成一座雕像！",
        "你不小心把眼睛光線射到自己的身上，好痛阿！",
        "$n不小心把眼睛光線射到自己的身上痛的哇哇大叫。"

    },
    {
	"faint",
	"你口吐白沫，昏倒在地上。",
	"$n口吐白沫，「咚」的一聲昏倒在地。",
	"你昏倒在$N的懷裡。",
	"$n昏倒在$N的懷裡。",
	"$n昏倒在你的懷裡。",
	"",
	""
    },

    {
	"fakerep",
	"你的狀態：30059/30059點生命力、30611/30582點魔法力、30130/30130點移動力、13500000點經驗值。",                                                                  "$n的狀態：30059/30059點生命力、30611/30582點魔法力、30130/30130點移動力、13500000點經驗值。",
	"",
	"",
	"",
	""
    },


    {
	"fart",
	"你懂不懂禮貌怎麼寫阿？",
	"$n 用力放了一個響屁...一陣綠色的煙幕慢慢飄過來！",
	"你朝著 $N 放了一個又臭又響的屁。 孩子，你真病態。",
	"$n 朝著 $N 放了一個又臭又響的屁。 你最好在 $e 轉向你這之前逃跑！",
	"$n 朝著你放了一個又臭又響的屁。 天阿!你不能呼吸了。",
	"你向著自己放了沖天連環臭屁。 這是你應得的。",
	"$n 向著自己放了沖天連環臭屁。  $e 的嗜好真是奇特阿。"
    },
    {
	"fear",
	"你害怕的直發抖。",
	"$n害怕的直發抖。",
	"你對著$N說：「哈哈～怕了吧。」",
	"$n對著$N說：「哈哈～怕了吧。」",
	"$n對著你說：「哈哈～怕了吧。」",
	"你瘋狂笑道：「怕我啊，我最喜歡別人怕我了！」",
	"$n瘋狂笑道：「怕我啊，我最喜歡別人怕我了！」"
	    
    },
        {
	    "five",
	    "你高舉著手，不知道要幹什麼。",
	    "$n高舉著手，不知道想幹嘛？",
	    "你高興的和$N打了個五。",
	    "$n高興的和$N打了個五。",
	    "$n高興的和你打了個五。",
	    "你對其他人說：「Hey! Give me FIVE !!」",
	    "$n對大家說：「Hey! Give me FIVE !!」"
	},

/*
    {
	"flip",
	"You flip head over heels.",
	"$n flips head over heels.",
	"You flip $M over your shoulder.",
	"$n flips $N over $s shoulder.",
	"$n flips you over $s shoulder.  Hmmmm.",
	"You tumble all over the room.",
	"$n does some nice tumbling and gymnastics."
    },
*/
    {
	"flop",
	"你撲通的跌了一跤。",
	"$n撲通的跌了一跤。",
	"你一個不小心，跌倒在$N的懷裡。",
	"$n一個不小心，跌倒在$N的懷裡。",
	"$n一個不小心，跌倒在你的懷裡。",
	"",
	""
    },

    {
	"fondle",
	"fondle 誰？",
	"$n的雙手在空中亂揮，臉上露出邪惡的表情。",
	"你深情地愛撫著$N。",
	"$n深情地愛撫著$N。",
	"$n深情地愛撫著你",
	"你用雙手在自己身上摸來摸去。",
	"$n用雙手在$e自己身上摸來摸去。"
    },
/*
    {
	"french",
	"Kiss whom?",
	"",
	"You give $N a long and passionate kiss.",
	"$n kisses $N passionately.",
	"$n gives you a long and passionate kiss.",
	"You gather yourself in your arms and try to kiss yourself.",
	"$n makes an attempt at kissing $mself."
    },
*/
    { // by skalt@ROC
        "forget",
        "你滿臉呆滯的環顧四周，像得了失憶症般",
        "$n滿臉呆滯的環顧四周，像得了失憶症般",
        "你裝傻似的對$N說：「你在說什麼，我不記得這回事啊」",
        "$n裝傻似的對$N說：「你在說什麼，我不記得這回事啊」",
        "$n裝傻似的對你說：「你在說什麼，我不記得這回事啊」",
        "你試著回憶起自己遺忘的姓名。",
        "$n喃喃唸著：「我是誰....我到底是誰><」"
    },

    {
	"frown",
	"你懊惱的皺起眉頭。",
	"$n懊惱的皺起眉頭。",
	"你對著$N皺起了眉頭。",
	"$n對著$N皺起了眉頭。",
	"$n對著你皺起了眉頭。",
	"",
	""
    },
/*
    {
	"fume",
	"You grit your teeth and fume with rage.",
	"$n grits $s teeth and fumes with rage.",
	"You stare at $M, fuming.",
	"$n stares at $N, fuming with rage.",
	"$n stares at you, fuming with rage!",
	"That's right - hate yourself!",
	"$n clenches $s fists and stomps his feet, fuming with anger."
    },
*/
    {
	"gasp",
	"你驚訝的說不出話來。",
	"$n驚訝的說不出話來。",
	"你對$N的所作所為感到很訝異。",
	"$n對$N的所作所為感到很訝異。",
	"$n對你的所作所為感到很訝異。",
	"你搔搔頭，覺得自己的所作所為真是莫名奇妙。",
	"$n搔搔頭，覺得$e自己的所作所為真是莫名奇妙。"
    },

    {
	"gf",
	"你望空大喊：「神啊～賜我一個女朋友吧～」",
	"$n望空大喊：「神啊～賜我一個女朋友吧～」",
	"你生氣地對$N喊道：「小子，不淮動我的馬子！」",
	"$n生氣地對$N喊道：「小子，不淮動我的馬子！」",
	"$n生氣地對你喊道：「小子，不淮動我的馬子！」",
	"你生氣地喊道：「那一個傢伙敢動我的馬子！」",
	"$n生氣地喊道：「那一個傢伙敢動我的馬子！」"
    },

	{
	"gift",
	"你從口袋拿出個聖誕襪，翻呀翻的，似乎想從裡面翻出個禮物？！",
	"$n從口袋拿出個聖誕襪，翻呀翻的，似乎想從裡面翻出個禮物？！",
	"你拉著$N的手，對$N撒嬌的說：「好心的$N，送我個禮物好不好嘛？」",
	"$n拉著$N的手，對$N撒嬌的說：「好心的$N，送我個禮物好不好嘛？」",
	"$n拉著你的手，對你撒嬌的說：「好心的$N，送我個禮物好不好嘛？」",
	"你對天大喊：「偉大的天神，送我個禮物吧！」",
	"$n對天大喊：「偉大的天神，送我個禮物吧！」",
	},

    {
	"giggle",
	"你格格的笑了起來。",
	"$n格格的笑了起來。",
	"你對著$N格格的笑。",
	"$n對著$N格格的笑。",
	"$n對著你格格的笑。",
	"",
	""
    },

    {
	"glare",
	"你抬起頭，痴痴地望著天空。",
	"$n抬起頭，痴痴地望著天空。",
	"你睜大眼睛，盯著$N瞧個不停。",
	"$n睜大眼睛，盯著$N瞧個不停。",
	"$n睜大眼睛，盯著你瞧個不停。",
	"你沒辦法看到自己。",
	""
    },

    {
	"god",
	"你突然覺得這世界上一定有神的存在。",
	"$n突然覺得這世界上一定有神的存在。",
	"你突然覺得$N簡直跟神一樣，你對著$E不停地跪拜。",
	"$n突然覺得$N簡直跟神一樣，$e對著$E不停地跪拜。",
	"$n突然覺得你簡直跟神一樣，$e對著你不停地跪拜。",
	"你突然覺得你簡直跟神一樣",
	"$n突然覺得$e簡直跟神一樣"
    },

    {
	"good",
	"你大聲地說：「幹的好！」",
	"$n大聲地說：「幹的好！」",
	"你大聲地對著$N說：「好！」",
	"$n大聲地對著$N說：「好！」",
	"$n大聲地對著你說：「好！」",
	"你覺得自己真是了不起。",
	"$n覺得自己真是了不起。"
    },

    {
	"grin",
	"你露出邪惡的笑容。",
	"$n露出邪惡的笑容。",
	"你對$N露出邪惡的笑容。",
	"$n對$N露出邪惡的笑容。",
	"$n對你露出邪惡的笑容.. 你最好提高警覺喔!!",
	"",
	""
    },

    {
	"groan",
	"你輕輕的呻吟著。",
	"$n輕輕的呻吟著。",
	"你對著$N輕輕的呻吟著。",
	"$n對著$N輕輕的呻吟著。",
	"$n對著你輕輕的呻吟著。",
	"",
	""
    },
/*
    {
	"grope",
	"Whom do you wish to grope?",
	"",
	"Well, what sort of noise do you expect here?",
	"$n gropes $N.",
	"$n gropes you.",
	"You grope yourself - YUCK.",
	"$n gropes $mself - YUCK."
    },
*/
	{
	"gropen",
	"你對天大喊：「現在有沒有團阿？」",
	"$n對天大喊：「現在有沒有團阿？」",
	"你拉著$N的袖子，對$N說：「偉大的Leader $N，開個團好不好？」",
	"$n拉著$N的袖子，對$N說：「偉大的Leader $N，開個團好不好？」",
	"$n拉著你的袖子，對$N說：「偉大的Leader $N，開個團好不好？」",
	"你對天大喊：「我要開團了，快來跟隨我吧！」",
	"$n對天大喊：「我要開團了，快來跟隨我吧！」",
	},
/*
	{
	"grovel",
	"You grovel in the dirt.",
	"$n grovels in the dirt.",
	"You grovel before $M.",
	"$n grovels in the dirt before $N.",
	"$n grovels in the dirt before you.",
	"That seems a little silly to me.",
	""
    },
*/
    {
	"growl",
	"你望空高喊，「喔嗚～」",
	"$n望空高喊，「喔嗚～」",
	"你哼了一聲，說：「可惡的$N，看招！」",
	"$n對$N生氣的哼了一聲，看來馬上就有一場惡鬥。",
	"$n對著你說：「可惡的傢伙，看招！」",
	"你亂吼亂叫，心情舒服了一點。",
	"$n對著$e自己亂吼亂叫，真是莫名奇妙。"
    },

    {
	"grumble",
	"你不停地抱怨著。",
	"$n不停地抱怨著。",
	"你對著$N不停地抱怨著。",
	"$n對著$N不停地抱怨著。",
	"$n對著你不停地抱怨著。",
	"",
	""
    },
/*
    {
	"grunt",
	"GRNNNHTTTT.",
	"$n grunts like a pig.",
	"GRNNNHTTTT.",
	"$n grunts to $N.  What a pig!",
	"$n grunts to you.  What a pig!",
	"GRNNNHTTTT.",
	"$n grunts to nobody in particular.  What a pig!"
    },
*/
    {
	"hand",
	"和誰握手？",
	"",
	"你走向$N，和$E用力的握著手。",
	"$n走向$N，和$E用力的握著手。",
	"$n走了過來，和你用力的握著手。",
	"你無聊的玩著自己的雙手。",
	"$n無聊的玩著$e自己的手。"
    },
/*
    {
	"hangover",
	"You pop a few aspirin and put on your sunglasses.  Ow, your head hurts!",
	"$n holds $s head and says 'Quit breathing so loud!'",
	"Won't $N be quiet?  Your head is gonna split in two if $E keeps talking!",
	"$n complains to $N 'Be quiet!	I have a hangover!",
	"$n complains to you 'Be quiet!	 I have a hangover!",
	"You shoosh yourself.  Be quiet!  You have a hangover!",
	"$n shooshes $mself.  Wow, what a hangover that must be!"
    },
*/
    {
        "happy",
        "你開心的轉著圈圈！向你週遭的人灑花＊*＊\(￣▽￣)/＊*＊",
        "$n開心的轉著圈圈，向身邊的人灑花讓大家都感染了快樂的氣息！",
        "你牽起$N的手開心的轉著圈圈，週圍似乎也感受到快樂的氣息！",
        "$n輕快的拉起$N的小手開心的轉著圈圈！",
        "$n輕快的拉起你的手與你自得其樂的轉著圈圈",
        "你開心的轉著圈圈！向你週遭的人灑花＊*＊\(￣▽￣)/＊*＊",
        "$n你開心的轉著圈圈！向你週遭的人灑花＊*＊\(￣▽￣)/＊*＊"

    },
    {
	"hate",
	"你咬牙切齒，說道：「我好恨～我好恨～」",
	"$n咬牙切齒，說道：「我好恨～我好恨～」",
	"你把$N推開，生氣地說：「我恨你！」",
	"$n把$N推開，生氣地說：「我恨你！」",
	"$n把你推開，生氣地說：「我恨你！」",
	"你瘋狂地笑道：「恨我呀，我最喜歡別人恨我了！」",
	"$n瘋狂地笑道：「恨我呀，我最喜歡別人恨我了！」"
    },

    {
	"hehe",
	"你「嘿嘿嘿～」地奸笑了幾聲。",
	"$n「嘿嘿嘿～」地奸笑了幾聲。",
	"你「嘿嘿嘿～」地對$N奸笑了幾聲。",
	"$n「嘿嘿嘿～」地對$N奸笑了幾聲。",
	"$n「嘿嘿嘿～」地對你奸笑了幾聲。",
	"",
	""
    },

    {
	"hello",
	"你興奮地對所有人打招呼！",
	"$n興奮地對所有人打招呼！",
	"你興奮地對$N說：「嗨！」",
	"$n興奮地對$N說：「嗨！」",
	"$n興奮地對你說：「嗨！」",
	"",
	""
    },

    {
	"help!",
	"你慌張地大叫：「救命呀～救命呀～」",
	"$n慌張地大叫：「救命呀～救命呀～」",
	"你慌張地對$N大叫：「救命呀～快來救我呀～」",
	"$n慌張地對$N大叫：「救命呀～快來救我呀～」",
	"$n慌張地對你大叫：「救命呀～快來救我呀～」",
	"你對大家說：「有什麼問題儘管來找我好了。」",
	"$n對大家說：「有什麼問題儘管來找我好了。」"
    },


    {
	"hkpk",
	"你像發了瘋般的大叫：「你們這些廢柴，全都未夠班與我一戰！」",
	"$n像發了瘋般的大叫：「你們這些廢柴，全都未夠班與我一戰！」",
	"你囂張地對$N說：「口桀口桀口桀～今日我定要將你這廢柴轟殺啦！！」",
	"$n囂張地對$N說：「口桀口桀口桀～今日我定要將你這廢柴轟殺啦！！」",
	"$n囂張地對你說：「口桀口桀口桀～今日我定要將你這廢柴轟殺啦！！」",
	"你抬頭對天高喊：「沒可能！沒可能的！我怎會敗啊！！」",
	"$n抬頭對天高喊：「沒可能！沒可能的！我怎會敗啊！！」",
    },
/*
    {
      "hero",
      "You heroically risk life and limb to save the day.",
      "$n heroically risks life and limb to save the day.",
      "You say to $M, '$N, YOU'RE MY HERO!'",
      "$n says to $N, '$N, YOU'RE MY HERO!'",
      "$n says to you, '$N, YOU'RE MY HERO!'",
      "You try to look heroic.",
      "$n does $s best impression of a hero."
    },

    {
	"highfive",
	"You jump in the air...oops, better get someone else to join you.",
	"$n jumps in the air by $mself.	 Is $e a cheerleader, or just daft?",
	"You jump in the air and give $M a big highfive!",
	"$n jumps in the air and gives $N a big highfive!",
	"$n jumps in the air and gives you a big highfive!",
	"You jump in the air and congratulate yourself!",
	"$n jumps in the air and gives $mself a big highfive!  Wonder what $e did?"
    },

    {
      "holdon",
      "You pause for a moment, hesitant.",
      "$n pauses for a moment, somewhat hesitant.",
      "You hold $M closely, hugging $M.",
      "$n holds $N closely, hugging $M.",
      "$n holds you closely, hugging you.",
      "You have to hold yourself up you're so weary. Go to Sleep!",
      "$n has to hold $mself up $e's so tired. $e should go to bed and get some sleep."
    },
*/
    {
	"hmm",
	"你「嗯」的一聲，一副欲言又止的樣子。",
	"$n「嗯」的一聲，一副欲言又止的樣子。",
	"你對$N「嗯」了一聲。",
	"$n對$N「嗯」了一聲。",
	"$n對你「嗯」了一聲。",
	"",
	""
    },

    {
	"hop",
	"你高興的像個小孩子般地蹦蹦跳跳。",
	"$n高興的像個小孩子般地蹦蹦跳跳。",
	"你高興地跳到$N的身上。",
	"$n高興地跳到$N的身上。",
	"$n高興地跳到你的身上。",
	"",
	""
    },
/*
    {
	"howl",
	"AHOOOOOOOOOOOOooooooooooooo.",
	"$n howls over the smoking corpse.",
	"You howl at $M.",
	"$n howls at $N.  Someone's gonna die!",
	"$n howls at you.  You must be good looking.",
	"You howl at yourself.",
	"$n howls at $mself."
    },
  */  
    {
	"hug",
	"Hug 誰？",
	"",
	"你輕輕地擁著$N。",
	"$n輕輕地擁著$N。",
	"$n輕輕地擁著你。",
	"",
	""
    },

    {
	"hungry",
	"你用最後的一分力氣喊道：「好心的大爺～請賞我一點東西吃吧」",
	"$n用最後的一分力氣喊道：「好心的大爺～請賞我一點東西吃吧」",
	"你對$N無力地喊道：「好心的大爺～請賞我一點東西吃吧」",
	"$n對$N無力地喊道：「好心的大爺～請賞我一點東西吃吧」",
	"$n對你無力地喊道：「好心的大爺～請賞我一點東西吃吧」",
	"你的肚子咕嚕咕嚕地叫的好大聲",
	"$n的肚子咕嚕咕嚕地叫的好大聲"
    },
/*
    {
	"hum",
	"Hmm Hmm Hmm Hmmmmmmm.",
	"$n hums like a bee with a chest cold.",
	"You hum a little ditty for $M.	 Hmm Hmm Hmm Hmmmmmm.",
	"$n hums a little ditty for $N.	 Hmm Hmm Hmm Hmmmmmm.",
	"$n hums a little ditty for you.  Hmm Hmm Hmm Hmmmmmm.",
	"Hmm Hmm Hmmmmmmm.",
	"$n hums like a bee with a chest cold."
    }, 
 
    {
      "icecube",
      "You wonder who would like an ice cube down their shirt.",
      "With a devious look on $s face, $n hides something behind $s back.",
      "You stuff an ice cube down $S shirt. You laugh as $E hops around.",
      "$n stuffs an ice cube down $N's shirt, better hope that was $s last ice cube.",
      "$n stuffs an ice cube down your shirt. Brrr! $n laughs as you struggle to get it out.",
      "You stick an ice cube down your shirt. Brrr!",
      "$n sticks an icecube down $s shirt, trying to cool off."
    },

    {
	"invite",
	"You speak your best French in hopes of getting lucky.",
	"$n tells you, 'Voulez-vous couche avec moi ce soir?'",	 
	"You speak your best French to $M in hopes of getting lucky.",
	"$n tells $N, 'Voulez-vous couche avec moi ce soir?'",
	"$n tells you, 'Voulez-vous couche avec moi ce soir?'",
	"You speak your best French in hopes of getting lucky, with yourself???",
	"$n says to $mself, 'Voulez-vous couche avec moi ce soir?'"
    },
  */
    {
	"idiot",
	"你不屑地說：「哼！一群白痴。」",
	"$n不屑地說：「哼！一群白痴。」",
	"你對$N說：「你真是一個大白痴！」",
	"$n對$N說：「你真是一個大白痴！」",
	"$n對你說：「你真是一個大白痴！」",
	"你流著口水，傻笑著說：「我是白痴～我是白痴啊～」",
	"$n流著口水，傻笑著說：「我是白痴～我是白痴啊～」"
    },

    {
	"idle",
	"你坐在地上，怔怔地發起呆來了。",
	"$n坐在地上，怔怔地發起呆來了。",
	"",
	"",
	"",
	"",
	""
    },

    {
	"inn",
	"你覺得自己好委曲。",
	"$n覺得自己好委曲。",
	"你眼中充滿淚\水，無辜地望著$N。",
	"$n眼中充滿淚\水，無辜地望著$N。",
	"$n眼中充滿淚\水，無辜地望著你。",
	"你覺得自己是世界上最純真的人。",
	"$n覺得自己是世界上最純真的人。"
    },

    {
	"kickass",
	"你練習劈腿。",
	"$n自己在練習劈腿。",
	"你朝著$N的屁股，狠狠踢下去！",
	"$n朝著$N的屁股，狠狠踢下去！",
	"$n朝著你的屁股狠狠踢下去，哇，好痛啊！",
	"你用腳後跟踢自己的屁股，真無聊。",
	"$n用腳後跟踢自己的屁股，看來是無聊瘋了。"
    },

    {
	"kiss",
	"Kiss 誰？",
	"",
	"你深情地看著$N，給了$E深深的一吻。",
	"$n深情地看著$N，給了$E深深的一吻。",
	"$n深情地看著你，給了你深深的一吻。",
	"你親了一下自己的手。",
	"$n親了一下$e自己的手。"
    },


    {
	"kok",
	"你的手高高舉起，好像打算敲什麼東西。",
	"$n的手高高舉起，好像打算敲什麼東西。",
	"你的手高高舉起，往$N的頭上狠狠地敲了下去，打得$E眼冒金星。",
	"$n的手高高舉起，往$N的頭上狠狠地敲了下去，打得$E眼冒金星。",
	"$n的手高高舉起，往你的頭上狠狠地敲了下去，打得你眼冒金星。",
	"你往自己頭上狠狠地敲一下，把自己給打昏了。",
	"$n往自己頭上狠狠地敲一下，把$e給打昏了。"
    },

    {
	"kowtow",
	"你突然想不開，把自己的頭往地上一直撞個不停。",
	"$n突然想不開，把自己的頭往地上一直撞個不停。",
	"你連忙跑到$N跟前，磕頭道：「老大光臨，小弟未曾遠迎，死罪～死罪。」",
	"$n連忙跑到$N跟前，磕頭道：「老大光臨，小弟未曾遠迎，死罪～死罪。」",
	"$n連忙跑到你跟前，磕頭道：「老大光臨，小弟未曾遠迎，死罪～死罪。」",
	"你冷冷地說：「看到我來了，怎麼不來磕頭謝罪？」",
	"$n冷冷地說：「看到我來了，怎麼不來磕頭謝罪？」"
    },

    {
	"lag",
	"你揮動雙手，哇～怎麼這麼慢～",
	"$n緩緩地揮動雙手，哇～怎麼這麼慢～",
	"",
	"",
	"",
	"",
	""
    },

    {
	"laugh",
	"你倒在地上，抱著肚子笑的滾來滾去。",
	"$n倒在地上，抱著肚子笑的滾來滾去。",
	"你指著$N的鼻子大笑。",
	"$n指著$N的鼻子大笑。",
	"$n指著你的鼻子大笑。",
	"你對著自己不停的傻笑。",
	"$n對著$e自己不停的傻笑。"
    },

    {
	"lean",
	"lean 誰？",
	"",
	"你像隻小貓般地依偎在$N的懷裡。",
	"$n像隻小貓般地依偎在$N的懷裡。",
	"$n像隻小貓般地依偎在你的懷裡。",
	"",
	""
    },

    {
	"leave",
	"你急急忙忙地離開了。",
	"$n急急忙忙地離開了。",
	"你飛也似地逃離$N的身旁。",
	"$n飛也似地逃離$N的身旁。",
	"$n飛也似地逃離你的身旁。",
	"",
	""
    },

    {
	"lick",
	"你舔了舔自己的嘴脣。",
	"$n舔了舔$e自己的嘴脣。",
	"你舔了舔$N的臉，弄得$E滿臉口水，好噁哦～",
	"$n舔了舔$N的臉，弄得$E滿臉口水，好噁哦～",
	"$n舔了舔你的臉，弄得你滿臉口水，好噁哦～",
	"你舔了舔自己的手。",
	"$n舔了舔$e的手。"
    },
/*
    {
      "listen",
      "You listen intently.",
      "$n listens intently.",
      "You listen intently to what $E is saying.",
      "$n listens intently to what $N is saying.",
      "$n listens intently to what you are saying.",
      "You pay close attention to what you are saying, trying to avoid saying something you shouldn't.",
      "$n listens intently to $mself. $e must enjoy the sound of $s own voice."
    },
*/
    {
	"lonely",
	"你露出落寞的神情。",
	"$n露出落寞的神情。",
	"你對$N說：「HEY！你寂寞嗎？」",
	"$n對$N說：「HEY！你寂寞嗎？」",
	"$n對你說：「HEY！你寂寞嗎？」",
	"你忽然覺得自己好孤單！",
	"$n忽然覺得自己好孤單！"
    },

	{
	"lostway",
    "你無助地哭喊著：「我迷路了，好心的大哥哥大姐姐快來幫我 Ｑ_Ｑ」",
    "$n無助地哭喊著：「我迷路了，好心的大哥哥大姐姐快來幫我 Ｑ_Ｑ」",
    "你緊緊抱住$N的大腿求助：「拜託！帶我離開這該死的鬼地方 Ｑ_Ｑ」",
    "$n緊緊抱住$N的大腿求助：「拜託！帶我離開這該死的鬼地方 Ｑ_Ｑ」",
    "$n緊緊抱住你的大腿求助：「拜託！帶我離開這該死的鬼地方 Ｑ_Ｑ」",
    "你吐了吐舌頭，裝可愛的說：「嘿嘿....好像迷路了耶 >////<」",
    "$n吐了吐舌頭，裝可愛的說：「嘿嘿....好像迷路了耶 >////<」"
    },

    {
	"love",
	"Love 誰？",
	"",
	"你深情的對著$N說：「我愛你～此情永不移～」",
	"$n深情的對著$N說：「我愛你～此情永不移～」",
	"$n深情的對著你說：「我愛你～此情永不移～」",
	"",
	""
    },
/*
    {
	"marvelous",
	"You say 'Ah dahling, you look MAHVELLOUS!'",
	"$n says 'Ah dahling, you look MAHVELLOUS!'",
	"You say to $M, 'Ah dahling, you look MAHVELLOUS!'",
	"$n says to $N, 'Ah dahling, you look MAHVELLOUS!'",
	"$n says to you, 'Ah dahling, you look MAHVELLOUS!'",
	"You say 'Ah dahling, I look MAHVELLOUS!'",
	"$n says 'Ah dahling, I look MAHVELLOUS!'"
    },
*/
    {
	"massage",
	"Massage 誰？",
	"$n伸出雙手在空中亂抓。",
	"你替$N按摩，把$E弄得很舒服。",
	"$n替$N按摩，把$E弄得很舒服。",
	"$n替你按摩，啊～真舒服～",
	"",
	""
    },
/*
    {
	"meow",
	"MEOW.",
	"$n meows.  What's $e going to do next, wash $mself with $s tongue?",
	"You meow at $M, hoping $E will give you some milk.",
	"$n meows at $N, hoping $E will give $m some milk. ",
	"$n meows at you.  Maybe $e wants some milk.",
	"You meow like a kitty cat.",
	"$n meows like a kitty cat."
    },
*/
    {
	"miss",
	"Miss 誰？",
	"$n就連要想念誰也不知道。",
	"你熱情的衝向$N，對$E說：「好久不見，送你高崗屋。」",
	"$n熱情的衝向$N，對$E說：「好久不見，送你高崗屋。」",
	"$n熱情的衝了過來，對你說：「好久不見，送你高崗屋。」",
	"",
	""
    },

    {
    "mmm",
    "mmmmmmmmmm.......",
    "$n說道 'mmMMmmMMmmMMmm.'",
    "你對$N說道 'mmMMmmMMmmMMmm.'",
    "$n對$N說道 'mmMMmmMMmmMMmm.'",
    "$n對你說道 'mmMMmmMMmmMMmm.'",
    "",
    ""
    },
    
    {
	"moan",
	"你慘叫一聲，對自己的錯誤感到懊惱。",
	"$n慘叫一聲。露出一副很懊惱的表情。",
	"",
	"",
	"",
	"",
	""
    },

    {
	"monster",
	"你驚慌地大叫：「怪物來啦～快逃命啊～」",
	"$n驚慌地大叫：「怪物來啦～快逃命啊～」",
	"你對著$N驚慌地大叫：「怪物來啦～救命啊～」",
	"$n對著$N驚慌地大叫：「怪物來啦～救命啊～」",
	"$n對著你驚慌地大叫：「怪物來啦～救命啊～」",
	"你穿上怪物裝，出來嚇無辜的路人。",
	"$n穿上怪物裝，出來嚇無辜的路人。"
    },
/*
    {
	"mooch",
	"You beg for money, weapons, coins.",
	"$n says 'Spare change?'",
	"You beg $N for money, weapons, coins.",
	"$n begs you for favors of the insidious type...",
	"$n begs you for favors of the insidious type...",
	"You beg for money, weapons, coins.",
	"$n says 'Spare change?'"
    },
    
    {
	"moocow",
	"You make cow noises.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises at $M.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow at $N.",
	"$n looks at you and Mooooooooooooooooooooooooos like a cow.",
	"You make cow noises.  Mooooooooooooooooooo!",
	"$n Mooooooooooooooooooooooooos like a cow."
    },
    
    {
	"moon",
	"Gee your butt is big.",
	"$n moons the entire room.",
	"You show your big butt to $M.",
	"$n shows $s big butt to $N.  Find a paddle, quick!",
	"$n shows $s big ugly butt to you.  How do you like it?",
	"You moon yourself.",
	"$n moons $mself.  Ugliest butt you ever saw."
    },*/
    

  	{
     	"morning",
     	"你很有朝氣地對著大家說：「早 安 ~」",
     	"$n很有朝氣地對著大家說：「早 安 ~」",
     	"你開心地對著$N說一聲早",
     	"$n開心地對$N說：「早 安 ~」",
     	"$n開心地對你說聲早安",
     	"你忽然注意到外面射入的陽光，才發現已經天亮了",
     	"$n突然發現太陽已經出來了"
	},
/*
    {
	"mosh",
	"You MOSH insanely about the room.",
	"$n MOSHES insanely about the room.",
	"You MOSH $M into a pile of Jello pudding.  Ewww!",
	"$n MOSHES $N into Jello pudding.  Ah, the blood!",
	"$n MOSHES you into the ground.",
	"You MOSH yourself.  Ah the blood!",
	"$n MOSHES and MOSHES and MOSHES and MOSHES..."
    },
    
    {
	"muhaha",
	"You laugh diabolically.  MUHAHAHAHAHAHA!",
	"$n laughs diabolically.  MUHAHAHAHAHAHA!",
	"You laugh at $M diabolically.	MUHAHAHAHAHAHA!",
	"$n laughs at $N diabolically.	MUHAHAHAHAHAHA!",
	"$n laughs at you diabolically.	 MUHAHAHAHAHAHA!",
	"Muhaha at yourself??  Wierd.",
	""
    },

    {
	"mwalk",
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.",      
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
	"$n grabs $s black hat and sparkly glove and moonwalks across the room.", 
	"You grab your hat and moonwalk across the room.",
	"$n grabs $s black hat and sparkly glove and moonwalks across the room."
    },

    {
	"nail",
	"You nibble nervously on your nails.",
	"$n nibbles nervously on $s fingernails.",
	"You nibble nervously on your nails.",
	"$n nibbles nervously on $s fingernails.",
	"$n nibbles nervously on your fingernails.  Yuck!",
	"You nibble nervously on your nails.",
	"$n nibbles nervously on $s fingernails."
    },
    
    {
	"nasty",
	"You do the best imitation of the nasty you can.",
	"$n does the nasty solo? -- wow.",
	"You do the nasty with $M.",
	"$n does the nasty with $N.  Find a firehose quick!",
	"$n does the nasty with you.  How do you like it?",
	"You do the nasty with yourself.",
	"$n does the nasty with $mself.	 Hoo boy."
    },
    
    {
	"ni",
	"You exclaim proudly, 'I am a knight that goes NI!'",
	"$n exclaims proudly, 'I am a knight that goes NI!'",
	"You exclaim proudly to $M, 'I am a knight that goes NI!'",
	"$n exclaims proudly to $N, 'I am a knight that goes NI!'",
	"$n proudly exclaims to you, 'I am a knight that goes NI!'",
	"You exclaim proudly, 'I am a knight that goes NI!'",
	"$n exclaims proudly, 'I am a knight that goes NI!'"
    },
*/
    {
	"nibble",
	"Nibble 誰？",
	"",
	"你輕輕地咬著$N的耳朵。",
	"$n輕輕地咬著$N的耳朵。",
	"$n輕輕地咬著你的耳朵。",
	"你咬不到自己的耳朵。",
	""
    },

    {
	"no!",
	"你絕望地對天大叫：「不～」",
	"$n絕望地對天大叫：「不～」",
	"你對$N叫道：「不要啊～你不可以這樣啊～」",
	"$n對$N叫道：「不要啊～你不可以這樣啊～」",
	"$n對你叫道：「不要啊～你不可以這樣啊～」",
	"你叫道：「不要～不要～不要停～」",
	"$n叫道：「不要～不要～不要停～」"
    },

    {
	"nod",
	"你點了點頭。",
	"$n點了點頭。",
	"你對$N點了點頭。",
	"$n對$N點了點頭。",
	"$n對你點了點頭。",
	"",
	""
    },
/*
    {
      "noogie",
      "You look around, wondering who would like to get a noogie.",
      "$n looks around, wondering who would like to get a noogie.",
      "You put $M in a headlock and give $M a severe noogie.",
      "$n puts $N in a headlock and gives $M a noogie.",
      "$n puts you in a headlock and gives you a severe noogie.",
      "You give yourself a noogie. What a goof!",
      "$n gives $mself a noogie. What a goof!"
    },

    {
	"nose",
	"Gee your nose is big.",
	"$n noses around in your business.",
	"You tweak $S nose.",
	"$n tweaks $N's nose.  Ow!",
	"$n tweaks your nose.  Ow!",
	"You wiggle your nose.",
	"$n wiggles $s nose."
    },

   {
	"nudge",
	"Nudge whom?",
	"",
	"You nudge $M.",
	"$n nudges $N.",
	"$n nudges you.",
	"You nudge yourself, for some strange reason.",
	"$n nudges $mself, to keep $mself awake."
    },

    {
	"nuke",
	"Who do you want to nuke?",
	"$n tries to put something in the microwave.",
	"You envelop $M in a mushroom cloud.",
	"$n detonates a nuclear warhead at $N.	Uh oh!",
	"$n envelops you in a mushroom cloud.",
	"You nuke yourself.",
	"$n puts $mself in the microwave."
    },
*/
    {
	"nuzzle",
	"Nuzzle 誰？",
	"",
	"你用鼻子在$N的身上蹭來蹭去。",
	"$n用鼻子在$N的身上蹭來蹭去。",
	"$n用鼻子在你的身上蹭來蹭去。",
	"",
	""
    },
/*
    {
	"ogle",
	"Whom do you want to ogle?",
	"",
	"You ogle $M like $E was a piece of meat.",
	"$n ogles $N.  Maybe you should leave them alone for awhile?",
	"$n ogles you.	Guess what $e is thinking about?",
	"You ogle yourself.  You may just be too wierd for this mud.",
	"$n ogles $mself.  Better hope that $e stops there."
    },

    {
	"ohno",
	"Oh no!	 You did it again!",
	"Oh no!	 $n did it again!",
	"You exclaim to $M, 'Oh no!  I did it again!'",
	"$n exclaims to $N, 'Oh no!  I did it again!'",
	"$n exclaims to you, 'Oh no!  I did it again!'",
	"You exclaim to yourself, 'Oh no!  I did it again!'",
	"$n exclaims to $mself, 'Oh no!	 I did it again!'"
    },
    
    {
	"oink",
	"You make pig noises.  OINK!",
	"$n oinks 'OINK OINK OINK!'",
	"You make pig noises at $M.  OINK!",
	"$n oinks at $N: 'OINK OINK OINK!'",
	"$n oinks at you: 'OINK OINK OINK!'",
	"You make pig noises.  OINK!",
	"$n oinks 'OINK OINK OINK!'"
    },
 
    {
	"ooo",
	"You go ooOOooOOooOOoo.",
	"$n says, 'ooOOooOOooOOoo.'",
	"You go ooOOooOOooOOoo.",
	"$n says, 'ooOOooOOooOOoo.'",
	"$n thinks of you and says, 'ooOOooOOooOOoo.'",
	"You go ooOOooOOooOOoo.",
	"$n says, 'ooOOooOOooOOoo.'"
    },
*/
	{
	"ok!",
	"你做了個OK的手勢，說了聲：「沒問題！」",
	"$n做了個OK的手勢，說了聲：「沒問題！」",
	"你對著$N做了個OK的手勢，對$N說：「沒問題！」",
	"$n對著$N做了個OK的手勢，對$N說：「沒問題！」",
	"$n對著你做了個OK的手勢，對你說：「沒問題！」",
	"你做了個OK的手勢，仰天長笑：「我就知道沒有事情難得倒我的！」",
	"$n做了個OK的手勢，仰天長笑：「我就知道沒有事情難得倒我的！」",
	},

    {
	"ooo",
	"你蹲在牆角，滿臉委屈地在地上畫圈圈。",
	"$n蹲在牆角，滿臉委屈地在地上畫圈圈。",
	"你蹲在牆角，背對著$N在地上畫圈圈。",
	"$n蹲在牆角，背對著$N在地上畫圈圈。",
	"$n蹲在牆角，背對著你在地上畫圈圈。",
	"",
	""
    },
    {
        "opost",
        "你痛苦的抱著頭仰天長嘯：「老梗！！這是一個老梗啊！」",
        "$n痛苦的抱著頭仰天長嘯：「老梗！！這是一個老梗啊！」",
        "你摀住$N的嘴緊張地看了看四周說：「這兒多鄉民，老梗莫再提！」",
        "$n摀住$N的嘴緊張地看了看四周說：「這兒多鄉民，老梗莫再提！」",
        "$n摀住你的嘴緊張地看了看四周說：「這兒多鄉民，老梗莫再提！」",
        "你驚疑不定的喃喃自語：「我又老梗了嗎.....？」",
        "$n驚疑不定的喃喃自語：「我又老梗了嗎.....？」"
		},
	{
	"orz",
	"你一時無法接受這個打擊，失意的彎下身去 Ｏｒｚ ",
	"$n似乎受到很大的傷害，灰暗的將身體彎曲成 Ｏｒｚ 的姿勢",
	"你發現自己跟$N的水準實在差太多了，不自禁的擺\出了 Ｏｒｚ 的姿勢",
	"$n發現自己跟$N的水準實在差太多了，不自禁的擺\出了 Ｏｒｚ 的姿勢",
	"$n發現自己跟你的水準實在差太多了，只得擺\出敗者標誌 Ｏｒｚ ，哇哈哈",
	"你傷心難過痛苦不堪的，擺\出了失意體前彎的動作 Ｏｒｚ ",
	"$n傷心難過痛苦不堪的，擺\出了失意體前彎的動作 Ｏｒｚ "
	},
    {
	"ouch",
	"你大叫：「好痛啊～痛死人啦～」",
	"$n大叫：「好痛啊～痛死人啦～」",
	"",
	"",
	"",
	"你痛的在地上打滾。",
	"$n痛的在地上打滾。"
    },

    {
	"pat",
	"Pat 誰？",
	"",
	"你輕輕拍拍$N的頭。",
	"$n輕輕拍拍$N的頭。",
	"$n輕輕拍拍你的頭。",
	"你摸摸自己的頭。",
	"$n摸摸$e自己的頭。"
    },
    {
	"pig",
	"你說：「好豬哦！」",
	"$n說：「好豬哦！」",
	"你覺得$N簡直笨的像頭豬。",
	"$n覺得$N簡直笨的像頭豬。",
	"$n覺得你簡直笨的像頭豬。",
	"你覺得自己笨的像頭豬。",
	"$n覺得自己笨的跟豬一樣。"
    },

    /*{
	"pinch",
	"You toss a pinch of salt over your shoulder.",
	"$n tosses a pinch of salt over $s shoulder.",
	"You pinch $S rosy cheeks.",
	"$n pinches $N's rosy cheeks.",
	"$n pinches your chubby cheeks.",
	"You need a pinch of salt.",
	"$n needs a pinch of salt."
    },

    {
	"ping",
	"Ping who?",
	"",
	"You ping $N.  $N is unreachable.",
	"$n pings $N.  $N is unreachable.",
	"$n pings you.	Is your server dead or alive?",
	"You ping yourself. Ooof!",
	"$n pings $mself. Ooof!"
    },
*/
    { //by Zeel@ROC
        "pili",
        "你被突如其來的閃電打的全身漆黑！",
        "$n召喚天雷進行無差別攻擊！",
        "你把$N打的漆黑一片！",
        "$n召喚天雷把$N打得是漆黑一片！",
        "$n對你施放天雷召喚，打得你漆黑一片！",
        "你試著召喚天雷攻擊自己。",
        "$n試著對自己施以雷電攻擊？"
    },
    {
	"pk",
	"你像發了瘋般的大叫：「那一個不要命的，給我滾出來！」",
	"$n像發了瘋般的大叫：「那一個不要命的，給我滾出來！」",
	"你對$N說：「小子，明年今天就是你的忌日，看招！」",
	"$n對$N說：「小子，明年今天就是你的忌日，看招！」",
	"$n對你說：「小子，明年今天就是你的忌日，看招！」",
	"你拿出一把刀，朝自己的身上刺了進去，嘻，死了！",
	"$n拿出一把刀，朝$e自己身上刺了進去，嘻，死了！"
    },

    {
	"point",
	"Point 誰？",
	"",
	"你指著$N。",
	"$n指著$N。",
	"$n指著你。",
	"你指著自己。",
	"$n指著$e自己。"
    },

    {
	"poke",
	"Poke 誰？",
	"",
	"你無聊地戳了$N一下。",
	"$n無聊地戳了$N一下。",
	"$n無聊地戳了你一下。",
	"你戳了你自己一下。",
	"$n戳了$e自己一下。"
    },

/* I will use 'think' to replace 'ponder'   */
   {
	"ponder",
	"你低頭沉思.",
	"$n 低頭沉思.",
	"你認真考慮$N的問題。",
	"$n認真考慮$N的問題。",
	"$n認真考慮你的問題。",
	"你認真考慮著自己的未來。",
	"$n認真考慮著$e的未來。"
    },
/*
    {
	"possum",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD.",
	"$n hits the ground... DEAD.",
	"You do your best imitation of a corpse.",
	"$n hits the ground... DEAD."
    },
    
    {
	"potato",
	"You juggle a hot potato in your hands.",
	"$n juggles a hot potato in $s hands.",
	"You juggle a hot potato and pass it onto $M.",
	"$n juggles a hot potato and gives it to $N!",
	"$n juggles a hot potato and gives it to YOU!",
	"You juggle a hot potato in your hands.",
	"$n juggles a hot potato in $s hands."
    },
    
    {
	"poupon",
	"You say 'Pardon me, do you have any Grey Poupon?'.",
	"$n says 'Pardon me, do you have any Grey Poupon?'",
	"You say to $M, 'Pardon me, do you have any Grey Poupon?'.",
	"$n says to $N, 'Pardon me, do you have any Grey Poupon?'",
	"$n says to you, 'Pardon me, do you have any Grey Poupon?'",
	"You say 'Pardon me, do you have any Grey Poupon?'.",
	"$n says 'Pardon me, do you have any Grey Poupon?'"
    },
*/
    {
	"pout",
	"你不高興的翹起嘴巴。",
	"$n不高興的翹起嘴巴。",
	"",
	"",
	"",
	"",
	""
    },
/*
    {
	"pray",
	"You feel righteous, and maybe a little foolish.",
	"$n begs and grovels to the powers that be.",
	"You crawl in the dust before $M.",
	"$n falls down and grovels in the dirt before $N.",
	"$n kisses the dirt at your feet.",
	"Talk about narcissism ...",
	"$n mumbles a prayer to $mself."
    },
    {	
	"pretend",
	"You pretend you are a GOD, and slay everyone in sight!",
	"$n is pretending $e is an implementor again.  *sigh*",
	"You pretend you are a GOD, and demote $M to level 1.",
	"$n pretends $e is a GOD, and says, '$N, you're demoted to level 1!'",
	"$n pretends $e is a GOD, and says, 'You are demoted to level 1!'",
	"You pretend you are an implementor, and you demote yourself to level 1.",
	"$n pretends $e is a GOD, and demotes $mself to level 1."
    },

    {
	"propose",
	"You propose a contract game of bridge.",
	"$n proposes a contract game of bridge.",
	"You propose marriage to $M.",
	"$n gets down on one knee and proposes to $N.",
	"$n says quietly 'Will you marry me?'",
	"Even you wouldn't marry yourself.",
	"$n refuses the proposal made by $mself."
    },
    */
    {
	"puke",
	"你臉色一陣青一陣白，突然「哇」的一聲，吐了一地。",
	"$n臉色一陣青一陣白，突然「哇」的一聲，吐了一地。",
	"你臉色一陣青一陣白，突然「哇」的一聲，吐了$N一身。",
	"$n臉色一陣青一陣白，突然「哇」的一聲，吐了$N一身。",
	"$n臉色一陣青一陣白，突然「哇」的一聲，吐了你一身。",
	"你臉色一陣青一陣白，突然「哇」的一聲，吐了自己一身。",
	"$n臉色一陣青一陣白，突然「哇」的一聲，吐了$e自己一身。"
    },

    {
	"pull",
	"Pull 誰？",
	"",
	"你拉著$N的袖子，捨不得讓$E離開。",
	"$n拉著$N的袖子，捨不得讓$E離開。",
	"$n拉著你的袖子，捨不得讓你離開。",
	"你拉了拉自己的衣袖。",
	"$n拉了拉$e自己的衣袖。"
    },

    {
	"punch",
	"Punch 誰？",
	"",
	"你一拳打在$N的肚子上，爽啊！",
	"$n一拳打在$N的肚子上。",
	"$n一拳打在你的肚子上，好痛！",
	"你反手一拳，把自己打得倒在地上。",
	"$n反手一拳，把$e自己打得倒在地上。"
    },
/*
    {
	"purr",
	"MMMMEEEEEEEEOOOOOOOOOWWWWWWWWWWWW.",
	"$n purrs contentedly.",
	"You purr contentedly in $M lap.",
	"$n purrs contentedly in $N's lap.",
	"$n purrs contentedly in your lap.",
	"You purr at yourself.",
	"$n purrs at $mself.  Must be a cat thing."
    },

    {
      "push",
      "You push the history-eraser button! BUT, nothing seems to happen.",
      "$n pushes the history-eraser button! BUT, nothing seems to happen.",
      "You push $M away in disgust.",
      "$n pushes $N away in disgust.",
      "$n pushes you away in disgust.",
      "You push yourself to the limit of your abilities.",
      "$n pushes $mself to the limits of $s abilites."
    },
*/
    {
	"quick",
	"你著急的說：「快點呀，不然就來不及了。」",
	"$n著急的說：「快點呀，不然就來不及了。」",
	"你狠狠地催促著$N：「你動作快點好不好啊！」",
	"$n狠狠地催促著$N：「你動作快點好不好啊！」",
	"$n狠狠地催促著你：「你動作快點好不好啊！」",
	"你對自己的反應迅速感到很滿意。",
	"$n對$e自己的反應迅速感到很滿意。"
    },

    {
	"rice",
	"你大叫：「米缸～米缸～給我米缸其餘免談！」",
	"$n大叫：「米缸～米缸～給我米缸其餘免談！」",
	"你拉著$N的衣角，對$e說：「$N $N，給我米一下吧～」",
	"$n拉著$N的衣角，對$e說：「$N $N，給我米一下吧～」",
	"$n拉著你的衣角，對你說：「$N $N，給我米一下吧～」",
	"你對大家宣布：「要米我的就來吧！」",
	"$n對大家宣布：「要米我的就來吧！」",
    },
/*
    {
	"raise",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"You raise your hand in response.",
	"$n raises $s hand in response.",
	"$n raises $s hand in response to you.",
	"You raise your hand in response.",
	"$n raises $s hand in response."
    },
   
    {
      "reassure",
      "You reassure everyone that everything will be ok.",
      "$n reassures everyone that everything will be ok.",
      "You hug $M reassuringly.",
      "$n hugs $N reassuringly.",
      "$n hugs you reassuringly.",
      "You try to reassure yourself that everything will be ok.",
      "$n tries to reassure $mself that everything will be ok."
    },

    {	
	"renandstimpy",
	"You say, 'Oh Happy Happy, Joy Joy!'",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!'",
	"You exclaim, 'Oh Happy Happy, Joy Joy!' at the mere thought of $M.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' as $e sees $N enter the room.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' when $e sees you approach.",
	"You exclaim, 'Oh Happy Happy, Joy Joy!' at the thought of yourself.",
	"$n exclaims, 'Oh Happy Happy, Joy Joy!' at the thought of $mself."
    },	

    {
	"rofl",
	"You roll on the floor laughing hysterically.",
	"$n rolls on the floor laughing hysterically.",
	"You laugh your head off at $S remark.",
	"$n rolls on the floor laughing at $N's remark.",
	"$n can't stop laughing at your remark.",
	"You roll on the floor and laugh at yourself.",
	"$n laughs at $mself.  Join in the fun."
    },
    */
    {
	    "roll",
	    "你轉了轉你的眼睛 @_@ 。",
	    "$n的眼睛轉了一圈 @_@ 。",
	    "",
	    "",
	    "",
	    "",
	   "" 
	},

/*
    {
	"rub",
	"You rub your eyes.  How long have you been at this?",
	"$n rubs $s eyes.  $n must have been playing all day.",
	"You rub your eyes.  Has $N been playing as long as you have?",
	"$n rubs $s eyes.  $n must have been playing all day.",
	"$n rubs $s eyes.  Have you been playing as long as $m?",
	"You rub your eyes.  How long have you been at this?",
	"$n rubs $s eyes.  $n must have been playing all day."
    },
*/
    {
	"ruffle",
	"你撥了撥被風吹亂的頭髮。",
	"$n撥了撥被風吹亂的頭髮。",
	"你替$N撥了撥被風吹亂的頭髮。",
	"$n替$N撥了撥被風吹亂的頭髮。",
	"$n替你撥了撥被風吹亂的頭髮。",
	"",
	""
    },
/*
    {
	"russian",
	"You speak Russian.  Yeah, right.  Dream on.",
	"$n says 'Ya horosho stari malenky koshka.'  Huh?",
	"You speak Russian to $M.  Yeah, right.	 Dream on.",
	"$n says to $N 'Ya horosho stari malenky koshka.'  Huh?",
	"$n says to you 'Ya horosho stari malenky koshka.'  Huh?",
	"You speak Russian.  Yeah, right.  Dream on.",
	"$n says 'Ya horosho stari malenky koshka.'  Huh?"
    },
    */
    {
	"sad",
	"你難過地掉下淚\來。",
	"$n難過地掉下眼淚\。",
	"你望著$N，心中一陣酸楚，眼淚\不禁奪眶而出。",
	"$n神情難過的望著$N，眼淚\奪眶而出。",
	"$n神情難過的望著你，眼淚\奪眶而出。",
	"",""
    },

    {
	"salute",
	"你瀟灑地行了個舉手禮。",
	"$n瀟灑地行了個舉手禮。",
	"你對$N行了個舉手禮。",
	"$n對$N行了個舉手禮。",
	"$n對你行了個舉手禮。",
	"Huh?",
	""
    },
/*
    {
	"scold",
	"You scold everyone for being so naughty!",
	"$n scolds everyone for being so naughty!",
	"You scold $M for being so naughty!",
	"$n scolds $N for being so naughty!",
	"$n scolds you for being so naughty!",
	"You scold yourself for being so naughty!",
	"$n scolds $mself for being so naughty!"
    },
*/
    {
	"scream",
	"你大叫：「啊～～～～～」",
	"$n大聲地尖叫！",
	"",
	"",
	"",
	"",
	""
    },

	{
    "silent",
    "你大喝一聲；「大家安靜！」",
    "$n大喝一聲：「大家安靜！」",
    "你小聲的對$N說；「請安靜一下好嗎？」",
    "$n小聲的對$N說；「請安靜一下好嗎？」",
    "$n小聲的對你說；「請安靜一下好嗎？」看來你該檢討一下了",
    "你捂住自己的嘴巴，好像說了什麼不該說的話。",
    "$n捂住自己的嘴巴，好像說了什麼不該說的話。",
	},

    {
	"shake",
	"你搖了搖頭。",
	"$n搖了搖頭。",
	"你對$N搖了搖頭。",
	"$n對$N搖了搖頭。",
	"$n對你搖了搖頭。",
	"",
	""
    },

    {
	"shiver",
	"你冷的發抖！",
	"$n冷的發抖！",
	"你害怕$N而發抖。",
	"$n害怕$N而發抖。",
	"$n對著你直發抖，看起來好像很怕你。",
	"你縮著身子，發抖著說：「人家好怕好怕哦～」",
	"$n縮著身子，發抖著說：「人家好怕好怕哦～」"
    },

    {
	"shrug",
	"你無奈地聳了聳肩。",
	"$n無奈地聳了聳肩。",
	"你對著$N無奈地聳了聳肩。",
	"$n對著$N無奈地聳了聳肩。",
	"$n對著你無奈地聳了聳肩。",
	"",
	""
    },

    {
	"sigh",
	"你長長的嘆了一口氣，「唉～～」",
	"$n長長的嘆了一口氣。",
	"你對著$N長長的嘆了一口氣。",
	"$n對著$N長長的嘆了一口氣。",
	"$n對著你長長的嘆了一口氣。",
	"",
	""
    },

    {
	"sing",
	"你高興地哼起了小調.",
	"$n高聲地唱起歌來.",
	"你對$N愉快的哼著那首動聽的小調.",
	"$n對$N唱著：「你快樂嗎？我很快樂！」",
	"$n對你唱著：「你快樂嗎？我很快樂！」",
	"你一邊作其他事，一邊對自己哼著歌。",
	"$n一邊作其他事，一邊對自己哼著歌。"
    },

    {
	"slap",
	"Slap 誰？",
	"",
	"你狠狠地打了$N幾個耳光，打得$E整張臉都腫了起來。",
	"$n狠狠地打了$N幾個耳光，打得$E整張臉都腫了起來。",
	"$n狠狠地打了你幾個耳光，打得你整張臉都腫了起來。",
	"你狠狠地打了自己幾個耳光，確定自己不是在作夢。",
	"$n狠狠地打了自己幾個耳光，確定$e不是在作夢。"
    },
/*
    {
	"slobber",
	"You slobber all over the floor.",
	"$n slobbers all over the floor.",
	"You slobber all over $M.",
	"$n slobbers all over $N.",
	"$n slobbers all over you.",
	"You slobber all down your front.",
	"$n slobbers all over $mself."
    },
*/
	{
     "strangle",
     "你舉起雙手不知道想掐誰的脖子？",
     "$n舉起雙手不知道是不是想掐你的脖子？",
     "你舉起雙手用力的掐著$N的脖子，好像要勒死人了",
	 "$n舉起雙手用力的掐著$N的脖子，好像要勒死人了",
     "$n舉起雙手用力的掐著你的脖子，讓你快喘不過氣了",
     "你舉起雙手用力的掐著自己的脖子想要自盡",
     "$n舉起雙手用力的掐著自己的脖子似乎要自盡？"
	},

    {
	"smile",
	"你愉快的笑著。",
	"$n愉快的笑著。",
	"你對$N愉快的微笑著。",
	"$n對$N露出愉快的神情。",
	"$n對你愉快的笑著。",
	"",
	""
    },
{
           "smoking",
           "你輕輕吸了口煙，吐了個煙圈，眼神飄向遠方...",
           "$n輕輕吸了口煙，吐了個煙圈，眼神飄向遠方...",
           "你幫$N點上了菸，也給了自己一根，吐著煙雲。",
           "$n幫$N點上了菸，也給了自己一根，吐著煙雲。",
           "$n幫你點了嘴上的菸，對你微微一笑，一切盡在不言中...",
           "你倚著牆腳，點起手中最後一根菸，痴痴地看著天空...",
           "$n孤獨地倚在牆邊，看著遠方，點起手中最後一根菸..."
       },
    {
	"smirk",
	"你不屑地笑了一聲。",
	"$n不屑地笑了一聲。",
	"你覺得$N的話真是可笑。",
	"$n覺得$N的話真是可笑。",
	"$n覺得你的話真是可笑。",
	"你覺得你自己看起來很可笑。",
	"$n看起來很可笑。"
    },
    {
	"sm!",
	"你從身上拿出皮鞭、蠟蠋，開始尋找對象....",
	"$n從$m身上拿出皮鞭和蠟蠋，臉上露出邪惡的笑容。",
	"你狠狠地鞭打著$N：「快！叫我女王！」",
	"$n狠狠地鞭打著$N，一邊大喊著：「快！叫我女王！」",
	"$n狠狠地鞭打著你，還要你叫$m女王。",
	"",
	""
    }, 

/*
    {
	"smoke",
	"You calmly light a cigarette and take a puff.",
	"$n calmly lights a cigarette and take a puff.",
	"You blow smoke into $S eyes.",
	"$n blows smoke into $N's eyes.",
	"$n blows smoke rings into your eyes.",
	"You call down lightning and SMOKE yourself.",
	"$n calls down lightning and SMOKES $mself."
    },
	  
    {
	"smooch",
	"You are searching for someone to smooch.",
	"$n is looking for someone to smooch.",
	"You give $M a nice, wet smooch.",
	"$n and $N are smooching in the corner.",
	"$n smooches you passionately on the lips.",
	"You smooch yourself.",
	"$n smooches $mself.  Yuck."
    },
    
    {
	"snap",
	"PRONTO ! You snap your fingers.",
	"$n snaps $s fingers.",
	"You snap back at $M.",
	"$n snaps back at $N.",
	"$n snaps back at you!",
	"You snap yourself to attention.",
	"$n snaps $mself to attention."
    },

   {
	"snap",
	"PRONTO ! You snap your fingers.",
	"$n snaps $s fingers.",
	"You snap back at $M.",
	"$n snaps back at $N.",
	"$n snaps back at you!",
	"You snap yourself to attention.",
	"$n snaps $mself to attention."
    },

    {
	"snarl",
	"You grizzle your teeth and look mean.",
	"$n snarls angrily.",
	"You snarl at $M.",
	"$n snarls at $N.",
	"$n snarls at you, for some reason.",
	"You snarl at yourself.",
	"$n snarls at $mself."
    },
*/
    {
	"sneeze",
	"你打了一個噴嚏，「哈啾～」",
	"$n打了一個噴嚏。",
	"",
	"",
	"",
	"",
	""
    },

    {
	"snicker",
	"你躲在一旁偷偷的笑。",
	"$n躲在一旁偷偷的笑。",
	"你背對著$N，躲在一旁偷偷的笑。",
	"$n背對著$N，躲在一旁偷偷的笑。",
	"$n背對著你，躲在一旁偷偷的笑。",
	"你突然有了一個邪惡的念頭，於是偷偷的笑了起來。",
	"$n突然偷偷笑了起來，一定有什麼不對勁。"
    },

    {
	"sniff",
	"你低聲啜泣。",
	"$n低聲啜泣。",
	"你對著$N不停地哭哭啼啼。",
	"$n對著$N不停地哭哭啼啼。",
	"$n對著你不停地哭哭啼啼。",
	"你一個人躲在黑暗的角落，一副爺爺不疼，姥姥不愛的樣子。",
	"$n一個人躲在黑暗的角落，一副爺爺不疼，姥姥不愛的樣子。"
    },

    {
	"snore",
	"你大聲的打呼：「Zzzzzzzzzzzzzzzzz」",
	"$n大聲的打呼。",
	"",
	"",
	"",
	"",
	""
    },

    {
	"snowball",
	"Snowball 誰？",
	"",
	"你朝$N臉上丟了一顆雪球。",
	"$n朝$N臉上丟了一顆雪球。",
	"$n朝你臉上丟了一顆雪球。",
	"你朝自己臉上丟了一顆雪球。",
	"$n朝$e自己臉上丟了一顆雪球。"
    },

    {
	"snuggle",
	"你揉揉眼睛，一副想睡覺的樣子。",
	"$n揉揉眼睛，一副想睡覺的樣子。",
	"",
	"",
	"",
	"",
	""
    },

    {
	"sob",
	"你輕聲啜泣。",
	"$n不停地哭哭啼啼。",
	"你靠在$N懷裡不停地哭哭啼啼。",
	"$n靠在$N懷裡不停地哭哭啼啼。",
	"$n靠在你懷裡不停地哭哭啼啼。",
	"",""
    },
    {
	"soga",
	"你點了點頭，露出恍然大悟的表情。",
	"$n點了點頭，臉上一副「原來如此」的表情。",
	"",
	"",
	"",
	"",
	""
    },
    {
	"sorry",
	"你感到很抱歉。",
	"$n感到很抱歉。",
	"你對$N鄭重的道歉。",
	"$n對$N鄭重的道歉。",
	"$n對你鄭重的道歉。",
	"你覺得你愧\對全世界。",
	"$n覺得自己愧\對全世界。"
    },

    {
	"spank",
	"Spank 誰？",
	"",
	"你狠狠地朝$N的屁股打了一下，痛的$E跳了起來。",
	"$n狠狠地朝$N的屁股打了一下，痛的$E跳了起來。",
	"$n狠狠地朝你的屁股打了一下，痛的你跳了起來。",
	"你揍了一下自己的屁股，好痛！",
	"$n揍了一下自己的屁股。"
    },
/*
    {
	"spin",
	"You twirl in a graceful pirouette.",
	"$n twirls in a graceful pirouette.",
	"You spin $M on one finger.",
	"$n spins $N on $s finger.",
	"$n spins you around on $s finger.",
	"You spin yourself around and around and around....",
	"$n spins $mself around and around and around..."
    },
    
    {
	"squeal",
	"You squeal with delight.",
	"$n squeals with delight.",
	"You squeal at $M.",
	"$n squeals at $N.  Wonder why?",
	"$n squeals at you.  You must be doing something good.",
	"You squeal at yourself.",
	"$n squeals at $mself."
    },
    
    {
	"squeeze",
	"Where, what, how, whom?",
	"",
	"You squeeze $M fondly.",
	"$n squeezes $N fondly.",
	"$n squeezes you fondly.",
	"You squeeze yourself - try to relax a little!",
	"$n squeezes $mself."
    },

    {
	"squirm",
	"You squirm guiltily.",
	"$n squirms guiltily.  Looks like $e did it.",
	"You squirm in front of $M.",
	"$n squirms in front of $N.",
	"$n squirms in front of you.  You make $m nervous.",
	"You squirm and squirm and squirm....",
	"$n squirms and squirms and squirm....."
    },
    
    {
	"squish",
	"You squish your toes into the sand.",
	"$n squishes $s toes into the sand.",
	"You squish $M between your legs.",
	"$n squishes $N between $s legs.",
	"$n squishes you between $s legs.",
	"You squish yourself.",
	"$n squishes $mself.  OUCH."
    },
*/
    {
	"stare",
	"你用一種異樣的眼神掃視著在場的每一個人。",
	"$n用一種異樣的眼神掃視著在場的每一個人。",
	"你突然轉過頭去，朝$N狠狠瞪了一眼。",
	"$n突然轉過了頭，朝$N狠狠瞪了一眼。",
	"$n突然轉過頭來，朝你狠狠瞪了一眼。",
	"你瞧著自己在鏡中的模樣，為自己的美貌所陶醉。",
	"$n瞧著自己在鏡中的模樣，為$e自己的美貌所陶醉。"
    },
/*
    {
	"stickup",
	"You don a mask and ask for everyone's gold.",
	"$n says 'This is a stickup.  Gimme yer gold, NOW!'",
	"You don a mask and ask for $S gold.",
	"$n says to $N, 'This is a stickup.  Gimme yer gold, NOW!'",
	"$n says to you, 'This is a stickup.  Gimme yer gold, NOW!'",
	"You extort money from yourself.",
	"$n holds $s weapon to $s throat and says 'Ok, me, give me all my money!'"
    },
   
    {
	"stretch",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"$n stretches luxuriously.  Make you want to, doesn't it.",
	"You stretch and relax your sore muscles.",
	"$n stretches luxuriously.  Make you want to, doesn't it."
    },
    
    {
	"strip",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively around the room.",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively at $N.",
	"$n shows $s bare shoulder and glances seductively at you.",
	"You show some of your shoulder as you begin your performance.",
	"$n shows $s bare shoulder and glances seductively around the room."
    },

    {
	"strut",
	"Strut your stuff.",
	"$n struts proudly.",
	"You strut to get $S attention.",
	"$n struts, hoping to get $N's attention.",
	"$n struts, hoping to get your attention.",
	"You strut to yourself, lost in your own world.",
	"$n struts to $mself, lost in $s own world."
    },

    {
	"suffer",
	"No xp again?  You suffer at the hands of fate.",
	"$n is suffering.  Looks like $e can't seem to level.",
	"You tell $M how you suffer whenever you're away from $M.",
	"$n tells $N that $e suffers whenever they're apart.",
	"$n tells you that $e suffers whenever you're apart.",
	"No xp again?  You suffer at the hands of fate.",
	"$n is suffering.  Looks like $e can't seem to level."
    },	
    
    {
	"sulk",
	"You sulk.",
	"$n sulks in the corner.",
	"",
	"",
	"",
	"",
	""
    },

    {
	"surf",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave.",
	"You stoke your soul by catching a smooth, perfect wave.",
	"$n stokes $s soul by catching a smooth, perfect wave."
    },
    */
    {
	"sweat",
	"你擦了擦額頭上的汗水。",
	"$n擦了擦額頭上的汗水。",
	"你替$N擦了擦額頭上的汗水。",
	"$n替$N擦了擦額頭上的汗水。",
	"$n替你擦了擦額頭上的汗水。",
	"",
	""
    },
/*
    {
      "sweep",
      "You grab a broom and sweep up the mess you made.",
      "$n grabs a broom and sweeps up the mess $e made.",
      "You sweep $M up off $S feet and swirl $M around in happiness.",
      "$n sweeps $N up off $S feet and swirls $M around in happiness.",
      "$n sweeps you off your feet and swirls you around in happiness.",
      "You swirl around in happiness.",
      "$n swirls $mself around.	 Pretty silly, huh?"
    },

    {
	"swoon",
	"You swoon in ecstacy.",
	"$n swoons in ecstacy.",
	"You swoon in ecstacy at the thought of $M.",
	"$n swoons in ecstacy at the thought of $N.",
	"$n swoons in ecstacy as $e thinks of you.",
	"You swoon in ecstacy.",
	"$n swoons in ecstacy."
    },

    {
	"tackle",
	"You can't tackle the AIR!",
	"",
	"You run over to $M and bring $M down!",
	"$n runs over to $N and tackles $M to the ground!",
	"$n runs over to you and tackles you to the ground!",
	"You wrap your arms around yourself, and throw yourself to the ground.",
	"$n wraps $s arms around $mself and brings $mself down!?"
    },
    
    {
	"tap",
	"You tap your foot impatiently.",
	"$n taps $s foot impatiently.",
	"You tap your foot impatiently.	 Will $E ever be ready?",
	"$n taps $s foot impatiently as $e waits for $N.",
	"$n taps $s foot impatiently as $e waits for you.",
	"You tap yourself on the head.	Ouch!",
	"$n taps $mself on the head."
    },
*/	
    {
	"swing",
	"你高興的搖來搖去。",
	"$n高興的搖來搖去。",
	"你狠狠地搖著$N，快把$E的骨頭給搖散了。",
	"$n狠狠地搖著$N，快把$E的骨頭給搖散了。",
	"$n狠狠地搖著你，快把你的骨頭給搖散了。",
	"你不停地搖來搖去，看來要搖到外婆橋才會停下來。",
	"$n不停地搖來搖去，看來要搖到外婆橋才會停下來。"
    },

    {
	"tender",
	"你溫柔的說：「沒關係！」",
	"$n溫柔的說：「沒關係！」",
	"你對$N投以關愛的眼神。",
	"$n對$N投以關愛的眼神。",
	"$n對你投以關愛的眼神。",
	"你唱道：「我不醜～而且我很溫柔～」",
	"$n唱道：「我不醜～而且我很溫柔～」"
    },

    {
	"thank",
	"你對在場的所有人道謝！",
	"$n對在場的所有人道謝！",
	"你衷心地對$N表達感謝之意。",
	"$n衷心地對$N表達感謝之意。",
	"$n衷心地對你表達感謝之意。",
	"謝你自己？",
	""
    },

    {
	"think",
	"你低頭沉思。",
	"$n低頭沉思。",
	"你認真考慮$N的問題。",
	"$n認真考慮$N的問題。",
	"$n認真考慮你的問題。",
	"你認真考慮著自己的未來。",
	"$n認真考慮著$e的未來。"
    },

    {
	"thirsty",
	"你有氣無力的喊道：「水～給我水～」",
	"$n有氣無力的喊道：「水～給我水～」",
	"",
	"",
	"",
	"",
	""
    },
  /* 
    {
	"throttle",
	"Whom do you want to throttle?",
	"",
	"You throttle $M till $E is blue in the face.",
	"$n throttles $N about the neck, until $E passes out.  THUNK!",
	"$n throttles you about the neck until you pass out.  THUNK!",
	"That might hurt!  Better not do it!",
	"$n is getting a crazy look in $s eye again."
    },
*/
    {
	"tickle",
	"Tickle 誰？",
	"",
	"你伸手到$N的胳肢窩下面，弄得$E笑個不停。",
	"$n伸手到$N的胳肢窩下面，弄得$E笑個不停。",
	"$n伸手到你的胳肢窩下面，弄得你笑個不停。",
	"你伸手到自己的胳肢窩下面，弄得自己笑個不停。",
	"$n伸手到$e自己的胳肢窩下面，弄得$e笑個不停。"
    },
/*
    {
	"twiddle",
	"You patiently twiddle your thumbs.",
	"$n patiently twiddles $s thumbs.",
	"You twiddle $S ears.",
	"$n twiddles $N's ears.",
	"$n twiddles your ears.",
	"You twiddle your ears like Dumbo.",
	"$n twiddles $s own ears like Dumbo."
    },

    {
	"timeout",
	"You take a 'T' and drink some Gatorade (tm).",
	"$n takes a 'T' and drinks some Gatorade (tm).",
	"You take a 'T' and offer $M some Gatorade (tm).",
	"$n takes a 'T' and offers $N some Gatorade (tm).",
	"$n takes a 'T' and offers you some Gatorade (tm).",
	"You take a 'T' and drink some Gatorade (tm).",
	"$n takes a 'T' and drinks some Gatorade (tm)."
    },
    
    {
	"tongue",
	"You stick out your tongue.",
	"$n sticks out $s tongue.",
	"You stick your tongue out at $M.",
	"$n sticks $s tongue out at $N.",
	"$n sticks $s tongue out at you.",
	"You stick out your tongue and touch your nose.",
	"$n sticks out $s tongue and touches $s nose."
    },

    {
	"torture",
	"You have to torture someone!",
	"",
	"You torture $M with rusty weapons, Mwaahhhhh!!",
	"$n tortures $N with rusty weapons, $E must have been REAL bad!",
	"$n tortures you with rusty weapons!  What did you DO!?!",
	"You torture yourself with rusty weapons.  Was it good for you?",
	"$n tortures $mself with rusty weapons.	 Looks like $e enjoys it!?"
    },

    {	
	"tummy",
	"You rub your tummy and wish you'd bought a pie at the bakery.",
	"$n rubs $s tummy and wishes $e'd bought a pie at the bakery.",
	"You rub your tummy and ask $M for some food.",
	"$n rubs $s tummy and asks $N for some food.",
	"$n rubs $s tummy and asks you for some food.  Please?",
	"You rub your tummy and wish you'd bought a pie at the bakery.",
	"$n rubs $s tummy and wishes $e'd bought a pie at the bakery."
    },

    {
	"tweety",
	"You exclaim 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims 'I TAWT I TAW A PUTTY TAT!!'",
	"You exclaim to $M, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $N, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to you, 'I TAWT I TAW A PUTTY TAT!!'",
	"You exclaim to yourself, 'I TAWT I TAW A PUTTY TAT!!'",
	"$n exclaims to $mself, 'I TAWT I TAW A PUTTY TAT!!'"
    },
    
    {
	"twiddle",
	"You patiently twiddle your thumbs.",
	"$n patiently twiddles $s thumbs.",
	"You twiddle $S ears.",
	"$n twiddles $N's ears.",
	"$n twiddles your ears.",
	"You twiddle your ears like Dumbo.",
	"$n twiddles $s own ears like Dumbo."
    },

    {
	"type",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"$n couldn't type a period if there was only one key on the keyboard.",
	"You throw up yor handz in dizgust at yur losy typing skils.",
	"$n couldn't type a period if there was only one key on the keyboard."
    },
    */
    {
	"tsk",
	"你嘖嘖稱奇。",
	"$n嘖嘖稱奇。",
	"你對著$N嘖嘖稱奇。",
	"$n對著$N嘖嘖稱奇。",
	"$n對著你嘖嘖稱奇。",
	"你不停地發出嘖、嘖、嘖的聲音。",
	"$n不停地發出嘖、嘖、嘖的聲音。"
    },
    
    {
	"understand",
	"你點了點頭，露出恍然大悟的表情。",
	"$n點了點頭，臉上一副「我懂了」的表情。",
	"你眼露兇光，對$N說：「看來，一切你都知道了？」",
	"$n眼露兇光，對$N說：「看來，一切你都知道了？」",
	"$n眼露兇光，對你說：「看來，一切你都知道了？」",
	"你覺得這個世界上沒有別人瞭解你。",
	"$n覺得這個世界上沒有人能瞭解$e。"
    },

    {
	"viva",
	"你大喊一聲：「萬歲！」",
	"$n大喊一聲：「萬歲！」",
	"你萬分恭敬地朝$N跪拜下去，高呼：「萬歲，萬歲，萬萬歲！」",
	"$n萬分恭敬地朝$N跪拜下去，高呼：「萬歲，萬歲，萬萬歲！」",
	"$n萬分恭敬地朝你跪拜下去，高呼：「萬歲，萬歲，萬萬歲！」",
	"",
	""
    },
    
    {
	"waggle",
	"Waggle 誰？",
	"",
	"你對$N搖搖手指：「小朋友，這樣不好哦！」",
	"$n對$N搖搖手指：「小朋友，這樣不好哦！」",
	"$n對你搖搖手指：「小朋友，這樣不好哦！」",
	"",
	""
    },
    {
	"wait",
	"你在原地坐了下來，開始等待。",
	"$n在原地坐了下來，開始等待。",
	"你在原地坐了下來，開始等$N。",
	"$n在原地坐了下來，開始等$N。",
	"$n在原地坐了下來，開始等你。",
	"你耐心的等待著升42級的時候到來。",
	"$n耐心的等待著升42級的時候到來。"
    },

    {
	"wave",
	"你揮了揮手。",
	"$n揮了揮手。",
	"你對$N揮手道別。",
	"$n對$N揮手道別。",
	"$n對你揮手道別。",
	"要離開遊戲，用QUIT比較快。",
	""
    },

    {
	"weak",
	"你看起來楚楚可憐的，生怕別人欺負你。",
	"$n看起來楚楚可憐的，你們不會欺負$e吧？",
	"你囂張地對$N說：「哼！回去練一百年再來跟我交手吧！」",
	"$n囂張地對$N說：「哼！回去練一百年再來跟我交手吧！」",
	"$n囂張地對你說：「哼！回去練一百年再來跟我交手吧！」",
	"",
	""
    },

    {
	"welcome",
	"你代表天神歡迎大家來到這個世界。",
	"$n代表天神歡迎大家來到這個世界。",
	"你對$N說：「別客氣，今天我請客，盡量吃。」",
	"$n對$N說：「別客氣，今天我請客，盡量吃。」",
	"$n對你說：「別客氣，今天我請客，盡量吃。」",
	"你對大家說：「別客氣，今天我請客，大家盡量吃。」",
	"$n對大家說：「別客氣，今天我請客，大家盡量吃。」"
    },

    {
	"whistle",
	"你愉快地吹著口哨。",
	"$n愉快地吹著口哨。",
	"你對$N大聲的吹著口哨。",
	"$n對$N大聲的吹著口哨。",
	"$n對你大聲的吹著口哨。",
	"你吹著口哨，自得其樂。",
	"$n吹著口哨，自得其樂。"
    },
/*
    {
	"wedgie",
	"You look around for someone to wedgie.",
	"$n is looking around for someone to wedgie!  Run!",
	"You wedgie $M.	 Must be fun! ",
	"$n wedgies $N to the heavens.",
	"$n wedgies you!  Ouch!",
	"You delight in pinning your underwear to the sky.",
	"$n wedgies $mself and revels with glee."
    },

    {
	"whine",
	"You whine like the great whiners of the century.",
	"$n whines 'I want to be a god already.	 I need more hitpoints..I...'",
	"You whine to $M like the great whiners of the century.",
	"$n whines to $N 'I want to be an immortal already.  I need more hp...I..'",
	"$n whines to you 'I want to be an immortal already.  I need more hp...I...'",
	"You whine like the great whiners of the century.",
	"$n whines 'I want to be a god already.	 I need more hitpoints..I...'"
    },
    
    {
	"wiggle",
	"Your wiggle your bottom.",
	"$n wiggles $s bottom.",
	"You wiggle your bottom toward $M.",
	"$n wiggles $s bottom toward $N.",
	"$n wiggles his bottom toward you.",
	"You wiggle about like a fish.",
	"$n wiggles about like a fish."
    },

    {
	"wince",
	"You wince.  Ouch!",
	"$n winces.  Ouch!",
	"You wince at $M.",
	"$n winces at $N.",
	"$n winces at you.",
	"You wince at yourself.  Ouch!",
	"$n winces at $mself.  Ouch!"
    },

*/
    {
	"wink",
	"你眨了眨眼睛！",
	"$n眨了眨眼睛！",
	"你對$N眨了眨眼睛！",
	"$n對$N眨了眨眼睛！",
	"$n對你眨了眨眼睛！",
	"對自己眨眼？看得到嗎？",
	""
    },
/*
    {
	"wish",
	"You make a wish upon a star for a dream you have.",
	"$n sighs... and makes a wish upon a shooting star.",
	"You wish to be loved by $M.",
	"$n wishes to be loved by $N.",
	"$n wishes to be loved by you.",
	"You wish for more attention.",
	"$n wishes for more attention."
    },
*/
    /*{
	"worship",
	"You worship the powers that be.",
	"$n worships the powers that be.",
	"You drop to your knees in homage of $M.",
	"$n prostrates $mself before $N.",
	"$n believes you are all powerful.",
	"You worship yourself.",
	"$n worships $mself - ah, the conceitedness of it all."
    },*/
	
    { // by skalt@ROC
        "worm",
        "你像隻蟲般在地上蠕動著。",
        "$n像隻蟲般在地上蠕動著。",
        "你悄悄地把毛毛蟲丟到了$N的衣服裡。",
        "$n悄悄地把毛毛蟲丟到了$N的衣服裡。",
        "$n悄悄地把毛毛蟲丟到了你的衣服裡。",
        "你大叫一聲，試著抓出在背上移動的蟲蟲。",
        "$n慌張的抓著$s背，一面大喊：「有蟲，有蟲啦~><~」"
    },

    {
	"wow",
	"你對一切都感到很訝異。",
	"$n目瞪口呆，好像看到了什麼不可思議的事情。",
	"",
	"",
	"",
	"",
	""
    },
    /*
    {
	"wrap",
	"You wrap a present for your love.",
	"$n wraps a present for someone special.",
	"You wrap your legs around $M.",
	"$n wraps $s legs around $N.",
	"$n wraps $s legs around you.  Wonder what's coming next?",
	"You wrap yourself with some paper.",
	"$n wraps $mself with some wrapping paper.  Is it Christmas?"
    },
*/
    {
	"worry",
	"你突然感到莫名的擔心！",
	"$n突然感到莫名的擔心！",
	"你對$N苦口婆心地說：「孩子，你這樣實在讓人很擔心耶。」",
	"$n對$N苦口婆心地說：「孩子，你這樣實在讓人很擔心耶。」",
	"$n對你苦口婆心地說：「孩子，你這樣實在讓人很擔心耶。」",
	"你對自己的狀況感到很擔心。",
	"$n對$e自己的狀況感到很擔心。"
    },

    {
	"ya",
	"你露出勝利的表情，「YA～！」",
	"$n露出勝利的表情，「YA～！」",
	"你對$N露出勝利的表情，「YA～！」",
	"$n對$N露出勝利的表情，「YA～！」",
	"$n對你露出勝利的表情，「YA～！」",
	"",
	""
    },
    
    {
	"yabba",
	"YABBA-DABBA-DOO!",
	"$n 大喊著 '嘿！威瑪 -- YABBA DABBA DOO!'",
	"你大喊著 '嘿！$N -- YABBA DABBA DOO!'",
	"$n 大喊著 '嘿！$N -- YABBA DABBA DOO!'",
	"$n 大喊著 '嘿！$N -- YABBA DABBA DOO!'",
	"YABBA-DABBA-DOO!",
	"$n 大喊著 '嘿，威瑪 -- YABBA DABBA DOO!'"
    },
  /* 
    {
	"yahoo",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, kid, that was one in a million!'",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, $N, that was one in a million!'",
	"$n exclaims, 'YAHOO!  Great shot, $N, that was one in a million!'",
	"You do your best Han Solo impression.",
	"$n exclaims, 'YAHOO!  Great shot, $n, that was one in a million!'"
    },*/
    {
	"yawn",
	"你「呵～」的一聲，打了一個哈欠。",
	"$n「呵～」的一聲，打了一個哈欠。",
	"你「呵～」的一聲，對$N打了一個哈欠。",
	"$n「呵～」的一聲，對$N打了一個哈欠。",
	"$n「呵～」的一聲，對你打了一個哈欠。",
	"",
	""
    },

	{
	"tea",
	"你趁著閒來無事的時候沏了一壺好茶，聞著茶香好不悠哉～",
	"$n悠閒自得的沏了一壺茶，淡淡的茶香飄散在四周～",
	"你幫$N倒上了杯茶，也給了自己一杯，以茶代酒、滿溢此生情誼。",
	"$n幫$N倒上了杯茶，也給了自己一杯，倆人在茶香雲煙中暢快對談。",
	"$n幫你倒了杯茶，杯中的是友情也是思念，有友如此，此生何求？",
	"你給自己倒了杯茶，看著琥珀色的茶水想著往日的種種。",
	"$n在一旁喝起了老人茶，想著過去種種與昔日好友。"
	},

	{
	"twoone",
	"你向上天祈禱不要被二一，不過浮雲似乎變成了二一字樣....",
	"$n憂鬱的看著天空，喃喃祈禱著不要被二一",
	"你看了看$N，邊奸笑邊拿出巨大的\"二一印\"朝$N臉上重擊而去！",
	"$n在$N臉上蓋\了一個大大的二一，真是殘忍。",
	"$n無情的對你使用了傳說中的\"二一印\"，你頓時覺得前途無亮",
	"你拿出課表開始慢慢算學分....唉，大勢已去....",
	"$n拿著課表在一旁掐手指，然後嘆道：「二‧一‧了....」"
	},

	{
	"marry",
	"你拿出一朵鮮花，並對大家說：「誰要嫁給我呀！」",
	"$n拿出一朵鮮花所有人大喊：「誰要嫁給我呀！」",
	"你拿出一朵鮮花向$N悄悄的說：「嫁給我吧，依莎貝爾！」",
	"$n拿出一朵鮮花向$N悄悄的說：「嫁給我吧，依莎貝爾！」",
	"$n拿出一朵鮮花悄悄的對你說：「嫁給我吧，依莎貝爾！」",
	"你難道要跟自己結婚？",
	"$n對自己說：「我要結婚！」"
	},

	{
	"bed",
	"你從背後拿出了一張床，並躺在上面",
	"$n從背後拿出一張愛的小床，並曖昧的躺在上面",
	"你拿出一張愛的小床，並對$N說：「$N～$N～我們一起睡吧！」",
	"$n拿出一張愛的小床向$N說：「$N～$N～我們一起睡吧！」",
	"$n對出一張愛的小床向你說：「$N～$N～我們一起睡吧！」",
	"你拿出一張愛的小床，自己一個人睡在床上",
	"$n拿出愛的小床，自己一個人睡在床上，好悽涼阿"
	},

	{
	"ohoho",
	"妳像個女王般，發出高分貝的笑聲。",
	"$n單手叉腰，另一手放在嘴邊，發出高分貝的笑聲。哦～～呵呵呵～～",
	"妳單手叉腰，另一手放在嘴邊，對著$N發出高分貝的笑聲。",
	"$n對著$N發出高分貝的笑聲。哦～～呵呵呵～～",
	"$n單手叉腰，另一手放在嘴邊，對著妳發出高分貝的笑聲。哦～～呵呵呵～～",
	"妳像個女王般，發出高分貝的笑聲。",
	"$n像個女王般，發出高分貝的笑聲。  哦～～呵呵呵～～"
	},

	{
	"oldbie",
	"你對著大家神氣的說：「我是老手～需要幫忙找我就對了！」",
	"$n對著大家神氣的說：「我是老手～需要幫忙找我就對了！」",
	"你將$N放進骨灰罈中，擺\到神壇上開始祭拜。",
	"$n將$N放進骨灰罈裡，擺\在神桌上虔誠的祭拜著。",
	"$n把你當成骨灰擺\到神桌上，對著你虔誠的祭拜著。",
	"你對著大家狂笑道：「跟我這個老手比，你們都太嫩了啊！」",
	"$n對著大家狂笑道：「跟我這個老手比，你們都太嫩了啊！」"
	},

	{
	"avert",
	"你緩緩將目光轉移，假裝什麼都沒看到。",
	"$n突然將頭歪向一邊，假裝在欣賞著遠方的風景。",
	"你為了閃避$N的目光，不自然的將頭轉向旁邊。",
	"$n為了閃避$N的目光，不自然的將頭轉向旁邊。",
	"$n為了閃避你的目光，不自然的將頭轉向旁邊。",
	"你在眾人的目光下手足無措，只能四處張望著。",
	"$n看起來手足無措，緊張的四處張望著。"
	},

	{
    "sting",
    "你掏出祖傳小金針跟詛咒小草人，準備開始作法。",
    "$n拿著一根細長的金針，還奸惡的笑著，不知道想幹啥壞事。",
    "你在草人上寫上了$N的名字，開始瘋狂的草人插針遊戲。",
    "$n躲在陰暗的角落，口中喃喃唸著：「可惡的$N....我插你個小人頭....」。",
    "你看到$n拿針朝著草人刺下去，頓時你渾身刺痛，當場昏迷。",
    "你在草人上寫上自己的名字，然後猛力一刺--OUCH！好痛啊><。",
    "$n刺了草人一下，隨即痛的跳上半空大喊：「好討厭的感覺啊！」。"
	},

	{
	"strut",
	"你趾高氣昂地晃來晃去。",
	"$n趾高氣昂地晃來晃去。",
	"你得意的在$N面前晃來晃去。",
	"$n得意的在$N面前晃來晃去。",
	"$n得意的在你面前晃來晃去, 一副很欠扁的樣子。",
	"你意氣風發的在這兒來回踱步。",
	"$n意氣風發的在這兒來回踱步。"
	},

	{
	"fossil",
	"你四處挖掘, 並且喃喃地唸著「這邊不知道有沒有化石呢？」",
	"$n四處挖掘, 把這地方挖得一個洞一個洞地。",
	"你對著大家大喊「我發現珍貴的化石了！」, 並且用手指著$N",
	"$n對著大家大喊「我發現珍貴的化石了！」, 並且用手指著$N",
	"$n對著大家大喊「我發現珍貴的化石了！」, 並且用手指著你",
	"你大聲地喊著「不要再叫我化石了 >_<」",
	"$n大聲地喊著「不要再叫我化石了 >_<」"
	},
	{
	"tear",
	"你抬起頭，默默地看著天空, 眼淚\不由自主的流下....。",
	"$n抬起頭，默默地看著天空, 眼淚\不由自主的流下....。",
	"你拿起手帕，溫柔的幫$N拭去眼角的淚\痕....。",
	"$n拿起手帕，溫柔的幫$N拭去眼角的淚\痕....。",
	"$n拿起手帕，溫柔的幫妳拭去眼角的淚\痕....。",
	"你低下頭來，擦擦自己眼角的淚\水....。",
	"$n低下頭來，擦擦自己眼角的淚\水....。"
	},
	{
	"pant",
	"你滿身大汗，滿臉通紅地大聲喘氣。",
	"$n滿身大汗，滿臉通紅的大聲喘氣。",
	"你滿臉通紅地在$N的耳邊輕輕地喘息。",
	"$n滿臉通紅的朝著$N的耳邊喘氣。",
	"$n滿臉通紅地在你的耳邊輕輕地喘息。",
	"你紅著臉，盡力壓抑著自己的喘息。",
	"$n紅著臉，盡力壓抑著自己的喘息。"
	},
	{
	"kneel",
	"你驚恐的跪下，大聲地說：「奴才下次不敢了！」",
	"$n驚恐的跪下，大聲地說：「奴才下次不敢了！」",
	"你恭敬地單膝跪在$N的面前，親吻他的手背。",
	"$n恭敬地單膝跪在$N的面前，親吻他的手背。",
	"$n恭敬地在你面前單膝跪下，親吻你的手背。",
	"你頂著水桶，面對牆角跪在算盤上。",
	"$n頂著水桶，對著牆角跪在算盤上。"
	},
	{
	"angel",
	"你神情哀怨地說道：「我還在尋找屬於我的天使... 」",
	"$n神情哀怨地說道：「我還在尋找屬於我的天使... 」",
	"你突然覺得$N就像天使，不禁激動地握住她的雙手說道：「$N，你真是我的天使！」",
	"$n突然不發一語地握住$N的雙手，以戀慕的神情對它說道：「$N，你是我的天使！」",
	"$n猛然不發一語地握住你的雙手，神情戀慕地對你說道：「$N，你是我的天使！」",
	"在這一瞬間，你覺得自己像天使一般神聖純潔。",
	"在你身旁的$n突然覺得她自己像天使一般神聖純潔。"
	},
	{
     	"niceman",
     	"你放聲大喊：「有沒有人要加入好人俱樂部啊！」",
     	"$n放聲大喊：「有沒有人要加入好人俱樂部啊！」",
     	"你帶著歉疚的神情對$N說：「你是一個好人....」",
     	"$n帶著歉疚的神情對$N說：「你是一個好人....」，好人俱樂部又多一個人了！",
     	"$n帶著歉疚的神情對你說：「你是一個好人....」",
	"你絕望的大喊：「我不想當好人啊！」",
     	"$n絕望的大喊：「我不想當好人啊！」"
	},
	{
     	"notfunny",
     	"你覺得一切真是太無趣了。",
     	"$n嘆著一口氣說：「唉 ... 真無趣。」",
     	"你很認真地對$N說：「一 點 都 不 好 笑 ...。」",
     	"$n帶著認真的神情對$N說：「一 點 都 不 好 笑 ...。」",
     	"$n帶著認真的神情對你說：「一 點 都 不 好 笑 ...。」",
     	"你對自己的笑話感到懷疑地說：「有~ 這麼難笑嗎？」",
     	"$n哀怨地說：「有~ 這麼難笑嗎？」",
	},
	{
    	"crash",
    	"你衷心地祈求一切平安，不要有什麼意外發生。",
     	"$n突然對天祈禱，希望今天不要crash。",
   	"你生氣地對$N說：「都是你害 ROC crash 的啦!!」",
    	"$n生氣地對$N說：「都是你害 ROC crash 的啦!!」",
     	"$n生氣地對你說：「都是你害 ROC crash 的啦!!」",
     	"你開始懷疑自己是不是讓 ROC crash？",
     	"$n心虛地說：「不會是因為我 crash 的吧 ...。」"
	},
	{
     	"impossible",
     	"你覺得世界上不會有這種事情發生。",
     	"$n皺著眉頭說：「不可能吧 ...。 」",
     	"根據你的經驗，你覺得$N辦不到。",
     	"$n好心地勸$N說：「不可能吧 ...。 」",
     	"$n對你分析事情的真相，你開始對自己的信心動搖了。",
     	"你覺得這種事不會發生在你身上。",
     	"$n對自己說：「這種事對我來說是沒可能的呀。」"
	},
	{
     	"cantbe",
     	"你開始害怕事情已經不是你所能想像的。",
     	"$n非常不能接受地說：「不...不會吧...。」",
     	"你開始對$N起了疑心。",
     	"$n不懷好意地對$N說：「該不會是你吧 ...。」",
     	"$n不懷好意地對你說：「該不會是你吧 ...。」",
     	"你怎麼開始懷疑起自己來了？！",
     	"$n吞吞吐吐地說：「不 ... 不會是我吧 .. 。」"
	},
	{
    	 "nosmoking",
     	"你開始大聲宣揚著拒吸測試二手煙的理念。",
     	"$n開始大聲宣揚著拒吸二手煙的理念。",
     	"你朝著$N潑了一桶水, 並且大聲地說：「拒吸二手煙!」",
     	"$n朝著$N潑了一桶水, 並且大聲地說：「拒吸二手煙!」",
     	"$n朝著你潑了一桶水, 並且大聲地說：「拒吸二手煙!」",
     	"你到處貼著拒吸二手煙的廣告。",
     	"$n到處貼著拒吸二手煙的廣告。阿!, $e竟然把廣告貼到你臉上了!!"
	},
   /* 
    {
	"yeehaw",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'",
	"You mount your white pony (?) and shout 'YEEEEEEEEEEHAW!'",
	"$n mounts $s white pony (?) and shouts 'YEEEEHHHAAAAAAWWWW!'"
    },
*/
    {
	"",
	"", "", "", "", "", "", ""
    }
};



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
	CHAR_DATA *victim;
	char       arg [ MAX_INPUT_LENGTH ];
	int	       cmd;
	bool       found;

	found  = FALSE;
	for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
	{
		if ( command[0] == social_table[cmd].name[0]
				&& !str_prefix( command, social_table[cmd].name ) )
		{
			found = TRUE;
			break;
		}
	}

	if ( !found )
		return FALSE;

	if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
	{
		send_to_char( "你無法使用任何表情指令！\n\r", ch );
		return TRUE;
	}

	switch ( ch->position )
	{
		case POS_DEAD:
			send_to_char( "靜靜躺著吧，你已經死了.\n\r", ch );
			return TRUE;

		case POS_INCAP:
		case POS_MORTAL:
			send_to_char( "你的狀況太糟了.\n\r", ch );
			return TRUE;

		case POS_STUNNED:
			send_to_char( "你頭昏眼花.\n\r", ch );
			return TRUE;

		case POS_SLEEPING:
			/*
			 * I just know this is the path to a 12" 'if' statement.  :(
			 * But two players asked for it already!  -- Furey
			 */
			if ( !str_cmp( social_table[cmd].name, "snore" ) )
				break;
			send_to_char( "你正在睡夢中，請先 wake 起來。\n\r", ch );
			return TRUE;

	}

	one_argument( argument, arg );
	victim = NULL;
	if ( arg[0] == '\0' )
	{
		act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR );
		act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM );
	}
	else if ( !( victim = get_char_world( ch, arg ) ) )
	{
		send_to_char( "這裡沒有這個人.\n\r", ch );
	}
	else if ( victim == ch )
	{
		act( social_table[cmd].char_auto, ch, NULL, victim, TO_CHAR );
		act( social_table[cmd].others_auto, ch, NULL, victim, TO_ROOM );
	}
	else if ( !get_char_room( ch, arg ) && can_see( ch, victim ) )
	{
		if ( !IS_NPC( victim ) )
		{

			ROOM_INDEX_DATA *original;
			char *ldbase = "從遙遠的地方, ";
			char ldmsg [ MAX_STRING_LENGTH ];
			CHAR_DATA	*   fighting = NULL;

			if( ch->fighting ) fighting = ch->fighting;

			original = ch->in_room;
			char_from_room( ch );
			char_to_room( ch, original );
			strcpy( ldmsg, ldbase );
			if( social_table[cmd].char_found[0] != '\0' )
			{
				strcat( ldmsg, social_table[cmd].char_found );
				act( ldmsg, ch, NULL, victim, TO_CHAR );
			}
			if( social_table[cmd].vict_found[0] != '\0' ) {
				strcpy( ldmsg, ldbase );
				strcat( ldmsg, social_table[cmd].vict_found );
				act( ldmsg, ch, NULL, victim, TO_VICT );

			}
			char_from_room( ch );
			char_to_room( ch, original );
			if(fighting){ 
				stop_fighting(ch, FALSE);
				set_fighting(ch, fighting);
				set_fighting(fighting, ch);	//Added by Razgriz    20050618
			}
		}
		else
		{
			send_to_char( "這裡沒有這個人.\n\r", ch );
		}
	}
	else
	{
		act( social_table[cmd].char_found, ch, NULL, victim, TO_CHAR );
		act( social_table[cmd].vict_found, ch, NULL, victim, TO_VICT );
		act( social_table[cmd].others_found, ch, NULL, victim, TO_NOTVICT );

		if ( !IS_NPC( ch )
				&& IS_NPC( victim )
				&& !IS_AFFECTED( victim, AFF_CHARM )
				&& IS_AWAKE( victim ) )
		{
			if ( !can_see( victim, ch ) )
			{
				send_to_char( "他看不到你！\n\r", ch );
				return TRUE;
			}

			if (mprog_act_trigger( social_table[cmd].name,ch, victim))
				return TRUE;

			switch ( number_bits( 4 ) )
			{
				case 0:
					if ( ( victim->level < ch->level ) && !( victim->fighting ) ){
						multi_hit( victim, ch, TYPE_UNDEFINED );
					}
					break;

				case 1: case 2: case 3: case 4:
				case 5: case 6: case 7: case 8:
					act( social_table[cmd].char_found, victim, NULL, ch, TO_CHAR );
					act( social_table[cmd].vict_found, victim, NULL, ch, TO_VICT );
					act( social_table[cmd].others_found, victim, NULL, ch, TO_NOTVICT );
					break;

				case 9: case 10: case 11: case 12:
					act( "你狠狠地打了$N幾個耳光，打的$E整張臉都腫了起來。", victim, NULL, ch, TO_CHAR );
					act( "$n狠狠地打了你幾個耳光，打的你整張臉都腫了起來。", victim, NULL, ch, TO_VICT );
					act( "$n狠狠地打了$N幾個耳光，打的$E整張臉都腫了起來。", victim, NULL, ch, TO_NOTVICT );
					break;
			}
		}
	}

	return TRUE;
}
