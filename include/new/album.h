#pragma once

#include "../global.h"

#define MEMORIES_COUNT 9
#define sAlbumPtr (*((struct Album**) 0x203E038))
#define BG_MAP_BYTES 0x800
#define ALBUM_MEMORIES_PER_PAGE 7

#define VAR_ALBUM_SELECTED_MEMORY_IN_ALBUM 0x5100
#define VAR_ALBUM_SELECTED_MEMORY 0x5101
#define VAR_ALBUM_FIRST_TIME 0x5012

enum AlbumWindows
{
    WIN_ALBUM_HEADER,
    WIN_ALBUM_MEMORY_NAME,
    WIN_ALBUM_MEMORY_DESC,
    WIN_ALBUM_MISC,
    WIN_MAX_COUNT,
};

enum AlbumBGs
{
    BG_INTERFACE,
    BG_UNUSED,
    BG_UNUSED2,
    BG_BACKGROUND,
};

struct Memory
{
    bool8 unlocked;
    const u8* memoryName;
    const u8* memoryDesc;
};

struct Album
{
    // Image Data
    u16* bgMap;

    // Memory Data
    struct Memory memoryData[MEMORIES_COUNT + 2];

    // Tracker Data
    u8 selectedMemory;
    u8 selectedMemoryInAlbum; // Max of 7
};

static const struct TextColor sWhiteText =
{
	.bgColor = TEXT_COLOR_TRANSPARENT,
	.fgColor = TEXT_COLOR_WHITE,
	.shadowColor = TEXT_COLOR_DARK_GREY,
};

/* ============= Image Data ============= */
extern const u8 AlbumBGTiles[];
extern const u8 AlbumBGMap[];
extern const u16 AlbumBGPal[];

/* ============= Text Declarations ============== */
extern const u8 gText_AlbumHeader[];

// Memory Names
extern const u8 gText_Memory_MeloettaUnderTree[];
extern const u8 gText_Memory_PikachuAndEevee[];
extern const u8 gText_Memory_InsideCave[];
extern const u8 gText_Memory_NightSkyWish[];
extern const u8 gText_Memory_SeasideShells[];
extern const u8 gText_Memory_CampfireTales[];
extern const u8 gText_Memory_SakuraPath[];
extern const u8 gText_Memory_SnowballFight[];
extern const u8 gText_Memory_LabDiscovery[];

// Memory Descriptions
extern const u8 gText_MemoryDesc_MeloettaUnderTree[];
extern const u8 gText_MemoryDesc_PikachuAndEevee[];
extern const u8 gText_MemoryDesc_InsideCave[];
extern const u8 gText_MemoryDesc_NightSkyWish[];
extern const u8 gText_MemoryDesc_SeasideShells[];
extern const u8 gText_MemoryDesc_CampfireTales[];
extern const u8 gText_MemoryDesc_SakuraPath[];
extern const u8 gText_MemoryDesc_SnowballFight[];
extern const u8 gText_MemoryDesc_LabDiscovery[];

// Script call
extern const u8 EventScript_AlbumMemorySelected[];

struct ImageData 
{
    u8 *tiles;
    u8 *tilemap; 
    u16 *pal;
};

#define ImageDataTable ((const struct ImageData *) 0x8FE4E80)
#define tilemapbuffer (*((u8**) 0x203E03C))
