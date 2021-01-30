CC=gcc -std=c11 $(shell cat ./config/CONFIG_CFLAGS)
RM=rm -f
OPTS_RELEASE = -Wall -Werror -pedantic -O2
OPTS_DEBUG = -Wall -Werror -pedantic -g -DTOPAZDC_DEBUG
LIBRARIES = $(shell cat ./config/CONFIG_LIBRARIES)
SRCS = $(shell ls -1 ./src/*) ./config/config.c $(shell cat ./config/CONFIG_CSOURCES)
OBJS = $(SRCS:.c=.o)


release: $(OBJS)
	$(CC) -I./include/ $(OPTS_RELEASE)  $(SRCS) -c
	ar rcs libtopaz.a *.o	


debug:$(OBJS)
	ar rcs libtopaz.a *.o

%.o : %.c
	$(CC) -c $(OPTS_DEBUG) $<  -I./include/ -o $@


test : $(OBJS)
	$(CC) -I./include/ $(OPTS_DEBUG) $(OBJS)  -o libtopaz_test -lm $(LIBRARIES)


clean: 
	$(RM) $(OBJS) *.a
