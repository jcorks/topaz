#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(){
    fcntl(0, F_SETFL, fcntl(0, F_GETFL), | O_NONBLOCK);
    return 0;
}
