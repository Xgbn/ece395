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
extern void mpu_6050_write(char addr, int bytenum, char* buff);


/*
initializes mpu 6050
Input:
	none
Output:
	none
*/
extern void mpu_6050_Init();


#endif

