# ROC MUD 常數與數值參數對照總表

本文件詳細列出 ROC MUD 區域檔中使用的所有常數代碼、位元旗標（Bitwise Flags）以及材質效果，基於 `cnumber.txt` 規格文件編譯而成。

---

## 1. 材質代碼與數值對照表 (#MATERIAL)
材質會修改裝備屬性、附加狀態旗標、觸發魔法效果或提供種族剋性加成。

### 材質代碼區間與效果：
* **`1~100` 保留/基礎材質**（如：`1` normal 常見材質, `2` test 測試材質, `3` flesh 血肉, `4` food 食物, `5` liquid 藥水液體, `6` herb 草藥, `7` metal 金屬 (`ac -2`，`svLightning +1`)）。
* **`101~200` 常見材質區**：
  * `101` cloth 布質 (`svfire +1`，`dodge -2`)
  * `102` leather 皮革 (`svfire +1`，物理傷害結算 `-1`)
  * `103` wool 毛皮 (`svfire +1`，`hp +2`)
  * `104` wooden 木質 (`svfire +2`，`svearth -1`，`mp +2`)
  * `105` stone 石質 (`svpetrify +2`，`svearth -1`，物理傷害結算 `-1`)
  * `106` mud 泥質 (耐用差，`svpetrify +1`，`svLightning -1`，魔法傷害結算 `-1`)
  * `107` paper 紙質 (`svfire +2`，耐用差，`mp +4`，`pp +1`)
  * `108` fiber 綢布 (`svfire +1`，`dodge -2`)
  * `109` iron 鐵製 (`mv -1`，`ac -5`，`svLightning +2`，`svwind -1`，`dodge +2`，物理傷害結算 `-2`；剋 elf/drow)
  * `110` bronze 青銅 (`hp +2`，`ac -1`，`svfire -1`，`svLightning +2`，`svearth -1`，物理傷害結算 `-1`，魔法攻擊力結算 `+2`；剋 plant)
  * `111` brass 黃銅 (`mp +2`，`ac -1`，`svwater -1`，`svLightning +2` ... 物理傷害 `-1`，魔法攻擊結算 `+2`；剋 animal)
  * `112` scale 鱗片 (`ac -1`，物理傷害結算 `-1`)
  * `113` tin 錫 (`ac -2`，`svfire -1`，`svLightning +2`)
  * `114` lead 鉛 (`ac -2`，`svLightning +2`，魔法傷害結算 `-1`)
