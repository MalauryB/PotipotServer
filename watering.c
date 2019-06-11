#include <mysql/mysql.h>
#include <time.h>
#include <stdio.h>

int checkHumidty(){
	return 0;
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
