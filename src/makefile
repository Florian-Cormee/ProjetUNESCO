testh : test_haversine.o site.o haversine.o utils.o 
	gcc test_haversine.o site.o haversine.o utils.o -o testh -lm

test_harversine.o : test_haversine.c haversine.h utils.h site.h
	gcc -c test_haversine.c

haversine.o : haversine.c haversine.h
	gcc -c haversine.c
	
utils.o : utils.c utils.h site.h
	gcc -c utils.c

site.o : site.c site.h
	gcc -c site.c

clean :
	rm -f *.o testh
