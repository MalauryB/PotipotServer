#include <winsock.h>
#include <MYSQL/mysql.h>
#include <time.h>

int checkHumidtu(){
	return 0;
}

void saveWatering(int humidity){
	return;
}

void bddConnection(MYSQL * mysql){
	mysql_ini(MYSQL *mysql);
	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP,"option");

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
	mysql_query(&mysql, "SELECT dateTime FROM spray ORDER BY id DESC LIMIT 0,1");
	MYSQL_RES *result;
	MYSQL_ROW row;
	char* date;

	/* Insertion du jeu de résultat dans le pointeur result. */
	result = mysql_use_result(&mysql);

	while(row = mysql_fetch_row(result))
	{
		date = row(0);	
	}
	mysql_free_result(result);
	return date;
}

int getDayBetweenWatering(MYSQL * mysql)
{
	time_t t = time(NULL);
	mysql_query(&mysql, "SELECT dayNumber FROM needwatering WHERE needwatering.seasonId = 1 AND needwatering.temperatureRangeId=1");
	MYSQL_RES *result;
	MYSQL_ROW row;
	int dayBetweenWatering;
	
	/* Insertion du jeu de résultat dans le pointeur result. */
	result = mysql_use_result(&mysql);

	while(row = mysql_fetch_row(result))
	{
		date = atoi(row(0));	
	}
	mysql_free_result(result);
	return dayBetweenWatering;
}

State analyzeWatering(){
	
	/* Variable BDD et option */
	MYSQL mysql;
	bddConnection(&mysql);
	
	/* Récupération du dernier arrosage */
	char* lastDateSpray= getLastSpray(&mysql);
	

	/* Récupération du besoin d'arrosage */
	int dayBetweenWatering = getDayBetweenWatering(&mysql);
	
	mysql_close(&mysql);

	/** Comparaison du dernier arrosage au besoin de la plante **/
	//Conversion date time en t
	int needWatering = 0;

	struct State state;
	if(needWatering==0){
		state.code = 200;
		state.message = "est arrosée";
	}
	else{
		state.code = 500;
		state.message = "n'est pas arrosée";
	}
	return state;
}
