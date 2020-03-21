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
// Date		 Person		       	Comments
// ----		 ------		       	--------
// 2003/JAN/10	 Sergey A. Lizorkin    	Created file.
// 2003/MAY/15	 Sergey A. Lizorkin    	Reorganized patterns, city guards
//                                      selection is now made by race an then
//                                      by region type.
// 2003/JUN/05	 Sergey A. Lizorkin     Added PT_STARTCITY and PT_ALLRACES flags.

#ifndef NPC_PATTERNS
#define NPC_PATTERNS

struct NPCEquipment {
        int type;
        int num[4];
};

struct NPCSkill {
        int type;
        int level[4];
};

class NPCPattern {
public:
        char * name;
        enum {
                NPC_FLAG_GUARD = 0x0001,        /* defines if NPC should atempt to guard */
                NPC_FLAG_BEHIND = 0x0002,       /* defines if NPC should be behind */
                NPC_FLAG_HOLD = 0x0004,         /* defines if NPC should be holding */
                NPC_FLAG_AVOID = 0x0008,        /* defines if NPC should be avoiding */
                NPC_FLAG_ISMAGE = 0x0010,       /* defines if NPC should be considered as a mage */
                NPC_CONSTANTMEN = 0x0020,       /* defines if NPC should be created with constant
                                                number of men regardless of circumstances. Mages
                                                are always considered CONSTANTMEN */
                NPC_OCCUPYOBJECT = 0x0040,       /* defines if NPC should occupy buildings */
                NPC_NATIVE_GUARDS = 0x0080,      /* defines if city guards should be recruited from
                                                 locals, thus ignoring NPCPattern::men.type property */
                NPC_ALL_TERRAIN = 0x0100,        /* indicates that this NPC is suited for all types of terrain*/
        };
        int flags;
        NPCEquipment men;
        NPCEquipment equipment[10];
        NPCSkill skills[10];
        int combatSpell [4];
        int silverAmount [4];
        enum {
                NPC_OBJ_NONE = 0,
                NPC_OBJ_PROTECTION,
                NPC_OBJ_INN,
                NPC_OBJ_TEMPLE,
                NPC_OBJ_MARKET,
                NPC_OBJ_FARM,
                NPC_OBJ_RANCH,
        };

        int favoriteObject;
        enum {
                NPC_BHV_NONE = 0,
                NPC_BHV_GUARDSMEN,
                NPC_BHV_WMON,
                NPC_BHV_NEUTRAL,
                NPC_BHV_ADVENTURER,
                NPC_BHV_WMAGE,
                NPC_BHV_NMON,
                NPC_BHV_ROGUE,
        };
        int behavior;
        enum {
               NPC_FACT_GUARDSMEN = 1,
               NPC_FACT_WMON,
               NPC_FACT_NEUTRAL,
        };
        int faction;
        int terrainType[7];
};

extern NPCPattern * NPCDefs;

class CityGuardPattern {
public:
        int index[7];
        int raceType[5];
        enum {
           PT_DISABLED=0x01,
           PT_ALLRACES=0x02, // city guard suitable for all races
           PT_STARTCITY=0x04, // only for starting cities
        };
        int flags;
};

extern CityGuardPattern * CityGuardDefs;

#endif
