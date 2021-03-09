/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atomatoe <atomatoe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 19:05:10 by atomatoe          #+#    #+#             */
/*   Updated: 2021/03/09 18:27:01 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A
#define BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A

#include <string>
//
// look this site! https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/
// webserv!

#if __cplusplus >= 201703L
#include <string_view>
#endif  // __cplusplus >= 201703L

std::string base64_encode     (std::string const& s, bool url = false);
std::string base64_encode_pem (std::string const& s);
std::string base64_encode_mime(std::string const& s);

std::string base64_decode(std::string const& s, bool remove_linebreaks = false);
std::string base64_encode(unsigned char const*, size_t len, bool url = false);

#if __cplusplus >= 201703L
//
// Interface with std::string_view rather than const std::string&
// Requires C++17
// Provided by Yannic Bonenberger (https://github.com/Yannic)
//
std::string base64_encode     (std::string_view s, bool url = false);
std::string base64_encode_pem (std::string_view s);
std::string base64_encode_mime(std::string_view s);

std::string base64_decode(std::string_view s, bool remove_linebreaks = false);
#endif  // __cplusplus >= 201703L

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */