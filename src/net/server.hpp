#pragma once

#include <boost/asio/io_context.hpp>
#include <net/peer.hpp>

#include <thread>

namespace artha {

class Server {
public:
	using HandlerFunc = std::function<std::string(const std::string &)>;

	Server();
	~Server();

	void Listen(uint16_t port);
	void SetHandler(HandlerFunc callback);
	void Wait();
	
protected:
	void MainLoop(Endpoint self);
	void SocketHandler(tcp::socket socket);

protected:
	HandlerFunc _handler;
	std::thread _thread;
	boost::asio::io_context _ctx;
	bool _running = false;
};

}
