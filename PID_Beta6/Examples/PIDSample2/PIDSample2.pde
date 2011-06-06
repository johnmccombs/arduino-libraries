/***************************************************
 * Advanced PID Example
 * PID input: Analog input 0
 * output = time proportion on/off of output 0 (50% = 2 sec on, 2 sec off)
 * and just to make it more complicated:
 * - start with a setpoint ramp (0 to 500 over 5 minutes (300 seconds))
 * - use gap control (moderate tunings when within 100 of setpoint, aggressive when outside 100)
 ***************************************************/
 
 #include <PID_Beta6.h>
 
 unsigned long windowStartTime;
 unsigned long RampStartTime;
 double Input, Output, Setpoint;
 PID pid(&Input, &Output, &Setpoint, 3,4,1);
 
unsigned long printTime= millis();
 
 void setup()
 {
  
   pid.SetOutputLimits(0,4000); //tell the PID to range the output from 0 to 4000 
   Output = 4000; //start the output at its max and let the PID adjust it from there
  
   pid.SetMode(AUTO); //turn on the PID
   windowStartTime = millis();
   RampStartTime = millis();
 }
 
 
 void loop()
 {

  //Set Point Ramp
   if(millis()-RampStartTime<300000)
   {
     Setpoint = ((double)(millis()-RampStartTime))/(300000.0)*500.0;
   }
   else Setpoint = 500;
   
   //Set Tuning Parameters based on how close we are to setpoint
   if(abs(Setpoint-Input)>100)pid.SetTunings(6,4,1);  //aggressive
   else pid.SetTunings(3,4,1); //comparatively moderate
   
   //give the PID the opportunity to compute if needed
   Input = analogRead(0);
   pid.Compute();
   
   //turn the output pin on/off based on pid output
   if(millis()-windowStartTime>4000) windowStartTime+=4000;
   if(Output > millis()-windowStartTime) digitalWrite(0,HIGH);
   else digitalWrite(0,LOW);

 }

