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

