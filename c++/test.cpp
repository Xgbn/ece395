#include <string>
#include <iostream>
#include "serial.h"
#include "json.h"


using namespace std;

int main() {

	serial_port port("/dev/ttyUSB0");

	string test_json = "{\"Ac_X\": 1244, \"Ac_Y\": 772, \"Ac_Z\": 13948, \"Temp\": -3248, \"Gy_X\": -36, \"Gy_Y\": 118, \"Gy_Z\": -78}";

	Json::Value root;
	Json::Reader reader;
	bool parse_success;

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