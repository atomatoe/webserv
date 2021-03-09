/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:49:55 by welease           #+#    #+#             */
/*   Updated: 2021/03/09 11:37:07 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <vector>
# include <unistd.h>
# define memBody std::vector < std::pair <size_t, char *> >

typedef struct		        s_list
{
	void			        *content;
	struct  s_list	        *next;
}					        t_list;

t_list				        *ft_lstnew(void *content);
int					        ft_lstsize(t_list *lst);
t_list				        *ft_lstlast(t_list *lst);
void				        ft_lstdelone(t_list *lst, void (*del)(void*));
void				        ft_lstclear(t_list **lst, void (*del)(void*));
void				        ft_lstiter(t_list *lst, void (*f)(void *));
t_list				        *ft_lstmap(t_list *lst, void *(*f)(void *),
					        void (*del)(void *));
void				        *ft_memset (void *destination, int c, size_t n);
void				        ft_bzero(void *s, size_t n);
void				        *ft_memcpy(void *dest, const void *source, size_t count);
void				        *ft_memccpy(void *destination,
					        const void *source, int c, size_t n);
void				        *ft_memmove(void *destination,
					        const void *source, size_t n);
void				        *ft_memchr(const void *arr, int c, size_t n);
int					        ft_memcmp(const void *s1, const void *s2, size_t n);
size_t				        ft_strlen(const char *str);
size_t				        ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t				        ft_strlcat(char *dst, const char *src, size_t dstsize);
char				        *ft_strchr(const char *s, int c);
char				        *ft_strrchr(const char *s, int c);
char				        *ft_strnstr(char *str, char *to_find, size_t len);
int					        ft_strncmp(const char *s1, const char *s2, size_t n);
int					        ft_atoi(const char *str);
int					        ft_isalpha(int c);
int					        ft_isdigit(int c);
int					        ft_isalnum(int c);
int					        ft_isascii(int c);
int					        ft_isprint(int c);
int					        ft_toupper(int c);
int					        ft_tolower(int c);
void				        *ft_calloc(size_t num, size_t size);
char				        *ft_strdup(char const *src);
char				        *ft_substr(char const *s, unsigned int start, size_t len);
char				        *ft_strjoin(char const *s1, char const *s2);
char				        *ft_strtrim(char const *s1, char const *set);
char				        *ft_itoa(int n);
char				        *ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				        ft_putchar_fd(char c, int fd);
void				        ft_putstr_fd(char *s, int fd);
void				        ft_putendl_fd(char *s, int fd);
void				        ft_putnbr_fd(int n, int fd);

bool	                    ft_str_is_num(char const *str);
char			            **ft_splitTim(char const *s, char c);
size_t	                    ft_strstrlen(char **splitted);
void	                    ft_free_strstr(char **splitted);
bool	                    ft_isspace(int c);
char				        *ft_memdup(char const *src, size_t size);
char 				        *ft_memjoin(char *s1, char *s2, size_t len1, size_t len2);
memBody				         *ft_memsplit(char *s, char *c, size_t size, size_t len);
char				        *ft_memmem(char *haystack, size_t hlen, char const *needle, size_t nlen);
std::vector<std::string>    ft_splitString(std::string s, std::string delimiter);

#endif
