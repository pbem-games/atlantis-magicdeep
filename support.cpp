//---------------------------------------------------------------------------

#pragma hdrstop

#include "support.h"
#include "CFractalMap.h"
#include "NameGen.h"
#include "npc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

bool GetChance(int iSeed)
{
	if(iSeed >= 0)
	{
		return getrandom(iSeed) == 1;
	}
	else
	{
		return getrandom(-iSeed) != 1;
	}
}

int RaceToItem(int race)
{
	for(int i=0; i<NITEMS; i++)
	{
		if(ItemDefs[i].type == IT_MAN && ItemDefs[i].index == race)
		{
			return i;
		}
	}
	return -1;
}

void ConvertWorld(ARegionList *regions, ARMGenerator* pARM, int level)
{
	int x,y;
	for(y = 0; y < pARM->HEIGHT; y++)
	{
		for(x = 0; x < pARM->WIDTH; x++)
		{
			if(!((x + y) % 2))
			{
				ARegion *reg = regions->pRegionArrays[level]->GetRegion(x,y);
				//
				// Some initial values; these will get reset
				//
				reg->type = pARM->GetStandartRType(pARM->m_pMap[x][y].type, Globals->ADVANCED_MAP);
//				reg->race = -1;
//				reg->wages = -1;
//				reg->maxwages = -1;
//				reg->SetName(pARM->GetElementName(x,y).c_str());

				regions->pRegionArrays[level]->SetRegion(x, y, reg);

				int i;
/*				int aPop[6];

				for(i=0; i<3; i++)
				{
					aPop[i*2] = pARM->SortedRacesByPop(x,y,i);
					aPop[i*2+1] = pARM->m_pMap[x][y].aPop[aPop[i*2]];
				}

				reg->ARMSetup(aPop);
*/
				reg->ARMSetup(pARM->SortedRacesByPop(x,y,0), pARM->CountLand(x,y));
//				reg->Setup();

				for(i=0; i<NUMMAN; i++)
				{
					if(ItemDefs[(reg->race)].index == i)
					{
						pARM->m_pMap[x][y].aPop[i] = reg->Population();
					}
					else
					{
						pARM->m_pMap[x][y].aPop[i] = 0;
					}
				}
			}
		}
	}

	for(y = 0; y < pARM->HEIGHT; y++)
	{
		for(x = 0; x < pARM->WIDTH; x++)
		{
			if(!((x + y) % 2))
			{
				ARegion *reg = regions->pRegionArrays[level]->GetRegion(x,y);

				reg->SetName(pARM->GetElementName(x,y).Str());
			}
		}
	}
}
void ARMAddFortress(ARegion *reg, int ft)
{
	Object * obj;

	obj = new Object(reg);
	obj->num = reg->buildingseq++;
	obj->type = ft;

	AString s;
	if(getrandom(3)==1 && reg->town)
	{
		s = (AString)reg->town->name->Str() + "'s " + ObjectDefs[ft].name;
	}
	else
	{
		s = GetObjectName(getrandom(32000), ft).Str();
	}
	AString *str;
	str = new AString(s.Str());
	obj->SetName(str);
	if(GetChance(Globals->ARM_FORTRESS_RUINED_CHANCE))
	{
		obj->incomplete = 0;
	}
	else
	{
		obj->incomplete = 1+getrandom(ObjectDefs[ft].cost*0.5);
	}

	reg->objects.Add(obj);
}

