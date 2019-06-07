watering.o: watering.c watering.h
	gcc -o watering.o -c watering.c -lwsock

potipot.o: potipot.c potipot.h watering.o
	gcc -o potipot.o -c potipot.c

potipot : potipot.o 
	gcc -o potipot potipot.o




