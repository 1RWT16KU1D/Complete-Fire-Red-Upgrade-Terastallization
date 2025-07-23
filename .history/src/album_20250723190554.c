#include "defines.h"
#include "../include/battle.h"
#include "../include/bg.h"
#include "../include/event_data.h"
#include "../include/event_object_movement.h"
#include "../include/field_effect.h"
#include "../include/field_effect_helpers.h"
#include "../include/field_message_box.h"
#include "../include/field_player_avatar.h"
#include "../include/field_weather.h"
#include "../include/fieldmap.h"
#include "../include/gpu_regs.h"
#include "../include/international_string_util.h"
#include "../include/item_menu.h"
#include "../include/map_name_popup.h"
#include "../include/menu.h"
#include "../include/m4a.h"
#include "../include/main.h"
#include "../include/metatile_behavior.h"
#include "../include/new_menu_helpers.h"
#include "../include/overworld.h"
#include "../include/palette.h"
#include "../include/pokemon.h"
#include "../include/pokemon_icon.h"
#include "../include/random.h"
#include "../include/region_map.h"
#include "../include/scanline_effect.h"
#include "../include/script.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/start_menu.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/text_window.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/constants/abilities.h"
#include "../include/constants/field_effects.h"
#include "../include/constants/items.h"
#include "../include/constants/maps.h"
#include "../include/constants/metatile_behaviors.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/songs.h"
#include "../include/constants/species.h"
#include "../include/gba/io_reg.h"

#include "../include/new/album.h"
#include "../include/new/battle_strings.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/daycare.h"
#include "../include/new/dexnav.h"
#include "../include/new/dexnav_config.h"
#include "../include/new/dexnav_data.h"
#include "../include/new/dns.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new/overworld.h"
#include "../include/new/wild_encounter.h"
#include "../include/new/util.h"

static void CleanWindow(u8 windowId)
{
	FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
}

static void CleanWindows(void)
{
	for (u32 i = 0; i < WIN_MAX_COUNT; ++i)
		CleanWindow(i);
}

static void DisplayAlbumBG(void)
{
	const u8 *tiles, *map;
	const u16 *palette, *altPalette;

	#ifdef UNBOUND
	tiles = DexNavBGUnboundTiles;
	map = DexNavBGUnboundMap;
	palette = DexNavBGUnboundPal;
	#else
	tiles = DexNavBGTiles;
	map = DexNavBGMap;
	palette = DexNavBGPal;
	#endif

	decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, tiles, 0, 0, 0);
	LZDecompressWram(map, sDexNavGUIPtr->tilemapPtr);

	//Choose palette based on current location
	if (IsMapTypeIndoors(GetCurrentMapType()))
		altPalette = DexNavBG_IndoorPal;
	else if (IsCurrentAreaVolcano()) //Load special palette for volcanos
		altPalette = DexNavBG_VolcanoPal;
	#ifdef UNBOUND
	else if (MAP_IS(FLOWER_PARADISE_A))
		altPalette = DexNavBG_FlowerParadiseAPal;
	else if (MAP_IS(FLOWER_PARADISE_B))
		altPalette = DexNavBG_FlowerParadiseBPal;
	else if (MAP_IS(FLOWER_PARADISE_C))
		altPalette = DexNavBG_FlowerParadiseCPal;
	#endif
	else if (IsCurrentAreaDarkerCave())
		altPalette = DexNavBG_DarkerCavePal;
	else if (GetCurrentMapType() == MAP_TYPE_UNDERGROUND)
		altPalette = DexNavBG_CavePal;
	else if (IsCurrentAreaAutumn())
		altPalette = DexNavBG_AutumnPal;
	else if (IsCurrentAreaWinter())
		altPalette = DexNavBG_WinterPal;
	else if (IsCurrentAreaDesert())
		altPalette = DexNavBG_DesertPal;
	else if (IsCurrentAreaSwamp())
		altPalette = DexNavBG_SwampPal;
	else
		altPalette = NULL;

	LoadPalette(palette, 0, 0x20); //Pal 0
	if (altPalette != NULL)
		LoadPalette(altPalette + 1, 1, 0x2 * 6); //Pal 0 - copy first 6 real colours
	LoadMenuElementsPalette(12 * 0x10, 1); //Pal 12
	Menu_LoadStdPalAt(15 * 0x10); //Pal 15
}

static void PrintAlbumHeader(void)
{
    const u8* text = gText_AlbumHeader;
    u8 fontSize = 1;
    CleanWindow(WIN_ALBUM_HEADER);

    s16 x = 50;
    WindowPrint(WIN_ALBUM_HEADER, 1, x, 3, &sWhiteText, 0, text);
}

void ShowAlbum_Init(void)
{
    // RAM Clear
    CleanWindows();

    // Show the BG

    
    // Print text
    PrintAlbumHeader();
}

