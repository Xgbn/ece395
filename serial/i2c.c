#include "i2c.h"
#include <stdio.h>
#include "LPC11xx.h"


/*
internal rc oscillator freq (also main oscillator frequency)
ahb clk freq (12 MHz)
__IRC_OSC_CLK     (12000000UL)

*/


void i2c_Init(){
	// configure pins 0.4 and 0.5 for I2C

	//INITIALIZE I2C pins, system components, and enable master transmit mode
     LPC_IOCON->PIO0_4         &= ~(0x303); //clear "FUNC" and "I2CMODE" bits (sec 7.4.11)
     LPC_IOCON->PIO0_4         |= 0x1;      //select I2C function SCL (sec 7.4.11)
     LPC_IOCON->PIO0_5         &= ~(0x303); //clear "FUNC" and "I2CMODE" bits (sec 7.4.12)
     LPC_IOCON->PIO0_5         |= 0x1;      //select I2C function SDA (sec 7.4.12)
     LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);   //enable clock to I2C block (sec 3.5.14)
     LPC_SYSCON->PRESETCTRL    |= (0x2);    //disable reset to I2C unit (sec 3.5.2)
     
	
	// set duty cycle for i2c for 400kb/s, SCLH + SCLL = 50
	LPC_I2C->SCLH = 0x54;
	LPC_I2C->SCLL = 0x50;
	
	LPC_I2C->CONSET           |= (1<<6);   //put I2C unit in master transmit mode (sec 15.8.1 and 15.7.1)

//	printf("stat reg after init: %x\n\r", I2C_STAT_REG);
//	printf("control register after init: %x\n\r", (LPC_I2C->CONSET));
}

/*
send a repeated start to device
Input:
	none
Output:
	none
*/
void i2c_rep_start(){
	STASET;
	SICLR;
	while(!I2C_SI){};
	STACLR;
}


/*
basic polling function for begin transmission
Input:
	addr: hex address for I2C device
	rw	: 1 for read, 0 for write
Output:
	return 0 on success, -1 on fail
*/
void i2c_begin(char addr, int rw){
	// get data ready to send
	addr = addr << 1;
	if(rw == I2C_WRITE)
		addr &= 0xFE;		/* change r/w bit to 0 for write */
	else
		addr |= 0x1;	/* change r/w bit to 1 for read */
	I2C_DATA_REG = addr;
	
	// set start signal
	STASET;
	printf("%x\n\r", I2C_STAT_REG & 0xff);
	// wait for start sig sent
	while((I2C_STAT_REG & 0xff) != 0x08){
	}
	SICLR;
	
	printf("%x\n\r", I2C_STAT_REG & 0xff);
	printf("checkpoint 1\n\r");
	//STACLR;

	while((I2C_STAT_REG & 0xff) != I2C_EVENT_SLAW_ACK){
	//printf("%x\n\r", I2C_STAT_REG & 0xff);
	}
	// for debugging
	printf("checkpoint 2\n\r");
}

void i2c_write(char msg){
	I2C_DATA_REG = msg;
	SICLR;
	while(!I2C_SI){};
	// for debugging
	printf("msg sent!\n");
}


/*
basic polling function for read
Input:
	stop	:	true for NACK response
					false for ACK response
Output:
	return the char read from device
*/
char i2c_read(bool stop){
	char read;
	if(stop)
		AACLR;
	else
		AASET;
	SICLR;
	while(!I2C_SI){};
	read = (char)(0xFF && I2C_DATA_REG);
	AACLR;
	return read;
}


/*
basic end transmission
Input:
	none
Output:
	none
*/
extern void i2c_end(){
	STOSET;
	SICLR;
	STOCLR;
}



















