#pragma once

#include <cstdint>
#include <tuple>

#include <crypto/secp256k1.hpp>
#include <crypto/pubkey.hpp>

namespace artha {

class SecretKey {
public:
	SecretKey() = default;
	SecretKey(std::span<const uint8_t> keydata);

	void Set(const std::string &string);
	void Set(std::span<const uint8_t> keydata);
	
	std::tuple<ByteArray, bool> Sign(const MessageHash &msg);
	PublicKey GetPublicKey(bool compresed = false);

	void GenerateNew();
	bool IsValid();

	ByteArray ToHex() const { return EncodeBase16(this->ToBytes());}
	ByteArray ToBytes() const { return {_keydata.begin(), _keydata.end()}; }

	static SecretKey New();

public:
	static constexpr size_t SIGNATURE_SIZE = 72;
	static constexpr size_t SIZE = 32;

protected:
	std::array<uint8_t, SIZE> _keydata;

};


}

