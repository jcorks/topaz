CC=gcc -std=c11 $(shell cat ./config/CONFIG_CFLAGS)
RM=rm
OPTS_RELEASE = -Wall -Werror -pedantic -O2
OPTS_DEBUG = -Wall -Werror -pedantic -g -DTOPAZDC_DEBUG
LIBRARIES = $(shell cat ./config/CONFIG_LIBRARIES)
SRCS = $(shell cat ./config/CONFIG_CSOURCES)


release:
	$(CC) -I./include/ $(OPTS_RELEASE) ./src/* $(SRCS) -c
	ar rcs libtopaz.a *.o	


debug:
	$(CC) -I./include/ $(OPTS_DEBUG) ./src/* $(SRCS) -c
	ar rcs libtopaz.a *.o


test:
	$(CC) -I./include/ $(OPTS_DEBUG) ./src/* $(SRCS) ./tests/* -o libtopaz_test -lm $(LIBRARIES)


clean: 
	$(RM) *.o *.a
