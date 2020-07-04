all: ideas.exe

ideas.exe: ideas.o
	gcc -o ideas.exe *.o

ideas.o: clean
	gcc -c *.c

clean:
	rm -f *.o ideas.exe