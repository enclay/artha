#pragma once

#include <wallet.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

#include <crypto/pubkey.hpp>
#include <crypto/sha256.hpp>
#include <crypto/key.hpp>

namespace artha {

using nlohmann::json;

Wallet::Wallet()
{
	if (std::filesystem::exists(WALLET_FILENAME)) {
		Load(WALLET_FILENAME);
	}
}

Wallet::~Wallet()
{
	Save(WALLET_FILENAME);
}

std::string Wallet::CreateAddress()
{
	SecretKey priv = SecretKey::New();
	PublicKey pub = priv.GetPublicKey();

	auto pubHex = pub.ToHex();
	auto privHex = priv.ToHex();

	auto pubRaw = std::string(pubHex.begin(), pubHex.end());
	auto privRaw = std::string(privHex.begin(), privHex.end());

	_keypairs.push_back({pubRaw, privRaw});
	return pubRaw;
}

std::list<std::string> Wallet::Addresses()
{
	std::list<std::string> addresses;
	for (const auto &it: _keypairs) {
		addresses.push_back(it.pubKey);
	}
	return addresses;
}

void Wallet::Save(const std::string &name)
{
	std::ofstream out(WALLET_FILENAME);

	std::vector<json> keypairs;
	for (auto &it : _keypairs) {
		keypairs.push_back({
			{"public", it.pubKey},
			{"secret", it.secretKey},
		});
	}

	out << json(keypairs).dump();
	out.close();
}

void Wallet::Load(const std::string &name)
{
	std::ifstream str(WALLET_FILENAME);
	std::stringstream buffer; buffer << str.rdbuf();
	std::string raw = buffer.str();

	try {
		json j = json::parse(raw);
		for (auto &v: j) {
			if (v.contains("public") && v.contains("secret")) {
				_keypairs.emplace_back(v["public"], v["secret"]);
			}
		}
	}
	catch (...) {}
}

std::string Wallet::GetActiveAddress()
{
	return _keypairs.empty() ? CreateAddress() : _keypairs.front().pubKey;
}

std::string Wallet::GetSecretKey(const std::string &pubkey)
{
	for (const auto &v: _keypairs) {
		if (v.pubKey == pubkey) {
			return v.secretKey;
		}
	}
	return {};
}

bool Wallet::SignInputs(Transaction &tx)
{
	for (auto &input: tx.Inputs()) {

		std::string priv = GetSecretKey(input.address);
		if (priv.empty()) return false;

		SecretKey sk(priv);

		Hash hash = SHA256(input.Serialize());

		auto [signature, ok] = sk.Sign(hash);
		if (!ok) return false;

		input.signature = signature;
	}
	return true;
}

std::tuple<Transaction, TransactionError> Wallet::Send(const std::string &to, const uint64_t &amount)
{
	Transaction tx;
		 
	tx.AddInput({amount, GetActiveAddress()});
	tx.AddOutput({amount, to});

	// Sign individual inputs
	if (!SignInputs(tx))
		return std::make_tuple(tx, TransactionError::MISSING_INPUTS);

	auto err = tx.Validate();
	return std::make_tuple(tx, err);
}

}
