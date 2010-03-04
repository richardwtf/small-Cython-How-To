all:
	cython vector.pyx
	gcc -W -Wall -pedantic -ansi -std=c99 -O3 -fPIC -lm -c whash.c -o whash.o
	gcc -W -pthread -fno-strict-aliasing -pipe -fomit-frame-pointer -O3 -DNDEBUG -fPIC -I/usr/include/python2.6 -c vector.c -o vector.o
	gcc -pthread -O3 -fomit-frame-pointer -pipe -shared vector.o whash.o -L/usr/lib64 -lpython2.6 -o vector.so

test:
	gcc -g -W -Wall -pedantic -ansi -std=c99 -lm whash_test.c whash.c -o whash_test
	./whash_test

clean:
	rm *.{o,so}
	rm vector.c
