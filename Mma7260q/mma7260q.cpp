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
#ifndef mma7260q_cpp
#define mma7260q_cpp

#include "mma7260q.h"

#include "WProgram.h"

Mma7260q::Mma7260q(int xpin, int ypin, int zpin)
{
  xaxis = xpin;
  yaxis = ypin;
  zaxis = zpin;
 
  pinMode(xaxis,INPUT);
  pinMode(yaxis,INPUT);
  pinMode(zaxis,INPUT);  

  minx = INT_MAX;
  maxx = 0;
  miny = INT_MAX;
  maxy = 0;
  minz = INT_MAX;
  maxz = 0;

  g0x = 0;
  g0y = 0;
  g0z = 0;
}


void Mma7260q::readTilt(float *rho,float *phi,float *theta)
{
    int pfx /*valx*/ = analogRead(xaxis);    // read the value from the sensor
    int pfy /*valy*/ = analogRead(yaxis);    // read the value from the sensor
    int pfz /*valz*/ = analogRead(zaxis);    // read the value from the sensor

    autoZeroCalibration(pfx,pfy,pfz);
  
    int fx = (pfx - g0x);
    int fy = (pfy - g0y);
    int fz = (pfz - g0z);
    
    float ax = (fx*2.0)/((maxx-minx));    
    float ay = (fy*2.0)/((maxy-miny));
    float az = (fz*2.0)/((maxz-minz));
    
    if (rho != NULL) 
    { 
      *rho =   atan(ax/sqrt(pow(ay,2)+pow(az,2)))*(57.2957795129); //57.2957795129 degrees per rad  
    }
    if (phi != NULL) 
    {
      *phi =   atan(ay/sqrt(pow(ax,2)+pow(az,2)))*(57.2957795129);  
    }
    if (theta != NULL)
    {
      *theta = atan(sqrt(pow(ay,2)+pow(ax,2))/az)*(57.2957795129);  
    }
  
}


void Mma7260q::autoZeroCalibration(int pfx, int pfy, int pfz)
{
     if (pfx < minx) minx = pfx;
     if (pfy < miny) miny = pfy;
     if (pfz < minz) minz = pfz;
  
     if (pfx > maxx) maxx = pfx;
     if (pfy > maxy) maxy = pfy;
     if (pfz > maxz) maxz = pfz;
    
     g0x = ((maxx - minx)/2)+minx;
     g0y = ((maxy - miny)/2)+miny;
     g0z = ((maxz - minz)/2)+minz;    
}

void Mma7260q::getMaxValues(int *x, int*y, int *z)
{
  *x = maxx;
  *y = maxy;
  *z = maxz;
}
void Mma7260q::getMinValues(int *x, int*y, int *z)
{
  *x = minx;
  *y = miny;
  *z = minz;
}



#endif
