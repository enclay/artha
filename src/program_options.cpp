#include <program_options.hpp>

namespace artha {

ProgramOptions ProgramOptions::On(const std::string &opt, std::function<void()> callback)
{
	_handlers[opt] = callback;
	return *this;
}

void ProgramOptions::Process()
{
	Setup();

	std::string opt = Option<std::string>("cmd");
	if (!_handlers.contains(opt)) {
		std::cout << "wrong command" << std::endl;
		return;
	}
	_handlers[opt]();
}

OptionInitWrapper ProgramOptions::Builder() {
	return OptionInitWrapper{_desc.add_options()}
		.Arg<std::string>("cmd", "command")
		.Arg<std::string>("value");
}

bool ProgramOptions::HasOption(const std::string &name)
{
	return static_cast<bool>(_options.count(name));
}

void ProgramOptions::OptionMissingError(const std::string &name)
{
	std::cout << "Required option " << name << " missing" << std::endl;
	std::exit(-1);
}

void ProgramOptions::Setup()
{
	_positional.add("cmd", 1);
	_positional.add("value", 1);

	try {
		po::store(po::command_line_parser(_argc, _argv).options(_desc).positional(_positional).run(), _options);
		boost::program_options::notify(_options);
	}
	catch (boost::program_options::error &e) {
		std::cout << "error: " << e.what() << std::endl;
		return;
	}
}

}

