//---------------------------------------------------------------------------

#ifndef CFractalMapH
#define CFractalMapH
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//---------------------------------------------------------------------------

typedef struct
{
	int h;
	int contiID;
	int barrier;
	int allowed;
}TFRegion;

typedef struct
{
	int x,y;
	int chance;
}TPotent;
//---------------------------------------------------------------------------

class TMapPoint
{
public:
	int x,y;
	TMapPoint();
	TMapPoint(int xx, int yy);
	~TMapPoint();
};

class CFractalMap
{
private:
	int iWidth;
	int iHeight;
protected:
	void Clear();
	void AddContinent(int iSize, int contiID);
	int CountBoundsID(int x, int y, int contiID, int iRange);
	void BuildSeas();
public:
	void Smooth();        // private user declarations
	int m_iDepth;
	void GoDeeper(int iNewDepth);
	int m_iWater;
	int WIDTH;
	int HEIGHT;
	TFRegion **m_pMap;
	CFractalMap();
	~CFractalMap();
	void CreateWorld(int Width, int Height, int WaterPercent, int ContinentsCount, int Smoothness, int BarrierHeight=-1);
	int GetHeight(int iX, int iY);
	void CreateSphere(int Width, int Height, int WaterPercent, int ContinentsCount, int Smoothness);
	TMapPoint GetNeighbour(int x, int y, int dir, int allowed, TFRegion **pNewMap=nullptr, int iNewWIDTH=0, int iNewHEIGHT=0, int onlyExists=1);
	bool IsAllowed(int x, int y);
	TMapPoint GetOppositeNeighbour(int x, int y, int dir, int allowed, TFRegion **pNewMap=nullptr, int iNewWIDTH=0, int iNewHEIGHT=0);
	int GetContiID(int iX, int iY);
};
//---------------------------------------------------------------------------
#endif

