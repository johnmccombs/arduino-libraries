/*
  Example for Charliplexing library
  
  Alex Wenger <a.wenger@gmx.de> http://arduinobuch.wordpress.com/

  History:
  	30/Dez/09 - V0.0 wrote the first version at 26C3/Berlin

*/
#include "Charliplexing.h"

struct point {
  uint8_t xp;        // Point Position in X direction (multplied by 16)
  uint8_t x_speed;   // Speed
  uint8_t flag;  
} points[9];

void setup()                    // run once, when the sketch starts
{
  LedSign::Init();
  
  for(uint8_t i = 0; i < 9; i++)
  {
    points[i].xp = 0;
    points[i].x_speed = random(1, 16);
    points[i].flag = 1;
  }
}

uint8_t heart_p[] = {
  4,5,
  3,4,
  2,4,
  5,4,
  6,4,
  7,5,
  1,5,
  7,6,
  1,6,
  6,7,
  2,7,
  5,8,
  3,8,
  4,9,
};

void heart()
{
  for(uint8_t y = 0; y < 9; y++)
    for(uint8_t x = 3; x < 11; x++)
    {
      LedSign::Set(x,y,0);    
    }
  for(uint8_t i = 0; i < 14; i++)
  {
    LedSign::Set(heart_p[i*2+1],heart_p[i*2],1);    
  }
}

uint8_t heart_flag;

void loop()                     // run over and over again
{
  for(uint8_t i = 0; i < 9; i++)
  {
    points[i].xp += points[i].x_speed;
    if (points[i].xp >= 14*16) 
    {
      points[i].x_speed = random(1, 16);
      points[i].xp = 0;
      points[i].flag ^= 1;
    }
    LedSign::Set(points[i].xp/16,i,points[i].flag);    
  }
  
  heart_flag++;
  if (heart_flag < 20) {
    heart();
  }
  
  delay(40);
}
