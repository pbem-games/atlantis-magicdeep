//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop

#include "CFractalMap.h"
#include "gameio.h"
#include <math.h>
//#include <Math.hpp>
//#pragma package(smart_init)
//---------------------------------------------------------------------------
/*
const TMapPoint aDir1[3] = {TMapPoint(-1,0),
				   TMapPoint(+1,0),
				   TMapPoint(0,-1)};
const TMapPoint aDir2[3] = {TMapPoint(-1,0),
				   TMapPoint(+1,0),
				   TMapPoint(0,+1)};
*/
int Log2(int val)
{
	return log(val)/log(2);
}

const TMapPoint aCounterDir1[3] = {TMapPoint(+3,-1),
							   TMapPoint(-3,-1),
							   TMapPoint(0,+2)};
const TMapPoint aCounterDir2[3] = {TMapPoint(+3,+1),
							   TMapPoint(-3,+1),
							   TMapPoint(0,-2)};
//*/
const TMapPoint aDir1[3] = {TMapPoint(-1,0),
				   TMapPoint(0,-1),
				   TMapPoint(+1,0)};
const TMapPoint aDir2[3] = {TMapPoint(-1,0),
				   TMapPoint(0,+1),
				   TMapPoint(+1,0)};

//---------------------------------------------------------------------------
TMapPoint::TMapPoint()
{
	x = 0;
	y = 0;
}
//---------------------------------------------------------------------------
TMapPoint::TMapPoint(int xx, int yy)
{
	x = xx;
	y = yy;
}
//---------------------------------------------------------------------------
TMapPoint::~TMapPoint()
{
	x = 0;
	y = 0;
}

//---------------------------------------------------------------------------
CFractalMap::CFractalMap()
{
	m_pMap = NULL;
	WIDTH = 0;
	HEIGHT = 0;
}
//---------------------------------------------------------------------------
void CFractalMap::Clear()
{
	if(m_pMap)
	{
		int i;
		for(i=0; i<WIDTH; i++)
		{
			free(m_pMap[i]);
		}
		free(m_pMap);
	}
	m_pMap = NULL;
	WIDTH = 0;
	HEIGHT = 0;
}

void CFractalMap::AddContinent(int iSize, int contiID)
{
	TMapPoint *continent;
	continent = new TMapPoint[iSize];
	int iCurrentSize = 0;

	TPotent	*frontier;
	frontier = new TPotent[iSize*3];
	int iFrontierLength = 0;

	int iX, iY;
	int cond = 7;
	int counter = 0;
	do
	{
		if(counter > WIDTH*HEIGHT)
		{
			cond--;
			counter = 0;
		}

		iX = getrandom(WIDTH);
		iY = getrandom(HEIGHT);

		counter++;
	}
	while((m_pMap[iX][iY].barrier != 0 || m_pMap[iX][iY].h != 0 || CountBoundsID(iX, iY, contiID, 3) < cond) && cond >= 0);

	if(cond<0)
	{
		return;
	}

	m_pMap[iX][iY].h = m_iWater + (0 + getrandom(8))*16;
	m_pMap[iX][iY].contiID = contiID;
//	m_pMap[iX][iY] = m_iWater;
	continent[0] = TMapPoint(iX, iY);
	iCurrentSize = 1;

	while(iCurrentSize < iSize)
	{
		iFrontierLength = 0;
		int chanceMax = 0;
		int chanceMaxCount = 0;
//		int iNewX;
//		int iNewY;
		TMapPoint	map;
		int iDir;

		int i;
		for(i=0; i<iCurrentSize; i++)
		{
			iX = continent[i].x;
			iY = continent[i].y;

			for(iDir=0; iDir<3; iDir++)
			{
				map = GetNeighbour(iX, iY, iDir, 1);
				if(map.x != iX || map.y != iY)
				{
					int chance;
					chance = CountBoundsID(map.x, map.y, contiID, 3);
					chance = chance*chance;
					if(m_pMap[map.x][map.y].barrier != 0 || m_pMap[map.x][map.y].h != 0)
					{
						chance = 0;
					}

					if(chance > 0)
					{
						chance += getrandom(3);
						frontier[iFrontierLength].x = map.x;
						frontier[iFrontierLength].y = map.y;

						if(chance == chanceMax)
						{
							chanceMaxCount++;
						}
						if(chance > chanceMax)
						{
							chanceMax = chance;
							chanceMaxCount = 1;
						}
						frontier[iFrontierLength].chance = chance;
						iFrontierLength++;

						if(iFrontierLength > iSize*3)
						{
							iFrontierLength--;
						}
					}
				}
			}
		}
		if(chanceMaxCount > 0)
		{
			int chance;
			chance = getrandom(chanceMaxCount);
			for(i=0; i<iFrontierLength; i++)
			{
				if(frontier[i].chance == chanceMax)
				{
					chance--;
					if(chance<0)
					{
						map.x = frontier[i].x;
						map.y = frontier[i].y;
						chance = iFrontierLength*2;
					}
				}
			}

//			m_pMap[map.x][map.y].h = m_iWater + 2 + getrandom(4);
			m_pMap[map.x][map.y].h = m_iWater + (0 + getrandom(8))*16;
			m_pMap[map.x][map.y].contiID = contiID;
//			m_pMap[map.x][map.y] = m_iWater + i;
			continent[iCurrentSize] = map;
			iCurrentSize++;
		}
		else
		{
			break;
		}
	}

	delete[] frontier;
	delete[] continent;
}

