#include "Command.hpp"

Command::Command():
	_rawcmd(NULL),
    _cmd(NULL)	{}

Command::Command(const Command & src):
	_rawcmd(src.get_rawcmd()),
	_prefix(src.get_prefix()),
	_cmd(src.get_cmd()),
	_param(src.get_param()),
	_channels(src.get_channels())	{}

Command &	Command::operator=(Command const & rhs)
{
	(void)rhs;
	return (*this);
}

Command::Command(const std::string	cmd):
     _prefix(""), _cmd("")
{
	_rawcmd = cmd;
	set_prefix(_rawcmd);
	set_cmd(&_rawcmd[_prefix.size()]);
	set_param(&_rawcmd[_prefix.size() + _cmd.size()]);
}

Command::~Command()	{
	// _prefix.clear();
	// _cmd.clear();
	
	// _channels.clear();
}

void	Command::set_rawcmd(std::string rawcmd)
{
		_rawcmd = rawcmd;
}

void	Command::clear()
{
	_prefix.clear();
	_cmd.clear();
	_rawcmd.clear();
	for (size_t i = 0; i < _param.size(); i++)
	{
		(_param[i]).clear();
	}
}

std::string const	Command::get_prefix() const
{
	return (_prefix);
}

void	Command::set_prefix(std::string raw_cmd)
{
	size_t	len_prefix;

	if (raw_cmd[0] != ':')
	{
		_prefix = "";
		return ;
	}
	len_prefix = raw_cmd.find(" ");
	if (len_prefix != std::string::npos)
		raw_cmd.resize(len_prefix);
	raw_cmd.erase(0, 1);
	_prefix = raw_cmd;
}

void	Command::set_cmd(std::string raw_cmd)
{
	size_t	len_cmd = 0;

	raw_cmd.erase(0, raw_cmd.find_first_not_of(' ', 0));
	len_cmd = raw_cmd.find(" ");
	if (len_cmd != std::string::npos)
		raw_cmd.resize(len_cmd);
	_cmd = raw_cmd;
}

std::string const	Command::get_cmd() const
{
	return (_cmd);
}

void	Command::set_param(std::string raw_cmd)
{
	raw_cmd.erase(0, raw_cmd.find_first_not_of(' ', 0));
	for (std::size_t i = 0; raw_cmd.size(); raw_cmd.erase(0, raw_cmd.find_first_not_of(' ', 0)))
	{
		if (raw_cmd[0] == ':')
		{
			_param.push_back(raw_cmd.substr(1, std::string::npos));
			break ;
		}
		i = raw_cmd.find_first_of(' ', 0);
		_param.push_back(raw_cmd.substr(0, i));
		if (raw_cmd[0] == '#')
			add_multiple_channels_to_channel_list(extract_multiple_chanel_from_single_arg(_param.back()));
		raw_cmd.erase(0, i);
	}
}

void	Command::print_param()
{
	for (std::size_t i = 0; i < _param.size(); i++)
	{
		std::cout << "_param[" << i << "] : " << _param[i] << std::endl;
	}
}

std::string const	Command::get_rawcmd() const
{
	return (_rawcmd);
}

std::vector<std::string> const		Command::get_param() const
{
	return (_param);
}

std::vector<std::string> const		Command::get_channels() const
{
	return (_channels);
}

std::vector<std::string>	Command::extract_multiple_chanel_from_single_arg(std::string arg)	{
	std::vector<std::string>	channels;

	for (std::size_t i = 0; arg.size(); arg.erase(0, arg.find_first_not_of(',', 0)))
	{
		i = arg.find_first_of(',', 0);
		channels.push_back(arg.substr(0, i));
		arg.erase(0, i);
	}
	return (channels);
}

void	Command::add_multiple_channels_to_channel_list(std::vector<std::string> multiple_channels)	{
	
	for (std::vector<std::string>::iterator it = multiple_channels.begin(); it != multiple_channels.end(); it++)	{
		_channels.push_back(*it);
	}
}
