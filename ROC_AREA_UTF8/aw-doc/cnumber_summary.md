# ROC MUD Constant & Numeric Parameters Summary

This document serves as an exhaustive reference of all numeric constant parameters, bitwise flags, and material effects in ROC MUD, parsed from the `cnumber.txt` specification file.

---

## 1. Material Table (#MATERIAL)
Materials modify statistics, apply flags, trigger spells, or provide racial advantages.

### Material ID Ranges:
* **`1~100`**: Reserved/Base Materials (e.g. `1` normal, `2` test, `3` flesh (corpses), `4` food, `5` liquid (potions), `6` herb, `7` metal (`ac -2`, `svLightning +1`)).
* **`101~200`**: Common Materials:
  * `101` cloth (`svfire +1`, `dodge -2`)
  * `102` leather (`svfire +1`, physical damage taken `-1`)
  * `103` wool (`svfire +1`, `hp +2`)
  * `104` wooden (`svfire +2`, `svearth -1`, `mp +2`)
  * `105` stone (`svpetrify +2`, `svearth -1`, physical damage taken `-1`)
  * `106` mud (low durability, `svpetrify +1`, `svLightning -1`, magic damage taken `-1`)
  * `107` paper (`svfire +2`, low durability, `mp +4`, `pp +1`)
  * `108` fiber (`svfire +1`, `dodge -2`)
  * `109` iron (`mv -1`, `ac -5`, `svLightning +2`, `svwind -1`, `dodge +2`, physical dmg taken `-2`, slayer vs elf/drow)
  * `110` bronze (`hp +2`, `ac -1`, `svfire -1`, `svLightning +2`, `svearth -1`, physical dmg taken `-1`, spell damage `+2`, slayer vs plant)
  * `111` brass (`mp +2`, `ac -1`, `svwater -1`, `svLightning +2`, `svwind -1`, physical dmg taken `-1`, spell damage `+2`, slayer vs animal)
  * `112` scale (`ac -1`, physical dmg taken `-1`)
  * `113` tin (`ac -2`, `svfire -1`, `svLightning +2`)
  * `114` lead (`ac -2`, `svLightning +2`, magic damage taken `-1`)