int CFractalMap::CountBoundsID(int x, int y, int contiID, int iRange)
{
	if(m_pMap[x][y].h != 0 && m_pMap[x][y].contiID != contiID)
	{
		return 0;
	}

	if(iRange <= 0)
	{
		return 1;
	}


	int iResult = 1 + iRange*3;//общее число регионов в заданном радиусе
	int oldContiID = -1;

	int i;
	for(i=0; i<3; i++)
	{
		TMapPoint map;
		map = GetNeighbour(x, y, i, 1);
		if(map.x != x || map.y != x)
		{
			if(m_pMap[map.x][map.y].h != 0 && m_pMap[map.x][map.y].contiID != contiID)//если регион уже принадлежит другому континенту
			{
				iResult -= iRange;
				if(oldContiID != -1 && oldContiID != m_pMap[map.x][map.y].contiID)//не нервый найденный другой континент
				{
					iResult -= iRange;
				}
				else
				{
					oldContiID = m_pMap[map.x][map.y].contiID;
				}
			}
			else
			{
				iResult -= 1 + (iRange-1)*3 - CountBoundsID(map.x, map.y, contiID, iRange-1);
			}
		}
	}

	return iResult;
}



void CFractalMap::BuildSeas()
{
	int x,y;
	for(x=0; x<WIDTH; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			if(m_pMap[x][y].contiID == -1 && m_pMap[x][y].allowed == 1 && m_pMap[x][y].barrier == 0)
			{
				m_pMap[x][y].h = m_iWater - (1 + CountBoundsID(x, y, -1, 1) + getrandom(CountBoundsID(x, y, -1, 1)))*16;
				if(m_pMap[x][y].h < 1)
					m_pMap[x][y].h = 1;
			}
		}
	}
}

