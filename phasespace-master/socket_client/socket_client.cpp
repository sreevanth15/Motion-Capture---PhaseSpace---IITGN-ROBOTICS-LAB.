#include "socket_client.h"

API SocketClient::SocketClient()
{
}

API SocketClient::~SocketClient()
{
	closesocket(mClient);
	WSACleanup();
}

API void SocketClient::CreateSocket()
{
	int isSocketOK = WSAStartup(MAKEWORD(2, 2), &mWSAData);
	if (isSocketOK != 0)
	{
		std::cerr << "[Client] Cannot initialize socket!" << '\n';
		return;
	}
	mServer = socket(AF_INET, SOCK_STREAM, 0);
	if (mServer == INVALID_SOCKET)
	{
		std::cerr << "[Client] Cannot create a socket!" << '\n';
		return;
	}
}

API void SocketClient::ConnectToServer()
{
	mAddr.sin_addr.s_addr = inet_addr("192.168.50.19"); // local ip
	mAddr.sin_family = AF_INET;
	mAddr.sin_port = htons(5555);
	if (!connect(mServer, (SOCKADDR*)&mAddr, sizeof(mAddr)))
	{
		std::cerr << "[Client] Cannot connect to server" << '\n';
		//std::cout << "[Client] Connected to server!" << '\n';
	}
}

API void SocketClient::ReceiveMsg()
{
	char buffer[100];
	int result;
	do {
		result = recv(mServer, buffer, sizeof(buffer), 0);
		if (result > 0)
		{
			//std::cout << "[Client] Received data" << '\n';
			//mRecordData.SetCurData(buffer);
		}
		else if (result == 0)
		{
			//std::cout << "[Client] Server closed." << '\n';
			CloseSocket();
			break;
		}
		else
		{
			//std::cerr << "[Client] Receive data failed. Closing." << '\n';
			CloseSocket();
			break;
		}
		memset(buffer, 0, sizeof(buffer));
	} while (result > 0);
}

API int SocketClient::ReceiveMsgOnce()
{
	char buffer[100];
	char test[4];
	int result;
	result = recv(mServer, buffer, sizeof(buffer), 0);
	if (result > 0)
	{
		strcpy(test, buffer);
		std::cout << test << '\n';
		//std::cout << "[Client] Received data: " << buffer << '\n';
		//mRecordData.SetCurData(buffer);
		return 1;
	}
	else if (result == 0)
	{
		//std::cout << "[Client] Server closed." << '\n';
		CloseSocket();
		return -1;
	}
	else
	{
		std::cerr << "[Client] Receive data failed. Closing." << '\n';
		CloseSocket();
		return -1;
	}
}

API void SocketClient::CloseSocket()
{
	closesocket(mClient);
	WSACleanup();
}