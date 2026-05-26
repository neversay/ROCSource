# ROC MUD Area File (.are) Format Summary

This document serves as a comprehensive reference guide for the **ROC MUD Area File (`.are`)** format, based on the `carea.txt` documentation. ROC MUD is an Envy/Merc 2.2 derivative with extensive customization by Neversay and other ROC developers. 

---

## 1. General Structure & Data Types

An area file is composed of several sections, each starting with a specific header prefix (e.g., `#AREA`, `#MOBILES`). Sections must appear in a logical order (specifically, `#MATERIAL` must precede `#OBJECTS`). The file terminates with the `#$` marker.

### Data Types parsed by the Engine:
* **`letter`**: A single non-whitespace character.
* **`word`**: A sequence of non-whitespace characters ending with whitespace.
* **`string`**: A sequence of characters ending with a tilde (`~`). Can span multiple lines and contain spaces.
* **`number`**: Integer values. Can use the bitwise OR symbol `|` to combine flags (e.g., `1|64` yields `65` internally since all `|` values are added). Some parameters also support string constants equivalent to numeric values (e.g. `ACT_AGGRESSIVE` instead of `32`).
* **`number as string`**: A number represented inside a tilde-terminated string (e.g., `100~`).
* **`vnum`**: Virtual Numbers uniquely identifying rooms, mobiles, and objects. Range: `1` to `1,000,000`. Traditionally, an area shares a specific block of vnums (e.g., `3000` to `3399`).

---

## 2. Section Reference

### 2.1 #AREADATA
Defines metadata for the area.
* **Syntax Format**:
  ```text
  #AREADATA
  Name        [String] Level recommendation & Area display name (e.g., { ALL } Skara Brae~)
  Identity    [String] ID used in MobProgs (e.g., skarabrae~)
  Builders    [String] Original authors~
  Translator  [String] Translator~
  Modifier    [String] Editor/Modifier~
  VNUMs       [Number] [Number] (Start and End of VNUM range for OLC)
  Security    [Number] Security clearance/permission for editing (1-9)
  Recall      [Number] VNUM of the recall point (Default: 3001)
  Death       [Number] VNUM of death/respawn room (Optional; defaults to Recall VNUM)
  F_toggle    [Number] (Optional) If 1, room flags in this area are inverted. If 0, normal.
  End
  ```

---

### 2.2 #HELPS
Defines help documentation built into the area file.
* **Syntax Format**:
  ```text
  #HELPS
  [level:number] [keywords:string]
  [help-text:string]
  0 $~  (Terminator)
  ```
  * *Note*: If `level` is negative, the help file has special system functions (e.g., `-1` for login greetings).

---

### 2.3 #MATERIAL
ROC custom section for defining item materials. Must precede `#OBJECTS`.
* **Syntax Format**:
  ```text
  #MATERIAL
  #[vnum:number]
  [name:string]
  [cname:string]
  [unused:string]
  [unused:string]
  [clan-lv:number] [extra-flags:number] [clan-cost:number]
  [hr_mod_p:number as string] [dr_mod_p:number as string] [ac_mod_p:number as string] [durability:number as string]
  [weight_mod_p:number] [cost_mod_p:number] [cost-per-day:number]
  { A [apply-type:number] [apply-value:number] }   (Multiple allowed; stat modifications)
  { H [spell-slot:number] [percent:number] [level:number] [time:number] } (Multiple allowed; spell trigger: 1=wear, 2=fight, 4=weapon combat, 8=shield block, 16=arrow hit)
  { L [obj-level:number] }                        (Max 1; sets level limit)
  { P [obj-permit:number] }                       (Max 1; class permissions)
  { R [race-permit:number] }                      (Max 1; race permissions)
  { V [race:string] [hr_mod_q] [hr_mod_p] [dr_mod_q] [dr_mod_p] } (Multiple allowed; racial combat modifiers)
  { F [spell:number/string] }                     (Multiple allowed; continuous spell buff while worn, e.g. `F AFF_FLYING`)
  #0    (Terminator)
  ```

---