void CFractalMap::GoDeeper(int iNewDepth)
{
	if(iNewDepth <= m_iDepth)
	{
		return;
	}

  do
  {
	int iNewHEIGHT, iNewWIDTH;
	iNewHEIGHT = HEIGHT * 2;
	iNewWIDTH = WIDTH * 2;

	TFRegion **pNewMap;
	pNewMap = (TFRegion **)calloc(sizeof(TFRegion *), iNewWIDTH);
	int x, y;
	for(x=0; x<iNewWIDTH; x++)
	{
		pNewMap[x] = (TFRegion *)calloc(sizeof(TFRegion), iNewHEIGHT);
		for(y=0; y<iNewHEIGHT; y++)
		{
			pNewMap[x][y].h = -1;
			pNewMap[x][y].contiID = -128;
			pNewMap[x][y].allowed = 0;
			pNewMap[x][y].barrier = 0;
		}
	}

	float dx,dy;
	dx = iNewWIDTH/WIDTH;
	dy = iNewHEIGHT/HEIGHT;
	for(x=0; x<WIDTH; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			int iNewX, iNewY;
			iNewX = x*dx;
			iNewY = y*dy;
			if(x%2 == y%2)
			{
				iNewY++;
			}
			pNewMap[iNewX][iNewY].h = m_pMap[x][y].h;
			pNewMap[iNewX][iNewY].allowed = m_pMap[x][y].allowed;
			pNewMap[iNewX][iNewY].contiID = m_pMap[x][y].contiID;
			pNewMap[iNewX][iNewY].barrier = m_pMap[x][y].barrier;

			TMapPoint map;
			int iDir;
			for(iDir=0; iDir<3; iDir++)
			{
				map = GetNeighbour(iNewX, iNewY, iDir, 0, pNewMap, iNewWIDTH, iNewHEIGHT);
				if(map.x != iNewX || map.y != iNewY)
				{
					pNewMap[map.x][map.y].allowed = m_pMap[x][y].allowed;
				}
			}
		}
	}
	for(x=0; x<iNewWIDTH; x++)
	{
		for(y=0; y<iNewHEIGHT; y++)
		{
			if(pNewMap[x][y].contiID == -128 && pNewMap[x][y].allowed == 1)
			{
				TMapPoint map;
				int iDir;
				for(iDir=0; iDir<3; iDir++)
				{
					map = GetNeighbour(x, y, iDir, 1, pNewMap, iNewWIDTH, iNewHEIGHT);
					if(pNewMap[map.x][map.y].contiID != -128 && pNewMap[map.x][map.y].allowed == 1 && pNewMap[map.x][map.y].barrier != 0)
					{
						TMapPoint counter;
						counter = GetOppositeNeighbour(x, y, iDir, 1, pNewMap, iNewWIDTH, iNewHEIGHT);
						pNewMap[x][y].barrier = pNewMap[counter.x][counter.y].barrier;
						if(pNewMap[x][y].barrier != 0)
						{
							pNewMap[x][y].h = pNewMap[counter.x][counter.y].h;
						}
						if(getrandom(4)==0)
						{
							pNewMap[x][y].barrier = pNewMap[map.x][map.y].barrier;
							if(pNewMap[x][y].barrier != 0)
							{
								pNewMap[x][y].h = pNewMap[map.x][map.y].h;
							}
						}
					}
				}
			}
		}
	}
	for(x=0; x<iNewWIDTH; x++)
	{
		for(y=0; y<iNewHEIGHT; y++)
		{
			if(pNewMap[x][y].contiID == -128 && pNewMap[x][y].allowed == 1)
			{
				TMapPoint map;
				int iDir;
				bool notBarrier = false;
				for(iDir=0; iDir<3; iDir++)
				{
					map = GetNeighbour(x, y, iDir, 1, pNewMap, iNewWIDTH, iNewHEIGHT);
					if(pNewMap[map.x][map.y].barrier == 0)
					{
						notBarrier = true;
					}
				}
				if(!notBarrier)
				{
					pNewMap[x][y].barrier = pNewMap[map.x][map.y].barrier;
					pNewMap[x][y].h = pNewMap[map.x][map.y].h;
				}
			}
		}
	}

	for(x=0; x<iNewWIDTH; x++)
	{
		for(y=0; y<iNewHEIGHT; y++)
		{
			if(pNewMap[x][y].contiID == -128 && pNewMap[x][y].allowed == 1 && pNewMap[x][y].barrier == 0)
			{
				TMapPoint map;
				int iDir;
				for(iDir=0; iDir<3; iDir++)
				{
					map = GetNeighbour(x, y, iDir, 1, pNewMap, iNewWIDTH, iNewHEIGHT);
					if(map.x != x || map.y != y)
					{
						if(pNewMap[map.x][map.y].contiID != -128)
						{
							int h1,h2;
							h1 = pNewMap[map.x][map.y].h;


							TMapPoint counter;
							counter = GetOppositeNeighbour(x, y, iDir, 1, pNewMap, iNewWIDTH, iNewHEIGHT);

							if(pNewMap[counter.x][counter.y].contiID != -128)
							{
								h2 = pNewMap[counter.x][counter.y].h;
							}
							else
							{
								h2 = h1;
							}

//							pNewMap[x][y].h = (2*h1 + h2)/3;
//							pNewMap[x][y].h = (3*h1 + h2)/4;
							if(getrandom(2) == 1)
								pNewMap[x][y].h = (3*h2 + h1)/4;
							else
								pNewMap[x][y].h = (3*h1 + h2)/4;
//							pNewMap[x][y].allowed = 1;
//							if(getrandom(2) == 1)
							{
								pNewMap[x][y].h += getrandom(32) - 16;
//								pNewMap[x][y].h += getrandom(32) - 16;
//								pNewMap[x][y].h += getrandom(16) - 8;
							}
							if(pNewMap[map.x][map.y].contiID > pNewMap[counter.x][counter.y].contiID)
							{
								pNewMap[x][y].contiID = pNewMap[map.x][map.y].contiID;
							}
							else
							{
								pNewMap[x][y].contiID = pNewMap[counter.x][counter.y].contiID;
							}
						}
					}
				}
			}
		}
	}
	for(x=0; x<iNewWIDTH; x++)
	{
		for(y=0; y<iNewHEIGHT; y++)
		{
			if(pNewMap[x][y].contiID == -128)
				pNewMap[x][y].contiID = -1;
		}
	}

	Clear();
	m_pMap = pNewMap;
	HEIGHT = iNewHEIGHT;
	WIDTH = iNewWIDTH;
	m_iDepth++;
  }
  while(iNewDepth > m_iDepth);

}
void CFractalMap::Smooth()
{
	int x,y;
	for(x=0; x<WIDTH; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			if(m_pMap[x][y].barrier == 0 && m_pMap[x][y].allowed == 1)
			{
				TMapPoint map;

				int iDir;
				iDir = getrandom(6);

				map = GetNeighbour(x, y, iDir, 1);
				if(map.x != x || map.y != y)
				{
					m_pMap[x][y].h = (3*m_pMap[x][y].h + m_pMap[map.x][map.y].h)/4;
					m_pMap[x][y].h += getrandom(16) - 8;

					if(m_pMap[x][y].h < 1)
					{
						m_pMap[x][y].h = 1;
					}
				}
			}
		}
	}
}

