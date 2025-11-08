#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>


struct shared_data {
    int multiple;
    int counter;
};

int main() {
    key_t memory_key = 0x1234;
    int shared_mem_id;
    struct shared_data *shared_mem;
    
    printf("Parent starting\n");
    
    shared_mem_id = shmget(memory_key, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shared_mem_id < 0) {
        printf("failed to create a shared memory\n");
        return 1;
    }
    
 
    shared_mem = (struct shared_data*) shmat(shared_mem_id, NULL, 0);
    if (shared_mem == (void*)-1) {
        printf("Failed\n");
        return 1;
    }
    

    shared_mem->multiple = 3;
    shared_mem->counter = 0;
    
    printf("Shared memory: multiple=%d, counter=%d\n", 
           shared_mem->multiple, shared_mem->counter);
    

    pid_t child_pid = fork();
    
    if (child_pid < 0) {
        printf("Failed to create child process\n");
        return 1;
    }
    
    if (child_pid == 0) {
   
        printf("loading child program\n");
        execl("./part4_child", "part4_child", NULL);
        printf("Failed to load child\n");
        exit(1);
    } else {

        printf("Parent %d now child with PID %d\n", getpid(), child_pid);
        printf("Parent counting up \n");
        

        while (1) {
            shared_mem->counter++;
            int current_count = shared_mem->counter;
            int current_multiple = shared_mem->multiple;
            
            if (current_count % current_multiple == 0) {
                printf("Paren ;count = %d - %d is multiple of %d\n", 
                       current_count, current_count, current_multiple);
            } else {
                printf("Parent Count = %d\n", current_count);
            }
            
            usleep(150000); 
            

            if (current_count > 500) {
                printf("Parent reached %d, stopping now\n", current_count);
                break;
            }
        }
        

        printf("Parent waiting for child\n");
        wait(NULL);
        
        shmdt(shared_mem);
        shmctl(shared_mem_id, IPC_RMID, NULL);
        printf("everthing is done\n");
    }
    
    return 0;
}
