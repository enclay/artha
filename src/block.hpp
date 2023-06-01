#pragma once

#include <cstdint>
#include <ctime>
#include <initializer_list>

#include <transaction.hpp>

namespace artha {

class Block {
public:
	Block();
	Block(std::initializer_list<Transaction> txs);

	void SetHeight(uint8_t height) { _height = height; }
	uint8_t Height() const { return _height; }

	void SetNonce(uint8_t nonce) { _nonce = nonce; }
	uint8_t Nonce() const { return _nonce; }
	
	void SetPreviousHash(const std::string &hash) { _prevHash = hash; }
	std::string PreviousHash() const { return _prevHash; }

	void SetTimestamp(const std::time_t &time) { _timestamp = time; }
	std::time_t Timestamp() const { return _timestamp; }

	std::vector<Transaction> Transactions() const { return _txs; }
	void SetTransactions(std::vector<Transaction> txs) { _txs = txs; }

	void AddTransaction(const Transaction &tx);
	void UpdateTimestamp();
	std::string Hash() const;

	std::string ToString() const;
	static Block FromString(const std::string &raw);
	static Block Genesis();

protected:
	std::vector<Transaction> _txs;
	std::time_t _timestamp;
	uint8_t _height;
	uint8_t _nonce;
	std::string _prevHash;
};

}
