#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <sstream>
#define BASICNUMBER 50
/*
Pour rejoindre,
If user->is_registered
    If le chqnel existe
        if le chanel est visible (mode)
            on rentre dedans
            Tout les utilisateurs presents dans le channel recoivent un PRIVNSG qui dit que le mec a rejoint souf au mec qui a rejoint
        else (mode)
            ???
    else
        creer le chanel
    Envoyer: ":<NICKNAME> JOIN <CHANNEL_NAME>\n"
else
    quitter
*/

class User;

enum channelMode {

	NO_CHANNEL_MODE = 0,
	MODE_CHANNEL_I = (1 << 0),
	MODE_CHANNEL_M = (1 << 1),
	MODE_CHANNEL_N = (1 << 2),
	MODE_CHANNEL_P = (1 << 3),
	MODE_CHANNEL_T = (1 << 4),
	MODE_CHANNEL_K = (1 << 5),
	MODE_CHANNEL_L = (1 << 6)
};

class Channel   {
    private:
        Channel(Channel const & src);
		Channel & operator=(Channel const & rhs);
        std::string             _name;
        std::string             _topic;
        std::string             _keyword;
        std::vector<User *>     _users;
        std::vector<User *>     _operators;
        std::vector<User *>     _invites;
        std::vector<User *>     _ban;
        size_t                  _client_limit;

        char                    _channelMode;
        int                     _toggle_topic;
    public:
        Channel();
        Channel(std::string &topic);
        Channel(std::string channel_name, std::string channel_topic, User *user);
        ~Channel();

        void    setName(std::string &name);
        void    setTopic(std::string &topic);
        void    setKeyword(std::string &keyword);
		void	setClientLimit(size_t	x);

        std::string			getName();
        std::string 		getTopic();        
        std::string			getKeyword();
        User *              get_User_By_Name(std::string name);
		std::vector<User *>	get_users();
		std::vector<User *>	get_operators();

        bool    add_user(User *user);
        bool    remove_user(User *user);

        void    assignMode(channelMode mode);
        void    removeMode(channelMode mode);
        bool	isModeOnChan(channelMode mode);

        void	sendToAllUsers(std::string &msg);
		void	sendToAllOtherUsers(std::string &msg, int client_socket);
		void	sendToAllOtherNotBannedUsers(std::string &msg, int client_socket);
		void	sendToMyself(std::string &msg, int client_socket);
		void	add_to_invite(User *u);
        void    delete_invite(User *u);
		void	reset_invite();
		void	ban_user(User *u);
		void	unban_user(User *u);
        void    add_op(User *u);
        void    del_op(User *u);

        size_t  nb_user();
        int     can_user_join(User *u, std::string key);
        bool                             is_user_operator(User *u);
        bool    is_topic_on();
        bool                             is_user_in_channel(User *u);
        bool                             is_user_in_channel(std::string name);
		bool                             is_user_banned(User *u);
        //debug
        void    d_print_all_users();
    private:
        std::vector<User *>::iterator    _is_user_in_channel(std::string nickname);
        bool                             _is_user_invited(User *u);
        
};

#endif
