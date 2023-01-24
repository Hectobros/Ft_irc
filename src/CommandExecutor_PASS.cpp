#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::pass()
{
	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = "PASS";

	if (_cmd_to_exec->get_param().size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_user_src->get_is_registered())
	{
		_server->process_reply(ERR_ALREADYREGISTERED, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_server->get_pwd().compare(_cmd_to_exec->get_param()[0]))
	{
		_user_src->set_pwd_checked(0);
		_server->process_reply(ERR_PASSWDMISMATCH, _user_src->get_socket(), arg_for_srv_reply);
	}
	else
		_user_src->set_pwd_checked(1);
}
