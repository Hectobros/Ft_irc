#include "User.hpp"

User::User()
	: _is_registered (0), _pwd_checked(0), _is_nick_set(0), _is_user_set(0), _is_away(0), \
		 _is_op(0),_nick(""), _user(""), _host(""), _realname(""), _pseudo(""),\
		 _awaymsg(""), _has_kill_msg(0)
{
}

User::User(User const & src)
{
	*this = src;
}

User &	User::operator=(User const & rhs)
{
	(void)rhs;
	return (*this);
}

User::~User()
{
}

void		User::set_socket(int socket)
{
	_sock = socket;
}

int			User::get_socket(){
	return _sock;
}

void		User::set_idx(int idx)
{
	_idx = idx;
}

int			User::get_idx()
{
	return (_idx);
}

void		User::store_raw_msg_in_client(std::string raw_msg)
{
	_msg_received.store_raw_msg(raw_msg);
}

void	User::set_registered()
{
	_is_registered = true;
}

void		User::set_pwd_checked(bool arg)
{
	_pwd_checked = arg;
}

void		User::set_is_nick_set()
{
	_is_nick_set = true;
}

void		User::set_is_user_set()
{
	_is_user_set = true;
}

void		User::set_is_away(bool arg)
{
	_is_away = arg;
}

Message		& User::get_msg_received()
{
	return (_msg_received);
}

int			User::get_pwd_checked()
{
	return (_pwd_checked);
}

bool		User::get_is_registered()
{
	return (_is_registered);
}

bool		User::get_is_nick_set()
{
	return (_is_nick_set);
}

bool		User::get_is_user_set()
{
	return (_is_user_set);
}

bool		User::get_is_away()
{
	return (_is_away);
}

const std::string	User::get_nick()	{
	return (_nick);
}

const std::string	User::get_user()	{
	return (_user);
}

const std::string	User::get_host()	{
	return (_host);
}

const std::string	User::get_awaymsg()	{
	return (_awaymsg);
}

std::vector<Channel *>	User::get_channel_lst()	{
	return (_lst_chan);
}

void	User::send_msg_to_user(std::string msg)
{
	send(_sock, msg.c_str(), msg.size(), 0);
}

void		User::set_nick(std::string nick)
{
	_nick = nick;
}

void		User::set_user(std::string user)
{
	_user = user;
}

void		User::set_pseudo(std::string pseudo)
{
	_pseudo = pseudo;
}

void		User::set_host(std::string host)
{
	_host = host;
}

void		User::set_realname(std::string realname)
{
	_realname = realname;
}

void		User::set_awaymsg(std::string awaymsg)
{
	_awaymsg = awaymsg;
}

bool    User::add_channel(Channel *channel)
{
	if (_lst_chan.size() != 0 && is_channel_in_user(channel->getName()) != _lst_chan.end())
		return (false);
	_lst_chan.push_back(channel);
	return (true);
}

bool	User::get_has_kill_msg()
{
	return (_has_kill_msg);
}


void	User::set_has_kill_msg(bool kill_msg)
{
	_has_kill_msg = kill_msg;
}

bool    User::remove_channel(Channel *channel)
{
	std::vector<Channel *>::iterator it;

	it = is_channel_in_user(channel->getName());
	if (it == _lst_chan.end())
		return (false);
	_lst_chan.erase(it);
	return (true);
}

void	User::set_user_ip(std::string user_ip)
{
	_user_ip = user_ip;
}

const std::string	User::get_user_ip()
{
	return (_user_ip);
}

std::vector<Channel *>::iterator    User::is_channel_in_user(std::string channel_name)
{
	for (std::vector<Channel *>::iterator it = _lst_chan.begin(); it != _lst_chan.end(); it++)	{
		if ((*it)->getName() == channel_name)
			return (it);
	}
	return (_lst_chan.end());
}

std::vector<User *>	User::find_all_related_users()
{
	std::vector<User *>	ret;

	ret.push_back(this);
	return ret;
	for (std::vector<Channel *>::iterator it1 = _lst_chan.begin(); it1 != _lst_chan.end(); ++it1)
	{
		std::vector<User *>::iterator	it2;
		std::vector<User *>	tmp = (*it1)->get_users();
		for (it2 = tmp.begin(); it2 != tmp.end(); ++it2)
		{
			std::vector<User *>::iterator it3 = std::find(ret.begin(), ret.end(), (*it2));
			if (it3 == ret.end())
				ret.push_back((*it2));
		}
	}
	return (ret);
}

void		User::set_is_op(bool arg)
{
	_is_op = arg;
}

bool		User::get_is_op()
{
	return _is_op;
}

void		User::set_oper_kill(std::string oper_kill)
{
	_oper_kill = oper_kill;
}

void		User::kill_user()
{
	send_msg_to_user("ERROR\r\n");
	send_msg_to_user(_oper_kill);
}
