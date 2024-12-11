>defun_prog dhead_fericle_givedrink~
    mpoload 50130 $n 1 target
    mpechoat $n $I替你倒了一杯酒。
    mpforce $n drink wine
~
>defun_prog dhead_fericle_givereward~
    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
    mpmod_qmark $n dhead_fericle_winequest v0 = 3
    mpmod_qmark $n dhead_fericle_winequest v1 = 0
    mpcallout 1 say 太好了！三件蜥蜴皮都收到了。
    mpcallout 3 say 這是給你的酬金。
    mpcallout 4 mpgold $n 100000
    mpcallout 4 mpechoat $n $I給了你一袋金幣，裡面有十萬金幣。
    mpcallout 4 mpechoaround $I給了 $n 一袋金幣。
    mpcallout 6 另外不妨坐一下，讓我請你喝一杯吧。
    mpcallout 7 mpcall dhead_fericle_givedrink $n 
    mpcallout 8 giggle
    mpcallout 8 mpdel_qmark self dhead_allmob_speaking
~
>defun_prog dhead_fericle_randprog~
    if pcnum($) > 0
        if is_qmark(self) != dheah_allmob_speaking
            if is_qmark($r) == dhead_fericle_winequest 300015
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpdel_qmark $r dhead_fericle_winequest
                if is_qmark(%r) != dhead_allquest
                    mpadd_qmark $r dhead_allquest
                endif
                mpmod_qmark $r dhead_allquest v0 | 2
                mpcallout 1 say 真的很感謝你的幫忙。
                mpcallout 3 say 這是給你的酬金。
                mpcallout 4 mpgold $r 1000000
                mpcallout 4 mpechoat $n $I給了你一袋金幣，裡面有一百萬金幣。
                mpcallout 4 mpechoaround $I給了 $n 一袋金幣。
                mpcallout 4 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>defun_prog dhead_fericle_gotwine~
    if is_qmark($n) < dhead_fericle_winequest 300015
        say 還有其他的酒嗎？
        giggle
    endif
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) & dhead_allquest 2
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 hmm
            mpcallout 2 say 怎樣？要來買酒嗎？
            mpcallout 3 giggle $n
            mpcallout 5 如果要買特製的酒
            mpcallout 3 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) > dhead_fericle_winequest 300001
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 say 怎樣？
                mpcallout 2 say 
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            else
                if level($n) > 30
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 hmm
                    mpcallout 2 say 要買酒嗎？
                    mpcallout 3 say 品嚐看看本店特製的蜥皮酒(lizard wine)，包君滿意！
                    mpcallout 3 mpadd_qmark $n dhead_fericle -1 0 0 0 0 1
                    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                else
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                    mpcallout 1 hmm
                    mpcallout 2 say 要買酒嗎？
                    mpcallout 3 say 你還太年輕，還是少喝點吧。
                    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                endif
            endif
        endif
    endif
~
>talk_prog lizard wind~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say 嗯？蜥皮酒(lizard wine)啊。
            mpcallout 2  say 顧名思義...就是用蜥蜴的皮做的。
            mpcallout 3  say 這很奇怪嗎？
            mpcallout 4  giggle $n
            mpcallout 6  mpecho $I的眼睛一亮，似乎突然想起什麼事。
            mpcallout 7  say 對了！庫存的蜥蜴皮剛好不夠用了。
            mpcallout 8  mpechoat $n $I仔細地打量著你。
            mpcallout 8  mpechoaround $n $I仔細地打量著 $n 。
            mpcallout 10 say 這位朋友似乎是很有經驗的冒險者。
            mpcallout 11 say 可否替我打幾張蜥蜴皮嗎？(yes/no)
            mpcallout 11 mpmod_qmark $n dhead_fericle v0 = 1
            mpcallout 11 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog yes~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            say 太好了！
            mpcallout 1 giggle $n
            mpcallout 3 say 我用來釀酒的蜥蜴皮，是取自某種大頭蜥蜴身上。
            mpcallout 4 say 這種蜥蜴只會在外面的荒原一帶出沒。
            mpcallout 5 say 我需要三張，打到後請直接交給我。
            mpcallout 6 addoil $n
            mpcallout 6 mpmod_qmark $n dhead_fericle_winequest v0 = 2
            mpcallout 6 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog no~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpdel_qmark $n dhead_fericle_winequest
            hmm
            mpcallout 1 sigh
            mpcallout 3 say 那真是可惜。
            mpcallout 4 mpecho $I轉過身去，繼續清理酒杯。
            mpcallout 4 mpdel_qmark self dhead_allmob_speaking
        endif
        else
            - $n
        endif
    endif
