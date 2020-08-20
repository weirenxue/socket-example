# This project is an example of using Win32 API WSAStartup, socket, bind, listen, accept, recv, send and connect.
### Enveironment: Visual Studio Pro 2017, Windows10 x64.

1. There is one .sln file and two projects in this repository. The project names are server and client respectively.  
2. server : The server defines the listener socket and binds it to a specific address and port. Listen to and wait for accept client's connection. Finally, receive message from client and reply message.
3. client : The client defines the connection socket and connects to the server. Send the prepared message to the server and receive the message from the server. Close the socket and WSACleanup.

## How To Use
1. Open the server.exe.
2. Open the client.exe.