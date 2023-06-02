#include "crypto/base16.hpp"
#include <crypto/pubkey.hpp>

#include <stdexcept>

namespace artha {

void PublicKey::Set(std::span<const uint8_t> data)
{
	for (size_t i = 0; i < SIZE && i < data.size(); i++)
		_data[i] = data[i];
}

PublicKey::PublicKey(std::span<const uint8_t> data)
{
	Set(data);
}

PublicKey::PublicKey(std::string hex)
{
	Set(DecodeBase16({hex.begin(), hex.end()}));
}

bool PublicKey::Verify(const MessageHash &msg, const ByteArray &signatureData)
{
	if (_data.size() != SIZE)
		throw std::runtime_error("Invalid public key size: " + std::to_string(_data.size()));

	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_parse(Secp256k1Context::Get(), &pubkey, _data.data(), _data.size())) {
		return false;
	} 

	secp256k1_ecdsa_signature signature;
	if (!secp256k1_ecdsa_signature_parse_der(Secp256k1Context::Get(), &signature, signatureData.data(), signatureData.size())) {
		return false;
	}

	secp256k1_ecdsa_signature_normalize(Secp256k1Context::Get(), &signature, &signature);
	bool ret = secp256k1_ecdsa_verify(Secp256k1Context::Get(), &signature, msg.data(), &pubkey);
	secp256k1_context_destroy(Secp256k1Context::Get());
	return ret;
}

}
