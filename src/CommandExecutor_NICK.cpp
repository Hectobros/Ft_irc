#include "CommandExecutor.hpp"
#include "Server.hpp"

bool	CommandExecutor::is_nick_valid(std::string nick)
{
	if (nick.size() < 1
			|| nick.find_first_of(" ,*?!@") != std::string::npos
			|| nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '&')
		return (false);
	return (true);
}

void	CommandExecutor::nick()
{
	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = "NICK";

	if (!_user_src->get_pwd_checked())
		return ;
	if (_cmd_to_exec->get_param().size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_cmd_to_exec->get_param()[0].size() < 1)
	{
		_server->process_reply(ERR_NONICKNAMEGIVEN, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	arg_for_srv_reply["nick"] = _cmd_to_exec->get_param()[0];
	if (!is_nick_valid(_cmd_to_exec->get_param()[0]))
	{
		_server->process_reply(ERR_ERRONEUSNICKNAME, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (!_server->get_lst_user().empty())
	{
		for (std::vector<User *>::iterator it = _server->get_lst_user().begin(); it != _server->get_lst_user().end(); ++it)
		{
			if ((*it)->get_is_nick_set() && !(*it)->get_nick().compare(_cmd_to_exec->get_param()[0]))
			{
				_server->process_reply(ERR_NICKNAMEINUSE, _user_src->get_socket(), arg_for_srv_reply);
				return ;
			}
		}
	}
	if (!_user_src->get_is_registered())
	{
		_user_src->set_nick(_cmd_to_exec->get_param()[0]);
		_user_src->set_is_nick_set();
		if (_user_src->get_is_user_set())
			register_user();
	}
	else
	{
		print_nick();
		_user_src->set_nick(_cmd_to_exec->get_param()[0]);
		_user_src->set_is_nick_set();
	}
}
