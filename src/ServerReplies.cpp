#include "ServerReplies.hpp"


ServerReplies::ServerReplies()
{
	reply_msg[RPL_WELCOME]				=	"<client> Welcome to the Internet Relay Network";
	reply_msg[RPL_YOURHOST]				=	"<client> Your host is localhost";
	reply_msg[RPL_CREATED]				=	"<client> This server was created today";
	// reply_msg[RPL_MYINFO]				=	"<servername> <version> <available user modes> <available channel modes>";
	reply_msg[RPL_MYINFO]				=	"";
	reply_msg[RPL_AWAY]					=	"<client> <nick> :<message>";
	reply_msg[RPL_UNAWAY]				=	"<client> :You are no longer marked as being away";
	reply_msg[RPL_NOWAWAY]				=	"<client> :You have been marked as being away";
	reply_msg[RPL_ENDOFWHO]				=	"<client> <mask> :End of WHO list";
	reply_msg[RPL_NOTOPIC]				=	"<client> <channel> :No topic is set";
	reply_msg[RPL_TOPIC]				=	"<client> <channel> :<topic>";
	reply_msg[RPL_TOPICWHOTIME]			=	"<client> <channel> <nick> <setat>";
	reply_msg[RPL_CHANNELMODEIS]		=	"<client> <channel> <modestring>";
	reply_msg[RPL_INVITING]				=	"<client> <nick> <channel>";
	reply_msg[RPL_NAMREPLY]				=	"<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}";
	reply_msg[RPL_KILLDONE]				=	"Kill by an oper because he doesn't like you";
	reply_msg[RPL_ENDOFNAMES]			=	"<client> <channel> :End of /NAMES list";
	reply_msg[RPL_ENDOFBANLIST]			=	"<client> <channel> :End of channel ban list";	
	reply_msg[RPL_YOUREOPER]			=	"<client> :You are now an IRC operator";
	reply_msg[ERR_NOSUCHNICK]			=	"<client> <nick> :No such nick/channel";
	reply_msg[ERR_NOSUCHCHANNEL]		=	"<client> <channel> :No such channel";
	reply_msg[ERR_CANNOTSENDTOCHAN]		=	"<client> <channel> :Cannot send to channel";
	reply_msg[ERR_TOOMANYCHANNELS]		=	"<client> <channel> :You have joined too many channels";
	reply_msg[ERR_TOOMANYTARGETS]		=	"<client> <target> :<error code> recipients. <abort message>";
	reply_msg[ERR_NOORIGIN]				=	"<client> :No origin specified";
	reply_msg[ERR_NORECIPIENT]			=	"<client> :No recipient given (<command>)";
	reply_msg[ERR_NOTEXTTOSEND]			=	"<client> :No text to send";
	reply_msg[ERR_UNKNOWNCOMMAND]		=	"<client> <command> :Unkown command";
	reply_msg[ERR_NONICKNAMEGIVEN]		=	"<client> :No nickname given";
	reply_msg[ERR_ERRONEUSNICKNAME]		=	"<client> <nick> :Erroneus nickname";
	reply_msg[ERR_NICKNAMEINUSE]		=	"<client> <nick> :Nickname is already in use";
	reply_msg[ERR_USERNOTINCHANNEL]		=	"<client> <nick> <channel> :They aren't on that channel";
	reply_msg[ERR_NOTONCHANNEL]			=	"<client> <channel> :You're not on that channel";
	reply_msg[ERR_USERONCHANNEL]		=	"<client> <nick> <channel> :is already on channel";
	reply_msg[ERR_NEEDMOREPARAMS]		=	"<client> <command> :Not enough parameters";
	reply_msg[ERR_ALREADYREGISTERED]	=	"<client> :You may not reregister";
	reply_msg[ERR_PASSWDMISMATCH]		=	"<client> :Password incorrect";
	reply_msg[ERR_CHANNELISFULL]		=	"<client> <channel> :Cannot join channel (+l)";
	reply_msg[ERR_UNKNOWNMODE]			=	"<client> <modechar> :is unknown mode char to me";
	reply_msg[ERR_INVITEONLYCHAN]		=	"<client> <channel> :Cannot join channel (+i)";
	reply_msg[ERR_BANNEDFROMCHAN]		=	"<client> <channel> :Cannot join channel (+b)";
	reply_msg[ERR_BADCHANNELKEY]		=	"<client> <channel> :Cannot join channel (+k)";
	reply_msg[ERR_NOPRIVILEGES]			=	 "<client> :Permission Denied- You're not an IRC operator";
	reply_msg[ERR_CHANOPRIVSNEEDED]		=	"<client> <channel> :You're not channel operator";
	reply_msg[ERR_NOOPERHOST]			=	"<client> :No O-lines for your host";
}

ServerReplies::ServerReplies(ServerReplies &rhs)
{
	*this = rhs;
};

ServerReplies &ServerReplies::operator=(ServerReplies &rhs)
{
	reply = rhs.reply;
	reply_msg = rhs.reply_msg;
	return *this;
};
ServerReplies::~ServerReplies()
{
	reply_msg.clear();
}

void	ServerReplies::select_reply(numeric_replies num_reply)
{
	reply = num_reply;
}

void			ServerReplies::_replace_arg(std::map<std::string, std::string> arg, std::string &str_reply)
{
	std::size_t	begin = 0;
	std::size_t	end = 0;
	std::string value;
	begin = str_reply.find_first_of('<', 0);
	end = str_reply.find_first_of('>', begin);
	while (begin != std::string::npos && end != std::string::npos)
	{
		std::string key(str_reply.begin() + begin + 1, str_reply.begin() + end);
		try {
			value = arg.at(key);
		}
		catch (const std::out_of_range& e){
			value = "42";
		}
		str_reply.replace(begin, end - begin + 1, value);
		begin = str_reply.find_first_of('<', 0);
		end = str_reply.find_first_of('>', begin);
	}	
}

// penser à rajouter le server, le channel, et le user
std::string		ServerReplies::str_reply(std::map<std::string, std::string> arg)
{
	std::string	str_reply = ":localhost ";

	/*if (reply < 400)
	{*/
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(3) << reply;
		std::string num_reply = ss.str();
		str_reply += num_reply +" "; /*+ " : ";
	} */
	str_reply += reply_msg[reply];
	_replace_arg(arg, str_reply);
	str_reply += "\r\n";
	return (str_reply);
}

void		ServerReplies::send_num_reply(int client_socket, std::string rply)
{
	send(client_socket, rply.c_str(), rply.size(), 0);
}
