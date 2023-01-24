#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::quit()
{
	std::stringstream	ss;
	ss << "QUIT	:";
	if (_cmd_to_exec->get_param().size())
		ss << _cmd_to_exec->get_param()[0];
	ss << "\r\n";
	_user_src->send_msg_to_user("ERROR\r\n");
	_user_src->send_msg_to_user(ss.str());
}