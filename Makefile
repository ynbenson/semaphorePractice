
all: Load Query Clean Print Change
	
Load: header.o Load.o
	gcc -o $@ $^ 
Query: header.o Query.o
	gcc -o $@ $^ 
Clean: header.o Clean.o
	gcc -o $@ $^ 
Change: header.o Change.o
	gcc -o $@ $^ 
Print: header.o Print.o
	gcc -o $@ $^ 
header.o: header.c
	gcc -o $@ -c $^
Load.o:	Load.c
	gcc -o $@ -c $^
Query.o: Query.c
	gcc -o $@ -c $^
Clean.o: Clean.c
	gcc -o $@ -c $^
Change.o: Change.c
	gcc -o $@ -c $^
Print.o: Print.c
	gcc -o $@ -c $^
.PHONY:clean

clean:;		rm -f *.o Load Change Print Query Clean 
