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


// This file's functions
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
static void ResetHighlightPalettes(void);
static void PrintGUIAlbumItems(void);

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
    [WIN_ALBUM_MEMORIES_COUNT] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 22,
        .tilemapTop = 5,
        .width = 8,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 533,
    },
    [WIN_ALBUM_INSTRUCTIONS] =
    {
        .bg = BG_INTERFACE,
        .tilemapLeft = 22,
        .tilemapTop = 10,
        .width = 8,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 565,
    },
    DUMMY_WIN_TEMPLATE,
};

static const u8 *const sMemoryNames[] =
{
    gText_None,
    gText_Memory_1,
    gText_Memory_2,
    gText_Memory_3,
    gText_Memory_4,
    gText_Memory_5,
    gText_Memory_6,
    gText_Memory_7,
    gText_Memory_8,
    gText_Memory_9,
    gText_Memory_10,
    gText_Memory_11,
    gText_Memory_12,
    gText_Memory_13,
    gText_Memory_14,
    gText_Memory_15,
    gText_Memory_16,
    gText_Memory_17,
    gText_Memory_18,
    gText_Memory_19,
    gText_Memory_20,
    gText_Memory_21,
    gText_Memory_22,
    gText_Memory_23,
    gText_Memory_24,
    gText_Memory_25,
    gText_Memory_26,
    gText_Memory_27,
    gText_Memory_28,
    gText_Memory_29,
    gText_Memory_30,
    gText_Memory_31,
    gText_Memory_32,
    gText_Memory_33,
    gText_Memory_34,
    gText_Memory_35,
    gText_Memory_36,
    gText_Memory_37,
    gText_Memory_38,
    gText_Memory_39,
    gText_Memory_40,
    gText_Memory_41,
    gText_Memory_42,
    gText_Memory_43,
};

static const u8 *const sMemoryDescs[] =
{
    gText_Desc_None,
    gText_MemoryDesc_1,
    gText_MemoryDesc_2,
    gText_MemoryDesc_3,
    gText_MemoryDesc_4,
    gText_MemoryDesc_5,
    gText_MemoryDesc_6,
    gText_MemoryDesc_7,
    gText_MemoryDesc_8,
    gText_MemoryDesc_9,
    gText_MemoryDesc_10,
    gText_MemoryDesc_11,
    gText_MemoryDesc_12,
    gText_MemoryDesc_13,
    gText_MemoryDesc_14,
    gText_MemoryDesc_15,
    gText_MemoryDesc_16,
    gText_MemoryDesc_17,
    gText_MemoryDesc_18,
    gText_MemoryDesc_19,
    gText_MemoryDesc_20,
    gText_MemoryDesc_21,
    gText_MemoryDesc_22,
    gText_MemoryDesc_23,
    gText_MemoryDesc_24,
    gText_MemoryDesc_25,
    gText_MemoryDesc_26,
    gText_MemoryDesc_27,
    gText_MemoryDesc_28,
    gText_MemoryDesc_29,
    gText_MemoryDesc_30,
    gText_MemoryDesc_31,
    gText_MemoryDesc_32,
    gText_MemoryDesc_33,
    gText_MemoryDesc_34,
    gText_MemoryDesc_35,
    gText_MemoryDesc_36,
    gText_MemoryDesc_37,
    gText_MemoryDesc_38,
    gText_MemoryDesc_39,
    gText_MemoryDesc_40,
    gText_MemoryDesc_41,
    gText_MemoryDesc_42,
    gText_MemoryDesc_43,
};

static const u8 *const sBonusMemoryNames[] =
{
    gText_BonusMemory_1,
    gText_BonusMemory_2,
    gText_BonusMemory_3,
    gText_BonusMemory_4,
    gText_BonusMemory_5,
};

static const u8 *const sBonusMemoryDescs[] =
{
    gText_BonusDesc_1,
    gText_BonusDesc_2,
    gText_BonusDesc_3,
    gText_BonusDesc_4,
    gText_BonusDesc_5,
};

// Tables defining the display order for the memories.
// Modify these arrays to reorder memories in the album.
static const u8 sMemoryOrder[MEMORIES_COUNT] =
{
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43,
};

static const u8 sBonusMemoryOrder[BONUS_MEMORIES_COUNT] =
{
    0, 1, 2, 3, 4,
};

