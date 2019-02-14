/*
 * DRV8860_Lib.h - Library for controlling serial interface for
 *  TI 8860 Stepper Motor Driver
 *  Created by Ryan Merrill, February 6, 2019
 */
#ifndef DRV8860Lib_h
#define DRV8860Lib_h

#if (ARDUINO >= 100)
  #include "Arduino.h"
  #include <Stdint.h>
  #include <TimerOne.h>//for timer ISR
  #include <digitalWriteFast.h>//for writing to pins faster
#else
    #include "WProgram.h"
    #include <Stdint.h>
    #include <Timer1.h>//for timer ISR
	#include <digitalWriteFast.h>//for writing to pins faster
#endif


class DRV8860 {
  public:
    //properties
	DRV8860(void);
   
	int stepinput;//variable to set step input to 
    int number_of_steps;//counter to check number of steps
    static void myISR(void);
	 static void _myISR(void);//idk how this works 
	 static DRV8860 *_instance;//but it does
   //methods
   void init(void); //initialize pins
   void theThing(void);//ISR instructions
  
  //full rotation = 64 steps
   void on(int steps,int direction);//allow for loop to to Send_data8 n number of steps?
   

   void help(void); //print out instructions
 	void timer_initialize(void);//initialization of timer for ISR
	void Send_DATA8(uint8_t Data);//function for shifting through and writing data to pin

};


#endif