* **`201~300` 高級與特殊材質區**（多有價格加成）：
  * `201` steel 鋼質 (`mp -10`，`hp +10`，`mv -5`，`ac -8` ... `svs +1`，`svLightning +2`，`dodge -2`，物理傷害結算 `-4`；剋 drow/elf)
  * `202` silver 銀質 (高價，`mp +4`，`svpetrify -3` ... `svLightning +3`，`pp +1`，魔法傷害結算 `-2`；剋 werewolf)
  * `203` gold 金質 (高價，`mp +4`，`ac +2`，`svpetrify -2` ... `svLightning +3`，`pp +1`，魔法傷害結算 `-1`；剋 demon)
  * `204` aluminum 鋁質 (抗中立，`mp +2`，`hp +2`，`svbreath -1` ... `svLightning +3`，魔法傷害結算 `-1`；剋 insect/troll)
  * `205` ebony 黑檀木 (高價，`hp +3`，`mv +2`，`svparalyze -2` ... `svfire +2`，`svelement -2`，魔法傷害結算 `-2`)
  * `206` juniper 檜木 (`mp +8`，`svbreath -1` ... `svfire +2`，`svelement -1`，魔法傷害結算 `-1`；隱藏能力：10% 觸發 `Protection` 聖光)
  * `207` oak 橡木 (`mp +8`，`ac -1`，`svbreath -1` ... `svfire +2`，`svearth -1`，`pp +1`；隱藏能力：10% 觸發 `Protection` 聖光)
  * `208` granite 花崗岩 (`hp +5`，`mv -5`，`ac -1`，`svpetrify +2` ... `svearth +2`，物理傷害結算 `-3`)
  * `209` marble 大理石 (`mp +5`，`mv -5` ... `svpetrify +2`，`svLightning -2`，`svearth +2`，物理傷害結算 `-3`)
  * `210` basalt 玄武岩 (`mp -5`，`hp +10`，`mv -5` ... `svpetrify +2`，`svfire -2`，`svearth +2`，物理傷害結算 `-3`)
  * `211` glass 玻璃 (脆弱，`mp +5`，`ac +1` ... `pp +1`，物理傷害結算 `+10`，魔法傷害結算 `-10`)
  * `212` bone 骨質 (`mp +1`，`hp -2`，`mv +4` ... `svearth -1`，物理結算 `-3`，魔法結算 `+2`；剋 elf/human/ghost)
  * `213` ivory 象牙 (`mp +3`，`svs -2`，`svearth -2`，物理結算 `-2`，魔法結算 `+1`；剋 ghost)
  * `214` dragon bone 龍骨 (高價，`mp +5`，`hp -10` ... `svbreath -1`，`svs -1`；剋 dragon/worm)
  * `215` dragon scale 龍鱗 (高價，`hp -10`，`mv -2` ... `svs -1`，`svbreath -2`，`pp +1`；剋 dragon/worm)
  * `216` dragon crystal 龍晶 (高價，`hp +5`，`mp +5` ... `svbreath -1`，`svfire -1`；剋 dragon/worm)
  * `217` feather 輕羽 (脆弱高價，`mv +13`，`ac +1` ... `svwind +3`，`dodge -5`，魔法傷害結算 `-1`)
  * `218` mithril 秘銀 (`svpetrify -2`，`svbreath -1` ... `svLightning +4`，`svelement -1`，魔法傷害結算 `-1`；剋 ghost/undead/phantom/demon/vampire)
  * `219` magnetism 磁質 (`mp +5`，`svpetrify -1` ... `svelement -1`，魔法結算 `-2`；持續狀態：`detect magic`)
  * `220` ice 冰晶 (脆弱，`mp +5`，`mv -3` ... `svfire -2`，`svwater +3`，魔法結算 `-2`；隱藏能力：10% 觸發 `Chill Touch`)
  * `221` mercury 水銀 (`mp +5`，`hp -5` ... `svLightning +2`，`svelement -2`，魔法結算 `-1`；持續狀態：`detect magic`)
  * `222` sulfur 硫磺 (`mp -5` ... `svwand -2`，`svfire +4`，`svelement -2`，魔法結算 `-1`；持續狀態：`detect magic`)
  * `223` salt 鹽質 (`mp +5`，`hp +5` ... `svpetrify -2`，`svearth +4`，`svelement -2`，魔法結算 `-1`；持續狀態：`detect magic`)
* **`301~400` 寶石材質**（昂貴且脆弱，提供 `mp +5`，`svelement -1`，`svearth +3`，魔法傷害結算 `-2` 等基礎加成）：
  * `301` gem 劣等寶石
  * `302` jade 玉質 (`svparalyze -2`，`svs -1`；隱藏能力：3% 觸發 `Protection`；剋 ghost/undead)
  * `303` crystal 水晶 (`svs -1`，`pp +1`；隱藏能力：3% 觸發 `Cure light`；剋 ghost/undead)
  * `304` onyx 瑪瑙 (`hp +5`，`svs -1`，`svfire -1`；剋 ghost/demon)
  * `305` ruby 紅寶石 (`svfire -2`；剋 ghost/orc/elf/vampire/plant)
  * `306` sapphire 藍寶石 (`svLightning -2`；剋 ghost/mindflayer/fish/hydra/statue)
  * `307` emerald 翡翠 (`svwind -2`；剋 ghost/imp/pixie/mist/bat)
  * `308` topaz 黃玉 (`svearth -2`，`svwind +3`；剋 ghost/dwarf/gnome/hobbit/object)
  * `309` amethyst 紫水晶 (`svbreath -2`；剋 ghost/dragon/snake/lizard/lizardman)
  * `310` diamond 鑽石 (`svwater -2`；剋 ghost/human/demon/elemental/animal)
