#include "CommandExecutor.hpp"
#include "Server.hpp"


void	CommandExecutor::privmsg()
{
	std::map<std::string, std::string>	arg_for_srv_reply;
	if (!_user_src->get_is_nick_set())
		arg_for_srv_reply["client"] = "*";
	else
		arg_for_srv_reply["client"] = _user_src->get_nick();
	arg_for_srv_reply["command"] = "PRIVMSG";

	if (_cmd_to_exec->get_param().size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_cmd_to_exec->get_param()[0].size() < 1)
	{
		_server->process_reply(ERR_NORECIPIENT, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}
	if (_cmd_to_exec->get_param().size() == 1 || _cmd_to_exec->get_param()[1].size() < 1)
	{
		_server->process_reply(ERR_NOTEXTTOSEND, _user_src->get_socket(), arg_for_srv_reply);
		return ;
	}

	std::vector<std::string>	targets = _split_string(_cmd_to_exec->get_param()[0], ',');
	User	*user_dest = NULL;
	User	*user_tmp = NULL;
	Channel	*chan_dest = NULL;
	bool	err = false;
	for (std::vector<std::string>::iterator it1 = targets.begin(); it1 != targets.end(); ++it1)
	{
		if ((*it1)[0] != '#')
		{
			user_tmp = NULL;
			for (std::vector<User *>::iterator it2 = _server->get_lst_user().begin(); it2 != _server->get_lst_user().end(); ++it2)
			{
				if (!(*it2)->get_nick().compare(*it1))
					user_tmp = (*it2);
			}
			if (!user_tmp)
			{
				arg_for_srv_reply["nick"] = (*it1);
				_server->process_reply(ERR_NOSUCHNICK, _user_src->get_socket(), arg_for_srv_reply);
				err = true;
			}
			else if (!user_dest && !chan_dest)
				user_dest = user_tmp;
		}
		else //doesn't take channel prefixes into account, if you use them it won't be recognized as a channel
		{
			if  (!_server->is_channel_existing(*it1))
			{
				arg_for_srv_reply["nick"] = (*it1);
				_server->process_reply(ERR_NOSUCHNICK, _user_src->get_socket(), arg_for_srv_reply);
				err = true;
			}
			else if (!user_dest && !chan_dest)
				chan_dest = *_server->find_channel(*it1);
		}
	}
	if (err == true)
		return ;
	if (targets.size() > 1)
	{
		arg_for_srv_reply["target"] = targets[1];
		arg_for_srv_reply["error code"] = "Too many";
		arg_for_srv_reply["abort message"] = "Only 1 processed";
		_server->process_reply(ERR_TOOMANYTARGETS, _user_src->get_socket(), arg_for_srv_reply);
	}
	if (user_dest && !chan_dest)
	{
		if (user_dest->get_is_away() == true)
		{
			arg_for_srv_reply["nick"] = user_dest->get_nick();
			arg_for_srv_reply["message"] = user_dest->get_awaymsg();
			_server->process_reply(RPL_AWAY, _user_src->get_socket(), arg_for_srv_reply);
		}
		std::stringstream	ss;
		ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() << "@" << _user_src->get_host() << " PRIVMSG " << user_dest->get_nick() << " :" << _cmd_to_exec->get_param()[1] << "\r\n"; //check for 'get_host' param
		std::string	str = ss.str();
		user_dest->send_msg_to_user(str);
	}
	else if (chan_dest && !user_dest)
	{
		if ((!chan_dest->is_user_in_channel(_user_src) && chan_dest->isModeOnChan(MODE_CHANNEL_N)) || (chan_dest->isModeOnChan(MODE_CHANNEL_M) && !chan_dest->is_user_operator(_user_src)) ||chan_dest->is_user_banned(_user_src))
		{
			arg_for_srv_reply["channel"] = chan_dest->getName();
			_server->process_reply(ERR_CANNOTSENDTOCHAN, _user_src->get_socket(), arg_for_srv_reply);
			return ;
		}
		
		std::stringstream	ss;
		ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() << "@" << _user_src->get_host() << " PRIVMSG " << chan_dest->getName() << " :" << _cmd_to_exec->get_param()[1] << "\r\n"; //check for 'get_host' param
		std::string	str = ss.str();
		chan_dest->sendToAllOtherNotBannedUsers(str, _user_src->get_socket());
	}
}
