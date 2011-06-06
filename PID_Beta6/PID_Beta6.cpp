
#include <wiring.h>
#include <PID_Beta6.h>

/* Standard Constructor (...)***********************************************
 *    constructor used by most users.  the parameters specified are those for
 * for which we can't set up reliable defaults, so we need to have the user
 * set them.
 ***************************************************************************/ 
PID::PID(double *Input, double *Output, double *Setpoint, double Kc, double TauI, double TauD)
{

  PID::ConstructorCommon(Input, Output, Setpoint, Kc, TauI, TauD);  
  UsingFeedForward = false;
  PID::Reset();

  
}

/* Overloaded Constructor(...)**********************************************
 *    This one is for more advanced users.  it's essentially the same as the
 * standard constructor, with one addition.  you can link to a Feed Forward bias,
 * which lets you implement... um.. Feed Forward Control.  good stuff.
 ***************************************************************************/
PID::PID(double *Input, double *Output, double *Setpoint, double *FFBias, double Kc, double TauI, double TauD)
{

  PID::ConstructorCommon(Input, Output, Setpoint, Kc, TauI, TauD);  
  UsingFeedForward = true;			  //tell the controller that we'll be using an external
  myBias = FFBias;                              //bias, and where to find it
  PID::Reset();

}

/* ConstructorCommon(...)****************************************************
 *    Most of what is done in the two constructors is the same.  that code
 * was put here for ease of maintenance and (minor) reduction of library size
 ****************************************************************************/
void PID::ConstructorCommon(double *Input, double *Output, double *Setpoint, double Kc, double TauI, double TauD)
{
  PID::SetInputLimits(0, 1023);		//default the limits to the 
  PID::SetOutputLimits(0, 255);		//full ranges of the I/O

  tSample = 1000;			//default Controller Sample Time is 1 second

  PID::SetTunings( Kc, TauI, TauD);

  nextCompTime = millis();
  inAuto = false;
  myOutput = Output;
  myInput = Input;
  mySetpoint = Setpoint;
}


/* SetInputLimits(...)*****************************************************
 *	I don't see this function being called all that much (other than from the
 *  constructor.)  it needs to be here so we can tell the controller what it's
 *  input limits are, and in most cases the 0-1023 default should be fine.  if
 *  there's an application where the signal being fed to the controller is
 *  outside that range, well, then this function's here for you.
 **************************************************************************/
