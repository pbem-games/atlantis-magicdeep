//---------------------------------------------------------------------------

//#include <vcl.h>
//#pragma hdrstop

#include "ARMGen.h"
#include "CFractalMap.h"
#include "NameGen.h"
#include "support.h"
#include "gameio.h"
#include <math.h>
//#include <vector.h>
//#include <mfc/afxtempl.h>
//#pragma package(smart_init)
//---------------------------------------------------------------------------
//const WIDTH = 128;
//const HEIGHT = 128;

int aHumidity[rtCOUNT][2] =
{
	{0,10},//rtUNKNOWN,
	{3,10},//rtBARRIERWATER,
	{2,10},//rtBARRIERMOUNTAIN,
	{3,10},//5,//rtDEEPWATER,
	{4,10},//5,//4,//rtSHALLOW,
	{6,10},//5,//rtRIVER,
	{6,10},//5,//rtLAKE,
	{2,10},//rtMONTANA,
	{3,10},//rtHILLS
	{0,10},//rtVOLKANO,
	{3,10},//rtPLAIN,
	{4,2},//rtFOREST,
	{5,3},//rtJUNGLE,
	{8,2},//4,//rtSWAMP,
	{0,2},//rtDESERT
	{2,10},//rtTUNDRA,
	{0,1}};//rtWASTELANDS


int aTemperature[rtCOUNT][2] =
{
	{0,10},//rtUNKNOWN,
	{0,10},//rtBARRIERWATER,
	{0,10},//rtBARRIERMOUNTAIN,
	{0,10},//5,//rtDEEPWATER,
	{0,10},//5,//4,//rtSHALLOW,
	{0,10},//5,//rtRIVER,
	{2,10},//5,//rtLAKE,
	{0,10},//rtMONTANA,
	{0,10},//rtHILLS
	{0,10},//rtVOLKANO,
	{0,10},//rtPLAIN,
	{2,4},//rtFOREST,
	{6,9},//rtJUNGLE,
	{2,6},//4,//rtSWAMP,
	{4,8},//rtDESERT
	{0,1},//rtTUNDRA,
	{0,3}};//rtWASTELANDS

ARMGenerator::ARMGenerator(int xSize, int ySize)
{
//    Randomize();
//	RandSeed = Time();

	m_pMap = NULL;
	m_pUnderMap = NULL;
	m_pSeasLocation = NULL;
	WIDTH = xSize;
	HEIGHT = ySize;
	bRiversEnabled = true;
	bAdvancedMap = true;
	iMaxHeight = 1;
	iWaterPercent = 40;
	watercount = WIDTH*HEIGHT/2;
	m_iClimate = ctCLASSIC;
//	m_iClimate = ctETERNALMIDNIGHT;

	if(m_pMap == NULL)
	{
		m_pMap = (TARMRegion **)calloc(WIDTH, sizeof(TARMRegion *));
		for(int i=0; i<WIDTH; i++)
		{
			m_pMap[i] = (TARMRegion *)calloc(HEIGHT, sizeof(TARMRegion));
			for(int j=0; j<HEIGHT; j++)
			{
				m_pMap[i][j].h = 0;
				m_pMap[i][j].type = rtDEEPWATER;
				m_pMap[i][j].count = -1;
				m_pMap[i][j].nameID = -1;
				m_pMap[i][j].contiID = -1;
				m_pMap[i][j].iHumidity = 0;
				m_pMap[i][j].iTemperature = 0;
				m_pMap[i][j].island = false;
				m_pMap[i][j].riverID = -1;
				m_pMap[i][j].riverNameID = -1;
				m_pMap[i][j].bRareResource = false;
				m_pMap[i][j].iCitySize = 0;
				int k;
				for(k=0; k<NUMMAN; k++)
				{
					m_pMap[i][j].aPop[k] = 0;
				}
			}
		}
	}
	if(m_pUnderMap == NULL)
	{
		m_pUnderMap = (TARMRegion **)calloc(WIDTH/2, sizeof(TARMRegion *));
		for(int i=0; i<WIDTH/2; i++)
		{
			m_pUnderMap[i] = (TARMRegion *)calloc(HEIGHT/2, sizeof(TARMRegion));
			for(int j=0; j<HEIGHT/2; j++)
			{
				m_pUnderMap[i][j].h = 0;
				m_pUnderMap[i][j].type = rtSOLIDROCK;
				m_pUnderMap[i][j].count = -1;
				m_pUnderMap[i][j].nameID = -1;
				m_pUnderMap[i][j].contiID = -1;
				m_pUnderMap[i][j].iHumidity = 0;
				m_pUnderMap[i][j].iTemperature = 0;
				m_pUnderMap[i][j].island = false;
				m_pUnderMap[i][j].riverID = -1;
				m_pUnderMap[i][j].riverNameID = -1;
				m_pUnderMap[i][j].bRareResource = false;
				m_pUnderMap[i][j].iCitySize = 0;
				int k;
				for(k=0; k<NUMMAN; k++)
				{
					m_pUnderMap[i][j].aPop[k] = 0;
				}
			}
		}
	}

	m_pSeasLocation = NULL;
	m_pProvinceInfo = NULL;
	water = 10;
}
//---------------------------------------------------------------------------

int ARMGenerator::RaiseRegion(int x, int y, int type)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	if(m_pMap[x][y].type == rtBARRIERWATER || m_pMap[x][y].type == rtBARRIERMOUNTAIN)
	{
		return 0;
	}

	if(m_pMap[x][y].h < water)
	{
		type = type/2;
	}

	int xx,yy;

	int start = getrandom(3);
	for(int i=start; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[x][y].h - m_pMap[xx][yy].h > type)
			{
//				if(getrandom(type) == 0)
//				if(getrandom((m_pMap[x][y].h - m_pMap[xx][yy].h - type)/4) != 0)
					return RaiseRegion(xx,yy,type);
			}
		}
	}
	int oldH = m_pMap[x][y].h;
	m_pMap[x][y].h++;
//	m_pMap[x][y].h += type;

	if(oldH < water && m_pMap[x][y].h >= water)
		bulk = true;

	return m_pMap[x][y].h;
}

int ARMGenerator::GetHeight(int x, int y)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y>=0 && y<HEIGHT)
		return m_pMap[x][y].h;
	else
		return 0;
}

void ARMGenerator::PlainMap()
{
	if(m_pMap == NULL)
	{
		return;
	}

	int x,y;
	float rad,teta,eps;
	int type;

	type = 1 + getrandom(4);

	x = getrandom(WIDTH);
	do
	{
		y = getrandom(HEIGHT);
	}
	while(x%2 != y%2);

	rad = 5 + getrandom(40);
	teta = (float)getrandom(360)*2.0*M_PI/360.0;
	eps = (float)getrandom(100)/100.0;

	for(int k=0; k<M_PI*rad*rad; k++)
	{
		float angle;
		angle = (float)getrandom(360)*2.0*M_PI/360.0;
		float r = getrandom(rad/(1 + eps*cos(teta+angle)));

		int xx,yy;
		xx = x + r*cos(angle);
		yy = y + r*sin(angle);

		if(xx%2 != yy%2)
			yy++;

		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			PlainRegion(xx,yy,type);
		}
	}
}

void ARMGenerator::PlainRegion(int x, int y, int type)
{
	if(m_pMap == NULL)
	{
		return;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return;
	}

	if(m_pMap[x][y].h <= water)
	{
		return;
	}

	int xx,yy;

	for(int i=0; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[x][y].h > m_pMap[xx][yy].h + type)
			{
				m_pMap[x][y].h--;
				RaiseRegion(xx,yy,type);
			}
		}
	}
}

int ARMGenerator::GetMaxDelta(int x, int y)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	int xx,yy;
	int delta;

	delta = 0;

	for(int i=0; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			int val;
			if(m_pMap[xx][yy].h < water)
			{
//				val = water;
				val = (m_pMap[xx][yy].h + water)/2;
//				val = m_pMap[xx][yy].h;// - (water - m_pMap[xx][yy].h);
			}
			else
			{
				val = m_pMap[xx][yy].h;
			}
			if(m_pMap[xx][yy].type == rtBARRIERMOUNTAIN)
			{
				val = val*20;
			}
			if(abs(m_pMap[x][y].h - val) > delta)
			{
				delta = abs(m_pMap[x][y].h - val);
			}
		}
	}
	return delta;
}

void ARMGenerator::RandomizeMap()
{
	if(m_pMap == NULL)
	{
		return;
	}

	int k;
	for(k=0; k<WIDTH*HEIGHT; k++)
//	for(int j=0; j<HEIGHT; j++)
//	{
//		for(int i=0; i<WIDTH; i++)
		{
			int i,j;
			i = getrandom(WIDTH);
			j = getrandom(HEIGHT);
			if(i % 2 == j % 2)
			{
/*
			if(GetMaxDelta(i,j)<5)
			{
				if(getrandom(6)==0)
				{
					m_pMap[i][j].h++;
				}
				else
				{
					if(getrandom(5)==0)
					{
						m_pMap[i][j].h--;
					}
				}
			}
*/
				if(m_pMap[i][j].h == water)
				{
					m_pMap[i][j].h = water+1;
				}

				int ocean = HasInRange(i,j,rtDEEPWATER,1) + HasInRange(i,j,rtSHALLOW,1) + HasInRange(i,j,rtBARRIERWATER,1);
				if(ocean>0 && ocean != 6)
				{
//					if(getrandom(GetMaxDelta(i,j)) < 4)
					if(ocean == 2 && m_pMap[i][j].h >= water)
					{
//						m_pMap[i][j].h += 1 - getrandom(3);
						m_pMap[i][j].h = water + 0 - getrandom(2);
						if(m_pMap[i][j].h >= water)
						{
							m_pMap[i][j].type = rtPLAIN;
						}
						else
						{
							m_pMap[i][j].type = rtSHALLOW;
						}
					}
				}
				if(ocean >= 4 && m_pMap[i][j].h >= water+2)
				{
					m_pMap[i][j].h = water+1+getrandom(2);
				}

				if(ocean == 6 && m_pMap[i][j].h >= water)
				{
					if(getrandom(3)==0)
					{
						m_pMap[i][j].h = water-1;
					}
					for(int k=0; k<6; k++)
					{
						if(getrandom(3)!=0)
						{
							int xx,yy;

							xx = i + rround[k].x;
							yy = j + rround[k].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								m_pMap[xx][yy].h = water+getrandom(2);
							}
						}
					}
				}
			}
		}
//	}
}

void ARMGenerator::SetTypes(bool fast)
{
	if(m_pMap == NULL)
	{
		return;
	}

	if(m_pProvinceInfo)
	{
		delete m_pProvinceInfo;
	}
	m_pProvinceInfo = new ARMProvinceInfoTable();

	if(m_pSeasLocation)
		free(m_pSeasLocation);
	m_pSeasLocation = NULL;
	maxID = 0;

	watercount = 0;
	plaincount = 0;
	montanacount = 0;

	swampcount = 0;
	forestcount = 0;
	junglecount = 0;
	desertcount = 0;
	tundracount = 0;

	int i,j,k;
	for(i=0; i<WIDTH; i++)
		for(j=0; j<HEIGHT; j++)
		{
			if(i % 2 == j % 2)
			{
				m_pMap[i][j].nameID = -1;

				if(m_pMap[i][j].h<water)
				{
					if(m_pMap[i][j].type != rtBARRIERWATER)
					{
						if(m_pMap[i][j].h<water-2 || !bAdvancedMap)
							m_pMap[i][j].type = rtDEEPWATER;
						else
							m_pMap[i][j].type = rtSHALLOW;

						watercount++;
					}
				}
				else
				{
					if(m_pMap[i][j].type != rtBARRIERMOUNTAIN && m_pMap[i][j].type != rtBARRIERWATER)
					{
						m_pMap[i][j].type = rtPLAIN;
					}
				}
			}
		}

//return;

	for(i=0; i<WIDTH; i++)
		for(j=0; j<HEIGHT; j++)
		{
			if(i % 2 == j % 2)
			{
				if(CanBeMontana(i, j, true))
				{
					m_pMap[i][j].type = rtMONTANA;
					m_pMap[i][j].nameID = maxID++;
//					montanacount += GrowMontana(i, j, 30);
					int newArea;
					newArea = GrowElement(rtMONTANA, i, j, 20);//+1;
//					if(newArea > 1)
					{
						montanacount += newArea;
					}
//					else
//					{
//						m_pMap[i][j].type = rtPLAIN;
//						m_pMap[i][j].nameID = -1;
//					}
				}
			}
		}

	if(bAdvancedMap)
	{
		for(i=0; i<WIDTH; i++)
			for(j=0; j<HEIGHT; j++)
			{
				if(i % 2 == j % 2)
				{
					if(CanBeHill(i, j, true))
					{
						m_pMap[i][j].type = rtHILLS;
						m_pMap[i][j].nameID = maxID++;
						int newArea;
						newArea = GrowElement(rtHILLS, i, j, 40);//+1;
						if(newArea > 1)
						{
							montanacount += newArea;
						}
						else
						{
							m_pMap[i][j].type = rtPLAIN;
							m_pMap[i][j].nameID = -1;
						}
					}
				}
			}
	}

	for(i=0; i<WIDTH; i++)
		for(j=0; j<HEIGHT; j++)
		{
			if(i % 2 == j % 2)
			{
				if(m_pMap[i][j].type == rtPLAIN)
				{
					plaincount++;
				}
			}
		}

	if(fast)
	{
		return;
	}

	if(bAdvancedMap)
	{
		FixShallows();
	}

	if(bRiversEnabled)
	{
		int length = 0;
		int cou = 0;
		while(length < m_iMoisure*m_iMoisure*plaincount/8000 && cou<HEIGHT*WIDTH/2) //8
		{
			length += AddElementAtRandom(rtLAKE, 3);
			cou++;
		}

		length = 0;
		cou = 0;
		while(length < m_iMoisure*m_iMoisure*plaincount/(HEIGHT*5) && cou<HEIGHT*WIDTH/2) //8
		{
			i = getrandom(WIDTH);
			j = getrandom(HEIGHT);
			if(i % 2 == j % 2)// && length < 3*plaincount/HEIGHT)
			{
				if(m_pMap[i][j].h > water && m_pMap[i][j].type != rtRIVER && m_pMap[i][j].riverID == -1 && m_pMap[i][j].type != rtBARRIERMOUNTAIN && (GetMaxDelta(i,j)>3 || m_pMap[i][j].h>9))
//				if(m_pMap[i][j].h > water && m_pMap[i][j].type != rtRIVER && m_pMap[i][j].type != rtBARRIERMOUNTAIN && (GetMaxDelta(i,j)>3 || m_pMap[i][j].h>9))
				length += AddRiver(i,j);
				cou++;
			}
		}
	}

	for(j=0; j<HEIGHT; j++)
		for(i=0; i<WIDTH; i++)
		{
			if(i % 2 == j % 2)
			{
				if(m_pMap[i][j].type == rtMONTANA && HasInRange(i,j,rtMONTANA,2) > 8)//12)
				{
					if(HasInRange(i,j,rtVOLKANO,2) < 1 && getrandom(6)==0)
//					if(HasInRange(i,j,rtVOLKANO,4+getrandom(2)) < 1 && getrandom(2)==0)
					{
						m_pMap[i][j].type = rtVOLKANO;
					}
				}
			}
		}

	CalculateHumidity();
	CalculateTemperature();

	for(k=0; k<HEIGHT*WIDTH; k++)
	{
		AddElementAtRandom(rtSWAMP, m_iMoisure*getrandom(10));
	}
//	FixSwamps();

//	for(k=0; k<HEIGHT*WIDTH; k++)
	for(k=0; k<HEIGHT*(0.5+(float)(10-m_iMoisure)/2.0); k++)
	{
		AddElementAtRandom(rtDESERT, 5*m_iT+getrandom(m_iT*m_iT));
//		AddElementAtRandom(rtDESERT, 3*m_iT+getrandom(4*m_iT));
//		AddElementAtRandom(rtWASTELANDS, HEIGHT/10+getrandom(HEIGHT/6));
	}
	FixDeserts();

	for(k=0; k<HEIGHT*WIDTH; k++)
	{
		AddElementAtRandom(rtJUNGLE, 8+getrandom(20));
	}
	for(k=0; k<HEIGHT*WIDTH; k++)
	{
		AddElementAtRandom(rtFOREST, 10+getrandom(30));
	}
	FixForestnJungle();

	for(k=0; k<HEIGHT*WIDTH; k++)
	{
		AddElementAtRandom(rtTUNDRA, 10+getrandom(30));
	}

	for(i=0; i<WIDTH; i++)
		for(j=0; j<HEIGHT; j++)
		{
			if(i % 2 == j % 2)
			{
				if(m_pMap[i][j].type == rtPLAIN && m_pMap[i][j].nameID == -1)
				{
					m_pMap[i][j].nameID = maxID++;
					GrowElement(rtPLAIN, i, j, 30);
				}
			}
		}

	for(j=0; j<HEIGHT; j++)
		for(i=0; i<WIDTH; i++)
		{
			if(i % 2 == j % 2)
			{
				if(m_pMap[i][j].type == rtFOREST)
				{
					forestcount++;
				}
				if(m_pMap[i][j].type == rtJUNGLE)
				{
					junglecount++;
				}
				if(m_pMap[i][j].type == rtSWAMP)
				{
					swampcount++;
				}
				if(m_pMap[i][j].type == rtDESERT)
				{
					desertcount++;
				}
				if(m_pMap[i][j].type == rtTUNDRA)
				{
					tundracount++;
				}
			}
		}

	FormateSeasNames();
	MergeProvinces();
}

