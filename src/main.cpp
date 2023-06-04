#include <program_options.hpp>
#include <net/server.hpp>
#include <net/client.hpp>
#include <wallet.hpp>
#include <miner.hpp>
#include <termcolor/termcolor.hpp>

using namespace artha;

int main(int argc, char** argv)
{
	ProgramOptions opts(argc, argv);

	opts.Builder()
		.Arg<uint16_t>("port")
		.Arg<std::string>("address")
		.Arg<uint64_t>("amount")
		.Arg<std::string>("peers");

	opts.On("send", [&]()
	{
		auto peers = opts.Require<std::string>("peers");
		auto address = opts.Require<std::string>("address");
		auto amount = opts.Require<uint64_t>("amount");

		Client client;
		client.AddPeers(peers);

		Wallet wallet;
		auto [tx, err] = wallet.Send(address, amount);
		if (!err) {
			auto response = client.Request(tx.ToString());
			std::cout << termcolor::green << response << std::endl;
		}
	});

	opts.On("mine", [&]()
	{
		Block block;
		block.AddTransaction(Transaction::CreateRandom());
		block.AddTransaction(Transaction::CreateRandom());
		block.AddTransaction(Transaction::CreateRandom());

		Blockchain chain;
		chain.AddBlock(block);
		
		auto port = opts.Require<uint16_t>("port");
		Server server;
		server.SetHandler([&](const std::string &msg) -> std::string
		{
			auto tx = Transaction::FromString(msg);
			chain.AddTransaction(tx);
			Miner miner{chain};
			miner.StartWithTimeout(2000);	
			return "Transaction accepted by miner";
		});
		server.Listen(port);
	});

	opts.Process();
}
