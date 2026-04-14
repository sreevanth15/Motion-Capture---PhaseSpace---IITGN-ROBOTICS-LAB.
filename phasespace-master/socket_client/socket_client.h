#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#define API __declspec(dllexport)
#pragma once

#include <winsock2.h>
#include <iostream>
#include <signal.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

class API SocketClient
{
public:
	SocketClient();
	~SocketClient();

	// socket communication
	void CreateSocket();
	void ConnectToServer();
	void ReceiveMsg();
	int ReceiveMsgOnce();
	void CloseSocket();

private:
	WSADATA mWSAData;
	SOCKET mServer;
	SOCKET mClient;
	SOCKADDR_IN mServerAddr;
	SOCKADDR_IN mClientAddr;
	SOCKADDR_IN mAddr;
};

#endif // !SOCKET_CLIENT_H
