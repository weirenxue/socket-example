#ifndef UNICODE
#define UNICODE 1
#endif

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#include <iostream>
#include <Winsock2.h>

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
	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons(60095);

	// Bind the socket.
	iResult = bind(listenSocket, (SOCKADDR *)&service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	// Listen for incoming connection requests 
	// on the created socket
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		wprintf(L"listen function failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	// Create a SOCKET for accepting incoming requests.
	SOCKET acceptSocket;
	wprintf(L"Waiting for client to connect...\n");

	// Always accept the connection.
	while (1) {
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return -1;
		} 
		else {
			wprintf(L"Client connected.\n");
			// Receive message from the client. 
			char recvbuf[DEFAULT_BUFLEN];
			int recvbuflen = DEFAULT_BUFLEN;
			// Receive message loop.
			do {
				iResult = recv(acceptSocket, recvbuf, recvbuflen, 0);
				if (iResult > 0) {
					printf("Bytes received: %d. Content:\"%s\" \n", iResult, recvbuf);
					// Send an initial buffer
					const char *sendbuf = "Retrun message from the server to the client.";
					iResult = send(acceptSocket, sendbuf, (int)(strlen(sendbuf) + 1), 0);
					if (iResult == SOCKET_ERROR) {
						printf("send failed: %d\n", WSAGetLastError());
						closesocket(acceptSocket);
						break;
					}
					printf("Bytes Sent: %ld\n", iResult);
				}
				else if (iResult == 0)
					printf("Connection closed\n");
				else
					printf("recv failed: %d\n", WSAGetLastError());
			} while (iResult > 0);
		}
	}

	// No longer need server socket
	closesocket(listenSocket);
	WSACleanup();

	system("PAUSE");
	return 0;

}

