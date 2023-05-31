#include <crypto/ecdsa.hpp>

#include <crypto/random.hpp>
#include <crypto/sha256.hpp>

#include <chrono>
#include <random>
#include <tuple>
#include <vector>
#include <exception>
#include <iostream>


namespace artha {

ECPoint::ECPoint()
	: _ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY))
{
	std::string priv = GenerateRandomPrivateKey();
	_privkey = {priv.begin(), priv.end()};
	
	if (_privkey.size() != 32)
		throw ECPointException("size not equal to 32: " + std::to_string(_privkey.size()));

	if (!VerifyKey())
		throw ECPointException("Unable to create and verify key:	");

	if (!CreatePublicKey())
		throw ECPointException("Unable to create public key");
}

ECPoint::~ECPoint()
{
	secp256k1_context_destroy(_ctx);
}

ECPoint::ECPoint(const Key &privateKey)
	: _ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY))
{
	_privkey = artha::DecodeBase16(privateKey);

	if (!VerifyKey())
		throw ECPointException("Unable to create and verify key");

	if (!CreatePublicKey())
		throw ECPointException("Unable to create public key");
}

bool ECPoint::VerifyKey()
{
	return secp256k1_ec_seckey_verify(_ctx, _privkey.data());
}

bool ECPoint::CreatePublicKey(bool compressed)
{
	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_create(_ctx, &pubkey, _privkey.data()))
		return false;

	size_t outsize = PUBLIC_KEY_SIZE;
	_pubkey.resize(outsize);

	unsigned flags = compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
	secp256k1_ec_pubkey_serialize(_ctx, _pubkey.data(), &outsize, &pubkey, flags);

	_pubkey.resize(outsize);
	return true;
}

std::tuple<ByteArray, bool> ECPoint::Sign(const MessageHash &msg) const
{
	secp256k1_ecdsa_signature signature;
	int ret = secp256k1_ecdsa_sign(_ctx, &signature, msg.data(), _privkey.data(), secp256k1_nonce_function_rfc6979, nullptr);
	if (ret != 1)
		return std::make_tuple<ByteArray>({}, false);

	ByteArray buffer(SIGNATURE_SIZE);
	size_t bufferSize = SIGNATURE_SIZE;
	ret = secp256k1_ecdsa_signature_serialize_der(_ctx, &buffer[0], &bufferSize, &signature);
	if (ret != 1)
		return std::make_tuple<ByteArray>({}, false);

	buffer.resize(bufferSize);
	return std::make_tuple(buffer, true);
}

bool ECPoint::Verify(const std::array<uint8_t, 32> &messageHash, const ByteArray &signatureData, const Key &pubkeyData)
{
	if (pubkeyData.size() != PUBLIC_KEY_SIZE)
		throw ECPointException("Invalid public key size: " + std::to_string(pubkeyData.size()));

	secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, pubkeyData.data(), pubkeyData.size())) {
		std::cerr << "here error" << std::endl;
		return false;
	} 

	secp256k1_ecdsa_signature signature;
	if (!secp256k1_ecdsa_signature_parse_der(ctx, &signature, signatureData.data(), signatureData.size())) {
		return false;
	}

	secp256k1_ecdsa_signature_normalize(ctx, &signature, &signature);
	bool ret = secp256k1_ecdsa_verify(ctx, &signature, messageHash.data(), &pubkey);
	secp256k1_context_destroy(ctx);
	return ret;
}

}
