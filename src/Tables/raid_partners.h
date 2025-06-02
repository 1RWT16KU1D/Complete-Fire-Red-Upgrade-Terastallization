#include "../config.h"
#include "../../include/constants/moves.h"
#include "../../include/constants/items.h"
#include "../../include/constants/species.h"
#include "../../include/constants/trainers.h"
#include "../../include/constants/event_objects.h"

// Raid Partners for the Battle Tower / Tera Raids
static const struct BattleTowerSpread sRaidPartnerSpread_Red[] =
{
	{
		.species = SPECIES_PIKACHU_CAP_ORIGINAL,
		.nature = NATURE_MODEST,
		.hpIv = 31,
		.atkIv = 0,
		.defIv = 31,
		.spAtkIv = 31,
		.spDefIv = 31,
		.spdIv = 31,
		.spAtkEv = 252,
		.spDefEv = 4,
		.spdEv = 252,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_LIGHTNINGROD
		.item = ITEM_PIKASHUNIUM_Z,
		.moves =
		{
			MOVE_THUNDERBOLT,
			MOVE_GRASSKNOT,
			MOVE_VOLTSWITCH,
			MOVE_REFLECT,
		},
		.ball = BALL_TYPE_POKE_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
	{
		.species = SPECIES_SNORLAX,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.spDefEv = 4,
		.ability = FRONTIER_ABILITY_HIDDEN, //ABILITY_GLUTTONY
		.item = ITEM_AGUAV_BERRY,
		.moves =
		{
			MOVE_BODYSLAM,
			MOVE_CRUNCH,
			MOVE_HIGHHORSEPOWER,
			MOVE_RECYCLE,
		},
		.ball = BALL_TYPE_MASTER_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
		.gigantamax = TRUE,
	},
	{
		.species = SPECIES_MEWTWO,
		.nature = NATURE_ADAMANT,
		.hpIv = 31,
		.atkIv = 31,
		.defIv = 31,
		.spAtkIv = 0,
		.spDefIv = 31,
		.spdIv = 31,
		.hpEv = 252,
		.atkEv = 252,
		.ability = FRONTIER_ABILITY_1, //ABILITY_PRESSURE
		.item = ITEM_MEWTWONITE_X,
		.moves =
		{
			MOVE_ZENHEADBUTT,
			MOVE_BRICKBREAK,
			MOVE_POISONJAB,
			MOVE_RECOVER,
		},
		.ball = BALL_TYPE_MASTER_BALL,
		.forSingles = TRUE,
		.forDoubles = TRUE,
		.modifyMovesDoubles = FALSE,
	},
};

static const struct BattleTowerSpread sRaidPartnerSpread_Leaf[] =
{
    {
        .species = SPECIES_VENUSAUR,
        .nature = NATURE_BOLD,
        .hpIv = 31, .atkIv = 0, .defIv = 31, .spAtkIv = 31, .spDefIv = 31, .spdIv = 31,
        .hpEv = 252, .defEv = 252, .spDefEv = 4,
        .ability = FRONTIER_ABILITY_HIDDEN, // ABILITY_CHLOROPHYLL
        .item = ITEM_LEFTOVERS,
        .moves = {MOVE_GIGADRAIN, MOVE_SLUDGEBOMB, MOVE_SYNTHESIS, MOVE_LEECHSEED},
        .ball = BALL_TYPE_ULTRA_BALL,
        .forSingles = TRUE, .forDoubles = TRUE, .modifyMovesDoubles = FALSE,
    },
};

static const struct BattleTowerSpread sRaidPartnerSpread_Blue[] =
{
    {
        .species = SPECIES_ARCANINE,
        .nature = NATURE_ADAMANT,
        .hpIv = 31, .atkIv = 31, .defIv = 31, .spAtkIv = 0, .spDefIv = 31, .spdIv = 31,
        .hpEv = 252, .atkEv = 252, .spdEv = 4,
        .ability = FRONTIER_ABILITY_HIDDEN, // ABILITY_INTIMIDATE
        .item = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FLAREBLITZ, MOVE_EXTREMESPEED, MOVE_WILDCHARGE, MOVE_MORNINGSUN},
        .ball = BALL_TYPE_GREAT_BALL,
        .forSingles = TRUE, .forDoubles = TRUE, .modifyMovesDoubles = FALSE,
    },
};