void Game::ARMAddAncientBuildings(int level)
{
	int x,y;
	for(y = 0; y < regions.pRegionArrays[level]->y; y++)
	{
		for(x = 0; x < regions.pRegionArrays[level]->x; x++)
		{
			if(!((x + y) % 2))
			{
				ARegion *reg = regions.pRegionArrays[level]->GetRegion(x,y);

				if(reg->town || (reg->type != R_OCEAN && getrandom(128)==1))
				{
//Firstly, add fortresses
					if(reg->Population() > 4000 && GetChance(Globals->ARM_FORTRESS_PROBABILITY))
					{
						ARMAddFortress(reg, O_CASTLE);
					}

					if(reg->Population() > 3000 && GetChance(Globals->ARM_FORTRESS_PROBABILITY))
					{
						ARMAddFortress(reg, O_KEEP);
					}

					if(reg->Population() > 2000 && GetChance(Globals->ARM_FORTRESS_PROBABILITY))
					{
						ARMAddFortress(reg, O_FORT);
					}

					if(reg->Population() > 1000 && GetChance(Globals->ARM_FORTRESS_PROBABILITY))
					{
						ARMAddFortress(reg, O_TOWER);
					}

					if(reg->Population() > 500 && GetChance(Globals->ARM_FORTRESS_PROBABILITY))
					{
						ARMAddFortress(reg, O_TOWER);
					}

//Than, add inns...
					int money;
					money = reg->Population() * (reg->Wages() - Globals->MAINTENANCE_COST);
					money = money / Globals->ENTERTAIN_FRACTION;

					Object * obj;
					while(money>0)
					{
						if(GetChance(Globals->ARM_INN_PROBABILITY))
						{
							obj = new Object(reg);
							obj->num = reg->buildingseq++;
							obj->type = O_INN;
							AString *str;
							str = new AString(GetObjectName(getrandom(32000), O_INN));
							obj->SetName(str);
							if(GetChance(Globals->ARM_INN_RUINED_CHANCE))
							{
								obj->incomplete = 0;
							}
							else
							{
								obj->incomplete = 1+getrandom(ObjectDefs[O_INN].cost*0.75);
							}

							reg->objects.Add(obj);
/*
				Faction *folkfac = GetFaction( &factions, 1 );
				Unit *u;
				u = GetNewUnit( folkfac );

				s = new AString("Inn Keeper");
				u->SetName(s);

				u->type = U_NORMAL;
				u->SetMen(race,1);

				u->SetMoney(100 + getrandom(100));

				u->SetSkill(S_ENTERTAINMENT,2 + getrandom(3));
				u->SetFlag(FLAG_BEHIND, 1);
				u->SetFlag(FLAG_HOLDING, 1);

				u->MoveUnit(obj);

				u = GetNewUnit( folkfac );

				s = new AString("servants");
				u->SetName(s);

				u->type = U_NORMAL;
				int iManCount = 5+getrandom(5);
				u->SetMen(race,iManCount);

				u->SetMoney((10 + getrandom(10))*iManCount);

				u->SetSkill(S_ENTERTAINMENT,1 + getrandom(3));
				u->SetFlag(FLAG_BEHIND, 1);
				u->SetFlag(FLAG_HOLDING, 1);

				u->MoveUnit(obj);
*/
						}
						money -= 500;
					}

//Finally add roads...
					ARegion *nearestCity;
					int num = 0;
					do					{						nearestCity = regions.ARMGetNearestSettlement(reg, num++);						regions.ARMBuildAncientRoad(reg, nearestCity);					}					while(nearestCity != NULL && getrandom(3)!=1);				}//And ships...
				if(reg->IsCoastal())
				{
					Object * obj;

					int i;
					for(i=0; i<6; i++)
					{
						if(reg->neighbors[i])
						{
							if(reg->neighbors[i]->type == R_OCEAN && getrandom(96)==1)
							{
								obj = new Object(reg);
								obj->num = shipseq++;
								int ft = getrandom(30);

								obj->type = O_LONGBOAT;
								if(ft > 23)
								{
									obj->type = O_CLIPPER;
								}
								if(ft > 27)
								{
									obj->type = O_GALLEON;
								}

								AString s;
								s = GetObjectName(getrandom(32000), obj->type).Str();

								AString *str;
								str = new AString(s.Str());
								obj->SetName(str);

								obj->incomplete = ObjectDefs[obj->type].cost*0.5 + getrandom(ObjectDefs[obj->type].cost*0.5);

								reg->objects.Add(obj);
							}
						}
					}
				}
			}
		}
	}

//Roads must be massively destroed...
	for(y = 0; y < regions.pRegionArrays[level]->y; y++)
	{
		for(x = 0; x < regions.pRegionArrays[level]->x; x++)
		{
			if(!((x + y) % 2))
			{
				ARegion *reg = regions.pRegionArrays[level]->GetRegion(x,y);

				forlist (&(reg->objects))
				{
					Object * o = (Object *) elem;
					if(o->IsRoad() && GetChance(-Globals->ARM_ROAD_PROBABILITY))
//					if(o->IsRoad())
					{
//						o->incomplete = o->incomplete + ObjectDefs[o->type].cost/Globals->ARM_ROAD_PROBABILITY;
						o->incomplete = ObjectDefs[o->type].cost;
					}
				}
			}
		}
	}
}

