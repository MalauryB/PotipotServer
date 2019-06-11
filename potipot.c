#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

char * IP = ("127.0.0.1");
#define PORT 5000
int erreur = 0;

void sendNewState(SOCKET csock){
	int code = analyzeWatering();
	char* buffer = code;
	
	int sock_err = send(csock,buffer, 300, 0);
	if(sock_err != SOCKET_ERROR)
		printf("State envoyé : %s\n", buffer);
	else
		printf("Erreur de transmission \n");
	return;
}

int main(int argc, char* argv[]){
	/* Socket et contexte d'adressage du serveur */
	SOCKADDR_IN sin;
	SOCKET sock;
	socklen_t recsize = sizeof(sin);
	    
	/* Socket et contexte d'adressage du client */
	SOCKADDR_IN csin;
	SOCKET csock;
	socklen_t crecsize = sizeof(csin);
	    
	int sock_err;
	char buffer[300];
	    
	    
	if(!erreur)
	{
		/* Création d'une socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);
		
		/* Si la socket est valide */
		if(sock != INVALID_SOCKET)
		{
		    printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);
		    
		    /* Configuration */
		    sin.sin_addr.s_addr = inet_addr(IP);  /* Adresse IP automatique */
		    sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
		    sin.sin_port = htons(PORT);               /* Listage du port */
		    sock_err = bind(sock, (SOCKADDR*)&sin, recsize);
		    
		    /* Si la socket fonctionne */
		    if(sock_err != SOCKET_ERROR)
		    {
		        /* Démarrage du listage (mode server) */
		        sock_err = listen(sock, 5);
		        printf("Listage du port %d...\n", PORT);
		        
		        /* Si la socket fonctionne */
		        if(sock_err != SOCKET_ERROR)
		        {
		            /* Attente pendant laquelle le client se connecte */
		            printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
		            csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
		            printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
			    while(recv(sock, buffer, 300, 0) != SOCKET_ERROR)
				printf("Reçu : %s\n", buffer);
			    sendNewState(csock);
		        }
		        else
		            perror("listen");
		    }
		    else
		        perror("bind");
		    
		    /* Fermeture de la socket client et de la socket serveur */
		    printf("Fermeture de la socket client\n");
		    closesocket(csock);
		    printf("Fermeture de la socket serveur\n");
		    closesocket(sock);
		    printf("Fermeture du serveur terminée\n");
		}
	else
		perror("socket");
	}
	    
	    return EXIT_SUCCESS;
}

void saveWatering(MYSQL * mysql, int humidity){
	mysql_query(mysql,"INSERT INTO spray (degree, humidity) values (20, 500)");
	mysql_affected_rows(mysql);
	return;
}

void bddConnection(MYSQL * mysql){
	mysql_init(mysql);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP,"option");

	/* Connexion à la bdd */
	if(mysql_real_connect(mysql,
		"127.0.0.1", /* Nom de domaine ou @hebérgeur */
		"root", /* Identifiant de connexion */
		"root", /* mot de passe */
		"potipot", /* nom de la bdd */
		3306, /* port */
		NULL, /* Socket utilisé, ici NULL */
		0 /* flag */))
	{
	}
	else
		perror("ERROR- BDD connection");
	return;
}

char* getDateLastSpray(MYSQL * mysql){
	mysql_query(mysql, "SELECT dateTime FROM spray ORDER BY id DESC LIMIT 0,1");
	MYSQL_RES *result;
	MYSQL_ROW row;
	char* date;

	/* Insertion du jeu de résultat dans le pointeur result. */
	result = mysql_use_result(mysql);

	while(row = mysql_fetch_row(result))
	{
		date = row[0];	
	}
	mysql_free_result(result);
	return date;
}

int getDayBetweenWatering(MYSQL * mysql)
{
	mysql_query(mysql, "SELECT dayNumber FROM needwatering WHERE needwatering.seasonId = 1 AND needwatering.temperatureRangeId=1");
	MYSQL_RES *result;
	MYSQL_ROW row;
	int dayBetweenWatering;
	
	/* Insertion du jeu de résultat dans le pointeur result. */
	result = mysql_use_result(mysql);

	while(row = mysql_fetch_row(result))
	{
		dayBetweenWatering = atoi(row[0]);	
	}
	mysql_free_result(result);
	return dayBetweenWatering;
}

int analyzeWatering(){
	
	/* Variable BDD et option */
	MYSQL mysql;
	bddConnection(&mysql);
	
	/* Récupération du dernier arrosage */
	char* lastDateSpray= getDateLastSpray(&mysql);
	

	/* Récupération du besoin d'arrosage */
	int dayBetweenWatering = getDayBetweenWatering(&mysql);
	
	mysql_close(&mysql);

	/** Comparaison du dernier arrosage au besoin de la plante **/
	//Conversion date time en
	struct tm * t;
	t->tm_mon = lastDateSpray[5] + lastDateSpray[6];
	t->tm_mday = lastDateSpray[8] + lastDateSpray[9];
	t->tm_year = lastDateSpray[0] + lastDateSpray[1] + lastDateSpray[2] + lastDateSpray[3];
	time_t time1 = mktime(t);
	time_t time2 = time(NULL);
	double seconds = difftime(time1, time2);
	int needWatering = seconds/60/60/24;

	if(needWatering<dayBetweenWatering){
		return 701;
	}
	else{
		return 702;
	}
}



