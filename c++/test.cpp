#include <string>
#include <iostream>
#include "serial.h"


using namespace std;

int main() {
	// asio::io_service io;
	// asio::serial_port* port;

	// port = new asio::serial_port(io);

	// port->open("/dev/ttyUSB0");
	// port->set_option(asio::serial_port_base::baud_rate(115200));

	// char c;
	// std::string line;

	// while(1){
	// 	line.clear();
	// 	c = '1';
	// 	asio::write(*port, asio::buffer(&c,1));
	// 	while(1){
	// 		asio::read(*port, asio::buffer(&c,1));
	// 		if(c == '\n')
	// 			break;
	// 		line += c;
	// 	}
	// 	std::cout << line << std::endl << std::endl;
	// }
	// port->close();
	// delete port;

	serial_port port("/dev/ttyUSB0");
	while(1){
		cout << port.readline() << endl << endl;
	}

	return 0;
}