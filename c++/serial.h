#pragma once
#include <string>
#include <boost/asio.hpp> 
#include <boost/asio/serial_port.hpp>


/**
*	This class is used to get values from hardware
*
*	Usage:
*		Create with constructor, passing name of port
*		Read dataline with readline()
**/

using namespace boost;

class serial_port{
	public:
		serial_port();
		serial_port(std::string new_name);
		void open(std::string new_name);
		~serial_port();
		std::string readline();

	private:
		asio::io_service io;
		asio::serial_port port = asio::serial_port(io);
		std::string line;
		char c;


};