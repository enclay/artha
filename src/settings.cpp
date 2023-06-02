#include <settings.hpp>

#include <fstream>
#include <sstream>

namespace artha {

Settings::Settings()
{
	if (std::filesystem::exists(SETTINGS_FILENAME)) {
		Load();
	}
}

Settings::~Settings()
{
	Save();
}

void Settings::Load(const std::string &name)
{
	std::ifstream str(name);
	std::stringstream buffer; buffer << str.rdbuf();
	std::string raw = buffer.str();
	_json = raw.empty() ? json{} : json::parse(raw);
}

void Settings::Save(const std::string &name)
{
	std::ofstream str(name);
	str << _json.dump();
	str.close();
}

}
