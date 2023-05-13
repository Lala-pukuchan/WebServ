NAME 	:= webserv
RM := rm -fr

CONFIGDIR := config
CONFIGS := LocationConfig.cpp ServerConfig.cpp Servers.cpp
CONFIGS := $(addprefix $(CONFIGDIR)/, $(CONFIGS))

SERVERDIR := server
SERVERS := ServerSocket.cpp Webserv.cpp
SERVERS := $(addprefix $(SERVERDIR)/, $(SERVERS))

SRCDIR	:= src
SRCS	:= main.cpp ClientRequest.cpp $(CONFIGS) $(SERVERS)
SORCES	:= $(addprefix $(SRCDIR)/, $(SRCS))

OBJDIR	:= objs
OBJS	:= $(SRCS:.cpp=.o)
OBJECTS	:= $(addprefix $(OBJDIR)/, $(OBJS))

INCLUDE := include

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98

GR= \033[32;1m
RE= \033[31;1m
YE= \033[33;1m
CY= \033[36;1m
RC= \033[0m

all: $(NAME)

$(NAME) : $(OBJECTS)
	@printf "$(GR)"
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) $(OBJECTS) -o $@
	@printf "$(RC)"

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)/$(*D)
	@printf "$(YE)"
	$(CXX) $(CXXFLAGS) -I $(INCLUDE) -c $< -o $@
	@printf "$(RC)"

clean :
	@printf "$(RE)"
	$(RM) $(OBJDIR)
	@printf "$(RC)"

fclean : clean
	@printf "$(RE)"
	$(RM) $(NAME)
	@printf "$(RC)"

re : fclean all

t : all
	./webserv default.conf

.PHONY: all clean fclean re t

