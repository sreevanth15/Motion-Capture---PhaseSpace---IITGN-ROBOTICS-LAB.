#include <signal.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <owl/owl.hpp>
#include "record_data.h"
#include "communication.h"
using namespace std;

Communication gCommunication;

void ExitHandler(int s)
{
	std::cout << "Ctrl+C detected!" << '\n';
	gCommunication.flag = 1;
	gCommunication.CloseSocket();
}

void PhaseSpaceStreaming(const std::string& address)
{
	// create phasespace thread
	if (gCommunication.ConnectToPhaseSpace(address))
	{
		gCommunication.StartStreaming();
	}
	else
	{
		exit(-1);
	}
	std::cout << "[Server] PhaseSpace stop streaming." << '\n';
	return;
}

void SocketHostStreaming()
{
	// create socket thread
	gCommunication.CreateSocket();
	gCommunication.BindAndListen();
	do {
		gCommunication.SendMsg();
	} while (!gCommunication.flag);
	return;
}

int main(int argc, const char **argv)
{
	// read PhaseSpace IP address
	std::string phasespaceIP;
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(0);
	std::cout << "[Server] Please enter PhaseSpace IP address (default: 192.168.1.230): ";
	std::cin >> phasespaceIP;

	signal(SIGINT, ExitHandler);

	// TCP socket to host threading
	std::thread socketHostThread(SocketHostStreaming);

	// PhaseSpace streaming
	std::thread phasespaceThread(PhaseSpaceStreaming, phasespaceIP);

	socketHostThread.join();
	phasespaceThread.join();
	
	return 0;
}
