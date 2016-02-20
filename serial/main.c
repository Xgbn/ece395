#include "LPC11xx.h"
#include <stdio.h>
#include <rt_misc.h>
#include "6050.h"

/* Import external functions from Serial.c file                               */
extern void SER_init (void);


void configureGPIO()
{
	//Enable CLKOUT
	/*
	LPC_IOCON->PIO0_1 &= ~0x3F; // Select clkout function for P0.1
	LPC_IOCON->PIO0_1 |= 0x01;
	LPC_SYSCON->CLKOUTCLKSEL = 0x00; // IRC: 0x00 System osci: 0x01 WTD: 0x02 Main clk: 0x03
	LPC_SYSCON->CLKOUTUEN = 0x01; // Update clock
	LPC_SYSCON->CLKOUTUEN = 0x00; // Toggle update register once
	LPC_SYSCON->CLKOUTUEN = 0x01;
	while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) ); // Wait until updated
	LPC_SYSCON->CLKOUTDIV = 1; // Divided by 255
	*/

	//set port 0_7 to output (high current drain in LPC1114)
    LPC_GPIO0->DIR |= (1<<7);
}

void ledOn()
{
	LPC_GPIO0->DATA &= ~(1<<7);
}

void ledOff()
{						 
	LPC_GPIO0->DATA |= (1<<7);
}

int main()
{
	int i, j = 0;
	SER_init();
	configureGPIO();
		i2c_Init();
	LPC_I2C->CONSET = 0x40;
	
	
//		ledOn();
//		printf("Led On, Iteration %d\n\r", j);
//		for (i = 0; i < 0x0027FFFF; i++)
//		{
//		}
//		ledOff();
//		printf("Led Off, Iteration %d\n\r", j);
//		for (i = 0; i < 0x0007FFFF; i++)
//		{
//		}
//		j++;
		//mpu_6050_Init();
		//printf("lol, the control register is: %x\n\r", (LPC_I2C->CONSET));

	while (1){
		ledOn();
		STASET;
		SICLR;
		printf("lol, the control register is: %x\n\r", (LPC_I2C->CONSET));
		ledOff();
	}
}
