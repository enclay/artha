#pragma once

#include <cstdint>
#include <list>

#include <crypto/key.hpp>
#include <transaction.hpp>

namespace artha {

struct Keypair {
	std::string pubKey;
	std::string secretKey;
};

class Wallet {
public:
	Wallet();
	~Wallet();

	std::string CreateAddress();
	std::string GetActiveAddress();
	std::list<std::string> Addresses();

	void Save(const std::string &name);
	void Load(const std::string &name);
	std::string GetSecretKey(const std::string &pubkey);

	bool SignInputs(Transaction &tx);

	std::tuple<Transaction, TransactionError>
	Send(const std::string &to, const uint64_t &amount);

public:
	static constexpr auto WALLET_FILENAME = "wallet.json";

protected:
	std::vector<Keypair> _keypairs;
};

};
