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
	: _ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY)) {

	std::string new_key = GenerateRandomPrivateKey();
	_privkey = {new_key.begin(), new_key.end()};
	if (_privkey.size() != 32)
		throw ECPointException("size not equal to 32: " + std::to_string(_privkey.size()));

	if (!VerifyKey())
		throw ECPointException("Unable to create and verify key:	");

	if (!CreatePublicKey())
		throw ECPointException("Unable to create public key");
}

ECPoint::~ECPoint() {
	secp256k1_context_destroy(_ctx);
}

ECPoint::ECPoint(const Key &privateKey)
	: _ctx(secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY)) {

	_privkey = artha::DecodeBase16(privateKey);

	if (!VerifyKey())
		throw ECPointException("Unable to create and verify key");

	if (!CreatePublicKey())
		throw ECPointException("Unable to create public key");
}

bool ECPoint::VerifyKey() {
	return secp256k1_ec_seckey_verify(_ctx, _privkey.data());
}

bool ECPoint::CreatePublicKey(bool compressed) {
	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_create(_ctx, &pubkey, _privkey.data()))
		return false;

	size_t outsize = PUBLIC_KEY_SIZE;
	_pubkey.resize(outsize);
	secp256k1_ec_pubkey_serialize(_ctx, _pubkey.data(), &outsize, &pubkey,
		compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
	_pubkey.resize(outsize);
	return true;
}

std::tuple<std::vector<uint8_t>, bool> ECPoint::Sign(Bytes hash) const {
	secp256k1_ecdsa_signature sig;
	int ret = secp256k1_ecdsa_sign(_ctx, &sig, hash.data(), _privkey.data(),
		secp256k1_nonce_function_rfc6979, nullptr);
	if (ret != 1)
		return std::make_tuple(Key{}, false);

	Key sigout(SIGNATURE_SIZE);
	size_t sigout_sz = SIGNATURE_SIZE;
	ret = secp256k1_ecdsa_signature_serialize_der(_ctx, &sigout[0], &sigout_sz, &sig);
	if (ret != 1)
		return std::make_tuple(Key{}, false);

	sigout.resize(sigout_sz);
	return std::make_tuple(sigout, true);
}

bool ECPoint::Verify(Bytes msgHash, const Bytes sign, const Key pubKey) {

	if (pubKey.size() != PUBLIC_KEY_SIZE)
		throw ECPointException("Invalid public key size: " + std::to_string(pubKey.size()));

	secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

	secp256k1_pubkey pubkey;
	if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, pubKey.data(), pubKey.size()))
		return false;

	secp256k1_ecdsa_signature sig;
	if (secp256k1_ecdsa_signature_parse_der(ctx, &sig, sign.data(), sign.size()) == 0)
		return false;

	secp256k1_ecdsa_signature_normalize(ctx, &sig, &sig);
	bool ret = secp256k1_ecdsa_verify(ctx, &sig, msgHash.data(), &pubkey);
	secp256k1_context_destroy(ctx);
	return ret;
}

}
