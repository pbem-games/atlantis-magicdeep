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
// Date     Person        Comments
// ----     ------        --------
// 2003/JAN/10   Sergey A. Lizorkin  Added City Guards customization.
// 2003/MAY/27   Sergey A. Lizorkin  Bugfixes, moved city guard selection code
//                                   to ARegion::PickCityMon ()
// 2003/JUN/05   Sergey A. Lizorkin  Added handling of PT_STARTCITY bit in
//                                   CityGuardPattern::flags.
// 2003/JUL/11   Sergey A. Lizorkin  Minor fixes in City Guards maintnance
//                                   and regeneration.

#include "game.h"
#include "gamedata.h"
#include "npc.h"

void Game::CreateWMons()
{
    if( !Globals->WANDERING_MONSTERS_EXIST )
    {
        return;
    }

    GrowWMons( 50 );
}

void Game::CreateLMons()
{
    if( !Globals->LAIR_MONSTERS_EXIST )
    {
        return;
    }

    GrowLMons( 50 );
}

void Game::GrowWMons(int rate)
{
    //
    // Now, go through each 8x8 block of the map, and make monsters if
    // needed.
    //
    int level;
    for( level = 0; level < regions.numLevels; level++ )
    {
        ARegionArray *pArr = regions.pRegionArrays[ level ];
        int xsec;
        for (xsec=0; xsec< pArr->x / 8; xsec++)
        {
            for (int ysec=0; ysec< pArr->y / 16; ysec++)
            {
                /* OK, we have a sector. Count mons, and wanted */
                int mons=0;
                int wanted=0;
                for (int x=0; x<8; x++)
                {
                    for (int y=0; y<16; y+=2)
                    {
                        ARegion *reg = pArr->GetRegion(x+xsec*8,y+ysec*16+x%2);
                        if (!reg->IsGuarded())
                        {
                            mons += reg->CountWMons();
              /*
               * Make sure there is at least one monster type
               * enabled for this region
               */
              int avail = 0;
              int mon = TerrainDefs[reg->type].smallmon;
              if(!((mon == -1) ||
                   (ItemDefs[mon].flags & ItemType::DISABLED)))
                avail = 1;
              mon = TerrainDefs[reg->type].bigmon;
              if(!((mon == -1) ||
                   (ItemDefs[mon].flags & ItemType::DISABLED)))
                avail = 1;
              mon = TerrainDefs[reg->type].humanoid;
              if(!((mon == -1) ||
                   (ItemDefs[mon].flags & ItemType::DISABLED)))
                avail = 1;

              if(avail)
                wanted += TerrainDefs[reg->type].wmonfreq;

                        }
                    }
                }

                wanted /= 10;
                wanted -= mons;
                wanted = (wanted*rate + getrandom(100))/100;
                if (wanted > 0)
                {
                    for (int i=0; i< wanted;)
                    {
                        int m=getrandom(8);
                        int n=getrandom(8)*2+m%2;
                        ARegion *reg = pArr->GetRegion(m+xsec*8,n+ysec*16);
                        if (!reg->IsGuarded() && MakeWMon( reg ))
                        {
                            i++;
                        }
                    }
                }
            }
        }
    }
}

void Game::GrowLMons( int rate )
{
    forlist(&regions) {
        ARegion * r = (ARegion *) elem;
        //
        // Don't make lmons in guarded regions
        //
        if (r->IsGuarded()) continue;

        forlist(&r->objects) {
            Object * obj = (Object *) elem;
            if (obj->units.Num()) continue;
            int montype = ObjectDefs[obj->type].monster;
      int grow=!(ObjectDefs[obj->type].flags&ObjectType::NOMONSTERGROWTH);
            if ((montype != -1) && grow) {
                //+++++ Artifact creatures can't be placed to lairs
                if (!(MonDefs[ItemDefs[montype].index].spoiltype & IT_ARTIFACT))
                  if (getrandom(100) < rate) {
                        MakeLMon( obj );
                  }
            }
        }
    }
}

