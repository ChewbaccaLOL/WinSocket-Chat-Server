#include "pch.h"

#include "Server.h"

bool Server::SendInt(int ID, int _int)
{
	int RetnCheck = send(Connections[ID], (char*)&_int, sizeof(int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::GetInt(int ID, int & _int)
{
	int RetnCheck = recv(Connections[ID], (char*)&_int, sizeof(int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::SendPacketType(int ID, Packet _packettype)
{
	int RetnCheck = send(Connections[ID], (char*)&_packettype, sizeof(Packet), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::GetPacketType(int ID, Packet & _packettype)
{
	int RetnCheck = recv(Connections[ID], (char*)&_packettype, sizeof(Packet), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::SendString(int ID, std::string & _string)
{
	if (!SendPacketType(ID, P_ChatMessage))
		return false;
	int bufferlength = _string.size();
	if (!SendInt(ID, bufferlength))
		return false;
	int RetnCheck = send(Connections[ID], _string.c_str(), bufferlength, NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Server::GetString(int ID, std::string & _string)
{
	int bufferlength;
	if (!GetInt(ID, bufferlength))
		return false;
	char * buffer = new char[bufferlength + 1];
	buffer[bufferlength] = '\0';
	int RetnCheck = recv(Connections[ID], buffer, bufferlength, NULL);
	_string = buffer;
	delete[] buffer;
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}