NAME = webserv
GNL_A = GetNextLine/gnl.a
LIBFT_A = Libft/libft.a

DIR_GNL = GetNextLine
DIR_LIBFT = Libft

FLAGS_W = -Wall -Wextra -Werror
CC = clang++
RM = rm -rf

SRCS =	Sources/main.cpp \
        Sources/ServersProcessing.cpp \
        Sources/CGI/CGI.cpp \
        Sources/PageHtmlGeneration/PageHtml.cpp \
        GetNextLine/get_next_line.cpp \
        GetNextLine/get_next_line_utils.cpp \
        Includes/includes.cpp \
        Sources/Response/Response.cpp \
        Sources/WebServer/WebServ.cpp \
        Sources/Request/Request.cpp \
        Sources/ParseConfig/Location.cpp \
        Sources/ParseConfig/ParseUtils.cpp \
		Sources/Client/Client.cpp \
		Sources/Bytes/Bytes.cpp \
		Sources/ParseConfig/ParseConfig.cpp \
		AuthClients/base64.cpp

OBJS = $(SRCS:%.cpp=%.o)

ifneq (,$(findstring xterm,${TERM}))
	GREEN	:=	$(shell tput -Txterm setaf 2)
	YELLOW	:=	$(shell tput -Txterm setaf 3)
	RESET	:=	$(shell tput -Txterm sgr0)
else
	GREEN	:=	""
	RESET	:=	""
	YELLOW	:=	""
endif

.PHONY: all clean fclean  re
all: $(NAME)

$(NAME): $(LABEL) $(OBJS)
	@echo "Preparing GetNextLine..."
	@make -C $(DIR_GNL)
	@echo "Preparing Libft..."
	@make -C $(DIR_LIBFT)
	@$(CC) $(OBJS) $(INCLUDES_OBJS) $(RR_OBJS) $(PARSE_OBJS) $(GNL_A) $(LIBFT_A) -o $(NAME) #TODO add WWW flags
	@echo "${GREEN}WebServer is ready to work!٩(๑˃̵ᴗ˂̵)و${RESET}"

%.o:%.cpp
	@$(CC) -o $@ -c $<

clean:
	@make -C $(DIR_GNL) clean
	@make -C $(DIR_LIBFT) clean
	@$(RM) $(OBJS)
	@echo "${YELLOW}All cleaned!${RESET}"

fclean: clean
	@make -C $(DIR_GNL) fclean
	@make -C $(DIR_LIBFT) fclean
	@$(RM) $(NAME)
	@echo "${YELLOW}All fcleaned!${RESET}"

re: fclean all
