#include "../include/battle.h"
#include "../include/bg.h"
#include "../include/decompress.h"
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
#include "../include/new/ram_locs.h"
#include "../include/new/Vanilla_functions.h"

// Imported functions
static void CommitWindow(u8 windowId);
static void CleanWindow(u8 windowId);
static void CleanWindows(void);
static void CommitWindows(void);
static void ShowImage(void);
static void Task_AlbumShowImage(u8 taskId);
static void LoadAlbumImage(u8 memoryId);
static void Task_ImageFadeIn(u8 taskId);
static void Task_ImageWaitForKeyPress(u8 taskId);
static void Task_ImageFadeOut(u8 taskId);
static void CB2_Image(void);
static void VBlankCB_Image(void);
static void MainCB2_Image(void);

static u8 sSavedSelectedMemory;
static u8 sSavedSelectedMemoryInAlbum;
static bool8 sReturnFromImage;

static const struct BgTemplate sAlbumBgTemplates[] =
{
    [BG_INTERFACE] =
    {
        .bg = BG_INTERFACE,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    [BG_UNUSED] =
    {
        .bg = BG_UNUSED,
        .charBaseIndex = 1,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
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
        .tilemapLeft = 24,
        .tilemapTop = 1,
        .width = 8,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    [WIN_ALBUM_MEMORY_NAME] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 20,
        .height = 16,
        .paletteNum = 15,
        .baseBlock = 33,
    },
    [WIN_ALBUM_MEMORY_DESC] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 1,
        .tilemapTop = 16,
        .width = 30,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 353,
    },
    [WIN_ALBUM_MISC] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 22,
        .tilemapTop = 7,
        .width = 8,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 533,
    },
    DUMMY_WIN_TEMPLATE,
};

static void InitAlbumData(void)
{
    sAlbumPtr->memoryData[0].memoryName = gText_Memory_MeloettaUnderTree;
    sAlbumPtr->memoryData[0].memoryDesc = gText_MemoryDesc_MeloettaUnderTree;

    sAlbumPtr->memoryData[1].memoryName = gText_Memory_PikachuAndEevee;
    sAlbumPtr->memoryData[1].memoryDesc = gText_MemoryDesc_PikachuAndEevee;

    sAlbumPtr->memoryData[2].memoryName = gText_Memory_InsideCave;
    sAlbumPtr->memoryData[2].memoryDesc = gText_MemoryDesc_InsideCave;

    sAlbumPtr->memoryData[3].memoryName = gText_Memory_NightSkyWish;
    sAlbumPtr->memoryData[3].memoryDesc = gText_MemoryDesc_NightSkyWish;

    sAlbumPtr->memoryData[4].memoryName = gText_Memory_SeasideShells;
    sAlbumPtr->memoryData[4].memoryDesc = gText_MemoryDesc_SeasideShells;

    sAlbumPtr->memoryData[5].memoryName = gText_Memory_CampfireTales;
    sAlbumPtr->memoryData[5].memoryDesc = gText_MemoryDesc_CampfireTales;

    sAlbumPtr->memoryData[6].memoryName = gText_Memory_SakuraPath;
    sAlbumPtr->memoryData[6].memoryDesc = gText_MemoryDesc_SakuraPath;

    sAlbumPtr->memoryData[7].memoryName = gText_Memory_SnowballFight;
    sAlbumPtr->memoryData[7].memoryDesc = gText_MemoryDesc_SnowballFight;

    sAlbumPtr->memoryData[8].memoryName = gText_Memory_LabDiscovery;
    sAlbumPtr->memoryData[8].memoryDesc = gText_MemoryDesc_LabDiscovery;

    // Initial cursor position
    sAlbumPtr->selectedMemory = 0;
    sAlbumPtr->selectedMemoryInAlbum = 0;
}

static void DisplayAlbumBG(void)
{
    // Tiles
    decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, AlbumBGTiles, 0, 0, 0);

    // BGMap
    LZDecompressWram(AlbumBGMap, sAlbumPtr->bgMap);
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
    WindowPrint(WIN_ALBUM_HEADER, fontSize, 0, 0, &sWhiteText, 0, text);
    CommitWindow(WIN_ALBUM_HEADER);
}

