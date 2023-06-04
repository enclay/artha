#include <net/client.hpp>

#include <iostream>

namespace artha {

Client::Client()
	:  _resolver(_ctx)
{
}

void Client::AddPeers(std::vector<Endpoint> peers)
{
	for (Endpoint peer: peers)
		AddPeer(peer);
}

void Client::AddPeers(const std::string peerString)
{
	AddPeers(ParsePeers(peerString));
}

std::vector<Endpoint> Client::ParsePeers(const std::string &peerString)
{
	std::vector<Endpoint> endpoints;
	std::istringstream stream(peerString);
	std::string peer;
	while (getline(stream, peer, ';')) {
		if (std::size_t del = peer.find(":"); del != std::string::npos) {
			std::string addr = peer.substr(0, del);
			std::string port = peer.substr(del + 1);
			endpoints.emplace_back(addr, std::stoi(port));
		}
	}
	return endpoints;
}

void Client::AddPeer(Endpoint peer)
{
	_peers.emplace_back(_ctx, peer);
	_peers.back().Connect(_resolver);
}

std::string Client::Request(const std::string &msg)
{
	return _peers.front().Send(msg);
}

void Client::Broadcast(const std::string &msg)
{
	for (auto &peer: _peers)
		peer.Send(msg);
}

}
