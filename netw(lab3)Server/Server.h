	#pragma once
	#pragma comment(lib, "ws2_32.lib")
	#include <winsock2.h>
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	#include <string>
	#include <iostream>


	#pragma warning(disable: 4996)


	enum Packet {
		P_ChatMessage,
		P_Test
	};



	class Server
	{
	public:
		Server(int PORT, bool BroadcastPuplically = false);
		bool ListenForNewConnection();

	private:
		//send
		bool SendInt(int ID,int _int);
		bool SendPacketType(int ID, Packet _packettype);
		bool SendString(int ID, std::string & _string);

		//get
		bool GetInt(int ID, int & _int);
		bool GetPacketType(int ID, Packet & _packettype);
		bool GetString(int ID, std::string & _string);

		bool ProcessPacket(int ID, Packet _packettype);

		static void ClientHandlerThread(int ID);

	private:
		SOCKET Connections[100];
		int TotalConnections = 0;
		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		SOCKET sListen;


	};

	static Server * serverptr;