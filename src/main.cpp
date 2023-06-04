#include <net/client.hpp>
#include <net/server.hpp>

using namespace artha;

int main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "missing arguments" << std::endl;
		return -1;
	}

	std::string opt = argv[1];

	if (opt == "client") {
		Client client;
		client.AddPeer({"127.0.0.1", 5002});
		auto response = client.Request("Hey there!");
		std::cout << response << std::endl;
	}

	else if (opt == "server") {
		Server server;
		server.Listen(5002);
	}
}
