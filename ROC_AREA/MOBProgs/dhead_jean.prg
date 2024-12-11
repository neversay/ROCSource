>defun_prog dhead_jean_question~
    mpechoat $n ----  詢問這一帶的情報(information)
    mpechoat $n ----  關心珍的傷勢(wound)
    mpechoat $n ----  尋求一些補給品(supplies)
    if is_qmark($n) == dhead_uruk_merchantquest 100002
        mpechoat $n ----  關於烏魯克(uruk)的委託
    endif
    if is_qmark($n) == dhead_uruk_merchantquest 100003
        mpechoat $n ----  關於沙漠之光(desert sun)
    endif
~
>repop_prog 100~
    mpoload 50010 20
    mpoload 50010 20
    mpoload 50012 20
    mpoload 50013 20
    mpat 50000 wear scimitar
    mpat 50000 wear scimitar
    mpat 50000 wear khaki
    mpat 50000 wear jeans
    mpgroup roda-dhead $i 1 0
~
>greet_prog 100~
    if isqmark(self) != dhead_jean_greeting
        mpadd_qmark self dhead_jean_greeting 1 0 0 0 1 0
        smile $n
        mpcallout 3 mpdel_qmark self dhead_jean_greeting
    endif
~
>fight_prog 100~
    if isaffected(self) $ 1
        mpdis_magic self blindness
        mpdis_magic self 'sun flash'
    endif
    if rand(33)
        say 可惡的傢伙...看招！
        disarm
        mpnowait
        flaming
        mpnowait
    else
        if rand(50)
        say 可惡的傢伙...看招！
        circular
        mpnowait
        circular
        mpnowait
        endif
    endif
~
>talk_prog topic~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) !& dhead_allquest 100001
            if is_qmark($n) != dhead_uruk_merchantquest
                mpadd_qmark $n dhead_uruk_merchantquest -1 0 0 0 0 1
            endif
            if is_qmark($n) == dhead_uruk_merchantquest 200000
                mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
                mpcallout 1 smile
                mpcallout 2 say 你好，在下是來自史卡拉貝的冒險者。
                mpcallout 3 say 敝姓弗列特...請叫我珍(jean)就可以了。
                mpcallout 4 mpecho $I仔細打量著$n
                mpcallout 6 say 各位來到這裡...請問有什麼事嘛?
                mpcallout 7 mpcall dhead_jean_question $n
                mpcallout 7 mpmod_qmark $n dhead_uruk_merchantquest v1 = 1
                mpcallout 8 mpdel_qmark self dhead_allmob_speaking
            else
                if is_qmark($n) == dhead_uruk_merchantquest
                    mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            	    mpcallout 1 say ...還有什麼事嗎?
            	    mpcallout 2 mpcall dhead_jean_question $n
            	    mpcallout 3 mpdel_qmark self dhead_allmob_speaking
                endif
            endif
        endif
    endif
