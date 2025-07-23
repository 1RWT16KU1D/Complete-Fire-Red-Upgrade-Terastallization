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

struct AlbumData
{
    u8* tilemapPtr;
};

#define sAlbumPtr (*((struct AlbumData**) 0x203E038))

enum
{
    BG_TEXTBOX,
    BG_TEXT_2,
    BG_TEXT,
    BG_BACKGROUND,
};

static const struct BgTemplate sAlbumBgTemplates[] =
{
    [BG_TEXTBOX] =
    {
        .bg = BG_TEXTBOX,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    [BG_TEXT_2] =
    {
        .bg = BG_TEXT_2,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    [BG_TEXT] =
    {
        .bg = BG_TEXT,
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
        .bg = BG_TEXT,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = 28,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [WIN_ALBUM_TEXT] =
    {
        .bg = BG_TEXT,
        .tilemapLeft = 1,
        .tilemapTop = 3,
        .width = 28,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 57,
    },
    [WIN_ALBUM_DESC] =
    {
        .bg = BG_TEXT,
        .tilemapLeft = 1,
        .tilemapTop = 6,
        .width = 28,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 113,
    },
    DUMMY_WIN_TEMPLATE,
};

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
        const u16 *palette, *altPalette = NULL;

	tiles = AlbumBGTiles;
	map = AlbumBGMap;
	palette = AlbumBGPal;

        decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, tiles, 0, 0, 0);
        LZDecompressWram(map, sAlbumPtr->tilemapPtr);

	LoadPalette(palette, 0, 0x20); //Pal 0
	if (altPalette != NULL)
		LoadPalette(altPalette + 1, 1, 0x2 * 6); //Pal 0 - copy first 6 real colours
	LoadMenuElementsPalette(12 * 0x10, 1); //Pal 12
	Menu_LoadStdPalAt(15 * 0x10); //Pal 15
}

static void PrintAlbumHeader(void)
{
    const u8* text = gText_AlbumHeader;

    CleanWindow(WIN_ALBUM_HEADER);
    WindowPrint(WIN_ALBUM_HEADER, 1, 50, 3, &sWhiteText, 0, text);
    CopyWindowToVram(WIN_ALBUM_HEADER, COPYWIN_BOTH);
    PutWindowTilemap(WIN_ALBUM_HEADER);
}

static void CommitWindows(void)
{
    for (u32 i = 0; i < WIN_MAX_COUNT; ++i)
    {
        CopyWindowToVram(i, COPYWIN_BOTH);
        PutWindowTilemap(i);
    }
}

static void ClearTasksAndGraphicalStructs(void)
{
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetTempTileDataBuffers();
    ResetPaletteFade();
    FreeAllSpritePalettes();
}

static void ClearVramOamPlttRegs(void)
{
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetGpuReg(REG_OFFSET_BG3CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0VOFS, DISPCNT_MODE_0);
}

static void VBlankCB_Album(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_Album(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void Task_AlbumFadeOut(u8 taskId)
{
    if (!gPaletteFade->active)
    {
        SetMainCallback2(CB2_ReturnToFieldContinueScript);
        Free(sAlbumPtr->tilemapPtr);
        Free(sAlbumPtr);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
    }
}

static void Task_AlbumWaitForKeyPress(u8 taskId)
{
    if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_AlbumFadeOut;
    }
}

static void Task_AlbumFadeIn(u8 taskId)
{
    if (!gPaletteFade->active)
        gTasks[taskId].func = Task_AlbumWaitForKeyPress;
}

static void InitAlbum(void)
{
    PrintAlbumHeader();
    CommitWindows();
}

static void CB2_Album(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        ClearVramOamPlttRegs();
        gMain.state++;
        break;
    case 1:
        ClearTasksAndGraphicalStructs();
        gMain.state++;
        break;
    case 2:
        sAlbumPtr->tilemapPtr = Malloc(0x1000);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sAlbumBgTemplates, NELEMS(sAlbumBgTemplates));
        SetBgTilemapBuffer(BG_BACKGROUND, sAlbumPtr->tilemapPtr);
        gMain.state++;
        break;
    case 3:
        DisplayAlbumBG();
        gMain.state++;
        break;
    case 4:
        if (!free_temp_tile_data_buffers_if_possible())
        {
            ShowBg(BG_TEXT);
            ShowBg(BG_BACKGROUND);
            CopyBgTilemapBufferToVram(BG_BACKGROUND);
            gMain.state++;
        }
        break;
    case 5:
        InitWindows(sAlbumWinTemplates);
        DeactivateAllTextPrinters();
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 7:
        SetVBlankCallback(VBlankCB_Album);
        InitAlbum();
        CreateTask(Task_AlbumFadeIn, 0);
        SetMainCallback2(MainCB2_Album);
        gMain.state = 0;
        break;
    }
}

static void Task_InitAlbum(u8 taskId)
{
    if (!gPaletteFade->active)
    {
        sAlbumPtr = Calloc(sizeof(struct AlbumData));
        PlayRainStoppingSoundEffect();
        SetMainCallback2(CB2_Album);
        DestroyTask(taskId);
    }
}

void ShowAlbum_Init(void)
{
    FadeScreen(FADE_TO_BLACK, 0);
    CreateTask(Task_InitAlbum, 0);
}

bool8 AlbumCallback(void)
{
    if (!gPaletteFade->active)
    {
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        CleanupOverworldWindowsAndTilemaps();
        sAlbumPtr = Calloc(sizeof(struct AlbumData));
        SetMainCallback2(CB2_Album);
        return TRUE;
    }

    return FALSE;
}
