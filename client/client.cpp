#ifndef UNICODE
#define UNICODE 1
#endif

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#include <iostream>
#include <Winsock2.h>
#include <time.h> 

#define DEFAULT_BUFLEN 512

int main()
{

	// Can display traditional Chinese.
	setlocale(LC_ALL, "zh-TW");

	WSADATA wsaData = { 0 };
	int iResult = 0;

	// Initialize Winsock version 2.2.
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return -1;
	}

	// Get IPv4 and TCP/IP socket.
	// Create a SOCKET for listening for 
	// incoming connection requests
	SOCKET connectSocket = INVALID_SOCKET;
	connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(60095);

	// Connect to server.
	iResult = connect(connectSocket, (SOCKADDR *)& server, sizeof(server));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}

	// Send an initial buffer
	const char *sendbuf = "Send message from the client to the server.";
	iResult = send(connectSocket, sendbuf, (int)(strlen(sendbuf) + 1), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return -1;
	}
	printf("Bytes Sent: %ld\n", iResult);

	// Receive message from the server. 
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	// Receive message loop.
	do {
		iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d. Content:\"%s\" \n", iResult, recvbuf);
			closesocket(connectSocket);
			break;
		}
		else if (iResult == 0) 
			printf("Connection closed\n");
		else 
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	// No longer need server socket
	closesocket(connectSocket);
	WSACleanup();

	system("PAUSE");
	return 0;

}

