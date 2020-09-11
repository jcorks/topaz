#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

int main(){
    struct timespec ctime;
    clock_gettime(CLOCK_REALTIME, &ctime);
}