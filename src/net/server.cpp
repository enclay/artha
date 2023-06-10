#include <net/server.hpp>

namespace artha {

Server::Server()
{
	_handler = [](const std::string &msg) {
		return "Echo from server: " + msg;
	};
}

Server::~Server()
{
	if (_running)
		Wait();	
}

void Server::SetHandler(Server::HandlerFunc callback)
{
	_handler = callback;
}

void Server::Listen(uint16_t port)
{
	Endpoint self{"127.0.0.1", port};

	_thread = std::thread(&Server::MainLoop, this, self);
	_running = true;
	std::cout << "Listening on port " << port << "..." << std::endl;
}

void Server::Wait() {
	_thread.join();
}

void Server::MainLoop(Endpoint self)
{
	try {
		tcp::endpoint endpoint{net::ip::make_address(self.addr), self.port};
		tcp::acceptor acceptor{_ctx, endpoint};
		for(;;) {
			tcp::socket socket{_ctx};
			acceptor.accept(socket);

			std::thread(&Server::SocketHandler, this, std::move(socket)).detach();
		}
	}
	catch (const std::runtime_error &e) {
		std::cout << "Loop error: " << e.what() << std::endl;
	}
}

void Server::SocketHandler(tcp::socket socket)
{
	try {
		websocket::stream<tcp::socket> ws{std::move(socket)};
		ws.accept();

		for(;;) {
			beast::flat_buffer buffer;
			ws.read(buffer);
			ws.text(ws.got_text());

			std::string request = beast::buffers_to_string(buffer.data());
			std::string response = _handler(request);
			ws.write(net::buffer(response));
		}
	}
	catch (const beast::system_error &e) {
		if(e.code() != websocket::error::closed)
			std::cout << "Socket closed: " << e.code().message() << std::endl;
	}
}

}