int Game::MakeWMon( ARegion *pReg )
{
        AString *nameavatar = new AString(GetHeroName((2+numav+numlessav)*(2+numav+numlessav), 0));

  if(!Globals->WANDERING_MONSTERS_EXIST) return 0;

  if (TerrainDefs[pReg->type].wmonfreq == 0) return 0;

  int montype = TerrainDefs[ pReg->type ].smallmon;
  if (getrandom(2) && (TerrainDefs[pReg->type].humanoid != -1))
    montype = TerrainDefs[ pReg->type ].humanoid;
  if (TerrainDefs[ pReg->type ].bigmon != -1 && !getrandom(8))
    montype = TerrainDefs[ pReg->type ].bigmon;

        if (!getrandom(20) && (numlessav < Globals->NUM_LESSAVATAR) &&
                        (TerrainDefs[ pReg->type ].similar_type != R_OCEAN)) {
                montype = I_LESSAVATAR;
                numlessav++;
        } else {
                if (!getrandom(30) && (numav < Globals->NUM_AVATAR) &&
                        (TerrainDefs[ pReg->type ].similar_type != R_OCEAN)) {
                        montype = I_AVATAR;
                        numav++;
                }
        }
  if((montype == -1) || (ItemDefs[montype].flags & ItemType::DISABLED))
    return 0;

  int mondef = ItemDefs[montype].index;
  Faction *monfac = GetFaction( &factions, 2 );
  Unit *u = GetNewUnit( monfac, 0 );
  u->MakeWMon( MonDefs[mondef].name, montype,
      (MonDefs[mondef].number+getrandom(MonDefs[mondef].number)+1)/2);
        if(montype == I_AVATAR || montype == I_LESSAVATAR) u->SetName(nameavatar);      // VVR

  u->MoveUnit( pReg->GetDummy() );
  return( 1 );
}

void Game::MakeLMon( Object *pObj )
{
  if(!Globals->LAIR_MONSTERS_EXIST) return;
  if(ObjectDefs[pObj->type].flags & ObjectType::NOMONSTERGROWTH) return;

  int montype = ObjectDefs[ pObj->type ].monster;

  if (montype == I_TRENT) {
    montype = TerrainDefs[ pObj->region->type].bigmon;
  }
  if (montype == I_CENTAUR) {
    montype = TerrainDefs[ pObj->region->type ].humanoid;
  }
  if((montype == -1) || (ItemDefs[montype].flags & ItemType::DISABLED))
    return;

  int mondef = ItemDefs[montype].index;
  Faction *monfac = GetFaction( &factions, 2 );
  Unit *u = GetNewUnit( monfac, 0 );
  switch(montype) {
    case I_IMP:
      u->MakeWMon( "Demons", I_IMP,
          getrandom( MonDefs[MONSTER_IMP].number + 1 ));
      u->items.SetNum( I_DEMON,
          getrandom( MonDefs[MONSTER_DEMON].number + 1 ));
      u->items.SetNum( I_BALROG,
          getrandom( MonDefs[MONSTER_BALROG].number + 1 ));
      break;
    case I_SKELETON:
      u->MakeWMon( "Undead", I_SKELETON,
          getrandom( MonDefs[MONSTER_SKELETON].number + 1 ));
      u->items.SetNum( I_UNDEAD,
          getrandom( MonDefs[MONSTER_UNDEAD].number + 1 ));
      u->items.SetNum( I_LICH,
          getrandom( MonDefs[MONSTER_LICH].number + 1 ));
      break;
    case I_MAGICIANS:
      u->MakeWMon(MonDefs[MONSTER_WARRIORS].name, I_WARRIORS,
          (MonDefs[MONSTER_WARRIORS].number +
           getrandom( MonDefs[MONSTER_WARRIORS].number ) + 1) / 2);
      u->MoveUnit( pObj );
      u = GetNewUnit( monfac, 0 );
      u->MakeWMon( "Evil Mages", I_MAGICIANS,
          (MonDefs[MONSTER_MAGICIANS].number +
           getrandom( MonDefs[MONSTER_MAGICIANS].number ) + 1) / 2);
      u->items.SetNum( I_SORCERERS,
          getrandom( MonDefs[MONSTER_SORCERERS].number + 1));
      u->SetFlag(FLAG_BEHIND, 1);
      break;
    case I_DARKMAGE:
      u->MakeWMon( MonDefs[MONSTER_DROW].name, I_DROW,
          (MonDefs[MONSTER_DROW].number +
           getrandom( MonDefs[MONSTER_DROW].number ) + 1) / 2);
      u->MoveUnit( pObj );
      u = GetNewUnit( monfac, 0 );
      u->MakeWMon( "Dark Mages", I_MAGICIANS,
          (MonDefs[MONSTER_MAGICIANS].number +
           getrandom( MonDefs[MONSTER_MAGICIANS].number ) + 1) / 2);
      u->items.SetNum( I_SORCERERS,
          getrandom( MonDefs[MONSTER_SORCERERS].number + 1));
      u->items.SetNum( I_DARKMAGE,
          getrandom( MonDefs[MONSTER_DARKMAGE].number + 1));
      u->SetFlag(FLAG_BEHIND, 1);
      break;
        case I_ILLYRTHID:
      u->MakeWMon( "Undead", I_SKELETON,
          getrandom( MonDefs[MONSTER_SKELETON].number + 1 ));
      u->items.SetNum( I_UNDEAD,
          getrandom( MonDefs[MONSTER_UNDEAD].number + 1 ));
      u->MoveUnit( pObj );
      u = GetNewUnit( monfac, 0 );
      u->MakeWMon( MonDefs[MONSTER_ILLYRTHID].name, I_ILLYRTHID,
          (MonDefs[MONSTER_ILLYRTHID].number +
           getrandom( MonDefs[MONSTER_ILLYRTHID].number ) + 1) / 2);
      u->SetFlag(FLAG_BEHIND, 1);
      break;
        case I_STORMGIANT:
      if (getrandom(3) < 1) {
        mondef = MONSTER_CLOUDGIANT;
        montype = I_CLOUDGIANT;
      }
      u->MakeWMon( MonDefs[mondef].name, montype,
          (MonDefs[mondef].number +
           getrandom( MonDefs[mondef].number ) + 1) / 2);
      break;
    default:
      u->MakeWMon( MonDefs[mondef].name, montype,
          (MonDefs[mondef].number +
           getrandom( MonDefs[mondef].number ) + 1) / 2);
      break;
  }
  u->MoveUnit( pObj );
}

