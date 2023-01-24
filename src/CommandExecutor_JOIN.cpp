#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::_mega_part()
{
	std::stringstream ss;
	std::string	rply;
	Channel *chan = NULL;
	std::vector<Channel *> chans = _user_src->get_channel_lst();

	for(std::vector<Channel *>::iterator it = chans.begin() ; it != chans.end(); it++)
	{
		chan = *it;
		ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "PART " << chan->getName() << "\r\n";
		rply = ss.str();
		chan->sendToAllUsers(rply);
		chan->remove_user(_user_src);
		_user_src->remove_channel(chan);
		if (chan->nb_user() == 0)
			_server->remove_channel(chan->getName());
		ss.str(std::string());
	}
}

void	CommandExecutor::join()
{
	Channel 	*chan = NULL;
	std::string	rply;
	std::string thechan;
	std::string thekey;
	std::vector<std::string> chans;
	std::vector<std::string> keychain;
	std::map<std::string, std::string> arg;
	arg["command"] = "JOIN";
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
	chans = _zero_de_pute(_split_string(_cmd_to_exec->get_param()[0], ','));
	if (chans[0].size() == 1 && chans[0][0] == '0')
	{
		_mega_part();
		chans.erase(chans.begin());
	}
	try
	{
		keychain = _split_string(_cmd_to_exec->get_param().at(1), ',');
	}
	catch (std::out_of_range& e){
		keychain.push_back("");
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
		try {
			thekey = keychain.at(x);
		}
		catch (std::out_of_range& e){
			thekey = "";
		}
		arg["channel"] = thechan;
		if (thechan.size() != 0)
		{
			if(thechan[0] != '#')
				_server->process_reply(ERR_NOSUCHCHANNEL, _user_src->get_socket(), arg);
			else
			{
				if (!_server->is_channel_existing(thechan))
				{
					_server->add_new_channel(thechan, "Classic topic I guess", _user_src);
					chan = *_server->find_channel(thechan);
					_server->add_user_to_channel(_user_src, chan);
					_print_join(chan, arg);
				}
				else
				{
					chan = *_server->find_channel(thechan);
					switch (chan->can_user_join(_user_src, thekey))
					{
						case 474:
							_server->process_reply(ERR_BANNEDFROMCHAN, _user_src->get_socket(), arg);
							break;
						case 473:
							_server->process_reply(ERR_INVITEONLYCHAN, _user_src->get_socket(), arg);
							break;
						case 471:
							_server->process_reply(ERR_CHANNELISFULL, _user_src->get_socket(), arg);
							break;
						case 475:
							_server->process_reply(ERR_BADCHANNELKEY, _user_src->get_socket(), arg);
							break;
						case 999:
							break;
						default:
							_server->add_user_to_channel(_user_src, chan);
							chan->delete_invite(_user_src);
							_print_join(chan, arg);
							break;
					}
				}
			}
		}
	}
		
}

void	CommandExecutor::_print_join(Channel *chan, std::map<std::string, std::string> arg)
{
	std::string str;
	std::stringstream ss;
	std::vector<User *>     users;
	users = chan->get_users();
	chan->assignMode(MODE_CHANNEL_T);
	chan->assignMode(MODE_CHANNEL_N);
	str = _format_cmd_message("JOIN", chan->getName());
	chan->sendToAllUsers(str);
	ss << ":localhost MODE " << chan->getName() << " +nt \r\n";
	str = ss.str();
	chan->sendToMyself(str, _user_src->get_socket());
	ss.str(std::string());
	if (chan->is_topic_on())
	{
		arg["topic"] = chan->getTopic();
		_server->process_reply(RPL_TOPIC, _user_src->get_socket(), arg);
	}
	else
		_server->process_reply(RPL_NOTOPIC, _user_src->get_socket(), arg);
	ss << ":localhost 353 " << _user_src->get_nick() << " = " <<  chan->getName() << " :";
	for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
	{
		if (chan->is_user_operator(*it))
			ss << "@";
		ss << (*it)->get_nick() << " ";
	}
	ss << "\r\n";
	str = ss.str();
	chan->sendToMyself(str, _user_src->get_socket());
	_server->process_reply(RPL_ENDOFNAMES, _user_src->get_socket(), arg);
}