CFractalMap::~CFractalMap()
{
	Clear();
}

void CFractalMap::CreateWorld(int Width, int Height, int WaterPercent, int ContinentsCount, int Smoothness, int BarrierHeight)
{
	Clear();

	int iTotalLand;
	int iArchipelagos;


	m_iDepth = 4;
	do
	{
		m_iDepth++;
		HEIGHT = pow(2,m_iDepth);
		WIDTH = HEIGHT*3;
		iTotalLand = WIDTH*HEIGHT*(100-WaterPercent)/100;
		if(BarrierHeight >= 0)
		{
			WIDTH = HEIGHT*2;
			iTotalLand = M_PI*(WIDTH/2-2)*(HEIGHT-2)*(100-WaterPercent)/400;
		}
	}
	while(iTotalLand < ContinentsCount*3);

	iArchipelagos = iTotalLand*0.25;
	iTotalLand = iTotalLand*0.75;

	m_pMap = (TFRegion**)calloc(sizeof(TFRegion *), WIDTH);
	int x, y;
	for(x=0; x<WIDTH; x++)
	{
		m_pMap[x] = (TFRegion *)calloc(sizeof(TFRegion), HEIGHT);
		for(y=0; y<HEIGHT; y++)
		{
			m_pMap[x][y].h = 0;
			m_pMap[x][y].allowed = 1;
			m_pMap[x][y].contiID = -1;
			m_pMap[x][y].barrier = 0;
			if(BarrierHeight >= 0)
			{
				float a,b;
				a = (float)(x-WIDTH/2)*(x-WIDTH/2)/((WIDTH/4-1)*(WIDTH/4-1));
				b = (float)(y-HEIGHT/2)*(y-HEIGHT/2)/((HEIGHT/2-1)*(HEIGHT/2-1));
				if(a+b > 1.0 || (a+b>0.7 && getrandom(2)==0))
				{
					m_pMap[x][y].h = BarrierHeight;
					m_pMap[x][y].barrier = 1;
				}
			}
		}
	}

	int iSize;
	iSize = iTotalLand/ContinentsCount;

	if(iSize<1)
	{
		iSize = 1;
	}

	m_iWater = 128;

	int i;
	for(i=0; i<ContinentsCount; i++)
	{
		int iRealSize;
		iRealSize = iSize/2 + getrandom(iSize);
		if(iRealSize > iTotalLand - ContinentsCount + i)
		{
			iRealSize = iTotalLand - ContinentsCount + i;
		}
		if(iRealSize < 1)
		{
			iRealSize = 1;
		}
		AddContinent(iRealSize, i);
		iTotalLand -= iRealSize;
	}
	int iIslandsCount;
	iIslandsCount = ContinentsCount*3;
	if(iIslandsCount > iArchipelagos)
	{
		iIslandsCount = 1 + getrandom(iArchipelagos);
	}
	iSize = iArchipelagos/iIslandsCount;
	for(i=0; i<iIslandsCount; i++)
	{
		int iRealSize;
		iRealSize = iSize/2 + getrandom(iSize);
		if(iRealSize > iArchipelagos - iIslandsCount + i)
		{
			iRealSize = iArchipelagos - iIslandsCount + i;
		}
		if(iRealSize < 1)
		{
			iRealSize = 1;
		}
		AddContinent(iRealSize, i);
		iArchipelagos -= iRealSize;
	}
	BuildSeas();

	int dim;
	if(Width>Height)
	{
		dim = Width;
	}
	else
	{
		dim = Height;
	}

	int depth;
	depth = Log2(dim);

	if(pow(2,depth) < dim)
	{
		depth++;
	}

	GoDeeper(depth);

	iWidth = Width;
	iHeight = Height;

	for(i=0; i<Smoothness*Smoothness; i++)
	{
		Smooth();
	}
}

