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
#include "game.h"
#include "gamedata.h"

void Game::ProcessCastOrder(Unit * u,AString * o, OrdersCheck *pCheck )
{
    int val;
    int lv = -5;
    AString * token = o->gettoken();
    if (!token) {
        ParseError( pCheck, u, 0, "CAST: No skill given.");
        return;
    }
    if((*token == "LEVEL") || (*token == "level")) {
        token = o->gettoken();
        // TODO: check for no level number given
        lv = token->value();
        if((lv < 1) || (lv > 5)) lv = 1;
        token = o->gettoken();
    }

    if (!token) {
        ParseError( pCheck, u, 0, "CAST: No skill given.");
        return;
    }

    int sk = ParseSkill(token);
    delete token;
    if (sk==-1) {
        ParseError( pCheck, u, 0, "CAST: Invalid skill.");
        return;
    }

    if( !( SkillDefs[sk].flags & SkillType::MAGIC )) {
        ParseError( pCheck, u, 0, "CAST: That is not a magic skill.");
        return;
    }
    if( !( SkillDefs[sk].flags & SkillType::CAST )) {
        ParseError( pCheck, u, 0, "CAST: That skill cannot be CAST.");
        return;
    }

    int ul = u->GetSkill(sk);
    if((lv == -5) || (lv > ul)) lv = ul;

    if( !pCheck ) {
        //
        // XXX -- should be error checking spells
        //
        switch(sk) {
			case S_MIND_READING:
				ProcessMindReading(u,o, pCheck, lv);
				break;
                        case S_MANA_GENERATION:
                        case S_CREATE_MANA_POTION:
			case S_CONSTRUCT_PORTAL:
			case S_CONSTRUCT_GATE:
			case S_ENGRAVE_RUNES_OF_WARDING:
			case S_SUMMON_IMPS:
			case S_SUMMON_DEMON:
			case S_SUMMON_BALROG:
			case S_SUMMON_SKELETONS:
			case S_RAISE_UNDEAD:
			case S_SUMMON_LICH:
			case S_WOLF_LORE:
			case S_NATURE_LORE:
			case S_CREATE_RING_OF_INVISIBILITY:
			case S_CREATE_CLOAK_OF_INVULNERABILITY:
			case S_CREATE_STAFF_OF_FIRE:
			case S_CREATE_STAFF_OF_LIGHTNING:
			case S_CREATE_AMULET_OF_TRUE_SEEING:
			case S_CREATE_AMULET_OF_PROTECTION:
			case S_CREATE_RUNESWORD:
			case S_CREATE_SHIELDSTONE:
			case S_CREATE_MAGIC_CARPET:
			case S_CREATE_FOOD:
                        case S_CREATE_WINDSTONE:
                        case S_ALCHEMY:
				ProcessGenericSpell(u,sk, pCheck, lv);
				break;
                        // Goony
                        // all enchants and transmutes are to be here
			case S_ENCHANT_WEAPON:
			case S_ENCHANT_ARMOR:
			case S_TRANSMUTE:
			case S_TRANSMUTE_WEAPON:
			case S_TRANSMUTE_ARMOR:
                                ProcessTransformFromSpell(u,o,sk,pCheck,lv);
                                break;
			case S_CLEAR_SKIES:
				val = SkillDefs[S_CLEAR_SKIES].rangeIndex;
				if(val != -1)
					ProcessRegionSpell(u, o, sk, pCheck, lv);
				else
					ProcessGenericSpell(u, sk, pCheck, lv);
				break;
			case S_FARSIGHT:
			case S_TELEPORTATION:
			case S_WEATHER_LORE:
				ProcessRegionSpell(u, o, sk, pCheck, lv);
				break;
			case S_BIRD_LORE:
				ProcessBirdLore(u,o, pCheck, lv);
				break;
			case S_INVISIBILITY:
				ProcessInvisibility(u,o, pCheck, lv);
				break;
                        case S_TRUE_SEEING:
                                ProcessTrueSeeing(u,o, pCheck, lv);
                                break;
			case S_GATE_LORE:
				ProcessCastGateLore(u,o, pCheck, lv);
				break;
			case S_PORTAL_LORE:
				ProcessCastPortalLore(u,o, pCheck, lv);
				break;
			case S_CREATE_PHANTASMAL_BEASTS:
				ProcessPhanBeasts(u,o, pCheck, lv);
				break;
			case S_CREATE_PHANTASMAL_UNDEAD:
				ProcessPhanUndead(u,o, pCheck, lv);
				break;
			case S_CREATE_PHANTASMAL_DEMONS:
				ProcessPhanDemons(u,o, pCheck, lv);
				break;
                        case S_CREATE_CLOAKS:
                                ProcessCreateCloaks(u, o, sk, pCheck, lv);
                                break;
			case S_DRAGON_LORE:
                                ProcessDragonLore(u, o, sk, pCheck, lv);
                                break;
                        case S_SUMMON_GOLEM:
                                ProcessSummonGolem(u, o, sk, pCheck, lv);
                                break;
                        case S_CHANNEL:
                                ProcessCastChannel(u, o, pCheck, lv);
                                break;
		}
       }
}

void Game::ProcessMindReading(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
    UnitId *id = ParseUnit(o);

    if (!id) {
        u->Error("CAST: No unit specified.");
        return;
    }
    CastMindOrder *order = new CastMindOrder;
    order->id = id;
    order->spell = S_MIND_READING;
    order->level = lv;
    order->needl = lv;

    u->ClearCastOrders();
    u->castorders = order;
}

void Game::ProcessBirdLore(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
    AString *token = o->gettoken();

    if (!token) {
        u->Error("CAST: Missing arguments.");
        return;
    }

    if (*token == "raven" || *token == "ravens")
    {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_BIRD_LORE;
        order->level = 2;
        if(lv < 2) lv = 2;
        order->needl = lv;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "eagle" || *token == "eagles")
    {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_BIRD_LORE;
        order->level = 3;
        if(lv < 3) lv = 3;
        order->needl = lv;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "albatross" || *token == "albatrosses")
    {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_BIRD_LORE;
        order->level = 4;
        if(lv < 4) lv = 4;
        order->needl = lv;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "direction") {
        delete token;
        token = o->gettoken();

        if (!token) {
            u->Error("CAST: Missing arguments.");
            return;
        }

        int dir = ParseDir(token);
        delete token;
        if (dir == -1 || dir > NDIRS) {
            u->Error("CAST: Invalid direction.");
            return;
        }

        CastIntOrder *order = new CastIntOrder;
        order->spell = S_BIRD_LORE;
        order->level = 1;
        order->needl = lv;
        order->target = dir;
        u->ClearCastOrders();
        u->castorders = order;

        return;
    }

    u->Error("CAST: Invalid arguments.");
    delete token;
}

void Game::ProcessInvisibility(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
    AString *token = o->gettoken();

    if (!token || !(*token == "units")) {
        u->Error("CAST: Must specify units to render invisible.");
        return;
    }
    delete token;

    CastUnitsOrder *order;
    if (u->castorders && u->castorders->type == O_CAST &&
        ((CastOrder *) u->castorders)->spell == S_INVISIBILITY) {
        order = (CastUnitsOrder *) u->castorders;
    } else {
        order = new CastUnitsOrder;
        order->spell = S_INVISIBILITY;
        order->level = lv;
        order->needl = lv;
        u->ClearCastOrders();
        u->castorders = order;
    }

    UnitId *id = ParseUnit(o);
    while (id) {
        order->units.Add(id);
        id = ParseUnit(o);
    }
}

void Game::ProcessTrueSeeing(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
    AString *token = o->gettoken();

    if (!token || !(*token == "units")) {
        u->Error("CAST: Must specify units.");
        return;
    }
    delete token;

    CastUnitsOrder *order;
    if (u->castorders && u->castorders->type == O_CAST &&
        ((CastOrder *) u->castorders)->spell == S_TRUE_SEEING) {
        order = (CastUnitsOrder *) u->castorders;
    } else {
        order = new CastUnitsOrder;
        order->spell = S_TRUE_SEEING;
        order->level = lv;
        order->needl = lv;
        u->ClearCastOrders();
        u->castorders = order;
    }

    UnitId *id = ParseUnit(o);
    while (id) {
        order->units.Add(id);
        id = ParseUnit(o);
    }
}

void Game::ProcessPhanDemons(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
    CastIntOrder *order = new CastIntOrder;
    order->spell = S_CREATE_PHANTASMAL_DEMONS;
    order->level = 0;
    order->target = 1;

    AString *token = o->gettoken();

    if (!token) {
        u->Error("CAST: Illusion to summon must be given.");
        delete order;
        return;
    }

    if (*token == "imp" || *token == "imps") {
        order->level = 1;
        order->needl = lv;
    }

    if (*token == "demon" || *token == "demons") {
        order->level = 3;
        if(lv < 3) lv = 3;
        order->needl = lv;
    }

    if (*token == "balrog" || *token == "balrogs") {
        order->level = 5;
        if(lv < 5) lv = 5;
        order->needl = lv;
    }

    delete token;

    if (!order->level) {
        u->Error("CAST: Can't summon that illusion.");
        delete order;
        return;
    }

    token = o->gettoken();

    if (!token) {
        order->target = 1;
    } else {
        order->target = token->value();
        delete token;
    }

    u->ClearCastOrders();
    u->castorders = order;
}