void ARMGenerator::AddHeight(int r1, int r2, int type)
{
	if(m_pMap == NULL)
	{
		return;
	}

	int x,y;
	float rad,teta;

	x = getrandom(WIDTH);
	do
	{
		y = getrandom(HEIGHT);
	}
	while(x%2 != y%2);

	float rr = r2;

	rad = r1;
	teta = (float)getrandom(360)*2.0*M_PI/360.0;

	int max = 6*M_PI*rad*rad;

	float eps;
	if(getrandom(2)==0)
		eps = (M_PI/6.0)/max;
	else
		eps = -(M_PI/6.0)/max;

	for(int k=0; k<max; k++)
	{
		if(getrandom(max/2)==0)
		{
			if(getrandom(2)==0)
				eps = (M_PI/18.0)/max;
			else
				eps = -(M_PI/18.0)/max;
		}
		teta = teta+eps*getrandom(12);

		float angle;
		angle = (float)getrandom(360)*2.0*M_PI/360.0;
		float r = getrandom(rad);

		int xx,yy;
		xx = x + (rr*k/max)*cos(teta) + r*cos(angle);
		yy = y + (rr*k/max)*sin(teta) + r*sin(angle);

		if(xx%2 != yy%2)
			yy++;

		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			int hc;
			for(hc=0; hc<type; hc++)
			{
				int h;
				h = RaiseRegion(xx,yy,(9.0-r)*type/9.0 + getrandom(r*type/9.0));
				if(h > iMaxHeight)
				{
//					iWaterPercent = 100.0*water/iMaxHeight;
					iMaxHeight = h;
//					water = iMaxHeight*iWaterPercent/100.0;
				}
			}
		}
	}
}

int ARMGenerator::HasInRange(int x, int y, RegionType type, int range, bool start)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

/*	if(start)
		FillCount(x,y,-1,range);
*/

	int iResult=0;

//******************************************************************************
	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;
	int r = 0;

	for(r=1; r<range+1; r++)
//	while(iRIndex > 0)
	{
//		r++;
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;

						if(m_pMap[xx][yy].type == type || (type == rtRIVER && m_pMap[xx][yy].riverID != -1))// && m_pMap[xx][yy].count == -1)
						{
//							return true;
							iResult++;
						}

						newList[iRIndex].x = xx;
						newList[iRIndex].y = yy;
						iRIndex++;
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;

//******************************************************************************
	return iResult;
}

int ARMGenerator::IsExtremum(int x, int y)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	int xx,yy;
	int delta;

	delta = -255;

	for(int i=0; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			int newDelta = m_pMap[x][y].h - m_pMap[xx][yy].h;
			if(newDelta != 0)
			{
				if(delta == -255)
				{
					delta = newDelta;
				}
			else
				{
					if(delta*newDelta < 0)
					{
						delta = 0;
					}
				}
			}
		}
	}
	if(delta == -255)
	{
		delta = 0;
	}
	if(delta > 0)
	{
		delta = 1;
	}
	if(delta < 0)
	{
		delta = -1;
	}

	return delta;
}

void ARMGenerator::FillCount(int x, int y, int value, int range)
{
	if(m_pMap == NULL)
	{
		return;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return;
	}

	m_pMap[x][y].count = value;

	if(range <= 0)
		return;

	int xx,yy;

	for(int i=0; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			FillCount(xx, yy, value, range-1);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
int ARMGenerator::CountTheSame(int x, int y, bool start)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	int iResult=0;

//******************************************************************************
	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;
	int r = 0;

//	for(int r=1; r<range; r++)
	while(iRIndex > 0)
	{
		r++;
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;

						bool equal;
						equal = false;
						if(m_pMap[xx][yy].type == m_pMap[x][y].type)
							equal = true;
						if((m_pMap[xx][yy].type == rtMONTANA || m_pMap[xx][yy].type == rtHILLS) &&
							(m_pMap[x][y].type == rtMONTANA || m_pMap[x][y].type == rtHILLS))
							equal = true;
						if(equal)
						{
							iResult++;

							newList[iRIndex].x = xx;
							newList[iRIndex].y = yy;
							iRIndex++;
						}
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;

//******************************************************************************
	return iResult;
}

void ARMGenerator::Clear()
{
	if(m_pMap == NULL)
	{
		return;
	}
	m_iProgress = 0;

	int startPos = m_iProgress;

	int i,j;
	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			m_pMap[i][j].h = 0;
			m_pMap[i][j].type = rtDEEPWATER;
			m_pMap[i][j].count = -1;
			m_pMap[i][j].nameID = -1;
			m_pMap[i][j].contiID = -1;
			m_pMap[i][j].iHumidity = 0;
			m_pMap[i][j].island = false;
			m_pMap[i][j].riverID = -1;
			m_pMap[i][j].riverNameID = -1;
			m_pMap[i][j].bRareResource = false;
			m_pMap[i][j].iCitySize = 0;
			int k;
			for(k=0; k<NUMMAN; k++)
			{
				m_pMap[i][j].aPop[k] = 0;
			}
		}
		if(i>(m_iProgress-startPos)*WIDTH/10.0)
		{
			m_iProgress++;
		}
	}

	for(i=0; i<WIDTH/2; i++)
	{
		for(j=0; j<HEIGHT/2; j++)
		{
			m_pUnderMap[i][j].h = 0;
			m_pUnderMap[i][j].type = rtDEEPWATER;
			m_pUnderMap[i][j].count = -1;
			m_pUnderMap[i][j].nameID = -1;
			m_pUnderMap[i][j].contiID = -1;
			m_pUnderMap[i][j].iHumidity = 0;
			m_pUnderMap[i][j].island = false;
			m_pUnderMap[i][j].riverID = -1;
			m_pUnderMap[i][j].riverNameID = -1;
			m_pUnderMap[i][j].bRareResource = false;
			m_pUnderMap[i][j].iCitySize = 0;
			int k;
			for(k=0; k<NUMMAN; k++)
			{
				m_pUnderMap[i][j].aPop[k] = 0;
			}
		}
	}

	if(m_pProvinceInfo)
	{
		delete m_pProvinceInfo;
		m_pProvinceInfo = NULL;
	}

	if(m_pSeasLocation)
		free(m_pSeasLocation);
	m_pSeasLocation = NULL;

	iMaxHeight = 1;
	iWaterPercent = 40;
	watercount = WIDTH*HEIGHT/2;
	maxID = 0;
	water = 10;
}

void ARMGenerator::BalanceMap(float fNeedWater, float fNeedMontanas)
{
	if(m_pMap == NULL)
	{
		return;
	}

	int iNeedWater;
	int iNeedMontanas;

	SetTypes(true);

	iNeedWater = (watercount+plaincount+montanacount)*fNeedWater;
	iNeedMontanas = (watercount+plaincount+montanacount)*fNeedMontanas;

	while(watercount > iNeedWater)
	{
		water--;
		iWaterPercent = 100.0*water/iMaxHeight;
		SetTypes(true);
	}
	m_iProgress += 2;
	while(watercount < iNeedWater)
	{
		water++;
		iWaterPercent = 100.0*water/iMaxHeight;
		SetTypes(true);
	}
	m_iProgress += 2;

	while(montanacount < iNeedMontanas)
	{
		AddHeight(1 + getrandom(1),HEIGHT/8 + getrandom(HEIGHT/4),4+getrandom(4));

		SetTypes(true);
	}
	m_iProgress += 3;

	while(watercount != iNeedWater)
	{
		int x,y;
		x = getrandom(WIDTH);
		do
		{
			y = getrandom(HEIGHT);
		}
		while(x%2 != y%2);

		if(m_pMap[x][y].type != rtBARRIERWATER && m_pMap[x][y].type != rtBARRIERMOUNTAIN)
		{
			if(watercount > iNeedWater)
			{
				if(m_pMap[x][y].h < water)
				{
					m_pMap[x][y].h++;
					if(m_pMap[x][y].h >= water)
						watercount--;
				}
			}
			if(watercount < iNeedWater)
			{
				if(m_pMap[x][y].h >= water)
				{
					m_pMap[x][y].h--;
					if(m_pMap[x][y].h < water)
						watercount++;
				}
			}
		}
	}

	int x,y;
	for(x=0; x<WIDTH; x++)
		for(y=0; y<HEIGHT; y++)
		{
			if(x%2 == y%2)
			{
				if(m_pMap[x][y].type == rtBARRIERWATER)
				{
					int ocean = HasInRange(x,y,rtDEEPWATER,1) + HasInRange(x,y,rtSHALLOW,1) + HasInRange(x,y,rtBARRIERWATER,1);
					if(ocean != 6)
					{
						m_pMap[x][y].type = rtDEEPWATER;
					}
				}
			}
		}

	SetTypes(true);
	m_iProgress++;
}

ARMGenerator::~ARMGenerator()
{
	int x;
	for(x=0; x<WIDTH; x++)
	{
		free(m_pMap[x]);
	}
	free(m_pMap);

	for(x=0; x<WIDTH/2; x++)
	{
		free(m_pUnderMap[x]);
	}
	free(m_pUnderMap);

	if(m_pProvinceInfo)
	{
		delete m_pProvinceInfo;
		m_pProvinceInfo = NULL;
	}

	if(m_pSeasLocation)
		free(m_pSeasLocation);

	delete m_pProvinceInfo;		
}

void ARMGenerator::FormateSeasNames()
{
	if(m_pProvinceInfo)
	{
		delete m_pProvinceInfo;
		m_pProvinceInfo = NULL;
	}
	if(m_pSeasLocation)
		free(m_pSeasLocation);

//	maxID = 0;
	int x,y;
	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				if(m_pMap[x][y].h < 0)
					m_pMap[x][y].h = 0;
			}
		}


	for(int h=0; h<water; h++)
	{
		FormateSeasNamesOneStep(h);
	}

	int i;
	//int j;
	m_pProvinceInfo = new ARMProvinceInfoTable();

	m_pSeasLocation = (TMapRect *)calloc(maxID, sizeof(TMapRect));
	for(i=0; i<maxID; i++)
	{
		m_pSeasLocation[i].Left(WIDTH);
		m_pSeasLocation[i].Top(HEIGHT);
		m_pSeasLocation[i].Right(0);
		m_pSeasLocation[i].Bottom(0);
	}

	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
//				if(m_pMap[x][y].h == h && m_pMap[x][y].nameID != -1)
				if(m_pMap[x][y].nameID != -1)
				{
					m_pProvinceInfo->Add(m_pMap[x][y].nameID);
					for(int i=0; i<6; i++)
					{
						int xx = x + rround[i].x;
						int yy = y + rround[i].y;
						while(xx<0)
						{
							xx += WIDTH;
						}
						while(xx>=WIDTH)
						{
							xx -= WIDTH;
						}

						if(yy>=0 && yy<HEIGHT)
						{//25
							if(m_pMap[xx][yy].nameID != -1 && m_pMap[xx][yy].nameID != m_pMap[x][y].nameID && (m_pMap[xx][yy].type==rtDEEPWATER || m_pMap[xx][yy].type==rtSHALLOW))
							{
								if(m_pProvinceInfo->GetBorder(m_pMap[x][y].nameID,m_pMap[xx][yy].nameID) == 0)
									m_pProvinceInfo->SetBorder(m_pMap[x][y].nameID,m_pMap[xx][yy].nameID,CountSeaFrontier(x, y, m_pMap[xx][yy].nameID));
							}
						}
					}
					if(m_pProvinceInfo->GetArea(m_pMap[x][y].nameID) == 0)
						m_pProvinceInfo->SetArea(m_pMap[x][y].nameID, CountSeaArea(x,y));

					if(m_pSeasLocation[m_pMap[x][y].nameID].Left() > x)
						m_pSeasLocation[m_pMap[x][y].nameID].Left(x);
					if(m_pSeasLocation[m_pMap[x][y].nameID].Top() > y)
						m_pSeasLocation[m_pMap[x][y].nameID].Top(y);
					if(m_pSeasLocation[m_pMap[x][y].nameID].Right() < x)
						m_pSeasLocation[m_pMap[x][y].nameID].Right(x);
					if(m_pSeasLocation[m_pMap[x][y].nameID].Bottom() < y)
						m_pSeasLocation[m_pMap[x][y].nameID].Bottom(y);
				}
			}
		}

	int boundsCount1, area1, boundsCount2, area2;

	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				if(m_pMap[x][y].nameID != -1)
				{
					for(int i=0; i<6; i++)
					{
						int xx = x + rround[i].x;
						int yy = y + rround[i].y;
						while(xx<0)
						{
							xx += WIDTH;
						}
						while(xx>=WIDTH)
						{
							xx -= WIDTH;
						}

						if(yy>=0 && yy<HEIGHT)
						{//25
							if(m_pMap[xx][yy].nameID != -1 && m_pMap[xx][yy].nameID != m_pMap[x][y].nameID && (m_pMap[xx][yy].type==rtDEEPWATER || m_pMap[xx][yy].type==rtSHALLOW))
							{
								boundsCount1 = m_pProvinceInfo->GetBordersCount(m_pMap[x][y].nameID);
								boundsCount2 = m_pProvinceInfo->GetBordersCount(m_pMap[xx][yy].nameID);
/*								for(int j=0; j<maxID; j++)
								{
									if(m_pProvinceInfo->GetBorder(m_pMap[x][y].nameID,j) > 0)
									{
										boundsCount1++;
									}
									if(m_pProvinceInfo->GetBorder(m_pMap[xx][yy].nameID,j) > 0)
									{
										boundsCount2++;
									}
								}
*/
								area1 = m_pProvinceInfo->GetArea(m_pMap[x][y].nameID);
								area2 = m_pProvinceInfo->GetArea(m_pMap[xx][yy].nameID);

								if(area1<10 && boundsCount1 == 1 && area2<10 && boundsCount2 == 1)
								{
									m_pProvinceInfo->Merge(m_pMap[xx][yy].nameID, m_pMap[x][y].nameID);

									RenameArea(x,y,m_pMap[xx][yy].nameID);
								}
							}
						}
					}
				}
			}
		}
}

void ARMGenerator::GrowSea(int x, int y)
{
	if(m_pMap == NULL)
	{
		return;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return;
	}

	for(int i=0; i<6; i++)
	{
		int xx = x + rround[i].x;
		int yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[xx][yy].h == m_pMap[x][y].h && m_pMap[xx][yy].nameID == -1 && (m_pMap[xx][yy].type == rtDEEPWATER || m_pMap[xx][yy].type == rtSHALLOW))
			{
				m_pMap[xx][yy].nameID = m_pMap[x][y].nameID;
				GrowSea(xx,yy);
			}
		}
	}
}

