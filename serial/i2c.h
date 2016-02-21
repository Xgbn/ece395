#ifndef __LPC11xx_I2C_H__
#define __LPC11xx_I2C_H__
#include "LPC11xx.h"	// lpc11xx definitions
#include <stdbool.h>

// macro for controlling I2C
#define STASET LPC_I2C->CONSET |= 0x20
#define STACLR LPC_I2C->CONCLR = 0x20
#define STOSET LPC_I2C->CONSET |= 0x10
#define STOCLR LPC_I2C->CONCLR = 0x10
#define AASET LPC_I2C->CONSET |= 0x04
#define AACLR LPC_I2C->CONCLR = 0x04	
#define SICLR LPC_I2C->CONCLR = 0x8		// clear interrupt flag
#define SISET LPC_I2C->CONSET |= 0x8

#define I2C_DATA_REG LPC_I2C->DAT
#define I2C_STAT_REG LPC_I2C->STAT
#define I2C_SI (LPC_I2C->CONSET && 0x8)
#define I2C_CONSET LPC_I2C->CONSET

#define I2C_EVENT_START 0x8						// start condition has been sent
#define I2C_EVENT_REP_START 0x10			// repeat start condition has started
#define I2C_EVENT_SLAW_ACK 0x18				// SLA + write bit has been sent, ack recieved
#define I2C_EVENT_SLAW_NACK 0x20			// SLA + write bit has been sent, not ack recieved
#define I2C_EVENT_SLAR_NACK 0x48			// SLA + read bit has been sent, not ack recieved
#define I2C_EVENT_SLAR_ACK 0x40				// SLA + read bit has been sent, ack recieved
#define I2C_EVENT_DATR_ACK 0x50				// data to be written has been sent, ack recieved
#define I2C_EVENT_DATR_NACK 0x58			// data to be written has been sent, not ack recieved 
#define I2C_EVENT_DATW_ACK 0x28				// data to be written has been sent, ack recieved
#define I2C_EVENT_DATW_NACK 0x30			// data to be written has been sent, not ack recieved 
#define I2C_READ 0x1
#define I2C_WRITE 0x0

/*
Initialize i2c
*/
extern void i2c_Init();



/*
send a repeated start to device
Input:
	none
Output:
	none
*/
extern void i2c_rep_start();


/*
basic polling function for begin transmission
Input:
	addr: hex address for I2C device
	rw	: 1 for read, 0 for write
Output:
	return 0 on success, -1 on fail
*/
extern void i2c_begin(char addr, int rw);


/*
basic polling function for write
Input:
	msg		:	char to be written to device
Return:
	none
*/
extern void i2c_write(char msg);




/*
basic polling function for read
Input:
	stop	:	true for NACK response
					false for ACK response
Output:
	return the char read from device
*/
extern char i2c_read(bool stop);



/*
basic end transmission
Input:
	none
Output:
	none
*/
extern void i2c_end();


























#endif  /* __LPC11xx_H__ */