* **`201~300`**: High-Grade/Special Materials:
  * `201` steel (`mp -10`, `hp +10`, `mv -5`, `ac -8`, `svs +1`, `svLightning +2`, `dodge -2`, physical dmg taken `-4`, slayer vs drow/elf)
  * `202` silver (`mp +4`, `svpetrify -3`, `svs -1`, `svLightning +3`, `pp +1`, magic damage taken `-2`, slayer vs werewolf)
  * `203` gold (`mp +4`, `ac +2`, `svpetrify -2`, `svbreath -2`, `svLightning +3`, `pp +1`, magic damage taken `-1`, slayer vs demon)
  * `204` aluminum (`anti-neutral`, `mp +2`, `hp +2`, `svbreath -1`, `svs -1`, `svLightning +3`, magic damage taken `-1`, slayer vs insect/troll)
  * `205` ebony (`hp +3`, `mv +2`, `svparalyze -2`, `svs -1`, `svfire +2`, `svelement -2`, magic damage taken `-2`)
  * `206` juniper (`mp +8`, `svbreath -1`, `svs -1`, `svfire +2`, `svelement -1`, magic damage taken `-1`, Spells: `Protection` 10% rate)
  * `207` oak (`mp +8`, `ac -1`, `svbreath -1`, `svfire +2`, `svearth -1`, `pp +1`, Spells: `Protection` 10% rate)
  * `208` granite (`hp +5`, `mv -5`, `ac -1`, `svpetrify +2`, `svLightning -2`, `svearth +2`, physical dmg taken `-3`)
  * `209` marble (`mp +5`, `mv -5`, `ac -1`, `svpetrify +2`, `svLightning -2`, `svearth +2`, physical dmg taken `-3`)
  * `210` basalt (`mp -5`, `hp +10`, `mv -5`, `ac -1`, `svpetrify +2`, `svfire -2`, `svearth +2`, physical dmg taken `-3`)
  * `211` glass (fragile, `mp +5`, `ac +1`, `svpetrify -3`, `pp +1`, physical damage `+10`, magic damage `-10`)
  * `212` bone (`mp +1`, `hp -2`, `mv +4`, `svwand -1`, `svbreath +1`, `svearth -1`, physical dmg taken `-3`, magic damage `+2`, slayer vs elf/human/ghost)
  * `213` ivory (`mp +3`, `svs -2`, `svearth -2`, physical dmg taken `-2`, magic damage `+1`, slayer vs ghost)
  * `214` dragon bone (`mp +5`, `hp -10`, `mv -5`, `ac -1`, `svbreath -1`, `svs -1`, slayer vs dragon/worm)
  * `215` dragon scale (`hp -10`, `mv -2`, `svpetrify -3`, `svs -1`, `svbreath -2`, `pp +1`, slayer vs dragon/worm)
  * `216` dragon crystal (`hp +5`, `mp +5`, `ac -1`, magic damage taken `-2`, `svbreath -1`, `svfire -1`, slayer vs dragon/worm)
  * `217` feather (fragile, `mv +13`, `ac +1`, `svpetrify -3`, `svs -1`, `svfire +2`, `svwind +3`, `svelement -1`, `dodge -5`, magic damage taken `-1`)
  * `218` mithril (`svpetrify -2`, `svbreath -1`, `svs -1`, `svLightning +4`, `svelement -1`, magic damage taken `-1`, slayer vs ghost/undead/phantom/demon/vampire)
  * `219` magnetism (`mp +5`, `svpetrify -1`, `svs -1`, `svelement -1`, magic damage taken `-2`, Continuous spell: `detect magic`)
  * `220` ice (fragile, `mp +5`, `mv -3`, `svs -1`, `svfire -2`, `svwater +3`, magic damage taken `-2`, Spells: `Chill Touch` 10% rate)
  * `221` mercury (`mp +5`, `hp -5`, `mv +5`, `svparalyze -2`, `svLightning +2`, `svelement -2`, magic damage taken `-1`, Continuous: `detect magic`)
  * `222` sulfur (`mp -5`, `hp +5`, `mv +5`, `svwand -2`, `svfire +4`, `svelement -2`, magic damage taken `-1`, Continuous: `detect magic`)
  * `223` salt (`mp +5`, `hp +5`, `mv -5`, `svpetrify -2`, `svearth +4`, `svelement -2`, magic damage taken `-1`, Continuous: `detect magic`)
* **`301~400`**: Gemstones (All are expensive and fragile; modify `mp +5`, `svelement -1`, `svearth +3`, magic damage taken `-2` unless noted):
  * `301` gem
  * `302` jade (`svparalyze -2`, `svs -1`, Spells: `Protection` 3% rate, slayer vs ghost/undead)
  * `303` crystal (`svs -1`, `pp +1`, Spells: `Cure light` 3% rate, slayer vs ghost/undead)
  * `304` onyx (`hp +5`, `svs -1`, `svfire -1`, slayer vs ghost/demon)
  * `305` ruby (`svfire -2`, slayer vs ghost/orc/elf/vampire/plant)
  * `306` sapphire (`svLightning -2`, slayer vs ghost/mindflayer/fish/hydra/statue)
  * `307` emerald (`svwind -2`, slayer vs ghost/imp/pixie/mist/bat)
  * `308` topaz (`svearth -2`, `svwind +3`, slayer vs ghost/dwarf/gnome/hobbit/object)
  * `309` amethyst (`svbreath -2`, slayer vs ghost/dragon/snake/lizard/lizardman)
  * `310` diamond (`svwater -2`, slayer vs ghost/human/demon/elemental/animal)
