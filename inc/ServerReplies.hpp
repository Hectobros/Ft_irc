#ifndef SERVERREPLIES_HPP

#define SERVERREPLIES_HPP
#include <map>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include <iomanip>

enum	numeric_replies{
	RPL_WELCOME = 1,
	RPL_YOURHOST,
	RPL_CREATED,
	RPL_MYINFO,

	RPL_AWAY = 301,
	RPL_UNAWAY = 305,
	RPL_NOWAWAY,
	RPL_ENDOFWHO = 315,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME,

	RPL_INVITING = 341,
	RPL_NAMREPLY = 353,
	RPL_KILLDONE = 361,
	RPL_ENDOFNAMES = 366,
	RPL_ENDOFBANLIST = 368,
	RPL_YOUREOPER = 381,

	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN,
	ERR_TOOMANYCHANNELS,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL,
	ERR_USERONCHANNEL,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN,
	ERR_BADCHANNELKEY,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED ,
	ERR_NOOPERHOST = 491
};
class ServerReplies
{
	private:
		numeric_replies							reply;
		std::map<numeric_replies, std::string>	reply_msg;
		void		_replace_arg(std::map<std::string, std::string> arg, std::string &str_reply);

	public:
		ServerReplies();
		ServerReplies(ServerReplies &rhs);
		ServerReplies &operator=(ServerReplies &rhs);
		virtual ~ServerReplies();

		void		select_reply(numeric_replies num_reply);
		std::string	str_reply(std::map<std::string, std::string> arg);
		void		send_num_reply(int client_socket, std::string rply);
};




#endif
