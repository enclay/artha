#pragma once

#include <transaction.hpp>
#include <block.hpp>
#include <nlohmann/json.hpp>

namespace artha {

using nlohmann::json;

class Blockchain {
public:
	Blockchain(bool isMain = false);
	~Blockchain();

	Blockchain operator=(const Blockchain &other);
	
	void AddBlock(Block block);
	void AddTransaction(const Transaction &tx);

	std::vector<Block> Blocks() const { return _blocks; }
	void SetBlocks(std::vector<Block> blocks) { _blocks = blocks; }

	std::vector<Transaction> Pool() const { return _pool; }
	void SetPool(std::vector<Transaction> pool) { _pool = pool; }

	void ClearPool();
	bool IsEmpty() const;
	uint64_t GetBalance(const std::string &address) const;
	Block LastBlock() const;

	void Load(const std::string &filename = CHAIN_FILENAME);
	void Save(const std::string &filename = CHAIN_FILENAME);

	static Blockchain FromString(const std::string &raw);
	static Blockchain CreateRandom();
	std::string ToString() const;

public:
	static constexpr auto CHAIN_FILENAME = "chain.json";

protected:
	std::vector<Block> _blocks;
	std::vector<Transaction> _pool;
	
	// controls whether blockchain should be saved in filesystem
	bool _isMain;
};

}
