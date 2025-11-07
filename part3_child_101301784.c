#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int count = 0;
    
    printf("Child program will start;PID is %d\n", getpid());

    
    while (1) {
        printf("Child %d: current value = %d\n", getpid(), count);
        count--;
        

        usleep(200000);
        
      
        if (count < -500) {
            printf("Child %d: is done, the final count = %d\n", getpid(), count);
            break;
        }
    }
    
    printf("Child process finish\n");
    return 0;
}