static void PrintGUIAlbumMemoryNames(void)
{
    u8 fontSize = 1; // Normal Text
    u8 y = 0;

    u8 startId = sAlbumPtr->selectedMemory - sAlbumPtr->selectedMemoryInAlbum;

    CleanWindow(WIN_ALBUM_MEMORY_NAME);

    for (u8 i = 0; i < ALBUM_MEMORIES_PER_PAGE && (startId + i) < MEMORIES_COUNT; ++i)
    {
        WindowPrint(WIN_ALBUM_MEMORY_NAME, fontSize, 0, y, &sWhiteText, 0,
                   sAlbumPtr->memoryData[startId + i].memoryName);
        y += 16;
    }

    CommitWindow(WIN_ALBUM_MEMORY_NAME);
}

static void PrintGUIAlbumDescription(void)
{
    u8 fontSize = 1;
    u8 x = 0;
    u8 y = 0;
    u8 memoryId = sAlbumPtr->selectedMemory;

    CleanWindow(WIN_ALBUM_MEMORY_DESC);
    WindowPrint(WIN_ALBUM_MEMORY_DESC, fontSize, x, y, &sWhiteText, 0,
                   sAlbumPtr->memoryData[memoryId].memoryDesc);
    CommitWindow(WIN_ALBUM_MEMORY_DESC);
}

static void UpdateCursorHighlight(bool8 isKeyUp, bool8 isStartUp)
{
    const u16* romPal = AlbumBGPal;
    u16* pal = gPlttBufferFaded;
    u16 defaultPal = romPal[7];

    // Change the palette
    u8 newIndex = sAlbumPtr->selectedMemoryInAlbum;
    u8 palId = newIndex + 7;

    // Restore previous highlighted palette
    if (!isStartUp)
    {
        if (isKeyUp)
            pal[palId + 1] = defaultPal;
        else
            pal[palId - 1] = defaultPal;
    }

    // Highlight selected cursor
    pal[palId] = RGB(31,31,31); // Pure white
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
        Free(sAlbumPtr->bgMap);
        Free(sAlbumPtr);
        sAlbumPtr = NULL;
        FreeAllWindowBuffers();
        DestroyTask(taskId);
    }
}

static void Task_AlbumShowImage(u8 taskId)
{
    if (!gPaletteFade->active)
    {
        ShowImage();
        DestroyTask(taskId);
    }
}

static void Task_AlbumWaitForKeyPress(u8 taskId)
{
    bool8 scrolled = FALSE;

    if (gMain.newKeys & DPAD_DOWN)
    {
        if (sAlbumPtr->selectedMemory < MEMORIES_COUNT - 1)
        {
            sAlbumPtr->selectedMemory++;

            if (sAlbumPtr->selectedMemoryInAlbum < ALBUM_MEMORIES_PER_PAGE - 1 &&
                sAlbumPtr->selectedMemoryInAlbum < MEMORIES_COUNT - 1)
            {
                sAlbumPtr->selectedMemoryInAlbum++;
            }

            UpdateCursorHighlight(FALSE, FALSE);
            scrolled = TRUE;
        }
    }
    else if (gMain.newKeys & DPAD_UP)
    {
        if (sAlbumPtr->selectedMemory > 0)
        {
            sAlbumPtr->selectedMemory--;

            if (sAlbumPtr->selectedMemoryInAlbum > 0)
            {
                sAlbumPtr->selectedMemoryInAlbum--;
            }

            UpdateCursorHighlight(TRUE, FALSE);
            scrolled = TRUE;
        }
    }

    // Update the names and descriptions
    if (scrolled)
    {
        PrintGUIAlbumMemoryNames();
        PrintGUIAlbumDescription();
        PlaySE(SE_SELECT);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_AlbumShowImage;
    }

    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_AlbumFadeOut;
    }
}

static void Task_AlbumFadeIn(u8 taskId)
{
    if (!gPaletteFade->active)
    {
        UpdateCursorHighlight(FALSE, TRUE);
        gTasks[taskId].func = Task_AlbumWaitForKeyPress;
    }
}