int ARMGenerator::GrowSeaOneStep(int h)
{
	int result = 0;

	int x,y;
	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				m_pMap[x][y].count = m_pMap[x][y].nameID;
			}
		}

	for(y=0; y<HEIGHT; y++)
		for(x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				int existedID = -1;
				if(m_pMap[x][y].h == h && m_pMap[x][y].nameID == -1 && (m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW))
				{
					for(int i=0; i<6; i++)
					{
						int xx = x + rround[i].x;
						int yy = y + rround[i].y;
						while(xx<0)
						{
							xx += WIDTH;
						}
						while(xx>=WIDTH)
						{
							xx -= WIDTH;
						}

						if(yy>=0 && yy<HEIGHT)
						{
							if(m_pMap[xx][yy].nameID != -1 && m_pMap[xx][yy].count != -1 && (m_pMap[xx][yy].type == rtDEEPWATER || m_pMap[xx][yy].type == rtSHALLOW))
								existedID = m_pMap[xx][yy].nameID;
						}
					}

					if(existedID != -1)
					{
						m_pMap[x][y].nameID = existedID;
//						m_pMap[x][y].nameID = -2;
						result++;
					}
				}
			}
		}
/*
	for(int y=0; y<HEIGHT; y++)
		for(int x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				if(m_pMap[x][y].nameID == -2)
				{
					m_pMap[x][y].nameID = existedID;
					result++;
				}
			}
		}
*/
	return result;
}

void ARMGenerator::FormateSeasNamesOneStep(int h)
{
	if(h>=water)
		return;

	int uprise = 0;
	do
	{
		uprise = GrowSeaOneStep(h);
	}
	while(uprise>0);
/*
		for(int y=0; y<HEIGHT; y++)
			for(int x=0; x<WIDTH; x++)
			{
				if(x % 2 == y % 2)
				{
					if(m_pMap[x][y].h == h && m_pMap[x][y].nameID == -1)
					{
						int existedID = -1;
						for(int i=0; i<6; i++)
						{
							int xx = x + rround[i].x;
							int yy = y + rround[i].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								if(m_pMap[xx][yy].nameID != -1)
									existedID = m_pMap[xx][yy].nameID;
							}
						}

						if(existedID != -1)
						{
							m_pMap[x][y].nameID = existedID;
							GrowSea(x,y);
						}
					}
				}
			}
*/
	for(int y=0; y<HEIGHT; y++)
		for(int x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				if(m_pMap[x][y].h == h && m_pMap[x][y].nameID == -1 && (m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW))
				{
					m_pMap[x][y].nameID = maxID++;
					GrowSea(x,y);
				}
			}
		}

//	if(h > water/2)
//		return;
	MergeSeas();
}

void ARMGenerator::RenameArea(int x, int y, int newID)
{
	if(m_pMap == NULL)
	{
		return;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return;
	}

	if(m_pMap[x][y].nameID == newID)
	{
		return;
	}

	int oldID = m_pMap[x][y].nameID;
	m_pMap[x][y].nameID = newID;

	int xx,yy;

	for(int i=0; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[xx][yy].nameID == oldID)
			{
				RenameArea(xx, yy, newID);
			}
		}
	}
}

int ARMGenerator::CountSeaArea(int x, int y, bool start)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	int iResult=1;

//******************************************************************************
	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;
	int r = 0;

//	for(int r=1; r<range; r++)
	while(iRIndex > 0)
	{
		r++;
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;

						if(m_pMap[xx][yy].nameID == m_pMap[x][y].nameID)
//						if((m_pMap[xx][yy].type != rtBARRIERMOUNTAIN && m_pMap[xx][yy].h >= water && m_pMap[xx][yy].type != rtRIVER) && m_pMap[xx][yy].count != tempCountValue)
						{
							iResult++;

							newList[iRIndex].x = xx;
							newList[iRIndex].y = yy;
							iRIndex++;
						}
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;

//******************************************************************************
	return iResult;
}

int ARMGenerator::CountSeaFrontier(int x, int y, int otherID, bool start)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	int iResult=0;

//******************************************************************************
	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;
	int r = 0;

//	for(int r=1; r<range; r++)
	while(iRIndex > 0)
	{
		r++;
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;

						if(m_pMap[xx][yy].nameID == otherID)
						{
							iResult++;
						}
						if(m_pMap[xx][yy].nameID == m_pMap[x][y].nameID)
						{
							newList[iRIndex].x = xx;
							newList[iRIndex].y = yy;
							iRIndex++;
						}
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;

//******************************************************************************
	return iResult;
}

void ARMGenerator::MergeSeas()
{
	for(int y=0; y<HEIGHT; y++)
		for(int x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
//				if(m_pMap[x][y].h == h && m_pMap[x][y].nameID != -1)
				if(m_pMap[x][y].nameID != -1 && (m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW))
				{
					for(int i=0; i<6; i++)
					{
						int xx = x + rround[i].x;
						int yy = y + rround[i].y;
						while(xx<0)
						{
							xx += WIDTH;
						}
						while(xx>=WIDTH)
						{
							xx -= WIDTH;
						}

						if(yy>=0 && yy<HEIGHT)
						{//25
							if(m_pMap[xx][yy].nameID != -1 && m_pMap[xx][yy].nameID != m_pMap[x][y].nameID && (m_pMap[xx][yy].type == rtDEEPWATER || m_pMap[xx][yy].type == rtSHALLOW))
							{
								int area1, area2, bound1, bound2;
								area1 = CountSeaArea(x,y);
								area2 = CountSeaArea(xx,yy);
								bound1 = CountSeaFrontier(x, y, m_pMap[xx][yy].nameID);
								bound2 = CountSeaFrontier(xx, yy, m_pMap[x][y].nameID);
								if(M_PI*area1*4.0/9.0 <= bound1*bound1 ||
								   M_PI*area2*4.0/9.0 <= bound2*bound2 ||
								   (area1 < 4 && area2 < 4))
								{
									RenameArea(x,y,m_pMap[xx][yy].nameID);
								}
							}
						}
					}
				}
			}
		}
}

AString ARMGenerator::GetSeaName(int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Damned Waters";

	AString sResult, sNum;


//	sResult = "Vik's #" + IntToStr(nameID) + " (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";
//	sResult = (AString)regnames[(nameID*nameID)%1000];
	sResult = (AString)GetAbstractName(nameID*nameID).Str();
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	if(nameID%3 == 0)
	{
		if(m_pSeasLocation[nameID].Bottom() < HEIGHT/6)
			sResult = sResult + " Northern";
		if(m_pSeasLocation[nameID].Top() > HEIGHT*5/6)
			sResult = sResult + " Southern";
	}

	TARMProvince *pProvince;
	pProvince = m_pProvinceInfo->Find(nameID);

	if(!pProvince)
		return "Damned Waters";

	int totalBound, boundsCount, area;
	totalBound = 0;
	boundsCount = pProvince->m_iBordersCount;
	for(int i=0; i<boundsCount; i++)
	{
		totalBound += pProvince->m_pBorders[i].m_iLength;
	}
	area = pProvince->m_iArea;

	if(area<10)
	{
		if(boundsCount > 1)
		{
			sResult = sResult + " Strait";
		}
		else
		{
			if(boundsCount == 1)
			{
				sResult = sResult + " Gulf";
			}
			else
			{
				sResult = (AString)"Great " + sResult + " Lake";
			}
		}
		return sResult + sNum;
	}

	if(area<100)
	{
		if(boundsCount > 0)
		{
			sResult = sResult + " Sea";
		}
		else
		{
			sResult = sResult + " Inner Sea";
		}
		return sResult + sNum;
	}

	if(area<160)
	{
		if(boundsCount > 0)
		{
			sResult = (AString)"Great " + sResult + " Sea";
		}
		else
		{
			sResult = (AString)"Great " + sResult + " Inner Sea";
		}
		return sResult + sNum;
	}

	if(area<WIDTH*HEIGHT/16)
	{
		if(boundsCount > 0)
		{
			sResult = sResult + " Ocean";
		}
		else
		{
			sResult = sResult + " Inner Ocean";
		}
		return sResult + sNum;
	}

	if(boundsCount > 0)
	{
		sResult = (AString)"Great " + sResult + " Ocean";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Inner Ocean";
	}
	return sResult + sNum;
}

AString ARMGenerator::GetRiverName(int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless River";

	AString sResult, sNum;


//	sResult = "Vik's #" + IntToStr(nameID) + " (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";
//	sResult = (AString)regnames[(nameID*nameID)%1000];
	sResult = (AString)GetAbstractName(nameID*nameID).Str();
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

/*
	if(getrandom(3)==0)
	{
		if(m_pSeasLocation[nameID].Bottom < HEIGHT/6)
			sResult = sResult + " Northern";
		if(m_pSeasLocation[nameID].Top > HEIGHT*5/6)
			sResult = sResult + " Southern";
	}
*/
	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area<15)
	{
		sResult = sResult + " River";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " River";
	}
	return sResult + sNum;
}

bool ARMGenerator::CanBeDesert(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)
		return false;

//	if(m_pMap[x][y].iHumidity > aHumidity[rtDESERT][1]+1)
//		return false;

	if(y<HEIGHT*0.05*(10-m_iT) || y>HEIGHT*(1.0-0.05*(10-m_iT)))
		return false;

	if((y<HEIGHT*0.06*(10-m_iT) || y>HEIGHT*(1.0-0.06*(10-m_iT))) && getrandom(7)!=1)
		return false;

	if(HasInRange(x,y,rtSWAMP,1) > 0)
		return false;

//	if(GetMaxDelta(x,y) > 2)
	if(getrandom(GetMaxDelta(x,y)*GetMaxDelta(x,y)) > (10-m_iMoisure)*(10-m_iMoisure)*(10-m_iMoisure)/50)
		return false;

//	if(getrandom(HasInRange(x,y,rtSWAMP,2)) != 0)
//		return false;

	if(!CheckHumidity(x,y,rtDESERT))
		return false;

//	if(!CheckTemperature(x,y,rtDESERT))
//		return false;

	if(anchor)
	{
		if(HasInRange(x,y,rtLAKE,2) + HasInRange(x,y,rtRIVER,2) > 0)
			return false;

		if(HasInRange(x,y,rtLAKE,4) + HasInRange(x,y,rtRIVER,4) > 0 && getrandom(4) != 0)
			return false;

		if(HasInRange(x,y,rtDESERT,2) > 0)
			return false;
	}
	else
	{
		if(HasInRange(x,y,rtLAKE,1) + HasInRange(x,y,rtRIVER,1) > 0)
			return false;

		if(HasInRange(x,y,rtLAKE,3) + HasInRange(x,y,rtRIVER,3) > 0 && getrandom(4) != 0)
			return false;
	}

	return true;
}

int ARMGenerator::GrowElement(RegionType reg, int x, int y, const int maxWeight)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	TMapPoint	*pCatchedList = NULL;

	pCatchedList = (TMapPoint *)calloc(maxWeight, sizeof(TMapPoint));
