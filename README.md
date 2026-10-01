# FT_IRC

An IRC server built in C++ for the 42/1337 `ft_irc` project. Connect multiple clients, exchange messages, manage channels, and control a music bot through IRC commands.

The server uses TCP sockets and `poll()` to handle client connections in a single event loop. Its Makefile targets C++98; the optional music bot needs C++11 because it uses `std::stoi`.

## Features

- Password authentication and nickname/user registration.
- Public channel conversations and private messages.
- Joining and leaving channels, channel topics, invitations, and kicks.
- Channel operator privileges and configurable channel modes.
- A separate music bot with playlist, playback, and stop commands.

## Requirements

- A Unix-like environment with POSIX sockets and `poll()`.
- `make` and a C++ compiler available as `c++`.
- An IRC client such as LimeChat, or `nc` for terminal testing.
- macOS for the bot's existing audio playback implementation, which uses `afplay`.

## Build and run

```sh
git clone https://github.com/kirazizi/FT_IRC.git
cd FT_IRC
make
./ircserv 6667 demo1234
```

The server accepts two arguments:

```text
./ircserv <port> <password>
```

- Port: `1024` to `65535`.
- Password: 4 to 16 characters, without spaces.

The server binds to all available IPv4 interfaces. For local testing, connect your client to `127.0.0.1`, port `6667`, with the password `demo1234`.

### Connect with an IRC client

Create a server connection using the host, port, and password above. Choose a nickname, connect, and join a channel:

```text
/join #lobby
```

Open another client with a different nickname to try channel conversations and private messages.

### Test from the terminal

```sh
nc 127.0.0.1 6667
```

Enter these raw IRC commands, one per line:

```irc
PASS demo1234
NICK alice
USER alice 0 * :Alice
JOIN #lobby
PRIVMSG #lobby :Hello everyone!
```

Connect another terminal as `bob`, then try:

```irc
PRIVMSG bob :Hello Bob!
```

For raw IRC commands in clients that do not recognize a command, use the client's raw-command feature (for example, `/quote` where supported).

## Supported commands

| Command | Purpose |
| --- | --- |
| `PASS`, `NICK`, `USER` | Authenticate and register a client |
| `JOIN`, `PART` | Join or leave channels |
| `PRIVMSG` | Send channel or private messages |
| `TOPIC` | Read or change a channel topic |
| `INVITE`, `KICK` | Invite or remove channel members |
| `MODE` | Configure channel permissions and limits |
| `QUIT` | Disconnect from the server |
| `MPLAY` | Send instructions to the connected music bot |

### Channel modes

| Mode | Purpose | Example |
| --- | --- | --- |
| `i` | Invite-only channel | `MODE #lobby +i` |
| `t` | Restrict topic changes to operators | `MODE #lobby +t` |
| `k` | Set a channel password | `MODE #lobby +k secret` |
| `o` | Give a member operator privileges | `MODE #lobby +o bob` |
| `l` | Limit the number of members | `MODE #lobby +l 10` |

Use `-` instead of `+` to remove a mode. Operator permissions apply to protected channel changes.

## Music bot

Keep the server running. In another terminal, build the bot with C++11 and launch it from the repository root:

```sh
make bonus CFLAGS='-Wall -Wextra -Werror -std=c++11'
./mplayer 127.0.0.1 6667
```

The bot reads MP3 files from `mplay-bot/songs/`. Use these raw IRC commands from a registered client:

```irc
MPLAY list
MPLAY play 1
MPLAY play
MPLAY stop
```

`play 1` selects the first song; `play` without a number selects a random song. Keep at least one MP3 in the playlist before requesting random playback.

Audio plays on the machine running the bot, rather than streaming to IRC clients. The current implementation uses macOS `afplay`; playback on Linux or WSL requires adapting that implementation. Stopping playback uses `killall afplay`, which also stops other `afplay` processes on that machine.

## Project structure

```text
FT_IRC/
├── main.cpp          # Server entry point and argument handling
├── Makefile          # Server and optional bot builds
├── server/           # Socket setup, event loop, and command dispatch
├── client/           # Client state
├── commands/         # IRC command implementations
├── src/              # Channels, parsing, and shared helpers
└── mplay-bot/         # Music bot and local MP3 playlist
```

## Make targets

| Target | Action |
| --- | --- |
| `make` | Build `ircserv` |
| `make bonus` | Build `mplayer` (use the C++11 override above) |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executables |
| `make re` | Rebuild the server and bot (use a suitable compiler standard) |

## What this project explores

Socket programming, event-driven I/O, protocol parsing, client state, channel permissions, and a separate bot communicating with the server over TCP.

## References

- [IRC protocol — RFC 1459](https://www.rfc-editor.org/rfc/rfc1459)
- [Repository and contributors](https://github.com/kirazizi/FT_IRC/graphs/contributors)
