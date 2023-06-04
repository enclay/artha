#pragma once

#include <string>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>


namespace artha {

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;

using tcp = net::ip::tcp;

struct Endpoint {
	std::string addr;
	uint16_t port;
};

class Peer {
public:
	using WebSocket = websocket::stream<tcp::socket>;

	Peer(net::io_context &ctx, const Endpoint &peer);
	~Peer();

	void Connect(tcp::resolver &rv);
	void Close();

	std::string Send(const std::string &data);

protected:
	Endpoint _info;
	std::shared_ptr<WebSocket> _socket;
};

}
