#pragma once

#include <blockchain.hpp>
#include <wallet.hpp>
#include <block.hpp>

namespace artha {

class Miner {
public:
	Miner(Blockchain &chain);

	void Start();
	void StartWithTimeout(unsigned timeout);

	void ProcessNextBlock();
	bool EnoughTransactions();
	bool MineBlock(Block &newBlock);

public:
	// little for test purposes
	static constexpr auto MIN_TX_PER_BLOCK = 1;

	// number of leading zeroes to search for
	static constexpr auto DIFFICULTY = 2;

	// how frequent should miner check new transactions
	static constexpr auto CHECK_INTERVAL = 1000;

protected:
	Blockchain &_chain;
};

}
