#include "Message.hpp"

Message::Message():
    _raw_msg(""), _is_msg_complete(0)
{
}

Message::Message(Message const & src): _raw_msg(""), _is_msg_complete(0)
{
	(void)src;
}

Message &	Message::operator=(Message const & rhs)
{
	(void)rhs;
	return (*this);
}

Message::~Message()
{
	_raw_msg.clear();
	_command_lst.clear();
}

void 	Message::store_raw_msg(std::string msg)
{	
	std::string::const_iterator	it;
	std::exception				e;

	_raw_msg.append(msg);
	if (_raw_msg.rfind(END_MSG, std::string::npos) == _raw_msg.size() - 2)
	{
		parse_msg();
		_is_msg_complete = 1;
	}
	else if (_raw_msg.size() > 512)
	{
		_raw_msg.clear();
		throw e;
	}
	
}

void		Message::clear_msg()
{
	_is_msg_complete = 0;
	_raw_msg.clear();
	_command_lst.clear();
}

std::string Message::get_one_cmd(std::string raw_str)
{
	size_t	idx;

	idx = raw_str.find(END_MSG);
	raw_str.resize(idx);
	return (raw_str);
}

void	Message::parse_msg()
{
	std::string tmp_raw_msg(_raw_msg);

	for (std::size_t i = 0; tmp_raw_msg.size() > 0; tmp_raw_msg.erase(0, i + 2))
	{
		_command_lst.push_back(Command(get_one_cmd(tmp_raw_msg)));
		i = tmp_raw_msg.find(END_MSG);
	}
}

std::string		Message::get_raw_msg()
{
	return _raw_msg;
}

std::vector<Command>	&Message::get_cmd_lst()
{
	return (_command_lst);
}

bool					Message::get_is_cmd_complete()
{
	return (_is_msg_complete);
}
