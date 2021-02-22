NAME = libft.a
CC = clang++
CFLAGS = -Wall -Wextra -Werror
HEADERS=libft.h
SRC =	ft_atoi.cpp             ft_isalnum.cpp          ft_isprint.cpp          ft_memcmp.cpp           ft_putnbr_fd.cpp        ft_str_is_num.cpp       ft_strlcat.cpp          ft_strncmp.cpp          ft_strtrim.cpp \
        ft_bzero.cpp            ft_isalpha.cpp          ft_isspace.cpp          ft_memcpy.cpp           ft_putstr_fd.cpp        ft_strchr.cpp           ft_strlcpy.cpp          ft_strnstr.cpp          ft_substr.cpp \
        ft_calloc.cpp           ft_isascii.cpp          ft_itoa.cpp             ft_putchar_fd.cpp       ft_split.cpp            ft_strdup.cpp           ft_strlen.cpp           ft_strrchr.cpp \
        ft_free_strstr.cpp      ft_isdigit.cpp          ft_memchr.cpp           ft_putendl_fd.cpp       ft_splitTim.cpp         ft_strjoin.cpp          ft_strmapi.cpp          ft_strstrlen.cpp \
        ft_memjoin.cpp 			ft_memdup.cpp 			ft_memsplit.cpp 		ft_memmem.cpp
BONUS_SRC  = ft_lstmap.c ft_lstiter.c ft_lstclear.c ft_lstdelone.c ft_lstadd_back.c ft_lstlast.c ft_lstsize.c  ft_lstnew.c ft_lstadd_front.c
BONUS_OBJ=$(BONUS_SRC:.c=.o)
RM = rm -f
OBJ = $(SRC:%.cpp=%.o)

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

.PHONY: all clean fclean bonus re

all: $(NAME)

$(NAME): $(OBJ) $(HEADERS)
	@ar rc $(NAME) $(OBJ)
	@echo "${BLUE}libft is done${RESET}"

%.o:%.cpp
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@$(RM) $(OBJ)
	@$(RM) $(BONUS_OBJ)

fclean: clean
	@$(RM) $(NAME)

bonus: $(NAME) $(BONUS_SRC) $(HEADERS)
		$(CC) $(CFLAGS) -c $(BONUS_SRC)
		ar rc $(NAME) $(BONUS_OBJ)

re: fclean all

