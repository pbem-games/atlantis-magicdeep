// START A3HEADER
//
// This source file is part of the Atlantis PBM game program.
// Copyright (C) 1995-1999 Geoff Dunbar
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program, in the file license.txt. If not, write
// to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// See the Atlantis Project web page for details:
// http://www.prankster.com/project
//
// END A3HEADER
// MODIFICATIONS
// Date		Person			Comments
// ----		------			--------
// 2001/Feb/21 Joseph Traub	  Move the item and such defintions out into
//							   gamedata.cpp
//
#include "gamedata.h"
#include "gamedefs.h"

//
// Define the various globals for this game.
//
// If you change any of these, it is incumbent on you, the GM to change
// the html file containing the rules to correctly reflect the changes!
//

static int am[] = { 0, 1, 2, 3, 4, 5, 6, 7 }; //{ 0, 1, 2, 3, 5, 7 };
int *allowedMages = am;
int allowedMagesSize = sizeof(am) / sizeof(am[0]);

static int aa[] = { 0, 2, 4, 6, 8, 10, 12, 14 }; //{ 0, 2, 4, 6, 10, 14 };
int *allowedApprentices = aa;
int allowedApprenticesSize = sizeof(aa) / sizeof(aa[0]);

static int aw[] = { 0, 4, 10, 18, 28, 40, 54, 70 }; //{ 0, 5, 12, 22, 36, 54, 75, 100 };
int *allowedTaxes = aw;
int allowedTaxesSize = sizeof(aw) / sizeof(aw[0]);

static int at[] = { 0, 4, 10, 18, 28, 40, 54, 70 }; //{ 0, 5, 12, 22, 36, 54, 75, 100 };
int *allowedTrades = at;
int allowedTradesSize = sizeof(at) / sizeof(at[0]);

static int ppm[] = { 22, 20, 17, 14, 12, 9, 6 };
int *magesPPCosts = ppm;
int magesPPCostsSize = sizeof(ppm) / sizeof(ppm[0]);

static int ppa[] = { 6, 5 };
int *apprenticesPPCosts = ppa;
int apprenticesPPCostsSize = sizeof(ppa) / sizeof(ppa[0]);

