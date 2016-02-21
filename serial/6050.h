#ifndef MPU_6050_H
#define	MPU_6050_H
#include <stdbool.h>
#include "i2c.h"
#define MPU_ADDR 0x68





/*
write bytes to register in mpu 6050
Input:
	addr			: register address of mpu 6050 to write to
	bytenum		: number of bytes to write to
	buff			: buffer that contains bytes to write to mpu
Output:
	none
*/
extern void mpu_6050_write(uint8_t addr, int bytenum, uint8_t* buff);


/*
initializes mpu 6050
Input:
	none
Output:
	none
*/
extern void mpu_6050_Init();


/*
reads bytenum bytes from register starting from reg_addr
Input:
	reg_addr	: address of the register to start reading from
	bytenum		: number of bytes to read 
	buff			: buffer to store the read bytes
Output:
	none
*/
extern void mpu_6050_read(uint8_t reg_addr, int bytenum, uint8_t* buff);


#endif

