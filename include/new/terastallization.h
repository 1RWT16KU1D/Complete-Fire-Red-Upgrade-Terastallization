/* Custom File. Give credits if used!
   ~ 1RWT16KU1D
*/

// For Terastallization

#include "../pokemon.h"
#include "../global.h"

// Battle Scripts
extern u8 BattleScript_Terastallize[];

// Custom Functions
extern bool8 IsTerastallized(u8 bank);
extern u8 GetTeraType(u8 bank);
extern void ChangeTeraTypeInOW(void);
extern u8* DoTerastallize(u8 bank);
extern void GetTeraTypeInOW(void);
extern bool8 CanTerastallize(u8 bank);

// Visuals
extern void FadeBankPaletteForTera(u8 bank, u16 paletteOffset);
extern const u16 gTeraBlendColors[];

/* NEXT TO BE IMPLEMENTED LATER*/
// extern void AddTeraCrownSprite(u8 bank, u8 teraType, u8 x, u8 y);

// Sprite Data (TO BE IMPLEMENTED LATER)