void Game::ProcessPhanUndead(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
	CastIntOrder *order = new CastIntOrder;
	order->spell = S_CREATE_PHANTASMAL_UNDEAD;
	order->level = 0;
	order->target = 1;

	AString *token = o->gettoken();

	if (!token) {
		u->Error("CAST: Must specify which illusion to summon.");
		delete order;
		return;
	}

	if (*token == "skeleton" || *token == "skeletons") {
		order->level = 1;
                order->needl = lv;
	}

	if (*token == "undead") {
		order->level = 3;
                if(lv < 3) lv = 3;
                order->needl = lv;
	}

	if (*token == "lich" || *token == "liches") {
		order->level = 5;
                if(lv < 5) lv = 5;
                order->needl = lv;
	}

	delete token;

	if (!order->level) {
		u->Error("CAST: Must specify which illusion to summon.");
		delete order;
		return;
	}

	token = o->gettoken();

	if (token) {
		order->target = token->value();
		delete token;
	} else {
		order->target = 1;
	}

	u->ClearCastOrders();
	u->castorders = order;
}

void Game::ProcessPhanBeasts(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
	CastIntOrder *order = new CastIntOrder;
	order->spell = S_CREATE_PHANTASMAL_BEASTS;
	order->level = 0;
	order->target = 1;

	AString *token = o->gettoken();

	if (!token) {
		u->Error("CAST: Must specify which illusion to summon.");
		delete order;
		return;
	}

	if (*token == "dire wolf" || *token == "dire wolves") {
		order->level = 1;
                order->needl = lv;
	}
	if (*token == "eagle" || *token == "eagles") {
		order->level = 3;
                if(lv < 3) lv = 3;
                order->needl = lv;
	}
	if (*token == "dragon" || *token == "dragons") {
		order->level = 5;
                if(lv < 5) lv = 5;
                order->needl = lv;
	}

	delete token;
	if (!order->level) {
		delete order;
		u->Error("CAST: Must specify which illusion to summon.");
		return;
	}

	token = o->gettoken();
	if (token) {
		order->target = token->value();
		delete token;
	}

	u->ClearCastOrders();
	u->castorders = order;
}

void Game::ProcessGenericSpell(Unit *u,int spell, OrdersCheck *pCheck, int lv)
{
	CastOrder *orders = new CastOrder;
	orders->spell = spell;
	orders->level = lv;
        orders->needl = lv;
	u->ClearCastOrders();
	u->castorders = orders;
}

void Game::ProcessRegionSpell(Unit *u, AString *o, int spell,
		OrdersCheck *pCheck, int lv)
{
	AString *token = o->gettoken();
	int x = -1;
	int y = -1;
	int z = -1;
	int val = SkillDefs[spell].rangeIndex;
	RangeType *range = NULL;
	if(val != -1) range = &RangeDefs[val];

	if(token) {
		if(*token == "region") {
			delete token;
			token = o->gettoken();
			if(!token) {
				u->Error("CAST: Region X coordinate not specified.");
				return;
			}
			x = token->value();
			delete token;

			token = o->gettoken();
			if(!token) {
				u->Error("CAST: Region Y coordinate not specified.");
				return;
			}
			y = token->value();
			delete token;

			if(range && (range->flags & RangeType::RNG_CROSS_LEVELS)) {
				token = o->gettoken();
				if(token) {
					z = token->value();
					delete token;
					if(z < 0 || (z >= Globals->UNDERWORLD_LEVELS +
								Globals->UNDERDEEP_LEVELS +
								Globals->ABYSS_LEVEL + 2)) {
						u->Error("CAST: Invalid Z coordinate specified.");
						return;
					}
				}
			}
		} else {
			delete token;
		}
	}
	if(x == -1) x = u->object->region->xloc;
	if(y == -1) y = u->object->region->yloc;
	if(z == -1) z = u->object->region->zloc;

	CastRegionOrder *order;
	if(spell == S_TELEPORTATION)
		order = new TeleportOrder;
	else
		order = new CastRegionOrder;
	order->spell = spell;
	order->level = lv;
        order->needl = lv;
	order->xloc = x;
	order->yloc = y;
	order->zloc = z;

	u->ClearCastOrders();
	/* Teleports happen late in the turn! */
	if(spell == S_TELEPORTATION)
		u->teleportorders = (TeleportOrder *)order;
	else
		u->castorders = order;
}

void Game::ProcessCastPortalLore(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
	AString *token = o->gettoken();
	if (!token) {
		u->Error("CAST: Requires a target mage.");
		return;
	}
	int gate = token->value();
	delete token;
	token = o->gettoken();

	if (!token) {
		u->Error("CAST: No units to teleport.");
		return;
	}

	if (!(*token == "units")) {
		u->Error("CAST: No units to teleport.");
		delete token;
		return;
	}

	TeleportOrder *order;

	if (u->teleportorders && u->teleportorders->spell == S_PORTAL_LORE) {
		order = u->teleportorders;
	} else {
		order = new TeleportOrder;
		u->ClearCastOrders();
		u->teleportorders = order;
	}

	order->gate = gate;
	order->spell = S_PORTAL_LORE;
	order->level = lv;
        order->needl = lv;

	UnitId *id = ParseUnit(o);
	while(id) {
		order->units.Add(id);
		id = ParseUnit(o);
	}
}

void Game::ProcessCastGateLore(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
	AString *token = o->gettoken();

	if (!token) {
		u->Error("CAST: Missing argument.");
		return;
	}

	if ((*token) == "gate") {
		delete token;
		token = o->gettoken();

		if (!token) {
			u->Error("CAST: Requires a target gate.");
			return;
		}
		TeleportOrder *order;

		if (u->teleportorders && u->teleportorders->spell == S_GATE_LORE &&
				u->teleportorders->gate == token->value()) {
			order = u->teleportorders;
		} else {
			order = new TeleportOrder;
			u->ClearCastOrders();
			u->teleportorders = order;
		}

		order->gate = token->value();
		if (!order->gate) {
			u->Error("CAST: Requires a gate number.");
			return;
		}                                             
		order->spell = S_GATE_LORE;
		order->level = 3;
                if(lv < 3) lv = 3;
                order->needl = lv;

		delete token;

		token = o->gettoken();

		if (!token) return;
		if (!(*token == "units")) {
			delete token;
			return;
		}

		UnitId *id = ParseUnit(o);
		while(id) {
			order->units.Add(id);
			id = ParseUnit(o);
		}
		return;
	}

	if ((*token) == "random") {
		TeleportOrder *order;

		if (u->teleportorders && u->teleportorders->spell == S_GATE_LORE &&
				u->teleportorders->gate == -1 ) {
			order = u->teleportorders;
		} else {
			order = new TeleportOrder;
			u->ClearCastOrders();
			u->teleportorders = order;
		}

		order->gate = -1;
		order->spell = S_GATE_LORE;
		order->level = 1;
                order->needl = lv;

		delete token;

		token = o->gettoken();

		if (!token) return;
		if (!(*token == "units")) {
			delete token;
			return;
		}

		UnitId *id = ParseUnit(o);
		while(id) {
			order->units.Add(id);
			id = ParseUnit(o);
		}
		return;
	}

	if ((*token) == "detect") {
		delete token;
		u->ClearCastOrders();
		CastOrder *to = new CastOrder;
		to->spell = S_GATE_LORE;
		to->level = 2;
                if(lv < 2) lv = 2;
                to->needl = lv;
		u->castorders = to;
		return;
	}

	delete token;
	u->Error("CAST: Invalid argument.");
}

