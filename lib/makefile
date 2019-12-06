CC=gcc
OPTS_RELEASE = -Wall -Werror -pedantic -O2
OPTS_DEBUG = -Wall -Werror -pedantic -g -DTOPAZDC_DEBUG


release:
	$(CC) -I../include/ $(OPTS_RELEASE) ../src/* -c
	ar rcs libtopaz.a *.o	


debug:
	$(CC) -I../include/ $(OPTS_DEBUG) ../src/* -c
	ar rcs libtopaz.a *.o
