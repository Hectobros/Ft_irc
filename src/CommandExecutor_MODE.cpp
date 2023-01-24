#include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::mode()
{
	std::string thechan;
	std::string temp;
	Channel 	*chan = NULL;
	std::vector<std::string> params;
	std::map<std::string, std::string> arg;
	int imod = 0;
	int nmod = 0;
	int mmod = 0;
	int tmod = 0;
	int pmod = 0;
	int argiterator = 2;
	std::string mode;
	std::string temparg;
	std::stringstream ss;
	size_t lspe = 0;
	int plus = 1;

	params = _cmd_to_exec->get_param();
	arg["command"] = "MODE";
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
	thechan = (params[0]);
	arg["channel"] = thechan;
	if (_server->find_user_by_nickname(thechan) != _server->get_lst_user().end()) // Permet d'eviter que quand irssi se connect il check le mode de l'user
		return;
	if (!_server->is_channel_existing(thechan))
	{
		_server->process_reply(ERR_NOSUCHCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	chan = *_server->find_channel(thechan);
	if (params.size() == 1)
	{
		std::string modestring = "+";
		if (chan->isModeOnChan(MODE_CHANNEL_I))
			modestring += "i";
		if (chan->isModeOnChan(MODE_CHANNEL_M))
			modestring += "m";
		if (chan->isModeOnChan(MODE_CHANNEL_N))
			modestring += "n";
		if (chan->isModeOnChan(MODE_CHANNEL_P))
			modestring += "p";
		if (chan->isModeOnChan(MODE_CHANNEL_T))
			modestring += "t";
		if (chan->isModeOnChan(MODE_CHANNEL_K))
			modestring += "k";
		if (chan->isModeOnChan(MODE_CHANNEL_L))
			modestring += "l";
		if (modestring == "+")
			arg["modestring"] = "";
		else
			arg["modestring"] = modestring;
		_server->process_reply(RPL_CHANNELMODEIS, _user_src->get_socket(), arg);
		return;
	}
	if (!chan->is_user_in_channel(_user_src))
	{
		_server->process_reply(ERR_NOTONCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	if (!chan->is_user_operator(_user_src))
	{
		_server->process_reply(ERR_CHANOPRIVSNEEDED, _user_src->get_socket(), arg);
		return;
	}
	mode = params[1];
	if (params.size() == 2 && mode == "b")
	{
		_server->process_reply(RPL_ENDOFBANLIST, _user_src->get_socket(), arg);
		return;
	}
	for (std::string::iterator it = mode.begin(); it != mode.end(); it++)
	{
		if (*it == '+')
			plus = 1;
		else if (*it == '-')
			plus = 0;
		else if (*it == 'k' && plus == 1)
		{
			try{
				temparg = params.at(argiterator);
				if(!chan->isModeOnChan(MODE_CHANNEL_K))
					chan->assignMode(MODE_CHANNEL_K);
				chan->setKeyword(temparg);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+k " << temparg << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
			}
			catch (std::out_of_range& e) {
			}
			argiterator++;
		}
		else if(*it == 'k' && plus == 0)
		{
			if (chan->isModeOnChan(MODE_CHANNEL_K))
			{
				temparg = "*";
				chan->setKeyword(temparg);
				chan->removeMode(MODE_CHANNEL_K);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-k " << temparg << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
			}
			argiterator++;
		}
		else if(*it == 'b' && plus == 1)
		{
			try {
				temparg = params.at(argiterator);
				if (!chan->is_user_banned(*_server->find_user_by_nickname(temparg)))
				{
					chan->ban_user(*_server->find_user_by_nickname(temparg));
					ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+b " << temparg << "\r\n";
					temp = ss.str();
					chan->sendToAllUsers(temp);
					ss.str(std::string());
				}
			}
			catch (std::out_of_range& e) {
			}
			argiterator++;
		}
		else if(*it == 'b' && plus == 0)
		{
			try {
				temparg = params.at(argiterator);
				chan->unban_user(*_server->find_user_by_nickname(temparg));
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-b " << temparg << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
			}
			catch (std::out_of_range& e) {
			}
			argiterator++;
		}
		else if(*it == 'l' && plus == 1)
		{
			try {
				temparg = params.at(argiterator);
				ss << temparg;
				ss >> lspe;
				ss.str(std::string());
				ss.clear();
				if(chan->nb_user() <= lspe)
				{
					if(!chan->isModeOnChan(MODE_CHANNEL_L))
						chan->assignMode(MODE_CHANNEL_L);
					chan->setClientLimit(lspe);
					ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+l " << temparg << "\r\n";
					temp = ss.str();
					chan->sendToAllUsers(temp);
					ss.str(std::string());
				}
			}
			catch (std::out_of_range& e) {
			}
			argiterator++;
		}
		else if(*it == 'l' && plus == 0)
		{
			if(chan->isModeOnChan(MODE_CHANNEL_L))
			{
				chan->removeMode(MODE_CHANNEL_L);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-l" << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
			}
		}
		else if(*it == 'o' && plus == 1)
		{
			try {
				temparg = params.at(argiterator);
				if (!chan->is_user_in_channel(temparg))
				{
					arg["nickname"] = temparg;
					_server->process_reply(ERR_NOSUCHNICK, _user_src->get_socket(), arg);
				}
				else if (!chan->is_user_operator(chan->get_User_By_Name(temparg)))
				{
					chan->add_op(chan->get_User_By_Name(temparg));
					ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+o " << temparg << "\r\n";
					temp = ss.str();
					chan->sendToAllUsers(temp);
					ss.str(std::string());
				}
			}
			catch (std::out_of_range& e) {
			}
			argiterator++;
		}
		else if(*it == 'o' && plus == 0)
		{
			try {
				temparg = params.at(argiterator);
				if (!chan->is_user_in_channel(temparg))
				{
					arg["nickname"] = temparg;
					_server->process_reply(ERR_NOSUCHNICK, _user_src->get_socket(), arg);
				}
				else if (chan->is_user_operator(chan->get_User_By_Name(temparg)))
				{
					chan->del_op(chan->get_User_By_Name(temparg));
					ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-o " << temparg << "\r\n";
					temp = ss.str();
					chan->sendToAllUsers(temp);
					ss.str(std::string());
				}
			}
			catch (std::out_of_range& e) {
			}
			argiterator++;
		}
		else if(*it == 'i' && plus == 1)
		{
			if (imod == 0)
			{
				if (!chan->isModeOnChan(MODE_CHANNEL_I))
					chan->assignMode(MODE_CHANNEL_I);
				chan->reset_invite();
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+i " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				imod = 1;
			}
		}
		else if(*it == 'i' && plus == 0)
		{
			if (imod == 0)
			{
				if (chan->isModeOnChan(MODE_CHANNEL_I))
					chan->removeMode(MODE_CHANNEL_I);
				chan->reset_invite();
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-i " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				imod = 1;
			}
		}
		else if(*it == 'm' && plus == 1)
		{
			if (mmod == 0)
			{
				if (!chan->isModeOnChan(MODE_CHANNEL_M))
					chan->assignMode(MODE_CHANNEL_M);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+m " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				mmod = 1;
			}
		}
		else if(*it == 'm' && plus == 0)
		{
			if (mmod == 0)
			{
				if (chan->isModeOnChan(MODE_CHANNEL_M))
					chan->removeMode(MODE_CHANNEL_M);
				chan->reset_invite();
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-m " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				mmod = 1;
			}
		}
		else if(*it == 'n' && plus == 1)
		{
			if (mmod == 0)
			{
				if (!chan->isModeOnChan(MODE_CHANNEL_N))
					chan->assignMode(MODE_CHANNEL_N);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+n " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				tmod = 1;
			}
		}
		else if(*it == 'n' && plus == 0)
		{
			if (nmod == 0)
			{
				if (chan->isModeOnChan(MODE_CHANNEL_N))
					chan->removeMode(MODE_CHANNEL_N);
				chan->reset_invite();
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-n " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				nmod = 1;
			}
		}
		else if(*it == 't' && plus == 1)
		{
			if (tmod == 0)
			{
				if (!chan->isModeOnChan(MODE_CHANNEL_T))
					chan->assignMode(MODE_CHANNEL_T);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+t " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				tmod = 1;
			}
		}
		else if(*it == 't' && plus == 0)
		{
			if (tmod == 0)
			{
				if (chan->isModeOnChan(MODE_CHANNEL_T))
					chan->removeMode(MODE_CHANNEL_T);
				chan->reset_invite();
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-t " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				tmod = 1;
			}
		}
		else if(*it == 'p' && plus == 1)
		{
			if (pmod == 0)
			{
				if (!chan->isModeOnChan(MODE_CHANNEL_P))
					chan->assignMode(MODE_CHANNEL_P);
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "+p " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				pmod = 1;
			}
		}
		else if(*it == 'p' && plus == 0)
		{
			if (pmod == 0)
			{
				if (chan->isModeOnChan(MODE_CHANNEL_P))
					chan->removeMode(MODE_CHANNEL_P);
				chan->reset_invite();
				ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() <<  "@" << _user_src->get_user_ip() << " " <<  "MODE " << chan->getName() << " " << "-p " << "\r\n";
				temp = ss.str();
				chan->sendToAllUsers(temp);
				ss.str(std::string());
				pmod = 1;
			}
		}
		else
		{
			arg["modechar"] = &(*it);
			_server->process_reply(ERR_UNKNOWNMODE, _user_src->get_socket(), arg);
		}
	}
}
