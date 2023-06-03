#include <platform/system.hpp>
#include <transaction.hpp>
#include <blockchain.hpp>
#include <wallet.hpp>
#include <miner.hpp>

#include <chrono>
#include <iostream>

using namespace artha;

int main()
{
	Blockchain chain;
	chain.AddTransaction(Transaction::CreateRandom());
	chain.AddTransaction(Transaction::CreateRandom());

	Wallet wallet;

	Miner miner{chain};
	miner.Start();
}