//	try
//	{
//		pCatchedList = new TMapPoint[maxWeight];
//	}
//	catch(...)
//	{
//		return -1;
//	}
//	pCatchedList = new TMapPoint[30];
	if(pCatchedList == NULL)
	{
		return 0;
	}

	int iLastHead;
	int iCurrentHead;
	int iListSize;

	TMapPoint tp;

	tp.x = x;
	tp.y = y;
	pCatchedList[0] = tp;
	iLastHead = 0;
	iCurrentHead = 1;
	iListSize = 1;

	int xx, yy;

	while(iLastHead != iCurrentHead && iCurrentHead < maxWeight)
	{
		for(int k=iLastHead; k<iCurrentHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = pCatchedList[k].x + rround[i].x;
				yy = pCatchedList[k].y + rround[i].y;
				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT && iListSize<maxWeight)
				{
				  if(CheckTemperature(xx,yy,reg) && CheckHumidity(xx,yy,reg))
				  {
					switch(reg)
					{
						case rtTUNDRA:
							{
								if(CanBeTundra(xx, yy, false))
//								if(CanBeElement(xx, yy, rtFOREST))
								{
									m_pMap[xx][yy].type = rtTUNDRA;

									if(CountTheSame(xx,yy)<maxWeight || getrandom(2)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										CountTheSame(i,j);
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
							break;
						case rtPLAIN:
							{
								if(m_pMap[xx][yy].type == reg && m_pMap[xx][yy].nameID == -1)
								{
									m_pMap[xx][yy].type = reg;
									m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;

									if(CountTheSame(xx,yy)<maxWeight)// && getrandom(6)!=0)
//									if(CountTheSame(xx,yy)<maxWeight || getrandom(8)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
									}
								}
							}
							break;
						case rtDESERT:
							{
								if(CanBeDesert(xx, yy, false))
//								if(CanBeElement(xx, yy, rtDESERT))
								{
									m_pMap[xx][yy].type = rtDESERT;
									m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;

									if(CountTheSame(xx,yy)<maxWeight)
//									if(CountTheSame(xx,yy)<maxWeight && getrandom(3)!=0)
//									if(CountTheSame(xx,yy)<maxWeight || getrandom(8)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
									}
									else
									{
										m_pMap[xx][yy].type = rtPLAIN;
//										m_pMap[xx][yy].type = rtMONTANA + getrandom(rtDESERT - rtMONTANA);
									}
								}
							}
							break;
						case rtWASTELANDS:
							{
								if(CanBeWastes(xx, yy, false))
//								if(CanBeElement(xx, yy, rtWASTELANDS))
								{
									m_pMap[xx][yy].type = rtWASTELANDS;
									m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;

									if(CountTheSame(xx,yy)<maxWeight)
//									if(CountTheSame(xx,yy)<maxWeight && getrandom(3)!=0)
//									if(CountTheSame(xx,yy)<maxWeight || getrandom(8)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
									}
									else
									{
										m_pMap[xx][yy].type = rtPLAIN;
//										m_pMap[xx][yy].type = rtMONTANA + getrandom(rtDESERT - rtMONTANA);
									}
								}
							}
							break;
						case rtFOREST:
							{
								if(CanBeForest(xx, yy, false))
//								if(CanBeElement(xx, yy, rtFOREST))
								{
									m_pMap[xx][yy].type = rtFOREST;

									if(CountTheSame(xx,yy)<maxWeight || getrandom(2)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										CountTheSame(i,j);
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
							break;
						case rtJUNGLE:
							{
								if(CanBeForest(xx, yy, false))
//								if(CanBeElement(xx, yy, rtFOREST))
								{
									m_pMap[xx][yy].type = rtJUNGLE;

									if(CountTheSame(xx,yy)<maxWeight || getrandom(2)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										CountTheSame(i,j);
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
							break;
						case rtLAKE:
							{
								if(CanBeLake(xx, yy, false))
								{
									m_pMap[xx][yy].type = rtLAKE;

									if(CountTheSame(xx,yy)<maxWeight)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										CountTheSame(i,j);
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
							break;
						case rtMONTANA:
							{
								if(CanBeMontana(xx, yy, false))
								{
									m_pMap[xx][yy].type = rtMONTANA;

									if(CountTheSame(xx,yy)<maxWeight)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										m_pMap[xx][yy].type = rtMONTANA;
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
							break;
						case rtHILLS:
							{
								if(CanBeHill(xx, yy, false))
								{
									m_pMap[xx][yy].type = rtHILLS;

									if(CountTheSame(xx,yy)<maxWeight)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										m_pMap[xx][yy].type = rtMONTANA;
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
							break;
						case rtSWAMP:
							{
								if(CanBeSwamp(xx, yy, false))
//								if(CanBeElement(xx, yy, rtSWAMP))
								{
									m_pMap[xx][yy].type = rtSWAMP;

									if(CountTheSame(xx,yy)<maxWeight || getrandom(2)==0)
									{
										tp.x = xx;
										tp.y = yy;
										pCatchedList[iListSize++] = tp;
										m_pMap[xx][yy].nameID = m_pMap[pCatchedList[k].x][pCatchedList[k].y].nameID;
									}
									else
									{
//										CountTheSame(i,j);
										m_pMap[xx][yy].type = rtPLAIN;
									}
								}
							}
						default:
							std::cerr << "Unknown" << std::endl;
					}
				  }	
				}
			}
		}
		iLastHead = iCurrentHead;
		iCurrentHead = iListSize;
	}
/*
	for(int i=0; i<iListSize; i++)
	{
		int xx,yy;
		xx = pCatchedList[i].x;
		yy = pCatchedList[i].y;
//		GrowHumidity(xx, yy, aHumidity[m_pMap[xx][yy].type]);
//		m_pMap[xx][yy].iHumidity = aHumidity[m_pMap[xx][yy].type];
	}
*/
	free(pCatchedList);
//	delete[] pCatchedList;

	return iListSize;
}

bool ARMGenerator::CanBeForest(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)// && m_pMap[x][y].type != rtSWAMP)
		return false;

//	if(HasInRange(x,y,rtFOREST,2+getrandom(3)) > 12)
//		return false;

	if(m_pMap[x][y].type == rtPLAIN)
	{
// 		if(getrandom(5) <= HasInRange(i,j,rtFOREST,1)*HasInRange(i,j,rtFOREST,1))
		if(getrandom(HasInRange(x,y,rtDESERT,1)) > 1)
			return false;

		if(HasInRange(x,y,rtMONTANA,1) > 0)
			return true;

		if(anchor)
		{
			if(getrandom(HasInRange(x,y,rtSWAMP,1)) == 0 && HasInRange(x,y,rtMONTANA,1) == 0 && getrandom(2)!=1)
				return false;

			if((HasInRange(x,y,rtLAKE,4) + HasInRange(x,y,rtRIVER,4) ==0 || getrandom(5)==0) &&
			   (HasInRange(x,y,rtDEEPWATER,4) + HasInRange(x,y,rtSHALLOW,4)==0 || getrandom(5)==0))// &&
// 			   getrandom(2)!=0)
				return false;
		}
		else
		{
			if((HasInRange(x,y,rtFOREST,1)+HasInRange(x,y,rtJUNGLE,1))*(HasInRange(x,y,rtFOREST,1)+HasInRange(x,y,rtJUNGLE,1)) < getrandom(6))
				return false;

			if(getrandom(3)!=1)
				return false;
		}
	}
/*
	if(m_pMap[x][y].type == rtSWAMP)
	{
		if(anchor)
		{
			if(getrandom(8)!=0)
				return false;

			if((HasInRange(x,y,rtLAKE,4) + HasInRange(x,y,rtRIVER,4)==0 || getrandom(3)!=0) &&
			   (HasInRange(x,y,rtDEEPWATER,4) + HasInRange(x,y,rtSHALLOW,4)==0 || getrandom(3)!=0))
			   return false;
		}
		else
		{
			if(getrandom(5)!=0)
				return false;
		}

//		if(getrandom(5) <= HasInRange(i,j,rtFOREST,1)*HasInRange(i,j,rtFOREST,1))

//		if(getrandom(HasInRange(i,j,rtDESERT,1)) <= 1)
	}
*/
	return true;
}

bool ARMGenerator::CanBeLake(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type == rtBARRIERMOUNTAIN)
		return false;

//	if((m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW) && CountWater(x,y) < 6)
//		return true;

	if(HasInRange(x,y,rtDEEPWATER,1) + HasInRange(x,y,rtSHALLOW,1) + HasInRange(x,y,rtBARRIERWATER,1)>0)
		return false;

	if(IsExtremum(x,y) == 1)
		return false;

	if(HasInRange(x,y,rtLAKE,3+getrandom(3))>0)
		return false;

	if(anchor)
	{
//		if(getrandom((10-m_iMoisure)*(10-m_iMoisure)*10) != 0)
//			return false;
	}
	else
	{
		if(getrandom(9)!=1)
			return false;
	}

	return true;
}

bool ARMGenerator::CanBeMontana(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)
		return false;

	if(GetMaxDelta(x,y)<6)
		return false;

	if(anchor)
	{
		if(IsExtremum(x,y) != 1)
// 		if(m_pMap[i][j].type == rtUNKNOWN && (delta>5 || m_pMap[i][j].h>montana))
			return false;
	}
	else
	{
		if(HasInRange(x,y,rtMONTANA,1)*HasInRange(x,y,rtMONTANA,1) < getrandom(6))
			return false;
	}

	return true;
}

bool ARMGenerator::CanBeSwamp(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)
		return false;

	if(HasInRange(x,y,rtDEEPWATER,1) + HasInRange(x,y,rtSHALLOW,1)>0 && (getrandom(4)!=1))
		return false;

	if(m_pMap[x][y].iHumidity < aHumidity[rtSWAMP][0]-aHumidity[rtSWAMP][1])// || m_pMap[x][y].iHumidity > aHumidity[rtSWAMP]+1)
		return false;
//	if(getrandom(GetMaxDelta(xx,yy)) < 2)


	if(anchor)
	{
//		if(m_pMap[i][j].h <= water+1 && GetMaxDelta(i,j)<3)
		if(m_pMap[x][y].h > 1.1*water && HasInRange(x,y,rtLAKE,4) < 4 && HasInRange(x,y,rtRIVER,3) < 10)
			return false;

		if(HasInRange(x,y,rtLAKE,1) > 0)
			return false;
	}
	else
	{
		if(m_pMap[x][y].h > water+m_iMoisure/5 && (m_pMap[x][y].h > water+m_iMoisure/2 || getrandom(GetMaxDelta(x,y)) > m_iMoisure/2))
//		if(m_pMap[x][y].h > 1.05*water && (m_pMap[x][y].h > 1.08*water || getrandom(GetMaxDelta(x,y)) > 3))
			return false;

// 		if(m_pMap[xx][yy].h <= water+1)
// 		if(getrandom(6) <= HasInRange(xx,yy,rtSWAMP,1)*HasInRange(xx,yy,rtSWAMP,1))
// 		if(HasInRange(xx,yy,rtSWAMP,5) < 35)
	}

	return true;
}

bool ARMGenerator::CanBeTundra(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type == rtBARRIERWATER)
		return false;

	if(m_pMap[x][y].type == rtBARRIERMOUNTAIN)
		return false;

	if(m_pMap[x][y].type == rtDEEPWATER)
		return false;

	if(m_pMap[x][y].type == rtSHALLOW)
		return false;

	if(m_pMap[x][y].type == rtRIVER)
		return false;

	if(m_pMap[x][y].type == rtMONTANA)
		return false;

	if(m_pMap[x][y].type == rtSWAMP)
		return false;

	if(m_pMap[x][y].type == rtLAKE)
		return false;

	if(y<0.5*(10-m_iT) || y>=(float)HEIGHT-1-0.5*(10-m_iT))
		return true;

	if(HasInRange(x,y,rtTUNDRA,1)==0 && HasInRange(x,y,rtSWAMP,1)==0 && HasInRange(x,y,rtDEEPWATER,1) + HasInRange(x,y,rtSHALLOW,1)<2)
		return false;

//	float chance = getrandom(HEIGHT*0.004*(10-m_iT)*(10-m_iT));
//	if(chance <= y && chance < HEIGHT-1-y)
//		return false;
	int a,b,t;
	a = aTemperature[rtTUNDRA][0] - m_pMap[x][y].iTemperature;
	b = m_pMap[x][y].iTemperature - aTemperature[rtTUNDRA][1];
	if(a<0) a=0;
	if(b<0) b=0;
	if(a<b)
		t = b;
	else
		t = a;

	if(t>aTemperature[rtTUNDRA][1]-aTemperature[rtTUNDRA][0] || getrandom(t*t) != 0)
		return false;

	if(HasInRange(x,y,rtTUNDRA,1)*HasInRange(x,y,rtTUNDRA,1) < getrandom(6))
		return false;

	if(getrandom(3)!=1)
		return false;

	return true;
}

int ARMGenerator::AddRiver(int iStartX, int iStartY, bool start, int length)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(iStartX%2 != iStartY%2)
		iStartY++;

	while(iStartX<0)
	{
		iStartX += WIDTH;
	}
	while(iStartX>=WIDTH)
	{
		iStartX -= WIDTH;
	}

	if(iStartY<0 || iStartY>=HEIGHT)
	{
		return 0;
	}

	if(m_pMap[iStartX][iStartY].h < water)
	{
		return 0;
	}

	if(m_pMap[iStartX][iStartY].type == rtBARRIERMOUNTAIN)
	{
		return 0;
	}

	int xx,yy;
	int iRes = 0;
	for(int i=0; i<6; i++)
	{
		xx = iStartX + rround[i].x;
		yy = iStartY + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[xx][yy].type == rtLAKE)
			{
				return 0;
			}
			if(m_pMap[xx][yy].type == rtRIVER || m_pMap[xx][yy].riverID != -1 || m_pMap[xx][yy].h < water)
			{
				iRes++;
			}
		}
	}

	if(iRes > 2)
	{
		return 0;
	}

	if(start)
	{
		if(HasInRange(iStartX, iStartY, rtDEEPWATER, 4) + HasInRange(iStartX, iStartY, rtSHALLOW, 4)> 0)
		{
			return 0;
		}

		if(HasInRange(iStartX, iStartY, rtRIVER, 4)> 0)
		{
			return 0;
		}

		tempCountValue++;
		maxID++;
	}

	m_pMap[iStartX][iStartY].count = tempCountValue;

//	int length = 0;
	int lowest, lx, ly;

	for(int k=0; k<6; k++)
	{
		lowest = m_pMap[iStartX][iStartY].h;
		lx = -1;
		ly = -1;

		for(int i=0; i<6; i++)
		{
			xx = iStartX + rround[i].x;
			yy = iStartY + rround[i].y;
			while(xx<0)
			{
				xx += WIDTH;
			}
			while(xx>=WIDTH)
			{
				xx -= WIDTH;
			}

			if(yy>=0 && yy<HEIGHT)
			{
				if(m_pMap[xx][yy].type != rtBARRIERMOUNTAIN &&
				   m_pMap[xx][yy].h <= lowest &&
				   m_pMap[xx][yy].count != tempCountValue &&
				   m_pMap[xx][yy].type != rtRIVER &&
				   m_pMap[xx][yy].riverID == -1 &&
				   getrandom(3)!=1)
				{
					lowest = m_pMap[xx][yy].h;
					lx = xx;
					ly = yy;
				}
			}
		}
		if(lx != -1 && ly != -1)
		{
			int ttempCountValue;
			ttempCountValue = tempCountValue;
			if((m_pMap[lx][ly].type == rtDEEPWATER || m_pMap[lx][ly].type == rtSHALLOW))// && CountWater(lx,ly) > 4)// && m_pMap[lx][ly].count != tempCountValue)
//			if(m_pMap[lx][ly].h < water)
			{
				m_pMap[iStartX][iStartY].type = rtRIVER;
				m_pMap[iStartX][iStartY].riverID = length;
				m_pMap[iStartX][iStartY].nameID = maxID;
				m_pMap[iStartX][iStartY].riverNameID = maxID;

				m_pMap[lx][ly].riverID = length+1;
				m_pMap[lx][ly].riverNameID = maxID;
				return 1;
			}
			else
			{
				tempCountValue = ttempCountValue;
				m_pMap[lx][ly].count = tempCountValue;

				int l;
				l = AddRiver(lx, ly, false, length+1);
				if(l>0)// && getrandom(4)!=1)
				{
					m_pMap[iStartX][iStartY].riverID = length;
					m_pMap[iStartX][iStartY].type = rtRIVER;
					m_pMap[iStartX][iStartY].nameID = maxID;
					m_pMap[iStartX][iStartY].riverNameID = maxID;
					return l+1;
				}
			}
		}
	}
	return 0;
}

AString ARMGenerator::GetElementName(int x, int y)
{
	if(m_pMap == NULL)
	{
		return "ERROR";
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return "ERROR";
	}

	if(bAdvancedMap && (m_pMap[x][y].type == rtBARRIERWATER || m_pMap[x][y].type == rtBARRIERMOUNTAIN))
	{
		return "Edge of World";
	}

//	if(m_pMap[x][y].nameID == -1)
//	{
//		return "yet unnamed";
//	}

	AString sBase;

//	sBase = GetEthnicName(SortedRacesByPop(x,y,0), y*WIDTH + x);
//	sBase = GetEthnicName(SortedRacesByPop(x,y,0), m_pMap[x][y].nameID);
	sBase = GetEthnicName(GetAreaPrevealedRace(x,y), m_pMap[x][y].nameID*m_pMap[x][y].nameID).Str();

	if(m_pMap[x][y].island)
	{
		return sBase + " Island";
	}

	switch(m_pMap[x][y].type)
	{
		case rtBARRIERWATER:
		case rtDEEPWATER:
		case rtSHALLOW:
				return GetSeaName(m_pMap[x][y].nameID);
		case rtRIVER:
				return GetRiverName(m_pMap[x][y].nameID);
		case rtLAKE:
				return GetLakeName(m_pMap[x][y].nameID);
		case rtFOREST:
				return GetForestName(sBase, m_pMap[x][y].nameID);
		case rtJUNGLE:
				return GetJungleName(sBase, m_pMap[x][y].nameID);
		case rtDESERT:
				return GetDesertName(sBase, m_pMap[x][y].nameID);
		case rtVOLKANO:
				return GetVolcanoName(sBase, m_pMap[x][y].nameID);
		case rtBARRIERMOUNTAIN:
		case rtMONTANA:
				return GetMountainName(sBase, m_pMap[x][y].nameID);
		case rtHILLS:
				return GetHillsName(sBase, m_pMap[x][y].nameID);
		case rtSWAMP:
				return GetSwampName(sBase, m_pMap[x][y].nameID);
		case rtPLAIN:
				return GetPlainName(sBase, m_pMap[x][y].nameID);
		case rtTUNDRA:
				return GetTundraName(sBase, m_pMap[x][y].nameID);
		case rtWASTELANDS:
				return GetWastelandsName(sBase, m_pMap[x][y].nameID);
		default:
				return (AString)GetAbstractName(m_pMap[x][y].nameID*m_pMap[x][y].nameID).Str();
	}
/*
	if(m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW)
		return GetSeaName(m_pMap[x][y].nameID);

	if(m_pMap[x][y].type == rtRIVER)
		return GetRiverName(m_pMap[x][y].nameID);

	return "yet unnamed";
*/

//	return "ERROR";
}

int ARMGenerator::CountWater(int x, int y, bool start)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	if(m_pMap[x][y].type != rtDEEPWATER && m_pMap[x][y].type != rtSHALLOW)
	{
		return 0;
	}

	int iResult=0;

//******************************************************************************
	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;
	int r = 0;

//	for(int r=1; r<range; r++)
	while(iRIndex > 0)
	{
		r++;
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;

						if(m_pMap[xx][yy].type == rtDEEPWATER || m_pMap[xx][yy].type == rtSHALLOW)
						{
							iResult++;

							newList[iRIndex].x = xx;
							newList[iRIndex].y = yy;
							iRIndex++;
						}
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;

//******************************************************************************
	return iResult;
}

void ARMGenerator::CalculateHumidity()
{
	if(m_pMap == NULL)
	{
		return;
	}
	int i,j;
	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			m_pMap[i][j].iHumidity = 0;
		}
	}

	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			if(i%2 == j%2)
			{
				if(m_pMap[i][j].type == rtDEEPWATER || m_pMap[i][j].type == rtSHALLOW || m_pMap[i][j].type == rtRIVER || m_pMap[i][j].type == rtLAKE)
				{
//					GrowHumidity(i,j,aHumidity[m_pMap[i][j].type][0]+aHumidity[m_pMap[i][j].type][1]-getrandom(aHumidity[m_pMap[i][j].type][1]));
					GrowHumidity(i,j,aHumidity[m_pMap[i][j].type][0]+1-getrandom(2)+m_iMoisure/2);
//					GrowHumidity(i,j,aHumidity[m_pMap[i][j].type][0]);
				}
				else
				{
					GrowHumidity(i,j,0.5+(float)(10-m_iT+getrandom(2))*abs(j-HEIGHT/2)/(HEIGHT/2));
					if(m_pMap[i][j].riverID >= 0)
					{
//						GrowHumidity(i,j,aHumidity[rtRIVER][0]+1-getrandom(2)+m_iMoisure/2);
						GrowHumidity(i,j,3+sqrt(m_pMap[i][j].riverID)+m_iMoisure/2);
					}
					if(m_pMap[i][j].type == rtVOLKANO)
					{
//						GrowHumidity(i,j,aHumidity[rtVOLKANO][0]+aHumidity[rtVOLKANO][1]-getrandom(aHumidity[rtVOLKANO][1]));
						GrowHumidity(i,j,aHumidity[rtVOLKANO][0]+1-getrandom(2));
//						GrowHumidity(i,j,aHumidity[rtVOLKANO][0]);
					}
					if(m_pMap[i][j].type == rtMONTANA)
					{
//						GrowHumidity(i,j,aHumidity[rtMONTANA][0]+aHumidity[rtMONTANA][1]-getrandom(aHumidity[rtMONTANA][1]));
						GrowHumidity(i,j,aHumidity[rtMONTANA][0]+1-getrandom(2));
//						GrowHumidity(i,j,aHumidity[rtMONTANA][0]);
					}
				}
			}
		}
	}
	for(int k=0; k<WIDTH*HEIGHT; k++)
	{
		int x,y;
		x = getrandom(WIDTH);
		y = getrandom(HEIGHT);

		if(x%2 == y%2)
		{
			if(m_pMap[x][y].iHumidity < aHumidity[rtSWAMP][0])
			{
//				if(m_pMap[x][y].h - water < 0.02*m_iMoisure*water)
				if(m_pMap[x][y].h - water < 0.04*m_iMoisure*water)
//				if(m_pMap[x][y].h - water < 0.02*m_iMoisure*water && (GetMaxDelta(x,y)==0 || (GetMaxDelta(x,y)==1 && getrandom(3)==1)))
				{
					GrowHumidity(x,y,(m_iMoisure+m_pMap[x][y].iHumidity)/2);
//					GrowHumidity(x,y,m_pMap[x][y].iHumidity*2);
//					GrowHumidity(x,y,aHumidity[rtSWAMP]*0.66 + m_pMap[x][y].iHumidity*0.5);
//					GrowHumidity(x,y,(aHumidity[rtSWAMP]+m_pMap[x][y].iHumidity)/2);
//					GrowHumidity(x,y,m_pMap[x][y].iHumidity + (aHumidity[rtSWAMP]-m_pMap[x][y].iHumidity)*0.5);
//					GrowHumidity(x,y,m_pMap[x][y].iHumidity + 1);
				}
			}
		}
	}

	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			if(m_pMap[i][j].h > water)
				m_pMap[i][j].iHumidity = m_pMap[i][j].iHumidity / (int)sqrt(m_pMap[i][j].h - water);
		}
	}
}

