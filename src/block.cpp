#include "crypto/sha256.hpp"
#include <block.hpp>

#include <chrono>
#include <sstream>
#include <nlohmann/json.hpp>

namespace artha {

using namespace nlohmann;

Block::Block()
{
	UpdateTimestamp();
}

Block::Block(std::initializer_list<Transaction> txs): _txs(txs)
{
	UpdateTimestamp();
}

std::string Block::ToString() const
{
	std::vector<std::string> txs;
	for (auto &tx: _txs)
		txs.push_back(tx.ToString());

	json j;
	j["nonce"] = _nonce;
	j["height"] = _height;
	j["timestamp"] = _timestamp;
	j["txcount"] = _txs.size();
	j["prevhash"] = _prevHash;
	j["txs"] = txs;
	return j.dump();
}

Block Block::FromString(const std::string &raw)
{
	Block block;
	json j = json::parse(raw);
	return block;
}

Block Block::Genesis() {
	Transaction tx;
	tx.AddInput({1000, "fffff"});
	tx.AddOutput({1000, "0431adaf38ba920405bbdb788cb5018027e31c4ad002563f5b8410a855540083b2cd6fc850f3062a3774b65d2754545d61027ca8af3ad5978f1be36e7e57c610ef"});

	Block block;
	block.AddTransaction(tx);
	block.SetPreviousHash("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
	block.SetHash(block.CalcHash());
	block.SetHeight(0);
	
	return block;
}

void Block::UpdateTimestamp()
{
	auto now = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()
	);
	_timestamp = elapsed.count();
}

std::string Block::CalcHash() const
{
	return SHA256(ToString());
}

void Block::AddTransaction(const Transaction &tx)
{
	_txs.push_back(tx);
}

}
