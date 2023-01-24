#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Command.hpp"

#define END_MSG "\r\n"
#define FD_UNSET -2


typedef std::vector<Command>::iterator command_lst_iterator;

class Message
{
	private:
		Message(Message const & src);
		Message &	operator=(Message const & rhs);
		std::string				_raw_msg;
		std::vector<Command>	_command_lst;
		bool					_is_msg_complete;			

	public:
		Message();
		virtual ~Message();

		void					concat_msg(char *buff);
		void					store_raw_msg(std::string msg);
		void					parse_msg();
		std::string				get_raw_msg();
		std::string				get_one_cmd(std::string _raw_str);
		std::vector<Command>	& get_cmd_lst();
		bool					get_is_cmd_complete();
		void					clear_msg();
};

#endif
