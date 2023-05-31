#include <iostream>
#include <transaction.hpp>

using namespace artha;

int main()
{
	auto tx = Transaction::FromString(Transaction::CreateRandom().ToString());
	std::cout <<  tx.ToString() << std::endl;
	return 0;
}
