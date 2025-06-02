#include "../config.h"

// Define which Pokemon can be found in raids on Route 1
static struct Raid sRoute1Raids1Star[] =
{
	{
		.species = SPECIES_PELIPPER,
		.ability = RAID_ABILITY_RANDOM_ALL,
		.drops =
		{
			/*100 %*/ ITEM_RARE_CANDY,
			/* 80 %*/ ITEM_WACAN_BERRY,
			/* 80 %*/ ITEM_GREPA_BERRY,
			/* 50 %*/ ITEM_HYPER_POTION,
			/* 50 %*/ ITEM_NONE,
			/* 30 %*/ ITEM_NONE,
			/* 30 %*/ ITEM_NONE,
			/* 25 %*/ ITEM_PRETTY_WING,
			/* 25 %*/ ITEM_RARE_CANDY,
			/*  5 %*/ ITEM_WATER_GEM,
			/*  4 %*/ ITEM_LUCKY_EGG,
			/*  1 %*/ ITEM_BOTTLE_CAP,
		},
	},
	{
		.species = SPECIES_MASQUERAIN,
		.ability = RAID_ABILITY_RANDOM_ALL,
		.drops =
		{
			/*100 %*/ ITEM_RARE_CANDY,
			/* 80 %*/ ITEM_CHARTI_BERRY,
			/* 80 %*/ ITEM_TAMATO_BERRY,
			/* 50 %*/ ITEM_REVIVE,
			/* 50 %*/ ITEM_NONE,
			/* 30 %*/ ITEM_NONE,
			/* 30 %*/ ITEM_NONE,
			/* 25 %*/ ITEM_NUGGET,
			/* 25 %*/ ITEM_RARE_CANDY,
			/*  5 %*/ ITEM_FLYING_GEM,
			/*  4 %*/ ITEM_FOCUS_SASH,
			/*  1 %*/ ITEM_BOTTLE_CAP,
		},
	},
};

// Map Data that contains raids for each map section
const struct RaidData gRaidsByMapSection[KANTO_MAPSEC_COUNT][RAID_STAR_COUNT] =
{
	[MAPSEC_ROUTE_1 -  MAPSEC_DYNAMIC] =
	{
		[ONE_STAR_RAID] = {sRoute1Raids1Star, NELEMS(sRoute1Raids1Star)},
	},
};
