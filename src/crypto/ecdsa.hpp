#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <ostream>

#include <secp256k1.h>
#include <crypto/base16.hpp>

namespace artha {

typedef std::vector<unsigned char> Bytes, Key;

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

	Key PublicKey() const { return _pubkey; }
	Key PrivateKey() const { return _privkey; }
	Key PublicKeyHex() const { return EncodeBase16(_pubkey); }
	Key PrivateKeyHex() const { return EncodeBase16(_privkey); }

	std::tuple<Key, bool> Sign(Bytes hash) const;
	static bool Verify(Bytes msg_hash, const Bytes sig, const Key pubkey);

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