* **`401~500` 稀有物質**（高自訂屬性）：
  * `401` platinum 白金 (脆弱，`svwand -3` ... `svLightning +3`，魔法結算 `-2`；隱藏能力：3% 觸發 `Protection`；剋 ghost/undead/demon/vampire)
  * `402` obsidian 黑曜石 (`ac -3`，`svpetrify -1` ... `svearth +3`，物理結算 `-2`；隱藏能力：3% 觸發 `Detect Evil`；剋 dwarf/god/angel/elf)
  * `403` chaos matter 混沌 (脆弱，`mp +10`，`svbreath -2` ... `pp +1`，物理結算 `+10`，等級上限 `35`；剋 elf/dwarf/imp/hobbit/gnome)
  * `404` law matter 秩序 (脆弱，`hp +10`，`svbreath -2` ... `pp +1`，魔法結算 `+10`，等級上限 `35`；剋 drow/orc/beholder/lizardman/vampire)
  * `405` black matter 黑暗物質 (`ac +2`，`svs -1`，物理傷害結算 `+3`，魔法攻擊力 `-3`；剋 god/angel)
  * `406` perfect matter 完美物質 (`mp +3`，`svs -1`，`svelement -1`，魔法結算 `-3`，魔法攻擊力 `+5`)
  * `407` radiant 輻射物質 (`mp +5`，`hp -5`，`svs -1`，魔法攻擊力 `+5`；剋 human/elf/orc)
  * `408` aura 靈氣 (脆弱，`svbreath +5`，`svs -2` ... `svelement -2`，魔法結算 `-2`，魔法攻擊力 `+3`)
  * `409` fire aura 火靈氣 (脆弱，`svbreath +5`，`svfire -4`，`svwater +4` ... `svelement -2`，魔法攻擊力 `+3`)
  * `410` water aura 水靈氣 (脆弱，`svbreath +5` ... `svfire +4`，`svwater -4` ... `svelement -2`，魔法攻擊力 `+3`)
  * `411` wind aura 風靈氣 (脆弱，`svbreath +5` ... `svwind -4`，`svearth +4` ... `svelement -2`，魔法攻擊力 `+3`)
  * `412` earth aura 地靈氣 (脆弱，`svbreath +5` ... `svwind +4`，`svearth -4` ... `svelement -2`，魔法攻擊力 `+3`)

---

## 2. 房間常數設定 (#ROOMS)

### 2.1 ROOM_flags（房間旗標位元值）
* `DARK` = **1**（黑暗房間，玩家需要光源）
* `NO_MOB` = **4**（怪物/NPC 無法自行走入）
* `INDOORS` = **8**（室內，阻擋天氣與陽光，部分法術無法在室內施展）
* `UNDERGROUND` = **16**（地下室/地底，無日光變化，吸血鬼白天不扣血，阻擋某些法術）
* `PRIVATE` = **512**（私有房間，只容納最多 2 個生物）
* `SAFE` = **1024**（安全區，禁止任何 PK 與攻擊行為）
* `SOLITARY` = **2048**（單人房，只容納最多 1 個生物）
* `PET_SHOP` = **4096**（寵物店，此房間 vnum 後一號房間必須放置待售寵物）
* `NO_RECALL` = **8192**（此房間內禁止 recall 指令）
* `CONE_OF_SILENCE` = **16384**（靜音房，無法說話、叫喊、聊天或社群溝通）
* `GRAVE` = **32768**（墓地，目前無實質功能）
* `SUMMON_BIT` = **65536**（禁止傳送召喚，施術者與被召喚者所在房間都不能有此 flag 才會召喚成功）
* `AUCTION` = **131072**（此房間可叫價與拍賣）
* `SAVE` = **262144**（允許玩家下 save 指令存檔）
* `NO_TELEPORT_OUT` = **524288**（無法使用傳送術/瞬移術出去）
* `NO_TELEPORT_IN` = **1048576** (無法被傳送進來)
* `PK` = **2097152** (決鬥房，允許自由 PK 且死亡無懲罰、不掉經驗)

