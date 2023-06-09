#include <crypto/random.hpp>

#include <crypto/sha256.hpp>

#include <chrono>

namespace artha {

std::string RandomSecretKey()
{
	unsigned now = std::chrono::system_clock::now().time_since_epoch().count();
	std::seed_seq seed{now};
	std::mt19937_64 eng(seed);
	std::string randString;
	for (int i = 0; i < 10; i++)
		randString += eng();

	return HashToHex(SHA256(randString)).substr(0, 32);
}

}
