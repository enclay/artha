#include <net/peer.hpp>

#include <iostream>

namespace artha {

Peer::Peer(net::io_context &ctx, const Endpoint &peer)
	: _info(peer)
{
	_socket = make_shared<WebSocket>(net::make_strand(ctx));
}

Peer::~Peer()
{
	Close();
}

std::string Peer::Send(const std::string &text)
{
	try
	{
		beast::flat_buffer buffer;
		_socket->write(net::buffer(text));
		_socket->read(buffer);
		return beast::buffers_to_string(buffer.data());
	}
	catch (const beast::system_error &error)
	{
		if(error.code() != websocket::error::closed)
			std::cerr << "Send error: " << error.code().message() << std::endl;
		return "";
	}
}

void Peer::Connect(tcp::resolver &rv) 
{
	try {
		auto results = rv.resolve(_info.addr, std::to_string(_info.port));
		auto ep = net::connect(_socket->next_layer(), results);
		auto host = _info.addr + ':' + std::to_string(ep.port());
		_socket->handshake(host, "/");
	}
	catch (std::exception const &e) {
		std::cerr << "Connection error: " << e.what() << std::endl;
	}
}


void Peer::Close()
{
	if (_socket->is_open()) {
		_socket->close(websocket::close_code::normal);
	}
}

}
