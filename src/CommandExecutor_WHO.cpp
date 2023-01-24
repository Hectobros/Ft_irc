#include "CommandExecutor.hpp"
#include "Server.hpp"

void    CommandExecutor::who()
{
    std::map<std::string, std::string> arg;

    arg["command"] = "WHO";
	if (!_user_src->get_is_nick_set())
		arg["client"] = "*";
	else
		arg["client"] = _user_src->get_nick();
	if (!_user_src->get_is_registered())
	{
		//ERR user not registred
		return;
	}
    if (_cmd_to_exec->get_param().size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg);
		return ;
	}
    arg["mask"] = _cmd_to_exec->get_param()[0];
    _server->process_reply(RPL_ENDOFWHO, _user_src->get_socket(), arg);

}