int CFractalMap::GetHeight(int iX, int iY)
{
	float dx,dy;
	dx = (float)WIDTH/iWidth;
	dy = (float)HEIGHT/iHeight;

	int iNewX, iNewY;
	iNewX = iX*dx;
	iNewY = iY*dy;

	return m_pMap[iNewX][iNewY].h;
}

void CFractalMap::CreateSphere(int Width, int Height, int WaterPercent, int ContinentsCount, int Smoothness)
{
	Clear();

	m_iDepth = 2;//4;//3;
	HEIGHT = 3*pow(2,m_iDepth);
	WIDTH = 10*pow(2,m_iDepth);//HEIGHT*5;

	m_pMap = (TFRegion**)calloc(sizeof(TFRegion *), WIDTH);
	int x, y;
	for(x=0; x<WIDTH; x++)
	{
		m_pMap[x] = (TFRegion *)calloc(sizeof(TFRegion), HEIGHT);
		for(y=0; y<HEIGHT; y++)
		{
			m_pMap[x][y].h = 0;
			m_pMap[x][y].allowed = 1;
			m_pMap[x][y].contiID = -1;
			if(y<HEIGHT/3)
			{
				int xx;
				xx = x%(WIDTH/5);
				if(xx < WIDTH/10 - y || xx > WIDTH/10 + y)
				{
					m_pMap[x][y].allowed = 0;
					m_pMap[x][y].h = -1000;
				}
				else
				{
					m_pMap[x][y].allowed = 1;
				}
			}
			if(y>=2*HEIGHT/3)
			{
				int xx,yy;
				xx = x%(WIDTH/5);
				yy = y - 2*HEIGHT/3;
				if(xx < WIDTH/10 - yy || xx > WIDTH/10 + yy)
				{
					m_pMap[x][y].allowed = 1;
				}
				else
				{
					m_pMap[x][y].allowed = 0;
					m_pMap[x][y].h = -1000;
				}
			}
		}
	}

	int iTotalLand;
	iTotalLand = WIDTH*HEIGHT*(100-WaterPercent)*2/300;
	int iArchipelagos;
	iArchipelagos = iTotalLand*0.25;
	iTotalLand = iTotalLand*0.75;
	int iSize;
	iSize = iTotalLand/ContinentsCount;

	if(iSize<1)
	{
		iSize = 1;
	}

	m_iWater = 128;

	int i;
	for(i=0; i<ContinentsCount; i++)
	{
		int iRealSize;
		iRealSize = iSize/2 + getrandom(iSize);
		if(iRealSize > iTotalLand - ContinentsCount + i)
		{
			iRealSize = iTotalLand - ContinentsCount + i;
		}
		if(iRealSize < 1)
		{
			iRealSize = 1;
		}
		AddContinent(iRealSize, i);
		iTotalLand -= iRealSize;
	}
	int iIslandsCount;
	iIslandsCount = ContinentsCount*3;
	if(iIslandsCount > iArchipelagos)
	{
		iIslandsCount = 1 + getrandom(iArchipelagos);
	}
	iSize = iArchipelagos/iIslandsCount;
	for(i=0; i<iIslandsCount; i++)
	{
		int iRealSize;
		iRealSize = iSize/2 + getrandom(iSize);
		if(iRealSize > iArchipelagos - iIslandsCount + i)
		{
			iRealSize = iArchipelagos - iIslandsCount + i;
		}
		if(iRealSize < 1)
		{
			iRealSize = 1;
		}
		AddContinent(iRealSize, i);
		iArchipelagos -= iRealSize;
	}
	BuildSeas();

	int dim;
	if(Width/10>Height/3)
	{
		dim = Width/10;
	}
	else
	{
		dim = Height/3;
	}

	int depth;
	depth = Log2(dim);

    if(depth < m_iDepth)
    {
        depth = m_iDepth;
    }

	if(pow(2,depth) < dim)
	{
		depth++;
	}

	GoDeeper(depth);

	iWidth = Width;
	iHeight = Height;

	for(i=0; i<Smoothness*Smoothness; i++)
	{
		Smooth();
	}
}

