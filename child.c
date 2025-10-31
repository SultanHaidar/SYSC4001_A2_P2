#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int value = 0;
    
    printf("Child program, PID = %d\n", getpid());

    
    while (1) {
        printf("Child %d: Current value = %d\n", getpid(), value);
        value--; 
        usleep(350000); 
    }
    
    return 0;
}