bool Game::RunACastOrder(ARegion * r,Object *o,Unit * u)
{
	if (u->type != U_MAGE) {
		u->Error("CAST: Unit is not a mage.");
		return 0;
	}

	if (u->castorders->level == 0) {
		u->castorders->level = u->GetSkill(u->castorders->spell);
	}

	if (u->GetSkill(u->castorders->spell) < u->castorders->level ||
			u->castorders->level == 0) {
		u->Error("CAST: Skill level isn't high enough");
		return 0;
	}

        int lv = u->castorders->needl;
	int sk = u->castorders->spell;

	bool ret = 0;
	if(CheckCostCast(u, sk, lv))
	{
		switch (sk) {
			case S_MIND_READING:
				ret = RunMindReading(r,u,lv);
				break;

						// Goony //all enchants and transforms are to be here
			case S_ENCHANT_ARMOR:
			case S_ENCHANT_WEAPON:
				ret = RunTransformFromItem(r,u,sk,lv, false);
				break;
			case S_TRANSMUTE:
			case S_TRANSMUTE_WEAPON:
			case S_TRANSMUTE_ARMOR:
				ret = RunTransformFromItem(r,u,sk,lv, true);
				break;
			case S_CONSTRUCT_GATE:
				ret = RunConstructGate(r,u,lv);
				break;
			case S_ENGRAVE_RUNES_OF_WARDING:
				ret = RunEngraveRunes(r,o,u,lv);
				break;
			case S_CONSTRUCT_PORTAL:
				ret = RunCreateArtifact(r,u,sk,I_PORTAL,lv);
				break;
			case S_CREATE_CLOAKS:
				ret = RunCreateCloaks(r,u,sk,lv);
				break;
			case S_CREATE_RING_OF_INVISIBILITY:
				ret = RunCreateArtifact(r,u,sk,I_RINGOFI,lv);
				break;
			case S_CREATE_CLOAK_OF_INVULNERABILITY:
				ret = RunCreateArtifact(r,u,sk,I_CLOAKOFI,lv);
				break;
			case S_CREATE_STAFF_OF_FIRE:
				ret = RunCreateArtifact(r,u,sk,I_STAFFOFF,lv);
				break;
			case S_CREATE_STAFF_OF_LIGHTNING:
				ret = RunCreateArtifact(r,u,sk,I_STAFFOFL,lv);
				break;
			case S_CREATE_AMULET_OF_TRUE_SEEING:
				ret = RunCreateArtifact(r,u,sk,I_AMULETOFTS,lv);
				break;
			case S_CREATE_AMULET_OF_PROTECTION:
				ret = RunCreateArtifact(r,u,sk,I_AMULETOFP,lv);
				break;
			case S_CREATE_RUNESWORD:
				ret = RunCreateArtifact(r,u,sk,I_RUNESWORD,lv);
				break;
			case S_CREATE_SHIELDSTONE:
				ret = RunCreateArtifact(r,u,sk,I_SHIELDSTONE,lv);
				break;
			case S_CREATE_MAGIC_CARPET:
				ret = RunCreateArtifact(r,u,sk,I_MCARPET,lv);
				break;
			case S_CREATE_FLAMING_SWORD:
				ret = RunCreateArtifact(r,u,sk,I_FSWORD,lv);
				break;
			case S_SUMMON_IMPS:
				ret = RunSummonImps(r,u,lv);
				break;
			case S_SUMMON_DEMON:
				ret = RunSummonDemon(r,u,lv);
				break;
			case S_SUMMON_BALROG:
				ret = RunSummonBalrog(r,u,lv);
				break;
			case S_SUMMON_LICH:
				ret = RunSummonLich(r,u,lv);
				break;
			case S_RAISE_UNDEAD:
				ret = RunRaiseUndead(r,u,lv);
				break;
			case S_SUMMON_SKELETONS:
				ret = RunSummonSkeletons(r,u,lv);
				break;
			case S_DRAGON_LORE:
				ret = RunDragonLore(r,u,lv);
				break;
			case S_BIRD_LORE:
				ret = RunBirdLore(r,u,lv);
				break;
			case S_WOLF_LORE:
				ret = RunWolfLore(r,u,lv);
				break;
                        case S_SUMMON_GOLEM:
				ret = RunSummonGolem(r,u,lv);
				break;
			case S_INVISIBILITY:
				ret = RunInvisibility(r,u,lv);
				break;
			case S_TRUE_SEEING:
                                ret = RunTrueSeeing(r,u,lv);
                                break;
			case S_CREATE_PHANTASMAL_DEMONS:
				ret = RunPhanDemons(r,u,lv);
				break;
			case S_CREATE_PHANTASMAL_UNDEAD:
				ret = RunPhanUndead(r,u,lv);
				break;
			case S_CREATE_PHANTASMAL_BEASTS:
				ret = RunPhanBeasts(r,u,lv);
				break;
			case S_GATE_LORE:
				ret = RunDetectGates(r,o,u,lv);
				break;
			case S_FARSIGHT:
				ret = RunFarsight(r,u,lv);
				break;
			case S_NATURE_LORE:
				ret = RunNatureLore(r,u,lv);
				break;
			case S_WEATHER_LORE:
				ret = RunWeatherLore(r, u,lv);
				break;
			case S_CLEAR_SKIES:
				ret = RunClearSkies(r,u,lv);
				break;
			case S_CREATE_FOOD:
				ret = RunCreateFood(r, u,lv);
				break;
			case S_CREATE_MANA_POTION:
				ret = RunCreateManaPotion(r, u,lv);
				break;
			case S_CREATE_WINDSTONE:
				ret = RunCreateArtifact(r,u,sk,I_WINDSTONE,lv);
				break;
			case S_ALCHEMY:
				ret = RunAlchemy(r,u,lv);
				break;
                        case S_CHANNEL:
                                ret = RunChannel(r,u,lv);
                                break;
		}
	}

	if(ret)
		CalcCastMana(u, sk, lv);
	else
		ManaGeneration(u, 0);

	return 1;
}

int Game::GetRegionInRange(ARegion *r, ARegion *tar, Unit *u, int spell, int lv)
{
//	int level = u->GetSkill(spell);
	int level = lv;
	if(!level) {
		u->Error("CAST: You don't know that spell.");
		return 0;
	}

	int val = SkillDefs[spell].rangeIndex;
	if(val == -1) {
		u->Error("CAST: Spell is not castable at range.");
		return 0;
	}

	RangeType *range = &RangeDefs[val];

	int rtype = regions.GetRegionArray(r->zloc)->levelType;
	if((rtype == ARegionArray::LEVEL_NEXUS) &&
			!(range->flags & RangeType::RNG_NEXUS_SOURCE)) {
		u->Error("CAST: Spell does not work from the Nexus.");
		return 0;
	}

	if(!tar) {
		u->Error("CAST: No such region.");
		return 0;
	}

	rtype = regions.GetRegionArray(tar->zloc)->levelType;
	if((rtype == ARegionArray::LEVEL_NEXUS) &&
			!(range->flags & RangeType::RNG_NEXUS_TARGET)) {
		u->Error("CAST: Spell does not work to the Nexus.");
		return 0;
	}

	if((rtype != ARegionArray::LEVEL_SURFACE) &&
			(range->flags & RangeType::RNG_SURFACE_ONLY)) {
		u->Error("CAST: Spell can only target regions on the surface.");
		return 0;
	}
	if(!(range->flags&RangeType::RNG_CROSS_LEVELS) && (r->zloc != tar->zloc)) {
		u->Error("CAST: Spell is not able to work across levels.");
		return 0;
	}

	int maxdist;
	switch(range->rangeClass) {
		default:
		case RangeType::RNG_ABSOLUTE:
			maxdist = 1;
			break;
		case RangeType::RNG_LEVEL:
			maxdist = level;
			break;
		case RangeType::RNG_LEVEL2:
			maxdist = level * level;
			break;
		case RangeType::RNG_LEVEL3:
			maxdist = level * level * level;
			break;
	}
	maxdist *= range->rangeMult;

	int dist;
	if(range->flags & RangeType::RNG_CROSS_LEVELS)
		dist = regions.GetPlanarDistance(tar, r, range->crossLevelPenalty);
	else
		dist = regions.GetDistance(tar, r);
	if(dist > maxdist) {
		u->Error("CAST: Target region out of range.");
		return 0;
	}
	return 1;
}

bool Game::RunMindReading(ARegion *r,Unit *u, int lv)
{
	CastMindOrder *order = (CastMindOrder *) u->castorders;
//	int level = u->GetSkill(S_MIND_READING);
        int level = lv;

	Unit *tar = r->GetUnitId(order->id,u->faction->num);
	if (!tar) {
		u->Error("No such unit.");
		return 0;
	}

	AString temp = AString("Casts Mind Reading: ") + *(tar->name) + ", " +
		*(tar->faction->name);
	u->Practise(S_MIND_READING);

	if (level < 4) {
		u->Event(temp + ".");
		return 1;
	}

	temp += tar->items.Report(2,5,0) + ". Skills: ";
	temp += tar->skills.Report(tar->GetMen()) + ".";

	u->Event(temp);

        return 1;
}
/*
bool Game::RunEnchantSwords(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_ENCHANT_SWORDS);
        int level = lv;
	int max = ItemDefs[I_MSWORD].mOut * level;
	int num = 0;
	int count = 0;
	unsigned int c;
	int found;

	// Figure out how many components there are
	for(c=0; c<sizeof(ItemDefs[I_MSWORD].mInput)/sizeof(Materials); c++) {
		if(ItemDefs[I_MSWORD].mInput[c].item != -1) count++;
	}

	while(max) {
		int i, a;
		found = 0;
		// See if we have enough of all items
		for(c=0; c<sizeof(ItemDefs[I_MSWORD].mInput)/sizeof(Materials); c++) {
			i = ItemDefs[I_MSWORD].mInput[c].item;
			a = ItemDefs[I_MSWORD].mInput[c].amt;
			if(i != -1) {
				if(u->items.GetNum(i) >= a) found++;
			}
		}
		// We do not, break.
		if(found != count) break;

		// Decrement our inputs
		for(c=0; c<sizeof(ItemDefs[I_MSWORD].mInput)/sizeof(Materials); c++) {
			i = ItemDefs[I_MSWORD].mInput[c].item;
			a = ItemDefs[I_MSWORD].mInput[c].amt;
			if(i != -1) {
				u->items.SetNum(i, u->items.GetNum(i) - a);
			}
		}
		// We've made one.
		num++;
		max--;
	}

	u->items.SetNum(I_MSWORD,u->items.GetNum(I_MSWORD) + num);
	u->Event(AString("Enchants ") + num + " mithril swords.");
	u->Practise(S_ENCHANT_SWORDS);
	r->NotifySpell(u,S_ARTIFACT_LORE, &regions );

        return 1;
}
*/
bool Game::RunTransformFromItem(ARegion *r,Unit *u, int sk, int lv, bool squaredLevel)
{
	CastTransformFromOrder* ord=(CastTransformFromOrder*)u->castorders;

	int max = ItemDefs[ord->targetItemId].mOut * lv;
        int manamax = 0;
        int amtmana = 0;
        int mana = u->faction->mana;
        int mpot = u->items.GetNum(I_MANAPOTION);

        
        if(ItemDefs[ord->targetItemId].pInput[1].item == I_MANAPOTION) {
                amtmana = ItemDefs[ord->targetItemId].pInput[2].amt;
                manamax = amtmana * lv;
        }
        else {
                manamax = 1000;
        }

	if(squaredLevel)
	{
		max = max * lv;
	}

	int num = 0;
	int found, fomana;

	while(max && (manamax > 0))
	{
		found = u->items.GetNum(ord->sourceItemId);
                fomana = mana + mpot;
		if(found < ((num+1) * ord->sourceAmt)) break;
                if(fomana < ((num+1) * amtmana)) break;
		// We've made one.
		num++;
		max--;
                manamax -= amtmana;
	}
        int costm = num * amtmana;
        switch(u->manause){
                case 0:       // using faction pool (hope it will be obsolete soon)

	          if(mana >= costm) {
		        u->faction->mana -= costm;
        	  }
	          else {
		        u->faction->mana = 0;
		        u->items.SetNum(I_MANAPOTION, mana + mpot - costm);
                   }
                   break;

                case 1:        // using potions
                   if(mpot >= costm) {
        	        u->items.SetNum(I_MANAPOTION, mpot - costm);
                   }
                   else {
                        u->items.SetNum(I_MANAPOTION, 0);
                	u->faction->mana = mana + mpot - costm;
                   };
                   break;
        };

	u->items.SetNum(ord->sourceItemId, u->items.GetNum(ord->sourceItemId) - ord->sourceAmt*num);
	u->items.SetNum(ord->targetItemId,u->items.GetNum(ord->targetItemId) + num);
	AString sAction;
	switch (sk)
	{
			case S_ENCHANT_ARMOR:
				sAction = "Casts Enchant Armor, enchanting ";
				break;
			case S_ENCHANT_WEAPON:
				sAction = "Casts Enchant Weapon, enchanting ";
				break;
			case S_TRANSMUTE:
				sAction = "Casts Transmute, processing ";
				break;
			case S_TRANSMUTE_WEAPON:
				sAction = "Casts Transmute Weapon, processing ";
				break;
			case S_TRANSMUTE_ARMOR:
				sAction = "Casts Transmute Armor, processing ";
	}

	u->Event(sAction + ord->sourceAmt*num + " " + ItemDefs[ord->sourceItemId].names + " into " + num + " " + ItemDefs[ord->targetItemId].names);
	u->Practise(ord->spell);
		if (ord->tonotify>-1)
			r->NotifySpell(u,ord->tonotify, &regions );
		return 1;
}


