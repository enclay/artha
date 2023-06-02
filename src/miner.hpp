#pragma once

#include <blockchain.hpp>
#include <wallet.hpp>
#include <block.hpp>

namespace artha {

class Miner {
public:
	Miner(Blockchain &chain, const std::string &rewardAddr);

	void Start();
	bool MineBlock(Block &newBlock);

public:
	// little for test purposes
	static constexpr auto MIN_TX_PER_BLOCK = 1;
	static constexpr auto DIFFICULTY = 2;

protected:
	Blockchain &_chain;
	std::string _rewardAddr;
};

}
