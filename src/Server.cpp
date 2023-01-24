#include "Server.hpp"

Server::Server() : _serv_name("myircserv"), _one(1), _port(PORT_DEFAULT)
{

}

Server::Server(Server const & src) : _serv_name("myircserv"), _one(1), _port(PORT_DEFAULT)
{
	(void)_serv_name;
	(void)src;
}

Server &	Server::operator=(Server const & rhs)
{
	(void)rhs;
	return (*this);
}

Server::Server(const char *port, const char *pwd) :
	_serv_name("myircserv"), _one(1), _port(port), _run(1)
{
	std::string	str_port = _port;
	std::exception e;

	_serv_pwd = pwd;
	if (str_port.find_first_not_of("0123456789") != std::string::npos)
		throw e;
	_server_prefix = ":localhost";
	_cmd_executor.set_server(this);
}


Server::~Server()
{
	for (users_iterator it = _lst_users.begin(); it!= _lst_users.end(); it++)
		delete * it;
	for (channel_iterator it = _lst_chan.begin(); it!= _lst_chan.end(); it++)
		delete * it;
	freeaddrinfo(_myaddrinfo);
	close(_sock_serv);
}

bool	Server::get_run() const
{
	return (_run);
}

int		Server::get_socket()
{
	return (_sock_serv);
}

void	Server::exec_cmd_to_client(pollfds_iterator it, size_t idx_client)
{
	if (_lst_users.size() < idx_client)
		return;
	if (!_lst_users[idx_client]->get_msg_received().get_is_cmd_complete())
		return ;
	if ( it->revents & POLLOUT)
	{
		exec_lst_cmd_on_client(_lst_users[idx_client], (_lst_users[idx_client])->get_msg_received());
		it->revents = 0;
	}
}

void	Server::receive_client_msg(pollfds_iterator & it, size_t idx)
{
	char	buff[BUFF_SIZE_MSG] = "";

	if (!recv(it->fd, buff, BUFF_SIZE_MSG, MSG_DONTWAIT))
	{
		remove_user(it, idx);
	}
	else
		store_msg_in_user(idx, buff);
	it->revents = 0;
}

/**********************Client handling***************************************************/

void	Server::erase_user_from_all_channel(std::vector<Channel *> channels, int idx)
{
	for(channel_iterator it = channels.begin(); it != channels.end(); it++)
	{
		(*it)->unban_user(_lst_users[idx]);
		(*it)->delete_invite(_lst_users[idx]);
		(*it)->remove_user(_lst_users[idx]);
		if (!(*it)->nb_user())
			remove_channel((*it)->getName());
	}
}

void	Server::store_msg_in_user(int idx, char *msg)
{
	_lst_users[idx]->store_raw_msg_in_client(msg);
}

void	Server::remove_client(int idx)
{
	users_iterator	user_to_erase = _lst_users.begin() + idx;

	erase_user_from_all_channel(_lst_users[idx]->get_channel_lst(), idx);
	for(users_iterator it = user_to_erase; it != _lst_users.end(); it++)
		(*it)->set_idx((*it)->get_idx() - 1);
	delete *user_to_erase;
	_lst_users.erase(user_to_erase);
}

void	Server::add_new_socket_to_polfds(int new_socket)
{
	struct pollfd	new_struct;

	new_struct.fd = new_socket;
	new_struct.events = POLLIN | POLLOUT;
	new_struct.revents = 0;
	_pollfds.push_back(new_struct);
}

void				Server::remove_polfds(pollfds_iterator & fd)
{
	close(fd->fd);
	_pollfds.erase(fd--);
}


void	Server::add_new_client(int client_socket, std::string user_ip)
{
	User	*new_user = new User;

	new_user->set_socket(client_socket);
	new_user->set_idx(_pollfds.size() - 1);
	new_user->set_host(_host);
	new_user->set_user_ip(user_ip);
	_lst_users.push_back(new_user);
}

struct pollfd		*Server::get_polfds()
{
	return &_pollfds.front();
}

void				Server::remove_user(pollfds_iterator & it, size_t idx)
{
	remove_polfds(it);
	remove_client(idx);
}

/******************wait_for_client**************************/

void	Server::wait_for_client()
{
	nfds_t				nb_fds = _pollfds.size();
	size_t				idx_client = 0;
	char	buff[BUFF_SIZE_MSG] = "";
	std::exception e;

	if (poll(get_polfds(), nb_fds, TIMEOUT) < 0)
		throw e;
	if (_pollfds[0].revents & POLLIN)
	{
		if (!(read(_pollfds[0].fd, buff, BUFF_SIZE_MSG)))
			_run = false;
		return;
	}
	if (polfd_server->revents & POLLIN)
	{
		accept_new_client_connection();
		polfd_server->revents = 0;
	}
	for (pollfds_iterator it = _pollfds.begin() + 2; it != _pollfds.end(); it++)
	{
		if (_lst_users[idx_client]->get_has_kill_msg())
			remove_user(it, idx_client);
		if (it->revents & POLLIN)
			receive_client_msg(it, idx_client);
		if (it->revents & POLLOUT)
			exec_cmd_to_client(it, idx_client);
		idx_client++;
	}
}

