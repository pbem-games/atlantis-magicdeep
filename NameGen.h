//---------------------------------------------------------------------------

#ifndef NameGenH
#define NameGenH

//#include <system.hpp>
//#include <Classes.hpp>
#include "astring.h"
#include "gamedata.h"

//---------------------------------------------------------------------------

extern AString GetHeroName(int iSeed, char *classname, int iRace = MAN_LEADER);
extern AString GetAbstractName(int iSeed);
extern AString GetObjectName(int iSeed, int iType);
extern AString GetEthnicName(int iRace, int iSeed);
extern AString GetRegionName(int iType, int iSize, int island, int iRace, int iSeed);

#endif