static void InitAlbumData(bool8 bonusPage)
{
    const u8 *const *names;
    const u8 *const *descs;
    const u8 *order;
    u8 count;

    if (bonusPage)
    {
        names = sBonusMemoryNames;
        descs = sBonusMemoryDescs;
        order = sBonusMemoryOrder;
        count = BONUS_MEMORIES_COUNT;
    }
    else
    {
        names = sMemoryNames;
        descs = sMemoryDescs;
        order = sMemoryOrder;
        count = MEMORIES_COUNT;
    }

    for (u8 i = 0; i < count; ++i)
    {
        u8 memoryId = order[i];

        sAlbumPtr->memoryData[i].memoryName = names[memoryId];
        sAlbumPtr->memoryData[i].memoryDesc = descs[memoryId];

        if (bonusPage)
        {
            sAlbumPtr->memoryData[i].unlocked = TRUE;
        }
        else
        {
            sAlbumPtr->memoryData[i].unlocked = FlagGet(FLAG_FIRST_MEMORY + memoryId);

            if (!sAlbumPtr->memoryData[i].unlocked)
            {
                sAlbumPtr->memoryData[i].memoryName = gText_None;
                sAlbumPtr->memoryData[i].memoryDesc = gText_Desc_None;
            }
        }
    }
    sAlbumPtr->memoryCount = count;
}

static void DisplayAlbumBG(void)
{
    if (!FlagGet(FLAG_ALBUM_SE_DONE))
    {
        PlaySE(SE_RG_CARD3);
        FlagSet(FLAG_ALBUM_SE_DONE);
    }

    // Tiles
    decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, AlbumBGTiles, 0, 0, 0);

    // BGMap
    LZDecompressWram(AlbumBGMap, sAlbumPtr->bgMap);
    CopyBgTilemapBufferToVram(BG_BACKGROUND);

    // Palette
    if (sAlbumPtr->isBonusPage)
        LoadPalette(AlbumBonusBGPal, 0, 0x20);
    else
        LoadPalette(AlbumBGPal, 0, 0x20);

    LoadMenuElementsPalette(12 * 0x10, 1);
    Menu_LoadStdPalAt(15 * 0x10);
}

static void PrintGUIAlbumHeader(void)
{
    const u8* text = sAlbumPtr->isBonusPage ? gText_BonusHeader : gText_AlbumHeader;
    u8 fontSize = 1; // Normal text
    CleanWindow(WIN_ALBUM_HEADER);

    // Show message
    WindowPrint(WIN_ALBUM_HEADER, fontSize, 0, 0, &sWhiteText, 0, text);
    CommitWindow(WIN_ALBUM_HEADER);
}

static void PrintGUIAlbumPageInstructions(void)
{
    const u8* text = sAlbumPtr->isBonusPage ? gText_BonusPageInstructions : gText_AlbumPageInstructions;
    u8 fontSize = 0; // Smaller text
    CleanWindow(WIN_ALBUM_INSTRUCTIONS);

    // Show message
    WindowPrint(WIN_ALBUM_INSTRUCTIONS, fontSize, 0, 0, &sWhiteText, 0, text);
    CommitWindow(WIN_ALBUM_INSTRUCTIONS);
}

