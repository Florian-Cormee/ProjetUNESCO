unesco : projectUnesco.o algorithm.o  utils.o site.o lectureFichiers.o haversine.o
	gcc unesco.o algorithm.o  utils.o site.o lectureFichiers.o haversine.o -o unesco -lm 

main : main.o algorithm.o site.o utils.o lectureFichiers.o haversine.o
	gcc main.o algorithm.o site.o utils.o lectureFichiers.o haversine.o -o main -lm 

projectUnesco.o : projectUnesco.c utils.h site.h algorithm.h
	gcc -c projectUnesco.c 

main.o : main.c algorithm.h site.h utils.h
	gcc -c main.c 

algorithm.o : algorithm.c algorithm.h site.h utils.h
	gcc -c algorithm.c 

testh : test_haversine.o site.o haversine.o utils.o lectureFichiers.o
	gcc test_haversine.o site.o haversine.o utils.o lectureFichiers.o -o testh -lm 

test_harversine.o : test_haversine.c haversine.h utils.h site.h
	gcc -c test_haversine.c 

haversine.o : haversine.c haversine.h
	gcc -c haversine.c

utils.o : utils.c utils.h site.h
	gcc -c utils.c 

site.o : site.c site.h lectureFichiers.h lectureFichiers.o
	gcc -c site.c 

lectureFichiers.o : lectureFichiers.c lectureFichiers.h
	gcc -c lectureFichiers.c 

clean :
	rm -f *.o testh main unesco
