#ifndef WEBSERV_HEADERVALIDATION_HPP
#define WEBSERV_HEADERVALIDATION_HPP

//#include "../includes/includes.hpp"
#include <vector>
#include <iostream>
#include "../libft/libft.h"

class HeaderValidation {

private:
    size_t  headerCountLines;

    void    isValidLine_First(std::string);
    void    isValidLine_Second(std::string);
    void    isValidLine_KeyValue(std::string line, bool & hasHost);

public:
    void    valid(char *);
    size_t  getHeaderCountLines() const;

    class HeaderValidationException : public std::exception {
    private:
        std::string		_message;
    public:
        HeaderValidationException(std::string const &message) :
            _message("Incorrect line \"" + message + "\" in request head") {
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
