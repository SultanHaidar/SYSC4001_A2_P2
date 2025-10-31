#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;
    int count = 0;
    

    child_pid = fork();
    
    if (child_pid < 0) {
        printf("Error\n");
        return 1;
    }
    
    if (child_pid == 0) {

        printf(" PID is %d.\n", getpid());
        

        if (execl("./child", "child", NULL) == -1) {
            printf("Failed to run\n");
            return 1;
        }
    } else {

        printf("Parent process %d created child with PID %d\n", getpid(), child_pid);
        
   
        while (1) {
            if (count % 3 == 0) {
                printf("Parent %d: Count = %d (multiple of 3)\n", getpid(), count);
            } else {
                printf("Parent %d: Count = %d\n", getpid(), count);
            }
            count++;
            usleep(250000); 
        }
    }
    
    return 0;
}