### 2.2 SECT_types（地形種類與代碼）
* `0` = `INSIDE`（室內，夜晚不需要光源，永亮）
* `1` = `CITY`（城市街道）
* `2` = `FIELD`（原野）
* `3` = `FOREST`（森林）
* `4` = `HILLS`（丘陵）
* `5` = `MOUNTAIN`（山岳，移動耗損大）
* `6` = `WATER_SWIM`（水上淺流，需要游泳或船隻）
* `7` = `WATER_NOSWIM`（急流，不可直接游，需要船或飛行術）
* `8` = `UNDERWATER`（水下，需要避水咒或水中呼吸法）
* `9` = `AIR`（空中，必須具備天生飛行或飛天術；此處無法召喚土元素）
* `10` = `DESERT`（沙漠，此處無法召喚火元素）

### 2.3 出口方向 (Directs)
* `0` = 北 (`north`) | `1` = 東 (`east`) | `2` = 南 (`south`) | `3` = 西 (`west`) | `4` = 上 (`up`) | `5` = 下 (`down`)

### 2.4 門旗標 Door flags（位元相加值）
* `ISDOOR` = **1**（此出口有門）
* `CLOSED` = **2**（門目前是關著的）
* `LOCKED` = **4**（門是鎖著的，若不需鎖匙鑰匙欄請填 `-1`）
* `BASHED` = **8**（門曾被蠻力撞開）
* `BASHPROOF` = **16**（門無法被撞開）
* `PICKPROOF` = **32**（門無法被盜賊撬開）
* `PASSPROOF` = **64**（門無法使用 pass door 透門術穿過）

---

## 3. 行動生物常數設定 (#MOBILES)

### 3.1 ACT_flags（動作旗標位元值）
* `IS_NPC` = **1**（系統自動識別為 NPC，建議手動填上）
* `SENTINEL` = **2**（駐守，不會隨機亂走。怪物群體跟隨或具有日常作息者必須設）
* `SCAVENGER` = **4**（自動撿地上物品）
* `AGGRESSIVE` = **32**（主動攻擊玩家）
* `STAY_AREA` = **64**（活動範圍不出其所屬區域區域）
* `WIMPY` = **128**（血少會自動逃跑）
* `PET` = **256**（寵物）
* `TRAIN` = **512**（可訓練玩家屬性）
* `PRACTICE` = **1024**（可實習玩家技能）
* `CRITICAL` = **2048**（具備「致命一擊」能力）
* `HARD_DIS` = **4096**（具備「御劍/繳械」能力）
* `HUNT` = **8192**（當敵人逃跑時，會自動進行獵殺追擊）
* `TWO_WEAPON` = **16384**（雙手持雙武器）
* `ARTIFACT` = **65536**（可使用武器隱藏力量，目前無作用）
* `TACTICS` = **262144**（具備「戰術」技巧）
* `TRACK` = **524288**（銜尾追擊獵殺逃跑的敵人）
* `REGENERACY` = **1048576**（自動再生恢復生命力）
* `GAMBLE` = **2097152**（提供賭博）
* `BANKER` = **4194304**（銀行員）
* `MOB_AI` = **8388608**（具備基本人工智慧，自動挑選較弱的目標，受 AI 等級加值修正）
* `REFLEXION` = **16777216**（物理傷害反彈）
* `NO_PAR` = **33554432**（多次擊殺時，玩家獲得的經驗值不會遞減）
* `IGNORE_NO_MOB` = **67108864**（能走入 NO_MOB 房間）
* `SMITH` = **268435456**（鐵匠，可以幫人修復裝備）

