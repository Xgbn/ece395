#pragma once
#include <string>
#include <ctime>
#include <vector>
#include <chrono>
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

#define ACC_NOISE_FILTER 


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
typedef struct {
	float Ac_X, Ac_Y, Ac_Z, Gy_X, Gy_Y, Gy_Z;
}data;


class coordinates{
public:
	coordinates();
	coordinates(int new_buff_size, int acc_sen = 0, int rot_sen = 0);
	~coordinates();
	void setBuff(int new_buff_size);
	void setSensitivity(int acc_sen, int rot_sen);
	void update(raw_data new_data);




private:
	float X, Y, Z;			// coordinates of x y z
	float vX, vY, vZ;		// speed of x y z
	int buff_size;
	int count, acc_sensitivity;
	float rot_sensitivity;
	vector<data> buff;
	mytime_point start, finish;


	void reset();
	void filterCurr();
};









