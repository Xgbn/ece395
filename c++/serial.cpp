#include "serial.h"
#include <iostream>
#include <string>
#include <boost/asio.hpp> 
#include <boost/asio/serial_port.hpp>

using namespace boost;

serial_port::serial_port(){
}

serial_port::serial_port(std::string new_name){
	port.open(new_name);
	port.set_option(asio::serial_port_base::baud_rate(115200));
}

serial_port::~serial_port(){
	port.close();
}


void serial_port::open(std::string new_name){
	port.open(new_name);
	port.set_option(asio::serial_port_base::baud_rate(115200));
}


std::string serial_port::readline(){
	line.clear();
	c = '1';
	asio::write(port, asio::buffer(&c,1));
	while(1){
		asio::read(port, asio::buffer(&c,1));
		if(c == '\n')
			break;
		line += c;
	}
	return line;
}
