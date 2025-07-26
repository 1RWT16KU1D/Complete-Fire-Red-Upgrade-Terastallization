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
#include "../include/text.h"
#include "../include/text_window.h"
#include "../include/window.h"
#include "../include/constants/songs.h"

#include "../include/new/album.h"
#include "../include/new/dns.h"

// Imported functions
extern void CommitWindow(u8 windowId);
extern void CleanWindow(u8 windowId);
extern void CleanWindows(void);


static void InitAlbumData(void)
{
    sAlbumPtr->memoryData[0].memoryName = gText_Memory_None;
    sAlbumPtr->memoryData[0].memoryDesc = gText_Memory_None;

    sAlbumPtr->memoryData[1].memoryName = gText_Memory_MeloettaUnderTree;
    sAlbumPtr->memoryData[1].memoryDesc = gText_MemoryDesc_MeloettaUnderTree;

    sAlbumPtr->memoryData[2].memoryName = gText_Memory_PikachuAndEevee;
    sAlbumPtr->memoryData[2].memoryDesc = gText_MemoryDesc_PikachuAndEevee;

    sAlbumPtr->memoryData[3].memoryName = gText_Memory_PikachuAndEevee;
    sAlbumPtr->memoryData[3].memoryDesc = gText_MemoryDesc_PikachuAndEevee;

    sAlbumPtr->memoryData[4].memoryName = gText_Memory_None;
    sAlbumPtr->memoryData[4].memoryDesc = gText_Memory_None;

    sAlbumPtr->memoryData[5].memoryName = gText_Memory_None;
    sAlbumPtr->memoryData[5].memoryDesc = gText_Memory_None;

    sAlbumPtr->memoryData[6].memoryName = gText_Memory_None;
    sAlbumPtr->memoryData[6].memoryDesc = gText_Memory_None;

    sAlbumPtr->memoryData[7].memoryName = gText_Memory_None;
    sAlbumPtr->memoryData[7].memoryDesc = gText_Memory_None;

    sAlbumPtr->memoryData[8].memoryName = gText_Memory_None;
    sAlbumPtr->memoryData[8].memoryDesc = gText_Memory_None;
}

static void DisplayAlbumBG(void)
{
    // Tiles
    decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, AlbumBGTiles, 0, 0, 0);

    // BGMap
    LZDecompressWram(AlbumBGMap, sAlbumPtr->bg3Map);
    CopyBgTilemapBufferToVram(BG_BACKGROUND);

    // Palette
    LoadPalette(AlbumBGPal, 0, 0x20);
    LoadMenuElementsPalette(12 * 0x10, 1);
    Menu_LoadStdPalAt(15 * 0x10);
}

static void PrintGUIAlbumHeader(void)
{
    const u8* text = gText_AlbumHeader;
    u8 fontSize = 1; // Normal text
    CleanWindow(WIN_ALBUM_HEADER);

    // Show message
    WindowPrint(WIN_ALBUM_HEADER, fontSize, 175, 0, &sWhiteText, 0, text);
    CommitWindow(WIN_ALBUM_HEADER);
}

static void PrintOrUpdateGUIAlbumMemories(u8 currMemoryId)
{
    u8 fontSize = 1; // Normal Text
    u8 y = 0;

    CleanWindow(WIN_ALBUM_MEMORY_NAME);

    for (u8 i = 0; i < 7 && (currMemoryId + i) < MEMORIES_COUNT; ++i)
    {
        WindowPrint(WIN_ALBUM_MEMORY_NAME, fontSize, 0, y, &sWhiteText, 0,
                   sAlbumPtr->memoryData[currMemoryId + i].memoryName);
        y += 10;
    }

    CommitWindow(WIN_ALBUM_MEMORY_NAME);
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
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
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
        Free(sAlbumPtr->bg3Map);
        Free(sAlbumPtr);
        sAlbumPtr = NULL;
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

static void PrintGUIAlbumItems(void)
{
    PrintGUIAlbumHeader();
    PrintOrUpdateGUIAlbumMemories(1);
}

static void InitAlbum(void)
{
    // Remove glitches
    CleanWindows();
    CommitWindows();

    InitAlbumData();
    PrintGUIAlbumItems();
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
            sAlbumPtr->bg3Map = Calloc(BG_MAP_BYTES);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sAlbumBgTemplates, NELEMS(sAlbumBgTemplates));
            SetBgTilemapBuffer(BG_BACKGROUND, sAlbumPtr->bg3Map);
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            gMain.state++;
            break;
        case 3:
            DisplayAlbumBG();
            gMain.state++;
            break;
        case 4:
            if (!free_temp_tile_data_buffers_if_possible())
            {
                ShowBg(BG_BACKGROUND);
                CopyBgTilemapBufferToVram(BG_BACKGROUND);
                gMain.state++;
            }
            break;
        case 5:
            InitWindows(sAlbumWinTemplates);
            DeactivateAllTextPrinters();
            CopyBgTilemapBufferToVram(BG_INTERFACE);
            ShowBg(BG_INTERFACE);
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
        sAlbumPtr = Calloc(sizeof(struct Album));
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
        CleanWindows();
        PlayRainStoppingSoundEffect();
        DestroySafariZoneStatsWindow();
        CleanupOverworldWindowsAndTilemaps();
        sAlbumPtr = Calloc(sizeof(struct Album));
        SetMainCallback2(CB2_Album);
        return TRUE;
    }

    return FALSE;
}
