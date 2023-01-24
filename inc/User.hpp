#ifndef USER_HPP

#define USER_HPP

#include "Message.hpp"
#include "ServerReplies.hpp"
#include <Channel.hpp>

#include <string>
#include <algorithm>

class Channel;

class User
{
private:
	User & operator=(User const & rhs);
	bool			_is_registered;
	bool			_pwd_checked;
	bool			_is_nick_set;
	bool			_is_user_set;
	bool			_is_away;
	bool			_is_op;
	std::string		_nick;
	std::string		_user;
	std::string		_host;
	std::string		_realname;
	std::string		_pseudo;
	std::string		_awaymsg;
	int				_sock;
	int				_idx;
	Message			_msg_received;
	std::string		_user_ip;
	std::vector<Channel *>	_lst_chan;
	bool			_has_kill_msg;
	std::string		_oper_kill;
public:
	User( void );
	User(User const & src);
	virtual ~User();

	void		set_socket(int socket);
	int			get_socket();

	void		store_raw_msg_in_client(std::string raw_msg);

	void		set_idx(int idx);
	int			get_idx();

	void		set_registered();
	bool		get_is_registered();

	void		set_pwd_checked(bool arg);
	int			get_pwd_checked();

	void		set_is_nick_set();
	bool		get_is_nick_set();

	void		set_is_user_set();
	bool		get_is_user_set();

	void		set_is_away(bool arg);
	bool		get_is_away();

	void		set_is_op(bool arg);
	bool		get_is_op();
	const std::string	get_nick();
	const std::string	get_user();
	const std::string	get_host();
	const std::string	get_awaymsg();
	const std::string	get_user_ip();
	std::vector<Channel *>	get_channel_lst();
	bool					get_has_kill_msg();
	void					set_oper_kill(std::string oper_kill);

	Message		&get_msg_received();
	void		send_msg_to_user(std::string msg);
	void		set_nick(std::string nick);
	void		set_user(std::string user);
	void		set_pseudo(std::string pseudo);
	void		set_host(std::string host);
	void		set_realname(std::string realname);
	void		set_awaymsg(std::string awaymsg);
	void		set_user_ip(std::string user_ip);
	void		set_has_kill_msg(bool kill_msg);
	void		kill_user();

	bool		add_channel(Channel *chan);
	bool		remove_channel(Channel *chan);

	std::vector<Channel *>::iterator    is_channel_in_user(std::string channel_name);
	std::vector<User *>					find_all_related_users();
};

#endif
