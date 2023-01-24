#include "CommandExecutor.hpp"
#include "Server.hpp"


/*******************   DEBUG   *************************/
void	printvector(std::vector<std::string> v)
{
	std::cout << "Printing vector :" << std::endl;
	int x = 0;
	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
	{
		std::cout << "Elem : " << x << " : " << *it << std::endl;
		x++;
	}
	std::cout << "Fin du vector" << std::endl;
}

void	CommandExecutor::print_nick()
{
	std::stringstream	ss;
	ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() << "@" << _user_src->get_host() << " NICK :" << _cmd_to_exec->get_param()[0] << "\r\n";
	std::string	str = ss.str();
	std::vector<User *>	related_users = _user_src->find_all_related_users();
	for (std::vector<User *>::iterator it = related_users.begin(); it != related_users.end(); ++it)
		(*it)->send_msg_to_user(str);
}

/**************************   DEBUG  *************************/


CommandExecutor::CommandExecutor():
	_server(NULL), _user_src(NULL), _cmd_to_exec(NULL)
{
	_cmdtable["PASS"] = &CommandExecutor::pass;
	_cmdtable["USER"] = &CommandExecutor::user;
	_cmdtable["NICK"] = &CommandExecutor::nick;
	_cmdtable["PING"] = &CommandExecutor::ping;
	_cmdtable["JOIN"] = &CommandExecutor::join;
	_cmdtable["PART"] = &CommandExecutor::part;
	_cmdtable["KICK"] = &CommandExecutor::kick;
	_cmdtable["PRIVMSG"] = &CommandExecutor::privmsg;
	_cmdtable["AWAY"] = &CommandExecutor::away;
	_cmdtable["INVITE"] = &CommandExecutor::invite;
	_cmdtable["MODE"] = &CommandExecutor::mode;
	_cmdtable["TOPIC"] = &CommandExecutor::topic;
	_cmdtable["KILL"] = &CommandExecutor::kill;
	_cmdtable["QUIT"] = &CommandExecutor::quit;
	_cmdtable["OPER"] = &CommandExecutor::oper;
	_cmdtable["WHO"] = &CommandExecutor::who;
	_cmdtable["NOTICE"] = &CommandExecutor::notice;
}

CommandExecutor::CommandExecutor(const CommandExecutor & src)
{
	(void)src;
}

CommandExecutor &	CommandExecutor::operator=(CommandExecutor const & rhs)
{
	(void)rhs;
	return (*this);
}

CommandExecutor::~CommandExecutor()	{}

Server	*CommandExecutor::get_server()
{
	return (_server);
}

void	CommandExecutor::set_server(Server *server = NULL)
{
	_server = server;
}

User	*CommandExecutor::get_user_src()
{
	return (_user_src);
}

void	CommandExecutor::set_user_src(User *user_src = NULL)
{
	_user_src = user_src;
}

Command	*CommandExecutor::get_cmd_to_exec()
{
	return (_cmd_to_exec);
}

void	CommandExecutor::set_cmd_to_exec(Command *cmd_to_exec = NULL)
{
	_cmd_to_exec = cmd_to_exec;
}

int	CommandExecutor::exec_cmd()
{
	if (!are_params_set())
		return (1);

	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = _cmd_to_exec->get_cmd();

	std::string	tmp_cmd = _cmd_to_exec->get_cmd();
    for (std::string::iterator it = tmp_cmd.begin(); it != tmp_cmd.end(); ++it)
    {
        if (islower(*it))
            *it = toupper(*it);
    }
	if (_cmdtable.find(tmp_cmd) == _cmdtable.end())
	{
		if (_user_src->get_is_registered())
			_server->process_reply(ERR_UNKNOWNCOMMAND, _user_src->get_socket(), arg_for_srv_reply);
		return (1);
	}
	else
	{
		_cmd_to_exec->set_cmd(tmp_cmd);
		(this->*_cmdtable[_cmd_to_exec->get_cmd()])();
	}
	return (0);
}

int	CommandExecutor::exec_cmd(User *user_src, Command *cmd_to_exec)
{
	set_user_src(user_src);
	set_cmd_to_exec(cmd_to_exec);
	if (!are_params_set())
		return (1);
	return (exec_cmd());
}

bool	CommandExecutor::are_params_set()
{
	if (!get_server() || !get_user_src() || !get_cmd_to_exec())
		return (false);
	return (true);
}


std::vector<std::string> CommandExecutor::_split_string(std::string s, char c)
{
    size_t obj;
    size_t ocj;
    std::vector<std::string> results;

    if(s == "")
        return results;
    ocj = 0;
    for(obj = s.find_first_of(c); obj != std::string::npos; obj = s.find_first_of(c, ocj))
    {
        results.push_back(s.substr(ocj, obj - ocj));
        ocj = obj + 1;
    }
    results.push_back(s.substr(ocj, s.size() -ocj));
    return (results);
};

std::string		CommandExecutor::_format_cmd_message(std::string cmd, std::string params)
{
	std::stringstream ss;
	std::string str;

	ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " << cmd << " :" << params << "\r\n";
	str = ss.str();
	return (str);
};

void	CommandExecutor::register_user()
{
	std::map<std::string, std::string>	arg_for_srv_reply; //need to fill according to "RPL_MYINFO"
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();

	_server->process_reply(RPL_WELCOME, _user_src->get_socket(), arg_for_srv_reply);
	_server->process_reply(RPL_YOURHOST, _user_src->get_socket(), arg_for_srv_reply);
	_server->process_reply(RPL_CREATED, _user_src->get_socket(), arg_for_srv_reply);
	_server->process_reply(RPL_MYINFO, _user_src->get_socket(), arg_for_srv_reply);
	_user_src->set_registered();
}

std::vector<std::string>	CommandExecutor::_zero_de_pute(std::vector<std::string> chans) //changer nom
{
	for(std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		if((*it).size() == 1 && (*it)[0] == '0')
		{
			chans.erase(chans.begin(),it);
			return(chans);
		}
	}
	return chans;
}
