#include "coordinates.h"
#include <algorithm>
#include <cmath>

using namespace std;

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

	buff[count].Ac_X = GRAVITY * (double)new_data.Ac_X / acc_sensitivity;
	buff[count].Ac_Y = GRAVITY * (double)new_data.Ac_Y / acc_sensitivity;
	buff[count].Ac_Z = GRAVITY * (double)new_data.Ac_Z / acc_sensitivity;
	buff[count].Gy_X = (double)new_data.Gy_X / acc_sensitivity;
	buff[count].Gy_Y = (double)new_data.Gy_Y / acc_sensitivity;
	buff[count].Gy_Z = (double)new_data.Gy_Z / acc_sensitivity;

	filterCurr();

	count += 1;
	if(count == buff_size){
		finish = myclock::now();
		flush();
	}
}

void coordinates::print(){
	cout << "X: " << X << endl;
	cout << "Y: " << Y << endl;
	cout << "Z: " << Z << endl;
	cout << "vX: " << vX << endl;
	cout << "vY: " << vY << endl;
	cout << "vZ: " << vZ << endl;
	cout << "aX: " << aX << endl;
	cout << "aY: " << aY << endl;
	cout << "aZ: " << aZ << endl;
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
	gX = 0;
	gY = 0;
	gZ = 0;
	buff_size = 0;
	count = 0;
	velocity_reset_counter = 0;
	a = DEFAULT_LOW_PASS;
}

/**
*	Filter noise out of data
**/
void coordinates::filterCurr(){
	gX = gX * a + (1-a) * buff[count].Ac_X;
	gY = gY * a + (1-a) * buff[count].Ac_Y;
	gZ = gZ * a + (1-a) * buff[count].Ac_Z;

	buff[count].Ac_X -= gX;
	buff[count].Ac_Y -= gY;
	buff[count].Ac_Z -= gZ;
}

void coordinates::flush(){
	float delta;
	float test;
	float preVX, preVY, preVZ;
	data avg;
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
	delta = time_span.count();
	avg = getBuffAvg();
	preVX = vX;
	preVY = vY;
	preVZ = vZ;
	vX += avg.Ac_X * delta;
	vY += avg.Ac_Y * delta;
	vZ += avg.Ac_Z * delta;
	test = max(max(abs(aX), abs(aY)), abs(aZ));
	//cout << "test: " << test << endl;
	if(max(max(abs(aX), abs(aY)), abs(aZ)) < 0.3)
		if(velocity_reset_counter != VELOCITY_RESET_VAL)
			velocity_reset_counter += 1;
		else{
			vX = 0;
			vY = 0;
			vZ = 0;
			count = 0;
			cout << "hold velocity at zero..." << endl;
			return;
		}
	else
		velocity_reset_counter = 0;
	X += (preVX + vX) * delta / 2;
	Y += (preVY + vY) * delta / 2;
	Z += (preVZ + vZ) * delta / 2;

	//std::cout << delta << std::endl;
	count = 0;
}

data coordinates::getBuffAvg(){
	data avg;
	if(count == 0)
		return avg;
	for(int i = 0; i < count; i++){
		avg = avg + buff[i];
	}
	avg.Ac_X = avg.Ac_X / count;
	avg.Ac_Y = avg.Ac_Y / count;
	avg.Ac_Z = avg.Ac_Z / count;
	avg.Gy_X = avg.Gy_X / count;
	avg.Gy_Y = avg.Gy_Y / count;
	avg.Gy_Z = avg.Gy_Z / count;

	aX = avg.Ac_X;
	aY = avg.Ac_Y;
	aZ = avg.Ac_Z;

	return avg;
}

