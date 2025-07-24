#pragma once

#include "../global.h"

#define MEMORIES_COUNT 30
#define sAlbumPtr (*((struct Album**) 0x203E038))
#define BG_MAP_BYTES 0x800

enum AlbumWindows
{
    WIN_ALBUM_HEADER,
    WIN_ALBUM_MEMORY_NAME,
    WIN_ALBUM_MEMORY_DESC,
    WIN_MAX_COUNT,
};

enum AlbumBGs
{
    BG_UNUSED,
    BG_INTERFACE,
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
    u16* bg3Map;

    // Memory Data
    struct Memory memoryData[MEMORIES_COUNT];

    // Tracker Data
    u8 selectedMemory;
};

static const struct TextColor sWhiteText =
{
	.bgColor = TEXT_COLOR_TRANSPARENT,
	.fgColor = TEXT_COLOR_WHITE,
	.shadowColor = TEXT_COLOR_DARK_GREY,
};

static const struct BgTemplate sAlbumBgTemplates[] =
{
    [BG_UNUSED] =
    {
        .bg = BG_UNUSED,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    [BG_INTERFACE] =
    {
        .bg = BG_INTERFACE,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    [BG_UNUSED2] =
    {
        .bg = BG_UNUSED2,
        .charBaseIndex = 2,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    [BG_BACKGROUND] =
    {
        .bg = BG_BACKGROUND,
        .charBaseIndex = 3,
        .mapBaseIndex = 28,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sAlbumWinTemplates[WIN_MAX_COUNT + 1] =
{
    [WIN_ALBUM_HEADER] =
    {
        .bg = BG_INTERFACE,
		.tilemapLeft = 1,
		.tilemapTop = 1,
		.width = 28,
		.height = 4,
		.paletteNum = 15,
		.baseBlock = 1,
    },
    [WIN_ALBUM_MEMORY_NAME] =
    {
        .bg = BG_INTERFACE,
		.tilemapLeft = 1,
		.tilemapTop = 1,
		.width = 28,
		.height = 4,
		.paletteNum = 15,
		.baseBlock = 1,
    },
    [WIN_ALBUM_MEMORY_DESC] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 1,
        .tilemapTop = 6,
        .width = 28,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 113,
    },
    DUMMY_WIN_TEMPLATE,
};

/* ============= Image Data ============= */
extern const u8 AlbumBGTiles[];
extern const u8 AlbumBGMap[];
extern const u16 AlbumBGPal[];

// Memory Images
//extern const u8 Memory_MeloettaUnderTreeTiles[];
//extern const u8 Memory_MeloettaUnderTreeMap[];
//extern const u8 Memory_MeloettaUnderTreePal[];

/* ============= Text Declarations ============== */
extern const u8 gText_AlbumHeader[];

// Memory Names
extern const u8 gText_Memory_None[];
extern const u8 gText_Memory_MeloettaUnderTree[];
extern const u8 gText_Memory_PikachuAndEevee[];
extern const u8 gText_Memory_InsideCave[];

// Memory Descriptions
extern const u8 gText_MemoryDesc_None[];
extern const u8 gText_MemoryDesc_MeloettaUnderTree[];
extern const u8 gText_MemoryDesc_PikachuAndEevee[];
extern const u8 gText_MemoryDesc_InsideCave[];
