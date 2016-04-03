#include <string>
#include <iostream>
#include <cmath>
#include <thread>
#include "serial.h"
#include "json.h"
#include "plot.h"
#include "coordinates.h"


using namespace std;

int main() {
	//double ax, ay, az, G;
	DotPlot dot;
	std::thread dot_th(&DotPlot::run, &dot);
	double x,y,z;
	x = 0;
	y = 0;
	z = 0;
	while(1) {
		if (x > 2){
			x = 0;
		}
		if (y > 2){
			y = 0;
		}
		if (z > 2){
			z = 0;
		}
		x += 0.01;
		y += 0.01;
		z += 0.01;
		dot.updateDot(x,y,z);
		usleep(30000);
	}
	return 0;
	serial_port port("/dev/ttyUSB0");
	string test_json;
	raw_data new_data;
	Json::Value root;
	Json::Reader reader;
	bool parse_success;
	coordinates mycoord(SAMPLE_SIZE);

	while(1){
		for(int i=0; i< SAMPLE_SIZE; i++){
			test_json = port.readline();
			parse_success = reader.parse(test_json.c_str(), root);
			if(parse_success){
			new_data.Ac_X = root["Ac_X"].asInt();
			new_data.Ac_Y = root["Ac_Y"].asInt();
			new_data.Ac_Z = root["Ac_Z"].asInt();
			new_data.Gy_X = root["Gy_X"].asInt();
			new_data.Gy_Y = root["Gy_Y"].asInt();
			new_data.Gy_Z = root["Gy_Z"].asInt();
			//cout << root["Gy_Z"].asInt() << endl;
			//G = sqrt(ax*ax + ay*ay + az*az);
			mycoord.update(new_data);}
			else{
				cout << "parse fail" << endl;
			}
		}
		mycoord.printRotSpeed();
		mycoord.printRot();
		cout << endl << endl << endl << endl << endl;
		//cout << G << endl << endl;
	}

	parse_success = reader.parse(test_json.c_str(), root);

	if(parse_success){
		cout << root << endl;
	
		cout << endl << endl;
	
		cout << root["Ac_X"] << endl;
	}
	else{
		cout << "parse fail" << endl;
	}

	return 0;
}