~
>talk_prog information~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) >= dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  ponder
            mpcallout 3  say 如你所見，這裡是東方大沙漠的延伸。
            mpcallout 4  say 往南方前進，會到達一片沙地...
            mpcallout 5  say 那裡到處都是掘地蟲(ankheg)的陷阱。
            mpcallout 6  ponder
            mpcallout 8  say 通過沙地後，會抵達一片廣大的岩場。
            mpcallout 9  say 聽說...翻過某個峽谷後，可以找到蜥蜴人要塞。
            mpcallout 10 say 這一帶的情況大概就是這樣了。
            mpcallout 12 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog ankheg~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) >= dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 nod $n
            mpcallout 2 say 掘地蟲會埋伏在地底下，用陷阱捕捉經過的路人。
            mpcallout 3 say 不幸掉入陷阱的人，至今還沒聽說有生還者...
            mpcallout 4 glare
            mpcallout 5 mpecho $I似乎突然想到什麼，露出一副感到很噁心的表情。
            mpcallout 7 say 另外...從它們身上散發出的氣味...實在不怎麼好聞。
            mpcallout 8 say 總之，請記得用飛行(fly)與隱形(invis)通過那裡。
            mpcallout 9 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog wound~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) & dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say ...這是被礫石巨人(gravel giant)害的。
            mpcallout 2 sigh
            mpcallout 4 say 地點就在南方的某個峽谷...那裡埋伏著一堆巨人。
            mpcallout 5 say 他們的膚色就跟周圍的岩石一樣...等到發覺時已經被他們包圍了。
            mpcallout 6 say 最可惡的是...他們會待在高處，用落石攻擊路過的人。
            mpcallout 7 say 如果你們要經過那片峽谷，最好當心點。
            mpcallout 8 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog supply~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) & dhead_uruk_merchantquest 200001
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 say 水或食物嗎？
            mpcallout 2 say 我們這裡有很多庫存喔。
            mpcallout 3 say 如果有任何需要，直接問我身邊這個小朋友吧。
            mpcallout 4 mpforce roda-dhead say 我才不是小朋友勒！
            mpcallout 5 mpforce roda-dhead pout
            mpcallout 6 smile
            mpcallout 7 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog uruk~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_uruk_merchantquest 100002
            if is_qmark($n) == dhead_uruk_merchantquest 200000
                mpmod_qmark $n dhead_uruk_merchantquest v1 = 1
            endif
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpmod_qmark $n dhead_uruk_merchantquest v0 = 3
            mpmod_qmark $n dhead_uruk_merchantquest v2 = 1
            mpcallout 1  say 原來如此，烏魯克已經將任務委託你了嗎？
            mpcallout 2  ponder
            mpcallout 4  say ...其實烏魯克是個商人，他聘請身為冒險者的我同行。
            mpcallout 5  say 我們此次行程的目的，是為了尋找傳說中的沙漠之光(desert sun)...
            mpcallout 6  say 無奈地，在途中我們走過頭了...誤闖入巨人的峽谷
            mpcallout 7  mpecho $I說著說著...開始皺起了眉頭。
            mpcallout 9  say 在巨人的襲擊下...我受了重傷...整個計劃也因此被耽誤了...
            mpcallout 10 mpecho $I抬起頭來，用期待的眼神望著$n。
            mpcallout 12 say 既然烏魯克已經將工作委託給你...那就萬事拜託了。
            mpcallout 13 say 請問我們尋找沙漠之光，將他交給烏魯克(uruk)。
            mpcallout 14 bow $n
            mpcallout 15 mpdel_qmark self dhead_allmob_speaking
        else
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1 ? $n
            mpcallout 2 say ...有事找烏魯克嗎？
            mpcallout 3 say ...他八成正在帳棚外面的岩石那裡。
            mpcallout 4 say 有什麼事不妨親自去找他本人吧？
            mpcallout 5 mpdel_qmark self dhead_allmob_speaking
        endif
    endif
~
>talk_prog desert sun~
    if is_qmark(self) != dheah_allmob_speaking
        if is_qmark($n) == dhead_uruk_merchantquest 100003
            mpadd_qmark self dhead_allmob_speaking 1 0 0 0 1 0
            mpcallout 1  say 沙漠之光(desert sun)
            mpcallout 2  say 這個名字的來源...是沙漠之民古老的傳說。
            mpcallout 3  say 它是一種夢幻中的藥品...據說可以醫治百病。
            mpcallout 4  say 尤其對某種不治之症的熱病，有著驚人的療效。
            mpcallout 5  mpecho $I深深地吸了一口氣。
            mpcallout 7  say 我們急需要它...可是相關的情報實在很有限。
            mpcallout 8  say 最新的線索，指出要往沙漠南方繼續搜索。
            mpcallout 9  ponder
            mpcallout 11 say 真是抱歉...我知道的只有這些了。
            mpcallout 12 mpdel_qmark self dhead_allmob_speaking
        else
            ? $n
        endif
    endif
~
|