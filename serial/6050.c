#include <stdbool.h>
#include "i2c.h"
#include "6050.h"



/*
initializes mpu 6050
Input:
	none
Output:
	none
*/
void mpu_6050_Init(){
	char msg = 0x00;
	mpu_6050_write(0x6B, 1, &msg);
	i2c_end();
}






/*
write bytes to register in mpu 6050
Input:
	addr			: register address of mpu 6050 to write to
	bytenum		: number of bytes to write to
	buff			: buffer that contains bytes to write to mpu
Output:
	none
*/
void mpu_6050_write(char addr, int bytenum, char* buff){
	int i;
	
	i2c_begin(MPU_ADDR, I2C_WRITE);	// start a write to mpu
	// loop to write bytenum bytes
	for(i=0; i < bytenum; i++){
	i2c_write(buff[i]);
	}
	// end transmission to free I2C bus
	i2c_end();
}








