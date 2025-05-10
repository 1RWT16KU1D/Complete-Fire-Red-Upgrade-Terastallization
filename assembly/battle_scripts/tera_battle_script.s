.text
.thumb
.align 2

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_Terastallize

BattleScript_Terastallize:
	setword BATTLE_STRING_LOADER TeraReactingString
	printstring 0x184
	waitmessage DELAY_HALFSECOND

TeraAnimBS:
	playanimation BANK_SCRIPTING ANIM_TERASTAL 0x0
	callasm BackupScriptingBankMoveSelectionCursor
	reloadhealthbar BANK_SCRIPTING
	setword BATTLE_STRING_LOADER gText_TeraCompletedString
	printstring 0x184
	waitmessage DELAY_1SECOND
    end3

TeraReactingString: .byte 0xFD, 0x0F, 0x00, 0xDD, 0xE7, 0x00, 0xE7, 0xE8, 0xE3, 0xE6, 0xDD, 0xE2, 0xDB, 0xFE, 0xD9, 0xE2, 0xD9, 0xE6, 0xDB, 0xED, 0xAB, 0xFF
