#pragma once

#include "../global.h"

#define MEMORIES_COUNT 44
#define BONUS_MEMORIES_COUNT 5
#define sAlbumPtr (*((struct Album**) 0x203E038))
#define BG_MAP_BYTES 0x800
#define ALBUM_MEMORIES_PER_PAGE 7
#define FLAG_SYS_DEXNAV 0x91E

#define VAR_ALBUM_SELECTED_MEMORY_IN_ALBUM 0x5100
#define VAR_ALBUM_SELECTED_MEMORY 0x5101
#define VAR_ALBUM_FIRST_TIME 0x5012

#define FLAG_ALBUM_GET 0x15FF
#define FLAG_FIRST_MEMORY 0x1600
#define FLAG_ALBUM_LAST_MEMORY 0x1625

extern bool8 StartMenuAlbumCallback(void);

enum AlbumWindows
{
    WIN_ALBUM_HEADER,
    WIN_ALBUM_MEMORY_NAME,
    WIN_ALBUM_MEMORY_DESC,
    WIN_ALBUM_INSTRUCTIONS,
    WIN_ALBUM_MEMORIES_COUNT,
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
    struct Memory memoryData[MEMORIES_COUNT + BONUS_MEMORIES_COUNT + 2];

    // Tracker Data
    u8 selectedMemory;
    u8 selectedMemoryInAlbum; // Max of 7
    u8 displayedStartId; // Start ID for the displayed memories
    u8 normalSelectedMemory;
    u8 normalSelectedMemoryInAlbum;
    u8 normalDisplayedStartId;
    u8 bonusSelectedMemory;
    u8 bonusSelectedMemoryInAlbum;
    u8 bonusDisplayedStartId;
    u8 memoryCount;
    bool8 isBonusPage;
    bool8 isBonus;
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

extern const u16 AlbumBonusBGPal[];

/* ============= Text Declarations ============== */
extern const u8 gText_AlbumHeader[];
extern const u8 gText_BonusHeader[];

// Memory Names
extern const u8 gText_None[];
extern const u8 gText_Memory_1[];
extern const u8 gText_Memory_2[];
extern const u8 gText_Memory_3[];
extern const u8 gText_Memory_4[];
extern const u8 gText_Memory_5[];
extern const u8 gText_Memory_6[];
extern const u8 gText_Memory_7[];
extern const u8 gText_Memory_8[];
extern const u8 gText_Memory_9[];
extern const u8 gText_Memory_10[];
extern const u8 gText_Memory_11[];
extern const u8 gText_Memory_12[];
extern const u8 gText_Memory_13[];
extern const u8 gText_Memory_14[];
extern const u8 gText_Memory_15[];
extern const u8 gText_Memory_16[];
extern const u8 gText_Memory_17[];
extern const u8 gText_Memory_18[];
extern const u8 gText_Memory_19[];
extern const u8 gText_Memory_20[];
extern const u8 gText_Memory_21[];
extern const u8 gText_Memory_22[];
extern const u8 gText_Memory_23[];
extern const u8 gText_Memory_24[];
extern const u8 gText_Memory_25[];
extern const u8 gText_Memory_26[];
extern const u8 gText_Memory_27[];
extern const u8 gText_Memory_28[];
extern const u8 gText_Memory_29[];
extern const u8 gText_Memory_30[];
extern const u8 gText_Memory_31[];
extern const u8 gText_Memory_32[];
extern const u8 gText_Memory_33[];
extern const u8 gText_Memory_34[];
extern const u8 gText_Memory_35[];
extern const u8 gText_Memory_36[];
extern const u8 gText_Memory_37[];
extern const u8 gText_Memory_38[];
extern const u8 gText_Memory_39[];
extern const u8 gText_Memory_40[];
extern const u8 gText_Memory_41[];
extern const u8 gText_Memory_42[];
extern const u8 gText_Memory_43[];
extern const u8 gText_BonusMemory_1[];
extern const u8 gText_BonusMemory_2[];
extern const u8 gText_BonusMemory_3[];
extern const u8 gText_BonusMemory_4[];
extern const u8 gText_BonusMemory_5[];

// Memory Descriptions
extern const u8 gText_Desc_None[];
extern const u8 gText_MemoryDesc_1[];
extern const u8 gText_MemoryDesc_2[];
extern const u8 gText_MemoryDesc_3[];
extern const u8 gText_MemoryDesc_4[];
extern const u8 gText_MemoryDesc_5[];
extern const u8 gText_MemoryDesc_6[];
extern const u8 gText_MemoryDesc_7[];
extern const u8 gText_MemoryDesc_8[];
extern const u8 gText_MemoryDesc_9[];
extern const u8 gText_MemoryDesc_10[];
extern const u8 gText_MemoryDesc_11[];
extern const u8 gText_MemoryDesc_12[];
extern const u8 gText_MemoryDesc_13[];
extern const u8 gText_MemoryDesc_14[];
extern const u8 gText_MemoryDesc_15[];
extern const u8 gText_MemoryDesc_16[];
extern const u8 gText_MemoryDesc_17[];
extern const u8 gText_MemoryDesc_18[];
extern const u8 gText_MemoryDesc_19[];
extern const u8 gText_MemoryDesc_20[];
extern const u8 gText_MemoryDesc_21[];
extern const u8 gText_MemoryDesc_22[];
extern const u8 gText_MemoryDesc_23[];
extern const u8 gText_MemoryDesc_24[];
extern const u8 gText_MemoryDesc_25[];
extern const u8 gText_MemoryDesc_26[];
extern const u8 gText_MemoryDesc_27[];
extern const u8 gText_MemoryDesc_28[];
extern const u8 gText_MemoryDesc_29[];
extern const u8 gText_MemoryDesc_30[];
extern const u8 gText_MemoryDesc_31[];
extern const u8 gText_MemoryDesc_32[];
extern const u8 gText_MemoryDesc_33[];
extern const u8 gText_MemoryDesc_34[];
extern const u8 gText_MemoryDesc_35[];
extern const u8 gText_MemoryDesc_36[];
extern const u8 gText_MemoryDesc_37[];
extern const u8 gText_MemoryDesc_38[];
extern const u8 gText_MemoryDesc_39[];
extern const u8 gText_MemoryDesc_40[];
extern const u8 gText_MemoryDesc_41[];
extern const u8 gText_MemoryDesc_42[];
extern const u8 gText_MemoryDesc_43[];
extern const u8 gText_BonusDesc_1[];
extern const u8 gText_BonusDesc_2[];
extern const u8 gText_BonusDesc_3[];
extern const u8 gText_BonusDesc_4[];
extern const u8 gText_BonusDesc_5[];

// Instructions
extern const u8 gText_AlbumPageInstructions[];
extern const u8 gText_BonusPageInstructions[];

// Memories unlocked
extern const u8 gText_AlbumMemoriesUnlocked[];

struct ImageData 
{
    u8 *tiles;
    u8 *tilemap; 
    u16 *pal;
};

#define ImageDataTable ((const struct ImageData *) 0x90B7600)
#define tilemapbuffer (*((u8**) 0x203E038))
