cmake_minimum_required(VERSION 3.17)
project(webserv_merged)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cgi_proccesing)
include_directories(includes)
include_directories(libft)
include_directories(request)

add_executable(webserv_merged
        cgi_proccesing/cgi.h
        cgi_proccesing/cgi_proccesing.cpp
        includes/includes.cpp
        includes/includes.hpp
        includes/page_html.cpp
        includes/page_html.hpp
        libft/ft_atoi.cpp
        libft/ft_bzero.cpp
        libft/ft_calloc.cpp
        libft/ft_isalnum.cpp
        libft/ft_isalpha.cpp
        libft/ft_isascii.cpp
        libft/ft_isdigit.cpp
        libft/ft_isprint.cpp
        libft/ft_itoa.cpp
        libft/ft_memchr.cpp
        libft/ft_memcmp.cpp
        libft/ft_memcpy.cpp
        libft/ft_putchar_fd.cpp
        libft/ft_putendl_fd.cpp
        libft/ft_putnbr_fd.cpp
        libft/ft_putstr_fd.cpp
        libft/ft_split.cpp
        libft/ft_strchr.cpp
        libft/ft_strdup.cpp
        libft/ft_strjoin.cpp
        libft/ft_strlcat.cpp
        libft/ft_strlcpy.cpp
        libft/ft_strlen.cpp
        libft/ft_strmapi.cpp
        libft/ft_strncmp.cpp
        libft/ft_strnstr.cpp
        libft/ft_strrchr.cpp
        libft/ft_substr.cpp
        libft/ft_tolower.cpp
        libft/ft_toupper.cpp
        libft/libft.h
        request/Request.cpp
        request/Request.hpp
        main.cpp
        server.cpp
        server.hpp)
