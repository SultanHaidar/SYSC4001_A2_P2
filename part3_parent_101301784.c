#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t child_pid;
    int child_status;
    
    printf("Parent start\n");
    
   
    child_pid = fork();
    
    if (child_pid < 0) {
        printf("Couldn't create child process\n");
        return 1;
    }
    
    if (child_pid == 0) {

        printf("Loading the child program\n");
        

        if (execl("./part3_child", "part3_child", NULL) == -1) {
            printf("Failed to load program\n");
            exit(1);
        }
    } else {
      
        printf("Parent %d: create child with PID %d\n", getpid(), child_pid);
        printf("Parent waiting\n");
        

        waitpid(child_pid, &child_status, 0);
        
        printf("Child finished with status %d\n", child_status);
        printf("everything done, parent will \n");
    }
    
    return 0;
}

    
    
