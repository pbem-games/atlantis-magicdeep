#include "gamedata.h"
#include "items.h"
#include "skills.h"
#include "object.h"
#include "aregion.h"
#include "npc.h"
//#include <conio>

/*
	ManDefs - расы
	MonDefs - монстры
	WeaponDefs - оружие
	ArmorDefs - броньки
	MountDefs - верховые живтотные
	BattleItemDefs - боевые артефакты

	что есть что лежит в поле ItemDefs->type

     IT_NORMAL = 0x0001, 
     IT_ADVANCED = 0x0002, 
     IT_TRADE = 0x0004, 
   + IT_MAN = 0x0008, 
   + IT_MONSTER = 0x0010, 
     IT_MAGIC = 0x0020, 
   + IT_WEAPON = 0x0040, 
   + IT_ARMOR = 0x0080, 
   + IT_MOUNT = 0x0100, 
   + IT_BATTLE = 0x0200, 
     IT_SPECIAL = 0x0400, 
     IT_TOOL = 0x0800, 
     IT_FOOD = 0x1000, 
     IT_ARTIFACT = 0x2000, 
     IT_ETHEREAL = 0x4000 
*/
      /*
int main()
{
	cout << "<gamedata>\n";
        cout << "\t<items num=\""<< NITEMS <<"\">\n";
        for ( int i=0;i<NITEMS ;i++ )
        {
		cout << "\t\t<item\tabr=\""<< ItemDefs[i].abr << "\"\n";
		cout << "\t\t\tname=\""<< ItemDefs[i].name <<"\"\n";
		cout << "\t\t\tnames=\""<< ItemDefs[i].names <<"\"\n";
		cout << "\t\t\tflags=\""<< ItemDefs[i].flags <<"\"\n";
		cout << "\t\t\tpSkill=\""<< ItemDefs[i].pSkill <<"\"\n";
		cout << "\t\t\tpLevel=\""<< ItemDefs[i].pLevel <<"\"\n";
		cout << "\t\t\tpMonths=\""<< ItemDefs[i].pMonths <<"\"\n";
		cout << "\t\t\tpOut=\""<< ItemDefs[i].pOut << "\"\n";

		cout << "\t\t\tweight=\""<< ItemDefs[i].weight <<"\"\n";
		cout << "\t\t\ttype=\""<< ItemDefs[i].type <<"\"\n";
		cout << "\t\t\tbaseprice=\""<< ItemDefs[i].baseprice <<"\"\n";
		cout << "\t\t\tcombat=\""<< ItemDefs[i].combat <<"\"\n";
		cout << "\t\t\tindex=\""<< ItemDefs[i].index <<"\"\n";
		cout << "\t\t\tbattleindex=\""<< ItemDefs[i].battleindex <<"\"\n";
		cout << "\t\t\twalk=\""<< ItemDefs[i].walk <<"\"\n";
		cout << "\t\t\tride=\""<< ItemDefs[i].ride <<"\"\n";
		cout << "\t\t\tfly=\""<< ItemDefs[i].fly <<"\"\n";
		cout << "\t\t\tswim=\""<< ItemDefs[i].swim <<"\"\n";
		cout << "\t\t\thitchItem=\""<< ItemDefs[i].hitchItem <<"\"\n";
		cout << "\t\t\thitchWalk=\""<< ItemDefs[i].hitchwalk <<"\"\n";
		cout << "\t\t\tmult_item=\""<< ItemDefs[i].mult_item <<"\"\n";
		cout << "\t\t\tmult_val=\""<< ItemDefs[i].mult_val <<"\"\n"
  			<< "\t\t\t>\n\n";

		for ( int m=0; m<4;m++)
			cout << "\t\t\t<pInput item=\""<< ItemDefs[i].pInput[m].item <<"\" amt=\""<<
			        ItemDefs[i].pInput[m].amt <<"\"/>\n";
		cout << "\n";
		
		for ( int m=0; m<4;m++)
			cout << "\t\t\t<mInput item=\""<< ItemDefs[i].mInput[m].item <<"\" amt=\""<<
			        ItemDefs[i].mInput[m].amt <<"\"/>\n";
		cout << "\n";
		
		if (ItemDefs[i].type & IT_MAN ){
			cout << "\t\t\t<man speciallevel=\""<< ManDefs[ItemDefs[i].index].speciallevel <<"\" defaultlevel=\""<<
			       	ManDefs[ItemDefs[i].index].defaultlevel <<"\">\n";
			for (int m=0; m<6; m++)
				cout << "\t\t\t\t<skill id=\""<< ManDefs[ItemDefs[i].index].skills[m] <<"\"/>\n";

				cout << "\t\t\t</man>\n";
		}
		if (ItemDefs[i].type & IT_MONSTER ){
			cout << "\t\t\t<monster\n" <<
				"\t\t\t\tattackLevel=\""<< MonDefs[ItemDefs[i].index].attackLevel <<"\"\n" <<
				"\t\t\t\tnumAttacks=\""<< MonDefs[ItemDefs[i].index].numAttacks <<"\"\n" <<
				"\t\t\t\thits=\""<< MonDefs[ItemDefs[i].index].hits <<"\"\n" <<
				"\t\t\t\tregen=\""<< MonDefs[ItemDefs[i].index].regen <<"\"\n" <<
				"\t\t\t\ttactics=\""<< MonDefs[ItemDefs[i].index].tactics <<"\"\n" <<
				"\t\t\t\tstealth=\""<< MonDefs[ItemDefs[i].index].stealth <<"\"\n" <<
				"\t\t\t\tobs=\""<< MonDefs[ItemDefs[i].index].obs <<"\"\n" <<
				"\t\t\t\tspecial=\""<< MonDefs[ItemDefs[i].index].special <<"\"\n" <<
				"\t\t\t\tspecialLevel=\""<< MonDefs[ItemDefs[i].index].specialLevel <<"\"\n" <<
				"\t\t\t\tsilver=\""<< MonDefs[ItemDefs[i].index].silver <<"\"\n" <<
				"\t\t\t\tspoiltype=\""<< MonDefs[ItemDefs[i].index].spoiltype <<"\"\n" <<
				"\t\t\t\thostile=\""<< MonDefs[ItemDefs[i].index].hostile <<"\"\n" <<
				"\t\t\t\tnumber=\""<< MonDefs[ItemDefs[i].index].number <<"\"\n" <<
				"\t\t\t\tname=\""<< MonDefs[ItemDefs[i].index].name <<"\"\n" <<
				"\t\t\t\tmonbehind=\""<< MonDefs[ItemDefs[i].index].monbehind <<"\"\n" <<
				"\t\t\t\tmsup=\""<< MonDefs[ItemDefs[i].index].msup <<"\"\n" <<
				"\t\t\t\tnumsup=\""<< MonDefs[ItemDefs[i].index].numsup <<"\"\n\t\t\t\t>\n";
					for (int m = 0; m<NUM_ATTACK_TYPES; m++)
					cout << "\t\t\t\t\t<defence id=\""<< m << "\" value=\"" << MonDefs[ItemDefs[i].index].defense[m] <<"\" />\n";

				cout << "\t\t\t</monster>\n";
		}

		if (ItemDefs[i].type & IT_WEAPON ){
			cout << "\t\t\t<weapon\n" <<
				"\t\t\t\tflags=\"" << WeaponDefs[ItemDefs[i].index].flags <<"\"\n" <<
				"\t\t\t\tbaseSkill=\"" << WeaponDefs[ItemDefs[i].index].baseSkill <<"\"\n" <<
				"\t\t\t\torSkill=\"" << WeaponDefs[ItemDefs[i].index].orSkill <<"\"\n" <<
				"\t\t\t\tweapClass=\"" << WeaponDefs[ItemDefs[i].index].weapClass <<"\"\n" <<
				"\t\t\t\tattackType=\"" << WeaponDefs[ItemDefs[i].index].attackType <<"\"\n" <<
				"\t\t\t\tnumAttacks=\"" << WeaponDefs[ItemDefs[i].index].numAttacks <<"\"\n" <<
				"\t\t\t\tattackBonus=\"" << WeaponDefs[ItemDefs[i].index].attackBonus <<"\"\n" <<
				"\t\t\t\tdefenseBonus=\"" << WeaponDefs[ItemDefs[i].index].defenseBonus <<"\"\n" <<
				"\t\t\t\tmountBonus=\"" << WeaponDefs[ItemDefs[i].index].mountBonus <<"\"\n" <<
				"\t\t\t/>\n";
		}

		if (ItemDefs[i].type & IT_ARMOR ){
			cout << "\t\t\t<armor\tflags=\""<< ArmorDefs[ItemDefs[i].index].flags << "\"\n" <<
				"\t\t\t\tfrom=\"" << ArmorDefs[ItemDefs[i].index].from << "\"\n" <<
				"\t\t\t\t>\n";
			for (int m=0; m<NUM_WEAPON_CLASSES; m++)
				cout << "\t\t\t\t<save id=\""<< m << "\" value=\"" << ArmorDefs[ItemDefs[i].index].saves[m] << "\"/>\n";
			cout << "\t\t\t</armor>\n";
		}

		if (ItemDefs[i].type & IT_MOUNT )
			cout << "\t\t\t<mount\tskill=\""<< MountDefs[ItemDefs[i].index].skill <<"\"\n" <<
				"\t\t\t\tminBonus=\"" << MountDefs[ItemDefs[i].index].minBonus << "\"\n" <<
				"\t\t\t\tmaxBonus=\"" << MountDefs[ItemDefs[i].index].maxBonus << "\"\n" <<
				"\t\t\t\tmaxHamperedBonus=\"" << MountDefs[ItemDefs[i].index].maxHamperedBonus << "\"\n" <<
				"\t\t\t\tmountSpecial=\"" << MountDefs[ItemDefs[i].index].mountSpecial << "\"\n" <<
				"\t\t\t\tspecialLev=\"" << MountDefs[ItemDefs[i].index].specialLev << "\"\n" <<
				"\t\t\t/>\n";

		if (ItemDefs[i].type & IT_BATTLE )
			cout << "\t\t\t<battle\tflags=\""<< BattleItemDefs[ItemDefs[i].battleindex].flags <<"\"\n" <<
				"\t\t\t\titemNum=\"" << BattleItemDefs[ItemDefs[i].battleindex].itemNum << "\"\n" <<
				"\t\t\t\tindex=\"" << BattleItemDefs[ItemDefs[i].battleindex].index << "\"\n" <<
				"\t\t\t\tskillLevel=\"" << BattleItemDefs[ItemDefs[i].battleindex].skillLevel << "\"\n" <<
				"\t\t\t/>\n";
				
		cout << "\t\t</item>\n";
	}
	cout << "\t</items>\n\n";

	cout << "</gamedata>\n";
}
*/
