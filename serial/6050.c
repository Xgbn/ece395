#include <stdbool.h>
#include <stdio.h>
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
	uint8_t msg = 0x00;
	mpu_6050_write(0x6B, 1, &msg);
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
void mpu_6050_write(uint8_t addr, int bytenum, uint8_t* buff){
	int i;
	
	i2c_begin(MPU_ADDR, I2C_WRITE);	// start a write to mpu
	// loop to write bytenum bytes
	for(i=0; i < bytenum; i++){
	i2c_write(buff[i]);
	}
	// end transmission to free I2C bus
	i2c_end();
}





void mpu_6050_read(uint8_t reg_addr, int bytenum, uint8_t* buff){
	int i,k;
	uint32_t dummy = 0;
	bool stop = false;
	printf("checkpoint1\n\r");
	i2c_begin(MPU_ADDR, I2C_WRITE);
	i2c_write(reg_addr);
	i2c_rep_start(MPU_ADDR, I2C_READ);
	printf("checkpoint2\n\r");
	for(i = 0; i < bytenum; i++){
		for(k = 0; k < 1000; k++){
			if(k % 15 == 0)
				dummy ++;
		}
		if(i == bytenum -1)
			stop = true;
		buff[i] = i2c_read(stop);
	}
	printf("checkpoint3\n\r");
	i2c_end();
}