void Game::CreateCityMons()
{
    if( !Globals->CITY_MONSTERS_EXIST )
    {
        return;
    }

    forlist(&regions) {
        ARegion * r = (ARegion *) elem;
        if (r->type == R_NEXUS || r->IsStartingCity() || r->town)
        {
            CreateCityMon( r, 100 );
        }
    }
}

void Game::CreateCityMon( ARegion *r, int percent )
{
    r->PickCityMon();

    for (int i = 0; i < 7; i++)
    {
       if ( CityGuardDefs[r->guardtype].index[i] == -1 ) continue;
       int tf = 0;
       for (int t = 0; t < 7; t++)
          if (TerrainDefs[r->type].similar_type == NPCDefs[CityGuardDefs[r->guardtype].index[i]].terrainType[t])
             tf++;
       if (tf || ( NPCDefs[CityGuardDefs[r->guardtype].index[i]].flags & NPCPattern::NPC_ALL_TERRAIN) )
          CreateCityNPC( r, percent, CityGuardDefs[r->guardtype].index[i] );
    }
}

void ARegion::PickCityMon ()
{
    int a = 0; bool st = (type == R_NEXUS) || IsStartingCity();
    for (int j = 0; j < NUM_CITYGUARD; j++)
    {
       if ( CityGuardDefs[j].flags & CityGuardPattern::PT_DISABLED ) continue;
          if (st && (!(CityGuardDefs[j].flags & CityGuardPattern::PT_STARTCITY)))
             continue; // if the region is a starting city or a nexus, and the
                       // pattern is not suited for startcities skip it
          if (!st && (CityGuardDefs[j].flags & CityGuardPattern::PT_STARTCITY))
             continue; // if the region is not a starting city and is not a nexus,
                       // and the pattern is suited for startcities skip it
       for (int l = 0; l < 5; l++)
          if (CityGuardDefs[j].raceType[l]==race ||
             (CityGuardDefs[j].flags&CityGuardPattern::PT_ALLRACES) )
             a++;
    }

    if (a) {
       // if any patterns are found suitable fo this region select one of them
       int b = getrandom (a);

       for (int j = 1; j < NUM_CITYGUARD; j++)
       {
          if(CityGuardDefs[j].flags & CityGuardPattern::PT_DISABLED)
             continue; // if its disabled skip it
          if (st && (!(CityGuardDefs[j].flags & CityGuardPattern::PT_STARTCITY)))
             continue; // if the region is a starting city or a nexus, and the
                       // pattern is not suited for startcities skip it
          if (!st && (CityGuardDefs[j].flags & CityGuardPattern::PT_STARTCITY))
             continue; // if the region is not a starting city and is not a nexus,
                       // and the pattern is suited for startcities skip it
          for(int l = 0; l < 5; l++) {
             if (CityGuardDefs[j].raceType[l] == race ||
                (CityGuardDefs[j].flags & CityGuardPattern::PT_ALLRACES)) {
                if(b == 0)
                   guardtype = j;
                b--;
             }
          }
       }
    }
    else guardtype = CG_STANDART; // otherwise use default pattern
}


