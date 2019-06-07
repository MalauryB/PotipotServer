#include <stdio.h>
#include <stdlib.h>
#include "potipot.h"

char * IP = ("127.0.0.1");
#define PORT 5000

void closeSockets(SOCKET* sock, SOCKET* csock){
	/* Ferleture de la socket client et de la socket serveur */
	printf("Fermeture de la socket client\n");
	closesocket(csock);
	printf("Fermeture de la socket serveur \n");
	closesocket(sock);
	printf("Fermeture du serveur terminée\n");
}

void createSocketAndInformation(SOCKET* sock, SOCKADDR_IN* sin, socklen_t recsize){
	int sock_err;

	/*Création socket*/	
	sock(AF_INET, SOCK_STREAM, 0);

	/*Si la socket est valide*/
	if(sock != INVALID_SOCKET)
	{
		printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

		/* Configuration */
		sin.sin_addr.s_addr = inet_addr(IP); /* IP donnée au serveur */
		sin.sin_family = AF_INET; /* Protocole familial (IP); */
		sin.sin_port = htons(PORT); /* Listage du port */		
		sock_err = bind(sock, (SOCKADDR*)&sin, sizeof(sin));
	}
	else
		perror("ERROR - On socket creation.\n");
	return sock_err;
}

void serveurListening(SOCKET* sock, int* sock_err)
{
	/* Si la socket fonctionne */
	if(sock_err != SOCKET_ERROR)
	{
		/* Démarrage du listage (mode serveur) */
		sock_err = listen(sock, 1); /* 1 = nombre maximum de connexion possible */
		printf("Listage du port %d...\n", PORT);
	}
	perror("ERROR - On socket binding.\n");
	return;
}

void waitingForConnection(SOCKET* csock, SOCKADDR_IN* csin, socklen_t crecsize, int sock_err){
	if(sock_err != SOCKET_ERROR)
	{		
		/*Attente pendant laquelle le client se connecte*/
		printf("Patientez pendant que le client se connecte sur le port %d...\n",PORT);
		csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
		printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
	}
	else
		perror("ERROR - On socket listening.");
	return;
}

State analyzePlantHealth(){
	return analyzeWatering();
}

void sendNewState(Socket* csock){
	struct State state = analyzePlantHealth();
	char* buffer[300] = state.code + "-" + state.message;
	
	int sock_err = send(csock,buffer, 300, 0);
	if(sock_err != SOCKET_ERROR)
		printf("State envoyé : %s\n", buffer);
	else
		printf("Erreur de transmission \n");
	return;
}


int main(int argc,char *argv[]){

	/* Socket et contexte d'adressage du serveur */
	SOCKADDR_IN sin;
	SOCKET sock;
	socklen_t recsize = sizeof(sin);

	/* Socket et contexte d'adressage du client */
	SOCKADDR_IN csin;
	SOCKET csock;
	socklen_t crecsize = sizeof(csin);

	int sock_err = createSocketAndInformation(&sock,&sin,&recsize);
	serveurListening(&sock,&sock_err);
	waitingForConnection(&csock,&csin, crecsize,sock_err);

	/* Envoi d'une première analyse à l'aide des dernières données enregistrées. */
	sendNewState(&csock);

	closeSockets(&sock,&csock);
	return EXIT_SUCCESS;
}


