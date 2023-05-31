#include <iostream>
#include <crypto/key.hpp>

using namespace artha;

int main()
{
	SecretKey secretKey;
	secretKey.GenerateNew();
	PublicKey publicKey = secretKey.GetPublicKey();

	std::string myMessage = "Some secret Some secret Some sec";
	MessageHash msg;

	for (size_t i = 0; i < 32 && i < myMessage.size(); i++) {
		msg[i] = myMessage[i];
	}

	std::cout << "Public key: " << publicKey.ToHex() << std::endl;
	std::cout << "Private key: " << secretKey.ToHex() << std::endl;
	std::cout << "Message [" << myMessage.size() << "]: " << myMessage << std::endl;

	auto [signature, ok] = secretKey.Sign(msg);
	if (ok)
		std::cout << "Signed!" << std::endl;
	else {
		std::cout << "Not signed!" << std::endl;
		return -1;
	}

	auto verified = publicKey.Verify(msg, signature);
	if (verified)
		std::cout << "Verified!" << std::endl;
	else
		std::cout << "Not verified!" << std::endl;

	return 0;
}
