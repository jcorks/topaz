#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int main(){
    chdir((char*)0x0);
    mkdir("not windows mkdir", 0777);
}
