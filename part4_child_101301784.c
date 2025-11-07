#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


struct shared_data {
    int multiple;
    int counter;
};

int main() {
    key_t memory_key = 0x1234;
    int shared_mem_id;
    struct shared_data *shared_mem;
    
    printf("Child prog is starting, PID = %d\n", getpid());
    

    shared_mem_id = shmget(memory_key, sizeof(struct shared_data), 0666);
    if (shared_mem_id < 0) {
        printf("couldnt find shared memory\n");
        return 1;
    }
    

    shared_mem = (struct shared_data*) shmat(shared_mem_id, NULL, 0);
    if (shared_mem == (void*)-1) {
        printf("failed to connect to shared memory\n");
        return 1;
    }
    
    printf("connected to shared memory\n");
    

    while (shared_mem->counter <= 100) {
        usleep(100000); 
    }
    
    printf("Counter is now %d \n", shared_mem->counter);
    

    while (1) {
        printf("Child: Multiple = %d, Counter = %d\n", 
               shared_mem->multiple, shared_mem->counter);
        usleep(200000); 
        

        if (shared_mem->counter > 500) {
            printf("Child: Counter reached %d, so exit\n", shared_mem->counter);
            break;
        }
    }
    

    shmdt(shared_mem);
    printf("everything is done\n");
    
    return 0;
}
