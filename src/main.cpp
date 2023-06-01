#include <iostream>
#include <block.hpp>

using namespace artha;

int main()
{
	Block block{
		Transaction::CreateRandom(),
		Transaction::CreateRandom(),
		Transaction::CreateRandom()
	};
	block.UpdateTimestamp();
	std::cout << block.ToString() << std::endl;

	return 0;
}