bool ARegionList::ImportWorld(int level, int xSize, int ySize, char* name)
{
	Ainfile f;
	if( f.OpenByName( "map.csv" ) == -1 )
	{
		Awrite("ERROR: File `map.csv` not found in working directory...");
		return false;
	}

	Awrite("Reading file...");
	AString *s1 = f.GetStr();
	if(!s1)
	{
		Awrite("ERROR: File `map.csv` is empty...");
		return false;
	}
	if(!(*s1 == "Atlantis Map Generator Export File"))
	{
		Awrite("ERROR: File `map.csv` wrong format...");
		delete s1;
		return false;
	}
	delete s1;

	f.GetStr();
	s1 = f.GetStr();
	if(!s1)
	{
		Awrite("ERROR: File `map.csv` wrong format...");
		return false;
	}
	s1->gettoken();
	s1->gettoken();
	AString *s2 = s1->gettoken();
	int importXSize, importYSize;

	importXSize = s2->value();
	delete s2;
	delete s1;

	s1 = f.GetStr();
	if(!s1)
	{
		Awrite("ERROR: File `map.csv` wrong format...");
		return false;
	}
	s1->gettoken();
	s1->gettoken();
	s2 = s1->gettoken();

	importYSize = s2->value();
	delete s2;
	delete s1;

	if(importXSize != xSize || importYSize != ySize)
	{
		Awrite("WARNING: World size mismatch... Imported sizes will be used.");
		xSize = importXSize;
		ySize = importYSize;
	}

	MakeRegions(level, xSize, ySize);

	pRegionArrays[level]->SetName(name);
	pRegionArrays[level]->levelType = ARegionArray::LEVEL_SURFACE;

	Awrite("Import a level...");
	bool ready=false;
	do
	{
		s1 = f.GetStr();
		if(!s1)
		{
			Awrite("ERROR: File `map.csv` wrong format...");
			return false;
		}

		if(*s1 == "#Regions:")
		{
			f.GetStr();
			ready = true;
		}
		delete s1;
	}
	while(!ready);

	bool eof=false;
	do
	{
		s1 = f.GetStr();
		if(!s1)
		{
			Awrite("ERROR: File `map.csv` wrong format...");
			return false;
		}
		if(*s1 == "EOF.")
		{
			eof = true;
		}
		else
		{
			int x, y, type, race, rareResource, provinceID, provinceSize, contiID, contiSize, island, temperature, humidity, watersCount;
			AString *regName;
			s2 = s1->gettoken();
			x = s2->value();
			delete s2;
			s2 = s1->gettoken();
			y = s2->value();
			delete s2;
			s2 = s1->gettoken();
			type = R_OCEAN;
			for(int idType=0; idType<R_NUM; idType++)
			{
				if(*s2 == TerrainDefs[idType].name)
					type = idType;
			}
//			type = s2->value();
			delete s2;
			s2 = s1->gettoken();
			race = MAN_MERC;
			for(int idRace=0; idRace<NUMMAN; idRace++)
			{
				if(*s2 == ItemDefs[RaceToItem(idRace)].names)
					race = idRace;
			}
//			race = s2->value();
			delete s2;
			s2 = s1->gettoken();
			rareResource = s2->value();
			delete s2;
			s2 = s1->gettoken();
			provinceID = s2->value();
			delete s2;
			s2 = s1->gettoken();
			provinceSize = s2->value();
			delete s2;
			s2 = s1->gettoken();
			contiID = s2->value();
			delete s2;
			s2 = s1->gettoken();
			contiSize = s2->value();
			delete s2;
			s2 = s1->gettoken();
			island = s2->value();
			delete s2;
			s2 = s1->gettoken();
			temperature = s2->value();
			delete s2;
			s2 = s1->gettoken();
			humidity = s2->value();
			delete s2;
			s2 = s1->gettoken();
			watersCount = s2->value();
			delete s2;

			regName = s1->StripWhite();


			ARegion *reg = pRegionArrays[level]->GetRegion(x,y);
			//
			// Some initial values;
			//
			reg->type = type;
			reg->SetName(regName->Str());

			pRegionArrays[level]->SetRegion(x, y, reg);

			reg->ARMSetup(race, contiSize);
		}
		delete s1;
	}
	while(!eof);

	Awrite("");
	return true;
}

