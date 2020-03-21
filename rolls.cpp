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

#include "gameio.h"
#include "rolls.h"
#include <cmath>

int dice(int c, int d, int a){
   int sum = 0;
   for (int l=0; l<c; l++)
     sum += getrandom(d);

   return sum + a;
}

int rollfunc( int a[5] ){
  return rollfunc(a[0], a[1], a[2], a[3], a[4] );
}

int rollfunc(int x, int f, int k1, int k2, int k3){
  switch ( f )
  {
    case 0 : return 0;
    case 1 : return k1*(x+k2)+k3;
    case -1: return (x+k2)/k1 + k3;

    case 2 : return pow(x+k2,k1)+k3;

    case 3 : return exp(x+k1)+k2;
    case -3: return log(x+k1)+k2;

    case 4 : return dice(x,k1,k2);
  }
  return 0;
}