TMapPoint CFractalMap::GetNeighbour(int x, int y, int dir, int allowed, TFRegion **pNewMap, int iNewWIDTH, int iNewHEIGHT, int onlyExists)
{
	if(pNewMap == NULL)
	{
		pNewMap = m_pMap;
		iNewHEIGHT = HEIGHT;
		iNewWIDTH = WIDTH;
	}

	if(onlyExists = 1)
	{
		if(pNewMap[x][y].allowed == 0)
		{
			return TMapPoint(x,y);
		}
	}

	while(dir >= 6) dir-=6;
	while(dir < 0) dir+=6;

	int tdir;
	if(dir>2)
		tdir = 5-dir;
	else
		tdir = dir;

	int iNewX, iNewY;

	if(x%2 != y%2)
	{
		iNewX = x + aDir1[tdir].x;
		iNewY = y + aDir1[tdir].y;
	}
	else
	{
		iNewX = x + aDir2[tdir].x;
		iNewY = y + aDir2[tdir].y;
	}

	while(iNewX >= iNewWIDTH)
	{
		iNewX -= iNewWIDTH;
	}
	while(iNewX < 0)
	{
		iNewX += iNewWIDTH;
	}

	if(onlyExists = 1)
	{
		if(iNewY >= iNewHEIGHT)
		{
			iNewY = iNewHEIGHT-1;
		}
		if(iNewY < 0)
		{
			iNewY = 0;
		}
	}

	if(iNewY < iNewHEIGHT && iNewY >= 0)
	{
		while(pNewMap[iNewX][iNewY].allowed != allowed)
		{
			if(iNewX%2 != iNewY%2)
			{
				iNewX = iNewX + aDir1[tdir].x;
			}
			else
			{
				iNewX = iNewX + aDir2[tdir].x;
			}

			while(iNewX >= iNewWIDTH)
			{
				iNewX -= iNewWIDTH;
			}
			while(iNewX < 0)
			{
				iNewX += iNewWIDTH;
			}
		}
	}

	return TMapPoint(iNewX,iNewY);
}