* **`401~500`**: Rare Substances:
  * `401` platinum (fragile, `svwand -3`, `svpetrify -1`, `svbreath -1`, `svs -1`, `svLightning +3`, magic damage taken `-2`, Spells: `Protection` 3% rate, slayer vs ghost/undead/demon/vampire)
  * `402` obsidian (`ac -3`, `svpetrify -1`, `svbreath -1`, `svs -1`, `svearth +3`, physical dmg taken `-2`, Spells: `Detect Evil` 3% rate, slayer vs dwarf/god/angel/elf)
  * `403` chaos matter (`mp +10`, `svbreath -2`, `svs -2`, `svelement -2`, `pp +1`, physical damage `+10`, Level limit: `35`, slayer vs elf/dwarf/imp/hobbit/gnome)
  * `404` law matter (`hp +10`, `svbreath -2`, `svs -2`, `svelement -2`, `pp +1`, magic damage `+10`, Level limit: `35`, slayer vs drow/orc/beholder/lizardman/vampire)
  * `405` black matter (`ac +2`, `svs -1`, physical damage `+3`, magic damage `-3`, slayer vs god/angel)
  * `406` perfect matter (`mp +3`, `svs -1`, `svelement -1`, magic damage `-3`, magic damage power `+5`)
  * `407` radiant (`mp +5`, `hp -5`, `svs -1`, magic damage power `+5`, slayer vs human/elf/orc)
  * `408` aura (fragile, `svbreath +5`, `svs -2`, `svelement -2`, magic damage taken `-2`, magic damage power `+3`)
  * `409` fire aura (fragile, `svbreath +5`, `svfire -4`, `svwater +4`, `svs -2`, `svelement -2`, magic damage taken `-2`, magic damage power `+3`)
  * `410` water aura (fragile, `svbreath +5`, `svfire +4`, `svwater -4`, `svs -2`, `svelement -2`, magic damage taken `-2`, magic damage power `+3`)
  * `411` wind aura (fragile, `svbreath +5`, `svwind -4`, `svearth +4`, `svs -2`, `svelement -2`, magic damage taken `-2`, magic damage power `+3`)
  * `412` earth aura (fragile, `svbreath +5`, `svwind +4`, `svearth -4`, `svs -2`, `svelement -2`, magic damage taken `-2`, magic damage power `+3`)

---

## 2. Room Constants (#ROOMS)

### 2.1 ROOM_flags (Bitwise OR values)
* `DARK` = **1** (Needs light source)
* `NO_MOB` = **4** (NPCs cannot enter)
* `INDOORS` = **8** (Indoor space; weather-proof, affects certain spells)
* `UNDERGROUND` = **16** (Subterranean; no solar events, protects vampires)
* `PRIVATE` = **512** (Max 2 characters allowed)
* `SAFE` = **1024** (Peaceful zone; no attacks/PK)
* `SOLITARY` = **2048** (Max 1 character allowed)
* `PET_SHOP` = **4096** (Pet store; stores available pets in subsequent vnum room)
* `NO_RECALL` = **8192** (Recall command disabled)
* `CONE_OF_SILENCE` = **16384** (Silence zone; no chat/social commands)
* `GRAVE` = **32768** (Cemetery; unused)
* `SUMMON_BIT` = **65536** (Summoning disabled if caller or target is in this room)
* `AUCTION` = **131072** (Bidding and auctioning allowed)
* `SAVE` = **262144** (Players can save)
* `NO_TELEPORT_OUT` = **524288** (Teleport spell fails to exit)
* `NO_TELEPORT_IN` = **1048576** (Teleport spell fails to enter)
* `PK` = **2097152** (Safe PK zone; no death penalty, no exp loss)

### 2.2 SECT_types (Terrain Types)
* `0` = `INSIDE` (Always lit, indoor movement cost)
* `1` = `CITY` (Street)
* `2` = `FIELD` (Open grass)
* `3` = `FOREST` (Tree coverage)
* `4` = `HILLS` (High terrain)
* `5` = `MOUNTAIN` (Steep/rocky terrain)
* `6` = `WATER_SWIM` (Shallow water; swimming/boat required)
* `7` = `WATER_NOSWIM` (Deep water; boat/flying required)
* `8` = `UNDERWATER` (Submerged; underwater movement/breathing required)
* `9` = `AIR` (Flying required; cannot summon earth elementals here)
* `10` = `DESERT` (Arid sand; cannot summon fire elementals water-side)

### 2.3 Exits (Directs)
* `0` = North (`D0`)
* `1` = East (`D1`)
* `2` = South (`D2`)
* `3` = West (`D3`)
* `4` = Up (`D4`)
* `5` = Down (`D5`)

