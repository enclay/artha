#include <transaction.hpp>

#include <blockchain.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

namespace artha {

using nlohmann::json;

Blockchain::Blockchain(bool isMain): _isMain(isMain)
{
	if (_isMain && std::filesystem::exists(CHAIN_FILENAME)) {
		Load();
	}
}
Blockchain::~Blockchain()
{
	if (_isMain) {
		Save();
	}
}

void Blockchain::AddBlock(Block block)
{
	if (block.Hash().empty())
		block.SetHash(block.CalcHash());

	block.SetHeight(LastBlock().Height() + 1);
	block.SetPreviousHash(LastBlock().Hash());

	_blocks.push_back(block);
}

Block Blockchain::LastBlock() const
{
	return _blocks.size() ? _blocks.back() : Block{};
}

void Blockchain::AddTransaction(const Transaction &tx)
{
	_pool.push_back(tx);
}

void Blockchain::Load(const std::string &filename)
{
	std::ifstream str(filename);
	std::stringstream buffer; buffer << str.rdbuf();
	*this = Blockchain::FromString(buffer.str());
	str.close();	
}

void Blockchain::Save(const std::string &filename)
{
	std::ofstream str(filename);
	str << ToString() << std::endl;
	str.close();
}

std::string Blockchain::ToString() const
{
	std::vector<std::string> blocks, pool;

	for (auto &it: _blocks)
		blocks.push_back(it.ToString());

	for (auto &it: _pool)
		pool.push_back(it.ToString());

	json j;
	j["blocks"] = blocks;
	j["pool"] = pool;
	return j.dump();
}

void Blockchain::ClearPool()
{
	_pool.clear();
}

bool Blockchain::IsEmpty() const
{
	return _blocks.empty();
}

Blockchain Blockchain::operator=(const Blockchain &other)
{
	SetBlocks(other.Blocks());
	SetPool(other.Pool());
	return Save(), *this;
}

Blockchain Blockchain::FromString(const std::string &raw)
{
	Blockchain chain;
	json j = json::parse(raw);

	for (auto &it: j["blocks"])
		chain.AddBlock(Block::FromString(it));
	
	for (auto &it: j["pool"])
		chain.AddTransaction(Transaction::FromString(it));

	return chain;
}

Blockchain Blockchain::CreateRandom()
{
	Block b1 = {
		Transaction::CreateRandom(),
		Transaction::CreateRandom(),
		Transaction::CreateRandom()
	};


	Block b2 = {
		Transaction::CreateRandom(),
		Transaction::CreateRandom(),
		Transaction::CreateRandom()
	};

	Block b3 = {
		Transaction::CreateRandom(),
		Transaction::CreateRandom(),
		Transaction::CreateRandom()
	};

	Blockchain chain;
	chain.AddBlock(b1);
	chain.AddBlock(b2);
	chain.AddBlock(b3);

	return chain;
}

uint64_t Blockchain::GetBalance(const std::string &address) const
{
	uint64_t balance = 0;

	for (auto &block: _blocks) {
		for (auto &tx: block.Transactions()) {

			for (auto &input: tx.Inputs())
				if (input.address == address)
					balance -= std::min(balance, input.amount);

			for (auto &output: tx.Outputs())
				if (output.address == address)
					balance += output.amount;
		}
	}

	return balance;
}

}
