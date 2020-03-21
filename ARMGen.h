//---------------------------------------------------------------------------

#ifndef ARMGenH
#define ARMGenH
//---------------------------------------------------------------------------
#include "game.h"
#include "gamedata.h"
#include "CFractalMap.h"
//#include <Classes.hpp>
//---------------------------------------------------------------------------
struct TMapRect
{
  long left, top, right, bottom;
  TMapRect() {}
  TMapRect(const TMapPoint& TL, const TMapPoint& BR) { left=TL.x; top=TL.y; right=BR.x; bottom=BR.y; }
  TMapRect(int l, int t, int r, int b) { left=l; top=t; right=r; bottom=b; }
  int Width () const { return right  - left; }
  int Height() const { return bottom - top ; }

  bool operator ==(const TMapRect& rc) const
  {
	return left ==  rc.left  && top==rc.top &&
		   right == rc.right && bottom==rc.bottom;
  }
  bool operator !=(const TMapRect& rc) const
  {  return !(rc==*this); }

  __property long Left    = { read=left,   write=left   };
  __property long Top     = { read=top,    write=top };
  __property long Right   = { read=right,  write=right  };
  __property long Bottom  = { read=bottom, write=bottom };
};


const TMapPoint rround[6] = {TMapPoint(-1,-1),
				   TMapPoint( 0,+2),
				   TMapPoint(+1,-1),
				   TMapPoint(-1,+1),
				   TMapPoint(+1,+1),
				   TMapPoint( 0,-2)};

typedef enum
{
	rtUNKNOWN,
	rtBARRIERWATER,
	rtBARRIERMOUNTAIN,
	rtDEEPWATER,
	rtSHALLOW,
	rtRIVER,
	rtLAKE,
	rtMONTANA,
	rtHILLS,
	rtVOLKANO,
	rtPLAIN,
	rtFOREST,
	rtJUNGLE,
	rtSWAMP,
	rtDESERT,
	rtTUNDRA,
	rtWASTELANDS,
	rtSOLIDROCK,
	rtTUNNEL,
	rtCAVE,
	rtUNDERFOREST,
	rtLAVALAKE,
	rtCOUNT
} RegionType;

typedef enum{ctCLASSIC, ctETERNALMIDNIGHT, ctETERNALTWILIGHT, ctCOUNT} ClimateType;

typedef struct
{
	int h;
	RegionType type;
	int count;
	int iHumidity;
	int iTemperature;
	int nameID;
	int contiID;
	bool island;
	int aPop[NUMMAN];
	int iCitySize;
	bool bRareResource;
	int riverID;
	int riverNameID;
}TARMRegion;

typedef struct
{
	int m_iID;
	int m_iLength;
}TARMProvinceBorder;

typedef struct TARMProvince
{
	int m_iID;
	int m_iArea;
	int m_iBordersCount;
	TARMProvinceBorder *m_pBorders;
	TARMProvince *m_pPrev;
	TARMProvince *m_pNext;
}TARMProvince;

#define HASH_SIZE 7919

class ARMProvinceInfoTable
{
private:
	TARMProvince *m_pProvinces[HASH_SIZE];
	int size[HASH_SIZE];
	void DeleteBorder(int iID1, int iID2);
public:
	__fastcall ARMProvinceInfoTable();
	~ARMProvinceInfoTable();
	TARMProvince * Find(int iID);
	void Delete(TARMProvince *);
	int GetBorder(int iID1, int iID2);
	TARMProvince * Add(int iID);
	void SetArea(int iID, int iArea);
	int GetArea(int iID);
	void SetBorder(int iID1, int iID2, int iLength);
	TARMProvince * Merge(int iID1, int iID2);
	void IncreaseBorder(int iID1, int iID2, int iLength);
	int Hash(int iID);
	int GetBordersCount(int iID);
};