bool CFractalMap::IsAllowed(int x, int y)
{
	float dx,dy;
	dx = (float)WIDTH/iWidth;
	dy = (float)HEIGHT/iHeight;

	int iNewX, iNewY;
	iNewX = x*dx;
	iNewY = y*dy;

	return m_pMap[iNewX][iNewY].allowed;
}

TMapPoint CFractalMap::GetOppositeNeighbour(int x, int y, int dir, int allowed, TFRegion ** pNewMap, int iNewWIDTH, int iNewHEIGHT)
{
	TMapPoint counter[4];

	int cnt;
	if(x%2 != y%2)
	{
		cnt = dir*2-1;
	}
	else
	{
		cnt = 6-dir*2;
	}
	cnt = cnt+2;

	counter[0] = GetNeighbour(x, y, cnt, 1, pNewMap, iNewWIDTH, iNewHEIGHT, 0);
	if((abs(counter[0].x-x)>1 && abs(counter[0].x-x)<iNewWIDTH-1) || abs(counter[0].y-y)>1)
	{
		if((counter[0].y > iNewHEIGHT/2) == (counter[0].x > x))
			cnt--;
		else
			cnt++;
	}

	cnt++;
	counter[1] = GetNeighbour(counter[0].x, counter[0].y, cnt, 1, pNewMap, iNewWIDTH, iNewHEIGHT, 0);
	if((abs(counter[1].x-counter[0].x)>1 && abs(counter[1].x-counter[0].x)<iNewWIDTH-1) || abs(counter[1].y-counter[0].y)>1)
	{
		if((counter[1].y > iNewHEIGHT/2) == (counter[1].x > counter[0].x))
			cnt--;
		else
			cnt++;
	}

	cnt++;
	counter[2] = GetNeighbour(counter[1].x, counter[1].y, cnt, 1, pNewMap, iNewWIDTH, iNewHEIGHT, 0);
	if((abs(counter[2].x-counter[1].x)>1 && abs(counter[2].x-counter[1].x)<iNewWIDTH-1) || abs(counter[2].y-counter[1].y)>1)
	{
		if((counter[2].y > iNewHEIGHT/2) == (counter[2].x > counter[1].x))
			cnt--;
		else
			cnt++;
	}

	cnt--;
	counter[3] = GetNeighbour(counter[2].x, counter[2].y, cnt, 1, pNewMap, iNewWIDTH, iNewHEIGHT);

	return counter[3];
}

int CFractalMap::GetContiID(int iX, int iY)
{
	float dx,dy;
	dx = (float)WIDTH/iWidth;
	dy = (float)HEIGHT/iHeight;

	int iNewX, iNewY;
	iNewX = iX*dx;
	iNewY = iY*dy;

	return m_pMap[iNewX][iNewY].contiID;
}