void ARMGenerator::GrowHumidity(int x, int y, int iHumidity)
{
	if(m_pMap == NULL)
	{
		return;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return;
	}

//	if(m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW || m_pMap[x][y].type == rtRIVER || m_pMap[x][y].type == rtLAKE)
//	{
//		return;
//	}

	if(iHumidity <= 0)
	{
		return;
	}

	m_pMap[x][y].iHumidity = iHumidity + getrandom(2);// - 2;

	for(int i=0; i<6; i++)
	{
		int xx = x + rround[i].x;
		int yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[xx][yy].type != rtDEEPWATER && m_pMap[xx][yy].type != rtSHALLOW &&
			   m_pMap[xx][yy].type != rtRIVER && m_pMap[xx][yy].type != rtLAKE &&
			   m_pMap[xx][yy].type != rtMONTANA && m_pMap[xx][yy].type != rtVOLKANO)
			{
				if(m_pMap[xx][yy].iHumidity < iHumidity)
				{
//					m_pMap[xx][yy].iHumidity = iHumidity;
					GrowHumidity(xx, yy, iHumidity-1);// + getrandom(2) - 2);
				}
			}
		}
	}
}

bool ARMGenerator::CanBeElement(int x, int y, RegionType tRegion)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)
		return false;

//	if(m_pMap[x][y].iHumidity < aHumidity[tRegion][0]-aHumidity[tRegion][1] || m_pMap[x][y].iHumidity > aHumidity[tRegion][0]+aHumidity[tRegion][1])
//		return false;

	if(!CheckHumidity(x,y,tRegion))
		return false;

	if(!CheckTemperature(x,y,tRegion))
		return false;

	if(HasInRange(x,y,tRegion,1) > 0 && getrandom(9)!=1)
		return false;

	return true;
}

int ARMGenerator::FindLand(int x, int y, int range)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}


	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;

	for(int r=1; r<range; r++)
	{
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
        	iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;
						if(m_pMap[xx][yy].h >= water)
						{
							if(oldList)
							{
								delete[] oldList;
							}
							if(newList)
							{
								delete[] newList;
							}
							return r;
						}
						else
						{
							newList[iRIndex].x = xx;
							newList[iRIndex].y = yy;
							iRIndex++;
						}
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;
	return range;
}

void ARMGenerator::GenerateFractal(int iContinents, int iAge, int iT, int iWet, float fNeedWater, float fNeedMontanas, int bounds)
{
	if(m_pMap == NULL)
	{
		return;
	}

	m_iAge = iAge;
	m_iT = iT;
	m_iMoisure = iWet;

	maxContiID = 0;

	CFractalMap *pFractal;
	pFractal = new CFractalMap();

	pFractal->CreateWorld(WIDTH, HEIGHT, 100*fNeedWater, iContinents, iAge, bounds);

	int x,y;
	for(x=0; x<WIDTH; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			m_pMap[x][y].h = pFractal->GetHeight(x,y)*20.0/256.0;
			m_pMap[x][y].contiID = pFractal->GetContiID(x,y);
			if(m_pMap[x][y].contiID > maxContiID)
			{
				maxContiID = m_pMap[x][y].contiID;
			}

			if(pFractal->GetHeight(x,y) == bounds)
			{
				if(bounds == 0)
				{
					m_pMap[x][y].type = rtBARRIERWATER;
				}
				if(bounds == 255)
				{
					m_pMap[x][y].type = rtBARRIERMOUNTAIN;
				}
			}
		}
	}
//	iMaxHeight = 256;
//	water = 128;
	iMaxHeight = 20;
	water = 10;

	delete pFractal;

//	SetTypes(true);
	BalanceMap(fNeedWater, fNeedMontanas);

	RandomizeMap();
	SetTypes();
	InitPop();
	SetRareResources();

	GenerateUnderground();
}

void ARMGenerator::MergeProvinces()
{
//	return;

	for(int y=0; y<HEIGHT; y++)
	{
		for(int x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
//				if(m_pMap[x][y].h == h && m_pMap[x][y].nameID != -1)
				if(m_pMap[x][y].nameID != -1 && !IsRegionCompatible(x,y,rtDEEPWATER))
				{
					if(m_pProvinceInfo->GetArea(m_pMap[x][y].nameID) < 7)
					{
						if(CountLand(x,y) <= 15)
						{
							m_pMap[x][y].island = true;
						}
						else
						{
							m_pMap[x][y].island = false;
						}

						for(int i=0; i<6; i++)
						{
							int xx = x + rround[i].x;
							int yy = y + rround[i].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								if((m_pMap[xx][yy].nameID != -1) && !IsRegionCompatible(xx,yy,rtDEEPWATER) && m_pMap[x][y].nameID != m_pMap[xx][yy].nameID)
								{
									if(m_pProvinceInfo->GetArea(m_pMap[x][y].nameID) <= m_pProvinceInfo->GetArea(m_pMap[xx][yy].nameID))
									{
										if(m_pMap[x][y].island)
										{
											m_pProvinceInfo->Merge(m_pMap[xx][yy].nameID, m_pMap[x][y].nameID);
											RenameArea(x,y,m_pMap[xx][yy].nameID);
										}
										else
										{
											if(IsRegionCompatible(x,y,m_pMap[xx][yy].type) &&
											   1 + getrandom(2*m_pProvinceInfo->GetArea(m_pMap[xx][yy].nameID)) >= m_pProvinceInfo->GetArea(m_pMap[x][y].nameID) && getrandom(3)==0)
											{
												m_pProvinceInfo->Merge(m_pMap[xx][yy].nameID, m_pMap[x][y].nameID);
												RenameArea(x,y,m_pMap[xx][yy].nameID);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//TARMProvince *pProvince;
	//pProvince = m_pProvinceInfo->Find(1743);
}

AString ARMGenerator::GetForestName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Forest";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Forest";
		else
			sResult = sResult + " Woods";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Forest";
	}
	return sResult + sNum;
}

AString ARMGenerator::GetDesertName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Desert";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Desert";
		else
			sResult = sResult + " Sands";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Desert";
	}
	return sResult + sNum;
}

AString ARMGenerator::GetJungleName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Jungle";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Jungle";
		else
			sResult = sResult + " Woods";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Jungle";
	}
	return sResult + sNum;
}

AString ARMGenerator::GetMountainName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Mountain";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
//	sNum = " (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area==1)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Mountain";
		else
			sResult = sResult + " Peak";

		return sResult + sNum;
	}

	if(area<15)
	{
		if(nameID%3 == 1)
			sResult = sResult + " Mountains";
		else
		{
			if(nameID%2 == 1)
				sResult = sResult + " Heights";
			else
				sResult = sResult + " Rocks";
		}
		return sResult + sNum;
	}

	sResult = (AString)"Great " + sResult + " Mountains";
	return sResult + sNum;
}

AString ARMGenerator::GetSwampName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Swamp";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Swamp";
		else
			sResult = sResult + " Marshes";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Swamp";
	}
	return sResult + sNum;
}

bool ARMGenerator::IsRegionCompatible(int x, int y, int iType)
{
	if(m_pMap[x][y].type == iType)
	{
		return true;
	}

	if(iType == rtDEEPWATER || iType == rtSHALLOW)
	{
		if(m_pMap[x][y].type == rtDEEPWATER || m_pMap[x][y].type == rtSHALLOW)
		{
			return true;
		}
	}

	if(iType == rtRIVER || iType == rtLAKE)
	{
		if(m_pMap[x][y].type == rtRIVER || m_pMap[x][y].type == rtLAKE)
		{
			return true;
		}
	}

	if(iType == rtMONTANA || iType == rtVOLKANO || iType == rtHILLS)
	{
		if(m_pMap[x][y].type == rtMONTANA || m_pMap[x][y].type == rtVOLKANO || m_pMap[x][y].type == rtHILLS)
		{
			return true;
		}
	}

	if(iType == rtJUNGLE || iType == rtFOREST || iType == rtSWAMP)
	{
		if(m_pMap[x][y].type == rtJUNGLE || m_pMap[x][y].type == rtFOREST || m_pMap[x][y].type == rtSWAMP)
		{
			return true;
		}
	}

	return false;
}

AString ARMGenerator::GetPlainName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Plain";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area==1)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Dale";
		else
			sResult = sResult + " Plain";

		return sResult + sNum;
	}

	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Plains";
		else
			sResult = sResult + " Valley";

		return sResult + sNum;
	}

	sResult = (AString)"Great " + sResult + " Plains";
	return sResult + sNum;
}

int ARMGenerator::CountLand(int x, int y, bool start)
{
	if(m_pMap == NULL)
	{
		return 0;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return 0;
	}

	if(m_pMap[x][y].h < water)
	{
		return 0;
	}

	int iResult=0;

//******************************************************************************
	int iLastHead;
	int iCurrentHead;

	iLastHead = 0;
	iCurrentHead = 0;

	TMapPoint *oldList;
	TMapPoint *newList;

	oldList = NULL;
	newList = NULL;

	iCurrentHead = 1;
	newList = new TMapPoint[iCurrentHead];
	newList[0].x = x;
	newList[0].y = y;

	tempCountValue++;

	int xx, yy;

	int iRIndex = 1;
	int r = 0;

//	for(int r=1; r<range; r++)
	while(iRIndex > 0)
	{
		r++;
		if(oldList)
		{
			delete[] oldList;
		}
		oldList = newList;
		iLastHead = iCurrentHead;

		if(iLastHead != iRIndex)
		{
			iLastHead = iRIndex;
		}

		iCurrentHead = 6*r;
		newList = new TMapPoint[iCurrentHead];

		iRIndex = 0;

		for(int k=0; k<iLastHead; k++)
		{
			for(int i=0; i<6; i++)
			{
				xx = oldList[k].x + rround[i].x;
				yy = oldList[k].y + rround[i].y;

				if(oldList[k].x>WIDTH || oldList[k].y>HEIGHT)
				{
					xx = 0;
					yy = 0;
				}

				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].count != tempCountValue && iRIndex < iCurrentHead)
					{
						m_pMap[xx][yy].count = tempCountValue;

						if(m_pMap[xx][yy].type != rtBARRIERMOUNTAIN && m_pMap[xx][yy].h >= water && m_pMap[xx][yy].type != rtRIVER)
						{
							iResult++;

							newList[iRIndex].x = xx;
							newList[iRIndex].y = yy;
							iRIndex++;
						}
					}
				}
			}
		}
	}

	delete[] oldList;
	delete[] newList;

//******************************************************************************

	return iResult;
}

int ARMGenerator::GetStandartRType(RegionType rType, bool bAdvancedMap)
{
	switch(rType)
	{
		case rtDEEPWATER:
				return R_OCEAN;
		case rtSHALLOW:
				return (bAdvancedMap)? R_SHALLOW:R_OCEAN;
		case rtRIVER:
				return (bAdvancedMap)? R_RIVER:R_OCEAN;
		case rtLAKE:
				return R_LAKE;
		case rtMONTANA:
				return (bAdvancedMap)? R_HIGH_MOUNTAIN:R_MOUNTAIN;
		case rtHILLS:
				return (bAdvancedMap)? R_HILLS:R_MOUNTAIN;
		case rtVOLKANO:
				return R_VOLCANO;
		case rtDESERT:
				return R_DESERT;
		case rtPLAIN:
				return R_PLAIN;
		case rtSWAMP:
				return R_SWAMP;
		case rtFOREST:
				return R_FOREST;
		case rtJUNGLE:
				return R_JUNGLE;
		case rtTUNDRA:
				return R_TUNDRA;
		case rtWASTELANDS:
				return R_CERAN_WASTELAND;
		default:
				return R_OCEAN;
	}

	return R_OCEAN;
}

void ARMGenerator::InitPop()
{
	for(int y=0; y<HEIGHT; y++)
	{
		for(int x=0; x<WIDTH; x++)
		{
			if(x % 2 == y % 2)
			{
				InitPopReg(x,y);
			}
		}
	}
}

void ARMGenerator::InitPopReg(int x, int y)
{
	TerrainType * typer = &(TerrainDefs[GetStandartRType(m_pMap[x][y].type, Globals->ADVANCED_MAP)]);

	int pop = typer->pop;
//	int mw = typer->wages;

	if (pop == 0)
	{
		m_pMap[x][y].iCitySize = 0;
		int k;
		for(k=0; k<NUMMAN; k++)
		{
			m_pMap[x][y].aPop[k] = 0;
		}
		return;
	}
	int noncoastalraces = sizeof(typer->races)/sizeof(int);
	int allraces = noncoastalraces + sizeof(typer->coastal_races)/sizeof(int);

	int race;
	race = -1;
	while (race == -1 || (ItemDefs[race].flags & ItemType::DISABLED))
	{
		int coastral;
		coastral = HasInRange(x,y,rtDEEPWATER,1) + HasInRange(x,y,rtSHALLOW,1) + HasInRange(x,y,rtRIVER,1);
		int n = getrandom(coastral>0 ? allraces : noncoastalraces);
		if(n > noncoastalraces-1)
		{
			race = typer->coastal_races[n-noncoastalraces-1];
		}
		else
		{
			race = typer->races[n];
		}
	}

	m_pMap[x][y].aPop[ItemDefs[race].index] = (pop + getrandom(pop)) / 2;
/*
	if(Globals->RANDOM_ECONOMY)
	{
		m_pMap[x][y].aPop[ItemDefs[race].index] = (pop + getgetrandom(pop)) / 2;
	}
	else
	{
		m_pMap[x][y].aPop[ItemDefs[race].index] = pop;
	}
*/	
}

int ARMGenerator::SortedRacesByPop(int x, int y, int place)
{
	if(place >= NUMMAN)
	{
		return -1;
	}

	int aPop[NUMMAN];

	int i;
	for(i=0; i<NUMMAN; i++)
	{
		aPop[i] = i;
	}

	bool changed;
	do
	{
		changed = false;
		for(i=1; i<NUMMAN; i++)
		{
			if(m_pMap[x][y].aPop[aPop[i-1]] < m_pMap[x][y].aPop[aPop[i]])
			{
				int temp;
				temp = aPop[i];
				aPop[i] = aPop[i-1];
				aPop[i-1] = temp;
				changed = true;
			}
		}
	}
	while(changed);

	if(m_pMap[x][y].aPop[aPop[place]] > 0)
	{
		return aPop[place];
	}

	return -1;
}

void ARMGenerator::DynamicDemographyOneStep()
{
	int x,y;

//рождаемость
	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<WIDTH; x++)
		{
			if(x%2 == y%2)
			{
				TerrainType * typer = &(TerrainDefs[GetStandartRType(m_pMap[x][y].type, Globals->ADVANCED_MAP)]);

				int pop = typer->pop;

				if(pop > 0)
				{
					int i;
					for(i=0; i<NUMMAN; i++)
					{
						m_pMap[x][y].aPop[i] = m_pMap[x][y].aPop[i] +  getrandom(0.02*m_pMap[x][y].aPop[i]);
					}
				}
			}
		}
	}
