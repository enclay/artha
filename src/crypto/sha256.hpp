#pragma once

#include <string>
#include <vector>
#include <array>

namespace artha {
	using Hash = std::array<uint8_t, 32>;

	Hash SHA256(const std::string &str);
	std::string HashToHex(const Hash &hash);
}
