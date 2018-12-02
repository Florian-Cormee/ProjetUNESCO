unesco : projectUnesco.o algorithm.o site.o utils.o lectureFichiers.o haversine.o
	gcc projectUnesco.o algorithm.o site.o utils.o lectureFichiers.o haversine.o -o unesco -lm

projectUnesco.o : projectUnesco.c algorithm.h site.h utils.h
	gcc -c projectUnesco.c

algorithm.o : algorithm.c algorithm.h site.h utils.h
	gcc -c algorithm.c 

haversine.o : haversine.c haversine.h
	gcc -c haversine.c

utils.o : utils.c utils.h site.h
	gcc -c utils.c 

site.o : site.c site.h lectureFichiers.h lectureFichiers.o
	gcc -c site.c 

lectureFichiers.o : lectureFichiers.c lectureFichiers.h
	gcc -c lectureFichiers.c 

clean :
	rm -f *.o unesco
