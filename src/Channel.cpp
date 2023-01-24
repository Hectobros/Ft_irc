#include "Channel.hpp"

Channel::Channel() : _name(NULL) {}

Channel::Channel(Channel const & src) : _name(NULL)
{
	(void)src;
}

Channel::Channel(std::string &topic) : _topic(topic), _keyword(""), _channelMode(0), _toggle_topic(0) {}

Channel::Channel(std::string channel_name, std::string channel_topic, User *user) : _name(channel_name), _topic(channel_topic), _client_limit(BASICNUMBER), _channelMode(0), _toggle_topic(0)
{
	_users.push_back(user);
	_operators.push_back(user);
}

Channel &	Channel::operator=(Channel const & rhs)
{
	(void)rhs;
	return (*this);
}

Channel::~Channel()
{
	_users.clear();
}

void Channel::setName(std::string &name)
{
	_name = name;
}

void Channel::setTopic(std::string &topic)
{
	_topic = topic;
	_toggle_topic = 1;
}

void Channel::setKeyword(std::string &keyword)
{
	_keyword = keyword;
}

std::string Channel::getName()
{
	return (_name);
}

std::string Channel::getTopic()
{
	return (_topic);
}

std::string Channel::getKeyword()
{
	return (_keyword);
}

std::vector<User *> Channel::get_users()
{
	return (_users);
}

std::vector<User *> Channel::get_operators()
{
	return (_operators);
}

bool Channel::add_user(User *user)
{
	if (_is_user_in_channel(user->get_nick()) != _users.end())
		return (false);
	_users.push_back(user);
	return (true);
}

bool Channel::remove_user(User *user)
{
	std::vector<User *>::iterator it;

	it = _is_user_in_channel(user->get_nick());
	if (it == _users.end())
		return (false);
	_users.erase(it);
	return (true);
}

void Channel::assignMode(channelMode mode)
{
	_channelMode |= mode;
}

void Channel::removeMode(channelMode mode)
{
	_channelMode ^= mode;
}

bool Channel::isModeOnChan(channelMode mode)
{
	if (_channelMode & mode)
		return (true);
	return (false);
}

void Channel::sendToAllUsers(std::string &msg)
{
	std::vector<User *>::iterator it = _users.begin();

	while (it != _users.end())
	{
		(*it)->send_msg_to_user(msg);
		it++;
	}
}

void Channel::sendToAllOtherUsers(std::string &msg, int client_socket)
{
	std::vector<User *>::iterator it = _users.begin();

	while (it != _users.end())
	{
		if ((*it)->get_socket() != client_socket)
			(*it)->send_msg_to_user(msg);
		it++;
	}
}

void Channel::sendToAllOtherNotBannedUsers(std::string &msg, int client_socket)
{
	std::vector<User *>::iterator it = _users.begin();

	while (it != _users.end())
	{
		if ((*it)->get_socket() != client_socket && !is_user_banned(*it))
			(*it)->send_msg_to_user(msg);
		it++;
	}
}

void Channel::sendToMyself(std::string &msg, int client_socket)
{
	std::vector<User *>::iterator it = _users.begin();

	while (it != _users.end())
	{
		if ((*it)->get_socket() == client_socket)
		{
			(*it)->send_msg_to_user(msg);
			break;
		}
		it++;
	}
}

size_t Channel::nb_user()
{
	return (_users.size());
}

std::vector<User *>::iterator Channel::_is_user_in_channel(std::string nickname)
{
	for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (!(*it)->get_nick().compare(nickname))
			return (it);
	}
	return (_users.end());
}

bool Channel::_is_user_invited(User *u)
{
	for (std::vector<User *>::iterator it = _invites.begin(); it != _invites.end(); it++)
	{
		if (*it == u)
			return (true);
	}
	return (false);
};

bool Channel::is_user_banned(User *u)
{
	for (std::vector<User *>::iterator it = _ban.begin(); it != _ban.end(); it++)
	{
		if (*it == u)
			return (true);
	}
	return (false);
};

bool Channel::is_user_in_channel(User *u)
{
	for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (*it == u)
			return (true);
	}
	return (false);
};

bool Channel::is_user_in_channel(std::string name)
{
	if (_is_user_in_channel(name) == _users.end())
		return false;
	return true;
};

bool Channel::is_user_operator(User *u)
{
	for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == u)
			return (true);
	}
	return (false);
};

int Channel::can_user_join(User *u, std::string key)
{
	if (is_user_banned(u))
		return (474);
	if (isModeOnChan(MODE_CHANNEL_I) && (!_is_user_invited(u)))
		return (473);
	if (isModeOnChan(MODE_CHANNEL_K) && key != _keyword)
		return (475);
	if (isModeOnChan(MODE_CHANNEL_L) && _client_limit == nb_user())
		return (471);
	if (is_user_in_channel(u))
		return (999);
	return (-1);
}

bool Channel::is_topic_on()
{
	if (_toggle_topic == 0)
		return false;
	else
		return true;
}
void Channel::d_print_all_users()
{
	int x = 0;
	for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); it++)
	{
		std::cout << "Dans la liste users du chan " << _name << " : User Num " << x << " nick: " << (*it)->get_nick() << std::endl;
		x++;
	}
};

User *Channel::get_User_By_Name(std::string name)
{
	return (*(_is_user_in_channel(name)));
};

void Channel::add_to_invite(User *u)
{
	if (!_is_user_invited(u))
		_invites.push_back(u);
	return;
}

void Channel::delete_invite(User *u)
{
	if (_is_user_invited(u))
	{
		for (std::vector<User *>::iterator it = _invites.begin(); it != _invites.end(); it++)
		{
			if (*it == u)
			{
				_invites.erase(it);
				break;
			}
		}
	}
}

void Channel::reset_invite()
{
	_invites.clear();
}

void Channel::ban_user(User *u)
{
	if (!u)
		return;
	if (!is_user_banned(u))
		_ban.push_back(u);
};

void Channel::unban_user(User *u)
{
	if (!u)
		return;
	if (is_user_banned(u))
	{
		for (std::vector<User *>::iterator it = _ban.begin(); it != _ban.end(); it++)
		{
			if (*it == u)
			{
				_ban.erase(it);
				break;
			}
		}
	}
}

void Channel::setClientLimit(size_t x)
{
	if (x >= nb_user())
		_client_limit = x;
}

void    Channel::add_op(User *u)
{
	if (!is_user_operator(u))
	{
		_operators.push_back(u);
	}
}

void	Channel::del_op(User *u)
{
	if (is_user_operator(u))
	{
		for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); it++)
		{
			if (*it == u)
			{
				_operators.erase(it);
				break;
			}
		}
	}
}
