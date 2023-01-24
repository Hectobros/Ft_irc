#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::away()
{
	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = "AWAY";

	if (!_cmd_to_exec->get_param().size())
	{
		_user_src->set_is_away(0);
		_user_src->set_awaymsg("");
		_server->process_reply(RPL_UNAWAY, _user_src->get_socket(), arg_for_srv_reply);
	}
	if (_cmd_to_exec->get_param().size() > 0)
	{
		_user_src->set_is_away(1);
		_user_src->set_awaymsg(_cmd_to_exec->get_param()[0]);
		_server->process_reply(RPL_NOWAWAY, _user_src->get_socket(), arg_for_srv_reply);
	}
}
