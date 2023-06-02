#include <crypto/base16.hpp>
#include <crypto/key.hpp>
#include <crypto/random.hpp>

#include <stdexcept>

namespace artha {

SecretKey::SecretKey(const std::string &hex)
{
	Set(DecodeBase16({hex.begin(), hex.end()}));
}

SecretKey::SecretKey(std::span<const uint8_t> keydata)
{
	Set(keydata);
}

void SecretKey::Set(std::span<const uint8_t> data)
{
	for (size_t i = 0; i < 32 && i < data.size(); i++)
		_keydata[i] = data[i];
}

void SecretKey::Set(const std::string &data)
{
	Set(std::vector<uint8_t>(data.begin(), data.end()));
}


std::tuple<ByteArray, bool> SecretKey::Sign(const MessageHash &msg)
{
	secp256k1_ecdsa_signature signature;
	int ret = secp256k1_ecdsa_sign(Secp256k1Context::Get(), &signature, msg.data(), _keydata.data(), secp256k1_nonce_function_rfc6979, nullptr);
	if (ret != 1)
		return std::make_tuple<ByteArray>({}, false);

	ByteArray buffer(SIGNATURE_SIZE);
	size_t bufferSize = SIGNATURE_SIZE;
	ret = secp256k1_ecdsa_signature_serialize_der(Secp256k1Context::Get(), &buffer[0], &bufferSize, &signature);
	if (ret != 1)
		return std::make_tuple<ByteArray>({}, false);

	buffer.resize(bufferSize);
	return std::make_tuple(buffer, true);
}

void SecretKey::GenerateNew()
{
	Set(RandomSecretKey());
	
	if (_keydata.size() != 32)
		throw std::runtime_error("size not equal to 32: " + std::to_string(_keydata.size()));

	if (!IsValid())
		throw std::runtime_error("unable to verify key");
}

PublicKey SecretKey::GetPublicKey(bool compressed)
{
	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_create(Secp256k1Context::Get(), &pubkey, _keydata.data()))
		throw std::runtime_error("invalid public key");

	ByteArray buffer;
	size_t outsize = PublicKey::SIZE;
	buffer.resize(outsize);

	unsigned flags = compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
	secp256k1_ec_pubkey_serialize(Secp256k1Context::Get(), buffer.data(), &outsize, &pubkey, flags);

	buffer.resize(outsize);
	return PublicKey(buffer);
}

bool SecretKey::IsValid()
{
	return secp256k1_ec_seckey_verify(Secp256k1Context::Get(), _keydata.data());
}

SecretKey SecretKey::New()
{
	SecretKey key;
	key.GenerateNew();
	return key;
}

}
