# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qtamaril <qtamaril@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/07 08:04:23 by qtamaril          #+#    #+#              #
#    Updated: 2021/03/09 18:27:01 by welease          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl.a
FLAGS_W = -Wall -Wextra -Werror
RM = rm -rf

HEADER = get_next_line.hpp
SRCS = get_next_line.cpp get_next_line_utils.cpp
OBJS = $(SRCS:%.cpp=%.o)

ifneq (,$(findstring xterm,${TERM}))
	GREEN	:=	$(shell tput -Txterm setaf 2)
	YELLOW	:=	$(shell tput -Txterm setaf 3)
	BLUE   	:=	$(shell tput -Txterm setaf 6)
	RESET	:=	$(shell tput -Txterm sgr0)
else
	GREEN	:=	""
	RESET	:=	""
	YELLOW	:=	""
endif

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	@ar rc $(NAME) $(OBJS)
	@echo "${BLUE}GetNextLine is done!${RESET}"

%.o: %.cpp
	@clang++ $(FLAGS_W) -o $@ -c $<

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