### 3.2 AFF_flags（持續法術狀態位元值）
* `BLIND` = **1** (盲目) | `INVISIBLE` = **2** (隱形) | `DETECT_EVIL` = **4** | `DETECT_INVIS` = **8** | `DETECT_MAGIC` = **16** | `DETECT_HIDDEN` = **32**
* `HOLD` = **64**（禁錮狀態；無法走路或發言）
* `SANCTUARY` = **128**（聖光狀態；傷害減半）
* `FAERIE_FIRE` = **256**（妖精之火；降低防禦 AC）
* `INFRARED` = **512**（夜視術）
* `CURSE` = **1024**（詛咒）
* `CHANGE_SEX` = **2048**（性別改變）
* `POISON` = **4096** (毒擊) | `PROTECT` = **8192** (神聖保護) | `POLYMORPH` = **16384** (種族變異)
* `SNEAK` = **32768** (潛行) | `HIDE` = **65536** (藏匿) | `SLEEP` = **131072** (睡眠) | `CHARM` = **262144** (迷亂)
* `FLYING` = **524288**（飛行狀態）
* `PASS_DOOR` = **1048576**（透明狀態；可穿過門）
* `WATERWALK` = **2097152**（水中行走術）
* `SUMMONED` = **4194304**（幻術師召喚物）
* `MUTE` = **8388608**（禁言沈默）
* `SEALMAGIC` = **16777216**（魔法封印，禁止施法）
* `FLAMING` = **134217728**（焰甲火焰反彈傷害）
* `PARALYZED` = **268435456**（麻痺癱瘓）
* `CRAZY` = **536870912**（混亂狂暴狀態）
* `WEAKEN` = **1073741824**（衰弱虛弱）
* `NIGHTSTALK` = **2147483648**（暗夜潛行）
* `DARK_RAID` = **4294967296**（微暗夜視）
* `MANASHIELD` = **8589934592**（魔法護盾，受損扣法力）
* `GILLS` = **17179869184** (水中呼吸術) | `TARDY` = **34359738368** (遲緩術)
* `MYSTIC_MIRROR` = **68719476736**（魔法鏡反彈，可被 dm 消除）
* `MYSTIC_ARMOR` = **137438953472**（魔法吸收鎧甲，法術傷害照算，但吸取等同消耗的 mana）
* `CONTINUE_CURE` = **549755813888**（持續性治療狀態）
* `AFF_DISEASE` = **1099511627776**（疾病狀態，持續損失 MP 與 MV）
* `AFF_FEAR` = **2199023255552**（恐懼，持續不斷試圖逃跑）

### 3.3 怪物抗性/特性旗標 Mob Special Number (P 參數位元值)
* `NO_DM` = **1** (免疫 Dispell Magic) | `NO_LORE` = **2** (無法被偵測) | `NO_BLIND` = **4** | `NO_CHARM` = **8**
* `NO_CURSE` = **16** | `NO_FAEFIRE` = **32** | `NO_POISON` = **64** (含免疫疾病) | `NO_SLEEP` = **128**
* `HALF_MAGIC` = **256**（魔法傷害折半）
* `ANTI_MAGIC` = **512**（魔法全面免疫）
* `REF_MAGIC` = **1024**（魔法常駐反射，不可被 dm 消除）
* `ALY_SAN` = **2048** (永久聖光) | `ALY_INVIS` = **4096** | `ALY_HIDE` = **8192** | `ALY_SNEAK` = **16384**
* `ALY_DI` = **32768** (常駐偵測隱形) | `ALY_DH` = **65536** (常駐偵測藏匿) | `ALY_INF` = **131072** | `ALY_BAR` = **262144** (常駐魔盾)
* `NO_TARDY` = **524288**（免疫遲緩術）
* `NO_MUTE` = **1048576**（免疫沈默術）
* `NO_SEALMAGIC` = **2097152**（免疫封魔）
* `EXPERT` = **4194304**（精英怪物，較難纏但給予轉職玩家更多經驗）
* `NO_CRAZY` = **8388608**（免疫惡魔戰歌混亂效果）
* `MOBAI_LV2` = **16777216**（智慧挑選玩家，挑選 HP、AC、Dodge 結算下最脆弱者）
* `MOBAI_LV3` = **33554432**（最高人工智慧）
  * *註*：僅有 `MOB_AI` 而無設定 LV2/LV3 時為 LV1。LV1 預設優先砍 Savant/Bishop > Wizard/Alchemist > Lord > Cleric/Mage。
* `NO_PARA` = **67108864**（免疫麻痺）
* `SPELL_FURY` = **268435456**（具備法術狂擊）
* `NO_FEAR` = **536870912**（免疫恐懼）

---

## 4. 物品常數設定 (#OBJECTS)

