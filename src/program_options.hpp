#pragma once

#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options.hpp>

#include <string>
#include <functional>
#include <iostream>
#include <map>
#include <list>


namespace po = boost::program_options;

namespace artha {

class OptionInitWrapper {
public:

	template <class T>
	OptionInitWrapper &Arg(const char *name) { return Arg<T>(name, name); }

	template <class T>
	OptionInitWrapper &Arg(const char *name, const char *fullname) {
		return _handle(name, po::value<T>(), fullname), *this;
	}

	OptionInitWrapper(po::options_description_easy_init &&d): _handle(d) {}

private:
	po::options_description_easy_init _handle;
};


class ProgramOptions {
public:
	ProgramOptions(int argc, char **argv): _argc(argc), _argv(argv), _desc("artha cryptocurrency") {

	}

	ProgramOptions On(const std::string &opt, std::function<void()> callback);
	void Process();

	template <class T>
	T Option(const std::string &name)
	{
		try {
			return _options[name].as<T>();
		}
		catch (...) {
			std::exit(-1);
		}
	}

	template <class T>
	T Require(const std::string &param)
	{
		if (!HasOption(param))
			OptionMissingError(param);

		return Option<T>(param);
	}
	

	OptionInitWrapper Builder();

	bool HasOption(const std::string &name);

protected:
	void Setup();
	void OptionMissingError(const std::string &name);

protected:
	std::map<std::string, std::function<void()>> _handlers;

	po::options_description _desc;
	po::positional_options_description _positional;
	po::variables_map _options;

	int _argc;
	char **_argv;
};

}
