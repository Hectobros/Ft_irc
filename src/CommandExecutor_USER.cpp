#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::user()
{
	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = "USER";

	if (!_user_src->get_pwd_checked())
		return ;
	if (_cmd_to_exec->get_param().size() < 4 || _cmd_to_exec->get_param()[0].size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_user_src->get_is_registered())
	{
		_server->process_reply(ERR_ALREADYREGISTERED, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	_user_src->set_user(_cmd_to_exec->get_param()[0]);
	_user_src->set_pseudo(_cmd_to_exec->get_param()[1]); //doc says this "SHOULD be sent as one zero ('0')"
	_user_src->set_host(_cmd_to_exec->get_param()[2]); //doc says this "SHOULD be sent as one asterisk ('*')" 
	_user_src->set_realname(_cmd_to_exec->get_param()[3]); //do we account for spaces? if so, is it already handled in Message/Command or should it be done here?
	_user_src->set_is_user_set();
	if (_user_src->get_is_nick_set())
		register_user();
}