bool Game::RunCreateFood(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_CREATE_FOOD);
        int level = lv;
	int max = ItemDefs[I_FOOD].mOut * level;
	int num = 0;
	int count = 0;
	unsigned int c;
	int found;

	// Figure out how many components there are
	for(c=0; c<sizeof(ItemDefs[I_FOOD].mInput)/sizeof(Materials); c++) {
		if(ItemDefs[I_FOOD].mInput[c].item != -1) count++;
	}

	while(max) {
		int i, a;
		found = 0;
		// See if we have enough of all items
		for(c=0; c<sizeof(ItemDefs[I_FOOD].mInput)/sizeof(Materials); c++) {
			i = ItemDefs[I_FOOD].mInput[c].item;
			a = ItemDefs[I_FOOD].mInput[c].amt;
			if(i != -1) {
				if(u->items.GetNum(i) >= a) found++;
			}
		}
		// We do not, break.
		if(found != count) break;

		// Decrement our inputs
		for(c=0; c<sizeof(ItemDefs[I_FOOD].mInput)/sizeof(Materials); c++) {
			i = ItemDefs[I_FOOD].mInput[c].item;
			a = ItemDefs[I_FOOD].mInput[c].amt;
			if(i != -1) {
				u->items.SetNum(i, u->items.GetNum(i) - a);
			}
		}
		// We've made one.
		num++;
		max--;
	}

	u->items.SetNum(I_FOOD,u->items.GetNum(I_FOOD) + num);
	u->Event(AString("Creates ") + num + " food.");
	u->Practise(S_CREATE_FOOD);
	r->NotifySpell(u,S_NATURE_LORE, &regions );

        return 1;
}

bool Game::RunConstructGate(ARegion *r,Unit *u, int lv)
{
	if (TerrainDefs[r->type].similar_type == R_OCEAN) {
		u->Error("Gates may not be constructed at sea.");
		return 0;
	}

	if (TerrainDefs[r->type].similar_type == R_NEXUS) {
		u->Error("Gates may not be constructed here.");
		return 0;
	}

	if (r->gate) {
		u->Error("There is already a gate in that region.");
		return 0;
	}

	if (u->GetMoney() < 50000) {
		u->Error("There is no money for construction of a gate.");
		return 0;
	}

        if (lv*20 < getrandom(100)) {
                u->Event(AString("Construction of a gate has failed."));
                return 0;
        }

	u->Event(AString("Constructs a Gate in ")+r->ShortPrint( &regions )+".");
	u->Practise(S_CONSTRUCT_GATE);
	u->SetMoney(u->GetMoney() - 50000);
	regions.numberofgates++;
	r->gate = regions.numberofgates;
	r->NotifySpell(u,S_ARTIFACT_LORE, &regions );

        return 1;
}

bool Game::RunEngraveRunes(ARegion *r,Object *o,Unit *u, int lv)
{
	if (!o->IsBuilding()) {
		u->Error("Runes of Warding may only be engraved on a building.");
		return 0;
	}

	if (o->incomplete > 0) {
		u->Error( "Runes of Warding may only be engraved on a completed "
				"building.");
		return 0;
	}

//	int level = u->GetSkill(S_ENGRAVE_RUNES_OF_WARDING);
        int level = lv;

	switch (level) {
		case 5:
			if (o->type == O_MKEEP) break;
			if (o->type == O_MCASTLE) break;
			if (o->type == O_MCITADEL) break;
		case 4:
			if (o->type == O_CITADEL) break;
		case 3:
			if (o->type == O_CASTLE) break;
		case 2:
			if (o->type == O_FORT) break;
			if (o->type == O_KEEP) break;
			if (o->type == O_MTOWER) break;
		case 1:
			if (o->type == O_TOWER) break;
		default:
			u->Error("Not high enough level to engrave Runes of Warding on "
					"that building.");
			return 0;
	}

	if (u->GetMoney() < 10000) {
		u->Error("Can't afford to engrave Runes of Warding.");
		return 0;
	}

	u->SetMoney(u->GetMoney() - 10000);
	if( o->type == O_MKEEP  ||
		o->type == O_MCASTLE  ||
		o->type == O_MCITADEL) {
		o->runes = 5;
	} else if(o->type == O_MTOWER) {
		o->runes = 4;
	} else {
		o->runes = 3;
	}
	u->Event(AString("Engraves Runes of Warding on ") + *(o->name) + ".");
	u->Practise(S_ENGRAVE_RUNES_OF_WARDING);
	r->NotifySpell(u,S_ARTIFACT_LORE, &regions );

		return 1;
}

bool Game::RunSummonBalrog(ARegion *r,Unit *u, int lv)
{
	if (u->items.GetNum(I_BALROG)) {
		u->Error("Can't control more than one balrog.");
		return 0;
	}

//	int level = u->GetSkill(S_SUMMON_BALROG);
        int level = lv;

	int num = (level * 20 + getrandom(100)) / 100;

	u->items.SetNum(I_BALROG,u->items.GetNum(I_BALROG) + num);
	u->Event(AString("Summons ") + ItemString(I_BALROG,num) + ".");
	u->Practise(S_SUMMON_BALROG);
	r->NotifySpell(u,S_DEMON_LORE, &regions );

        return 1;
}

bool Game::RunSummonDemon(ARegion *r,Unit *u, int lv)
{
//      int level = u->GetSkill(S_SUMMON_DEMON);
        int level = lv;
        int num = 0;

        for(int i=0; i < level; i++) num += 1 + getrandom(4);

	u->items.SetNum(I_DEMON,u->items.GetNum(I_DEMON) + num);
	u->Event(AString("Summons ") + ItemString(I_DEMON, num) + ".");
	u->Practise(S_SUMMON_DEMON);
	r->NotifySpell(u,S_DEMON_LORE, &regions );

        return 1;
}

bool Game::RunSummonImps(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_SUMMON_IMPS);
        int level = lv;
        int num = 0;

        for(int i=0; i < level; i++) num += 1 + getrandom(10);

	u->items.SetNum(I_IMP,u->items.GetNum(I_IMP) + num);
	u->Event(AString("Summons ") + ItemString(I_IMP, num) + ".");
	u->Practise(S_SUMMON_IMPS);
	r->NotifySpell(u,S_DEMON_LORE, &regions );

        return 1;
}

bool Game::RunCreateArtifact(ARegion *r,Unit *u,int skill,int item, int lv)
{
//	int level = u->GetSkill(skill);
        int level = lv;
	unsigned int c;

	int num = (level * ItemDefs[item].mOut)/100;

	for(c = 0; c < sizeof(ItemDefs[item].mInput)/sizeof(Materials); c++) {
		if(ItemDefs[item].mInput[c].item == -1) continue;
		int amt = u->items.GetNum(ItemDefs[item].mInput[c].item);
		int cost = num * ItemDefs[item].mInput[c].amt;
		if(amt < cost) {
			u->Error(AString("Doesn't have sufficient ") +
					ItemDefs[ItemDefs[item].mInput[c].item].name +
					" to create that.");
			return 0;
		}
	}

	// Deduct the costs
	for(c = 0; c < sizeof(ItemDefs[item].mInput)/sizeof(Materials); c++) {
		if(ItemDefs[item].mInput[c].item == -1) continue;
		int amt = u->items.GetNum(ItemDefs[item].mInput[c].item);
		int cost = num * ItemDefs[item].mInput[c].amt;
		u->items.SetNum(ItemDefs[item].mInput[c].item, amt-cost);
	}

	u->items.SetNum(item,u->items.GetNum(item) + num);
	u->Event(AString("Creates ") + ItemString(item,num) + ".");
	u->Practise(skill);
	r->NotifySpell(u,S_ARTIFACT_LORE, &regions );

        return 1;
}

bool Game::RunSummonLich(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_SUMMON_LICH);
        int level = lv;

	int num = ((20 * level) + getrandom(100))/100;

	u->items.SetNum(I_LICH,u->items.GetNum(I_LICH) + num);
	u->Event(AString("Summons ") + ItemString(I_LICH,num) + ".");
	u->Practise(S_SUMMON_LICH);
	r->NotifySpell(u,S_NECROMANCY, &regions );

        return 1;
}

bool Game::RunRaiseUndead(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_RAISE_UNDEAD);
        int level = lv;
        int num = 0;

        for(int i=0; i < level; i++) num += 1 + getrandom(6);

	u->items.SetNum(I_UNDEAD,u->items.GetNum(I_UNDEAD) + num);
	u->Event(AString("Raises ") + ItemString(I_UNDEAD,num) + ".");
	u->Practise(S_RAISE_UNDEAD);
	r->NotifySpell(u,S_NECROMANCY, &regions );

        return 1;
}

