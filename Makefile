# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tajjid <tajjid@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/23 14:01:20 by sbzizal           #+#    #+#              #
#    Updated: 2024/01/04 18:33:36 by tajjid           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++

CFLAGS = -Wall -Wextra -Werror

SRC = main.cpp parsing.cpp server.cpp client.cpp command.cpp op_command.cpp channel.cpp

OBJ = ircserv

all: $(OBJ)

$(OBJ): $(SRC) parsing.hpp server.hpp client.hpp channel.hpp
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

re: fclean all

fclean:
	rm -f $(OBJ)

.PHONY: all fclean