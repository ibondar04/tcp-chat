# TCP Chat

A small multi-client TCP chat application written in C++.

This project was built to practice socket programming, client/server architecture, concurrency, threads, mutexes, shared state, and basic TCP communication.

## Features

* TCP server and client
* Multiple clients can connect simultaneously
* Username support
* Real-time text messaging
* Server broadcasts messages to other connected clients
* Concurrent client handling using `std::thread`
* Shared client state protected with `std::mutex`
* Clean client disconnect using `/quit`
* Basic socket and communication error handling

## Technologies

* C++
* Linux / WSL
* TCP sockets
* `std::thread`
* `std::mutex`
* Make

## Build

Compile both the server and client:

```bash
make
```

## Run

Start the server:

```bash
./server
```

Then open another terminal for each client:

```bash
./client
```

Enter a username when prompted.

Type messages to chat with the other connected clients.

To disconnect:

```text
/quit
```

## Architecture

The server listens for incoming TCP connections on port `8080`.

Each connected client is handled in a separate thread. The server keeps track of connected client sockets in a shared vector protected by a mutex.

When a client sends a message:

1. The server receives the message.
2. The server adds the sender's username.
3. The message is broadcast to all other connected clients.

Each client also uses a separate receiving thread so it can receive messages from the server while still allowing the user to type and send messages.

## Concepts Practiced

* TCP/IP basics
* Socket programming
* `socket()`
* `bind()`
* `listen()`
* `accept()`
* `connect()`
* `send()`
* `recv()`
* Blocking I/O
* Multi-threading
* Mutex synchronization
* Shared state between threads
* Basic client/server message flow
