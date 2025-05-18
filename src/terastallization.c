/* Custom File. Give credits if used!
   ~ 1RWT16KU1D
*/

// For Terastallization
#include "defines.h"
#include "defines_battle.h"

#include "../include/battle_message.h"
#include "../include/battle.h"
#include "../include/event_data.h"
#include "../include/item.h"
#include "../include/mgba.h"
#include "../include/palette.h"
#include "../include/pokemon.h"

#include "../include/constants/items.h"
#include "../include/constants/vars.h"

#include "../include/gba/macro.h"

#include "../include/new/ai_util.h"
#include "../include/new/battle_indicators.h"
#include "../include/new/battle_script_util.h"
#include "../include/new/frontier.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/ram_locs.h"
#include "../include/new/terastallization.h"
#include "../include/field_weather.h"
#include "../include/random.h"
#include "../include/string_util.h"

// Exported Functions
extern u16 VarGet(u16 var);
extern bool8 VarSet(u16 var, u16 value);

// BattleScript(s)
extern u8 BattleScript_Terastallize[];

// Bag Helper Functions
extern u8 GetPocketByItemId(u16 itemId);
extern u8* GetBagItemQuantityPointer(u8 pocket, u8 bagId);
extern u16* GetBagItemsPointer(u8 pocket, u8 bagId);

#define TRAINER_ITEM_COUNT 4

static const item_t sTeraOrbTable[] =
{
	ITEM_TERA_ORB,
};

const u16 gTeraBlendColors[] =
{
    [TYPE_NORMAL]   = RGB(25, 25, 25),  // Light Gray
    [TYPE_FIGHTING] = RGB(27, 6, 4),    // Crimson Red
    [TYPE_FLYING]   = RGB(18, 22, 31),  // Sky Blue
    [TYPE_POISON]   = RGB(22, 2, 28),   // Deep Violet
    [TYPE_GROUND]   = RGB(16, 7, 1),    // Deep Brown
    [TYPE_ROCK]     = RGB(15, 13, 6),   // Dusty Beige
    [TYPE_BUG]      = RGB(18, 25, 6),   // Leafy Green
    [TYPE_GHOST]    = RGB(10, 6, 16),   // Haunting Indigo
    [TYPE_STEEL]    = RGB(19, 19, 23),  // Bluish Steel Gray
    [TYPE_MYSTERY]  = RGB(31, 31, 31),  // White
    [TYPE_FIRE]     = RGB(30, 12, 4),   // Fiery Orange
    [TYPE_GRASS]    = RGB(6, 26, 8),    // Forest Green
    [TYPE_WATER]    = RGB(8, 17, 31),   // Watery Blue?
    [TYPE_ELECTRIC] = RGB(31, 28, 5),   // Vivid Yellow
    [TYPE_PSYCHIC]  = RGB(31, 6, 18),   // Magenta
    [TYPE_ICE]      = RGB(20, 28, 31),  // Ice Blue
    [TYPE_DRAGON]   = RGB(8, 10, 31),   // Indigo
    [TYPE_DARK]     = RGB(5, 5, 5),     // Black
    [TYPE_FAIRY]    = RGB(31, 18, 24),  // Rose Pink
    [TYPE_BLANK]    = RGB(31, 31, 31),  // White
    [TYPE_STELLAR]  = RGB(28, 28, 31),  // Still unsure - Gave it a Silvery Blue Tint
};

// Check if the Pokemon has terastallized or not
bool8 IsTerastallized(u8 bank)
{
    return gNewBS->teraData.done[GetBattlerSide(bank)][gBattlerPartyIndexes[bank]];
}

// Fetch the Pokemon's current Tera Type
u8 GetTeraType(u8 bank)
{
    struct Pokemon *mon;

    if (GetBattlerSide(bank) == B_SIDE_PLAYER)
        mon = &gPlayerParty[gBattlerPartyIndexes[bank]];
    else
        mon = &gEnemyParty[gBattlerPartyIndexes[bank]];

    return mon->teraType;
}

// Fetch the Pokemon's Tera Type from OW (scripts)
void GetTeraTypeInOW(void)
{
    u8 partySlot = VarGet(Var8002); // Get Pokemon's index from script

    // Ensure Slot is Valid (0-5)
    if (partySlot >= PARTY_SIZE)
    {
        VarSet(VAR_TEMP_1, TYPE_BLANK); // Set to Blank to indicate an error
        return;
    }

    // Fetch Correct Pokemon's Tera Type
    u8 monTeraType = gPlayerParty[partySlot].teraType;

    // Ensure Tera Type is within range
    VarSet(VAR_TEMP_1, (monTeraType < NUMBER_OF_MON_TYPES) ? monTeraType : TYPE_BLANK);
}

// Change the Pokemon's Tera type in OW
void ChangeTeraTypeInOW(void)
{
    u8 partySlot = VarGet(Var8002);   // Get Pokemon's party index from script
    u8 newTeraType = VarGet(Var8001); // Get new Tera Type from script

    // Error Checks
    if (partySlot >= PARTY_SIZE)
        return;

    if (newTeraType >= NUMBER_OF_MON_TYPES)
        return;

    gPlayerParty[partySlot].teraType = newTeraType;
}

// Check whether Pokemon can Tera
bool8 CanTerastallize(u8 bank)
{
    #ifndef TERASTAL_FEATURE
		return FALSE;
	#else

    if (GetBattlerSide(bank) == B_SIDE_OPPONENT)
	{
        return TRUE;
    }
    else {
    if (FlagGet(FLAG_TERA_BATTLE) && !IsTerastallized(bank))
        return TRUE;

    return FALSE;
    }
    #endif
}

