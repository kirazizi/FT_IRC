/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 16:03:35 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/27 18:30:57 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

void parsing::parsing_input(int port, std::string password){
    if (port < 0 || port > 65536)
        std::cout << "Port number must be between 0 and 65536" << std::endl;
    (void)password;
    // else if (password.length() < 6)
    //     std::cout << "Password must be at least 6 characters" << std::endl;
}