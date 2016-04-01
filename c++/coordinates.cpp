#include "coordinates.h"

coordinates::coordinates():acc_sensitivity(ACC_SENSITIVITY_0), rot_sensitivity(ROT_SENSITIVITY_0){
	reset();
}

coordinates::coordinates(int new_buff_size, int acc_sen, int rot_sen){
	reset();
	buff_size = new_buff_size;
	buff.resize(new_buff_size);
	setSensitivity(acc_sen, rot_sen);
}

coordinates::~coordinates(){}

void coordinates::setBuff(int new_buff_size){
	buff_size = new_buff_size;
	buff.resize(new_buff_size);
}


void coordinates::setSensitivity(int acc_sen, int rot_sen){
	switch(acc_sen){
		case 1:
			acc_sensitivity = ACC_SENSITIVITY_1;
			break;
		case 2:	
			acc_sensitivity = ACC_SENSITIVITY_2;
			break;
		case 3:
			acc_sensitivity = ACC_SENSITIVITY_3;
			break;
		default:
			acc_sensitivity = ACC_SENSITIVITY_0;
	}

	switch(rot_sen){
		case 1:
			rot_sensitivity = ROT_SENSITIVITY_1;
			break;
		case 2:	
			rot_sensitivity = ROT_SENSITIVITY_2;
			break;
		case 3:
			rot_sensitivity = ROT_SENSITIVITY_3;
			break;
		default:
			rot_sensitivity = ROT_SENSITIVITY_0;
	}
}


void coordinates::update(raw_data new_data){
	if(count == 0)
		start = myclock::now();

	buff[count].Ac_X = new_data.Ac_X / acc_sensitivity;
	buff[count].Ac_Y = new_data.Ac_Y / acc_sensitivity;
	buff[count].Ac_Z = new_data.Ac_Z / acc_sensitivity;
	buff[count].Gy_X = new_data.Gy_X / acc_sensitivity;
	buff[count].Gy_Y = new_data.Gy_Y / acc_sensitivity;
	buff[count].Gy_Z = new_data.Gy_Z / acc_sensitivity;

	filterCurr();

	count += 1;
	if(count == buff_size){
		finish = myclock::now();
	}
}




/**
*
*		Helper Functions
*
**/

void coordinates::reset(){
	X = 0;
	Y = 0;
	Z = 0;
	vX = 0;
	vY = 0;
	vZ = 0;
	buff_size = 0;
	count = 0;
}

/**
*	Filter noise out of data
**/
void coordinates::filterCurr(){}



