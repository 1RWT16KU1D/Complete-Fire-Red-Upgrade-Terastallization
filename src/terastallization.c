/* Custom File. Give credits if used!
   ~ 1RWT16KU1D
*/

// For Terastallization

#include "defines_battle.h"

#include "../include/battle_message.h"
#include "../include/battle.h"
#include "../include/event_data.h"
#include "../include/pokemon.h"

#include "../include/constants/vars.h"

#include "../include/new/battle_script_util.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/terastallization.h"
#include "../include/field_weather.h"

// Defines for later functions
#define BATTLER_HP(bank) gBattleMons[bank].hp

// Exported Functions
extern u16 VarGet(u16 var);
extern bool8 VarSet(u16 var, u16 value);

// BattleScript(s)
extern u8 BattleScript_Terastallize[];

// Check if the Pokemon has terastallized or not
bool8 IsTerastallized(u8 bank)
{
    return gNewBS->teraData.done[bank];
}

// Fetch the Pokemon's current Tera Type
u8 GetTeraType(u8 bank)
{
    return gPlayerParty[gBattlerPartyIndexes[bank]].teraType;
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
    u8 partySlot = VarGet(Var8002);   // Get Pokemon’s party index from script
    u8 newTeraType = VarGet(Var8001); // Get new Tera Type from script

    // Error Checks
    if (partySlot >= PARTY_SIZE)
        return;

    if (newTeraType >= NUMBER_OF_MON_TYPES)
        return;

    gPlayerParty[partySlot].teraType = newTeraType;
}

// Change back the Pokemon's types to what they were before
void RevertMonTypes(u8 bank)
{
    u8 originalType1 = gBaseStats[gBattleMons[bank].species].type1;
    u8 originalType2 = gBaseStats[gBattleMons[bank].species].type2;

    gBattleMons[bank].type1 = originalType1;
    gBattleMons[bank].type2 = originalType2;
    gBattleMons[bank].type3 = TYPE_BLANK;
}

// Main Function
u8 *DoTerastallize(u8 bank)
{
    if (!IsTerastallized(bank) && FlagGet(FLAG_TERA))
    {
        SET_BATTLER_TYPE(bank, GetTeraType(bank));
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, GetTeraType(bank));
        gNewBS->teraData.done[bank] = TRUE;
        FlagClear(FLAG_TERA);
        FadeScreen(FADE_FROM_BLACK, 10);
        return BattleScript_Terastallize;
    }
    return NULL;
}