static const struct BattleTowerSpread sRaidPartnerSpread_Cynthia[] =
{
    {
        .species = SPECIES_GARCHOMP,
        .nature = NATURE_JOLLY,
        .hpIv = 31, .atkIv = 31, .defIv = 31, .spAtkIv = 0, .spDefIv = 31, .spdIv = 31,
        .hpEv = 252, .atkEv = 252, .spdEv = 4,
        .ability = FRONTIER_ABILITY_1, // ABILITY_SAND_VEIL
        .item = ITEM_LUM_BERRY,
        .moves = {MOVE_DRAGONCLAW, MOVE_EARTHQUAKE, MOVE_SWORDSDANCE, MOVE_ROCKSLIDE},
        .ball = BALL_TYPE_DUSK_BALL,
        .forSingles = TRUE, .forDoubles = TRUE, .modifyMovesDoubles = FALSE,
    },
};

static const struct BattleTowerSpread sRaidPartnerSpread_Steven[] =
{
    {
        .species = SPECIES_METAGROSS,
        .nature = NATURE_ADAMANT,
        .hpIv = 31, .atkIv = 31, .defIv = 31, .spAtkIv = 0, .spDefIv = 31, .spdIv = 31,
        .hpEv = 252, .atkEv = 252, .defEv = 4,
        .ability = FRONTIER_ABILITY_1, // ABILITY_CLEAR_BODY
        .item = ITEM_METAGROSSITE,
        .moves = {MOVE_METEORMASH, MOVE_EARTHQUAKE, MOVE_BULLETPUNCH, MOVE_ZENHEADBUTT},
        .ball = BALL_TYPE_TIMER_BALL,
        .forSingles = TRUE, .forDoubles = TRUE, .modifyMovesDoubles = FALSE,
    },
};

// Trainer Name Defines
extern const u8 sTrainerName_Red[];
extern const u8 sTrainerName_Leaf[];
extern const u8 sTrainerName_Blue[];
extern const u8 sTrainerName_Cynthia[];
extern const u8 sTrainerName_Steven[];

// Example spreads for each partner (customize as needed)

const struct MultiRaidTrainer gRaidPartners[] =
{
	{
		.owNum = EVENT_OBJ_GFX_LEAF_NORMAL,
		.trainerClass = CLASS_PKMN_TRAINER_3,
		.backSpriteId = TRAINER_BACK_PIC_LEAF,
		.gender = FEMALE,
		.otId = 0x2,
		.name = sTrainerName_Leaf,
		.spreads = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Leaf,
		},
		.spreadSizes = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Leaf),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_RIVAL,
		.trainerClass = CLASS_RIVAL_2,
		.backSpriteId = TRAINER_BACK_PIC_BRENDAN,
		.gender = MALE,
		.otId = 0x3,
		.name = sTrainerName_Blue,
		.spreads = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Blue,
		},
		.spreadSizes = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Blue),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_MAY,
		.trainerClass = CLASS_CHAMPION,
		.backSpriteId = TRAINER_BACK_PIC_MAY,
		.gender = FEMALE,
		.otId = 0x4,
		.name = sTrainerName_Cynthia,
		.spreads = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Cynthia,
		},
		.spreadSizes = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Cynthia),
		},
	},
	{
		.owNum = EVENT_OBJ_GFX_BEAUTY,
		.trainerClass = CLASS_BEAUTY,
		.backSpriteId = TRAINER_BACK_PIC_LEAF,
		.gender = FEMALE,
		.otId = 0x5,
		.name = sTrainerName_Steven,
		.spreads = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = sRaidPartnerSpread_Steven,
		},
		.spreadSizes = {
			[ONE_STAR_RAID ... THREE_STAR_RAID] = NELEMS(sRaidPartnerSpread_Steven),
		},
	},
};

const u8 gNumRaidPartners = NELEMS(gRaidPartners);
