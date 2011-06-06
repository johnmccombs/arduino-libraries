/*
 * Mma7260q.h
 * 
 * Copyright (c) 2009 Ruben Laguna <ruben.laguna at gmail.com>. All rights reserved.
 * 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef mma7260q_h
#define mma7260q_h

#include <limits.h>


class Mma7260q
{
  public:
    Mma7260q(int xpin, int ypin, int zpin);
    void autoZeroCalibration(int x, int y, int z);
    void readTilt(float *rho,float *phi,float *theta);
    void getMaxValues(int *x,int *y,int *z);
    void getMinValues(int *x,int *y,int *z);
    
  private:

    int xaxis;
    int yaxis;
    int zaxis;
  
    int minx;
    int maxx;
    int miny;
    int maxy;
    int minz;
    int maxz;

    float g0x;
    float g0y;
    float g0z;
  
};


#endif
