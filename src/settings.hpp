#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace artha {

using nlohmann::json;

class Settings {
public:
	Settings();
	~Settings();

	template <class _Type>
	inline void Set(const std::string &key, const _Type &value)
	{
		_json[key] = value;
	}

	template <class _Type>
	inline _Type Get(const std::string &key, const _Type &fallback = {}) const
	{
		return _json.contains(key) ? _json[key].get<_Type>() : fallback;
	}


#define SETTING_GETTER(_type, _name, _default) \
	inline _type _name() { return Get<_type>(#_name, _default); }

#define SETTING_SETTER(_type, _name) \
	inline void Set##_name(const _type &value) { Set<_type>(#_name, value); }

#define SETTING(_type, _name, _default) \
	SETTING_GETTER(_type, _name, _default) \
	SETTING_SETTER(_type, _name)

public:
	void Load(const std::string &name = SETTINGS_FILENAME);
	void Save(const std::string &name = SETTINGS_FILENAME);

public:
	static constexpr auto SETTINGS_FILENAME = "settings.json";

	SETTING(std::string, RewardAddress, "");
	SETTING(unsigned short, Port, 5001);

protected:
	json _json;

};

}
