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
	if(first){
		start = myclock::now();
		first = false;
	}

	buff[count].Ac_X = GRAVITY * (double)new_data.Ac_X / acc_sensitivity;
	buff[count].Ac_Y = GRAVITY * (double)new_data.Ac_Y / acc_sensitivity;
	buff[count].Ac_Z = GRAVITY * (double)new_data.Ac_Z / acc_sensitivity;
	buff[count].Gy_X = (double)new_data.Gy_X / rot_sensitivity;
	buff[count].Gy_Y = (double)new_data.Gy_Y / rot_sensitivity;
	buff[count].Gy_Z = (double)new_data.Gy_Z / rot_sensitivity;


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

void coordinates::printPos(){
	cout << "X: " << X << endl;
	cout << "Y: " << Y << endl;
	cout << "Z: " << Z << endl;
}

void coordinates::printRot(){
	cout << "rX: " << rX << endl;
	cout << "rY: " << rY << endl;
	cout << "rZ: " << rZ << endl;
}

void coordinates::printRotSpeed(){
	cout << "gyX: " << gyX << endl;
	cout << "gyY: " << gyY << endl;
	cout << "gyZ: " << gyZ << endl;
}

float coordinates::getX(){
	return X;
}

float coordinates::getY(){
	return Y;
}

float coordinates::getZ(){
	return Z;
}

void coordinates::printToFile(ofstream& f){
  //  if(first_zero){
  //      return;
  //  }
    if(!f.is_open()){
        return;
    }
    f << elapsed_time << ",";
    f << X << ",";
    f << Y << ",";
    f << Z << ",";
    f << vX << ",";
    f << vY << ",";
    f << vZ << ",";
    f << aX << ",";
    f << aY << ",";
    f << aZ << endl;



    return;
}


/**
*
*		Helper Functions
*
**/

void coordinates::reset(){
	elapsed_time = 0.0;
        X = 0;
	Y = 0;
	Z = 0;
	rX = 0;
	rY = 0;
	rZ = 0;
	gyX = 0;
	gyY = 0;
	gyZ = 0;
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
	first = true;
	first_zero = true;
}

/**
*	Filter noise out of data
**/
void coordinates::filterCurr(){
        return;
	gX = gX * a + (1-a) * buff[count].Ac_X;
	gY = gY * a + (1-a) * buff[count].Ac_Y;
	gZ = gZ * a + (1-a) * buff[count].Ac_Z;

	buff[count].Ac_X -= gX;
	buff[count].Ac_Y -= gY;
	buff[count].Ac_Z -= gZ;

	// if(abs(buff[count].Ac_X) < 0.3)
	// 	buff[count].Ac_X = 0;
	// if(abs(buff[count].Ac_Y) < 0.3)
	// 	buff[count].Ac_Y = 0;
	// if(abs(buff[count].Ac_Z) < 0.3)
	// 	buff[count].Ac_Z = 0;
}

void coordinates::flush(){
	float delta;
	float preVX, preVY, preVZ;
	float prevGyX, prevGyY, prevGyZ;
	data avg;
	// get time interval
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
	delta = time_span.count();
	elapsed_time += delta;
        start = finish;
	// get avg of previous data
	avg = getBuffAvg();
	// record previous velocity
	preVX = vX;
	preVY = vY;
	preVZ = vZ;
	// update new velocity
	vX += avg.Ac_X * delta;
	vY += avg.Ac_Y * delta;
	vZ += avg.Ac_Z * delta;
	// update rotation and modulate inside [0, 360]
	prevGyX = gyX;
	gyX = avg.Gy_X;
	rX += (prevGyX + gyX) * delta / 2;
	if(rX < 0)
		rX += 360;
	else if(rX > 360)
		rX -= 360;
	prevGyY = gyY;
	gyY = avg.Gy_Y;
	rY += (prevGyY + gyY) * delta / 2;
	if(rY < 0)
		rY += 360;
	else if(rY > 360)
		rY -= 360;
	prevGyZ = gyZ;
	gyZ = avg.Gy_Z;
	rZ += (prevGyZ + gyZ) * delta / 2;
	if(rZ < 0)
		rZ += 360;
	else if(rZ > 360)
		rZ -= 360;
	// check if velocity should be reset to zero
	if(max(max(abs(aX), abs(aY)), abs(aZ)) < 0.2 && false)
		if(velocity_reset_counter != VELOCITY_RESET_VAL)
			velocity_reset_counter += 1;
		else{
			if(first_zero){
				X = 0;
				Y = 0;
				Z = 0;
				
                                
                                
                                first_zero = false;
			}
			vX = 0;
			vY = 0;
			vZ = 0;
			count = 0;
			cout << "hold velocity at zero..." << endl;
			return;
		}
	else
		velocity_reset_counter = 0;
	// accumulate position shift based on velocity
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