~
>talk_prog past~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100003
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  hmm $n
            mpcallout 3  glare
            mpcallout 5  say 年輕的時候...我也曾經在外面旅行過一陣子。
            mpcallout 6  say 嗯，那是剛通過戰士試練(contest)的時候吧。
            mpcallout 7  say 當時還是瞞著親人，偷偷跟隨外地的冒險者離家出走的。
            mpcallout 8  giggle
            mpcallout 10 say 少年的心總是嚮往遼闊的遠方...
            mpcallout 11 say 嗯，不過那都是過去的事了。
            mpcallout 12 say 如今我只想待在故鄉，過著酒店老闆(bartender)的悠閒生活。
            mpcallout 13 giggle
            mpcallout 13 mpmod_qmark $n dhead_fericle_winequest v0 = 4
            mpcallout 13 mpdel_qmark self dhead_allmob_speaking
        else
            if is_qmark($n) > dhead_fericle_winequest 100003
              or is_qmark($n) & dhead_allquest 100002
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 say 無輪如何，現在的我只是個悠閒的酒店老闆(bartender)而已。
                mpcallout 2 giggle
                mpcallout 2 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 dunno $n
                mpcallout 2 say 我只是個平凡的調酒師而已。
                mpcallout 3 giggle
                mpcallout 3 mpdel_qmark self dhead_allmob_speaking
            endif
        endif
    endif
