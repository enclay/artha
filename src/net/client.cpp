#include <net/client.hpp>

#include <iostream>

namespace artha {

Client::Client()
	:  _resolver(_ctx)
{
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
