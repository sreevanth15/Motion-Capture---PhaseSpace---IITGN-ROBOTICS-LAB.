#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#pragma once

#include <winsock2.h>
#include <owl/owl.hpp>
#include <iostream>
#include <signal.h>
#include <thread>
#include "record_data.h"

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)
#define DEBUG

class Communication
{
public: 
	Communication();
	~Communication();

	// handling PhaseSpace data
	int ConnectToPhaseSpace(const std::string& address);
	void StartStreaming();
	
	// socket communication
	void CreateSocket();
	void BindAndListen();
	void ConvertDataToString(
		const int& frameNum, const int& id, 
		const float& x, const float&y, const float& z);
	void SendMsg();
	void SendMsgTest();
	void CloseSocket();

	int flag;
private:
	int mMarkerCount;
	std::string mSocketData;
	RecordData mRecordData;
	OWL::Context mOwl;
	OWL::Markers mMarkers;
	std::thread mPhaseSpaceThread;
	WSADATA mWSAData;
	SOCKET mServer;
	SOCKET mClient;
	SOCKADDR_IN mServerAddr;
	SOCKADDR_IN mClientAddr;
	SOCKADDR_IN mAddr;
};

#endif // !COMMUNICATION_H
