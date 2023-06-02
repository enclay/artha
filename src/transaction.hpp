#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include <crypto/key.hpp>
#include <crypto/pubkey.hpp>
#include <crypto/sha256.hpp>
#include <crypto/base16.hpp>
#include <crypto/random.hpp>

#include <error.hpp>

#include <nlohmann/json.hpp>

namespace artha {

using nlohmann::json;

struct Input {
	uint64_t amount;
	std::string address;
	ByteArray signature;

	std::string Serialize() const;
	void Deserialize(const std::string &raw);
};

struct Output {
	uint64_t amount;
	std::string address;

	std::string Serialize() const;
	void Deserialize(const std::string &raw);
};

class Transaction {
public:
	std::vector<Input> &Inputs() { return _inputs; }
	std::vector<Output> &Outputs() { return _outputs; }

	void AddInput(const Input &input) { _inputs.push_back(input); }
	void AddOutput(const Output &output) { _outputs.push_back(output); }

	std::string ToString() const;
	Hash ToHash() const;

	TransactionError Validate();

	static Transaction Create(const std::string &from, const std::string &to, uint64_t amount);
	static Transaction CreateRandom();
	static Transaction FromString(const std::string &raw);

protected:
	std::vector<Input> _inputs;
	std::vector<Output> _outputs;
};

}
