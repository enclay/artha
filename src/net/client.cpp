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

}
