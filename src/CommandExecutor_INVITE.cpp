#include "CommandExecutor.hpp"
#include "Server.hpp"

// Checker si deja invited (Ne fait rien pour l'instant)
// Checker lorsque le client join de bien supperimer son invite
void	CommandExecutor::invite()
{
	Channel 	*chan = NULL;
	User		*tar = NULL;
	std::string	rply;
	std::string thechan;
	std::string target;
	std::vector<std::string> params;
	std::map<std::string, std::string> arg;
	std::stringstream ss;

	params = _cmd_to_exec->get_param();
	arg["command"] = "INVITE";
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
	thechan = (params[1]);
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
	if (!chan->is_user_operator(_user_src) && chan->isModeOnChan(MODE_CHANNEL_I))
	{
		_server->process_reply(ERR_CHANOPRIVSNEEDED, _user_src->get_socket(), arg);
		return;
	} 
	target = params[0]; 
	arg["nick"] = target;
	if (chan->is_user_in_channel(target))
	{
		_server->process_reply(ERR_USERONCHANNEL, _user_src->get_socket(), arg);
		return;
	}
	ss << ":" << _user_src->get_nick() << "!~" << _user_src->get_user() << "@" << _user_src->get_user_ip() << " " <<  "INVITE " << target << " " << thechan << "\r\n";
	rply = ss.str();
	tar = *_server->find_user_by_nickname(target);
	tar->send_msg_to_user(rply);
	chan->add_to_invite(tar);
	_server->process_reply(RPL_INVITING, _user_src->get_socket(), arg);
}
