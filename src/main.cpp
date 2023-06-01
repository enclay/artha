#include "blockchain.hpp"
#include "termcolor/termcolor.hpp"
#include <iostream>
#include <block.hpp>

using namespace artha;

int main()
{
	Blockchain chain = Blockchain::CreateRandom();
	std::cout << chain.ToString() << std::endl;
	return 0;
}
