#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::kill()
{
	std::map<std::string, std::string> arg;
	std::vector<User *>::iterator	user;

	if (!_user_src->get_is_nick_set())
		arg["client"] = "*";
	else
		arg["client"] = _user_src->get_nick();
	arg["command"] = "KILL";
	if (_cmd_to_exec->get_param().size() != 2)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg);
		return;
	}
	arg["nick"] = _cmd_to_exec->get_param()[0];
	if (!_user_src->get_is_op())
	{
		_server->process_reply(ERR_NOPRIVILEGES, _user_src->get_socket(), arg);
		return;
	}
	user = _server->find_user_by_nickname(_cmd_to_exec->get_param()[0]);
	if (user == _server->get_lst_user().end())
	{
		_server->process_reply(ERR_NOSUCHNICK, _user_src->get_socket(), arg);
		return;
	}
	if (user != _server->get_lst_user().end())
	{
		std::stringstream	ss;
		ss << _user_src->get_nick() << " killed ";
		if (_cmd_to_exec->get_param().size())
			ss << _cmd_to_exec->get_param()[0] << " :"<< _cmd_to_exec->get_param()[1];
		ss << "\r\n";
		(*user)->set_has_kill_msg(1);
		(*user)->send_msg_to_user("ERROR\r\n");
		(*user)->send_msg_to_user(ss.str());
	}
}
