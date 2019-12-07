CC=gcc
RM=rm
OPTS_RELEASE = -Wall -Werror -pedantic -O2
OPTS_DEBUG = -Wall -Werror -pedantic -g -DTOPAZDC_DEBUG -fsanitize=address -fsanitize=undefined


release:
	$(CC) -I../include/ $(OPTS_RELEASE) ../src/* -c
	ar rcs libtopaz.a *.o	


debug:
	$(CC) -I../include/ $(OPTS_DEBUG) ../src/* -c
	ar rcs libtopaz.a *.o


test:
	$(CC) -I../include/ $(OPTS_DEBUG) ../src/* ../tests/* -o libtopaz_test


clean: 
	$(RM) *.o *.a
