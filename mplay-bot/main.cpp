/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 11:32:51 by sbzizal           #+#    #+#             */
/*   Updated: 2024/02/16 12:35:09 by sbzizal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mplayer.hpp"

int main(int ac, char **av) {
    if (ac != 3) {
        std::cout << "Usage: " << av[0] << " <Server IP> <Port>";
        return 1;
    }
    try{
        system("clear");
        signal(SIGINT, signal_handler);
        mplayer mp(std::atoi(av[2]), av[1]);
        mp.run();
    }
    catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
    return 0;
}
