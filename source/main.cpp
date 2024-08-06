/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyeh <lyeh@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 22:49:41 by lyeh              #+#    #+#             */
/*   Updated: 2024/08/06 20:03:23 by lyeh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

int main()
{
    weblog::Logger logger;

    logger.set_level(weblog::INFO);
    logger.log(weblog::INFO, "Hello, world!");
    logger.log(weblog::DEBUG, "This is a debug message");
    logger.log(weblog::WARNING, "This is a warning message");
    logger.log(weblog::ERROR, "This is an error message");
    logger.log(weblog::CRITICAL, "This is a critical message");

    return 0;
}
