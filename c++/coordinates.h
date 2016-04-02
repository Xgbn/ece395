#pragma once
#include <string>
#include <ctime>
#include <vector>
#include <chrono>
#include <iostream>
#include "json.h"

/**
*	sensitivity of acceleration is determined by AFS_SEL
*	Register 0x1C bit 4 bit 3
*
*	actual_acc = raw_acc / acc_sensitivity (in terms of g)

*	sensitivity of acceleration is determined by FS_SEL
*	Register 0x1B bit 4 bit 3
*
*	actual_rot = raw_rot / rot_sensitivity (in terms of degrees / second)
*
*****/

#define ACC_SENSITIVITY_0 16384
#define ACC_SENSITIVITY_1 8192
#define ACC_SENSITIVITY_2 4069
#define ACC_SENSITIVITY_3 2048
#define ROT_SENSITIVITY_0 131
#define ROT_SENSITIVITY_1 65.5f
#define ROT_SENSITIVITY_2 32.8f
#define ROT_SENSITIVITY_3 16.4f

#define DEFAULT_LOW_PASS 0.5f
#define VELOCITY_RESET_VAL 5
#define GRAVITY 9.81f


using namespace std;

typedef std::chrono::steady_clock myclock;
typedef myclock::time_point mytime_point;





// structure for raw hardware data
typedef struct {
	int Ac_X, Ac_Y, Ac_Z, Gy_X, Gy_Y, Gy_Z;
}raw_data;


/** structure for actuall data
* acceleration is mesured in g (9.8 m / s^2)
* rotation is mesured in (degrees / second)
**/
struct processed_data{
	float Ac_X=0, Ac_Y=0, Ac_Z=0, Gy_X=0, Gy_Y=0, Gy_Z=0;

	processed_data & operator=(const processed_data & other){
		if(&other == this)
			return *this;
		Ac_X = other.Ac_X;
		Ac_Y = other.Ac_Y;
		Ac_Z = other.Ac_Z;
		Gy_X = other.Gy_X;
		Gy_Y = other.Gy_Y;
		Gy_Z = other.Gy_Z;
		return *this;
	}

	processed_data operator+(const processed_data & other){
		processed_data newdata;
		newdata.Ac_X = this->Ac_X + other.Ac_X;
		newdata.Ac_Y = this->Ac_Y + other.Ac_Y;
		newdata.Ac_Z = this->Ac_Z + other.Ac_Z;
		newdata.Gy_X = this->Gy_X + other.Gy_X;
		newdata.Gy_Y = this->Gy_Y + other.Gy_Y;
		newdata.Gy_Z = this->Gy_Z + other.Gy_Z;
		return newdata;
	}
};

typedef processed_data data;


class coordinates{
public:
	coordinates();
	coordinates(int new_buff_size, int acc_sen = 0, int rot_sen = 0);
	~coordinates();
	void setBuff(int new_buff_size);
	void setSensitivity(int acc_sen, int rot_sen);
	void update(raw_data new_data);
	void print();




private:
	float X, Y, Z;			// coordinates of x y z
	float vX, vY, vZ;		// speed of x y z
	float aX, aY, aZ;
	float gX, gY, gZ;		// value for low pass filter
	float a;			// constant for low pass band
	int buff_size;
	int count, acc_sensitivity;
	int velocity_reset_counter;
	float rot_sensitivity;
	vector<data> buff;
	mytime_point start, finish;


	void reset();
	void filterCurr();
	void flush();
	data getBuffAvg();
};









