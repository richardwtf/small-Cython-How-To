all:
	cython vector.pyx
	gcc -W -Wall -pedantic -ansi -std=c99 -O3 -fPIC -lm -c whash.c -o whash.o
	gcc -pthread -fno-strict-aliasing -pipe -fomit-frame-pointer -O3 -fPIC -I/usr/include/python2.6 -c vector.c -o vector.o
	gcc -L/usr/lib64 -lpython2.6 -pthread -O3 -fomit-frame-pointer -pipe -shared vector.o whash.o -o vector.so

test:
	gcc -g -W -Wall -pedantic -ansi -std=c99 -lm whash_test.c whash.c -o whash_test
	./whash_test

clean:
	rm -f *.{o,so}
	rm -f vector.c
	rm -f vectors.xml