void Game::CreateCityNPC( ARegion *pReg, int percent, int NPCPatt )
{
        int townsize; int num; int lev; int item; int sk; int addpercent;
        townsize = pReg->GetGuardLevel();

        if( pReg->type == R_NEXUS || pReg->IsStartingCity() )
            addpercent = 100;
        else
            if (pReg->town)
                addpercent = (int) ((pReg->town->pop + pReg->population / 2 + 675) * 100 / 4675);
            else /* The region isn`t the Nexus, isn`t a city, what the hell is it? */
                addpercent=100;

        if (NPCDefs[NPCPatt].flags & NPCPattern::NPC_FLAG_ISMAGE ||
                NPCDefs[NPCPatt].flags & NPCPattern::NPC_CONSTANTMEN ) {
                addpercent = 100; percent = 100; }

        int eqpercent = percent - 50;
        if (eqpercent < 0) eqpercent = 0;
        eqpercent *= 2;

        if ((NPCDefs[NPCPatt].flags & NPCPattern::NPC_NATIVE_GUARDS) && pReg->race != -1)
                item = pReg->race;
        else item = NPCDefs[NPCPatt].men.type;

        num = NPCDefs[NPCPatt].men.num[townsize];

        if (!num) return;

        Faction *pFac = GetFaction( &factions, guardfaction );
        Unit *u = GetNewUnit( pFac );

        u->SetMen(item, (int) num * percent * addpercent / 10000);

        u->MoveUnit( pReg->GetDummy() );

        if (NPCDefs[NPCPatt].flags & NPCPattern::NPC_FLAG_ISMAGE)
           u->type = U_GUARDMAGE;
        else
           u->type = U_GUARD;
        u->npctype = NPCPatt;

        if (NPCDefs[NPCPatt].flags & NPCPattern::NPC_FLAG_GUARD)
            u->guard = GUARD_GUARD;
        if (NPCDefs[NPCPatt].flags & NPCPattern::NPC_FLAG_HOLD)
            u->SetFlag(FLAG_HOLDING,1);
        if (NPCDefs[NPCPatt].flags & NPCPattern::NPC_FLAG_BEHIND)
            u->SetFlag(FLAG_BEHIND,1);
        u->reveal=REVEAL_FACTION;

        AString *temp = new AString("");
        if(num == 1 && (( NPCDefs[NPCPatt].flags & NPCPattern::NPC_CONSTANTMEN )
                || (NPCDefs[NPCPatt].flags & NPCPattern::NPC_FLAG_ISMAGE)) )
        // if this is a single-man unit, and its a mage or some other special unit,
        // assume that he's a hero, and get a proper name for him
        * temp += GetHeroName(u->num * u->num * (pReg->xloc + 1),
                        NPCDefs[u->npctype].name, ItemDefs[item].index);
        else { // otherwise, it's just a ``simple'' unit
                if(pReg->town)
                        * temp += * (pReg->town->name);
                else
                        if(pReg->type==R_NEXUS)
                                * temp += "Nexus";
                        else
                                * temp += * (pReg->name);
                * temp += NPCDefs[NPCPatt].name;
        }

        u->SetName(temp);

        u->combat = NPCDefs[NPCPatt].combatSpell[townsize];
        u->SetMoney((int) NPCDefs[NPCPatt].silverAmount[townsize] * num
                                        * percent * addpercent / 10000 );

        for (int it=0; it<10; it++) {
                num = NPCDefs[NPCPatt].equipment[it].num[townsize];
                if (!num) continue;
                item = NPCDefs[NPCPatt].equipment[it].type;
                if (ItemDefs[item].type & IT_MONSTER)
                   u->items.SetNum( item, (int) num * percent * addpercent / 10000);
                else
                   u->items.SetNum( item, (int) num * eqpercent * addpercent / 10000);
        }

        for (int it=0; it<10; it++) {
                lev = NPCDefs[NPCPatt].skills[it].level[townsize];
                if (!lev) continue;
                sk = NPCDefs[NPCPatt].skills[it].type;
                u->SetSkill(sk, lev);
        }
}