//миграци€
	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<WIDTH; x++)
		{
			if(x%2 == y%2)
			{
				TerrainType * typer = &(TerrainDefs[GetStandartRType(m_pMap[x][y].type, Globals->ADVANCED_MAP)]);

				int pop = typer->pop;

				if(pop > 0)
				{
					int i;
					for(i=0; i<NUMMAN; i++)
					{
						int k;
						int xx,yy;
						for(k=0; k<6; k++)
						{
							xx = x + rround[k].x;
							yy = y + rround[k].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								m_pMap[xx][yy].aPop[i] = m_pMap[xx][yy].aPop[i] + getrandom(0.02*m_pMap[x][y].aPop[i]);
							}
						}
					}
				}
			}
		}
	}
//разборки
	TerrainType *typerT = &(TerrainDefs[1]);

	int popEta = typerT->pop;
	int popEtaLess = popEta/10;


	int aPopDelta[NUMMAN];

	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<WIDTH; x++)
		{
			if(x%2 == y%2)
			{
				TerrainType * typer = &(TerrainDefs[GetStandartRType(m_pMap[x][y].type, Globals->ADVANCED_MAP)]);

				int pop = typer->pop;

				if(pop > 0)
				{
					int coastral;
					coastral = HasInRange(x,y,rtDEEPWATER,1) + HasInRange(x,y,rtSHALLOW,1) + HasInRange(x,y,rtRIVER,1);

					int i,j;
					for(i=0; i<NUMMAN; i++)
					{
						aPopDelta[i] = 0;
					}

					for(i=0; i<NUMMAN-1; i++)
					{
						if(m_pMap[x][y].aPop[i] > 0)
						{
							for(j=i+1; j<NUMMAN; j++)
							{
								if(m_pMap[x][y].aPop[j] > 0)
								{
									float kI,kJ;
									if(IsSpecificRegionRace(m_pMap[x][y].type, i, coastral))
									{
										kI = (float)popEta/pop;
//										m_pMap[x][y].aPop[i] = m_pMap[x][y].aPop[i] - 0.05*m_pMap[x][y].aPop[j];
									}
									else
									{
										kI = (float)popEtaLess/pop;
									}

									if(IsSpecificRegionRace(m_pMap[x][y].type, j, coastral))
									{
										kJ = (float)popEta/pop;
									}
									else
									{
										kJ = (float)popEtaLess/pop;
									}

									aPopDelta[i] += 0.05*kJ*m_pMap[x][y].aPop[j];
									aPopDelta[j] += 0.05*kI*m_pMap[x][y].aPop[i];
								}
							}
						}
					}
					for(i=0; i<NUMMAN; i++)
					{
						m_pMap[x][y].aPop[i] = m_pMap[x][y].aPop[i] - getrandom(aPopDelta[i]);
					}
				}
			}
		}
	}
//перенаселЄнность
	for(y=0; y<HEIGHT; y++)
	{
		for(x=0; x<WIDTH; x++)
		{
			if(x%2 == y%2)
			{
				TerrainType * typer = &(TerrainDefs[GetStandartRType(m_pMap[x][y].type, Globals->ADVANCED_MAP)]);

				int pop = typer->pop;

				int i;
				for(i=0; i<NUMMAN; i++)
				{
					if(m_pMap[x][y].aPop[i] < 0)
					{
						m_pMap[x][y].iCitySize = 0;
						m_pMap[x][y].aPop[i] = 0;
					}
/*
					if(m_pMap[x][y].aPop[i] > pop)
					{
						m_pMap[x][y].iCitySize = 1;
					}
					if(m_pMap[x][y].aPop[i] > pop*2)
					{
						m_pMap[x][y].iCitySize = 2;
					}
*/
					if(m_pMap[x][y].aPop[i] > pop)
					{
						m_pMap[x][y].iCitySize = 3;
						m_pMap[x][y].aPop[i] = pop;
					}
				}
			}
		}
	}
}

bool ARMGenerator::IsSpecificRegionRace(int rType, int race, int coastal)
{
	TerrainType * typer = &(TerrainDefs[GetStandartRType((RegionType)rType, Globals->ADVANCED_MAP)]);

	int i;
	for(i=0; i<4; i++)
	{
		if(typer->races[i] == RaceToItem(race))
		{
			return true;
		}
	}

	if(coastal == 0)
	{
		return false;
	}

	for(i=0; i<3; i++)
	{
		if(typer->coastal_races[i] == RaceToItem(race))
		{
			return true;
		}
	}

	return false;
}

AString ARMGenerator::GetLakeName(int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Lake";

	AString sResult, sNum;


//	sResult = (AString)regnames[nameID%1000];
	sResult = (AString)GetAbstractName(nameID*nameID).Str();
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area == 1)
	{
//		if(nameID%2 == 1)
			sResult = sResult + " Lake";
//		else
//			sResult = sResult + " Woods";
	}
	else
	{
		sResult = sResult + " Lakes";
	}
	return sResult + sNum;
}

AString ARMGenerator::GetTundraName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Tundra";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

//	int totalBound, boundsCount, area;
//	totalBound = 0;
//	boundsCount = 0;

//	area = m_pProvinceInfo->GetArea(nameID);


//	if(area<15)
//	{
//		if(nameID%2 == 1)
			sResult = sResult + " Tundra";
//		else
//			sResult = sResult + " Valley";
//
//		return sResult + sNum;
//	}

//	sResult = "Great " + sResult + " Plains";
	return sResult + sNum;
}

int ARMGenerator::GetAreaPrevealedRace(int x, int y)
{
//	int aPop[NUMMAN];
	int *aPop;
	int aPopID[NUMMAN];

	aPop = new int[NUMMAN];

	int i;
	for(i=0; i<NUMMAN; i++)
	{
		aPop[i] = 0;
		aPopID[i] = i;
	}

	aPop = CountAreaRaces(aPop, x, y);

	bool changed;
	do
	{
		changed = false;
		for(i=1; i<NUMMAN; i++)
		{
			if(aPop[aPopID[i-1]] < aPop[aPopID[i]])
			{
				int temp;
				temp = aPopID[i];
				aPopID[i] = aPopID[i-1];
				aPopID[i-1] = temp;
				changed = true;
			}
		}
	}
	while(changed);

	if(aPop[aPopID[0]] > 0)
	{
		delete[] aPop;
		return aPopID[0];
	}

	delete[] aPop;
	return -1;
}

int * ARMGenerator::CountAreaRaces(int *aPop, int x, int y, bool start)
{
	if(m_pMap == NULL)
	{
		return aPop;
	}

	if(aPop == NULL)
	{
		return aPop;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return aPop;
	}

	if(start)
	{
		tempCountValue++;
	}

	m_pMap[x][y].count = tempCountValue;

	int xx,yy;

//	bResult = false;

	int i;
	for(i=0; i<NUMMAN; i++)
	{
		aPop[i] += m_pMap[x][y].aPop[i];
	}

	for(i=0; i<6; i++)
	{
		xx = x + rround[i].x;
		yy = y + rround[i].y;
		while(xx<0)
		{
			xx += WIDTH;
		}
		while(xx>=WIDTH)
		{
			xx -= WIDTH;
		}

		if(yy>=0 && yy<HEIGHT)
		{
			if(m_pMap[x][y].island)
			{
				if((m_pMap[xx][yy].type != rtDEEPWATER && m_pMap[xx][yy].type != rtSHALLOW && m_pMap[xx][yy].type != rtRIVER) && m_pMap[xx][yy].count != tempCountValue)
				{
					aPop = CountAreaRaces(aPop, xx, yy, false);
				}
			}
			else
			{
				if(m_pMap[xx][yy].nameID == m_pMap[x][y].nameID && m_pMap[xx][yy].count != tempCountValue)
				{
					aPop = CountAreaRaces(aPop, xx, yy, false);
				}
			}
		}
	}
	return aPop;
}

bool ARMGenerator::CanBeHill(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)
		return false;

	if(GetMaxDelta(x,y)<4)
		return false;

//	if(anchor)
//	{
//		if(IsExtremum(x,y) != 1)
//			return false;
//	}
//	else
//	{
		if(HasInRange(x,y,rtMONTANA,1)*HasInRange(x,y,rtMONTANA,1) < getrandom(6))
			return false;
//	}

	return true;
}

void ARMGenerator::CalculateTemperature()
{
	if(m_pMap == NULL)
	{
		return;
	}
	int i,j;
	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			m_pMap[i][j].iTemperature = 0;
		}
	}

	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			if(i%2 == j%2)
			{
				switch(m_iClimate)
				{
					case ctCLASSIC:
						{
							int plate;
							plate = sqrt((HEIGHT/2+1)*(HEIGHT/2+1) - (j-HEIGHT/2)*(j-HEIGHT/2));
							m_pMap[i][j].iTemperature = plate*m_iT/(HEIGHT/2);
						}
						break;
					case ctETERNALMIDNIGHT:
						{
							int plate;
							plate = sqrt(4*(i-WIDTH/2)*(i-WIDTH/2) + (j-HEIGHT/2)*(j-HEIGHT/2));
							m_pMap[i][j].iTemperature = m_iT - plate*m_iT/(HEIGHT/2);
						}
						break;
					case ctETERNALTWILIGHT:
					default:
						break;
				}

				if(m_pMap[i][j].h < water || m_pMap[i][j].type == rtRIVER)
				{
					m_pMap[i][j].iTemperature = (m_pMap[i][j].iTemperature + m_iT)/2;
				}

				if(m_pMap[i][j].type == rtSHALLOW)
				{
					m_pMap[i][j].iTemperature++;
				}

				if(m_pMap[i][j].type == rtVOLKANO)
				{
					m_pMap[i][j].iTemperature+=10;
				}
			}
		}
	}

	for(int k=0; k<WIDTH*HEIGHT/2; k++)
	{
		int x,y;
		x = getrandom(WIDTH);
		y = getrandom(HEIGHT);

		if(x%2 == y%2)
		{
			for(int i=0; i<6; i++)
			{
				int xx = x + rround[i].x;
				int yy = y + rround[i].y;
				while(xx<0)
				{
					xx += WIDTH;
				}
				while(xx>=WIDTH)
				{
					xx -= WIDTH;
				}

				if(yy>=0 && yy<HEIGHT)
				{
					if(m_pMap[xx][yy].type == rtVOLKANO || m_pMap[xx][yy].type == rtSHALLOW)
					{
						m_pMap[x][y].iTemperature = 0.5+(m_pMap[x][y].iTemperature + 3*m_pMap[xx][yy].iTemperature)/4;
//						m_pMap[x][y].iTemperature = 0.5+(m_pMap[x][y].iTemperature + m_pMap[xx][yy].iTemperature)/2;
					}
					else
					{
						m_pMap[xx][yy].iTemperature = 0.5+(3*m_pMap[x][y].iTemperature + m_pMap[xx][yy].iTemperature)/4;
//						m_pMap[xx][yy].iTemperature = 0.5+(m_pMap[x][y].iTemperature + m_pMap[xx][yy].iTemperature)/2;
					}
				}
			}
		}
	}

}

bool ARMGenerator::CheckTemperature(int x, int y, int tRegion)
{
	int a,b,t;
	a = aTemperature[tRegion][0] - m_pMap[x][y].iTemperature;
	b = m_pMap[x][y].iTemperature - aTemperature[tRegion][1];
	if(a<0) a=0;
	if(b<0) b=0;
	if(a<b)
		t = b;
	else
		t = a;

	if(t == 0)
		return true;

//	if(t>aTemperature[tRegion][1]-aTemperature[tRegion][0] || getrandom(t*t*t) != 0)
	if(getrandom(aTemperature[tRegion][1]-aTemperature[tRegion][0]) < t)
		return false;

	return true;
}

bool ARMGenerator::CheckHumidity(int x, int y, int tRegion)
{
	int a,b,t;
	a = aHumidity[tRegion][0] - aHumidity[tRegion][1] - m_pMap[x][y].iHumidity;
	b = m_pMap[x][y].iHumidity - aHumidity[tRegion][0] - aHumidity[tRegion][1];
	if(a<b)
		t = b;
	else
		t = a;

	if(t <= 0)
		return true;

//	if(t>aTemperature[tRegion][1]-aTemperature[tRegion][0] || getrandom(t*t*t) != 0)
	if(getrandom(aHumidity[tRegion][1]) < t)
		return false;

	return true;
}

int ARMGenerator::AddElementAtRandom(int tRegion, int mass)
{
	int i,j;
	i = getrandom(WIDTH);
	do
	{
		j = getrandom(HEIGHT);
	}
	while(i % 2 != j % 2);

	if(CanBeElement(i, j, RegionType(tRegion)))
	{
		m_pMap[i][j].type = RegionType(tRegion);
		m_pMap[i][j].nameID = maxID++;

		int newArea;
		newArea = GrowElement(RegionType(tRegion), i, j, mass);
		if(newArea > 1)
		{
			return newArea;
		}
		else
		{
			m_pMap[i][j].type = rtPLAIN;
			m_pMap[i][j].nameID = -1;
		}
	}

	return 0;
}

void ARMGenerator::FixDeserts()
{
	int i,j;
	for(j=0; j<HEIGHT; j++)
		for(i=0; i<WIDTH; i++)
		{
			if(i % 2 == j % 2)
			{
				//одиночный пустыни  разрешены только на побережье
				if(m_pMap[i][j].type == rtDESERT)
				{
					if((HasInRange(i,j,rtDEEPWATER,1) + HasInRange(i,j,rtSHALLOW,1) < 1) &&
					   (HasInRange(i,j,rtDESERT,1) < 2))
					{
						m_pMap[i][j].type = rtPLAIN;
					}
				}
				//сглаживаем контуры пустынь - удал€ем "€зыки" равнин
				if(m_pMap[i][j].type == rtPLAIN)
				{
					if(HasInRange(i,j,rtDESERT,1) > HasInRange(i,j,rtPLAIN,1))
					{
						m_pMap[i][j].type = rtDESERT;
						for(int z=0; z<6; z++)
						{
							int xx,yy;
							xx = i + rround[z].x;
							yy = j + rround[z].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								if(m_pMap[xx][yy].type == rtDESERT)
								{
									m_pMap[i][j].nameID = m_pMap[xx][yy].nameID;
								}
							}
						}
					}
				}
			}
		}
}

void ARMGenerator::FixSwamps()
{
	int i,j;
	for(j=0; j<HEIGHT; j++)
		for(i=0; i<WIDTH; i++)
		{
			if(i % 2 == j % 2)
			{
				//удал€ем одиночные болота
				if(m_pMap[i][j].type == rtSWAMP)
				{
					if(HasInRange(i,j,rtSWAMP,1) < 2)
					{
						m_pMap[i][j].type = rtPLAIN;
						for(int z=0; z<6; z++)
						{
							int xx,yy;
							xx = i + rround[z].x;
							yy = j + rround[z].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								if(m_pMap[xx][yy].type == rtPLAIN)
								{
									m_pMap[i][j].nameID = m_pMap[xx][yy].nameID;
								}
							}
						}
					}
				}
			}
		}
}

void ARMGenerator::FixForestnJungle()
{
	int i,j;
	for(j=0; j<HEIGHT; j++)
		for(i=0; i<WIDTH; i++)
		{
			if(i % 2 == j % 2)
			{
				//удал€ем одиночные леса, оставл€ем только на побережье
				if(m_pMap[i][j].type == rtFOREST || m_pMap[i][j].type == rtJUNGLE)
				{
					if((HasInRange(i,j,rtDEEPWATER,1) + HasInRange(i,j,rtSHALLOW,1) < 3) &&
					   (HasInRange(i,j,rtFOREST,1) + HasInRange(i,j,rtJUNGLE,1) < 2))
					{
						m_pMap[i][j].type = rtPLAIN;
						for(int z=0; z<6; z++)
						{
							int xx,yy;
							xx = i + rround[z].x;
							yy = j + rround[z].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								if(m_pMap[xx][yy].type == rtPLAIN)
								{
									m_pMap[i][j].nameID = m_pMap[xx][yy].nameID;
								}
							}
						}
					}
				}
				//удал€ем "пол€ны" в лесах
				if(m_pMap[i][j].type == rtPLAIN)
				{
					if(HasInRange(i,j,rtFOREST,1) + HasInRange(i,j,rtJUNGLE,1) + HasInRange(i,j,rtSWAMP,1) == 6)
//					if((HasInRange(i,j,rtWATER,1) < 2) &&
//					   (HasInRange(i,j,rtPLAIN,1) < 2))
					{
						m_pMap[i][j].type = rtSWAMP;
						for(int z=0; z<6; z++)
						{
							int xx,yy;
							xx = i + rround[z].x;
							yy = j + rround[z].y;
							while(xx<0)
							{
								xx += WIDTH;
							}
							while(xx>=WIDTH)
							{
								xx -= WIDTH;
							}

							if(yy>=0 && yy<HEIGHT)
							{
								if(m_pMap[xx][yy].type == rtSWAMP || m_pMap[xx][yy].type == rtFOREST || m_pMap[xx][yy].type == rtJUNGLE)
								{
									m_pMap[i][j].nameID = m_pMap[xx][yy].nameID;
								}
							}
						}
					}
				}
			}
		}
}

