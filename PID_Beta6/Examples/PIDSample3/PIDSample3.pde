/***************************************************
 * Feed Forward Example : RepRap Extruder Nozzle Temperature
 * PID input: Nozzle Temperature
 * PID output: PWM signal (0-255) to the heater control
 * Other Input: Plastic Feed rate is being read on input 1
 * For the feed forward piece...
 *  The amount of heat we want to add to the nozzle is largely dependent
 *  on how fast plastic is being fed.  if the feed is stopped we're going
 *  to want a lot less heat than when the system is running.  A pid 
 *  control by itself will eventually adjust the heat, but by then the temp
 *  will be really high.  feed forward can help with this.
 *    so to make Feed Forward work, at every cycle we look at the current feed speed
 *  and compute a "bias" which we feed to the pid controller.  sort of: "Based on my
 *  knowledge, your output should be about X.  adjust off of that as you see fit"
 *    What does it get you?  well, the instant the feed stops, the baseline drops. That
 *  means that the controller output immediately drops. if you had been using just a pid, 
 *  you'd have to wait for the temperature to rise quite a bit before seeing the same
 *  drop in output.
 * 
 * (for this example, we're assuming that for a feed rate of 1023 the output should be
 *  about 130, and at a rate of 0 the output should be around 10)
 ***************************************************/
 
 #include <PID_Beta6.h>
 int FeedRate;
 
 double Input, Output, Setpoint, Bias;
 PID pid(&Input, &Output, &Setpoint, &Bias, 3, 4, 1);
 
 void setup()
 {
   Setpoint = 400;
   pid.SetMode(AUTO); //turn on the PID

 }
 
 
 void loop()
 {

   //Read in the Feed rate and compute a baseline output to the heater
   FeedRate = analogRead(1);  //read the feed rate
   Bias = (FeedRate/1023)*(120)+10;
   
   //give the PID the opportunity to compute if needed
   Input = analogRead(0);
   pid.Compute();
   
   analogWrite(0, Output);

 }
