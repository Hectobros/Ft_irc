#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::ping()
{
	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = "PING";

	if (_cmd_to_exec->get_param().size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_cmd_to_exec->get_param()[0].size() < 1)
	{
		_server->process_reply(ERR_NOORIGIN, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}

	std::stringstream	ss;
	ss << ":localhost PONG localhost :" << _cmd_to_exec->get_param()[0] << "\r\n";
	std::string	str = ss.str();
	_user_src->send_msg_to_user(str);
}