### 2.4 #MOBILES
Defines NPC characters (Mobiles).
* **Syntax Format**:
  ```text
  #MOBILES
  #[vnum:number]
  [keywords:string]
  [short-description:string]
  [long-description:string]
  [description:string]
  [act-flags:number/string] [affected-flags:number/string] [alignment:number] S
  [level:number] [mod_hitplus] [mod_mana] [mod_hitroll] d [mod_damroll] + [mod_ac] [mod_svs] d [dodge] + [unused]
  [gold:number] [unused]
  [unused] [position:number] [sex:number]
  { R [mob_race:string] }          (Race name, e.g. Dragon~)
  { C [mob_class:number/string] }  (Class: 0=NORMAL, 1=WARRIOR, 2=MAGE, 4=THIEF, 8=SWORD, 16=SHOT ARROW, 32=BLOODY. Can be combined with `|`)
  { P [mob_spell_spec:number] }    (Spell/behavior modifications, e.g. 8 for No Charm)
  { W [skill-name:string] [percent] [damage] } (Weaknesses/resistances; e.g. `W magic missile~ 200 300` makes NPC take 3x damage from Magic Missile)
  { S [apply-type] [degree] }      (Stat boost modifiers; e.g. `S 40 5` adds 5 magic potential)
  { K [skills~] }                  (Skills the NPC can teach if practicing. Prefix with `*` to require a matching qmark)
  { J [join-classes~] }            (Classes this NPC can transfer players to)
  { A[hour:number]                 (Daily routine/schedule action list)
    [action1]+[action2]...
    ~                              (Routines are separated by `~` and terminated by `|`)
  }
  #0    (Terminator)
  ```

---

### 2.5 #MOBPROGS & #ROOMPROGS
Maps vnums to specialized, modular trigger script files (`.prg`) located in `area/MOBProgs/` and `area/ROOMProgs/`.
* **Syntax Format**:
  ```text
  #MOBPROGS
  M [mob-vnum:number] [program-file-name:string]
  S   (Terminator)
  ```
  ```text
  #ROOMPROGS
  R [room-vnum:number] [roomprog-file-name:string]
  S   (Terminator)
  ```

---

### 2.6 #OBJECTS
Defines items.
* **Syntax Format**:
  ```text
  #OBJECTS
  #[vnum:number]
  [keywords:string]
  [short-description:string]
  [long-description:string]
  [action-description:string]
  [item-type:number] [extra-flags:number] [wear-flags:number]
  [value-0:number as string] [value-1:number as string] [value-2:number as string] [value-3:number as string]
  [weight:number] [cost:number] [level-limited:number] (level-limited prevents high levels from abusing newbie gear)
  { E [keyword:string] [description:string] } (Multiple allowed; extra description when looked at)
  { A [apply-type:number] [apply-value:number] } (Multiple allowed; stat modifications)
  { H [spell-slot] [percent] [level] [time] } (Multiple allowed; spell trigger)
  { L [obj-level:number] }                    (Max 1; sets level limit)
  { T [obj-timer:number] }                    (Max 1; ticks until item decays/disappears)
  { C [obj-cost:number] }                     (Max 1; cost override, should match base cost)
  { P [obj-permit:number] }                   (Max 1; class limits)
  { R [race-permit:number] }                  (Max 1; race limits)
  { M [trigger_event:0/1] [message:string] }  (Max 2; message played when equipped (0) or removed (1))
  { X [material:string] }                     (Max 1; sets item material)
  { V [race:string] [hr_mod_q] [hr_mod_p] [dr_mod_q] [dr_mod_p] } (Multiple allowed; racial combat modifier)
  { F [spell:number/string] }                 (Multiple allowed; continuous spell buff while worn)
  { U [replica-vnum:number] }                 (Max 1; unique item marker. Spawn replica if unique already exists online)
  #0    (Terminator)
  ```

#### Common Item Types & `value[0-3]` Settings:
* **01 ITEM_LIGHT**: `v2` = Light duration in ticks (`0` = empty, `-1` = infinite).
* **02 ITEM_SCROLL**: `v0` = Spell level, `v1` = Spell 1, `v2` = Spell 2, `v3` = Spell 3.
* **03 ITEM_WAND** / **04 ITEM_STAFF**: `v0` = Spell level, `v1` = Max charges, `v2` = Current charges, `v3` = Spell name.
* **05 ITEM_WEAPON**: 
  * `v0` = Durability (`-1` for indestructible; otherwise `xxx1yyy` representing current/max values, or single integer if brand new).
  * `v1` = Min damage, `v2` = Max damage.
  * `v3` = Damage type (e.g. `00` hit, `01` slice, `02` stab, `03` slash, `11` pierce, `14` shot).
