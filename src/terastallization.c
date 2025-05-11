/* Custom File. Give credits if used!
   ~ 1RWT16KU1D
*/

// For Terastallization

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
#include "../include/new/move_battle_scripts.h"
#include "../include/new/ram_locs.h"
#include "../include/new/terastallization.h"
#include "../include/field_weather.h"

// Exported Functions
extern u16 VarGet(u16 var);
extern bool8 VarSet(u16 var, u16 value);

// BattleScript(s)
extern u8 BattleScript_Terastallize[];

// Bag Helper Functions
extern u8 GetPocketByItemId(u16 itemId);
extern u8* GetBagItemQuantityPointer(u8 pocket, u8 bagId);
extern u16* GetBagItemsPointer(u8 pocket, u8 bagId);

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
    // Check if the player/opponent has Tera Orb
    if (SIDE(bank) == B_SIDE_PLAYER)
    {
        if (!CheckBagHasItem(ITEM_TERA_ORB, 1))
            return FALSE;
    }
    else if (SIDE(bank) == B_SIDE_OPPONENT)
    {
        if (!CheckBagHasItem(ITEM_TERA_ORB, 1))
            return FALSE;
    }

    return (!IsTerastallized(bank) && (GetTeraType(bank) != TYPE_BLANK));
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

// Main Function
u8 *DoTerastallize(u8 bank)
{
    if (!IsTerastallized(bank) && FlagGet(FLAG_TERA))
    {
        u8 teraType = GetTeraType(bank);

        SET_BATTLER_TYPE(bank, teraType);
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, teraType);
        gNewBS->teraData.done[GetBattlerSide(bank)][gBattlerPartyIndexes[bank]] = TRUE;
        FlagClear(FLAG_TERA);
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
