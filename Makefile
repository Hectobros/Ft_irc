.SUFFIXES	:

NAME		=	ircserv
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -MMD -g3
RM		=	rm -rf

INC_PATH	= ./inc

OBJ_PATH	=	obj/

SRCS_PATH 		=	src

SRC_SERVERCMD_PATH	= ServerCommands/

SRCS_CMD_LST		=	$(addprefix $(SRC_SERVERCMD_PATH), $(SRC_SERVERCMD))

SRCS_LIST		=	main.cpp \
					Server.cpp \
					User.cpp \
					Message.cpp \
					Command.cpp \
					ServerReplies.cpp \
					Channel.cpp \
					CommandExecutor.cpp \
					CommandExecutor_NICK.cpp \
					CommandExecutor_PASS.cpp \
					CommandExecutor_USER.cpp \
					CommandExecutor_PING.cpp \
					CommandExecutor_JOIN.cpp \
					CommandExecutor_PART.cpp \
					CommandExecutor_KICK.cpp \
					CommandExecutor_AWAY.cpp \
					CommandExecutor_INVITE.cpp \
					CommandExecutor_MODE.cpp \
					CommandExecutor_PRIVMSG.cpp \
					CommandExecutor_TOPIC.cpp \
					CommandExecutor_OPER.cpp \
					CommandExecutor_KILL.cpp \
					CommandExecutor_QUIT.cpp \
					CommandExecutor_WHO.cpp \
					CommandExecutor_NOTICE.cpp


SRCS			=	$(addprefix $(SRCS_PATH), $(SRCS_LIST))



OBJS	=	$(addprefix $(OBJ_PATH), $(SRCS_LIST:.cpp=.o))


$(OBJ_PATH)%.o:		$(SRCS_PATH)/%.cpp
					@mkdir -p $(OBJ_PATH)
					@mkdir -p obj/src
					@mkdir -p obj/ServerCommands
					$(CXX) $(CXXFLAGS)  -I$(INC_PATH) -c $< -o $@

.Phony: all
all:	$(NAME)

$(NAME):	$(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -I$(INC_PATH) -o $(NAME)
		@echo "$(NAME) is compiled"

DEPS	=	$(OBJS:.o=.d)

.Phony: clean
clean:	
		$(RM) $(OBJS)
		$(RM) $(DEPS)
		@echo "Cleaned .o !"

.Phony: fclean
fclean:		clean
			$(RM) $(NAME)
			$(RM) $(DEPS)
			$(RM) $(OBJ_PATH)
			@echo "All cleaned !"
.Phony: re
re:	fclean all


-include $(DEPS)
