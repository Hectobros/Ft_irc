#ifndef SERVER_HPP
#define SERVER_HPP

#include "User.hpp"
#include "Message.hpp"
#include "ServerReplies.hpp"
#include "Command.hpp"
#include "Channel.hpp"
#include "CommandExecutor.hpp" 

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <cerrno>
#include <clocale>
#include <netdb.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <poll.h>
#include <stdlib.h>
#include <signal.h>
#include <exception>
#include <cctype>
#include <algorithm>
#include <netinet/in.h>
#include <arpa/inet.h>

// #define LOCAL_HOST "127.0.0.1"
#define LOCAL_HOST "10.11.11.8"
#define PROTOCOL SOCK_STREAM
#define IP_TYPE  AF_INET
#define PORT_DEFAULT "6667"
#define MAX_LEN_QUEUE	100
#define TIMEOUT			-1
#define BUFF_SIZE_MSG	512
#define RECV_FLAGS 		0
#define SERVER_IDX		1
#define PASSWD "admin"
#define ID "admin"
#define ADMIN_IP "127.0.0.1"

#define polfd_server (get_polfds() + 1)


typedef std::vector<struct pollfd>::iterator	pollfds_iterator;
typedef std::vector<User *>::iterator			users_iterator;
typedef std::vector<Channel *>::iterator		channel_iterator;

class CommandExecutor;
class Command;
class ServerReplies;

class Server
{
private:
	Server(Server const & src);
	struct addrinfo					*_myaddrinfo;
	const char *					_serv_name;
	int								_sock_serv;
	int								_one;
	const char						*_port;
	std::vector<struct pollfd>		_pollfds;
	std::vector<User *>				_lst_users;
	std::vector<Channel *>			_lst_chan;
	std::string						_server_prefix;
	ServerReplies					_serv_replies;
	std::string						_serv_pwd;
	std::string						_host;

	CommandExecutor					_cmd_executor;
	bool							_run;
	Server & operator=(Server const & rhs);
public:
	Server( void );
	Server(const char *port, const char *pwd);
	virtual ~Server();

	/*****launch_serv**********/
	void				launch_serv();
	void				init_serv_addrinfo();
	void				bind_serv();
	void				set_socket_serv_listen();
	void				add_socket();

	
	void				wait_for_client();

	int					get_socket();
	void				add_new_socket_to_polfds(int new_socket);
	void				accept_new_client_connection();
	void				add_new_client(int client_socket, std::string user_ip);
	void				remove_client(int idx);
	struct pollfd	  	*get_polfds();
	void				remove_polfds(pollfds_iterator & fd);
	void				receive_client_msg(pollfds_iterator & it, size_t idx);
	void				store_msg_in_user(int idx, char *msg);
	void				exec_cmd_to_client(pollfds_iterator it, size_t idx_client);
	void				remove_user(pollfds_iterator & it, size_t idx);
	std::vector<struct pollfd>		get_lst_pollfds();
	
	std::vector<User *>::iterator	find_user_by_nickname(std::string nickname);

	void	process_reply(numeric_replies num_reply, int client_socket, std::map<std::string, std::string> arg);
	void	exec_lst_cmd_on_client(User *user_src, Message &cmd_to_exec);
	std::string	get_pwd();
	std::vector<User *>	& get_lst_user();
	std::vector<Channel *>	& get_lst_chan();
	
	void	add_new_channel(std::string channel_name, std::string channel_topic, User *user);
	void	remove_channel(std::string channel_name);
	std::vector<Channel *>::iterator	find_channel(std::string channel_name);
	bool	is_channel_existing(std::string channel_name);

	void	add_user_to_channel(User *user, Channel *channel);
	void	remove_user_from_channel(User *user, Channel *channel);

	bool	get_run() const;
	void	init_stdin();

	void	erase_user_from_all_channel(std::vector<Channel *> channels, int idx);

};

#endif
