#pragma once

#include <thread>
#include <vector>
#include <nlohmann/json.hpp>
#include <net/peer.hpp>
#include <blockchain.hpp>

namespace artha {

using nlohmann::json;

class Client {
public:
	Client();
	void AddPeer(Endpoint peer);
	void AddPeers(std::vector<Endpoint> peers);
	void AddPeers(const std::string peerString);

	std::vector<Peer> &Peers() { return _peers; }

	std::string Request(const std::string &msg);
	void Broadcast(const std::string &msg);

	static std::vector<Endpoint> ParsePeers(const std::string &peerString);

protected:
	net::io_context _ctx;
	tcp::resolver _resolver;
	std::vector<Peer> _peers;
};

}
