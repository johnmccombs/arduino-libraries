/*
 * TimeAlarmExample.pde
 *
 * This example calls alarm functions at 8:30 am and at 5:45 pm (17:45)
 * and simulates turning lights on at night and off in the morning
 *
 * A timer is called every 15 seconds
 * Another timer is called once only after 10 seconds
 *
 * At startup the time is set to Jan 1 2010  8:29 am
 */

#include <Time.h>
#include <TimeAlarms.h>

void setup()
{
  Serial.begin(9600);    
  Serial.println("TimeAlarms Example");
  Serial.println("Alarms are triggered daily at 8:30 am and 17:45 pm");
  Serial.println("One timer is triggered every 15 seconds");
  Serial.println("Another timer is set to trigger only once after 10 seconds");
  Serial.println();
  
  setTime(8,29,40,1,1,10); // set time to 8:29:40am Jan 1 2010 

  Alarm.alarmRepeat(8,30,0, MorningAlarm);  // 8:30am every day
  Alarm.alarmRepeat(17,45,0,EveningAlarm);  // 5:45pm every day 
 
  Alarm.timerRepeat(15, RepeatTask);            // timer for every 15 seconds    
  Alarm.timerOnce(10, OnceOnlyTask);            // called once after 10 seconds 
}

void MorningAlarm()
{
  Serial.println("Alarm: - turn lights off");    
}

void EveningAlarm()
{
  Serial.println("Alarm: - turn lights on");           
}

void RepeatTask()
{
  Serial.println("15 second timer");         
}

void OnceOnlyTask()
{
  Serial.println("This timer only triggers once");  
}

void  loop()
{  
  digitalClockDisplay();
  Alarm.delay(1000); // wait one second between clock display
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

