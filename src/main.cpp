#include <wallet.hpp>
#include <iostream>

using namespace artha;

int main()
{
	Wallet wallet;

	if (auto [tx, err] = wallet.Send("AABB", 50); !err) 
		std::cout << "Transaction has been successfully signed!" << std::endl;
	else
		std::cout << ErrorToString(err) << std::endl;

	return 0;
}
