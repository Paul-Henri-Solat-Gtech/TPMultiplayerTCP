#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

void SendMsgToServer(SOCKET& mySocket, sockaddr_in& addrDestinataire)
{
	std::string text;
	std::cout << "Enter text to Send : ";
	std::cin >> text;
	const char* msg = text.c_str();

	int ret = sendto(mySocket, msg, strlen(msg), 0, reinterpret_cast<const sockaddr*>(&addrDestinataire), sizeof(addrDestinataire));
	if (ret < 0)
	{
		std::cout << "Impossible denvoyer : " << WSAGetLastError() << std::endl;
		return;
	}
	std::cout << "Envoie terminer ! " << std::endl;
}

int main()
{
	// Initialise le dll pour les sockets
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	// Verifie si le socket est bien creer
	// Socket : interface pour echanger des donnees une fois connecter
	SOCKET newSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (newSocket == INVALID_SOCKET)
	{
		std::cout << "Erreur creation socket : " << WSAGetLastError() << std::endl;
		return 1;
	}

	// Inatialisation du serveur et parametres
	//sockaddr_in newAddress;
	//newAddress.sin_addr.s_addr = INADDR_ANY;
	//newAddress.sin_port = htons(1025);
	//newAddress.sin_family = AF_INET;

	/*newSocket est le socket depuis lequel envoyer les donnees.
	buffer est le tampon de donnees à envoyer.
	len est la taille du tampon en octets.
	flags permet de specifier des options pour cet envoi, généralement 0 pour aucune option particuliere.
	addrDestinataire est ladresse du destinataire.
	fromlen est la taille de la structure de ladresse du destinataire.*/
	
	// Envoie
	sockaddr_in addrDestinataire;
	addrDestinataire.sin_port = htons(1025);
	addrDestinataire.sin_family = AF_INET;
	if (inet_pton(AF_INET, "127.0.0.1", &addrDestinataire.sin_addr) <= 0)
	{
		// impossible de déterminer l'adresse
		std::cout << "Erreur addresse destinataire : " << WSAGetLastError() << std::endl;
		closesocket(newSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "Connecte a ladresse ! " << std::endl;
	
	// Envoie data
	SendMsgToServer(newSocket, addrDestinataire);

	// Recus
	while (true) 
	{
		char buffer[1024];
		sockaddr_in from;
		int fromlen = sizeof(from);
		int ret = recvfrom(newSocket, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr*>(&from), &fromlen);
		if (ret <= 0)
		{
			std::cout << "Impossible de recevoir : " << WSAGetLastError() << std::endl;
			return 1;
		}
		buffer[ret] = '\0';
		std::cout << "Message de Server : " << buffer << std::endl;
	}

	// Fermeture du socket et nettoyage des ressources
	closesocket(newSocket);
	WSACleanup();

	return 0;
}
