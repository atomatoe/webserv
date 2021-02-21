NAME = webserv

DIR_GNL = gnl
DIR_LIBFT = libft

FLAGS_W = -Wall -Wextra -Werror
CC = clang++

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

$(NAME):
	@make -C $(DIR_GNL)
	@make -C $(DIR_LIBFT)
	@echo "${GREEN}all done!${RESET}"

%.o:%.cpp
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@make -C $(DIR_GNL) clean
	@make -C $(DIR_LIBFT) clean
	@echo "${YELLOW}all cleaned${RESET}"

fclean: clean
	@make -C $(DIR_GNL) fclean
	@make -C $(DIR_LIBFT) fclean
	@rm -rf $(NAME)
	@echo "${YELLOW}all fcleaned${RESET}"

re: fclean all