void Game::AdjustCityMons( ARegion *r )
{
	int hasnpcguard = 0, setguard = 0, wguard[7];
	for (int i = 0; i < 7; i++)
	{
		wguard [i] = -1;
		int tf = 0;
		if(CityGuardDefs[r->guardtype].index[i] != -1)
		{
			for (int t = 0; t < 7; t++)
				if (TerrainDefs[r->type].similar_type == NPCDefs[CityGuardDefs[r->guardtype].index[i]].terrainType[t])
					tf++;
			if (tf || NPCDefs[CityGuardDefs[r->guardtype].index[i]].flags & NPCPattern::NPC_ALL_TERRAIN)
				wguard[i] = CityGuardDefs[r->guardtype].index[i];
		}
	}

	forlist(&r->objects)
	{
		Object * o = (Object *) elem;
			forlist(&o->units)
			{
				Unit * u = (Unit *) elem;
				if (u->npctype != -1)
				{
					AdjustCityMon( r, u );
					if (NPCDefs[u->npctype].behavior == NPCPattern::NPC_BHV_GUARDSMEN)
					// this is city Guard
					hasnpcguard = 1;
					for (int i = 0; i <7; i++)
						if (wguard[i] == u->npctype) wguard[i] = -1;
				}
				if (u->guard == GUARD_GUARD) setguard = 1;
			}
		}

		if (hasnpcguard)
		{ // there are some guards in the region
			for (int i = 0; i < 7; i++)
			{
				if (wguard[i] != -1) CreateCityNPC( r, Globals->GUARD_REGEN, wguard[i] );
			}
		}
		else
		{
			if  ( getrandom(100) < Globals->GUARD_REGEN && setguard == 0)
			{
				// create City Guards if no unit has guard flag exposed and the
				// roll is success
				CreateCityMon( r, Globals->GUARD_REGEN );
			}
			// otherwise do nothing
		}
}


