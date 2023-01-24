#include "CommandExecutor.hpp"
#include "Server.hpp"

void    CommandExecutor::oper()
{
	std::string pass;
	std::string id;
	std::string ip;
	std::vector<std::string> params;
	std::map<std::string, std::string> arg;
	std::stringstream ss;

	params = _cmd_to_exec->get_param();
	arg["command"] = "KICK";
	if (!_user_src->get_is_nick_set())
		arg["client"] = "*";
	else
		arg["client"] = _user_src->get_nick();
	if (!_user_src->get_is_registered())
	{
		return;
		//ERR user not registred
	}
    if (params.size() < 2)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg);
		return;
	}
    id = params[0];
    pass = params[1];
    if (!(id == ID && pass == PASSWD))
    {
        _server->process_reply(ERR_PASSWDMISMATCH, _user_src->get_socket(), arg);
		return;
    }
    if (!(_user_src->get_user_ip() == LOCAL_HOST))
    {
        _server->process_reply(ERR_NOOPERHOST, _user_src->get_socket(), arg);
		return;
    }
   _user_src->set_is_op(true);
   _server->process_reply(RPL_YOUREOPER, _user_src->get_socket(), arg);
}