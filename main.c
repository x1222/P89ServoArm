#include <stdlib.h>
#include <stdio.h>
#include <p89lpc9351.h>
#include "Global.h"
#include "Servo_PWM.h"
#include "Serial.h"
#include "Positioning.h"
#include "Delay.h"
#include "IR.h"

#define COOLING_FANS P2_3

void set_Timer0 (void);
void set_Timer1 (void);
void init_Servo(void);
void init_fans(void);

//Req: angles must >= 0 && <= 175, bicep < 145
void init_Servo(void){

	unsigned char ii = 0; 
 	unsigned char starting_Angles[] = {WRIST_ANGLE + SERVO_WRIST_OFFSET, 180 - ELBOW_ANGLE + SERVO_ELBOW_OFFSET,
 										  BICEP_ANGLE + SERVO_BICEP_OFFSET, SHOULDER_ANGLE + SERVO_SHOULDER_OFFSET};
 	
	P0M1 = 0x00;//set low to 0 and high to 1 for output
	P0M2 = 0xFF;
	P0 = 0;
	
	for(ii =0; ii < NUM_OF_SERVOS; ii++){
		max_Servo_Angle[ii] = starting_Angles[ii];
		servo_Angle[ii] = starting_Angles[ii];
	}
}

void set_Timer0 (void) 
{ 
	TR0 = 0; // Stop timer 0 
   	TL0 = 0;
   	TH0 = 0; 
   	TMOD = (TMOD|0x01);
	ET0 = 1; // Enable timer 0 interrupt 
	EA = 1; // Enable global interrupts 
	TR0 = 1; // Start timer 0 
} 

void set_Timer1 (void) 
{ 
	TR1 = 0; // Stop timer 1 
   	TL1 = 0;
   	TH1 = 0; 
   	TMOD = (TMOD|0x10);
	ET1 = 1; // Enable timer 1 interrupt 
	EA = 1; // Enable global interrupts 
	TR1 = 1; // Start timer 1  
}


void init_Fans(void)
{
	P2M1 = 0x00;
	P2M2 = 0xFF;
	
	COOLING_FANS = 1; //turns low
	FIRE_FAN_PORT = 0;
}

void main(void)
{	
	init_Servo();

	EA = 0;
	set_Pulse_Width();
	set_Timer0();
	//set_Timer1();
	init_Serial_Port();
	init_ADC();
	wait1S();
	init_Fans();
	printf_tiny("Ready for commands!\n");
	
	while(1)
	{		
		
		if (RI == 1)
		{
			get_Command();
		}
		
		set_Pulse_Width();
	}
	
}
