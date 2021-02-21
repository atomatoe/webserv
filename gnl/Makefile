# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qtamaril <qtamaril@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/07 08:04:23 by qtamaril          #+#    #+#              #
#    Updated: 2020/09/07 08:37:19 by qtamaril         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gnl.a
FLAGS_W = -Wall -Wextra -Werror
RM = rm -rf

HEADER = get_next_line.hpp
SRCS = get_next_line.cpp get_next_line_utils.cpp
OBJS = $(SRCS:%.cpp=%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	@ar rc $(NAME) $(OBJS)
	@echo "gnl is created"

%.o: %.cpp
	@clang++ $(FLAGS_W) -o $@ -c $<

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all
