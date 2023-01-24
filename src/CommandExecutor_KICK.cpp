 #include "CommandExecutor.hpp"
#include "Server.hpp"

void	CommandExecutor::kick()
{
	Channel 	*chan = NULL;
	std::string	rply;
	std::string thechan;
	std::string target;
	std::string after;
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
	thechan = (_cmd_to_exec->get_channels())[0];
	arg["channel"] = thechan;
	if (!_server->is_channel_existing(thechan))
	{
		_server->process_reply(ERR_NOSUCHCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	chan = *_server->find_channel(thechan);
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
	target = _split_string(params[1], ',')[0]; //Recupere mon premier arguement (En theorie ma liste d'user a ban separer par une ,) et recuere le premier (La seule personne que l'on ban)
	if (!chan->is_user_in_channel(target))
	{
		arg["nick"] = target;
		_server->process_reply(ERR_USERNOTINCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	try
	{
		after = params[2];
		if (after == "")
			after = "On l'a kick car c'est pas gentil d'être méchant";
	}
	catch (std::out_of_range& e)
	{
		after = "On l'a kick car c'est pas gentil d'être méchant";
	}
	ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() << "@" << _user_src->get_user_ip() << " " <<  "KICK " << thechan << " " << target << " :" << after << "\r\n";
	rply = ss.str();
	chan->sendToAllUsers(rply);
	_user_src->remove_channel(chan);
	chan->remove_user(chan->get_User_By_Name(target));
	if (chan->nb_user() == 0)
		_server->remove_channel(thechan);
	ss.str(std::string());
	return;
}
