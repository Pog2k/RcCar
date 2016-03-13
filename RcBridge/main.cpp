//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


using boost::asio::ip::tcp;

boost::asio::io_service ioService;
boost::asio::serial_port serialPort(ioService);
boost::condition_variable RxComplete;
boost::condition_variable TcpRxComplete;
boost::mutex mutex;
boost::mutex mutex2;
std::string TmpString="";


const int TcpBuffer_maxLength = 1024;

void session(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
  try
  {
    for (;;)
    {
      char data[TcpBuffer_maxLength];

      boost::system::error_code error;
      size_t length = sock->read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
    	{
    	std::cout << "connection closed:" << "\n";
        break; // Connection closed cleanly by peer.
    	}
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout << "received:"<< data << "\n";
      mutex2.lock();
      TmpString = std::string(data);
      mutex2.unlock();
      std::cout << "send notify" << "\n";
      TcpRxComplete.notify_all();

      boost::asio::write(*sock, boost::asio::buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
	auto sock = boost::shared_ptr<tcp::socket>(new tcp::socket(io_service));
    a.accept(*sock);
    boost::thread t3(session,sock);
    t3.detach();
     //std::thread(session, std::move(sock)).detach();
  }
}

void writeSerial( )
{
	std::string s = "";
	boost::mutex::scoped_lock lock(mutex); // lock for wait condition
	std::cout << "Thread: writeSerial entered. Listen for thread which handles socket"<< std::endl;

	while(true)
	{
		TcpRxComplete.wait(lock);  // wait for notify from other thread
		mutex2.lock();
		serialPort.write_some(boost::asio::buffer(TmpString));
		mutex2.unlock();
		std::cout << "writeSerial to serial: "<< TmpString << std::endl;
	}
}

// read character from serial port
void readSerial()
{
	char c;
	const uint8_t serial_max_char_len = 50;
	uint8_t cnt=0;
	std::cout << "readSerial function entered" << std::endl;
	std::string serial_rx_buffer;

	// receive uart characters until newline \n
	while(true)
	{
		serial_rx_buffer.clear();
		do
		{
		   read(serialPort,boost::asio::buffer(&c,1));
		   serial_rx_buffer+=c;
		  std::cout << c << std::endl;
		   cnt++;
		}
		while(c != '\n' || cnt >=serial_max_char_len );

		if(c=='\n')
		{
			// send receive string
			// Do Anything with data. Maybe send ?
			 std::cout << "serial_rx_buffer:"<<serial_rx_buffer;
		}


		if(cnt >= serial_max_char_len)
		{
			//send receive string
			 std::cout << "max uart charactes received \n";
		}
		cnt=0;

	}
}




int main(int argc, char* argv[])
{
	// check input arguments and inform user if fails

    if (argc != 3)
    {
      std::cerr << "Missing input arguments" << std::endl;
      std::cerr << "Usage: specifiy socket port and tty  <port X> <ttyX>" << std::endl;
      return 1;
    }

	try
	    {
			// open and init usb port
	        serialPort.open(argv[2]); // /dev/tty{choose me}
	        serialPort.set_option(boost::asio::serial_port::baud_rate(9600));
	        serialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
	        serialPort.set_option(boost::asio::serial_port::character_size(boost::asio::serial_port::character_size(8)));
	        serialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
	        serialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));

	    }
	    catch (boost::system::system_error& error)
	    {
	        std::cout << error.what() << std::endl;
	    }

	    // create threads
	    boost::thread t1(&writeSerial);
	    boost::thread t2(&readSerial);

	    // threads running independent
	    t1.detach();
	    t2.detach();


  // open socket
  try
  {
    boost::asio::io_service io_service;
    server(io_service, std::atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  std::cout << "return main " << "\n";

  serialPort.close();
  return 0;
}
