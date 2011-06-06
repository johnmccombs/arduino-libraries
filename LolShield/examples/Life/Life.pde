/*
 Conway's "Life"
 
 Writen for the LoL Shield, designed by Jimmie Rodgers:
 http://jimmieprodgers.com/kits/lolshield/
 
 This needs the Charliplexing library, which you can get at the
 LoL Shield project page: http://code.google.com/p/lolshield/
 
 Created by Jimmie Rodgers on 12/30/2009.
 Adapted from: http://www.arduino.cc/playground/Main/DirectDriveLEDMatrix
 
 History:
  	December 30, 2009 - V1.0 first version written at 26C3/Berlin

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Version 3 General Public
  License as published by the Free Software Foundation; 
  or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Charliplexing.h>    //Imports the library, which needs to be
                              //Initialized in setup.

#define DELAY 150             //Sets the time each generation is shown
#define RESEEDRATE 5000       //Sets the rate the world is re-seeded
#define SIZEX 14              //Sets the X axis size
#define SIZEY 9               //Sets the Y axis size
byte world[SIZEX][SIZEY][2];  //Creates a double buffer world
long density = 50;            //Sets density % during seeding
int geck = 0;                 //Counter for re-seeding

void setup() {
  LedSign::Init();            //Initilizes the LoL Shield
  randomSeed(analogRead(5));
  //Builds the world with an initial seed.
  for (int i = 0; i < SIZEX; i++) {
    for (int j = 0; j < SIZEY; j++) {
      if (random(100) < density) {
        world[i][j][0] = 1;
      }
      else {
        world[i][j][0] = 0;
      }
      world[i][j][1] = 0;
    }
  }
}

void loop() {
  // Birth and death cycle 
  for (int x = 0; x < SIZEX; x++) { 
    for (int y = 0; y < SIZEY; y++) {
      // Default is for cell to stay the same
      world[x][y][1] = world[x][y][0];
      int count = neighbours(x, y); 
      geck++;
      if (count == 3 && world[x][y][0] == 0) {
        // A new cell is born
        world[x][y][1] = 1; 
        LedSign::Set(x,y,1);
      } 
      else if ((count < 2 || count > 3) && world[x][y][0] == 1) {
        // Cell dies
        world[x][y][1] = 0;
        LedSign::Set(x,y,0);
      }
    }

  }
  
  //Counts and then checks for re-seeding
  //Otherwise the display will die out at some point
  geck++;
  if (geck > RESEEDRATE){
    seedWorld();
    geck = 0;
  }

  // Copy next generation into place
  for (int x = 0; x < SIZEX; x++) { 
    for (int y = 0; y < SIZEY; y++) {
      world[x][y][0] = world[x][y][1];
    }
  }
  delay(DELAY);
}

//Re-seeds based off of RESEEDRATE
void seedWorld(){
  randomSeed(analogRead(5));
  for (int i = 0; i < SIZEX; i++) {
    for (int j = 0; j < SIZEY; j++) {
      if (random(100) < density) {
        world[i][j][1] = 1;
      }
    } 
  }
}

//Runs the rule checks, including screen wrap
int neighbours(int x, int y) {
  return world[(x + 1) % SIZEX][y][0] + 
    world[x][(y + 1) % SIZEY][0] + 
    world[(x + SIZEX - 1) % SIZEX][y][0] + 
    world[x][(y + SIZEY - 1) % SIZEY][0] + 
    world[(x + 1) % SIZEX][(y + 1) % SIZEY][0] + 
    world[(x + SIZEX - 1) % SIZEX][(y + 1) % SIZEY][0] + 
    world[(x + SIZEX - 1) % SIZEX][(y + SIZEY - 1) % SIZEY][0] + 
    world[(x + 1) % SIZEX][(y + SIZEY - 1) % SIZEY][0]; 
}