static void PrintGUIAlbumItems(void)
{
    PrintGUIAlbumHeader();
    PrintGUIAlbumMemoryNames();
    PrintGUIAlbumDescription();
}

static void InitAlbum(void)
{
    // Remove glitches
    CleanWindows();
    CommitWindows();

    InitAlbumData();
    if (sReturnFromImage)
    {
        sAlbumPtr->selectedMemory = sSavedSelectedMemory;
        sAlbumPtr->selectedMemoryInAlbum = sSavedSelectedMemoryInAlbum;
        sReturnFromImage = FALSE;
    }
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
            sAlbumPtr->bgMap = Calloc(BG_MAP_BYTES);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sAlbumBgTemplates, NELEMS(sAlbumBgTemplates));
            SetBgTilemapBuffer(BG_BACKGROUND, sAlbumPtr->bgMap);
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

static void ShowAlbum_Init(void)
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

extern void CleanWindow(u8 windowId)
{
	FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
}

extern void CleanWindows(void)
{
	for (u32 i = 0; i < WIN_MAX_COUNT; ++i)
		CleanWindow(i);
}

extern void CommitWindow(u8 windowId)
{
	CopyWindowToVram(windowId, COPYWIN_BOTH);
	PutWindowTilemap(windowId);
}

static void CommitWindows(void)
{
        for (u32 i = 0; i < WIN_MAX_COUNT; ++i)
                CommitWindow(i);
}

static const struct BgTemplate sImageBgTemplate =
{
    .bg = BG_BACKGROUND,
    .charBaseIndex = 3,
    .mapBaseIndex = 28,
    .screenSize = 0,
    .paletteMode = 0,
    .priority = 0,
    .baseTile = 0,
};

static void LoadAlbumImage(u8 memoryId)
{
    const struct ImageData *image = &ImageDataTable[memoryId];
    decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, image->tiles, 0, 0, 0);
    LZDecompressWram(image->tilemap, tilemapbuffer);
    CopyBgTilemapBufferToVram(BG_BACKGROUND);
    LoadPalette(image->pal, 0, 0x20);
}

static void VBlankCB_Image(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MainCB2_Image(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void Task_ImageFadeIn(u8 taskId)
{
    if (!gPaletteFade->active)
        gTasks[taskId].func = Task_ImageWaitForKeyPress;
}

static void Task_ImageWaitForKeyPress(u8 taskId)
{
    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_PC_OFF);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_ImageFadeOut;
    }
}

static void Task_ImageFadeOut(u8 taskId)
{
    if (!gPaletteFade->active)
    {
        Free(tilemapbuffer);
        tilemapbuffer = NULL;
        CleanupOverworldWindowsAndTilemaps();
        sAlbumPtr = Calloc(sizeof(struct Album));
        gMain.state = 0;
        SetMainCallback2(CB2_Album);
        DestroyTask(taskId);
    }
}

static void CB2_Image(void)
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
            ResetBgsAndClearDma3BusyFlags(0);
            tilemapbuffer = Calloc(BG_MAP_BYTES);
            InitBgsFromTemplates(0, &sImageBgTemplate, 1);
            SetBgTilemapBuffer(BG_BACKGROUND, tilemapbuffer);
            gMain.state++;
            break;
        case 2:
            LoadAlbumImage(sSavedSelectedMemory);
            ShowBg(BG_BACKGROUND);
            gMain.state++;
            break;
        case 3:
            if (!free_temp_tile_data_buffers_if_possible())
            {
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
                SetVBlankCallback(VBlankCB_Image);
                CreateTask(Task_ImageFadeIn, 0);
                SetMainCallback2(MainCB2_Image);
                gMain.state = 0;
            }
            break;
    }
}

static void ShowImage(void)
{
    sSavedSelectedMemory = sAlbumPtr->selectedMemory;
    sSavedSelectedMemoryInAlbum = sAlbumPtr->selectedMemoryInAlbum;
    sReturnFromImage = TRUE;

    Free(sAlbumPtr->bgMap);
    Free(sAlbumPtr);
    sAlbumPtr = NULL;
    FreeAllWindowBuffers();

    gMain.state = 0;
    SetMainCallback2(CB2_Image);
}