bool Game::RunSummonSkeletons(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_SUMMON_SKELETONS);
        int level = lv;
        int num = 0;

        for(int i=0; i < level; i++) num += 1 + getrandom(12);

	u->items.SetNum(I_SKELETON,u->items.GetNum(I_SKELETON) + num);
	u->Event(AString("Summons ") + ItemString(I_SKELETON,num) + ".");
	u->Practise(S_SUMMON_SKELETONS);
	r->NotifySpell(u,S_NECROMANCY, &regions );

        return 1;
}

bool Game::RunSummonGolem(ARegion *r,Unit *u, int lv)
{
        CastIntOrder *order = (CastIntOrder *) u->castorders;

        int kol;
        int golem = order->target;
        if (!golem) return 0; //huh?

        // at first, check valid summons
        if (golem == I_EGOLEM) {
                int num = u->items.GetNum(I_EGOLEM);
                kol = lv;
                num += kol;
       		u->items.SetNum(I_EGOLEM, num);
                if(kol == 1) {
                	u->Event("Summons a earth golem.");
                } else {
                	u->Event(AString("Summons a ") + kol + " earth golems.");
                }
        }
        if (golem == I_IGOLEM) {
	        int num = u->items.GetNum(I_IGOLEM);
                if (lv < 2) return 0;
                kol = lv - 1;
                num += kol;
       		u->items.SetNum(I_IGOLEM, num);
                if(kol == 1) {
                	u->Event("Summons a iron golem.");
                } else {
                	u->Event(AString("Summons a ") + kol + " iron golems.");
                }
        }
        if (golem == I_FGOLEM) {
                int num = u->items.GetNum(I_FGOLEM);
                if (lv < 3) return 0;
                kol = lv - 2;
                num += kol;
       		u->items.SetNum(I_FGOLEM, num);
                if(kol == 1) {
                	u->Event("Summons a fire golem.");
                } else {
                	u->Event(AString("Summons a ") + kol + " fire golems.");
                }
        }
	u->Practise(S_SUMMON_GOLEM);

        return 1;
}

bool Game::RunDragonLore(ARegion *r, Unit *u, int lv)
{
//   Alternate Spell

        CastIntOrder *order = (CastIntOrder *) u->castorders;

        int drag = order->target;
        if (!drag) return 0; //huh?

        // at first, check valid summons
        if (drag == I_WYVERN) {
                int num = u->items.GetNum(I_WYVERN);
                num += lv;
       		u->items.SetNum(I_WYVERN, num);
        	u->Event("Summons a wyverns.");
        }
        if (drag == I_REDDRAGON) {
	        int num = u->items.GetNum(I_GREENDRAGON) + u->items.GetNum(I_BLUEDRAGON);
                if (num) return 0; // no mixing red dragons with others is permitted
                num = u->items.GetNum(I_REDDRAGON);
                if (num) return 0; // no more than one red dragon per mage is permitted
                if (lv < 5) return 0;
                if (getrandom(100) < 80) {
        		u->items.SetNum(I_REDDRAGON, 1);
	        	u->Event("Summons a red dragon.");
	        } else {
		        u->Event("Attempts to summon a red dragon, but fails.");
        	}
        }
        if (drag == I_GREENDRAGON) {
                int num = u->items.GetNum(I_REDDRAGON);
                if (num) return 0; // no mixing red dragons with others is permitted

                num = u->items.GetNum(I_GREENDRAGON);

                int summon = 1 + ((lv-2) * 60 + getrandom (100)) / 100;
       		u->items.SetNum(I_GREENDRAGON, num + summon);
        	u->Event("Summons a green dragons.");
        }
        if (drag == I_BLUEDRAGON) {
                int num = u->items.GetNum(I_REDDRAGON);
                if (num) return 0; // no mixing red dragons with others is permitted

	        num = u->items.GetNum(I_BLUEDRAGON);
                if (num > lv - 2 ) return 0; // exceeds control level

       		u->items.SetNum(I_BLUEDRAGON, num + 1);
        	u->Event("Summons a blue dragon.");
        }
	u->Practise(S_DRAGON_LORE);
	r->NotifySpell(u,S_NATURE_LORE, &regions );

        return 1;
/*  DISABLE  a Dragon
        int level = lv;

	int num = u->items.GetNum(I_DRAGON);
	if (num >= level) {
		u->Error("Mage may not summon more dragons.");
		return 0;
	}

	int chance = level * level * 4;
	if (getrandom(100) < chance) {
		u->items.SetNum(I_DRAGON,num + 1);
		u->Event("Summons a dragon.");
	} else {
		u->Event("Attempts to summon a dragon, but fails.");
	}
	u->Practise(S_DRAGON_LORE);
	r->NotifySpell(u,S_NATURE_LORE, &regions );

        return 1;
*/
}

bool Game::RunBirdLore(ARegion *r,Unit *u, int lv)
{
	CastIntOrder *order = (CastIntOrder *) u->castorders;
	int type = regions.GetRegionArray(r->zloc)->levelType;

	if(type != ARegionArray::LEVEL_SURFACE) {
		AString error = "CAST: Bird Lore may only be cast on the surface of ";
		error += Globals->WORLD_NAME;
		error += ".";
		u->Error(error.Str());
		return 0;
	}

	// New usage:
	r->NotifySpell(u,S_NATURE_LORE, &regions );

	if (lv == 1){ //order->level == 1) {
   	// spy on adjacent hex
		int dir = order->target;
		ARegion *tar = r->neighbors[dir];
		if (!tar) {
			u->Error("CAST: No such region.");
			return 0;
		}
		Farsight *f = new Farsight;
		f->faction = u->faction;
		f->level = lv;
		tar->farsees.Add(f);
		u->Event(AString("Sends birds to spy on ") +
				tar->Print( &regions ) + ".");
		u->Practise(S_BIRD_LORE);
		return 1;
	}

	int num;
	if (order->level == 2) {
        	// summon ravens
                num = 1 + getrandom(3 * lv );
		u->items.SetNum(I_RAVEN,u->items.GetNum(I_RAVEN) + num);
		u->Event(AString("Casts Bird Lore, summoning ")+ItemString(I_RAVEN, num) + ".");
		u->Practise(S_BIRD_LORE);
   	return 1;
	}

	if (order->level == 3) {
        	// summon eagles
                num = lv - 2;
		u->items.SetNum(I_EAGLE,u->items.GetNum(I_EAGLE) + num);
		u->Event(AString("Casts Bird Lore, summoning ")+ItemString(I_EAGLE, num) + ".");
		u->Practise(S_BIRD_LORE);
   	return 1;
	}

	if (order->level > 3) {
        	// summon albatrosses
                num = lv - 3;
		u->items.SetNum(I_ALBATROSS,u->items.GetNum(I_ALBATROSS) + num);
		u->Event(AString("Casts Bird Lore, summoning ")+ItemString(I_ALBATROSS, num) + ".");
		u->Practise(S_BIRD_LORE);
   	return 1;
	}
   // this shouldn't happen, though:
   u->Event(AString("Casts Bird Lore, but nothing happens"));
   return 1;

/*   old usage:
	if (order->level < 3) {
		int dir = order->target;
		ARegion *tar = r->neighbors[dir];
		if (!tar) {
			u->Error("CAST: No such region.");
			return 0;
		}

		Farsight *f = new Farsight;
		f->faction = u->faction;
		f->level = lv;
		tar->farsees.Add(f);
		u->Event(AString("Sends birds to spy on ") +
				tar->Print( &regions ) + ".");
		u->Practise(S_BIRD_LORE);
		r->NotifySpell(u,S_NATURE_LORE, &regions );
		return 1;
	}

//	int level = u->GetSkill(S_BIRD_LORE);
	int level = lv;
	int num = (level - 2) * (level - 2);

	u->items.SetNum(I_EAGLE,u->items.GetNum(I_EAGLE) + num);
	u->Event(AString("Cast Bird Lore, summoning ")+ItemString(I_EAGLE, num) + ".");
	u->Practise(S_BIRD_LORE);
	r->NotifySpell(u,S_NATURE_LORE, &regions );

   return 1;
*/
}

bool Game::RunWolfLore(ARegion *r,Unit *u, int lv)
{
/*	if (TerrainDefs[r->type].similar_type != R_MOUNTAIN &&
		TerrainDefs[r->type].similar_type != R_FOREST) {
		u->Error("CAST: Can only summon wolves in mountain and "
				 "forest regions.");
		return 0;
	}
*/
//	int level = u->GetSkill(S_WOLF_LORE);
        int level = lv;
        int num = 0;

        for(int i=0; i < level; i++) num += 1 + getrandom(6);

	u->Event(AString("Casts Wolf Lore, summoning ") +
			ItemString(I_DWOLF, num) + ".");
	u->items.SetNum(I_DWOLF, u->items.GetNum(I_DWOLF) + num);
	u->Practise(S_WOLF_LORE);
	r->NotifySpell(u,S_NATURE_LORE, &regions );

        return 1;
}