void ARegionList::CreateElementalWorld(int level, int xSize, int ySize, char* name)
{
	MakeRegions(level, xSize, ySize);

	pRegionArrays[level]->SetName(name);
	pRegionArrays[level]->levelType = ARegionArray::LEVEL_SURFACE;

	ARMGenerator* pARM = new ARMGenerator(xSize, ySize);

	pARM->Clear();

	float ocean = 0.0;
	while (ocean <= 0)
	{
		Awrite("Which part of world would be covered by oceans (by percents)? ");
		ocean = Agetint();
		if( ocean <= 0 || ocean >= 100 )
		{
			ocean = 0;
			Awrite( "Wrong value. Must be in range 1-99..." );
		}
	}
	float mountains = 0.0;
	while (mountains <= 0)
	{
		Awrite("Which part of world would be covered by mountains (by percents)? ");
		mountains = Agetint();
		if( mountains <= 0 || mountains >= 100-ocean )
		{
			mountains = 0;
			Awrite( (AString)"Wrong value. Must be in range 1-" + AString((int)(100-ocean)) + "..." );
		}
	}
	int contiCount = 0;
	while (contiCount <= 0)
	{
		int maxContiCount;
		maxContiCount = xSize*ySize*0.5*(100-ocean)*0.01*0.5;
		Awrite((AString)"How many continents (1-" + AString((int)maxContiCount) + ", recommended 5)? ");
//		Awrite("How many continents (1-30, recommended 5)? ");
		contiCount = Agetint();
		if( contiCount <= 0 || contiCount > maxContiCount )
		{
			contiCount = 0;
			Awrite( (AString)"Wrong value. Must be in range 1-" + AString((int)maxContiCount) + "..." );
		}
	}
	int temperature = 0;
	while (temperature <= 0)
	{
		Awrite("How warm the world would be (1-10, recommended 5)? ");
		temperature = Agetint();
		if( temperature <= 0 || temperature > 10 )
		{
			temperature = 0;
			Awrite( "Wrong value. Must be in range 1-10..." );
		}
	}
	int humi = 0;
	while (humi <= 0)
	{
		Awrite("How wet the world would be (1-10, recommended 5)? ");
		humi = Agetint();
		if( humi <= 0 || humi > 10 )
		{
			humi = 0;
			Awrite( "Wrong value. Must be in range 1-10..." );
		}
	}
	int age = 0;
	while (age <= 0)
	{
		Awrite("How old the world would be (1-5, strongly recommended 5)? ");
		age = Agetint();
		if( age <= 0 || age > 5 )
		{
			age = 0;
			Awrite( "Wrong value. Must be in range 1-5..." );
		}
	}

	Awrite("Making a level...");
	pARM->GenerateFractal(contiCount, age, temperature, humi, ocean/100.0, mountains/100.0, -1);//continents count, age, temperature, humidity, water portion, mountains portion

	pARM->InitPop();
/*
	int i;
	for(i=0; i<100; i++)
	{
		pARM->DynamicDemographyOneStep();
	}
*/
	Awrite("");
	Awrite("Making land");
	ConvertWorld(this, pARM, level);
	Awrite("");

	delete pARM;
}

