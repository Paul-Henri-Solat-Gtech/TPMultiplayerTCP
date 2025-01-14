#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// Initialise le dll pour les sockets
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	// Initialisation addresse du server (local)
	sockaddr_in adresseLocal;
	adresseLocal.sin_addr.s_addr = INADDR_ANY; // Indique que toutes les sources seront acceptees
	adresseLocal.sin_port = htons(1025); // Toujours traduire le port en reseau
	adresseLocal.sin_family = AF_INET; // Notre socket est TCP
	
	// Initialisation Socket (server)
	SOCKET newServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (newServer == INVALID_SOCKET)
	{
		std::cout << "Erreur creation socket : " << WSAGetLastError() << std::endl;
		return 0;
	}

	// Assigne adresse local au socket
	int res = bind(newServer, (sockaddr*)&adresseLocal, sizeof(adresseLocal));
	
	// Verifie si lassociation a fonctionner
	if (res != 0) {
		std::cout << "Erreur initialisation bind" << std::endl;
		return false;
	}
	std::cout << "Serveur initialise et bind reussi !" << std::endl;

	// Le serveur passe en mode "ecoute" et attend un connection entrante
	res = listen(newServer, SOMAXCONN);
	if (res != 0) 
	{
		std::cout << "Impossible de verifier les connections entrantes" << std::endl;
	}
	std::cout << "En attente de connection entrante..." << std::endl;

	// Connecte le client au serveur
	sockaddr_in addr = { 0 };
	int len = sizeof(addr);
	SOCKET newClient = accept(newServer, (sockaddr*)&addr, &len);
	if (newClient == INVALID_SOCKET) 
	{
		std::cout << "Error" << std::endl;
	}

	// Recuperation de lip du client et conversion en char
	if (newClient != INVALID_SOCKET)
	{
		char buff[INET6_ADDRSTRLEN] = { 0 };
		std::string clientAddress = inet_ntop(addr.sin_family, (void*)&(addr.sin_addr), buff, INET6_ADDRSTRLEN);
		std::cout << "Connexion de " << clientAddress.c_str() << ":" << addr.sin_port << std::endl;
	}

	// Boucle pour recuperer les datas du client
	while (newClient != INVALID_SOCKET)
	{
		char data[1024];
		int nb = recv(newClient, data, 1024, 0);
		data[nb] = 0;
		std::cout << data << std::endl;
		/*if (nb > 0) {
			send(newClient, data, strlen(data), 0);
		}*/
	}


	closesocket(newServer);
	WSACleanup();

	return 0;
}
