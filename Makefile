# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/23 14:01:20 by sbzizal           #+#    #+#              #
#    Updated: 2024/03/10 12:00:06 by sbzizal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98  -fsanitize=address

SRC = ./channel.cpp ./client.cpp ./command.cpp ./commands/invite_cmd.cpp \
./commands/join_cmd.cpp ./commands/kick_cmd.cpp ./commands/mode_cmd.cpp ./commands/mplay_cmd.cpp \
./commands/part_cmd.cpp ./commands/privmsg_cmd.cpp ./commands/quit_cmd.cpp ./commands/topic_cmd.cpp \
./commands/identify_cmd.cpp ./main.cpp ./parsing.cpp ./server.cpp utils.cpp \

BNS = ./mplay-bot/main.cpp ./mplay-bot/mplayer.cpp \

HDR = server.hpp client.hpp headers.hpp channel.hpp \

HDR_BNS = ./mplay-bot/mplayer.hpp \

OBJ = $(SRC:.cpp=.o)

OBJ_BNS = $(BNS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(OBJ_BNS)
	$(CC) $(CFLAGS) $(OBJ_BNS) -o mplayer

clean:
	rm -f $(OBJ) $(OBJ_BNS)

fclean: clean
	rm -f ircserv mplayer

re: fclean all

.PHONY: all bonus clean fclean re