void ARegion::ARMSetup(int iRace, int iLandSize)
{
	//
	// type and location have been setup, do everything else
// S <= 16 К=3, 36 >= S > 16 К=2, на остальной территории оставить по прежнему. S - площадь, К - коэф. частоты.
	if(iLandSize > 36)
	{
		ARMSetupProds(1);
	}
	if(iLandSize <= 36 && iLandSize > 16)
	{
		ARMSetupProds(2);
	}
	if(iLandSize <= 16)
	{
		ARMSetupProds(3);
	}

	ARMSetupPop(iRace);
//	SetupPop();

	//
	// Make the dummy object
	//
	Object * obj = new Object(this);
	objects.Add(obj);


	if(Globals->LAIR_MONSTERS_EXIST)
		LairCheck();
}

void ARegion::ARMSetupPop(int iRace)
{
	TerrainType * typer = &(TerrainDefs[type]);

	int pop = typer->pop;
	int mw = typer->wages;

	if (pop == 0) {
		population = 0;
		basepopulation = 0;
		wages = 0;
		maxwages = 0;
		money = 0;
		return;
	}

	race = RaceToItem(iRace);

	if(Globals->RANDOM_ECONOMY) {
		population = (pop + getrandom(pop)) / 2;
	} else {
		population = pop;
	}

	basepopulation = population;

	if(Globals->RANDOM_ECONOMY) {
		mw += getrandom(3);
	}

	/* Setup wages */
	wages = mw;
	maxwages = mw;

	if(Globals->TOWNS_EXIST) {
		int adjacent = 0;
		int prob = Globals->TOWN_PROBABILITY;
		if (prob < 1) prob = 100;
		int townch = (int) 80000 / prob;
		if (Globals->TOWNS_NOT_ADJACENT) {
			for (int d = 0; d < NDIRS; d++) {
				ARegion *newregion = neighbors[d];
				if ((newregion) &&  (newregion->town)) adjacent++;
			}
		}
		if(Globals->LESS_ARCTIC_TOWNS) {
			int dnorth = GetPoleDistance(D_NORTH);
			int dsouth = GetPoleDistance(D_SOUTH);
			if (dnorth < 9)
				townch = townch + 25 * (9 - dnorth) *
					(9 - dnorth) * Globals->LESS_ARCTIC_TOWNS;
			if (dsouth < 9)
				townch = townch + 25 * (9 - dsouth) *
					(9 - dsouth) * Globals->LESS_ARCTIC_TOWNS;
		}
		int townprob = TerrainDefs[type].economy * 4 /
			(Globals->TOWN_SPREAD+1) + 50 * Globals->TOWN_SPREAD;
		if (adjacent == 0)
			if (getrandom(townch) < townprob) AddTown();
	}

	Production * p = new Production;
	p->itemtype = I_SILVER;
	money = Population() * (Wages() - Globals->MAINTENANCE_COST);
	p->amount = money / Globals->WORK_FRACTION;
	p->skill = -1;
	p->level = 1;
	p->productivity = Wages();
	products.Add(p);

	//
	// Setup entertainment
	//
	p = new Production;
	p->itemtype = I_SILVER;
	p->amount = money / Globals->ENTERTAIN_FRACTION;
	p->skill = S_ENTERTAINMENT;
	p->level = 1;
	p->productivity = Globals->ENTERTAIN_INCOME;
	products.Add(p);

	SetupRecruts();
}