class ARMGenerator
{
private:
	int tempCountValue;
	int iIterRad;
	int iWaterPercent;
	bool bulk;
	void FillCount(int x, int y, int value, int range);
	int RaiseRegion(int x, int y, int type);
	void PlainMap();
	void PlainRegion(int x, int y, int type);
	int GetMaxDelta(int x, int y);
	int IsExtremum(int x, int y);
	int CountTheSame(int x, int y, bool start=true);
	void GrowSea(int x, int y);
	int GrowSeaOneStep(int h);
	void RenameArea(int x, int y, int newID);
	int CountSeaArea(int x, int y, bool start=true);
	int CountSeaFrontier(int x, int y, int otherID, bool start=true);
	int GrowElement(RegionType reg, int x, int y, const int maxWeight);
	bool CanBeDesert(int x, int y, bool anchor = false);
	bool CanBeForest(int x, int y, bool anchor = false);
	bool CanBeLake(int x, int y, bool anchor = false);
	bool CanBeMontana(int x, int y, bool anchor = false);
	bool CanBeSwamp(int x, int y, bool anchor = false);
	bool CanBeTundra(int x, int y, bool anchor = false);
	AString GetSeaName(int nameID);
	AString GetRiverName(int nameID);
//	int CountLand(int x, int y, bool start = true);
	void GrowHumidity(int x, int y, int iHumidity);
	bool CanBeElement(int x, int y, RegionType tRegion);
	int FindLand(int x, int y, int range);
	void MergeProvinces();
	void InitPopReg(int x, int y);
	int * CountAreaRaces(int *aPop, int x, int y, bool start=true);
	void FixDeserts();
	void FixSwamps();
	void FixForestnJungle();
	AString GetHillsName(AString sBase, int nameID);
	AString GetVolcanoName(AString sBase, int nameID);
	void FixShallows();
	void GenerateUnderground();
protected:
public:
	ClimateType m_iClimate;
	int GetHeight(int x, int y);
	int HasInRange(int x, int y, RegionType type, int range, bool start=true);
	int CountWater(int x, int y, bool start = true);
	int CountLand(int x, int y, bool start = true);
	int iMaxDist, iMinDist;
	int maxID;
	int maxContiID;
	int WIDTH, HEIGHT;
	TARMRegion **m_pMap;
	TARMRegion **m_pUnderMap;
//	int **m_pSeasInfo;
	ARMProvinceInfoTable *m_pProvinceInfo;
	TMapRect *m_pSeasLocation;
	int m_iAge;
	int m_iT;
	int m_iMoisure;
	int watercount, plaincount, montanacount;
	int swampcount, forestcount, junglecount, desertcount, tundracount;
	int water;
	int iMaxHeight;
	int m_iProgress;
	bool bRiversEnabled;
	bool bAdvancedMap;         // public user declarations

	void RandomizeMap();
	void SetTypes(bool fast=false);
	void AddHeight(int r1, int r2, int type);
	void GenerateFractal(int iContinents, int iAge, int iT, int iWet, float fNeedWater, float fNeedMontanas, int bounds);
	void Clear();
	void BalanceMap(float fNeedWater, float fNeedMontanas);

	__fastcall ARMGenerator(int xSize=128, int ySize=128);
	~ARMGenerator();
	void FormateSeasNames();
	void FormateSeasNamesOneStep(int h);
	void MergeSeas();
	int AddRiver(int iStartX, int iStartY, bool start=true, int length=0);
	AString GetElementName(int x, int y);
	void CalculateHumidity();
	AString GetForestName(AString sBase, int nameID);
	AString GetDesertName(AString sBase, int nameID);
	AString GetJungleName(AString sBase, int nameID);
	AString GetMountainName(AString sBase, int nameID);
	AString GetSwampName(AString sBase, int nameID);
	bool IsRegionCompatible(int x, int y, int iType);
	AString GetPlainName(AString sBase, int nameID);
	int GetStandartRType(RegionType rType, bool bAdvancedMap);
	void InitPop();
	int SortedRacesByPop(int x, int y, int place);
	void DynamicDemographyOneStep();
	bool IsSpecificRegionRace(int rType, int race, int coastal);
	AString GetLakeName(int nameID);
	AString GetTundraName(AString sBase, int nameID);
	int GetAreaPrevealedRace(int x, int y);
	bool CanBeHill(int x, int y, bool anchor = false);
	void CalculateTemperature();
	bool CheckTemperature(int x, int y, int tRegion);
	bool CheckHumidity(int x, int y, int tRegion);
	int AddElementAtRandom(int tRegion, int mass);
	void GenerateComplex(int iContinents, int iAge, int iT, int iWet, float fNeedWater, float fNeedMontanas, int bounds);
	bool CanBeWastes(int x, int y, bool anchor = false);
	AString GetWastelandsName(AString sBase, int nameID);
	void SetRareResources();
};
//---------------------------------------------------------------------------
#endif