* **09 ITEM_ARMOR**: `v0` = AC (`0` for auto-set), `v1` = Acid sprayed count, `v2` = Durability.
* **10 ITEM_POTION** / **26 ITEM_PILL**: `v0` = Spell level, `v1` = Spell 1, `v2` = Spell 2, `v3` = Spell 3.
* **15 ITEM_CONTAINER**: `v0` = Capacity weight, `v1` = Flags (`1` closable, `2` unpickable, `4` closed, `8` locked), `v2` = Key VNUM, `v3` = Weight reduction (positive = % weight reduction, negative = items counted reduction).
* **17 ITEM_DRINK_CON**: `v0` = Capacity, `v1` = Current liquid, `v2` = Liquid type (0=Water, 1=Beer, 2=Wine, 3=Ale, 13=Blood, etc.), `v3` = Poisoned status (`!=0` is poisoned).
* **19 ITEM_FOOD**: `v0` = Food status, `v2` = Food lifetime, `v3` = Poisoned status (`!=0` is poisoned).
* **27 ITEM_ARROW**: `v0` = Arrow count, `v1` = Min dmg modification, `v2` = Max dmg modification, `v3` = Extra damage.

---

### 2.7 #ROOMS
Defines environmental spaces (Rooms).
* **Syntax Format**:
  ```text
  #ROOMS
  #[vnum:number]
  [name:string]
  [description:string]
  [area:number] [room-flags:number/string] [sector-type:number/string]
  { D[door:number]                  (Defines an exit direction: 0=N, 1=E, 2=S, 3=W, 4=U, 5=D)
    [description:string]            (Description of exit)
    [keywords:string]               (Keywords for door commands)
    [locks:number] [key:number] [to_room:number] (Locks: 1=ISDOOR, 2=CLOSED, 4=LOCKED, 8=BASHED, 16=BASHPROOF, 32=PICKPROOF, 64=PASSPROOF)
  }
  { E [keywords:string] [description:string] } (Multiple allowed; extended descriptions)
  S     (Terminator for this room)
  #0    (Terminator for the section)
  ```

---

### 2.8 #RESETS
Prescribes instructions executed when starting or repopulating the area (Resets occur every 3 min if empty, 15 min if players present).
* **Syntax Format**:
  * **Comment**: `* [comment text]`
  * **Load NPC**: `M 0 [mob-vnum] [limit] [room-vnum]` (Load NPC into room, up to world-wide limit)
  * **Load Object**: `O 0 [obj-vnum] 0 [room-vnum]` (Load object in room)
  * **Put in Container**: `P 0 [obj-vnum] 0 [container-vnum]` (Puts object in the last loaded object container)
  * **Give to NPC**: `G 0 [obj-vnum] 0` (Give object to the last loaded NPC)
  * **Equip NPC**: `E 0 [obj-vnum] 0 [wear-loc]` (Equips last loaded NPC at `wear-loc`)
  * **Door State**: `D 0 [room-vnum] [door-dir] [state]` (Set door state: 0=open, 1=closed, 2=locked)
  * **Randomize Exits**: `R 0 [room-vnum] [last-door]` (Randomizes exit directions up to `last-door` for mazes)
  * **Group Mobiles**: `F [follower-vnum] [room-vnum] [protect] [leader-vnum]` (Group mobiles together; protects leader if `protect` != 0)
  * **Section Terminator**: `S`

#### NPC Equipment Wear Locations (`wear-loc` values):
* `-1` NONE (carrying) | `0` LIGHT | `1` FINGER_L | `2` FINGER_R | `3` NECK_1 | `4` NECK_2 | `5` BODY | `6` HEAD | `7` LEGS | `8` FEET | `9` HANDS | `10` ARMS | `11` SHIELD | `12` ABOUT | `13` WAIST | `14` WRIST_L | `15` WRIST_R | `16` WIELD (Main Weapon) | `17` HOLD | `18` WIELD_2 (Secondary Weapon) | `19` TWO_HAND | `20` ARROW | `22` FINGER_3 | `23` FINGER_4

---

### 2.9 #SHOPS
Sets up trade merchants.
* **Syntax Format**:
  ```text
  #SHOPS
  [keeper-vnum] [trade-0] [trade-1] [trade-2] [trade-3] [trade-4] [profit-buy] [profit-sell] [open-hour] [close-hour]
  0     (Terminator)
  ```
  * `keeper-vnum`: VNUM of the NPC shopkeeper.
  * `trade-0` to `trade-4`: Item types the merchant buys (0 if unused).
  * `profit-buy`: Cost multiplier when players buy (100 = base, 150 = 50% markup).
  * `profit-sell`: Cost multiplier when players sell (100 = base, 75 = 25% discount).
  * `open-hour` & `close-hour`: Store hours in 24h format (e.g., `0 23` for 24 hours).

---

### 2.10 #SPECIALS
Assigns specialized C behaviors/functions to NPCs.
* **Syntax Format**:
  ```text
  #SPECIALS
  M [mob-vnum] [spec-fun-name]
  S   (Terminator)
  ```
  * Example: `M 3011 spec_executioner` maps VNUM 3011 to executioner behavior.

---

### 2.11 #$
The absolute end-of-file marker for the `.are` area file.