AString ARMGenerator::GetHillsName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Hills";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
//	sNum = " (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area==1)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Hill";
		else
			sResult = sResult + " Barrow";

		return sResult + sNum;
	}

	if(area<15)
	{
		if(nameID%3 == 1)
			sResult = sResult + " Hills";
		else
		{
			if(nameID%2 == 1)
				sResult = sResult + " Heights";
			else
				sResult = sResult + " Barrows";
		}
		return sResult + sNum;
	}

	sResult = (AString)"Great " + sResult + " Hills";
	return sResult + sNum;
}

AString ARMGenerator::GetVolcanoName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Volkano";

	AString sResult, sNum;


	sResult = sBase;
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	//int area;
	//area = m_pProvinceInfo->GetArea(nameID);


	if(nameID%2 == 1)
		sResult = sResult + " Volkano";
	else
		sResult = sResult + " Peak";

	return sResult + sNum;
}

void ARMGenerator::GenerateComplex(int iContinents, int iAge, int iT, int iWet, float fNeedWater, float fNeedMontanas, int bounds)
{
	if(m_pMap == NULL)
	{
		return;
	}

	m_iAge = iAge;
	m_iT = iT;
	m_iMoisure = iWet;

	CFractalMap *pFractal;
	pFractal = new CFractalMap();

	pFractal->CreateWorld(WIDTH/2, HEIGHT, 40, 1, iAge, 0);

	int x,y;
	for(x=WIDTH/8; x<WIDTH/2-WIDTH/8; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			int trueX;
			trueX = x;
			int h = pFractal->GetHeight(x,y)*20.0/256.0;
			if(m_pMap[trueX][y].h < h)
				m_pMap[trueX][y].h = h;
		}
	}

	pFractal->CreateWorld(WIDTH, HEIGHT, 70, 50, iAge, 0);

	for(x=WIDTH/4; x<WIDTH-WIDTH/4; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			int trueX;
			trueX = x;
			int h = pFractal->GetHeight(x,y)*20.0/256.0;
			if(m_pMap[trueX][y].h < h)
				m_pMap[trueX][y].h = h;
		}
	}

	pFractal->CreateWorld(WIDTH/2, HEIGHT, 40, 1, iAge, 0);

	for(x=WIDTH/8; x<WIDTH/2-WIDTH/8; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			int trueX;
			trueX = WIDTH/2 + x;
			int h = pFractal->GetHeight(x,y)*20.0/256.0;
			if(m_pMap[trueX][y].h < h)
				m_pMap[trueX][y].h = h;
		}
	}

	pFractal->CreateWorld(WIDTH, HEIGHT, 70, 50, iAge, 0);

	for(x=WIDTH/4; x<WIDTH-WIDTH/4; x++)
	{
		for(y=0; y<HEIGHT; y++)
		{
			int trueX;
			trueX = WIDTH/2 + x;
			if(trueX >= WIDTH)
				trueX = trueX-WIDTH;
			int h = pFractal->GetHeight(x,y)*20.0/256.0;
			if(m_pMap[trueX][y].h < h)
				m_pMap[trueX][y].h = h;
		}
	}
//	iMaxHeight = 256;
//	water = 128;
	iMaxHeight = 20;
	water = 10;

	delete pFractal;

//	SetTypes(true);
	BalanceMap(fNeedWater, fNeedMontanas);

	RandomizeMap();
	SetTypes();
}

bool ARMGenerator::CanBeWastes(int x, int y, bool anchor)
{
	if(m_pMap == NULL)
	{
		return false;
	}

	if(x%2 != y%2)
		y++;

	while(x<0)
	{
		x += WIDTH;
	}
	while(x>=WIDTH)
	{
		x -= WIDTH;
	}

	if(y<0 || y>=HEIGHT)
	{
		return false;
	}

	if(m_pMap[x][y].type != rtPLAIN)
		return false;

//	if(m_pMap[x][y].iHumidity > aHumidity[rtDESERT][1]+1)
//		return false;

	if(getrandom(HasInRange(x,y,rtSWAMP,2)) != 0)
		return false;

	if(getrandom(3) != 0)
		return false;

	if(anchor)
	{
		if(HasInRange(x,y,rtLAKE,2) + HasInRange(x,y,rtRIVER,2) > 0)
			return false;

		if(HasInRange(x,y,rtLAKE,4) + HasInRange(x,y,rtRIVER,4) > 0 && getrandom(4) != 0)
			return false;

		if(HasInRange(x,y,rtDESERT,2) > 0)
			return false;
	}
	else
	{
		if(HasInRange(x,y,rtLAKE,1) + HasInRange(x,y,rtRIVER,1) > 0)
			return false;

		if(HasInRange(x,y,rtLAKE,3) + HasInRange(x,y,rtRIVER,3) > 0 && getrandom(4) != 0)
			return false;
	}

	return true;
}

AString ARMGenerator::GetWastelandsName(AString sBase, int nameID)
{
	if(m_pProvinceInfo == NULL)
		return "!!! not ready !!!";

	if(nameID == -1 || nameID>=maxID)
		return "Nameless Wastelands";

	AString sResult, sNum;


	sResult = sBase;
//	sResult = (AString)regnames[nameID%1000];
	sNum = "";//" (S=" + IntToStr(m_pSeasInfo[nameID][maxID]) + ")";

	int area;
//	totalBound = 0;
//	boundsCount = 0;

	area = m_pProvinceInfo->GetArea(nameID);


	if(area<15)
	{
		if(nameID%2 == 1)
			sResult = sResult + " Wastes";
		else
			sResult = sResult + " Wastelands";
	}
	else
	{
		sResult = (AString)"Great " + sResult + " Wastes";
	}
	return sResult + sNum;
}

void ARMGenerator::SetRareResources()
{
	if(m_pMap == NULL)
	{
		return;
	}

	int *aRareResourcesCountByContinents;
	int *aRareResourcesCountByProvinces;

	aRareResourcesCountByContinents = new int[maxContiID+1];
	aRareResourcesCountByProvinces = new int[maxID];

	int i,j,k;
	for(i=0; i<WIDTH; i++)
		for(j=0; j<HEIGHT; j++)
		{
			if(i % 2 == j % 2)
			{
				m_pMap[i][j].bRareResource = false;
			}
		}

	for(i=0; i<maxContiID+1; i++)
		aRareResourcesCountByContinents[i] = 0;

	for(i=0; i<maxID; i++)
		aRareResourcesCountByProvinces[i] = 0;

	int totalByType;
  for(int t=rtDEEPWATER; t<rtWASTELANDS; t++)
  {
	totalByType = 0;
	for(k=0; k<HEIGHT*WIDTH; k++)
	{
		i = getrandom(WIDTH);
		do
		{
			j = getrandom(HEIGHT);
		}
		while(i % 2 != j % 2);

		if(m_pMap[i][j].contiID+1 >= 0 && m_pMap[i][j].contiID+1 < maxContiID+1)
		{
			if(m_pMap[i][j].type == t && aRareResourcesCountByProvinces[m_pMap[i][j].nameID] == 0)
			{
				int chance;
				chance = HEIGHT/12 - aRareResourcesCountByContinents[m_pMap[i][j].contiID+1];
				if(getrandom(HEIGHT/12) < chance && getrandom(HEIGHT/40) >= totalByType)
				{
					m_pMap[i][j].bRareResource = true;
					aRareResourcesCountByProvinces[m_pMap[i][j].nameID] = 1;
					aRareResourcesCountByContinents[m_pMap[i][j].contiID+1]++;
					totalByType++;
				}
			}
		}
	}
  }

	delete[] aRareResourcesCountByContinents;
	delete[] aRareResourcesCountByProvinces;
}

ARMProvinceInfoTable::~ARMProvinceInfoTable()
{
	for(int i=0; i<HASH_SIZE; i++)
	{
		while(m_pProvinces[i])
		{
			Delete(m_pProvinces[i]);
		}
	}
}

ARMProvinceInfoTable::ARMProvinceInfoTable()
{
	for(int i=0; i<HASH_SIZE; i++)
	{
		m_pProvinces[i] = NULL;
		size[i] = 0;
	}
}

void ARMProvinceInfoTable::Delete(TARMProvince *pProvince)
{
	if(!pProvince)
		return;

	int hash;
	hash = Hash(pProvince->m_iID);

	if(pProvince->m_pPrev)
	{
		pProvince->m_pPrev->m_pNext = pProvince->m_pNext;
		if(pProvince->m_pNext)
			pProvince->m_pNext->m_pPrev = pProvince->m_pPrev;
	}
	else
	{
		m_pProvinces[hash] = pProvince->m_pNext;
		if(m_pProvinces[hash])
			m_pProvinces[hash]->m_pPrev = NULL;
	}
	size[hash]--;

	if(pProvince->m_pBorders)
		delete[] pProvince->m_pBorders;

	delete pProvince;
}

int ARMProvinceInfoTable::GetBorder(int iID1, int iID2)
{
	TARMProvince *pCurrent;
	pCurrent = Find(iID1);

	if(!pCurrent)
		return 0;

	int i;
	for(i=0; i<pCurrent->m_iBordersCount; i++)
	{
		if(pCurrent->m_pBorders[i].m_iID == iID2)
			return pCurrent->m_pBorders[i].m_iLength;
	}

	return 0;
}

TARMProvince * ARMProvinceInfoTable::Find(int iID)
{
	TARMProvince *pCurrent;

	int hash;
	hash = Hash(iID);

	pCurrent = m_pProvinces[hash];

	while(pCurrent)
	{
		if(pCurrent->m_iID == iID)
		{
			return pCurrent;
		}
		pCurrent = pCurrent->m_pNext;
	}

	return NULL;
}

TARMProvince * ARMProvinceInfoTable::Add(int iID)
{
	TARMProvince *pProvince;

	pProvince = Find(iID);

	if(!pProvince)
	{
		int hash;
		hash = Hash(iID);

		pProvince = new TARMProvince();

		pProvince->m_iID = iID;
		pProvince->m_iArea = 0;
		pProvince->m_iBordersCount = 0;
		pProvince->m_pBorders = NULL;
		pProvince->m_pPrev = NULL;
		pProvince->m_pNext = m_pProvinces[hash];

		if(m_pProvinces[hash])
			m_pProvinces[hash]->m_pPrev = pProvince;

		m_pProvinces[hash] = pProvince;
		size[hash]++;
	}
	return pProvince;
}

void ARMProvinceInfoTable::SetArea(int iID, int iArea)
{
	TARMProvince *pCurrent;
	pCurrent = Find(iID);

	if(pCurrent != NULL)
		pCurrent->m_iArea = iArea;
}

int ARMProvinceInfoTable::GetArea(int iID)
{
	TARMProvince *pCurrent;
	pCurrent = Find(iID);

	if(!pCurrent)
		return 0;

	return pCurrent->m_iArea;
}

void ARMProvinceInfoTable::SetBorder(int iID1, int iID2, int iLength)
{
	if(iLength <= 0)
	{
		DeleteBorder(iID1, iID2);
		return;
	}

	TARMProvince *pProvince1;
	pProvince1 = Add(iID1);

	TARMProvince *pProvince2;
	pProvince2 = Add(iID2);

	bool found;
	int i;
	found = false;
	for(i=0; i<pProvince1->m_iBordersCount; i++)
	{
		if(pProvince1->m_pBorders[i].m_iID == iID2)
		{
			pProvince1->m_pBorders[i].m_iLength = iLength;
			found = true;
		}
	}
	if(!found)
	{
		TARMProvinceBorder *pBorders;
		pBorders = new TARMProvinceBorder[pProvince1->m_iBordersCount+1];
		for(i=0; i<pProvince1->m_iBordersCount; i++)
		{
			pBorders[i].m_iID = pProvince1->m_pBorders[i].m_iID;
			pBorders[i].m_iLength = pProvince1->m_pBorders[i].m_iLength;
		}
		pBorders[pProvince1->m_iBordersCount].m_iID = iID2;
		pBorders[pProvince1->m_iBordersCount].m_iLength = iLength;

		delete[] pProvince1->m_pBorders;
		pProvince1->m_pBorders = pBorders;
		pProvince1->m_iBordersCount++;
	}

	found = false;
	for(i=0; i<pProvince2->m_iBordersCount; i++)
	{
		if(pProvince2->m_pBorders[i].m_iID == iID1)
		{
			pProvince2->m_pBorders[i].m_iLength = iLength;
			found = true;
		}
	}
	if(!found)
	{
		TARMProvinceBorder *pBorders;
		pBorders = new TARMProvinceBorder[pProvince2->m_iBordersCount+1];
		for(i=0; i<pProvince2->m_iBordersCount; i++)
		{
			pBorders[i].m_iID = pProvince2->m_pBorders[i].m_iID;
			pBorders[i].m_iLength = pProvince2->m_pBorders[i].m_iLength;
		}
		pBorders[pProvince2->m_iBordersCount].m_iID = iID1;
		pBorders[pProvince2->m_iBordersCount].m_iLength = iLength;

		delete[] pProvince2->m_pBorders;
		pProvince2->m_pBorders = pBorders;
		pProvince2->m_iBordersCount++;
	}
}

TARMProvince * ARMProvinceInfoTable::Merge(int iID1, int iID2)
{
	TARMProvince *pProvince1;
	pProvince1 = Find(iID1);

	TARMProvince *pProvince2;
	pProvince2 = Find(iID2);

	if(!pProvince1 || !pProvince2)
		return NULL;

	pProvince1->m_iArea = pProvince1->m_iArea + pProvince2->m_iArea;

	TARMProvinceBorder *pBorders;
	pBorders = new TARMProvinceBorder[pProvince1->m_iBordersCount + pProvince2->m_iBordersCount];
	int i, j, index;
	index = 0;
	for(i=0; i<pProvince1->m_iBordersCount; i++)
	{
		if(pProvince1->m_pBorders[i].m_iID != iID2)
		{
			pBorders[index].m_iID = pProvince1->m_pBorders[i].m_iID;
			pBorders[index].m_iLength = pProvince1->m_pBorders[i].m_iLength;
			index++;
		}
	}
	for(i=0; i<pProvince2->m_iBordersCount; i++)
	{
		bool found;
		found = false;
		if(pProvince2->m_pBorders[i].m_iID != iID1)
		{
			for(j=0; j<index; j++)
			{
				if(pBorders[j].m_iID == pProvince2->m_pBorders[i].m_iID)
				{
					pBorders[j].m_iLength = pBorders[j].m_iLength + pProvince2->m_pBorders[i].m_iLength;
					found = true;
				}
			}
			if(!found)
			{
				pBorders[index].m_iID = pProvince2->m_pBorders[i].m_iID;
				pBorders[index].m_iLength = pProvince2->m_pBorders[i].m_iLength;
				index++;
			}
			IncreaseBorder(pProvince2->m_pBorders[i].m_iID, iID1, pProvince2->m_pBorders[i].m_iLength);
			SetBorder(pProvince2->m_pBorders[i].m_iID, iID2, 0);
		}
	}

	if(pProvince1->m_pBorders)
		delete[] pProvince1->m_pBorders;
	pProvince1->m_pBorders = new TARMProvinceBorder[index];
	for(i=0; i<index; i++)
	{
		pProvince1->m_pBorders[i].m_iID = pBorders[i].m_iID;
		pProvince1->m_pBorders[i].m_iLength = pBorders[i].m_iLength;
	}
	pProvince1->m_iBordersCount = index;
	delete[] pBorders;

	Delete(pProvince2);

	return pProvince1;
}

