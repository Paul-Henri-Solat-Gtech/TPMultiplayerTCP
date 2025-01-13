#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// Initialise le dll pour les sockets
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	// Verifie si le socket est bien creer
	// Socket : interface pour echanger des donnees une fois connecter
	SOCKET newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (newSocket == INVALID_SOCKET)
	{
		std::cout << "Erreur creation socket : " << WSAGetLastError() << std::endl;
		return 0;
	}

	// Inatialisation du serveur et parametres
	sockaddr_in server;

	// Adresse IPV4/IPV6 (conversion du protocole de ladresse reseau internet)
	// 172.217.20.174 (Google)
	if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0)
	{
		return false;
	}
	server.sin_family = AF_INET;

	// Port
	server.sin_port = htons(1025);

	// Si le socket et le server sont OK on ce connecte
	if (connect(newSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		std::cout << "Erreur connection : " << WSAGetLastError() << std::endl;
		return 0;
	}
	std::cout << "Socket connecte !" << std::endl;

	// Envoie et recois les donnees envoyer en utiliasant le socket et le server (GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n)
	char buffer[1024];
	std::cout << "Enter text to Send : ";
	std::cin >> buffer;

	if (send(newSocket, buffer, strlen(buffer), 0) == -1)
	{
		std::cout << "Erreur Send : " << WSAGetLastError() << std::endl;
	}
	std::cout << "Sended: " << buffer << " Waiting Response..." << std::endl;

	if (recv(newSocket, buffer, strlen(buffer), 0) <= 0)
	{
		std::cout << "Erreur Send : " << WSAGetLastError() << std::endl;
	}
	std::cout << "Success : " << buffer << std::endl;

	while (connect(newSocket, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR)
	{

	}

	closesocket(newSocket);
	WSACleanup();

	return true;
}
