/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:40 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/29 21:01:34 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

std::string get_cmd(const std::string &str){
    std::istringstream iss(str);
    std::string firstWord;
    iss >> firstWord;  // Extract the first word
    return firstWord;
}

std::string get_value(const std::string &str){
    std::istringstream iss(str);
    std::string firstWord;
    iss >> firstWord;  // Extract the first word
    
    std::string rest = str.substr(firstWord.size() + 1);
    // trim leading and trailing whitespaces
    return rest.substr(rest.find_first_not_of(" \t\r\n"), rest.find_last_not_of(" \t\r\n") + 1);
    // std::string rest;
    // std::getline(iss, rest);  // Get the rest of the string after the first word

    // // Remove leading and trailing whitespaces from the rest
    // return rest.substr(rest.find_first_not_of(" \t\r\n"), rest.find_last_not_of(" \t\r\n") + 1);
}

int ft_strlen(char *str){
    if (!str)
        return 0;
    int i = 0;
    while (str[i])
        i++;
    return i;
}