void Game::AdjustCityMon( ARegion *pReg, Unit *u )
{
        int townsize, addpercent, item, num, percent, sk, lev;
        townsize = pReg->GetGuardLevel();

        if( pReg->type == R_NEXUS || pReg->IsStartingCity() )
            addpercent = 100;
        else
            if (pReg->town)
                addpercent = (int) ((pReg->town->pop + pReg->population / 2 + 675) * 100 / 4675);
            else addpercent = 100;

        if ((NPCDefs[u->npctype].flags & NPCPattern::NPC_NATIVE_GUARDS) && pReg->race != -1)
                item = pReg->race;
        else item = NPCDefs[u->npctype].men.type;
        num = NPCDefs[u->npctype].men.num[townsize];

        if (!num) return;

        if (NPCDefs[u->npctype].flags & NPCPattern::NPC_FLAG_ISMAGE ||
                    NPCDefs[u->npctype].flags & NPCPattern::NPC_CONSTANTMEN ) {
            percent = 100;
            addpercent = 100;
            }
        else percent = u->GetMen() * 10000 /
            (NPCDefs[u->npctype].men.num[townsize] * addpercent ) + 10;

        if (percent > 100) percent = 100;

        // equipment will appear only over 50% guards percentage. This is meant to
        // prevent collecting 'free' spoils from city guards on late turns.
        int eqpercent = percent - 50;
        if (eqpercent < 0) eqpercent = 0;
        eqpercent *= 2;

        u->SetMen(item, (int) num * percent * addpercent / 10000);

// TODO: NPC behavior related to building occupation

        if (NPCDefs[u->npctype].flags & NPCPattern::NPC_FLAG_GUARD)
            u->guard = GUARD_GUARD;
        if (NPCDefs[u->npctype].flags & NPCPattern::NPC_FLAG_HOLD)
            u->SetFlag(FLAG_HOLDING, 1);
        if (NPCDefs[u->npctype].flags & NPCPattern::NPC_FLAG_BEHIND)
            u->SetFlag(FLAG_BEHIND, 1);
        u->reveal=REVEAL_FACTION;

        AString *temp = new AString("");

        if(num == 1 && ((NPCDefs[u->npctype].flags & NPCPattern::NPC_CONSTANTMEN)
           || (NPCDefs[u->npctype].flags & NPCPattern::NPC_FLAG_ISMAGE)) ) {
        // if this is a single-man unit, and this is not because of casualities,
        // assume that he's a hero, and get a proper name for him

        // FIX: no action required, as Hero already got a name
        //   * temp += GetHeroName(u->num * u->num * (pReg->xloc + 1),
        //                NPCDefs[u->npctype].name, ItemDefs[item].index);
        // using current name generator here will randomize hero's gender each
        // turn, hehe ;-)
        }
        else { // otherwise, it's just a ``simple'' unit
           if(pReg->town)
              * temp += * (pReg->town->name);
           else
              if(pReg->type==R_NEXUS)
                 * temp += "Nexus";
              else
                 * temp += * (pReg->name);
           * temp += NPCDefs[u->npctype].name;
        }

        u->SetName(temp);
        u->combat = NPCDefs[u->npctype].combatSpell[townsize];
        u->SetMoney((int) NPCDefs[u->npctype].silverAmount[townsize]
                                                * num * percent * addpercent / 10000 );

        for (int it=0; it<10; it++) {
                item = NPCDefs[u->npctype].equipment[it].type;
                num = NPCDefs[u->npctype].equipment[it].num[townsize];
                if (num == -1) continue; // this means we shoud leave this item

                // just preventing the decrement of ammunition if guards have had
                // any casualities.
                if (u->items.GetNum(item) > num) continue;

                if (ItemDefs[item].type & IT_MONSTER)
                   u->items.SetNum( item, (int) num * percent * addpercent / 10000);
                else
                   u->items.SetNum( item, (int) num * eqpercent * addpercent / 10000);

                if (ItemDefs[item].type & IT_MONSTER)
                {
                   int mnum = (int) num * percent * addpercent / 10000;
                   if (u->items.GetNum(item) < mnum) u->items.SetNum(item, mnum);
                   else if (u->items.GetNum(item) > num) u->items.SetNum(item, num);
                }
                else
                {
                   int eqnum = (int) num * eqpercent * addpercent / 10000;
                   // just preventing the decrement of ammunition if guards have had
                   // any casualities.
                   if (u->items.GetNum(item) < eqnum) u->items.SetNum(item, eqnum);
                   // cutting off excessive equipment
                   else if (u->items.GetNum(item) > num) u->items.SetNum(item, num);
                }
        }

        for (int it=0; it<10; it++) {
                lev = NPCDefs[u->npctype].skills[it].level[townsize];
                if (!lev) continue;
                sk = NPCDefs[u->npctype].skills[it].type;
                u->SetSkill(sk, lev);
        }
}


int ARegion::GetGuardLevel()
{
        if( type == R_NEXUS )
            return TOWN_CITY + 1;
        if (Globals->GUARDS_LEVEL_BY_TAX) {
             if (money < 8000) return TOWN_VILLAGE;
             if (money < 14000) return TOWN_TOWN;
             if (money < 21000) return TOWN_CITY;
             return TOWN_CITY + 1;
        }
        else {
            if (town)
                return town->TownType();
            return 0;
        }
}


