test : test_haversine.o haversine.o
	gcc test_haversine.o haversine.o -o test_haversine -lm
	./test_haversine

test_harversine.o : test_haversine.c haversine.h
	gcc -c test_haversine.c

haversine.o : haversine.c haversine.h
	gcc -c haversine.c

clean :
	rm -f *.o test_haversine