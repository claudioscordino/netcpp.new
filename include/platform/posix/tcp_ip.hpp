/*
 * Copyright (C) 2013 Evidence Srl - www.evidence.eu.com
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef TCP_IP_HPP_
#define TCP_IP_HPP_

#include <stdexcept>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>


#include "posix_socket.hpp"

namespace net {
namespace ip {
namespace tcp {

class socket: public PosixSocket {

public:
	/**
	 * @brief Constructor: opens socket
	 */
	socket(): PosixSocket(AF_INET, SOCK_STREAM, 0){}
};





/**
 * @brief Server for IPv4 TCP communications.
 */
class server: public socket {
public:
	/**
	 * @brief Constructor
	 *
	 * This constructor allocates a concrete class derived from socket.
	 */
	server(const std::string& port) {
			bind(port);
			listen(100);
		}


	/**
	 * @brief Constructor
	 *
	 * This constructor allocates a concrete class derived from socket.
	 * @param max_pending_connections Number of maximum allowed pending connections
	 */
	server(const std::string& port, int max_pending_connections) {
			bind(port);
			listen(max_pending_connections);
		}


	/**
	 * @brief Constructor to accept() a TCP connection on an existing socket.
	 *
	 * This constructor allocates a class derived from socket and accept a TCP connection on the given (existing) TCP socket.
	 * @param srv Existing TCP socket on which the new connection must be accepted.
	 */
	server(const server& srv){
			accept(srv);
	}

	/**
	 * @brief Method to accept() a connection on the socket.
	 *
	 * This method calls accept().
	 * This method is usually invoked on the server-side for stream communications.
	 * @param srv Socket on which the new connection must be accepted.
	 * @exception runtime_error in case of error in accept()
	 */
	inline void accept(const server& srv) {
		fd_ = ::accept(srv.fd_, NULL, 0);
		if (fd_ < 0) {
			ERROR("Error in accept()!");
			throw std::runtime_error("Accept error");
		}
	}


	/**
	 * @brief Listen operation
	 *
	 * This method calls listen() and allows to specify the number
	 * of maximum allowed pending connections.
	 * This method is usually invoked on the server-side.
	 * @exception runtime_error in case of error in listen()
	 */
	inline void listen(int max_pending_connections) const{
		if (::listen(fd_, max_pending_connections) < 0) {
			::close(fd_);
			ERROR("Error when listening");
			throw std::runtime_error ("Listen error");
		}
	}

	/**
	 * @brief Method to bind() the socket to an address.
	 *
	 * This method calls bind().
	 * This method is usually invoked on the server-side.
	 * @param addr Address which the socket must be bound to
	 * @exception runtime_error in case of error in bind()
	 */
	inline void bind (const std::string& port)
	{
		struct sockaddr_in serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(std::stoi(port));
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		if (::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0){
			::close(fd_);
			ERROR("Error when binding socket");
			throw std::runtime_error ("Bind error");
		}
	}

};


/**
 * @brief Client for IPv4 TCP communications.
 */
class client: public socket {
public:
	/**
	 * @brief Method to connect() the socket to an address.
	 *
	 * This method calls connect().
	 * This method is usually invoked on the client-side.
	 * @param addr Address which the socket must be connected to
	 * @exception runtime_error in case of error in connect()
	 */
	client (const std::string& address, const std::string& port)
	{
		struct sockaddr_in serv_addr;
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(std::stoi(port));


		struct in_addr add;
		inet_aton(address.c_str(), &add);
		bcopy(&add, &serv_addr.sin_addr.s_addr, sizeof(add));

		if (::connect(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
			::close(fd_);
			ERROR("Error when creating client socket");
			throw std::runtime_error ("Client socket error");
		}
	}

};

}}} // net::ip::tcp

#endif // TCP_IP_HPP_