bool Game::RunInvisibility(ARegion *r,Unit *u, int lv)
{
	CastUnitsOrder *order = (CastUnitsOrder *) u->castorders;
//	int max = u->GetSkill(S_INVISIBILITY);
        int max = lv;
	max = max * max;

	int num = 0;
	forlist (&(order->units)) {
		Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
		if (!tar) continue;
		if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
		num += tar->GetSoldiers();
	}

	if (num > max) {
		u->Error("CAST: Can't render that many men invisible.");
		return 0;
	}

	if (!num) {
		u->Error("CAST: No valid targets to turn invisible.");
		return 0;
	}
	{
		forlist (&(order->units)) {
			Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
			if (!tar) continue;
			if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
			tar->SetFlag(FLAG_INVIS,1);
			tar->Event(AString("Is rendered invisible by ") +
					*(u->name) + ".");
		}
	}

	r->NotifySpell(u,S_ILLUSION, &regions );
	u->Practise(S_INVISIBILITY);
	u->Event("Casts invisibility.");

        return 1;
}
bool Game::RunTrueSeeing(ARegion *r,Unit *u, int lv)
{
	CastUnitsOrder *order = (CastUnitsOrder *) u->castorders;
        int max = 2*lv;

	int num = 0;
	forlist (&(order->units)) {
		Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
		if (!tar) continue;
		if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
		num += tar->GetSoldiers();
	}

	if (num > max) {
		u->Error("CAST: Can't render that many men.");
		return 0;
	}

	if (!num) {
		u->Error("CAST: No valid targets to turn true seeing.");
		return 0;
	}
	{
		forlist (&(order->units)) {
			Unit *tar = r->GetUnitId((UnitId *) elem,u->faction->num);
			if (!tar) continue;
			if (tar->GetAttitude(r,u) < A_FRIENDLY) continue;
			tar->SetFlag(FLAG_TRUES,1);
			tar->Event(AString("Is rendered true seeing by ") +
					*(u->name) + ".");
		}
	}

	r->NotifySpell(u,S_ILLUSION, &regions );
	u->Practise(S_TRUE_SEEING);
	u->Event("Casts true seeing.");

        return 1;
}

bool Game::RunPhanDemons(ARegion *r,Unit *u, int lv)
{
	CastIntOrder *order = (CastIntOrder *) u->castorders;
//	int level = u->GetSkill(S_CREATE_PHANTASMAL_DEMONS);
        int level = lv;
	int create,max;

	if (order->level < 3) {
		create = I_IIMP;
		max = level * level * 4;
	} else {
		if (order->level < 5) {
			create = I_IDEMON;
			max = (level - 2) * (level - 2);
		} else {
			create = I_IBALROG;
			max = 1;
		}
	}

	if (order->target > max || order->target < 0) {
		u->Error("CAST: Can't create that many Phantasmal Demons.");
		return 0;
	}

	u->items.SetNum(create,order->target);
	u->Event("Casts Create Phantasmal Demons.");
	u->Practise(S_CREATE_PHANTASMAL_DEMONS);
	r->NotifySpell(u,S_ILLUSION, &regions );

        return 1;
}

bool Game::RunPhanUndead(ARegion *r,Unit *u, int lv)
{
	CastIntOrder *order = (CastIntOrder *) u->castorders;
//	int level = u->GetSkill(S_CREATE_PHANTASMAL_UNDEAD);
        int level = lv;
	int create,max;

	if (order->level < 3) {
		create = I_ISKELETON;
		max = level * level * 4;
	} else {
		if (order->level < 5) {
			create = I_IUNDEAD;
			max = (level - 2) * (level - 2);
		} else {
			create = I_ILICH;
			max = 1;
		}
	}

	if (order->target > max || order->target < 0) {
		u->Error("CAST: Can't create that many Phantasmal Undead.");
		return 0;
	}

	u->items.SetNum(create,order->target);
	u->Event("Casts Create Phantasmal Undead.");
	u->Practise(S_CREATE_PHANTASMAL_UNDEAD);
	r->NotifySpell(u,S_ILLUSION, &regions );

        return 1;
}

bool Game::RunPhanBeasts(ARegion *r,Unit *u, int lv)
{
	CastIntOrder *order = (CastIntOrder *) u->castorders;
//	int level = u->GetSkill(S_CREATE_PHANTASMAL_BEASTS);
        int level = lv;
	int create,max;

	if (order->level < 3) {
		create = I_IWOLF;
		max = level * level * 4;
	} else {
		if (order->level < 5) {
			create = I_IEAGLE;
			max = (level - 2) * (level - 2);
		} else {
			create = I_IDRAGON;
			max = 1;
		}
	}

	if (order->target > max || order->target < 0) {
		u->Error("CAST: Can't create that many Phantasmal Beasts.");
		return 0;
	}

	u->items.SetNum(create,order->target);
	u->Event("Casts Create Phantasmal Beasts.");
	u->Practise(S_CREATE_PHANTASMAL_BEASTS);
	r->NotifySpell(u,S_ILLUSION, &regions );

        return 1;
}

bool Game::RunNatureLore(ARegion *r,Unit *u, int lv)
{
//	int level = u->GetSkill(S_NATURE_LORE);
        int level = lv;

	int amt = r->Wages() * level * 15;
	if (level > r->naturelore) r->naturelore = level;

	u->items.SetNum(I_SILVER,u->items.GetNum(I_SILVER) + amt);
//	u->Event(AString("Casts Earth Lore, raising ") + amt + " silver.");
	u->Event(AString("Casts Nature Lore, raising ") + amt + " silver.");
	u->Practise(S_NATURE_LORE);
	r->NotifySpell(u,S_NATURE_LORE, &regions );

        return 1;
}

bool Game::RunAlchemy(ARegion *r,Unit *u, int lv)
{
        int level = lv;

	int amt = level *  level * 100;

	u->items.SetNum(I_SILVER,u->items.GetNum(I_SILVER) + amt);
	u->Event(AString("Uses Alchemy, raising ") + amt + " silver.");
	u->Practise(S_ALCHEMY);
	r->NotifySpell(u,S_ALCHEMY, &regions );
        return 1;
}


bool Game::RunClearSkies(ARegion *r, Unit *u, int lv)
{
	ARegion *tar = r;
	AString temp = "Casts Clear Skies";
	int val;

	CastRegionOrder *order = (CastRegionOrder *)u->castorders;

	val = SkillDefs[S_CLEAR_SKIES].rangeIndex;
	if(val != -1) {
		tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
		val = GetRegionInRange(r, tar, u, S_CLEAR_SKIES, lv);
		if(!val) return 0;
		temp += " on ";
		temp += tar->ShortPrint(&regions);
	}
	temp += ".";
//	int level = u->GetSkill(S_CLEAR_SKIES);
        int level = lv;
	if (level > r->clearskies) r->clearskies = level;
	u->Event(temp);
	u->Practise(S_CLEAR_SKIES);
	r->NotifySpell(u,S_WEATHER_LORE, &regions);

        return 1;
}

bool Game::RunWeatherLore(ARegion *r, Unit *u, int lv)
{
	ARegion *tar;
	int val;

	CastRegionOrder *order = (CastRegionOrder *)u->castorders;

	tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
	val = GetRegionInRange(r, tar, u, S_WEATHER_LORE, lv);
	if(!val) return 0;

//	int level = u->GetSkill(S_WEATHER_LORE);
        int level = lv;
	int months = 3;
	if(level >= 5) months = 12;
	else if (level >= 3) months = 6;

	AString temp = "Casts Weather Lore on ";
	temp += tar->ShortPrint(&regions);
	temp += ". It will be ";
	int weather, futuremonth;
	for(int i; i <= months; i++) {
		futuremonth = (month + i)%12;
		weather=regions.GetWeather(tar, futuremonth);
		temp += SeasonNames[weather];
		temp += " in ";
		temp += MonthNames[futuremonth];
		if(i < (months-1))
			temp += ", ";
		else if(i == (months-1))
			temp += " and ";
		else
			temp += ".";
	}
	u->Event(temp);
	u->Practise(S_WEATHER_LORE);
	r->NotifySpell(u, S_WEATHER_LORE, &regions);

        return 1;
}

bool Game::RunFarsight(ARegion *r,Unit *u, int lv)
{
	ARegion *tar;
	int val;

	CastRegionOrder *order = (CastRegionOrder *)u->castorders;

	tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
	val = GetRegionInRange(r, tar, u, S_FARSIGHT, lv);
	if(!val) return 0;

	Farsight *f = new Farsight;
	f->faction = u->faction;
//	f->level = u->GetSkill(S_FARSIGHT);
        f->level = lv;
	f->unit = u;
	tar->farsees.Add(f);
	AString temp = "Casts Farsight on ";
	temp += tar->ShortPrint(&regions);
	temp += ".";
	u->Event(temp);
	u->Practise(S_FARSIGHT);

        return 1;
}

bool Game::RunDetectGates(ARegion *r,Object *o,Unit *u, int lv)
{
//	int level = u->GetSkill(S_GATE_LORE);
        int level = lv;

	if (level == 1) {
		u->Error("CAST: Casting Gate Lore at level 1 has no effect.");
		return 0;
	}

	u->Event("Casts Gate Lore, detecting nearby Gates:");
	int found = 0;
	for (int i=0; i<NDIRS; i++) {
		ARegion *tar = r->neighbors[i];
		if (tar) {
			if (tar->gate) {
				if(Globals->DETECT_GATE_NUMBERS) {
					u->Event(tar->Print(&regions) + " contains Gate " +
							tar->gate + ".");
				} else {
					u->Event(tar->Print(&regions) + " contains a Gate.");
				}
				found = 1;
			}
		}
	}
	if (!found)
		u->Event("There are no nearby Gates.");
	u->Practise(S_GATE_LORE);

        return 1;
}

bool Game::RunTeleport(ARegion *r,Object *o,Unit *u, int lv)
{
	ARegion *tar;
	int val;

	CastRegionOrder *order = (CastRegionOrder *)u->teleportorders;

	tar = regions.GetRegion(order->xloc, order->yloc, order->zloc);
	val = GetRegionInRange(r, tar, u, S_TELEPORTATION, lv);
	if(!val) return 0;

//	int level = u->GetSkill(S_TELEPORTATION);
        int level = lv;
	int maxweight = level * 50;

	if (u->Weight() > maxweight) {
		u->Error("CAST: Can't carry that much when teleporting.");
		return 0;
	}

	// Presume they had to open the portal to see if target is ocean
	u->Practise(S_TELEPORTATION);

	if (TerrainDefs[tar->type].similar_type == R_OCEAN) {
		u->Error(AString("CAST: ") + tar->Print(&regions) + " is an ocean.");
		return 0;
	}

	u->Event(AString("Teleports to ") + tar->Print(&regions) + ".");
	u->MoveUnit(tar->GetDummy());

        return 1;
}

