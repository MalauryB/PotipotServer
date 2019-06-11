#ifndef H_Watering
#define H_Watering
#include <mysql/mysql.h>

/** Cherche le pourcentage d'humidité sur le capteur d'humidité. Cette valeur sera tester toute les 10 secondes sur notre maquette.**/
int searchHumidity();

/** Analyse le pourcentage d'humidité pour d'étecter ou non un arrosage. L'enregistre ci celui-ci est reconnu.**/
int checkHumidity();


/**Enregistre un nouvel arrosage dans la base de données.**/
void saveWatering(MYSQL*,int);

/**Recherche le dernier arrosage et le compare à la current date et aux besoins de la plante pour savoir si la plante manque ou non d'eau. **/
int analyzeWatering();

#endif
