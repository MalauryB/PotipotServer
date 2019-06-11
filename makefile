watering.o: watering.c
	gcc -o watering.o -c watering.c -lwsock

potipot.o: potipot.c watering.o 
	gcc -o potipot.o -c potipot.c

potipot : potipot.o 
	gcc -o potipot potipot.o