bool Game::RunGateJump(ARegion *r,Object *o,Unit *u, int lv)
{
//	int level = u->GetSkill(S_GATE_LORE);
        int level = lv;
	int nexgate = 0;
	if( !level ) {
		u->Error( "CAST: Unit doesn't have that skill." );
		return 0;
	}

	TeleportOrder *order = u->teleportorders;

	if (order->gate != -1 && level < 3) {
		u->Error("CAST: Unit Doesn't know Gate Lore at that level.");
		return 0;
	}

	nexgate = Globals->NEXUS_GATE_OUT &&
		(TerrainDefs[r->type].similar_type == R_NEXUS);
	if (!r->gate && !nexgate) {
		u->Error("CAST: There is no gate in that region.");
		return 0;
	}

	int maxweight = 10;
	if (order->gate != -1) level -= 2;
	switch (level) {
		case 1:
			maxweight = 150;
			break;
		case 2:
			maxweight = 500;
			break;
		case 3:
		case 4:
		case 5:
			maxweight = 5000;
			break;
	}

	int weight = u->Weight();

	forlist (&(order->units)) {
		Unit *taru = r->GetUnitId((UnitId *) elem,u->faction->num);
		if (taru && taru != u) weight += taru->Weight();
	}

	if (weight > maxweight) {
		u->Error( "CAST: That mage cannot carry that much weight through "
				"a Gate." );
		return 0;
	}

	ARegion *tar;
	if (order->gate == -1) {
		int good = 0;
		int gatenum = getrandom(regions.numberofgates);
		tar = regions.FindGate(gatenum+1);

		if(tar && tar->zloc == r->zloc) good = 1;
		if(tar && nexgate && tar->zloc == ARegionArray::LEVEL_SURFACE)
			good = 1;

		while( !good ) {
			gatenum = getrandom(regions.numberofgates);
			tar = regions.FindGate(gatenum+1);
			if(tar && tar->zloc == r->zloc) good = 1;
			if(tar && nexgate && tar->zloc == ARegionArray::LEVEL_SURFACE)
				good = 1;
		}

		u->Event("Casts Random Gate Jump.");
		u->Practise(S_GATE_LORE);
	} else {
		if (order->gate < 1 || order->gate > regions.numberofgates) {
			u->Error("CAST: No such target gate.");
			return 0;
		}

		tar = regions.FindGate(order->gate);
		if (!tar) {
			u->Error("CAST: No such target gate.");
			return 0;
		}

		u->Event("Casts Gate Jump.");
		u->Practise(S_GATE_LORE);
	}

	int comma = 0;
	AString unitlist; {
		forlist(&(order->units)) {
			Location *loc = r->GetLocation((UnitId *) elem,u->faction->num);
			if (loc) {
				/* Don't do the casting unit yet */
				if (loc->unit == u) {
					delete loc;
					continue;
				}

				if (loc->unit->GetAttitude(r,u) < A_ALLY) {
					u->Error("CAST: Unit is not allied.");
				} else {
					if (comma) {
						unitlist += AString(", ") + AString(loc->unit->num);
					} else {
						unitlist += AString(loc->unit->num);
						comma = 1;
					}

					loc->unit->Event(AString("Is teleported through a ") +
							"Gate to " + tar->Print(&regions) + " by " +
							*u->name + ".");
					loc->unit->MoveUnit( tar->GetDummy() );
					if (loc->unit != u) loc->unit->ClearCastOrders();
				}
				delete loc;
			} else {
				u->Error("CAST: No such unit.");
			}
		}
	}

	u->Event(AString("Jumps through a Gate to ") +
			tar->Print( &regions ) + ".");
	u->Practise(S_GATE_LORE);
	if (comma) {
		u->Event(unitlist + " follow through the Gate.");
	}
	u->MoveUnit( tar->GetDummy() );

        return 1;
}

bool Game::RunPortalLore(ARegion *r,Object *o,Unit *u, int lv)
{
//	int level = u->GetSkill(S_PORTAL_LORE);
        int level = lv;
	TeleportOrder *order = u->teleportorders;

	if (!level) {
		u->Error("CAST: Doesn't know Portal Lore.");
		return 0;
	}

	if (!u->items.GetNum(I_PORTAL)) {
		u->Error("CAST: Unit doesn't have a Portal.");
		return 0;
	}

	int maxweight = 500 * (level*level);
	int maxdist = 2 * level * level;
	int weight = 0;
	forlist (&(order->units)) {
		Unit *taru = r->GetUnitId((UnitId *) elem,u->faction->num);
		if (taru) weight += taru->Weight();
	}

    if (weight > maxweight) {
		u->Error("CAST: That mage cannot teleport that much weight through a "
				"Portal.");
		return 0;
	}

	Location *tar = regions.FindUnit(order->gate);
	if (!tar) {
		u->Error("CAST: No such target mage.");
		return 0;
	}

	if (tar->unit->faction->GetAttitude(u->faction->num) < A_FRIENDLY) {
		u->Error("CAST: Target mage is not friendly.");
		return 0;
	}

	if (tar->unit->type != U_MAGE) {
		u->Error("CAST: Target is not a mage.");
		return 0;
	}

	if (!tar->unit->items.GetNum(I_PORTAL)) {
		u->Error("CAST: Target does not have a Portal.");
		return 0;
	}

	if (regions.GetDistance(r,tar->region) > maxdist) {
		u->Error("CAST: Can't Portal Jump that far.");
		return 0;
	}

	u->Event("Casts Portal Jump.");
	u->Practise(S_PORTAL_LORE);

	{
		forlist(&(order->units)) {
			Location *loc = r->GetLocation((UnitId *) elem,u->faction->num);
			if (loc) {
				if (loc->unit->GetAttitude(r,u) < A_ALLY) {
					u->Error("CAST: Unit is not allied.");
				} else {
					loc->unit->Event(AString("Is teleported to ") +
							tar->region->Print( &regions ) +
							" by " + *u->name + ".");
					loc->unit->MoveUnit( tar->obj );
					if (loc->unit != u) loc->unit->ClearCastOrders();
				}
				delete loc;
			} else {
				u->Error("CAST: No such unit.");
			}
		}
	}

	delete tar;

		return 1;
}

void Game::RunTeleportOrders()
{
	bool ret = 0;

	ARegion * r;
	forlist(&regions) {
		r = (ARegion *) elem;
		forlist(&r->objects) {
			Object * o = (Object *) elem;
			int foundone = 1;
			while (foundone)
			{
				foundone = 0;
				forlist(&o->units) {
					Unit * u = (Unit *) elem;
					if (u->teleportorders)
					{
						foundone = 1;
						ret = 0;
						int sk = 0;
						int lv = u->teleportorders->needl;
						if (u->GetSkill(u->teleportorders->spell) < u->teleportorders->level ||
							 lv == 0)
						{
							u->Error("CAST: Skill level isn't that high.");
//							ManaGeneration(u, 0);
//							return;
//							break;
						}
						else
						{
							switch (u->teleportorders->spell)
							{
								case S_GATE_LORE:
									sk = S_GATE_LORE;
									if (!CheckCostCast(u,sk,lv)) break;
									ret = RunGateJump(r,o,u,lv);
									break;
								case S_TELEPORTATION:
									sk = S_TELEPORTATION;
									if (!CheckCostCast(u,sk,lv)) break;
									ret = RunTeleport(r,o,u,lv);
									break;
								case S_PORTAL_LORE:
									sk = S_PORTAL_LORE;
									if (!CheckCostCast(u,sk,lv)) break;
									ret = RunPortalLore(r,o,u,lv);
									break;
							}
						}
						if(ret)
							CalcCastMana(u, sk, lv);
						else
							ManaGeneration(u, 0);

						delete u->teleportorders;
						u->teleportorders = 0;
						break;
					}
				}
			}
		}
	}
}

bool Game::RunCreateManaPotion(ARegion *r,Unit *u, int lv)
{
//  int level = u->GetSkill(S_CREATE_MANA_POTION);
    int level = lv;
    if( !level ) {
    	u->Error( "CAST: Unit doesn't have that skill." );
		return 0;
	}
	int mpotmax = 5 * level;
	int facmana = u->faction->mana;
	if(facmana < mpotmax) mpotmax = facmana;
	if(!mpotmax) {
		u->Error("No mana in faction mana pool.");
		return 0;
	}
	u->faction->mana -= mpotmax;
	u->items.SetNum(I_MANAPOTION, u->items.GetNum(I_MANAPOTION) + mpotmax);
	u->Event(AString("Creates ") + mpotmax + " mana potions.");
	u->Practise(S_CREATE_MANA_POTION);

	return 1;
}

void Game::ProcessCreateCloaks(Unit *u, AString *o, int sk, OrdersCheck *pCheck, int lv)
{

	AString *token = o->gettoken();

	if(token)
	{
	if (*token == "cloak_of_defence" || *token == "CLOD") {
		CastIntOrder *order = new CastIntOrder;
		order->spell = S_CREATE_CLOAKS;
		order->level = lv;
		order->needl = lv;
		order->target = I_CLOAKOFDEFENCE;
		u->ClearCastOrders();
		u->castorders = order;
		return;
	}
	if (*token == "cloak_of_reflection" || *token == "CLOR") {
		CastIntOrder *order = new CastIntOrder;
		order->spell = S_CREATE_CLOAKS;
		order->level = lv;
		order->needl = lv;
		order->target = I_CLOAKOFREFLECTION;
		u->ClearCastOrders();
		u->castorders = order;
		return;
	}
	if (*token == "cape_of_protection" || *token == "CPRO") {
		CastIntOrder *order = new CastIntOrder;
		order->spell = S_CREATE_CLOAKS;
		order->level = lv;
		order->needl = lv;
		order->target = I_CLOAKOFPROTECTION;
		u->ClearCastOrders();
		u->castorders = order;
		return;
	}
	if (*token == "cape_of_shield" || *token == "CSHD") {
		CastIntOrder *order = new CastIntOrder;
		order->spell = S_CREATE_CLOAKS;
		order->level = lv;
		order->needl = lv;
		order->target = I_CLOAKOFSHIELD;
		u->ClearCastOrders();
		u->castorders = order;
		return;
	}
	if (*token == "cape_of_stability" || *token == "CSTB") {
		CastIntOrder *order = new CastIntOrder;
		order->spell = S_CREATE_CLOAKS;
		order->level = lv;
		order->needl = lv;
		order->target = I_CLOAKOFSTABILITY;
		u->ClearCastOrders();
		u->castorders = order;
		return;
	}
	if (*token == "cloak_of_negation" || *token == "CLON") {
		CastIntOrder *order = new CastIntOrder;
		order->spell = S_CREATE_CLOAKS;
		order->level = lv;
		order->needl = lv;
		order->target = I_CLOAKOFNEGATION;
		u->ClearCastOrders();
		u->castorders = order;
		return;
	}
	}

	u->Error("CAST: Invalid arguments.");
	delete token;

}

