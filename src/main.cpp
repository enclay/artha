#include "blockchain.hpp"
#include <iostream>
#include <block.hpp>

using namespace artha;

int main()
{
	Blockchain chain = Blockchain::CreateRandom();
	Blockchain chain2 = Blockchain::FromString(chain.ToString());
	std::cout << chain.ToString() << std::endl;

	return 0;
}