static GameDefs g = {
	"MagicDeep",	 // RULESET_NAME
	MAKE_ATL_VER( 1, 2, 0 ), // RULESET_VERSION

	2, /* FOOT_SPEED */
	4, /* HORSE_SPEED */
	6, /* SHIP_SPEED */
	6, /* FLY_SPEED */
	8, /* MAX_SPEED */

	10, /* STUDENTS_PER_TEACHER */
	10, /* MAINTENANCE_COST */
	40, /* LEADER_COST */
        200,/* MAGE_COST */

	0,  /* MAINTAINENCE_MULTIPLIER */
	GameDefs::MULT_NONE, /* MULTIPLIER_USE */

	33, /* STARVE_PERCENT */
	GameDefs::STARVE_NONE, /* SKILL_STARVATION */

	10000, /* START_MONEY */
	5, /* WORK_FRACTION */
	20, /* ENTERTAIN_FRACTION */
	20, /* ENTERTAIN_INCOME */

	50, /* TAX_INCOME */

	5, /* HEALS_PER_MAN */

	20, /* GUARD_REGEN */ /* percent */
	40, /* CITY_GUARD */
	50, /* GUARD_MONEY */
	4000, /* CITY_POP */

	20, /* WMON_FREQUENCY */
	20, /* LAIR_FREQUENCY */

	7, /* FACTION_POINTS */

	50, /* TIMES_REWARD */

	1, // TOWNS_EXIST
        0, // STARTCITYES
	1, // LEADERS_EXIST
	3, // SKILL_LIMIT_NONLEADERS
	0, // MAGE_NONLEADERS
	1, // RACES_EXIST
	1, // GATES_EXIST
	1, // FOOD_ITEMS_EXIST
	1, // CITY_MONSTERS_EXIST
	1, // WANDERING_MONSTERS_EXIST
	1, // LAIR_MONSTERS_EXIST
	1, // WEATHER_EXISTS
	1, // OPEN_ENDED
	1, // NEXUS_EXISTS
	0, // CONQUEST_GAME

	1, // RANDOM_ECONOMY
	1, // VARIABLE_ECONOMY

	60, // CITY_MARKET_NORMAL_AMT
	25, // CITY_MARKET_ADVANCED_AMT
	50, // CITY_MARKET_TRADE_AMT
	0,  // CITY_MARKET_MAGIC_AMT
	1,  // MORE_PROFITABLE_TRADE_GOODS

	50,	// BASE_MAN_COST
	1,  // LASTORDERS_MAINTAINED_BY_SCRIPTS
	10, // MAX_INACTIVE_TURNS

	0, // EASIER_UNDERWORLD

	1, // DEFAULT_WORK_ORDER

	GameDefs::FACLIM_POWER_POINTS, // FACTION_LIMIT_TYPE
//	GameDefs::FACLIM_FACTION_TYPES, // FACTION_LIMIT_TYPE

	GameDefs::WFLIGHT_NONE,	// FLIGHT_OVER_WATER

	0,   // SAFE_START_CITIES
	250, // AMT_START_CITY_GUARDS
	250, // START_CITY_GUARDS_PLATE
	0,   // START_CITY_MAGES
	0,   // START_CITY_TACTICS

	0,   // APPRENTICES_EXIST

	"Atlantis Magicdeep", // WORLD_NAME

	1,  // NEXUS_GATE_OUT
	0,  // NEXUS_IS_CITY
	1,	// BATTLE_FACTION_INFO
	1,	// ALLOW_WITHDRAW
	50000,	// CITY_RENAME_COST
	0,	// TAX_PILLAGE_MONTH_LONG
	0,	// MULTI_HEX_NEXUS
	1,	// UNDERWORLD_LEVELS
	0,	// UNDERDEEP_LEVELS
	0,	// ABYSS_LEVEL
	50,	// TOWN_PROBABILITY
	1,	// TOWN_SPREAD
	1,	// TOWNS_NOT_ADJACENT
	0,	// LESS_ARCTIC_TOWNS
	0,	// ARCHIPELAGO
	0,	// LAKES_EXIST
	1,	// ADVANCED_MAP		//Klaus
	0,	// ADVANCED_MOVEMENT       //Klaus
	GameDefs::NO_EFFECT, // LAKE_WAGE_EFFECT
	1,	// LAKESIDE_IS_COASTAL
	0,	// ODD_TERRAIN
	1,	// IMPROVED_FARSIGHT
	0,	// GM_REPORT
	0,	// DECAY
	1,	// LIMITED_MAGES_PER_BUILDING
	GameDefs::REPORT_SHOW_FASTVIEW, // TRANSIT_REPORT
	1,      // MARKETS_SHOW_ADVANCED_ITEMS
	0,	// USE_PREPARE_COMMAND
	5,	// MONSTER_ADVANCE_MIN_PERCENT
	50,	// MONSTER_ADVANCE_HOSTILE_PERCENT
	1,	// HAVE_EMAIL_SPECIAL_COMMANDS
	0,	// START_CITIES_START_UNLIMITED
	1,	// PROPORTIONAL_AMTS_USAGE
	1,      // USE_WEAPON_ARMOR_COMMAND
	12, // MONSTER_NO_SPOILS
	3,  // MONSTER_SPOILS_RECOVERY
	0,  // MAX_ASSASSIN_FREE_ATTACKS
	1,  // RELEASE_MONSTERS
	1,  // CHECK_MONSTER_CONTROL_MID_TURN
	1,  // DETECT_GATE_NUMBERS
	GameDefs::ARMY_ROUT_HITS_FIGURE,  // ARMY_ROUT
	0,	// FULL_TRUESEEING_BONUS
	0,	// IMPROVED_AMTS
	0,	// FULL_INVIS_ON_SELF
	0,	// MONSTER_BATTLE_REGEN
	GameDefs::TAX_NORMAL, // WHO_CAN_TAX
	5,	// SKILL_PRACTISE_AMOUNT
	0,	// UPKEEP_MINIMUM_FOOD
	10,	// UPKEEP_MAXIMUM_FOOD
	10,	// UPKEEP_FOOD_VALUE
	1,	// PREVENT_SAIL_THROUGH
	1,	// ALLOW_TRIVIAL_PORTAGE
	150, // SPY_WEIGHT           // VVR
	1, // LEADER_CITY_ONLY     // VVR
	1,	// MULTY_RACIAL_CITIES   // Klaus
	7, // NUM_LESSAVATAR;      // VVR
	3, // NUM_AVATAR;          // VVR
        1, // SUP_SKILL_PP         // VVR
	// *** Klaus
	4,	// ARM_FORTRESS_PROBABILITY
	7,	// ARM_FORTRESS_RUINED_CHANCE
	3, // ARM_INN_PROBABILITY
	-5, // ARM_INN_RUINED_CHANCE
	3,	// ARM_FACTORY_PROBABILITY
	5,	// ARM_FACTORY_RUINED_CHANCE
	5,	// ARM_ROAD_PROBABILITY
	128,	// ARM_ROAD_RUINED_CHANCE		// Klaus ***

	1,	// GUARD_LEVEL_BY_TAX 		// Beholder
	1,	// ALLOW_WITHDRAW_MEN		// Beholder
	GameDefs::SAIL_LIGHT_SLOWED3_BAD_WEATHER|GameDefs::SAIL_AIR_SLOWED3_BAD_WEATHER, // SHIP_CLASS_MODIFERS;
	GameDefs::VISIBLE_SEA_FROM_ANY_FORTRESS, 	  // ADVANCED_VISIBILITY; //Klaus
	GameDefs::VISIBILITY_TRANSIT,                     // ADVANCED_VISIBILITY_TYPE; // Klaus
	0,	// PERSONAL_MANA        // wiz
	100,	// FACTION_POWER_POINTS		// Beholder
	3, // UFAS_OPTIONS		// Beholder
	1, // WIN_AMUNITION_LOSS // amunition loss by Beholder
	90, // SPOILS_PERCENTAGE
	1,// QUARTER_MANA_AT_SEA
	3, //STARTING_CITY_MANA_LIMIT
};

GameDefs * Globals = &g;
