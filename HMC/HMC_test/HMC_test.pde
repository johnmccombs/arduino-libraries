#include <HMC.h>

void setup()
{
  Serial.begin(9600);
  delay(5); // The HMC5843 needs 5ms before it will communicate
  HMC.init();
}

void loop()
{
  int x,y,z;
  delay(100); // There will be new values every 100ms
  HMC.getValues(&x,&y,&z);
  Serial.print("x:");
  Serial.print(x);
  Serial.print(" y:");
  Serial.print(y);
  Serial.print(" z:");
  Serial.println(z);
}