static bool8 IsItemTeraOrb(u16 item)
{
	for (u8 i = 0; i < ARRAY_COUNT(sTeraOrbTable); ++i)
	{
		if (item == sTeraOrbTable[i])
			return TRUE;
	}

	return FALSE;
}

static item_t FindTrainerTeraOrb(u16 trainerId)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK) || IsFrontierTrainerId(trainerId))
		return ITEM_TERA_ORB;

	for (u8 i = 0; i < TRAINER_ITEM_COUNT; ++i)
	{
		if (IsItemTeraOrb(gTrainers[trainerId].items[i]))
			return gTrainers[trainerId].items[i];
	}

	return ITEM_NONE;
}

static item_t FindPlayerTeraOrb(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK))
		return ITEM_TERA_ORB;

	for (u8 i = 0; i < ARRAY_COUNT(sTeraOrbTable); ++i)
	{
		if (CheckBagHasItem(sTeraOrbTable[i], 1))
			return sTeraOrbTable[i];
	}

	#ifdef DEBUG_TERASTAL
		return ITEM_TERA_ORB; //Give player Dynamax Band if they have none
	#endif

	return ITEM_NONE;
}

static item_t FindBankTeraOrb(u8 bank)
{
	#ifdef DEBUG_TERASRAL
		if (bank + 1)
			return ITEM_TERA_ORB;
	#endif

	if (SIDE(bank) == SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
		{
			if (GetBattlerPosition(bank) == B_POSITION_OPPONENT_LEFT)
				return FindTrainerTeraOrb(gTrainerBattleOpponent_A);
			else
				return FindTrainerTeraOrb(SECOND_OPPONENT);
		}
		else
			return FindTrainerTeraOrb(gTrainerBattleOpponent_A);
	}
	else //SIDE_PLAYER
	{
		if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
		{
			if (GetBattlerPosition(bank) == B_POSITION_PLAYER_RIGHT)
				return FindTrainerTeraOrb(VarGet(VAR_PARTNER));
			else
				return FindPlayerTeraOrb();
		}
		else
			return FindPlayerTeraOrb();
	}
}

bool8 TerastalEnabled(u8 bank)
{
	if (GetBattlerSide(bank) == B_SIDE_OPPONENT)
	{
		if (FindBankTeraOrb(bank) != ITEM_NONE)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (!FlagGet(FLAG_TERA_BATTLE))
			return FALSE;

		if (FindBankTeraOrb(bank) != ITEM_NONE)
			return TRUE;

		#ifdef DEBUG_TERASTAL
			return TRUE;
		#else
			return FALSE;
		#endif
	}
}

// Fades palette according to teraType
void FadeBankPaletteForTera(u8 bank, u16 paletteOffset)
{
    u8 teraType = GetTeraType(bank);

	if (IsTerastallized(bank))
	{
		BlendPalette(paletteOffset, 16, 6, gTeraBlendColors[teraType]);
		CpuCopy32(gPlttBufferFaded + paletteOffset, gPlttBufferUnfaded + paletteOffset, 32);
	}
}

// Main Function - Try type changes
u8 *DoTerastallize(u8 bank)
{
    if (!IsTerastallized(bank))
    {
        u8 teraType = GetTeraType(bank);
        u8 side = GetBattlerSide(bank);
        u8 partyIndex = gBattlerPartyIndexes[bank];

        gNewBS->teraData.done[side][partyIndex] = TRUE;

        gBattleScripting.bank = bank;

        // Because Stellar Tera Defensive Typing remains same
        if (teraType != TYPE_STELLAR)
            SET_BATTLER_TYPE(bank, teraType);
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, teraType);
        PREPARE_MON_NICK_BUFFER(gBattleTextBuff2, bank, partyIndex);

        return BattleScript_Terastallize;
    }
    return NULL;
}

// AI Logic for Terastallization
bool8 ShouldAIDelayTerastallization(u8 bankAtk, u8 bankDef, u16 move, bool8 optimizeAndLookAtTeraPotential, bool8 runDamageCalcs)
{
    if (optimizeAndLookAtTeraPotential && !CanTerastallize(bankAtk))
        return TRUE; // This bank can't Terastallize

    if (IsTerastallized(bankAtk)) // Is already Terastallized
        return FALSE;

    if (GetTeraType(bankAtk) == TYPE_BLANK)
        return TRUE;

    if (BATTLER_SEMI_INVULNERABLE(bankAtk))
        return TRUE; // Can't Terastallize this turn

    if (runDamageCalcs)
    {
        // Delay Terastallization if we can KO the opponent without it
        if (MoveWouldHitFirst(move, bankAtk, bankDef) // AI would attack first
        && CalculateMoveKnocksOutXHitsFresh(move, bankAtk, bankDef, 1)) // AI would KO in its base form
            return TRUE;

        // Delay Terastallization if opponent can KO us after we Terastallize
        if (!MoveWouldHitFirst(move, bankAtk, bankDef) // AI wouldn't attack first
        && MoveKnocksOutXHits(IsValidMovePrediction(bankDef, bankAtk), bankDef, bankAtk, 1)) // And foe would KO AI
            return TRUE;
    }
    return FALSE;
}

// givepokemon set-up
void SetGiftMonTeraType(void)
{
    u8 partySlot = VarGet(Var8002); // Slot passed from script
    if (partySlot >= PARTY_SIZE)
        return;

    struct Pokemon* mon = &gPlayerParty[partySlot];
    u16 species = mon->species;

    u8 type1 = gBaseStats[species].type1;
    u8 type2 = gBaseStats[species].type2;

    if (type1 == type2 || type2 == TYPE_MYSTERY)
        mon->teraType = type1;
    else
        mon->teraType = (Random() & 1 ) ? type1 : type2;
}
