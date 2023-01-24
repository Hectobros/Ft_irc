#ifndef COMMANDEXECUTOR_HPP
#define COMMANDEXECUTOR_HPP

#include <string>
#include <iostream>
#include <map>

#include "Command.hpp"
#include "ServerReplies.hpp"
#include "User.hpp"


class Server;
class CommandExecutor
{
private:
	CommandExecutor(const CommandExecutor & src);
	CommandExecutor &	operator=(CommandExecutor const & rhs);
	Server													*_server;
	User													*_user_src;
	Command													*_cmd_to_exec;
	std::map<std::string,void (CommandExecutor::*)(void)>	_cmdtable;

public:
	CommandExecutor();
	virtual ~CommandExecutor();

	Server	*get_server();
	void	set_server(Server *server);

	User	*get_user_src();
	void	set_user_src(User *user_src);

	Command	*get_cmd_to_exec();
	void	set_cmd_to_exec(Command *cmd_to_exec);

	int		exec_cmd();
	int		exec_cmd(User *user_src, Command *cmd_to_exec);

private:
	bool	are_params_set();

	void	pass();
	void	user();
	void	nick();
	void	ping();
	void	join();
	void	part();
	void	kick();
	void	privmsg();
	void	away();
	void	invite();
	void	topic();
	void	mode();
	void	oper();
	void	kill();
	void	quit();
	void	who();
	void	notice();

	bool						is_nick_valid(std::string nick);
	void						print_nick();
	std::vector<std::string>	_split_string(std::string s, char c);
	std::string					_format_cmd_message(std::string cmd, std::string params);
	void						_print_join(Channel 	*chan, std::map<std::string, std::string> arg);
	void						register_user();
	std::vector<std::string>	_zero_de_pute(std::vector<std::string> chans);
	void						_mega_part();
};

#endif
