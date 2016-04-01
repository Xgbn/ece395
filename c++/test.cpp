#include <string>
#include <iostream>
#include <cmath>
#include "serial.h"
#include "json.h"
#include "coordinates.h"


using namespace std;

int main() {
	double ax, ay, az, G;
	serial_port port("/dev/ttyUSB0");

	string test_json;

	Json::Value root;
	Json::Reader reader;
	bool parse_success;

	while(1){
		test_json = port.readline();
		parse_success = reader.parse(test_json.c_str(), root);
		if(parse_success){
		ax = (double)root["Ac_X"].asInt() / ACC_SENSITIVITY_0;
		ay = (double)root["Ac_Y"].asInt() / ACC_SENSITIVITY_0;
		az = (double)root["Ac_Z"].asInt() / ACC_SENSITIVITY_0;
		G = sqrt(ax*ax + ay*ay + az*az);

		cout << G << endl << endl;
	}
	else{
		cout << "parse fail" << endl;
	}
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