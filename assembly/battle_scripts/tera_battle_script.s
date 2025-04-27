.thumb
.align 2

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_Terastallize

BattleScript_Terastallize:
	setword BATTLE_STRING_LOADER gText_TeraCompletedString
	printstring 0x184
	waitmessage DELAY_1SECOND
    end3
