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
     
	
	// set duty cycle for i2c for 400kb/s, SCLH + SCLL = 30
	LPC_I2C->SCLH = 0x84;
	LPC_I2C->SCLL = 0x80;
	
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
int i2c_rep_start(char addr, int rw){
	int i = 0;
	uint32_t status;
	uint32_t ack_event;
	// get data ready to send
	addr = addr << 1;
	if(rw == I2C_WRITE){
		addr &= 0xFE;		/* change r/w bit to 0 for write */
		ack_event = I2C_EVENT_SLAW_ACK;
	}
	else{
		addr |= 0x1;	/* change r/w bit to 1 for read */
		ack_event = I2C_EVENT_SLAR_ACK;
	}
			
	LPC_I2C->CONSET |= (1<<5);             //set start bit to initiate transmission (sec 15.7.1)
	
	SICLR;
	
	do{                                    //wait for start condition to be sent
			status = LPC_I2C->STAT & 0xF8;          //store current state (sec 15.7.2)
			//printf("%x\n\r", status);
	}while(status != I2C_EVENT_REP_START);
	//printf("start sent...\n\r");
	
	LPC_I2C->DAT        = addr;            //transmit device address (sec 15.7.3)
	LPC_I2C->CONCLR     = 0x28;            //clear STA and SI bit (sec 15.7.6)
	
	while((I2C_STAT_REG & 0xF8) != ack_event)		//  && status != 0x30
	{
		i++;
		if(i > 100000){
		printf("waited too long begin, abort\n\r");
		i = 0;
			return 1;
		//return;
		}
	}
	
	return 0;
}


/*
basic polling function for begin transmission
Input:
	addr: hex address for I2C device
	rw	: 1 for read, 0 for write
Output:
	return 0 on success, -1 on fail
*/
int i2c_begin(char addr, int rw){
	int i = 0;
	uint32_t status;
	uint32_t ack_event;
	// get data ready to send
	addr = addr << 1;
	if(rw == I2C_WRITE){
		addr &= 0xFE;		/* change r/w bit to 0 for write */
		ack_event = I2C_EVENT_SLAW_ACK;
	}
	else{
		addr |= 0x1;	/* change r/w bit to 1 for read */
		ack_event = I2C_EVENT_SLAR_ACK;
	}
			
	LPC_I2C->CONSET |= (1<<5);             //set start bit to initiate transmission (sec 15.7.1)
	do{                                    //wait for start condition to be sent
			status = LPC_I2C->STAT & 0xF8;          //store current state (sec 15.7.2)
	}while(status != I2C_EVENT_START);
	//printf("start sent...\n\r");
	
	LPC_I2C->DAT        = addr;            //transmit device address (sec 15.7.3)
	LPC_I2C->CONCLR     = 0x28;            //clear STA and SI bit (sec 15.7.6)
	
	while((I2C_STAT_REG & 0xF8) != ack_event )		//  && status != 0x30
	{
		i++;
		if(i > 100000){
		printf("waited too long begin, abort\n\r");
		printf("%x\n\r", I2C_STAT_REG);
			//i2c_end();
			i = 0;
		return 1;
		}
	}
	return 0;
	//printf("perif addr sent...\n\r");
	
}

void i2c_write(char msg){
	int i = 0;
	uint32_t status;
	LPC_I2C->DAT = msg;
	LPC_I2C->CONCLR = (1<<3);
        //store current state (sec 15.7.2)
	while((I2C_STAT_REG & 0xF8) != I2C_EVENT_DATW_ACK)		//  && status != 0x30
	{
		i++;
		if(i > 100000){
		printf("waited too long write, abort\n\r");
		i = 0;
		//return;
		}
	}
	
}


/*
basic polling function for read
Input:
	stop	:	true for NACK response
					false for ACK response
Output:
	return the char read from device
*/
uint8_t i2c_read(bool stop){
	int i=0;
	char read;
	if(stop)
		AACLR;
	else
		AASET;
	SICLR;
	while((I2C_STAT_REG & 0xF8) != I2C_EVENT_DATR_ACK && (I2C_STAT_REG & 0xF8) != I2C_EVENT_DATR_NACK)		//  && status != 0x30
	{
		i++;
		if(i > 100000){
		printf("%x\n\r", (I2C_STAT_REG & 0xF8));
		printf("%d\n\r", stop);
		i = 0;
		//return 0x0;
		}
	}
	//printf("I2C_STAT_REG %x\n\r", (I2C_STAT_REG & 0xF8));
	read = (0xFF & I2C_DATA_REG);
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
	LPC_I2C->CONSET = 0x10;            //set stop bit (sec 15.7.1)
	LPC_I2C->CONCLR = (1<<3);
}



















