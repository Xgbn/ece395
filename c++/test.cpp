#include <string>
#include <iostream>
#include <boost/asio.hpp> 
#include <boost/asio/serial_port.hpp> 


using namespace boost;

int main() {
	asio::io_service io;
	asio::serial_port port(io);

	port.open("/dev/ttyUSB0");
	port.set_option(asio::serial_port_base::baud_rate(115200));

	char c;
	std::string line;

	// Read 1 character into c, this will block
	// forever if no character arrives.

	while(1){
		line.clear();
		c = '1';
		asio::write(port, asio::buffer(&c,1));
		while(1){
			asio::read(port, asio::buffer(&c,1));
			if(c == '\n')
				break;
			line += c;
		}
		std::cout << line << std::endl << std::endl;
	}


	asio::read(port, asio::buffer(&c,1));

	port.close();

	return 0;
}