void ARegion::ARMAddTown(int *aPop)
{
	town = new TownInfo;

	// Klaus
	town->name = new AString(GetEthnicName(ItemDefs[race].index, getrandom(32000)));

	if(Globals->RANDOM_ECONOMY) {
		int popch = 2500;
		if(Globals->LESS_ARCTIC_TOWNS) {
			int dnorth = GetPoleDistance(D_NORTH);
			int dsouth = GetPoleDistance(D_SOUTH);
			int dist = dnorth;

			/*
			 * On small worlds, or the underworld levels, both distances
			 * could be less than 9, so choose the smallest.
			 */
			if (dsouth < dist) dist = dsouth;

			if (dist < 9)
				popch = popch - (9 - dist) * ((9 - dist) + 10) * 15;
		}
		town->pop = 500+getrandom(popch);
	} else {
		town->pop = 500;
	}

	town->basepop = town->pop;
	town->activity = 0;

/*
	int totalPop=0;
	int i;
	for(i=0; i<3; i++)
	{
		totalPop += aPop[i*2+1];
	}
	for(i=1; i<3; i++)
	{
		town->aBonusRace[i-1].iRace = RaceToItem(aPop[i*2]);
		town->aBonusRace[i-1].iPop = town->pop*aPop[i*2+1]/totalPop;
	}
*/
	SetupCityMarket();
}


void ARegionList::ARMBuildAncientRoad(ARegion *start, ARegion *finish, ARegion *current)
{
	if(!start)
		return;

	if(!finish)
		return;

	ARegion *next;
	if(current == NULL)
	{
		current = start;
	}

	int d0;
	d0 = GetDistance(current, finish);

	int i;
	for(i=0; i<NDIRS; i++)
	{
		next = current->neighbors[i];
		if(next)
		{
			int d;
			d = GetDistance(next,finish);

			if((d<d0 || (d==d0 && getrandom(5)==1 && next->type != R_MOUNTAIN && next->type != R_VOLCANO)) && TerrainDefs[next->type].similar_type != R_OCEAN && next->type != R_LAKE)
			{
				Object *obj;

				bool alreadyBuilded = false;
				{
				forlist (&(current->objects))
				{
					Object * o = (Object *) elem;
					if (o->IsRoad())
					{
						if (o->type == current->GetRoadDirection(i))
						{
							alreadyBuilded = true;
						}

						if(finish->town)
						{
/*							AString ss;
							ss = o->name->Str();
							ss = ss.SubString(0,ss.Pos("[")-2);
							if(ss == (AString)"Ancient Road to " + finish->town->name->Str())
							{
								return;
							}
*/
							if(o->name->CheckPrefix((AString)"Ancient Road to " + finish->town->name->Str()))
							{
								return;
							}
						}
					}
				}
				}

//				if(GetChance(Globals->ARM_ROAD_PROBABILITY) && !alreadyBuilded)
				if(!alreadyBuilded)
				{
					obj = new Object(start);
					obj->num = current->buildingseq++;
					obj->type = current->GetRoadDirection(i);
					AString *str;
					if(finish->town)
					{
						str = new AString(((AString)"Ancient Road to " + finish->town->name->Str()));
					}
					else
					{
						str = new AString("Ancient Road to unknown...");
					}
					obj->SetName(str);
					if(GetChance(Globals->ARM_ROAD_RUINED_CHANCE))
					{
						obj->incomplete = 0;
					}
					else
					{
//						obj->incomplete = 1+getrandom(ObjectDefs[obj->type].cost*0.5);
						obj->incomplete = 1+getrandom(ObjectDefs[obj->type].cost-1);
					}

					current->objects.Add(obj);
				}

				alreadyBuilded = false;
				{
				forlist (&(next->objects))
				{
					Object * o = (Object *) elem;
					if (o->IsRoad())
					{
						if (o->type == next->GetRoadDirection(next->GetRealDirComp(i)))
						{
							alreadyBuilded = true;
						}
						if(start->town)
						{
/*							AString ss;
							ss = o->name->Str();
							ss = ss.SubString(0,ss.Pos("[")-2);
							if(ss == (AString)"Ancient Road to " + start->town->name->Str())
							{
								return;
							}
*/
							if(o->name->CheckPrefix((AString)"Ancient Road to " + start->town->name->Str()))
							{
								return;
							}
						}
					}
				}
				}

//				if(GetChance(Globals->ARM_ROAD_PROBABILITY) && !alreadyBuilded)
				if(!alreadyBuilded)
				{
					obj = new Object(next);
					obj->num = next->buildingseq++;
					obj->type = next->GetRoadDirection(next->GetRealDirComp(i));
					AString *str;
					if(start->town)
					{
						str = new AString((AString)"Ancient Road to " + start->town->name->Str());
					}
					else
					{
						str = new AString("Ancient Road to unknown...");
					}
					obj->SetName(str);
					if(GetChance(Globals->ARM_ROAD_RUINED_CHANCE))
					{
						obj->incomplete = 0;
					}
					else
					{
//						obj->incomplete = 1+getrandom(ObjectDefs[obj->type].cost*0.5);
						obj->incomplete = 1+getrandom(ObjectDefs[obj->type].cost-1);
					}

					next->objects.Add(obj);
				}
/*
				ARegion *third;
				third = ARMGetNearestSettlement(current);

				if(third != NULL && third != start && third != finish)
				{
					if(GetDistance(start, current) < GetDistance(finish, current))
					{
						ARMBuildAncientRoad(start, third, current);
					}
					else
					{
						ARMBuildAncientRoad(finish, third, current);
					}
				}
*/
				ARMBuildAncientRoad(start, finish, next);

				return;
			}
		}
	}
}