### 4.1 物品類型代碼 ITEM_types
* `1` = `LIGHT` (光源) | `2` = `SCROLL` (捲軸) | `3` = `WAND` (單體魔杖) | `4` = `STAFF` (全體法杖)
* `5` = `WEAPON` (武器) | `8` = `TREASURE` (寶物) | `9` = `ARMOR` (防具) | `10` = `POTION` (藥水)
* `12` = `FURNITURE` | `13` = `TRASH` | `15` = `CONTAINER` (容器) | `17` = `DRINK_CON` (液體容器)
* `18` = `KEY` | `19` = `FOOD` | `20` = `MONEY` | `22` = `BOAT` | `23` = `CORPSE_NPC` | `24` = `CORPSE_PC`
* `25` = `FOUNTAIN` | `26` = `PILL` | `27` = `ARROW` | `28` = `DART` (未使用)

### 4.2 物品額外旗標 Extra ITEM_flags（位元值）
* `GLOW` = **1**（發光增加價值）
* `HUM` = **2**（嗡嗡作響，減緩耐用度損耗）
* `DARK` = **4**（黑暗，給予 1% 物理吸血或 1% 靈魂吸取機率）
* `LOCK` = **8** (上鎖)
* `EVIL` = **16**（邪惡，使武器初始生成傷害加 1~5 點，但極易損壞）
* `INVIS` = **32** (隱形)
* `MAGIC` = **64**（魔法的，不可附魔(enc)，對不死、吸血、鬼、元素、夜靈等加 20% 傷害）
* `NODROP` = **128**（無法丟下）
* `BLESS` = **256**（祝福，不怕任何龍族屬性吐息）
* `ANTI_GOOD` = **512** | `ANTI_EVIL` = **1024** | `ANTI_NEUTRAL` = **2048**（陣營排斥）
* `NOREMOVE` = **4096**（綁定詛咒裝，穿上便無法脫下）
* `INVENTORY` = **8192** (不儲存，一旦從容器或屍體拿出後下線便會消失)
* `NOSAVE` = **16384**（無法存檔，下線消失）
* `VAMPIRE_BANE` = **32768**（吸血鬼嫌惡，吸血鬼無法拿取）
* `HOLY` = **65536**（神聖，武器 25% 機率加命中，防具 20% 機率加防 AC；邪惡陣營拿取會被燒傷）
* `POISONED` = **131072**（塗毒）
* `REMOVE_NODROP` = **262144**（一旦丟在地上立即轉為 `NODROP`）
* `DONATION_PIT` = **524288**（捐獻處容器；每個區域僅能設定一個）
* `UNIQUE` = **1048576**（獨一無二，線上同時只能存在一把）
* `CLAN` = **2097152**（幫會製成品，不可賣店）
* `PERSONAL` = **4194304**（專屬道具，無法被外人拿走，玩家死後會留在身上不隨屍體掉落）
* `LORE` = **16777216**（同名裝備限制，一個人身上只能同時穿戴一件同 vnum 的物品）
* `NO_SAC` = **33554432** (無法被 sac/donate)

### 4.3 物品裝備部位 WEAR_flags（位元值）
* `TAKE` = **1** (可撿起) | `WEAR_FINGER` = **2** | `WEAR_NECK` = **4** | `WEAR_BODY` = **8** | `WEAR_HEAD` = **16**
* `WEAR_LEGS` = **32** | `WEAR_FEET` = **64** | `WEAR_HANDS` = **128** | `WEAR_ARMS` = **256** | `WEAR_SHIELD` = **512**
* `WEAR_ABOUT` = **1024** | `WEAR_WAIST` = **2048** | `WEAR_WRIST` = **4096** | `WIELD` = **8192** (單手武器)
* `HOLD` = **16384** | `TWO_HAND` = **32768** | `ARROW` = **65536**

