#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>
#include <vector>

class Command
{
	private:
		Command &	operator=(Command const & rhs);
		std::string					_rawcmd;
		std::string					_prefix;
		std::string					_cmd;
		std::vector<std::string>	_param;
		std::vector<std::string>	_channels;
		// bool						is_cmd_valid;

	public:

		Command();
		Command(Command const & src);
		virtual ~Command();
		Command(const std::string one_cmd);

		void		set_rawcmd(std::string rawcmd);
		std::string const	get_rawcmd() const;
		std::string const	get_prefix() const;
		void		set_prefix(std::string raw_cmd);
		std::string gather_cmd_prefix(std::string raw_cmd);
		void		set_cmd(std::string raw_cmd);
		std::string const	get_cmd() const;
		void		clear();
		std::vector<std::string> const	get_param() const;
		std::vector<std::string> const	get_channels() const;

		// print function
		void	print_param();
	private:
		std::string	get_cmd_name(std::string raw_cmd);
		void		set_param(std::string raw_cmd);
		std::vector<std::string>	extract_multiple_chanel_from_single_arg(std::string arg);
		void	add_multiple_channels_to_channel_list(std::vector<std::string> multiple_channels);
};

#endif
