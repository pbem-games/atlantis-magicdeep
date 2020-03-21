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
#ifdef GAME_SPELLS
//
// If GAME_SPELLS is defined, this is being included from inside the Game
// class in game.h
//

//
// Spell parsing - generic
//
void ProcessGenericSpell(Unit *, int, OrdersCheck *pCheck, int level);
void ProcessRegionSpell(Unit *, AString *, int, OrdersCheck *pCheck, int level);

//
// Spell parsing - specific
//
void ProcessCastGateLore(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessCastPortalLore(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessPhanBeasts(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessPhanUndead(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessPhanDemons(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessInvisibility(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessTrueSeeing(Unit *,AString *, OrdersCheck *pCheck, int level);     // VVR
void ProcessBirdLore(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessMindReading(Unit *,AString *, OrdersCheck *pCheck, int level);
void ProcessLacandonTeleport(Unit *, AString *, OrdersCheck *pCheck, int level);
void ProcessCreateCloaks(Unit *, AString *, int, OrdersCheck *pCheck, int level);      // VVR

void ProcessDragonLore(Unit *, AString *, int, OrdersCheck *pCheck, int level); // Beholder
void ProcessSummonGolem(Unit *, AString *, int, OrdersCheck *pCheck, int level); // VVR
void ProcessTransformFromSpell(Unit *, AString *, int, OrdersCheck *pCheck, int level); // Goony

// dp_wiz
void ProcessCastChannel(Unit *u,AString *o, OrdersCheck *pCheck, int lv);

//
// Spell helpers
//
int GetRegionInRange(ARegion *r, ARegion *tar, Unit *u, int spell, int level);

//
// Spell running
//
bool RunDetectGates(ARegion *,Object *,Unit *, int level);
bool RunFarsight(ARegion *,Unit *, int level);
bool RunGateJump(ARegion *,Object *,Unit *, int level);
bool RunTeleport(ARegion *,Object *,Unit *, int level);
bool RunLacandonTeleport(ARegion *, Object *, Unit *, int level);
bool RunPortalLore(ARegion *,Object *,Unit *, int level);
bool RunNatureLore(ARegion *,Unit *, int level);
bool RunWeatherLore(ARegion *, Unit *, int level);
bool RunClearSkies(ARegion *,Unit *, int level);
bool RunPhanBeasts(ARegion *,Unit *, int level);
bool RunPhanUndead(ARegion *,Unit *, int level);
bool RunPhanDemons(ARegion *,Unit *, int level);
bool RunInvisibility(ARegion *,Unit *, int level);
bool RunTrueSeeing(ARegion *,Unit *, int level);        // VVR
bool RunWolfLore(ARegion *,Unit *, int level);
bool RunBirdLore(ARegion *,Unit *, int level);
bool RunDragonLore(ARegion *,Unit *, int level);
bool RunSummonSkeletons(ARegion *,Unit *, int level);
bool RunRaiseUndead(ARegion *,Unit *, int level);
bool RunSummonLich(ARegion *,Unit *, int level);
bool RunSummonImps(ARegion *,Unit *, int level);
bool RunSummonDemon(ARegion *,Unit *, int level);
bool RunSummonBalrog(ARegion *,Unit *, int level);
bool RunSummonGolem(ARegion *,Unit *, int level);
bool RunCreateArtifact(ARegion *,Unit *,int,int, int level);
bool RunEngraveRunes(ARegion *,Object *,Unit *, int level);
bool RunConstructGate(ARegion *,Unit *, int level);
//bool RunEnchantSwords(ARegion *,Unit *, int level);
//bool RunEnchantArmor(ARegion *,Unit *, int level);
bool RunTransformFromItem(ARegion *,Unit *, int skill, int level, bool squaredLevel); // Goony
bool RunMindReading(ARegion *,Unit *, int level);
bool RunCreateFood(ARegion *,Unit *, int level);
bool RunAlchemy(ARegion *,Unit *, int level);
        // VVR
bool RunManaGeneration(ARegion *,Unit *, int level);
bool RunCreateManaPotion(ARegion *,Unit *, int level);
bool RunCreateCloaks(ARegion *,Unit *,int, int level);
bool RunChannel(ARegion *,Unit *, int level);
#endif