### 2.4 Door Flags (Bitwise OR values)
* `ISDOOR` = **1** (Exit has a door)
* `CLOSED` = **2** (Door is currently shut)
* `LOCKED` = **4** (Door is locked; requires a key vnum or `-1` for keyless)
* `BASHED` = **8** (Door was bashed open)
* `BASHPROOF` = **16** (Door cannot be bashed)
* `PICKPROOF` = **32** (Door cannot be picked)
* `PASSPROOF` = **64** (Cannot use pass door spell to pass through)

---

## 3. Mobile Constants (#MOBILES)

### 3.1 ACT_flags (Bitwise OR values)
* `IS_NPC` = **1** (Automatically added by engine)
* `SENTINEL` = **2** (Stationary; mandatory for shops, groupings, or actions)
* `SCAVENGER` = **4** (Picks up items on the floor)
* `AGGRESSIVE` = **32** (Attacks players on sight)
* `STAY_AREA` = **64** (Will not walk out of its starting area zone)
* `WIMPY` = **128** (Flees combat when critical health is reached)
* `PET` = **256** (Tamable pet)
* `TRAIN` = **512** (Can train players)
* `PRACTICE` = **1024** (Can practice players' skills)
* `CRITICAL` = **2048** (Has critical hit capability)
* `HARD_DIS` = **4096** (Has sword-flight/disarm martial techniques)
* `HUNT` = **8192** (Tracks fleeing players)
* `TWO_WEAPON` = **16384** (Can dual-wield)
* `ARTIFACT` = **65536** (Unused)
* `TACTICS` = **262144** (Has tactics skill)
* `TRACK` = **524288** (Tracks and pursues fleeing targets)
* `REGENERACY` = **1048576** (Has health regeneration)
* `GAMBLE` = **2097152** (Hosts gambling games)
* `BANKER` = **4194304** (Bank merchant)
* `MOB_AI` = **8388608** (Attacks the weakest target first; modifies priority based on AI levels below)
* `REFLEXION` = **16777216** (Reflects physical damage back to attacker)
* `NO_PAR` = **33554432** (Killing repeatedly does not diminish player exp yields)
* `IGNORE_NO_MOB` = **67108864** (Can walk into `NO_MOB` rooms)
* `SMITH` = **268435456** (Can repair equipment)

### 3.2 AFF_flags (Active Spells/States - Bitwise OR)
* `BLIND` = **1** | `INVISIBLE` = **2** | `DETECT_EVIL` = **4** | `DETECT_INVIS` = **8** | `DETECT_MAGIC` = **16** | `DETECT_HIDDEN` = **32**
* `HOLD` = **64** (Stunned; cannot walk or chat)
* `SANCTUARY` = **128** (Sanc; half damage)
* `FAERIE_FIRE` = **256** (Reduces target's AC)
* `INFRARED` = **512** (Night vision)
* `CURSE` = **1024** (Cursed status)
* `CHANGE_SEX` = **2048** (Altered gender)
* `POISON` = **4096** (Poisoned tick damage)
* `PROTECT` = **8192** (Divine ward)
* `POLYMORPH` = **16384** (Shape-shifted)
* `SNEAK` = **32768** (Quiet movement)
* `HIDE` = **65536** (Invisible on room lookup)
* `SLEEP` = **131072** (Sleeping)
* `CHARM` = **262144** (Charmed follow/obey status)
* `FLYING` = **524288** (Flying)
* `PASS_DOOR` = **1048576** (Incorporeal; passes doors)
* `WATERWALK` = **2097152** (Walks on water terrain)
* `SUMMONED` = **4194304** (Summoned by an Illusionist)
* `MUTE` = **8388608** (Silenced; cannot speak)
* `SEALMAGIC` = **16777216** (Cannot cast magic)
* `FLAMING` = **134217728** (Flame shield; deal return damage)
* `PARALYZED` = **268435456** (Stunned/immobile)
* `CRAZY` = **536870912** (Confused/frenzied)
* `WEAKEN` = **1073741824** (Weakened stats)
* `NIGHTSTALK` = **2147483648** (Darkness stalking)
* `DARK_RAID` = **4294967296** (Low-light night vision)
* `MANASHIELD` = **8589934592** (Absorbs damage using MP)
* `GILLS` = **17179869184** (Water breathing)
* `TARDY` = **34359738368** (Slowed status)
* `MYSTIC_MIRROR` = **68719476736** (Reflects spell attacks; dispellable)
* `MYSTIC_ARMOR` = **137438953472** (Converts spell damage taken into MP)
* `CONTINUE_CURE` = **549755813888** (Continuous healing over time)
* `AFF_DISEASE` = **1099511627776** (Disease; loses MP & MV over time)
* `AFF_FEAR` = **2199023255552** (Fear; forced flee actions)

### 3.3 Mob Special Number (P-Parameters in Mob Headers - Bitwise OR)
* `NO_DM` = **1** (Immune to Dispel Magic)
* `NO_LORE` = **2** (Lore command fails on mob)
* `NO_BLIND` = **4** (Immune to blindness)
* `NO_CHARM` = **8** (Immune to charm)
* `NO_CURSE` = **16** (Immune to curse)
* `NO_FAEFIRE` = **32** (Immune to faerie fire)
* `NO_POISON` = **64** (Immune to poison and disease)
* `NO_SLEEP` = **128** (Immune to sleep)
* `HALF_MAGIC` = **256** (Takes 50% magic damage)
* `ANTI_MAGIC` = **512** (Immune to all magic)
* `REF_MAGIC` = **1024** (Reflects spell attacks permanently; non-dispellable)
* `ALY_SAN` = **2048** (Permanent Sanctuary)
* `ALY_INVIS` = **4096** (Permanent Invisibility)
* `ALY_HIDE` = **8192** (Permanent Hide)
* `ALY_SNEAK` = **16384** (Permanent Sneak)
* `ALY_DI` = **32768** (Permanent Detect Invis)
* `ALY_DH` = **65536** (Permanent Detect Hidden)
* `ALY_INF` = **131072** (Permanent Infrared)
* `ALY_BAR` = **262144** (Permanent Mana Shield)
* `NO_TARDY` = **524288** (Immune to slow/tardy)
* `NO_MUTE` = **1048576** (Immune to silence/mute)
* `NO_SEALMAGIC` = **2097152** (Immune to magic seal)
* `EXPERT` = **4194304** (Expert mob; high exp return, difficult encounter)
* `NO_CRAZY` = **8388608** (Immune to confusion)
* `MOBAI_LV2` = **16777216** (Targets physically weakest player based on HP, AC, Dodge ratios)
* `MOBAI_LV3` = **33554432** (Maximum artificial intelligence targeting)
  * *Note*: If neither `LV2` nor `LV3` are set, but `MOB_AI` is active, it default to `LV1` targeting priority: `Savant/Bishop` > `Wizard/Alchemist` > `Lord` > `Cleric/Mage`.
* `NO_PARA` = **67108864** (Immune to paralyze)
* `SPELL_FURY` = **268435456** (Has spell fury combo skills)
* `NO_FEAR` = **536870912** (Immune to fear status)

### 3.4 Mob Classes
* `NORMAL` = **0** | `WARRIOR` = **1** (+30% to 50% dmg, extra attacks) | `MAGE` = **2** (Bonus MP pools) | `THIEF` = **4** (Backstab/crit enabled, +30% dodge) | `SWORDSMAN` = **8** (Swordmaster combat abilities) | `SHOT ARROW` = **16** (Ranged arrow bonus damage) | `BLOODY` = **32** (Lifesteal/frenzy)

---

## 4. Object Constants (#OBJECTS)

### 4.1 ITEM_types (Object Categories)
* `1` = `LIGHT` | `2` = `SCROLL` | `3` = `WAND` | `4` = `STAFF` (Must be level 25+ to impact all enemies/allies)
* `5` = `WEAPON` | `8` = `TREASURE` | `9` = `ARMOR` | `10` = `POTION` | `12` = `FURNITURE` | `13` = `TRASH`
* `15` = `CONTAINER` | `17` = `DRINK_CON` | `18` = `KEY` | `19` = `FOOD` | `20` = `MONEY` | `22` = `BOAT`
* `23` = `CORPSE_NPC` | `24` = `CORPSE_PC` | `25` = `FOUNTAIN` | `26` = `PILL` | `27` = `ARROW` | `28` = `DART` (Unused)

### 4.2 Extra ITEM_flags (Bitwise OR values)
* `GLOW` = **1** (Glows in the dark; increases value)
* `HUM` = **2** (Hums; slows down durability decay rate)
* `DARK` = **4** (Shadow-cloaked; gives 1% lifesteal or 1% soulsteal chance)
* `LOCK` = **8** (Locked container)
* `EVIL` = **16** (Corrupt item; 20% base damage boost but breaks twice as fast)
* `INVIS` = **32** (Invisibility)
* `MAGIC` = **64** (Magical item; cannot be enchanted; grants +20% damage vs vampires, undead, githyanki, mists, ghosts, mindflayers, phantoms)
* `NODROP` = **128** (Cannot drop/get rid of)
* `BLESS` = **256** (Blessed item; immune to dragon breath elements)
* `ANTI_GOOD` = **512** | `ANTI_EVIL` = **1024** | `ANTI_NEUTRAL` = **2048** (Alignment restrictors)
* `NOREMOVE` = **4096** (Cursed equip; cannot unequip)
* `INVENTORY` = **8192** (Unsaved container item; disappears if removed from containers/corpses)
* `NOSAVE` = **16384** (Item is lost when logging out)
* `VAMPIRE_BANE` = **32768** (Repels vampire usage)
* `HOLY` = **65536** (Sacred; weapons get +0 to 4 HR; armor gets +0 to 3 AC; burns evil wearers)
* `POISONED` = **131072** (Item is poisoned)
* `REMOVE_NODROP` = **262144** (Becomes `NODROP` immediately upon dropping on the floor)
* `DONATION_PIT` = **524288** (Donation repository; limit 1 per zone)
* `UNIQUE` = **1048576** (Unique item; limits online instances to 1, fallback to replica)
* `CLAN` = **2097152** (Clan craft item; sell cost set to 0)
* `PERSONAL` = **4194304** (Bound gear; returns to owner's inventory upon death)
* `LORE` = **16777216** (Lore limit; player can only equip 1 item sharing this exact vnum)
* `NO_SAC` = **33554432** (Cannot be sacrificed/donated)

### 4.3 WEAR_flags (Equip Locations - Bitwise OR)
* `TAKE` = **1** (Can be picked up into inventory)
* `WEAR_FINGER` = **2** | `WEAR_NECK` = **4** | `WEAR_BODY` = **8** | `WEAR_HEAD` = **16** | `WEAR_LEGS` = **32**
* `WEAR_FEET` = **64** | `WEAR_HANDS` = **128** | `WEAR_ARMS` = **256** | `WEAR_SHIELD` = **512** | `WEAR_ABOUT` = **1024**
* `WEAR_WAIST` = **2048** | `WEAR_WRIST` = **4096** | `WIELD` = **8192** (Weapon) | `HOLD` = **16384**
* `TWO_HAND` = **32768** | `ARROW` = **65536**

### 4.4 APPLY_types (Statistics modification - used in A parameter)
* `0` = `NONE` | `1` = `STR` | `2` = `DEX` | `3` = `INT` | `4` = `WIS` | `5` = `CON` | `6` = `SEX` | `7` = `CLASS` | `8` = `LEVEL` | `9` = `AGE` | `10` = `HEIGHT` | `11` = `WEIGHT`
* `12` = `MANA` | `13` = `HIT` (HP) | `14` = `MOVE` (Stamina) | `15` = `GOLD`
* `16` = `EXP` (Increases/decreases kill experience multiplier; used on mobiles)
* `17` = `AC` (Defense; negative value improves defense)
* `18` = `HITROLL` (HR) | `19` = `DAMROLL` (DR)
* `20` = `SAVING_PARA` (Paralyze save) | `21` = `SAVING_ROD` (Wands/rod save) | `22` = `SAVING_PETRI` (Petrify save) | `23` = `SAVING_BREATH` (Breath save) | `24` = `SAVING_SPELL` (Spell save) | `27` = `SAVING_FIRE` (Fire save) | `28` = `SAVING_COLD` (Cold save) | `29` = `SAVING_LIGHTNING` (Lightning save) | `30` = `SAVING_EARTH` (Earth save) | `31` = `SAVING_WIND` (Wind save) | `35` = `SAVING_ELEMENT` (All elemental saves: Fire/Water/Wind/Earth combined)
* `38` = `RACE` (Alters character race)
* `39` = `DODGE` (Alters dodge capacity)
* `40` = `POWER_POTENTIAL` (Increases/decreases magic potential stats)
* `41` = `REDUCE_DAMAGE` (Modifies physical damage reduction threshold)
* `42` = `REDUCE_MAGIC` (Modifies magical damage reduction threshold)
* `43` = `APPLY_ADAMAGE` (Flat physical damage modifier)
* `44` = `APPLY_AMAGIC` (Flat magic damage modifier)
* `45` = `APPLY_GPHYSICAL` (NPC physical block barrier; hits below this value deal 0 damage)
* `46` = `APPLY_GMAGIC` (NPC magic block barrier; magic hits below this value deal 0 damage)

### 4.5 Class Restrictions (P-Parameter allow/deny values)
To restrict items to specific classes, sum up class bits. Add `1048576` (ALLOW_BIT) for an exclusive "ONLY these classes can equip" condition, or `2097152` (DENY_BIT) for a "these classes CANNOT equip" condition.
* `MAGE` = **1** | `CLERIC` = **2** | `THIEF` = **4** | `WARRIOR` = **8** | `BARBARIAN` = **16** | `WIZARD` = **32** | `ALCHEMIST` = **64** | `SAVANT` = **128** | `BISHOP` = **256** | `ASSASSIN` = **512** | `NINJA` = **1024** | `KNIGHT` = **2048** | `SWORDSMAN` = **4096** | `MONK` = **8192** | `SHAMAN` = **16384** | `SORCERER` = **32768** | `LORD` = **65536** | `RANGER` = **131072** | `WARLORD` = **262144** | `MARTIALMAN` = **524288**

### 4.6 Race Restrictions (R-Parameter allow/deny values)
Similar to class bits, sum up race bits and combine with `1048576` (ALLOW_BIT) or `2097152` (DENY_BIT).
* `HUMAN` = **1** | `ELF` = **2** | `DWARF` = **4** | `ORC` = **8** | `HOBBIT` = **16** | `IMP` = **32** | `GNOME` = **64** | `DROW` = **128** | `VAMPIRE` = **256** | `BEHOLDER` = **512** | `LIZARDMAN` = **1024** | `WEREWOLF` = **2048** | `MUMMY` = **4096**

---

## 5. Hidden Power Trigger Times (H-Parameter in Objs/Mats)
* `HP_WEAR` = **1** (Triggers on equipment equip; applies offensive/defensive to self)
* `HP_FIGHT` = **2** (Triggers randomly in combat; applies offensive to foe, others to self)
* `HP_WEAPON` = **4** (Triggers on main weapon combat hit; applies offensive to foe, others to self)
* `HP_BLOCK` = **8** (Triggers when shielding/blocking an attack; applies offensive to foe, others to self)
* `HP_ARROW` = **16** (Triggers when hit by arrow projectile; applies offensive to foe, others to self)

---

## 6. Equipment Wear Locations (RESETS section E parameter)
* `-1` = Carry in inventory
* `0` = `LIGHT` | `1` = `FINGER_L` | `2` = `FINGER_R` | `3` = `NECK_1` | `4` = `NECK_2` | `5` = `BODY` | `6` = `HEAD`
* `7` = `LEGS` | `8` = `FEET` | `9` = `HANDS` | `10` = `ARMS` | `11` = `SHIELD` | `12` = `ABOUT` | `13` = `WAIST`
* `14` = `WRIST_L` | `15` = `WRIST_R` | `16` = `WIELD` (Primary Weapon) | `17` = `HOLD` | `18` = `WIELD_2` (Secondary Weapon)
* `19` = `TWO_HAND` | `20` = `ARROW`
* `22` = `FINGER_3` (Beholder species) | `23` = `FINGER_4` (Beholder species)