bool Game::RunCreateCloaks(ARegion *r,Unit *u,int sk, int lv)
{

	CastIntOrder *order = (CastIntOrder *) u->castorders;
	int it;
        int skill = -1;
        int level = 0;
        it = order->target;

        switch(it) {
                case I_CLOAKOFDEFENCE:
                        break;
                case I_CLOAKOFREFLECTION:
                        skill = S_FORCE_SHIELD;
                        level = 3;
                        break;
                case I_CLOAKOFPROTECTION:
                        skill = S_SPIRIT_SHIELD;
                        level = 3;
                        break;
                case I_CLOAKOFSHIELD:
                        skill = S_ENERGY_SHIELD;
                        level = 3;
                        break;
                case I_CLOAKOFSTABILITY:
                        skill = S_CLEAR_SKIES;
                        level = 3;
                        break;
                case I_CLOAKOFNEGATION:
                        break;
                default:
                        skill = S_FORCE_SHIELD;
                        level = 6;
                        break;
        };
        bool ret = 0;
        if(skill == -1) {
                ret = RunCreateArtifact(r,u,sk,it,lv);
                return ret;
        }
        int ulevel = u->GetSkill(skill);

        if(ulevel >= level) {
                ret = RunCreateArtifact(r,u,sk,it,lv);
                return ret;
        }

        u->Error(AString("CAST: Second skill level isn't high enough."));

        return 0;

}

void Game::ProcessTransformFromSpell(Unit *u, AString *o, int sk, OrdersCheck *pCheck, int level)
{
		int sourceId=-1, sourceAmt=0;
		ItemType it;

		AString* token=o->gettoken();
		if (!token)
		{
//				u->Error("CAST: Not enough parameters");
//				delete token;
//				return;
			if(sk == S_ENCHANT_WEAPON) sourceId = I_SWORD;
			if(sk == S_ENCHANT_ARMOR) sourceId = I_PLATEARMOR;
		}
		else
		{
			for (int i=0;i<NITEMS;i++)
			{
				it=ItemDefs[i];
				if (*token==it.name || *token==it.abr)
				{
						sourceId=i;
						break;
				}
			}
		}

		delete token;

		if (sourceId==-1)
		{
				u->Error("CAST: Unknown item");
				return;
		}

        // now sourceId contains index of transforming item
        // searching if it can be enchanted

        int targetId=-1;

        for (int i=0;i<NITEMS && targetId==-1;i++)
        {
                it=ItemDefs[i];
                if (it.mSkill==sk)
                	for(int c=0; c<sizeof(it.mInput)/sizeof(Materials); c++)
                		if(it.mInput[c].item == sourceId)
                                {
                                        if (it.mLevel<=level)
                                        {
                                                targetId=i;
                                                sourceAmt=it.mInput[c].amt;
                                        }
                                        else
                                        {
                                                u->Error("CAST: Skill level isn't high enough.");
                                                return;
                                        }
                                }
        }

        if (targetId==-1)
        {
                u->Error("CAST: Item can't be transformed this way");
                return;
        }

        // now targetId contains index of item transforming to

	CastTransformFromOrder *orders = new CastTransformFromOrder;
	orders->spell = sk;
	orders->level = level;
        orders->needl = level;
        orders->sourceItemId=sourceId;
		orders->targetItemId=targetId;
        orders->sourceAmt=sourceAmt;
        switch (sk)
        {
                case S_ENCHANT_ARMOR:
                case S_ENCHANT_WEAPON:
                        orders->tonotify=S_ARTIFACT_LORE;
                        break;
                case S_TRANSMUTE:
                case S_TRANSMUTE_ARMOR:
                case S_TRANSMUTE_WEAPON:
                        orders->tonotify=S_EARTH_LORE;
                        break;
        }
	u->ClearCastOrders();
	u->castorders = orders;

}

void Game::ProcessDragonLore(Unit *u, AString *o, int sk, OrdersCheck *pCheck, int lv)
{
    AString *token = o->gettoken();

    if (!token)
    {
        u->Error("CAST: Missing arguments.");
        delete token;
        return;
    }

    if (*token == "wyvern" || *token == "WYVR") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_DRAGON_LORE;
        order->level = lv;
        order->needl = lv;
     	order->target = I_WYVERN;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "green_dragon" || *token == "GDRA") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_DRAGON_LORE;
        order->level = lv;
        order->needl = lv;
     	order->target = I_GREENDRAGON;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "blue_dragon" || *token == "UDRA") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_DRAGON_LORE;
        order->level = lv;
        order->needl = lv;
     	order->target = I_BLUEDRAGON;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "red_dragon" || *token == "RDRA") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_DRAGON_LORE;
        order->level = lv;
        order->needl = lv;
     	order->target = I_REDDRAGON;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    u->Error("CAST: Invalid arguments.");
    delete token;

}

void Game::ProcessSummonGolem(Unit *u, AString *o, int sk, OrdersCheck *pCheck, int lv)
{
    AString *token = o->gettoken();

    if (!token)
    {
        u->Error("CAST: Missing arguments.");
        delete token;
        return;
    }

    if (*token == "earth_golem" || *token == "EGOL") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_SUMMON_GOLEM;
        order->level = lv;
        order->needl = lv;
     	order->target = I_EGOLEM;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "iron_golem" || *token == "IGOL") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_SUMMON_GOLEM;
        order->level = lv;
        order->needl = lv;
     	order->target = I_IGOLEM;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    if (*token == "fire_golem" || *token == "FGOL") {
        CastIntOrder *order = new CastIntOrder;
        order->spell = S_SUMMON_GOLEM;
        order->level = lv;
        order->needl = lv;
     	order->target = I_FGOLEM;
        u->ClearCastOrders();
        u->castorders = order;
        return;
    }

    u->Error("CAST: Invalid arguments.");
    delete token;
}

void Game::ProcessCastChannel(Unit *u,AString *o, OrdersCheck *pCheck, int lv)
{
  AString *token = o->gettoken();

  ChannelOrder *order = new ChannelOrder;

  if (!token) {
    u->Error("CAST: Missing argument.");
    return;
  }

  if ( (*token == "to") || (*token == "TO") )
  {
    order->direction = 0;
  } else if ( (*token == "from") || (*token == "FROM") )
  {
    order->direction = 1;
  } else {
    u->Error("CAST: Invalid direction. Must be 'to' or 'from'.");
    return;
  }

  token = o->gettoken();

  if ( (*token == "unit") || (*token == "UNIT") )
  {
    order->targtype = 0; // units channeling
  } else {
    u->Error("CAST: Invalid target. Must be 'unit', '*faction', '*region' or '*item'.");
    return;
  }

  token = o->gettoken();
  if (token->value()) {
    order->target = token->value();
  } else {
    u->Error("CAST: Invalid channel target.");
    return;
  }

  token = o->gettoken();
  if ( (token) && (token->value()) ) {
    order->amount = token->value();
  }

  order->spell = S_CHANNEL;
  order->level = lv;
  order->needl = lv;

  u->castorders = order;

  delete token;
}

bool Game::RunChannel(ARegion *r,Unit *u, int lv)
{
    //int level = lv-1;
    if( !lv ) {
    	u->Error( "CAST: Unit doesn't have that skill." );
		return 0;
    }

    int maxchvalue = lv*lv*10;

    int maxdist = lv+1;

    ChannelOrder *orders = (ChannelOrder *) u->castorders;

    if ( orders->direction == 0 )
    {
        switch ( orders->targtype )
        {
          case 0:
               	Location *tar = regions.FindUnit(orders->target);
	        if (!tar) {
                  u->Error( "CAST: Channel to non-exist unit." );
	        }

        	if (tar->unit->faction->GetAttitude(u->faction->num) < A_FRIENDLY) {
	       	      u->Error("CAST: Target unit is not friendly.");
		      return 0;
	        }

        	if (regions.GetDistance(r,tar->region) > maxdist) {
        		u->Error("CAST: Can't channel that far.");
        		return 0;
                }
                
                if ( (orders->amount) && (u->items.GetNum(I_MANA) < orders->amount ) )
                {
                        u->Error("CAST: Not enough mana to channel requested.");
                        return 0;
                }

                if ( (orders->amount) && (maxchvalue < orders->amount) )
                {
                        u->Error("CAST: Not enough skill to channel requested.");
                        return 0;
                }

// all checks passed. go ahead.

                if ( orders->amount > 0 )
                  maxchvalue = orders->amount;

                tar->unit->items.SetNum(I_MANA, tar->unit->items.GetNum(I_MANA) + maxchvalue);
                u->items.SetNum(I_MANA, u->items.GetNum(I_MANA) - maxchvalue );

                u->Event(AString("Channels ") + maxchvalue + " mana to unit " + orders->target);
        	u->Practise(S_CHANNEL);

                break;
        }
    }
    return 1;
}
