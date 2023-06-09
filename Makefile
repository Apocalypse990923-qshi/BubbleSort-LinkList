#
# This is the Makefile that can be used to create the "warmup1" executable
# To create "warmup1" executable, do:
#	make warmup1
#

warmup1: warmup1.o my402list.o myfunction.o
	gcc -o warmup1 -g warmup1.o my402list.o myfunction.o

warmup1.o: warmup1.c my402list.h myfunction.h
	gcc -g -c -Wall warmup1.c

myfunction.o: myfunction.c my402list.h myfunction.h
	gcc -g -c -Wall myfunction.c

my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c

clean:
	rm -f *.o warmup1

