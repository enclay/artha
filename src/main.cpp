#include <iostream>
#include <crypto/ecdsa.hpp>

using namespace artha;

int main()
{
	ECPoint point;
	std::cout << "Public key: " << point.PublicKeyHex() << std::endl;
	return 0;
}
