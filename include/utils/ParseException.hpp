/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseException.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhuszar <mhuszar@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:32:03 by mhuszar           #+#    #+#             */
/*   Updated: 2024/10/29 22:21:00 by mhuszar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_EXCEPTION_HPP
# define PARSE_EXCEPTION_HPP

#include "defines.hpp"
#include <iostream>


class ParseException : public std::exception
{
    public:

        ParseException(webshell::StatusCode code, std::string msg);
        ~ParseException() throw();
        ParseException(const ParseException& other);
    
        void displayIssue() const;
        webshell::StatusCode code() const;
        std::string msg() const;
        

    private:

        ParseException(void);
        ParseException& operator=(const ParseException& other);

        webshell::StatusCode _code;
        std::string _msg;
};

#endif