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

extern const u8 AlbumCursorTiles[];
extern const u8 AlbumCursorPal[];

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