### 4.4 屬性增減類型 APPLY_types（A 參數屬性代碼）
* `0` = `NONE` | `1` = `STR` (力量) | `2` = `DEX` (敏捷) | `3` = `INT` (智力) | `4` = `WIS` (智慧) | `5` = `CON` (體質) | `6` = `SEX` | `7` = `CLASS` | `8` = `LEVEL` | `9` = `AGE` | `10` = `HEIGHT` | `11` = `WEIGHT`
* `12` = `MANA` | `13` = `HIT` (生命) | `14` = `MOVE` (體力) | `15` = `GOLD`
* `16` = `EXP` (增減擊殺經驗加成，通常用在 Mob 的 S 參數，可以是負值)
* `17` = `AC` (防禦值，防禦值越低(負數)代表防禦越強)
* `18` = `HITROLL` (命中額外加值) | `19` = `DAMROLL` (傷害額外加值)
* `20` = `SAVING_PARA` (麻痺存檢) | `21` = `SAVING_ROD` (法杖存檢) | `22` = `SAVING_PETRI` (石化存檢) | `23` = `SAVING_BREATH` (吐息存檢) | `24` = `SAVING_SPELL` (法術存檢) | `27` = `SAVING_FIRE` (火抗) | `28` = `SAVING_COLD` (冰抗) | `29` = `SAVING_LIGHTNING` (電抗) | `30` = `SAVING_EARTH` (土抗) | `31` = `SAVING_WIND` (風抗) | `35` = `SAVING_ELEMENT` (地水火風元素全抗)
* `38` = `RACE` (改變種族)
* `39` = `DODGE` (增減閃躲率)
* `40` = `POWER_POTENTIAL` (增減魔法潛力)
* `41` = `REDUCE_DAMAGE` (減免受到的物理傷害，物理抗傷結算)
* `42` = `REDUCE_MAGIC` (減免受到的魔法傷害，魔法抗傷結算)
* `43` = `APPLY_ADAMAGE` (增減物理輸出實值)
* `44` = `APPLY_AMAGIC` (增減魔法輸出實值)
* `45` = `APPLY_GPHYSICAL` (物理傷害臨界值，物傷未超過此數值直接無傷；mob 專用)
* `46` = `APPLY_GMAGIC` (魔法傷害臨界值，魔傷未超過此數值直接無傷；mob 專用)

### 4.5 職業限制判斷位元值 (P 參數位元相加值)
限制只有哪些職業可以穿戴，或是哪些職業不能穿戴。需串接 `1048576` (ALLOW_BIT 允許) 或 `2097152` (DENY_BIT 拒絕)。
* `MAGE` = **1** | `CLERIC` = **2** | `THIEF` = **4** | `WARRIOR` = **8** | `BARBARIAN` = **16** | `WIZARD` = **32** | `ALCHEMIST` = **64** | `SAVANT` = **128** | `BISHOP` = **256** | `ASSASSIN` = **512** | `NINJA` = **1024** | `KNIGHT` = **2048** | `SWORDSMAN` = **4096** | `MONK` = **8192** | `SHAMAN` = **16384** | `SORCERER` = **32768** | `LORD` = **65536** | `RANGER` = **131072** | `WARLORD` = **262144** | `MARTIALMAN` = **524288**

### 4.6 種族限制判斷位元值 (R 參數位元相加值)
同樣需串接 `1048576` (ALLOW_BIT 允許) 或 `2097152` (DENY_BIT 拒絕)。
* `HUMAN` = **1** | `ELF` = **2** | `DWARF` = **4** | `ORC` = **8** | `HOBBIT` = **16** | `IMP` = **32** | `GNOME` = **64** | `DROW` = **128** | `VAMPIRE` = **256** | `BEHOLDER` = **512** | `LIZARDMAN` = **1024** | `WEREWOLF` = **2048** | `MUMMY` = **4096**

---

## 5. 物品隱藏能力觸發時機 (H 參數觸發值)
* `HP_WEAR` = **1**（當穿戴裝備時觸發；攻防類法術施展在自己身上）
* `HP_FIGHT` = **2**（戰鬥中隨機觸發；攻擊性魔法施在對手身上，其他施在自己身上）
* `HP_WEAPON` = **4**（持此武器戰鬥攻擊時必定觸發；攻擊性魔法施在對手身上）
* `HP_BLOCK` = **8**（持此盾牌成功阻擋 block 時觸發；攻擊魔法施在對手身上）
* `HP_ARROW` = **16** (使用此箭矢擊中敵人時觸發)
