#include <transaction.hpp>

#include <nlohmann/json.hpp>

namespace artha {

using namespace nlohmann;

std::string Input::Serialize() const
{
	return json{{"amount", amount}, {"address", address}}.dump();
}

void Input::Deserialize(const std::string &raw)
{
	json j = json::parse(raw);
	amount = j["amount"], address = j["address"];
}

std::string Output::Serialize() const
{
	return json{{"amount", amount}, {"address", address}}.dump();
}

void Output::Deserialize(const std::string &raw)
{
	json j = json::parse(raw);
	amount = j["amount"], address = j["address"];
}

std::string Transaction::ToString() const
{
	std::vector<json> inputs, outputs;

	for (auto &input: _inputs)
		inputs.push_back(json::parse(input.Serialize()));

	for (auto &output: _outputs)
		outputs.push_back(json::parse(output.Serialize()));

	return json{{"inputs", inputs}, {"outputs", outputs}}.dump();
}

std::string Transaction::ToHash() const
{
	return SHA256(ToString());
}

Transaction Transaction::Create(const std::string &from, const std::string &to, uint64_t amount)
{
	Transaction tx;
	tx.AddInput({amount, from, {}});
	tx.AddOutput({amount, to});
	return tx;
}

Transaction Transaction::CreateRandom()
{
	SecretKey key = SecretKey::New();
	ByteArray pubkey = key.GetPublicKey().ToHex();
	std::string address{pubkey.begin(), pubkey.end()};
	uint64_t amount = Random<uint64_t>(100, 5000);

	Transaction tx;
	tx.AddInput({amount, address, {}});
	tx.AddOutput({amount, address});
	return tx;
	
}

Transaction Transaction::FromString(const std::string &raw)
{
	Transaction tx;
	json j = json::parse(raw);

	for (auto &v: j["inputs"]) {
		Input input; input.Deserialize(v.dump());
		tx.AddInput(input);
	}

	for (auto &v: j["outputs"]) {
		Output output; output.Deserialize(v.dump());
		tx.AddOutput(output);
	}
	return tx;
}

}
