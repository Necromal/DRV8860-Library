

/*
 * DRV8860_Lib.cpp - Library for controlling serial interface for
 *  TI 8860 Stepper Motor Driver
 *  Created by Ryan Merrill, February 6, 2019
 */
#ifndef DRV8860Lib_cpp
#define DRV8860Lib_cpp

#include "DRV8860Lib.h"
#include "Arduino.h"
#include <Stdint.h>
#include <TimerOne.h> //for timer ISR
#include <digitalWriteFast.h> //for writing to pins faster

//set clk, latch, din, and en pins defined
#define  CLK 13
#define  LATCH 12
#define  DIN 11
#define  EN 10
#define DOUT 9
#define nFAULT 2

//global variables to be used by ISR
uint8_t Stepper_on;
uint8_t Stepper_on_flag;
uint8_t Motor_step; 
uint8_t Motor_dir;
uint8_t Motordata;
uint8_t Motor_micro;

int stepinput;
int number_of_steps;

DRV8860 *DRV8860::_instance;

DRV8860::DRV8860(void){

}


void DRV8860::init(void){
  pinModeFast(CLK, OUTPUT); //Clk
  pinModeFast(LATCH, OUTPUT); //Latch
  pinModeFast(DIN, OUTPUT); //Din (To DRV8860)
  pinModeFast(EN, OUTPUT); // En
  pinModeFast(DOUT, INPUT); // Dout (From DRV8860)
  pinModeFast(nFAULT, INPUT); //nFault
  
  //move to other place to disable output with command
  digitalWriteFast(EN, HIGH);//enable outputs. //add to class
}
void DRV8860::on(int steps, int direction){
   stepinput = steps;
   
 switch (direction){
  case 0:// CW
	  Serial.print("Halfstep clockwise\r\n");
		  Motor_dir = 0;
		  Motor_step=9;
		  Stepper_on = 1;
  
	  
  break;
  case 1: //CCW
	  Serial.print("Halfstep counterclockwise\r\n");
	  Motor_dir = 1;
	  Motor_step= 9;
	 Stepper_on = 1;
	  
  break;
  
 }

}

void DRV8860::theThing(void){
   if(Stepper_on)  // stepper motor command only sent when active
  {
      
      digitalWriteFast(LATCH, HIGH);
      digitalWriteFast(CLK, LOW);
      delay(100);
      digitalWriteFast(LATCH, LOW);
      Send_DATA8(Motordata);
      delay(100);
      digitalWriteFast(LATCH, HIGH);
  }
////////////////////////////////////////////////////////////////////////
      if(Stepper_on)
      {
        //Serial.print("Stepper on flag == 1\r\n");
        Stepper_on_flag = 1;
    
      /*
        if (Motor_micro == 0) // Full step
        {
          switch(Motor_step) // Full sequence; two cycles
          {
            case 0:
              Motordata = 0x99; //1001 1001
              break;
            case 1:
              Motordata = 0xcc; //1100 1100
              break;
            case 2:
              Motordata = 0x66; //0110 0110
               break;
            case 3:
              Motordata = 0x33; //0011 0011
               break;
            case 4:
              Motordata = 0x99; //1001 1001
             break;
            case 5:
              Motordata = 0xcc; //1100 1100
           break;
            case 6:
              Motordata = 0x66; //0110 0110
               break;
            case 7:
              Motordata = 0x33; //0011 0011
               break;
            default:
              break;
    
          }
        }*/
        //else if (Motor_micro == 1) // half step sequence
        //{
          switch(Motor_step)
          {
            case 0:
              Motordata = 0x88; //1000 1000
              break;
            case 1:
              Motordata = 0xcc; //1100 1100
              break;
            case 2:
              Motordata = 0x44; //0100 0100
              break;
            case 3:
              Motordata = 0x66; //0110 0110
              break;
            case 4:
              Motordata = 0x22; //0010 0010
              break;
            case 5:
              Motordata = 0x33; //0011 0011
              break;
            case 6:
              Motordata = 0x11; //0001 0001
              break;
            case 7:
              Motordata = 0x99; //1001 1001
              break;
			case 10:
				Motordata = 0x00;
				goto END; //breaks to end to shutdown writing once number of steps reached.
            default:
              break;
          }
        //}
          if (Motor_dir == 0) // Clockwise direction
        {
         // Serial.print("Motor_step = ");
        //  Serial.print(Motor_step, DEC);
         // Serial.print(" \r\n");
          
          Motor_step++;
          if (Motor_step == 8) Motor_step = 0;
		 if(number_of_steps == stepinput){
			 Motor_step = 10;
		 }
		//  Serial.print("Number of steps ");
		 //  Serial.print(number_of_steps, DEC);
		//   Serial.print("\r\n");
		  number_of_steps++;
        }
        else if (Motor_dir == 1)// Counter-clockwise direction
        {
          if (Motor_step == 0) Motor_step =8;
        //  Serial.print("Motor_step = ");
         // Serial.print(Motor_step, DEC);
         // Serial.print(" \r\n");
        
          Motor_step--;
           if(number_of_steps ==stepinput){
			   Motor_step = 10;
		   }
		   //Serial.print("Number of steps ");
		 //  Serial.print(number_of_steps, DEC);
		  // Serial.print("\r\n");
		  number_of_steps++;
        }
      }
      else
      {
		END:
        if (Stepper_on_flag == 1)
        {
          //Serial.print("Stepper on flag set to 0\r\n");
          Stepper_on_flag = 0;
		  Stepper_on = 0;
          Motor_step = 9;
          Motordata = 0;
		  number_of_steps=0;
          //////////////////////////////////////////////
          digitalWriteFast(LATCH, HIGH);
          digitalWriteFast(CLK, LOW);
          delay(100);
          digitalWriteFast(LATCH, LOW);
          Send_DATA8(Motordata);
          delay(100);
          digitalWriteFast(LATCH, HIGH);
		  Serial.print("Finished writing\r\n");
          //////////////////////////////////////////
        }
      }

}

void DRV8860::help(void){
	Serial.print("Input as '(class).on(Motor StepCount, Motor Direction)' \r\n");
	
	Serial.print("Step count of '64' is halfstepping for 1 full rotation.\r\n");
	Serial.print("Motor direction of '0' is clockwise\r\n");
	Serial.print("Motor direction of '1' is counter-clockwise\r\n");
 
}


void DRV8860::timer_initialize(void){
	//Timer1.initialize(5000); 
	//Timer1.start();
	_instance = this;
	Timer1.attachInterrupt(_myISR, 5000); 
	
}
static void DRV8860::_myISR(void){
	_instance-> theThing();
}

void DRV8860::Send_DATA8(uint8_t Data){
	  uint8_t i;
      
      digitalWriteFast(CLK,LOW);
      for(i=8;i!=0;i--)//loop number of steps and check then break?
      {
       //bring DIN high or low according to data when using other commands
        delay(100);
        digitalWriteFast(CLK,HIGH);
		analogWrite(DIN, Data); //send data through pin
		//Serial.print(Data, HEX);
	   //Serial.print("\r\n");
        Data=(Data<<1);
       
        delay(100);
        if(i>1){
          digitalWriteFast(CLK,LOW);
          
        }
	  }
        
}



#endif