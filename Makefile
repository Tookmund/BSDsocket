SOCK = BSDsocket
CC = gcc
FLAGS = -ansi -Wall -Werror
INSTALL = /usr/local/lib

all: test static dynamic

test: test.c $(SOCK).c
	$(CC) $(FLAGS) test.c $(SOCK).c -o test
	
#based on tutorial from http://www.adp-gmbh.ch/cpp/gcc/create_lib.html and http://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

$(SOCK).o: $(SOCK).c
	$(CC) -c $(SOCK).c -fpic -o $(SOCK).o
	
lib$(SOCK).a: $(SOCK).o
	ar rcs lib$(SOCK).a $(SOCK).o
	
lib$(SOCK).so: $(SOCK).o 
	$(CC) -shared -o lib$(SOCK).so $(SOCK).o
	
install: dynamic
	cp lib$(SOCK).so $(INSTALL)
	ldconfig
	
clean:
	rm -rf *.so *.o *.a 

uninstall:
	rm $(INSTALL)/$(SOCK).so
