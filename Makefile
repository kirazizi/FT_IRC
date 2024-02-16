# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbzizal <sbzizal@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/23 14:01:20 by sbzizal           #+#    #+#              #
#    Updated: 2024/02/16 13:37:10 by sbzizal          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ./channel.cpp ./client.cpp ./command.cpp ./command_op.cpp ./commands/invite_cmd.cpp \
./commands/join_cmd.cpp ./commands/kick_cmd.cpp ./commands/mode_cmd.cpp ./commands/mplay_cmd.cpp \
./commands/part_cmd.cpp ./commands/privmsg_cmd.cpp ./commands/quit_cmd.cpp ./commands/topic_cmd.cpp \
./main.cpp ./minisrv.cpp  ./parsing.cpp ./server.cpp \

BNS = ./mplay-bot/main.cpp ./mplay-bot/mplayer.cpp \

HDR = server.hpp client.hpp headers.hpp parsing.hpp channel.hpp \

HDR_BNS = ./mplay-bot/mplayer.hpp \

EXE = ircserv

EXE_BNS = mplayer

all: $(EXE)

$(EXE): $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

bonus: $(EXE_BNS)

$(EXE_BNS): $(BNS) $(HDR_BNS)
	$(CC) $(CFLAGS) $(BNS) -o $(EXE_BNS)

clean:
	rm -f $(EXE) $(EXE_BNS)

fclean: clean

re: fclean all bonus

.PHONY: all bonus clean fclean re