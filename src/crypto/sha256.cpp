#include <crypto/sha256.hpp>

#include <cstdint>
#include <openssl/evp.h>

#include <sstream>
#include <iomanip>
#include <iostream>

namespace artha {

Hash SHA256(const std::string &str)
{
	EVP_MD_CTX *context = EVP_MD_CTX_new();
	Hash buffer; unsigned bufferSize;

	if (!EVP_DigestInit_ex(context, EVP_sha256(), NULL)) {
		std::cerr << "Message digest initialization failed" << std::endl;
		EVP_MD_CTX_free(context);
		return {};
	}

	if (!EVP_DigestUpdate(context, str.c_str(), str.size())) {
		std::cerr << "Message digest update failed" << std::endl;
		EVP_MD_CTX_free(context);
		return {};
	}

	if (!EVP_DigestFinal_ex(context, buffer.data(), &bufferSize)) {
		std::cerr << "Message digest finalization failed" << std::endl;
		EVP_MD_CTX_free(context);
		return {};
	}

	EVP_MD_CTX_free(context);
	return buffer;
}

std::string HashToHex(const Hash &hash)
{
	std::stringstream ss;
	for(size_t i = 0; i < 32; i++) {
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
	}
	return ss.str();
}

}
