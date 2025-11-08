#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>


struct shared_data {
    int multiple;
    int counter;
};


void semaphore_operation(int sem_id, short operation) {
    struct sembuf sem_buffer;
    sem_buffer.sem_num = 0;
    sem_buffer.sem_op = operation;  
    sem_buffer.sem_flg = 0;
    
    if (semop(sem_id, &sem_buffer, 1) == -1) {
        printf("operation failed\n");
        exit(1);
    }
}

int main() {
    key_t memory_key = 0x1234;
    int shared_mem_id;
    int semaphore_id;
    struct shared_data *shared_mem;
    
    printf("Child program is starting ; PID = %d\n", getpid());

    shared_mem_id = shmget(memory_key, sizeof(struct shared_data), 0666);
    if (shared_mem_id < 0) {
        printf("Couldn't find share memory\n");
        return 1;
    }
    
    shared_mem = (struct shared_data*) shmat(shared_mem_id, NULL, 0);
    if (shared_mem == (void*)-1) {
        printf("Failed\n");
        return 1;
    }
    

    semaphore_id = semget(memory_key, 1, 0666);
    if (semaphore_id == -1) {
        printf("Couldn't find semaphore\n");
        return 1;
    }
    
    
    printf("Child waiting for counter \n");
    

    while (1) {
        semaphore_operation(semaphore_id, -1); 
        int current_counter = shared_mem->counter;
        semaphore_operation(semaphore_id, 1);   
        
        if (current_counter > 100) {
            break;
        }
        usleep(100000); 
    }
    
    printf("Starting to monitor\n");
    

    while (1) {

        semaphore_operation(semaphore_id, -1);
        int current_counter = shared_mem->counter;
        int current_multiple = shared_mem->multiple;
        semaphore_operation(semaphore_id, 1);
        
        printf("Child ,multiple = %d, Counter = %d\n", 
               current_multiple, current_counter);
        
        usleep(200000); 

        if (current_counter > 500) {
            printf(" child counter reached %d, \n", current_counter);
            break;
        }
    }
    

    shmdt(shared_mem);
    printf("disconnection from shared memory\n");
    
    return 0;
}
