#pragma once

#include <span>

#include <crypto/secp256k1.hpp>
#include <crypto/base16.hpp>

namespace artha {

using ByteArray = std::vector<uint8_t>;
using MessageHash = std::array<uint8_t, 32>;

class PublicKey {
public:
	PublicKey() = default;
	PublicKey(std::span<const uint8_t> data);

	void Set(std::span<const uint8_t> data);
	bool Verify(const MessageHash &msg, const ByteArray &signature);

	ByteArray ToHex() const { return EncodeBase16(this->ToBytes());}
	ByteArray ToBytes() const { return ByteArray{_data.begin(), _data.end()}; }

public:
	static constexpr size_t SIZE = 65;

protected:
	std::array<uint8_t, SIZE> _data;
};

}

inline std::ostream &operator<<(std::ostream &stream, artha::ByteArray k)
{
	return stream << std::string(k.begin(), k.end());
}
