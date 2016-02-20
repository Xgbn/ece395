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
	printf("Start init for i2c\n\r");
	i2c_Init();
	printf("\n\r\n\r");
	
	printf("ready\n\r");
	
	while(1){
		ledOn();
		i2c_begin(MPU_ADDR, I2C_WRITE);
		i2c_end();
		printf("checkpoint\n\r");
		ledOff();
	}
	
	while (1){
		ledOn();
		LPC_I2C->CONSET |= 0x40;
		STASET;
		//SICLR;
		while((I2C_STAT_REG & 0xff) != 0x08){}
		printf("%x\n\r", I2C_STAT_REG);
		printf("START sent from master to slave\n\r");
		while(1);
		SICLR;
		ledOff();
	}
}