static void PrintGUIAlbumMemoryNames(void)
{
    u8 fontSize = 1; // Normal Text
    u8 y = 0;
    u8 startId = sAlbumPtr->displayedStartId;

    CleanWindow(WIN_ALBUM_MEMORY_NAME);

    for (u8 i = 0; i < ALBUM_MEMORIES_PER_PAGE && (startId + i) < sAlbumPtr->memoryCount; ++i)
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

static void PrintGUIAlbumMemoriesUnlocked(void)
{
    u8 fontSize = 0; // Smaller text
    u8 y = 0;
    u8 unlocked = 0;

    // Count unlocked memories
    for (u8 i = 0; i < sAlbumPtr->memoryCount; ++i)
        if (sAlbumPtr->memoryData[i].unlocked)
            unlocked++;

    CleanWindow(WIN_ALBUM_MEMORIES_COUNT);

    u8 buff[12];
    u8 num[4];

    StringCopy(buff, gText_AlbumMemoriesUnlocked);
    ConvertIntToDecimalStringN(num, unlocked, STR_CONV_MODE_LEFT_ALIGN, 3);

    StringAppend(buff, num);
    WindowPrint(WIN_ALBUM_MEMORIES_COUNT, fontSize, 0, y, &sWhiteText, 0, buff);

    CommitWindow(WIN_ALBUM_MEMORIES_COUNT);
}

static void UpdateCursorHighlight(bool8 isKeyUp, bool8 isStartUp)
{
    const u16* romPal = sAlbumPtr->isBonusPage ? AlbumBonusBGPal : AlbumBGPal;
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

static void ResetHighlightPalettes(void)
{
    const u16* romPal = sAlbumPtr->isBonusPage ? AlbumBonusBGPal : AlbumBGPal;
    u16* pal = gPlttBufferFaded;
    u16 defaultPal = romPal[7];

    for (u8 i = 0; i < ALBUM_MEMORIES_PER_PAGE; ++i)
        pal[7 + i] = defaultPal;
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
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        Free(sAlbumPtr->bgMap);
        Free(sAlbumPtr);
        sAlbumPtr = NULL;
        VarSet(VAR_IS_BONUS_PAGE, 0);
        VarSet(VAR_ALBUM_NORMAL_SELECTED_MEMORY, 0);
        VarSet(VAR_ALBUM_NORMAL_SELECTED_MEMORY_IN_ALBUM, 0);
        VarSet(VAR_ALBUM_NORMAL_DISPLAYED_START_ID, 0);
        VarSet(VAR_ALBUM_BONUS_SELECTED_MEMORY, 0);
        VarSet(VAR_ALBUM_BONUS_SELECTED_MEMORY_IN_ALBUM, 0);
        VarSet(VAR_ALBUM_BONUS_DISPLAYED_START_ID, 0);
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
    bool8 redrawNames = FALSE;
    u8 prevStartId = sAlbumPtr->displayedStartId;

    if ((gMain.newKeys & R_BUTTON) && !sAlbumPtr->isBonusPage)
    {
        sAlbumPtr->normalSelectedMemory = sAlbumPtr->selectedMemory;
        sAlbumPtr->normalSelectedMemoryInAlbum = sAlbumPtr->selectedMemoryInAlbum;
        sAlbumPtr->normalDisplayedStartId = sAlbumPtr->displayedStartId;
        sAlbumPtr->selectedMemory = sAlbumPtr->bonusSelectedMemory;
        sAlbumPtr->selectedMemoryInAlbum = sAlbumPtr->bonusSelectedMemoryInAlbum;
        sAlbumPtr->displayedStartId = sAlbumPtr->bonusDisplayedStartId;
        sAlbumPtr->isBonusPage = TRUE;
        DisplayAlbumBG();
        InitAlbumData(TRUE);
        PrintGUIAlbumItems();
        ResetHighlightPalettes();
        UpdateCursorHighlight(FALSE, TRUE);
        PlaySE(SE_WIN_OPEN);
        return;
    }
    else if ((gMain.newKeys & L_BUTTON) && sAlbumPtr->isBonusPage)
    {
        sAlbumPtr->bonusSelectedMemory = sAlbumPtr->selectedMemory;
        sAlbumPtr->bonusSelectedMemoryInAlbum = sAlbumPtr->selectedMemoryInAlbum;
        sAlbumPtr->bonusDisplayedStartId = sAlbumPtr->displayedStartId;
        sAlbumPtr->selectedMemory = sAlbumPtr->normalSelectedMemory;
        sAlbumPtr->selectedMemoryInAlbum = sAlbumPtr->normalSelectedMemoryInAlbum;
        sAlbumPtr->displayedStartId = sAlbumPtr->normalDisplayedStartId;
        sAlbumPtr->isBonusPage = FALSE;
        DisplayAlbumBG();
        InitAlbumData(FALSE);
        PrintGUIAlbumItems();
        ResetHighlightPalettes();
        UpdateCursorHighlight(FALSE, TRUE);
        PlaySE(SE_WIN_OPEN);
        return;
    }

    if (JOY_NEW_AND_REPEATED(DPAD_DOWN))
    {
        if (sAlbumPtr->selectedMemory < sAlbumPtr->memoryCount - 1)
        {
            sAlbumPtr->selectedMemory++;

            if (sAlbumPtr->selectedMemoryInAlbum < ALBUM_MEMORIES_PER_PAGE - 1 &&
                sAlbumPtr->selectedMemoryInAlbum < sAlbumPtr->memoryCount - 1)
            {
                sAlbumPtr->selectedMemoryInAlbum++;
            }

            UpdateCursorHighlight(FALSE, FALSE);
            scrolled = TRUE;
        }
    }
    else if (JOY_NEW_AND_REPEATED(DPAD_UP))
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

    if (scrolled)
    {
        u8 newStartId = sAlbumPtr->selectedMemory - sAlbumPtr->selectedMemoryInAlbum;
        if (newStartId != prevStartId)
        {
            sAlbumPtr->displayedStartId = newStartId;
            redrawNames = TRUE;
        }

        if (redrawNames)
            PrintGUIAlbumMemoryNames();

        PrintGUIAlbumDescription();
        PlaySE(SE_SELECT);
    }

    if (gMain.newKeys & A_BUTTON)
    {
        VarSet(VAR_ALBUM_SELECTED_MEMORY, sAlbumPtr->selectedMemory);
        VarSet(VAR_ALBUM_SELECTED_MEMORY_IN_ALBUM, sAlbumPtr->selectedMemoryInAlbum);

        // Save the page you're currently on
        if (sAlbumPtr->isBonusPage)
        {
            VarSet(VAR_ALBUM_BONUS_SELECTED_MEMORY,            sAlbumPtr->selectedMemory);
            VarSet(VAR_ALBUM_BONUS_SELECTED_MEMORY_IN_ALBUM,   sAlbumPtr->selectedMemoryInAlbum);
            VarSet(VAR_ALBUM_BONUS_DISPLAYED_START_ID,         sAlbumPtr->displayedStartId);

            // And also save the cached normal-page cursor we keep in RAM
            VarSet(VAR_ALBUM_NORMAL_SELECTED_MEMORY,           sAlbumPtr->normalSelectedMemory);
            VarSet(VAR_ALBUM_NORMAL_SELECTED_MEMORY_IN_ALBUM,  sAlbumPtr->normalSelectedMemoryInAlbum);
            VarSet(VAR_ALBUM_NORMAL_DISPLAYED_START_ID,        sAlbumPtr->normalDisplayedStartId);
        }
        else
        {
            VarSet(VAR_ALBUM_NORMAL_SELECTED_MEMORY,           sAlbumPtr->selectedMemory);
            VarSet(VAR_ALBUM_NORMAL_SELECTED_MEMORY_IN_ALBUM,  sAlbumPtr->selectedMemoryInAlbum);
            VarSet(VAR_ALBUM_NORMAL_DISPLAYED_START_ID,        sAlbumPtr->displayedStartId);

            VarSet(VAR_ALBUM_BONUS_SELECTED_MEMORY,            sAlbumPtr->bonusSelectedMemory);
            VarSet(VAR_ALBUM_BONUS_SELECTED_MEMORY_IN_ALBUM,   sAlbumPtr->bonusSelectedMemoryInAlbum);
            VarSet(VAR_ALBUM_BONUS_DISPLAYED_START_ID,         sAlbumPtr->bonusDisplayedStartId);
        }

        if (sAlbumPtr->memoryData[sAlbumPtr->selectedMemory].unlocked)
        {
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
            PlaySE(SE_SELECT);
            VarSet(VAR_IS_BONUS_PAGE, sAlbumPtr->isBonusPage);
            gTasks[taskId].func = Task_AlbumShowImage;
        }
    }

    if (gMain.newKeys & B_BUTTON)
    {
        PlaySE(SE_PC_OFF);
        FlagClear(FLAG_ALBUM_SE_DONE);
        VarSet(VAR_ALBUM_SELECTED_MEMORY, 0);
        VarSet(VAR_ALBUM_SELECTED_MEMORY_IN_ALBUM, 0);
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
    PrintGUIAlbumPageInstructions();
    PrintGUIAlbumMemoriesUnlocked();
}

static void InitAlbum(void)
{
    // Remove glitches
    CleanWindows();
    CommitWindows();

    // Which page are we coming back to?
    sAlbumPtr->isBonusPage = VarGet(VAR_IS_BONUS_PAGE);

    // Restore both pages' saved positions
    sAlbumPtr->normalSelectedMemory          = VarGet(VAR_ALBUM_NORMAL_SELECTED_MEMORY);
    sAlbumPtr->normalSelectedMemoryInAlbum   = VarGet(VAR_ALBUM_NORMAL_SELECTED_MEMORY_IN_ALBUM);
    sAlbumPtr->normalDisplayedStartId        = VarGet(VAR_ALBUM_NORMAL_DISPLAYED_START_ID);

    sAlbumPtr->bonusSelectedMemory           = VarGet(VAR_ALBUM_BONUS_SELECTED_MEMORY);
    sAlbumPtr->bonusSelectedMemoryInAlbum    = VarGet(VAR_ALBUM_BONUS_SELECTED_MEMORY_IN_ALBUM);
    sAlbumPtr->bonusDisplayedStartId         = VarGet(VAR_ALBUM_BONUS_DISPLAYED_START_ID);

    // For compatibility with existing single-page vars, fall back if unset
    if (!sAlbumPtr->isBonusPage)
    {
        if (sAlbumPtr->normalSelectedMemory == 0 
            && VarGet(VAR_ALBUM_SELECTED_MEMORY) != 0)
        {
            sAlbumPtr->normalSelectedMemory        = VarGet(VAR_ALBUM_SELECTED_MEMORY);
            sAlbumPtr->normalSelectedMemoryInAlbum = VarGet(VAR_ALBUM_SELECTED_MEMORY_IN_ALBUM);
            sAlbumPtr->normalDisplayedStartId      = sAlbumPtr->normalSelectedMemory
                                                    - sAlbumPtr->normalSelectedMemoryInAlbum;
        }
        sAlbumPtr->selectedMemory        = sAlbumPtr->normalSelectedMemory;
        sAlbumPtr->selectedMemoryInAlbum = sAlbumPtr->normalSelectedMemoryInAlbum;
        sAlbumPtr->displayedStartId      = sAlbumPtr->normalDisplayedStartId;
    }
    else
    {
        if (sAlbumPtr->bonusSelectedMemory == 0 
            && VarGet(VAR_ALBUM_SELECTED_MEMORY) != 0)
        {
            sAlbumPtr->bonusSelectedMemory         = VarGet(VAR_ALBUM_SELECTED_MEMORY);
            sAlbumPtr->bonusSelectedMemoryInAlbum  = VarGet(VAR_ALBUM_SELECTED_MEMORY_IN_ALBUM);
            sAlbumPtr->bonusDisplayedStartId       = sAlbumPtr->bonusSelectedMemory
                                                    - sAlbumPtr->bonusSelectedMemoryInAlbum;
        }
        sAlbumPtr->selectedMemory        = sAlbumPtr->bonusSelectedMemory;
        sAlbumPtr->selectedMemoryInAlbum = sAlbumPtr->bonusSelectedMemoryInAlbum;
        sAlbumPtr->displayedStartId      = sAlbumPtr->bonusDisplayedStartId;
    }

    InitAlbumData(sAlbumPtr->isBonusPage);
    PrintGUIAlbumItems();
}

static void CB2_Album(void)
{
    switch (gMain.state)
    {
        case 0:
            SetVBlankCallback(NULL);
            ClearVramOamPlttRegs();
            if (sAlbumPtr == NULL)
            {
                sAlbumPtr = Calloc(sizeof(struct Album));
                sAlbumPtr->isBonusPage = VarGet(VAR_IS_BONUS_PAGE);
            }
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

bool8 StartMenuAlbumCallback(void)
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
    .charBaseIndex = 0,
    .mapBaseIndex = 31,
    .screenSize = 0,
    .paletteMode = 0,
    .priority = 0,
    .baseTile = 0,
};

static void LoadAlbumImage(u8 memoryId)
{
    const struct ImageData *image = &ImageDataTable[memoryId];

    // Clear all 64 KB of BG CHR so large tilesets don't mix with leftovers
    CpuFastFill16(0, (void*)BG_CHAR_ADDR(0), BG_CHAR_SIZE * 4);

    CpuFastFill16(0, tilemapbuffer, BG_MAP_BYTES);
    decompress_and_copy_tile_data_to_vram(BG_BACKGROUND, image->tiles, 0, 0, 0);
    LZDecompressWram(image->tilemap, tilemapbuffer);
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
        {
            u16 index = VarGet(VAR_ALBUM_SELECTED_MEMORY);
            bool8 bonus = VarGet(VAR_IS_BONUS_PAGE);
            u8 memoryId = bonus ? sBonusMemoryOrder[index] : sMemoryOrder[index];
            LoadAlbumImage(memoryId);
            gMain.state++;
            break;
        }
        case 3:
            if (!free_temp_tile_data_buffers_if_possible())
            {
                CopyBgTilemapBufferToVram(BG_BACKGROUND);
                ShowBg(BG_BACKGROUND);
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
    Free(sAlbumPtr->bgMap);
    Free(sAlbumPtr);
    sAlbumPtr = NULL;
    FreeAllWindowBuffers();

    gMain.state = 0;
    SetMainCallback2(CB2_Image);
}
