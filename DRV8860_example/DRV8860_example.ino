#include <DRV8860Lib.h>

DRV8860 DRV8860;
void setup() {
  // put your setup code here, to run once:
 // DRV8860 myClass;
  DRV8860.init();
  DRV8860.timer_initialize();
//  myClass.begin();
  Serial.begin(115200);
  DRV8860.help();
  
  DRV8860.on(64,1);//fullrotation counterclockwise
  delayMicroseconds(5);
  DRV8860.on(32,0);//halfrotation  clockwise

}

void loop() {
  // put your main code here, to run repeatedly:
 
}
