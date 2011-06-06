/*
  Figure drawing library

  Copyright 2009/2010 Benjamin Sonntag <benjamin@sonntag.fr> http://benjamin.sonntag.fr/
  
  History:
  	2010-01-01 - V0.0 Initial code at Berlin after 26C3

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA.
*/

#include "Charliplexing.h"
#include "Figure.h"

#include "WProgram.h"
#include <inttypes.h>


uint8_t figuresData[][14][2] = { 
{ {0,0}, {1,0}, {2,0}, {0,1}, {2,1}, {0,2}, {2,2}, {0,3}, {2,3}, {0,4}, {1,4}, {2,4}, {9,9}, {9,9} },
{ {1,0}, {0,1}, {1,1}, {1,2}, {1,3}, {0,4}, {1,4}, {2,4}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {2,1}, {1,2}, {0,3}, {0,4}, {1,4}, {2,4}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {2,1}, {0,2}, {1,2}, {2,3}, {0,4}, {1,4}, {2,4}, {9,9}, {9,9}, {9,9}, {9,9} },
{ {0,0}, {2,0}, {0,1}, {2,1}, {0,2}, {1,2}, {2,2}, {2,3}, {2,4}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {0,1}, {0,2}, {1,2}, {2,2}, {2,3}, {0,4}, {1,4}, {2,4}, {9,9}, {9,9}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {0,1}, {0,2}, {1,2}, {2,2}, {0,3}, {2,3}, {0,4}, {1,4}, {2,4}, {9,9}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {2,1}, {2,2}, {1,3}, {1,4}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {0,1}, {2,1}, {0,2}, {1,2}, {2,2}, {0,3}, {2,3}, {0,4}, {1,4}, {2,4}, {9,9} },
{ {0,0}, {1,0}, {2,0}, {0,1}, {2,1}, {0,2}, {1,2}, {2,2}, {2,3}, {0,4}, {1,4}, {9,9}, {9,9}, {9,9} }
};


/* -----------------------------------------------------------------  */
/** Draws a figure (0-9). You should call it with set=1, 
 * wait a little them call it again with set=0
 * @param figure is the figure [0-9]
 * @param x,y coordinates, 
 * @param set is 1 or 0 to draw or clear it
 */
void Figure::Draw(int figure,int x,int y,int set) {
  for(int i=0;i<14;i++) {
      if (figuresData[figure][i][0]==9) break;
      if (
        figuresData[figure][i][0]+x<13 && 
        figuresData[figure][i][0]+x>=0 && 
        figuresData[figure][i][1]+y<8 && 
        figuresData[figure][i][1]+y>=0
       ) {
           LedSign::Set(figuresData[figure][i][0]+x,figuresData[figure][i][1]+y,set);
       } 
  }
}


/* -----------------------------------------------------------------  */
/** Draw a figure in the other direction (rotated 90°)
 * You should call it with set=1, 
 * wait a little them call it again with set=0
 * @param figure is the figure [0-9]
 * @param x,y coordinates, 
 * @param set is 1 or 0 to draw or clear it
*/
void Figure::Draw90(int figure,int x,int y,int set) {
  for(int i=0;i<14;i++) {
      if (figuresData[figure][i][0]==9) break;
      if (
        (5-figuresData[figure][i][1])+x<13 && 
        (5-figuresData[figure][i][1])+x>=0 && 
        figuresData[figure][i][0]+y<8 && 
        figuresData[figure][i][0]+y>=0
       ) {
           LedSign::Set((5-figuresData[figure][i][1])+x,figuresData[figure][i][0]+y,set);
       } 
  }
}


/* -----------------------------------------------------------------  */
/** Scroll a number from right to left
 * remove unused figures (0 at the left)
 * valid for up to 7 figures.
 * @param value is the value to draw and scroll
 * @param x is the coordinate where we put the top of the figure [0-13]
*/
void Figure::Scroll90(unsigned long value,uint8_t x) {
  int i,j,k;
  uint8_t figures[]={ 
    (value%10000000)/1000000, 
    (value%1000000)/100000, 
    (value%100000)/10000, 
    (value%10000)/1000,
    (value%1000)/100,
    (value%100)/10,
    (value%10)
  };
  j=0;
  while (figures[j]==0 && j<6) j++;
  
  for(i=0;i<9+(7-j)*5;i++) {
    for(k=j;k<=6;k++)
      Figure::Draw90(figures[k],3,-i+9+4*(k-j) ,1);
    delay(100);
    for(k=j;k<=6;k++)
      Figure::Draw90(figures[k],3,-i+9+4*(k-j) ,0);
  }
}