ARegion * ARegionList::ARMGetNearestSettlement(ARegion * reg, int num)
{
	if(!reg)
		return NULL;

	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	ARegion **oldList;
	ARegion **newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = (ARegion **)calloc(iCurrentHead, sizeof(ARegion *));
	newList[0] = reg;

	ARegion *next;
	int r=1;

	int iRIndex = 1;

	int n = 0;

	do
	{
		if(oldList)
		{
			free(oldList);
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = (ARegion **)calloc(iCurrentHead, sizeof(ARegion *));

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<NDIRS; i++)
			{
				next = oldList[k]->neighbors[i];
				if(next)
				{
					int d,d0;
					d0 = GetDistance(reg,oldList[k]);
					d = GetDistance(reg,next);
					if(d>d0 && next->type != R_OCEAN && next->type != R_LAKE && iRIndex < iCurrentHead)
					{
						if(next->town && n==num)
						{
							if(oldList)
							{
								free(oldList);
							}
							if(newList)
							{
								free(newList);
							}
							return next;
						}
						else
						{
							if(next->town)
							{
								n++;
							}
							newList[iRIndex] = next;
							iRIndex++;
						}
					}
				}
			}
		}
		r++;
	}
	while(iRIndex > 0);

	return NULL;
}



void ARegion::ARMSetupProds(int iLandSize)
{
	Production * p;
	TerrainType * typer = &(TerrainDefs[type]);

	if(Globals->FOOD_ITEMS_EXIST) {
		if (typer->economy) {
			if (getrandom(2)&&!(ItemDefs[I_GRAIN].flags & ItemType::DISABLED)){
				p = new Production(I_GRAIN,typer->economy);
				products.Add(p);
			} else {
				if(!(ItemDefs[I_LIVESTOCK].flags & ItemType::DISABLED)) {
					p = new Production(I_LIVESTOCK,typer->economy);
					products.Add(p);
				}
			}
		}
	}

	for(unsigned int c= 0; c < (sizeof(typer->prods)/sizeof(Product)); c++) {
		int item = typer->prods[c].product;
		int chance = typer->prods[c].chance * iLandSize;//Klaus
		int amt = typer->prods[c].amount;
		if(item != -1) {
			if(!(ItemDefs[item].flags & ItemType::DISABLED) &&
					(getrandom(100) < chance)) {
				p = new Production(item,amt);
				products.Add(p);
			}
		}
	}
}