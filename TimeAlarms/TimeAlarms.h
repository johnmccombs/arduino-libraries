
#ifndef TimeAlarms_h
#define TimeAlarms_h

#include <inttypes.h>

#include "Time.h"

#define dtNBR_ALARMS 6

typedef enum { dtMillisecond, dtSecond, dtMinute, dtHour, dtDay } dtUnits_t;

typedef struct  {
    uint8_t isAllocated            :1 ;  // the alarm is avialable for allocation if false
    uint8_t isEnabled              :1 ;  // the timer is only actioned if isEnabled is true 
    uint8_t isOneShot              :1 ;  // the timer will be de-allocated after trigger is processed 
    uint8_t isAlarm                :1 ;  // time of day alarm if true, period timer if false  
 }
    AlarmMode_t   ;

typedef uint8_t AlarmID_t;
typedef AlarmID_t AlarmId;  // Arduino friendly name
#define dtINVALID_ALARM_ID 255


class AlarmClass;  // forward reference
typedef void (*OnTick_t)();  // alarm callback function typedef 

// class defining an alarm instance, only used by dtAlarmsClass
class AlarmClass
{  
private:
public:
  AlarmClass();
  OnTick_t onTickHandler;  
  void updateNextTrigger();
  time_t value;
  time_t nextTrigger;
  AlarmMode_t Mode;
};

// class containing the collection of alarms
class TimeAlarmsClass
{
private:
   AlarmClass Alarm[dtNBR_ALARMS];
   void serviceAlarms();
   uint8_t isServicing;
   AlarmID_t create( time_t value, OnTick_t onTickHandler, uint8_t isAlarm, uint8_t isOneShot, uint8_t isEnabled=true);
   
public:
  TimeAlarmsClass();
  // functions to create alarms and timers
  AlarmID_t alarmRepeat(time_t value, OnTick_t onTickHandler);                    // trigger daily at given time of day
  AlarmID_t alarmRepeat(const int H,  const int M,  const int S, OnTick_t onTickHandler); // as above, with hms arguments
  AlarmID_t alarmRepeat(const timeDayOfWeek_t DOW, const int H,  const int M,  const int S, OnTick_t onTickHandler); // as above, with day of week 
 
  AlarmID_t alarmOnce(time_t value, OnTick_t onTickHandler);                     // trigger once at given time of day
  AlarmID_t alarmOnce( const int H,  const int M,  const int S, OnTick_t onTickHandler);  // as above, with hms arguments
  AlarmID_t alarmOnce(const timeDayOfWeek_t DOW, const int H,  const int M,  const int S, OnTick_t onTickHandler); // as above, with day of week 
  
  AlarmID_t timerOnce(time_t value, OnTick_t onTickHandler);   // trigger once after the given number of seconds 
  AlarmID_t timerOnce(const int H,  const int M,  const int S, OnTick_t onTickHandler);   // As above with HMS arguments
  
  AlarmID_t timerRepeat(time_t value, OnTick_t onTickHandler); // trigger after the given number of seconds continuously
  AlarmID_t timerRepeat(const int H,  const int M,  const int S, OnTick_t onTickHandler);   // As above with HMS arguments
  
  void delay(unsigned long ms);
   
  // utility methods
  uint8_t getDigitsNow( dtUnits_t Units);         // returns the current digit value for the given time unit
  void waitForDigits( uint8_t Digits, dtUnits_t Units);  
  void waitForRollover(dtUnits_t Units);
 
  // low level methods
  void enable(AlarmID_t ID);
  void disable(AlarmID_t ID);
  void write(AlarmID_t ID, time_t value);    // write the value (and enable) the alarm with the given ID
  time_t read(AlarmID_t ID);                 // return the value for the given timer 
};

extern TimeAlarmsClass Alarm;  // make an instance for the user

/*==============================================================================
 * MACROS
 *============================================================================*/

/* public */
#define waitUntilThisSecond(_val_) waitForDigits( _val_, dtSecond)
#define waitUntilThisMinute(_val_) waitForDigits( _val_, dtMinute)
#define waitUntilThisHour(_val_)   waitForDigits( _val_, dtHour)
#define waitUntilThisDay(_val_)    waitForDigits( _val_, dtDay)
#define waitMinuteRollover() waitForRollover(dtSecond)
#define waitHourRollover()   waitForRollover(dtMinute)
#define waitDayRollover()    waitForRollover(dtHour)

#define AlarmHMS(_hr_, _min_, _sec_) (_hr_ * SECS_PER_HOUR + _min_ * SECS_PER_MIN + _sec_)

#endif /* TimeAlarms_h */

