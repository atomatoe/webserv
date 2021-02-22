NAME = webserv
GNL_A = gnl/gnl.a
LIBFT_A = libft/libft.a

DIR_GNL = gnl
DIR_LIBFT = libft

FLAGS_W = -Wall -Wextra -Werror
CC = clang++
RM = rm -rf

SRCS =	WebServ.cpp \
		main.cpp \
		includes/includes.cpp \
		includes/page_html.cpp \
		Request_Response/Bytes.cpp \
		Request_Response/Request.cpp \
		Request_Response/Response.cpp \
		parse/Location.cpp \
		parse/ParseConfig.cpp \
		parse/parse_utils.cpp

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
	@echo "Preparing gnl.."
	@make -C $(DIR_GNL)
	@echo "Preparing libft.."
	@make -C $(DIR_LIBFT)
	@$(CC) $(OBJS) $(INCLUDES_OBJS) $(RR_OBJS) $(PARSE_OBJS) $(GNL_A) $(LIBFT_A) -o $(NAME)
	@echo "${GREEN}webserv is ready to work!${RESET}"


%.o:%.cpp
	@$(CC) -o $@ -c $<

clean:
	@make -C $(DIR_GNL) clean
	@make -C $(DIR_LIBFT) clean
	@$(RM) $(OBJS)
	@echo "${YELLOW}all cleaned${RESET}"

fclean: clean
	@make -C $(DIR_GNL) fclean
	@make -C $(DIR_LIBFT) fclean
	@$(RM) $(NAME)
	@echo "${YELLOW}all fcleaned${RESET}"

re: fclean all
