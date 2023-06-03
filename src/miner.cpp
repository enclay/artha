
#include <miner.hpp>

#include <settings.hpp>
#include <crypto/sha256.hpp>
#include <platform/system.hpp>

#include <termcolor/termcolor.hpp>

#include <iostream>
#include <limits>

namespace artha {

Miner::Miner(Blockchain &chain, const std::string &rewardAddr)
	: _chain(chain), _rewardAddr(rewardAddr)
{
	Settings settings;
	_rewardAddr = settings.RewardAddress();
	if (_rewardAddr.empty()) {
		_rewardAddr = Wallet().GetActiveAddress();
		settings.SetRewardAddress(_rewardAddr);
	}
}

void Miner::Start()
{
	for (;;) {
		while (_chain.Pool().size() < MIN_TX_PER_BLOCK) {
			std::cout << termcolor::yellow << "Not enough transactions" << std::endl;
			artha::Sleep(1000);
		}

		Block newBlock;
		for (const auto &tx: _chain.Pool())
			newBlock.AddTransaction(tx);

		std::cout << termcolor::red << "Trying to mine a block..." << std::endl;

		while (!MineBlock(newBlock));

		_chain.ClearPool();
	}
}

bool Miner::MineBlock(Block &newBlock)
{
	Block &&prev = _chain.LastBlock();
	newBlock.SetHeight(prev.Height() + 1);
	newBlock.SetPreviousHash(prev.Hash());
	newBlock.UpdateTimestamp();
	
	static constexpr auto MAX_NONCE = std::numeric_limits<uint8_t>::max();

	for (uint8_t n = 0; n < MAX_NONCE; n++) {
		newBlock.SetNonce(n);
		Hash &&hash = artha::SHA256(newBlock.CalcHash());
		
		bool hasLeadingZero = true;
		for (size_t i = 0; i < DIFFICULTY; i++) {
			hasLeadingZero &= (hash[i] == '0');
		}

		if (hasLeadingZero)	{
			std::cout << termcolor::green << "Block with " << newBlock.Transactions().size() << " transactions mined!" << std::endl;
			std::cout << termcolor::green << "Nonce is: " << (size_t)n << std::endl;
			return true;
		}
	}
	return false;
}

}
