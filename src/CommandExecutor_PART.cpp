#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::part()
{
	Channel 	*chan = NULL;
	std::string	rply;
	std::string thechan;
	std::string postfix;
	std::vector<std::string> chans;
	std::map<std::string, std::string> arg;
	std::stringstream ss;
	arg["command"] = "PART";
	if (!_user_src->get_is_nick_set())
		arg["client"] = "*";
	else
		arg["client"] = _user_src->get_nick();
	if (!_user_src->get_is_registered())
		return;
	if (_cmd_to_exec->get_param().size() < 1)
	{
		_server->process_reply(ERR_NEEDMOREPARAMS, _user_src->get_socket(), arg);
		return ;
	}
	chans = _split_string(_cmd_to_exec->get_param()[0], ',');
	try
	{
		postfix = _cmd_to_exec->get_param().at(1);
	}
	catch (std::out_of_range& e){
		postfix = "";
	}
	for (size_t x = 0; x < chans.size(); x++)
	{
		try
		{
			thechan = chans.at(x);
		}
		catch (std::out_of_range& e){
			thechan = "";
		}
		if (thechan.size() != 0)
		{
			arg["channel"] = thechan;
			if(!_server->is_channel_existing(thechan))
				_server->process_reply(ERR_NOSUCHCHANNEL, _user_src->get_socket(), arg);
			else
			{
				chan = *_server->find_channel(thechan);
				if (!chan->is_user_in_channel(_user_src))
					_server->process_reply(ERR_NOTONCHANNEL, _user_src->get_socket(), arg);
				else
				{
					ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "PART " << chan->getName();
					if (postfix != "")
						ss <<" :" << postfix;
					ss << "\r\n";
					rply = ss.str();
					chan->sendToAllUsers(rply);
					_user_src->remove_channel(chan);
					chan->remove_user(_user_src);
					if (chan->nb_user() == 0)
						_server->remove_channel(thechan);
					ss.str(std::string());
				}
			}
		}
	}
}
