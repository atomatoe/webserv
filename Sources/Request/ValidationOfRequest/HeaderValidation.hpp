#ifndef WEBSERV_HEADERVALIDATION_HPP
#define WEBSERV_HEADERVALIDATION_HPP

#include <vector>
#include <iostream>
#include "../../../Libft/libft.h"

class HeaderValidation {

private:
    int     headerCountLines;

    void    isValidLine_First(std::string, char *);
//    void    isValidLine_Second(std::string);
    void    isValidLine_KeyValue(std::string, char *, bool &);

public:
    void    valid(char *);
    size_t  getHeaderCountLines() const;

    class HeaderValidationException : public std::exception {
    private:
        std::string		_message;
    public:
        HeaderValidationException(std::string const &message, char *subStr) :
            _message("Incorrect line \"" + message + "\" in request head") {
            free(subStr);
        }
        virtual ~HeaderValidationException() throw() { }
        virtual const char*		what() const throw() { return (_message.c_str()); }
    };

    class NoHostInHeaderException : public std::exception {
    public:
        virtual const char*		what() const throw() { return ("No \"HOST\" key in header"); }
    };
};

#endif //WEBSERV_HEADERVALIDATION_HPP
