.thumb
.align 2

.include "../asm_defines.s"
.include "../battle_script_macros.s"

.global BattleScript_Terastallize

BattleScript_Terastallize:
	call BS_FLUSH_MESSAGE_BOX
	playanimation BANK_SCRIPTING ANIM_CALL_BACK_POKEMON
	waitanimation
	pause DELAY_1SECOND
	pause DELAY_HALFSECOND
	returntoball BANK_SCRIPTING
	waitstateatk
	callasm BackupScriptingBankMoveSelectionCursor
	switchinanim BANK_SCRIPTING 0x1
	waitanimation
	setword BATTLE_STRING_LOADER gText_TeraCompletedString
	printstring 0x184
	waitmessage DELAY_1SECOND
    end3
