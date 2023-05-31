#include <iostream>
#include <crypto/ecdsa.hpp>

using namespace artha;

int main()
{
	ECPoint point;
	std::string myMessage = "Some secret Some secret Some sec";
	std::string myMessage2 = "Some secret Some secret Some sec";
	ECPoint::MessageHash msg, msg2;
	for (size_t i = 0; i < 32 && i < myMessage.size(); i++) {
		msg[i] = myMessage[i];
		msg2[i] = myMessage2[i];
	}

	std::cout << "Public key: " << point.PublicKeyHex() << std::endl;
	std::cout << "Private key: " << point.PrivateKeyHex() << std::endl;
	std::cout << "Message [" << myMessage.size() << "]: " << myMessage << std::endl;
	
	auto [signature, ok] = point.Sign(msg);

	if (ok)
		std::cout << "Signed!" << std::endl;
	else {
		std::cout << "Not signed!" << std::endl;
		return -1;
	}

	auto verified = ECPoint::Verify(msg2, signature, point.PublicKey());
	if (verified)
		std::cout << "Verified!" << std::endl;
	else
		std::cout << "Not verified!" << std::endl;

	return 0;
}
