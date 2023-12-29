/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:30:17 by sbzizal           #+#    #+#             */
/*   Updated: 2023/12/29 20:43:24 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers.hpp"
#include "server.hpp"
#include <sstream>

std::string get_cmd(const std::string& str);
std::string get_value(const std::string& str);
int ft_strlen(char *str);