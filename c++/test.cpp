#include <SerialStream.h>
#include <string>
#include <iostream>
using namespace LibSerial;

int main(){
	char currChar;
	std::string line;

	SerialStream my_serial_stream;
	my_serial_stream.Open("/dev/ttyUSB0");
	my_serial_stream.SetBaudRate( SerialStreamBuf::BAUD_115200 );
	my_serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;


	

	while(1){
		my_serial_stream << "1";
		line.clear();
		while(1){
			my_serial_stream >> currChar;
			if(currChar == '\n')
				break;
			line += currChar;
		}
		std::cout << line << std::endl << std::endl << std::endl;
	}

	return 0;
}