void ARMProvinceInfoTable::IncreaseBorder(int iID1, int iID2, int iLength)
{
	TARMProvince *pProvince1;
	pProvince1 = Add(iID1);

	TARMProvince *pProvince2;
	pProvince2 = Add(iID2);

	bool found;
	int i;
	found = false;
	for(i=0; i<pProvince1->m_iBordersCount; i++)
	{
		if(pProvince1->m_pBorders[i].m_iID == iID2)
		{
			pProvince1->m_pBorders[i].m_iLength += iLength;
			found = true;
		}
	}
	if(!found)
	{
		TARMProvinceBorder *pBorders;
		pBorders = new TARMProvinceBorder[pProvince1->m_iBordersCount+1];
		for(i=0; i<pProvince1->m_iBordersCount; i++)
		{
			pBorders[i].m_iID = pProvince1->m_pBorders[i].m_iID;
			pBorders[i].m_iLength = pProvince1->m_pBorders[i].m_iLength;
		}
		pBorders[pProvince1->m_iBordersCount].m_iID = iID2;
		pBorders[pProvince1->m_iBordersCount].m_iLength = iLength;

		delete[] pProvince1->m_pBorders;
		pProvince1->m_pBorders = pBorders;
		pProvince1->m_iBordersCount++;
	}

	found = false;
	for(i=0; i<pProvince2->m_iBordersCount; i++)
	{
		if(pProvince2->m_pBorders[i].m_iID == iID1)
		{
			pProvince2->m_pBorders[i].m_iLength += iLength;
			found = true;
		}
	}
	if(!found)
	{
		TARMProvinceBorder *pBorders;
		pBorders = new TARMProvinceBorder[pProvince2->m_iBordersCount+1];
		for(i=0; i<pProvince2->m_iBordersCount; i++)
		{
			pBorders[i].m_iID = pProvince2->m_pBorders[i].m_iID;
			pBorders[i].m_iLength = pProvince2->m_pBorders[i].m_iLength;
		}
		pBorders[pProvince2->m_iBordersCount].m_iID = iID1;
		pBorders[pProvince2->m_iBordersCount].m_iLength = iLength;

		delete[] pProvince2->m_pBorders;
		pProvince2->m_pBorders = pBorders;
		pProvince2->m_iBordersCount++;
	}
}

void ARMProvinceInfoTable::DeleteBorder(int iID1, int iID2)
{
	TARMProvince *pProvince1;
	pProvince1 = Find(iID1);

	TARMProvince *pProvince2;
	pProvince2 = Find(iID2);

	if(!pProvince1 || !pProvince2)
		return;

	TARMProvinceBorder *pBorders;
	pBorders = new TARMProvinceBorder[pProvince1->m_iBordersCount];
	int i, index;
	index = 0;
	for(i=0; i<pProvince1->m_iBordersCount; i++)
	{
		if(pProvince1->m_pBorders[i].m_iID != iID2)
		{
			pBorders[index].m_iID = pProvince1->m_pBorders[i].m_iID;
			pBorders[index].m_iLength = pProvince1->m_pBorders[i].m_iLength;
			index++;
		}
	}

	if(pProvince1->m_pBorders)
		delete[] pProvince1->m_pBorders;
	pProvince1->m_pBorders = new TARMProvinceBorder[index];
	for(i=0; i<index; i++)
	{
		pProvince1->m_pBorders[i].m_iID = pBorders[i].m_iID;
		pProvince1->m_pBorders[i].m_iLength = pBorders[i].m_iLength;
	}
	pProvince1->m_iBordersCount = index;
	delete[] pBorders;

	pBorders = new TARMProvinceBorder[pProvince2->m_iBordersCount];
	index = 0;
	for(i=0; i<pProvince2->m_iBordersCount; i++)
	{
		if(pProvince2->m_pBorders[i].m_iID != iID1)
		{
			pBorders[index].m_iID = pProvince2->m_pBorders[i].m_iID;
			pBorders[index].m_iLength = pProvince2->m_pBorders[i].m_iLength;
			index++;
		}
	}

	if(pProvince2->m_pBorders)
		delete[] pProvince2->m_pBorders;
	pProvince2->m_pBorders = new TARMProvinceBorder[index];
	for(i=0; i<index; i++)
	{
		pProvince2->m_pBorders[i].m_iID = pBorders[i].m_iID;
		pProvince2->m_pBorders[i].m_iLength = pBorders[i].m_iLength;
	}
	pProvince2->m_iBordersCount = index;
	delete[] pBorders;
}

int ARMProvinceInfoTable::Hash(int iID)
{
//	iID *= iID;           // возвести ключ в квадрат
//	iID >>= 11;           // отбросить 11 младших бит

	iID *= 137;
	return iID%HASH_SIZE;
}

int ARMProvinceInfoTable::GetBordersCount(int iID)
{
	TARMProvince *pCurrent;
	pCurrent = Find(iID);

	if(pCurrent != NULL)
		return pCurrent->m_iBordersCount;

	return 0;
}

void ARMGenerator::FixShallows()
{
	int i,j;
	for(j=0; j<HEIGHT; j++)
		for(i=0; i<WIDTH; i++)
		{
			if(i % 2 == j % 2)
			{
				//удал€ем одиночные болота
				if(m_pMap[i][j].type == rtSHALLOW)
				{
					if((HasInRange(i,j,rtDEEPWATER,1) >= 5 && HasInRange(i,j,rtDEEPWATER,1)+HasInRange(i,j,rtSHALLOW,1)==6) ||
					   (FindLand(i,j,3) == 3))
					{
						m_pMap[i][j].type = rtDEEPWATER;
						m_pMap[i][j].h = water-3;
					}
				}
			}
		}
}

void ARMGenerator::GenerateUnderground()
{
//	rtSOLIDROCK,
//	rtTUNNEL,
//	rtCAVE,
//	rtUNDERFOREST,
//	rtLAVALAKE,

	int i,j;
	for(i=0; i<WIDTH/2; i++)
	{
		for(j=0; j<HEIGHT/2; j++)
		{
			m_pUnderMap[i][j].h = m_pMap[i*2][j*2].h +1 -getrandom(2);

			if(m_pUnderMap[i][j].h >= water+1)
				m_pUnderMap[i][j].type = rtCAVE;
			else
				m_pUnderMap[i][j].type = rtSOLIDROCK;

			if(m_pUnderMap[i][j].h >= (water+iMaxHeight)/2 - 1)
				m_pUnderMap[i][j].type = rtUNDERFOREST;

			if(m_pUnderMap[i][j].h >= water + 2 && getrandom(20) == 1)
				m_pUnderMap[i][j].type = rtUNDERFOREST;

			if(m_pUnderMap[i][j].h >= (water+iMaxHeight)/2 && getrandom(3) != 1)
				m_pUnderMap[i][j].type = rtDEEPWATER;

			m_pUnderMap[i][j].count = -1;
			m_pUnderMap[i][j].nameID = -1;
			m_pUnderMap[i][j].contiID = -1;
			m_pUnderMap[i][j].iHumidity = 0;
			m_pUnderMap[i][j].island = false;
			m_pUnderMap[i][j].riverID = -1;
			m_pUnderMap[i][j].riverNameID = -1;
			m_pUnderMap[i][j].bRareResource = false;
			m_pUnderMap[i][j].iCitySize = 0;
			int k;
			for(k=0; k<NUMMAN; k++)
			{
				m_pUnderMap[i][j].aPop[k] = 0;
			}
		}
	}

	for(i=0; i<WIDTH; i++)
	{
		for(j=0; j<HEIGHT; j++)
		{
			if(m_pMap[i][j].type == rtVOLKANO)
			{
				m_pUnderMap[i/2][j/2].type = rtSOLIDROCK;//rtLAVALAKE;
				m_pUnderMap[i/2][j/2].nameID = m_pMap[i][j].nameID;

				for(int z=0; z<6; z++)
				{
					int xx,yy;
					xx = i/2 + rround[z].x;
					yy = j/2 + rround[z].y;
					while(xx<0)
					{
						xx += WIDTH/2;
					}
					while(xx>=WIDTH/2)
					{
						xx -= WIDTH/2;
					}

					if(yy>=0 && yy<HEIGHT/2 && getrandom(4) != 1)
					{
						if(m_pUnderMap[xx][yy].type != rtSOLIDROCK)
						{
							m_pUnderMap[xx][yy].type = rtLAVALAKE;
							m_pUnderMap[xx][yy].nameID = m_pMap[i][j].nameID;
						}
					}
				}
			}
		}
	}

	for(i=0; i<WIDTH/2; i++)
	{
		for(j=0; j<HEIGHT/2; j++)
		{
			if(m_pUnderMap[i][j].type == rtCAVE)
			{
				int count;
				count = 0;
				for(int z=0; z<6; z++)
				{
					int xx,yy;
					xx = i + rround[z].x;
					yy = j + rround[z].y;
					while(xx<0)
					{
						xx += WIDTH/2;
					}
					while(xx>=WIDTH/2)
					{
						xx -= WIDTH/2;
					}

					if(yy>=0 && yy<HEIGHT/2)
					{
						if(m_pUnderMap[xx][yy].type == rtUNDERFOREST)
						{
							count++;
						}
					}
				}
				if(count >= 3)
				{
					m_pUnderMap[i][j].type = rtUNDERFOREST;
				}
			}
			if(m_pUnderMap[i][j].type == rtUNDERFOREST || m_pUnderMap[i][j].type == rtDEEPWATER)
			{
				for(int z=0; z<6; z++)
				{
					int xx,yy;
					xx = i + rround[z].x;
					yy = j + rround[z].y;
					while(xx<0)
					{
						xx += WIDTH/2;
					}
					while(xx>=WIDTH/2)
					{
						xx -= WIDTH/2;
					}

					if(yy>=0 && yy<HEIGHT/2)
					{
						if(m_pUnderMap[xx][yy].type == rtLAVALAKE)
						{
							m_pUnderMap[i][j].type = rtCAVE;
						}
					}
				}
			}
			if(m_pUnderMap[i][j].type != rtSOLIDROCK && m_pUnderMap[i][j].type != rtLAVALAKE)
			{
				int count;
				count = 0;
				for(int z=0; z<6; z++)
				{
					int xx,yy;
					xx = i + rround[z].x;
					yy = j + rround[z].y;
					while(xx<0)
					{
						xx += WIDTH/2;
					}
					while(xx>=WIDTH/2)
					{
						xx -= WIDTH/2;
					}

					if(yy>=0 && yy<HEIGHT/2)
					{
						if(m_pUnderMap[xx][yy].type != rtSOLIDROCK)
						{
							count++;
						}
					}
				}
				if(count <= 2)
				{
					m_pUnderMap[i][j].type = rtTUNNEL;
					m_pUnderMap[i][j].count = 1;
				}
			}
		}
	}

//	for(int id=0; id<WIDTH/2; id++)
	int totalLength;
	totalLength = 0;
	do
	{
		i = getrandom(WIDTH/2);
		do
		{
			j = getrandom(HEIGHT/2);
		}
		while(i%2 != j%2);

		if(m_pUnderMap[i][j].type == rtTUNNEL && m_pUnderMap[i][j].count == 1)
		{
			int length;
			length = WIDTH/4 + getrandom(WIDTH/4);
			float angle, dangle;
			angle = (float)getrandom(360)*2.0*M_PI/360.0;
			dangle = 0;

			int nameID = maxID++;

			int x,y;
			x = i;
			y = j;
			for(int len=0; len<length; len++)
			{
				if(getrandom(4)==1)
				{
					if(getrandom(2)==1)
						dangle = (M_PI/10.0);
					else
						dangle = -(M_PI/10.0);
				}
				angle = angle + dangle;
				float dx, dy;
				dx = cos(angle);
				dy = sin(angle);

				if(dx>0)
					dx = dx+0.5;
				else
					dx = dx-0.5;

				if(dy>0)
					dy = 1;
				else
					dy = -1;

				if(dx > -1 && dx < 1)
				{
					dy = dy*2;
					dx = 0;
				}

				x = x+dx;
				y = y+dy;

				while(x<0)
				{
					x += WIDTH/2;
				}
				while(x>=WIDTH/2)
				{
					x -= WIDTH/2;
				}

				if(y>=0 && y<HEIGHT/2)
				{
					if(m_pUnderMap[x][y].type == rtSOLIDROCK)
					{
						int count;
						count = 0;
						for(int z=0; z<6; z++)
						{
							int xx,yy;
							xx = x + rround[z].x;
							yy = y + rround[z].y;
							while(xx<0)
							{
								xx += WIDTH/2;
							}
							while(xx>=WIDTH/2)
							{
								xx -= WIDTH/2;
							}

							if(yy>=0 && yy<HEIGHT/2)
							{
								if(m_pUnderMap[xx][yy].type == rtTUNNEL)
								{
									count++;
								}
							}
						}
						if(count<=2)
						{
							m_pUnderMap[x][y].type = rtTUNNEL;
							m_pUnderMap[x][y].nameID = nameID;
							totalLength++;
						}
					}
				}
			}
		}
	}
	while(totalLength < WIDTH);

//    return;

	for(i=0; i<WIDTH/2; i++)
	{
		for(j=0; j<HEIGHT/2; j++)
		{
			if(m_pUnderMap[i][j].type == rtSOLIDROCK)
			{
				int found;
				found = -1;
				bool canBe;
				canBe = true;
				int count;
				count = 0;
				bool tunnel;
				tunnel = false;

				int nameID;

				for(int z=0; z<6; z++)
				{
					int xx,yy;
					xx = i + rround[z].x;
					yy = j + rround[z].y;
					while(xx<0)
					{
						xx += WIDTH/2;
					}
					while(xx>=WIDTH/2)
					{
						xx -= WIDTH/2;
					}

					if(yy>=0 && yy<HEIGHT/2)
					{
						if(m_pUnderMap[xx][yy].type != rtSOLIDROCK)
						{
							count++;
							if(found == -1)
							{
								found = z;
							}
							else
							{
								if(m_pUnderMap[xx][yy].type == rtTUNNEL && abs(rround[z].y-rround[found].y) <= 2 && abs(rround[z].x-rround[found].x) <= 1)
								{
									canBe = false;
								}
							}
							if(m_pUnderMap[xx][yy].type == rtTUNNEL)
							{
								tunnel = true;
								nameID = m_pUnderMap[xx][yy].nameID;
							}
						}
					}
				}
				if(canBe && tunnel && (count>=2 && count<=4))
				{
					m_pUnderMap[i][j].type = rtTUNNEL;
					m_pUnderMap[i][j].nameID = nameID;
				}
			}
		}
	}

	for(i=0; i<WIDTH/2; i++)
	{
		for(j=0; j<HEIGHT/2; j++)
		{
			if(m_pUnderMap[i][j].type == rtTUNNEL)
			{
				int count;
				count = 0;
				for(int z=0; z<6; z++)
				{
					int xx,yy;
					xx = i + rround[z].x;
					yy = j + rround[z].y;
					while(xx<0)
					{
						xx += WIDTH/2;
					}
					while(xx>=WIDTH/2)
					{
						xx -= WIDTH/2;
					}

					if(yy>=0 && yy<HEIGHT/2)
					{
						if(m_pUnderMap[xx][yy].type == rtTUNNEL)
						{
							count++;
						}
					}
				}
				if(count == 0)
				{
					m_pUnderMap[i][j].type = rtSOLIDROCK;
					m_pUnderMap[i][j].count = -1;
				}

				if(m_pUnderMap[i][j].count == 1)
				{
					if(m_pMap[i*2][j*2].h < water || !m_pMap[i*2][j*2].island)
					{
						m_pUnderMap[i][j].count = -1;
					}
					count = 0;
					for(int z=0; z<6; z++)
					{
						int xx,yy;
						xx = i + rround[z].x;
						yy = j + rround[z].y;
						while(xx<0)
						{
							xx += WIDTH/2;
						}
						while(xx>=WIDTH/2)
						{
							xx -= WIDTH/2;
						}

						if(yy>=0 && yy<HEIGHT/2)
						{
							if(m_pUnderMap[xx][yy].count == 1)
							{
								count++;
							}
						}
					}
					if(count > 0)
					{
						m_pUnderMap[i][j].count = -1;
					}
				}
			}
		}
	}
}
