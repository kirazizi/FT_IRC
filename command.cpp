/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 15:52:00 by sbzizal           #+#    #+#             */
/*   Updated: 2024/01/01 15:53:04 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "client.hpp"

// void hundel_cmd(std::string cmd, int fdclient){
//     int i = 0;
//     std::string commands[] = {"JOIN", "LEAVE", "MSG", "QUIT"};
//     while(i < 5){
//         if(cmd == commands[i])
//             break;
//         i++;
//     }
//     switch(i){
//         case 0:
//             // join_channel();
//         case 1:
//             // leave_channel();
//         case 2:
//             // send_msg();
//         case 3:
//             // quit();
//     }
// }