~
>talk_prog bartender~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100004
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say 這就要從我在外面冒險的那段日子說起了。
            mpcallout 2  say 當時的我跟著一隻組傭兵團旅行，曾經探訪過許多地方。
            mpcallout 3  say 從著名的史卡拉貝城，到綠樹王國、賀比特市，
            mpcallout 4  say 甚至遠渡重洋的光明鎮，都有我的足跡。
            mpcallout 5  say 當然，每到一個新的城鎮，就會藉機品嚐過當地特產的酒。
            mpcallout 6  giggle
            mpcallout 8  say 漸漸地對品酒、釀酒感到興趣，也學到不少技巧。
            mpcallout 9  say 後來傭兵團解散了，那段冒險生涯也跟著結束了。
            mpcallout 10  say 於是就想回故鄉，利用所學有關酒的技術開一間酒吧。
            mpcallout 11 say 讓愛喝酒(winebibbing)的族人也能品嚐到世界各地的名酒。
            mpcallout 12 giggle
            mpcallout 12 mpmod_qmark $n dhead_fericle_winequest v0 = 5
            mpcallout 12 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog winebibbing~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100005
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say 蜥蜴人啊，是一個出名地愛喝酒的種族。
            mpcallout 2 say 即使喝酒過多會有後遺症，常常吐酸(acid)吐得到處都是。
            mpcallout 3 shrug
            mpcallout 5 mpecho $I突然低下頭來，似乎想到什麼煩惱的事。
            mpcallout 7 say 不過，事實上...有件事我一直覺得很遺憾(regret)。
            mpcallout 7 mpmod_qmark $n dhead_fericle_winequest v0 = 6
            mpcallout 7 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog regret~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100006
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say 雖然酒店的客人以住在這裡的蜥蜴人為主，
            mpcallout 2  say 但偶然也會有來自外地的冒險者或軍官。
            mpcallout 3  glare
            mpcallout 5  say 印象蠻深刻的，有一位奇裝異服的人類劍士。
            mpcallout 6  say 他操著奇特的口音，自稱是來自一個叫清河鎮的地方。
            mpcallout 7  say 還有一位老邁的冒險者，曾經閒聊他在人魚村的經歷。
            mpcallout 8  say 那是個彷彿只出現在傳說中的地方。
            mpcallout 9  ponder
            mpcallout 11 say 這兩個地方我略有聽聞，想必當地都有特產的酒。
            mpcallout 12 say 可惜過去冒險生涯，我從未親自踏足這兩個地方。
            mpcallout 13 say 現在忙著酒吧的營業，不太可能去品嚐看看當地的酒了。
            mpcallout 14 say 如果你有機會經過，別忘了幫我帶一些酒回來。
            mpcallout 15 say 我會好好酬謝你的。
            mpcallout 16 giggle
            mpcallout 16 mpmod_qmark $n dhead_fericle_winequest v0 = 7
            mpcallout 16 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>give_prog 50060~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100002
            mpmod_qmark $n dhead_fericle_winequest v1 + 1
            mpjunk $o
            if is_qmark($n) == dhead_fericle_winequest 200001
                say 我收到第一件蜥蜴皮了。
            else
                if is_qmark($n) == dhead_fericle_winequest 200002
                    say 我收到第二件蜥蜴皮了。
                else
                    mpcall dhead_fericle_givereward $n
                endif
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 11114~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300001
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1 say 啊...這是清河鎮的竹葉青酒。
                mpcallout 2 mpecho $I用雙手捧著酒瓶，將鼻子緩緩地移進瓶口...
                mpcallout 3 mpecho 隨後露出一副很陶醉的表情。
                mpcallout 5 mpecho $I喃喃地念著：酒香從瓶子裡飄了出來...一種微醺之感填滿了我的身心。
                mpcallout 7 say 好酒！好酒！
                mpcallout 7 mpmod_qmark $n dhead_fericle_winequest v2 | 1
                mpcallout 8 mpcall dhead_fericle_gotwine $n
                mpcallout 8 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say 這不是清河鎮的竹葉青酒嗎？
                mpcallout 2 say 我已經品嚐過了啊。
                mpcallout 3 give $o $n
                mpcallout 4 shrug
                mpcallout 4 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 11128~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300002
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1 say 喔...這種酒叫做紹興酒啊。
                mpcallout 2 say 讓我來仔細地品嚐看看。
                mpcallout 3 mpecho $I拿出一只小酒杯，緩緩地倒入了少量的紹興酒...
                mpcallout 5 mpecho 只見他細細地嘗了一口，立即露出一副很陶醉的表情。
                mpcallout 6mpecho $I喃喃地念著：不愧是半百老酒，果然香醇可口。
                mpcallout 8 say 好酒！好酒！
                mpcallout 9 mpmod_qmark $n dhead_fericle_winequest v2 | 2
                mpcallout 9 mpcall dhead_fericle_gotwine $n
                mpcallout 9 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say 嗯，這不是清河鎮的紹興酒嗎？
                mpcallout 2 say 我已經品嚐過了啊。
                mpcallout 3 give $o $n
                mpcallout 4 shrug
                mpcallout 4 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 50005~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300004
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1  say 啊...這是傳說中的人魚村特產的昆布酒啊。
                mpcallout 2  tsk
                mpcallout 3  mpecho $I拿出一只小酒杯，緩緩地倒入了少量的昆布酒...
                mpcallout 5  mpecho 只見他淺嘗一口，立即滿臉通紅，一副血脈噴張的可憎表情。
                mpcallout 6  gasp
                mpcallout 8  mpecho $I好強烈的口感！彷彿汪洋大海般的氣勢注滿了我的全身！
                mpcallout 9  say 好酒！好酒！
                mpcallout 9  mpmod_qmark $n dhead_fericle_winequest v2 | 4
                mpcallout 10 mpcall dhead_fericle_gotwine $n
                mpcallout 10 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say 嗯，這是人魚村的昆布酒吧？
                mpcallout 2 say 那種味道真讓人沒齒難忘啊。
                mpcallout 3 say 不過，我想你還是先留著吧。
                mpcallout 4 give $o $n
                mpcallout 5 giggle
                mpcallout 5 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog 50008~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_fericle_winequest 100007
            if is_qmark($n) !& dhead_fericle_winequest 300008
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpjunk $o
                mpcallout 1  say 喔...你為我帶來人魚村特產的海草酒嗎？
                mpcallout 2  giggle
                mpcallout 4  mpecho $I將鼻子移近酒瓶，輕輕嗅入一口酒氣...
                mpcallout 5  mpecho 只見他捧著酒瓶越嗅越深，隨即開始淺嚐瓶中的酒。
                mpcallout 6  mpecho 你看見$I彷彿著魔似的，竟然直接大口大口地喝起酒來。
                mpcallout 7  mpecho 在$I的豪飲之下，很快地整個酒瓶被喝得滴酒不剩。
                mpcallout 9  mpecho $I一臉震驚地發出聲音：啊...
                mpcallout 10 say 我竟然在不知不覺的情況下，喝乾了整瓶酒！
                mpcallout 11 say 好酒！好酒！
                mpcallout 11 mpmod_qmark $n dhead_fericle_winequest v2 | 8
                mpcallout 12 mpcall dhead_fericle_gotwine $n
                mpcallout 12 mpdel_qmark self dhead_allmob_speaking
            else
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                give $o $n
                mpcallout 1 say 嗯，這是人魚村的海草酒吧？
                mpcallout 2 say 這種酒好喝地讓人總是一飲而盡。
                mpcallout 3 say 不過，我想你還是先留著吧。
                mpcallout 4 give $o $n
                mpcallout 5 giggle
                mpcallout 6 mpdel_qmark self dhead_allmob_speaking
            endif
        else
            drop all
        endif
    else
        drop all
    endif
~
>give_prog all~
    drop all
~
|