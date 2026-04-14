#include "socket_client.h"
#include <signal.h>
#include <string>

//#define OFFLINE

int main(int argc, const char **argv)
{
	// read target IP address for transmission
	std::string targetIP;
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	std::cin >> targetIP;

	// main code starts here
	SocketClient socketClient;
	socketClient.CreateSocket();
	socketClient.ConnectToServer();
#ifdef OFFLINE
	socketClient.ReceiveMsg();
#else
	socketClient.ReceiveMsgOnce();
#endif
	return 0;
}