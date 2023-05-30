#include <crypto/random.hpp>

#include <crypto/sha256.hpp>

#include <chrono>
#include <random>

namespace artha {

std::string GenerateRandomPrivateKey()
{
	unsigned now = std::chrono::system_clock::now().time_since_epoch().count();
	std::seed_seq seed{now};
	std::mt19937_64 eng(seed);
	std::string randString;
	for (int i = 0; i < 10; i++)
		randString += eng();

	return artha::SHA256(randString).substr(0, 32);
}

}
