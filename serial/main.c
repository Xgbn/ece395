#include "LPC11xx.h"
#include <stdio.h>
#include <rt_misc.h>
#include "6050.h"

/* Import external functions from Serial.c file                               */
extern void SER_init (void);


char * mpu_register[7] = {
	"Ac_X: ",
	"Ac_Y: ",
	"Ac_Z: ",
	"Temp: ",
	"Gy_X: ",
	"Gy_Y: ",
	"Gy_Z: "
};

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
	int j = 0, k;
	int www;
	unsigned int i = 0;
  unsigned int status = 0;
	uint8_t buff[14] = {0};
	uint8_t low, high;
	int16_t output;
	SER_init();
	configureGPIO();
	printf("Start init for i2c\n\r");
	i2c_Init();
	printf("\n\r\n\r");
	
	
	mpu_6050_Init();
	
		while(1){
		scanf("%d", &www);
		printf("Get input %d;", www);
		// set trigger signal
		if(i%2==0)
			ledOn();
		else
			ledOff();
		i++;
		printf("loop_start %d;", i);
		
		mpu_6050_read(0x3B, 14, buff);
		for(k=0; k < 7; k++){
			high = buff[k*2];
			low = buff[k*2+1];
			output = 0xFFFF & (high<<8 | low);
			printf("%s%d;",mpu_register[k] ,output);
		}
		printf("loop_end %d;", i);
		printf("\n");
		}
		
		
		while(1){                              //infinite loop
		if(i%2==0)
			ledOn();
		else
			ledOff();
		//I2C TRANSMIT
		LPC_I2C->CONSET |= (1<<5);             //set start bit to initiate transmission (sec 15.7.1)
		do{                                    //wait for start condition to be sent
				status = LPC_I2C->STAT & 0xF8;          //store current state (sec 15.7.2)
		}while(status != 0x08);                //compare current state with possible states (sec 15.10.1 table 235)
		printf("start sent...\n\r");
		//TRANSMIT DEVICE ADDRESS
		LPC_I2C->DAT        = 0xD0;            //transmit device address (sec 15.7.3)
		LPC_I2C->CONCLR     = 0x28;            //clear STA and SI bit (sec 15.7.6)
		//TRANSMIT CONTROL BYTE
		do{                                    //wait for start condition to be sent
				status = LPC_I2C->STAT & 0xF8;          //store current state (sec 15.7.2)
		}while(status != 0x18 && status != 0x20);          //wait for address byte to be sent (sec 15.10.1 table 235)
		printf("address sent...\n\r");
		LPC_I2C->DAT        = 0xAA;            //send data (sec 15.7.3)
		LPC_I2C->CONCLR     = (1<<3);          //clear SIC (sec 15.7.6)
		//TRANSMIT DATA BYTE
		do{                                    //wait for start condition to be sent
				status = LPC_I2C->STAT & 0xF8;          //store current state (sec 15.7.2)
		}while(status != 0x28 && status != 0x30);          //wait for address byte to be sent (sec 15.10.1 table 235)
		printf("register addr sent...\n\r");
		LPC_I2C->DAT        = 0xAA;             //send data (sec 15.7.3)
		LPC_I2C->CONCLR     = (1<<3);          //clear SIC (sec 15.7.6)
		//INITIATE STOP CONDITION
		while((LPC_I2C->CONSET & 0x8) != 0x8); //set STOP bit (sec 15.7.1)
		printf("data sent...\n\r");
		LPC_I2C->CONSET     = 0x10;            //set stop bit (sec 15.7.1)
		LPC_I2C->CONCLR     = (1<<3);          //clear SIC (sec 15.7.6)
		i++;
		printf("%d\n\r", i);
     }
}
