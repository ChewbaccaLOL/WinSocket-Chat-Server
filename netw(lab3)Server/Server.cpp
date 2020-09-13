#include "pch.h"

#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically)
{
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	
	if (BroadcastPublically)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr = inet_addr("192.168.1.106");//loc

	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::string ErrorMessage = "Failed to bind the address to our listening socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::string ErrorMessage = "Failed to listen the listening socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverptr = this;

}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection;

	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		std::cout << "Error #2\n";
		return false;
	}
	else {
		std::cout << "Client Connected!\n";
		Connections[TotalConnections] = newConnection;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL);
		std::string msg = "Hello. Welcome to the chat!";
		SendString(TotalConnections, msg);
		TotalConnections += 1;
		return true;
	}
}

bool Server::ProcessPacket(int index, Packet _packettype)
{
	switch (_packettype) {
	case P_ChatMessage:
	{
		std::string Message;
		if (!GetString(index, Message))
			return false;

		for (int i = 0; i < TotalConnections; i++) {
			if (i == index)
				continue;
			if (!SendString(i, Message))
			{
				std::cout << "Failed to send message from client ID: " << index << " to client ID: " << i << std::endl;
			}
		}
		std::cout << "Processed chat message packet from user ID: " << index << std::endl;
		break;
	}
	default:
		std::cout << "Unrecognized packet: " << _packettype << std::endl;
		break;
	}

	return true;
}

void Server::ClientHandlerThread(int index)
{
	Packet packettype;
	while (true) {
		if (!serverptr->GetPacketType(index, packettype))
			break;
		if (!serverptr->ProcessPacket(index, packettype)) {
			break;
		}
	}
	std::cout << "Lost connection to client ID: " << index << std::endl;
	closesocket(serverptr->Connections[index]);
}