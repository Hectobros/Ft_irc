#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::topic()
{
	Channel 	*chan = NULL;
	std::string	rply;
	std::string thechan;
	std::string target;
	std::vector<std::string> params;
	std::map<std::string, std::string> arg;
	std::stringstream ss;

	params = _cmd_to_exec->get_param();
	arg["command"] = "TOPIC";
	if (!_user_src->get_is_nick_set())
		arg["client"] = "*";
	else
		arg["client"] = _user_src->get_nick();
	if (!_user_src->get_is_registered())
	{
		return;
		//ERR user not registred
	}
	if (params.size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg);
		return;
	}
	
	thechan = (_split_string(params[0], ',')[0]); // tester si channels spearer par des virgules
	arg["channel"] = thechan;
	if (!_server->is_channel_existing(thechan))
	{
		_server->process_reply(ERR_NOSUCHCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	chan = *_server->find_channel(thechan);
	if (params.size() < 2)
	{
		if (chan->is_topic_on())
		{
			arg["topic"] = chan->getTopic();
			_server->process_reply(RPL_TOPIC, _user_src->get_socket(), arg);
		}
		else
		{
			_server->process_reply(RPL_NOTOPIC, _user_src->get_socket(), arg);
		}
		return;
	}
	if (!chan->is_user_in_channel(_user_src))
	{
		_server->process_reply(ERR_NOTONCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	if (!chan->is_user_operator(_user_src) && chan->isModeOnChan(MODE_CHANNEL_T))
	{
		_server->process_reply(ERR_CHANOPRIVSNEEDED, _user_src->get_socket(), arg);
		return;
	} // tester ce qui suit avec la , comme pour les channels
	target = params[1];
	arg["topic"] = target;
	chan->setTopic(target);
	ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "TOPIC " << thechan << " :" << target << "\r\n";
	rply = ss.str();
	chan->sendToAllUsers(rply);
	ss.str(std::string());
	return;
}