void	Server::accept_new_client_connection()
{
	struct sockaddr	addr;
	socklen_t		addrlen = sizeof(addr);
	int				client_socket;
	struct sockaddr_in *a;
	std::exception	e;

	if ((client_socket = accept(_sock_serv, &addr, &addrlen)) < 0)
		throw e;
	a = (struct sockaddr_in *)&addr;
	add_new_socket_to_polfds(client_socket);
	add_new_client(client_socket, inet_ntoa((struct in_addr)a->sin_addr));
}

std::vector<struct pollfd>		Server::get_lst_pollfds()
{
	return (_pollfds);
}

/*******************************launch_serv***************************/

void				Server::launch_serv()
{
	init_serv_addrinfo();
	init_stdin();
	add_socket();
	bind_serv();
	set_socket_serv_listen();
}

void	Server::init_stdin()
{
	struct pollfd	cinpoll;

	cinpoll.fd = 0;
	cinpoll.events = POLLIN;
	cinpoll.revents = 0;
	_pollfds.push_back(cinpoll);
}

void	Server::init_serv_addrinfo()
{
	std::exception e;
	struct sockaddr_in *addr;

	if (getaddrinfo(LOCAL_HOST, _port, 0, &_myaddrinfo)< 0)
		throw e;
    addr = (struct sockaddr_in *)_myaddrinfo->ai_addr;
	_host = inet_ntoa((struct in_addr)addr->sin_addr);
}

void	Server::add_socket()
{
	struct pollfd	serv;
	std::exception	e;

	if ((_sock_serv = socket(IP_TYPE, PROTOCOL, 0)) < 0)
		throw e;
	setsockopt(_sock_serv, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR , &_one, sizeof(int));
	serv.fd = _sock_serv;
	serv.events = POLLIN;
	serv.revents = 0;
	_pollfds.push_back(serv);
}
void	Server::bind_serv()
{
	std::exception	e;

	if (bind(_sock_serv, _myaddrinfo->ai_addr, _myaddrinfo->ai_addrlen) < 0)
		throw e;
}

void	Server::set_socket_serv_listen()
{
	std::exception	e;

	if (listen(_sock_serv, MAX_LEN_QUEUE) < 0)
		throw e;
}


std::vector<User *>::iterator	Server::find_user_by_nickname(std::string nickname)	{
	for (std::vector<User *>::iterator it = _lst_users.begin(); it != _lst_users.end(); it++)	{
		if ((*it)->get_nick() == nickname)
			return (it);
	}
	return (_lst_users.end());
}

void	Server::exec_lst_cmd_on_client(User *user_src, Message &cmd_to_exec)
{
	command_lst_iterator it = cmd_to_exec.get_cmd_lst().begin();

	for (;it != cmd_to_exec.get_cmd_lst().end(); it++)
	{
		_cmd_executor.exec_cmd(user_src, &(*it));
	}
	cmd_to_exec.clear_msg();
}

void	Server::process_reply(numeric_replies num_reply, int client_socket, std::map<std::string, std::string> arg)
{
	std::string					reply;

	_serv_replies.select_reply(num_reply);
	reply = _serv_replies.str_reply(arg);
	send(client_socket, reply.c_str(), reply.size(), 0);
}

std::string	Server::get_pwd()
{
	return   (_serv_pwd);
}

std::vector<User *>	& Server::get_lst_user()
{
	return (_lst_users);
}

std::vector<Channel *>	& Server::get_lst_chan()
{
	return (_lst_chan);
}

/********************Channel handling *******************/

void	Server::add_new_channel(std::string channel_name, std::string channel_topic, User *user)
{
	Channel *newChan = new Channel(channel_name, channel_topic, user);
	_lst_chan.push_back(newChan);
}

void	Server::remove_channel(std::string channel_name)	{
	std::vector<Channel *>::iterator it = find_channel(channel_name);

	delete *it;
	_lst_chan.erase(it);
}

std::vector<Channel *>::iterator	Server::find_channel(std::string channel_name)	{
	std::vector<Channel *>::iterator	it = _lst_chan.begin();

	while (it != _lst_chan.end())
	{
		if (!(*it)->getName().compare(channel_name))
			return (it);
		it++;
	}
	return (_lst_chan.end());
}

bool	Server::is_channel_existing(std::string channel_name)	{
	return (find_channel(channel_name) != _lst_chan.end() ? true : false);
}

void	Server::add_user_to_channel(User *user, Channel *channel)	{
	user->add_channel(channel);
	channel->add_user(user);
}

void	Server::remove_user_from_channel(User *user, Channel *channel)	{
	user->remove_channel(channel);
	channel->remove_user(user);
}
