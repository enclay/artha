#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <ostream>

#include <secp256k1.h>
#include <secp256k1_ecdh.h>
#include <secp256k1_recovery.h>

#include <crypto/base16.hpp>

namespace artha {

typedef std::vector<unsigned char> ByteArray, Key;

class ECPointException: public std::runtime_error {
public:
	ECPointException(const std::string &error) noexcept
		: std::runtime_error(error) {  }

	const char *what() const noexcept
	{
		return std::runtime_error::what();
	}
};

class ECPoint {
public:
	ECPoint();
	ECPoint(const Key &privateKey);
	~ECPoint();

	using MessageHash = std::array<uint8_t, 32>;

	Key PublicKey() const { return _pubkey; }
	Key PrivateKey() const { return _privkey; }
	Key PublicKeyHex() const { return EncodeBase16(_pubkey); }
	Key PrivateKeyHex() const { return EncodeBase16(_privkey); }

	std::tuple<ByteArray, bool> Sign(const MessageHash &msg) const;
	static bool Verify(const MessageHash &msg, const ByteArray &signature, const Key &pubkey);

protected:
	bool VerifyKey();
	bool CreatePublicKey(bool compressed = false);

protected:
	static constexpr size_t PUBLIC_KEY_SIZE = 65;
	static constexpr size_t SIGNATURE_SIZE	= 72;

private:
	secp256k1_context *_ctx{nullptr};
	Key _pubkey;
	Key _privkey;
};

}

inline std::ostream &operator<<(std::ostream &stream, artha::Key k)
{
	return stream << std::string(k.begin(), k.end());
}