void PID::SetInputLimits(double INMin, double INMax)
{
	//after verifying that mins are smaller than maxes, set the values
	if(INMin >= INMax) return;

	//rescale the working variables to reflect the changes
	lastInput = (lastInput) * (INMax - INMin) / (inSpan);
	accError *= (INMax - INMin) / (inSpan);

	//make sure the working variables are 
	//within the new limits
	if (lastInput > 1) lastInput = 1;
	else if (lastInput < 0) lastInput = 0;
	
	
        inMin = INMin;
	inSpan = INMax - INMin;
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void PID::SetOutputLimits(double OUTMin, double OUTMax)
{
	//after verifying that mins are smaller than maxes, set the values
	if(OUTMin >= OUTMax) return;

	//rescale the working variables to reflect the changes
	lastOutput = (lastOutput) * (OUTMax - OUTMin) / (outSpan);

	//make sure the working variables are 
	//within the new limits
	if (lastOutput > 1) lastOutput = 1;
	else if (lastOutput < 0) lastOutput = 0;

	outMin = OUTMin;
	outSpan = OUTMax - OUTMin;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted. 
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/
void PID::SetTunings(double Kc, double TauI, double TauD)
{
	//verify that the tunings make sense
	if (Kc == 0.0 || TauI < 0.0 || TauD < 0.0) return;

	//we're going to do some funky things to the input numbers so all
	//our math works out, but we want to store the numbers intact
	//so we can return them to the user when asked.
	P_Param = Kc;
	I_Param = TauI;
	D_Param = TauD;

	//convert Reset Time into Reset Rate, and compensate for Calculation frequency
	double tSampleInSec = ((double)tSample / 1000.0);
	double tempTauR;
	if (TauI == 0.0) 
		tempTauR = 0.0;
	else 
		tempTauR = (1.0 / TauI) * tSampleInSec;

	if (inAuto)
	{	//if we're in auto, and we just change the tunings, the integral term 
		//will become very, very, confused (trust me.) to achieve "bumpless
		// transfer" we need to rescale the accumulated error.
		if(tempTauR != 0.0) //(avoid divide by 0)
			accError *= (kc * taur)/(Kc * tempTauR);
		else
			accError = 0.0;
	}
	
	kc = Kc;
	taur = tempTauR;
	taud = TauD / tSampleInSec;
}

/* Reset()*********************************************************************
 *	does all the things that need to happen to ensure a bumpless transfer
 *  from manual to automatic mode.  this shouldn't have to be called from the
 *  outside. In practice though, it is sometimes helpful to start from scratch,
 *  so it was made publicly available
 ******************************************************************************/
void PID::Reset()
{

	if(UsingFeedForward)
	  bias = (*myBias - outMin) / outSpan;
	else
	  bias = (*myOutput - outMin) / outSpan;
	
        lastOutput = bias;
	lastInput = (*myInput - inMin) / inSpan;

	// - clear any error in the integral
	accError = 0;

}

/* SetMode(...)****************************************************************
 * Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
 * when the transition from manual to auto occurs, the controller is
 * automatically initialized
 ******************************************************************************/
void PID::SetMode(int Mode)
{
	if (Mode!=0 && !inAuto)
	{	//we were in manual, and we just got set to auto.
		//reset the controller internals
		PID::Reset();
	}
	inAuto = (Mode!=0);
}

/* SetSampleTime(...)*******************************************************
 * sets the frequency, in Milliseconds, with which the PID calculation is performed	
 ******************************************************************************/
void PID::SetSampleTime(int NewSampleTime)
{
	if (NewSampleTime > 0)
	{ 
		//convert the time-based tunings to reflect this change
		taur *= ((double)NewSampleTime)/((double) tSample);
		accError *= ((double) tSample)/((double)NewSampleTime);
		taud *= ((double)NewSampleTime)/((double) tSample);
		tSample = (unsigned long)NewSampleTime;
	}
}

/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   pid Output needs to be computed
 *
 *  Some notes for people familiar with the nuts and bolts of PID control:
 *  - I used the Ideal form of the PID equation.  mainly because I like IMC
 *    tunings.  lock in the I and D, and then just vary P to get more 
 *    aggressive or conservative
 *
 *  - While this controller presented to the outside world as being a Reset Time
 *    controller, when the user enters their tunings the I term is converted to
 *    Reset Rate.  I did this merely to avoid the div0 error when the user wants
 *    to turn Integral action off.
 *    
 *  - Derivative on Measurement is being used instead of Derivative on Error.  The
 *    performance is identical, with one notable exception.  DonE causes a kick in
 *    the controller output whenever there's a setpoint change. DonM does not.
 *
 *  If none of the above made sense to you, and you would like it to, go to:
 *  http://www.controlguru.com .  Dr. Cooper was my controls professor, and is
 *  gifted at concisely and clearly explaining PID control
 *********************************************************************************/
void PID::Compute()
{
	justCalced=false;
	if (!inAuto) return; //if we're in manual just leave;

	unsigned long now = millis();

	//millis() wraps around to 0 at some point.  depending on the version of the 
	//Arduino Program you are using, it could be in 9 hours or 50 days.
	//this is not currently addressed by this algorithm.
	
									
	//...Perform PID Computations if it's time...
	if (now>=nextCompTime)							
	{
		
		//pull in the input and setpoint, and scale them into percent span
		double scaledInput = (*myInput - inMin) / inSpan;
		if (scaledInput>1.0) scaledInput = 1.0;
		else if (scaledInput<0.0) scaledInput = 0.0;

		double scaledSP = (*mySetpoint - inMin) / inSpan;
		if (scaledSP>1.0) scaledSP = 1;
		else if (scaledSP<0.0) scaledSP = 0;
		
		//compute the error
		double err = scaledSP - scaledInput;
		
		// check and see if the output is pegged at a limit and only 
		// integrate if it is not. (this is to prevent reset-windup)
		if (!(lastOutput >= 1 && err>0) && !(lastOutput <= 0 && err<0))
		{
			accError = accError + err;
		}								

		// compute the current slope of the input signal
		double dMeas = (scaledInput - lastInput);  // we'll assume that dTime (the denominator) is 1 second. 
							   // if it isn't, the taud term will have been adjusted 
							   // in "SetTunings" to compensate

		//if we're using an external bias (i.e. the user used the 
		//overloaded constructor,) then pull that in now
		if(UsingFeedForward)
		{
			bias = (*myBias - outMin) / outSpan;
		}


		// perform the PID calculation.  
		double output = bias + kc * (err + taur * accError - taud * dMeas);

		//make sure the computed output is within output constraints
		if (output < 0.0) output = 0.0;
		else if (output > 1.0) output = 1.0;

		lastOutput = output;		// remember this output for the windup
						// check next time		
		lastInput = scaledInput;	// remember the Input for the derivative
						// calculation next time

		//scale the output from percent span back out to a real world number
                *myOutput = ((output * outSpan) + outMin);

		nextCompTime += tSample;				// determine the next time the computation
		if(nextCompTime < now) nextCompTime = now + tSample;	// should be performed	

		justCalced=true;  //set the flag that will tell the outside world that the output was just computed
	}								


}


/*****************************************************************************
 * STATUS SECTION
 * These functions allow the outside world to query the status of the PID
 *****************************************************************************/

bool PID::JustCalculated()
{
	return justCalced;
}
int PID::GetMode()
{
	if(inAuto)return 1;
	else return 0;
}

double PID::GetINMin()
{
	return inMin;
}
double PID::GetINMax()
{
	return inMin + inSpan;
}
double PID::GetOUTMin()
{
	return outMin;
}
double PID::GetOUTMax()
{
	return outMin+outSpan;
}
int PID::GetSampleTime()
{
	return tSample;
}
double PID::GetP_Param()
{
	return P_Param;
}
double PID::GetI_Param()
{
	return I_Param;
}

double PID::GetD_Param()
{
	return D_Param;
}





