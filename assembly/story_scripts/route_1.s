.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_RaidBattleScript

EventScript_RaidBattleScript:
    lock
    faceplayer
    setvar 0x8005 TYPE_GRASS
    setflag 0x820 @ FLAG_BADGE01_GET
    setflag 0xA08 @ FLAG_TERA_RAID_BATTLE
    special SPECIAL_IS_RAID_BATTLE_AVAILABLE
    compare LASTRESULT 0x0
    if 0x1 _goto RaidScript_NoBattle
    msgbox StartRaidBattle MSG_SIGN
    fadescreen FADEOUT_BLACK
    special SPECIAL_RAID_BATTLE_INTRO
    waitstate
    compare LASTRESULT 0x0
    if 0x1 _goto RaidScript_End
    setflag 0x908 @ FLAG_TAG_BATTLE
    msgbox PartnerTeamedUp MSG_SIGN
    special SPECIAL_CREATE_RAID_MON
    special SPECIAL_START_RAID_BATTLE
    waitstate
    special2 LASTRESULT 0xB4
    compare LASTRESULT 0x4
    if 0x1 _goto RaidScript_End
    compare LASTRESULT 0x5
    if 0x1 _goto RaidScript_End
    special SPECIAL_SET_RAID_BATTLE_FLAG
    setvar 0x4000 0x0

RaidScript_GiveReward:
    msgbox PartnerTeamedUp MSG_SIGN
    special SPECIAL_GIVE_RAID_BATTLE_REWARDS
    compare LASTRESULT 0x0
    if 0x1 _goto RaidScript_End
    callstd MSG_FIND
    goto RaidScript_GiveReward

RaidScript_End:
    release
    end

RaidScript_NoBattle:
    msgbox NoRaidAvailable MSG_NORMAL
    release
    end
