#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/wait.h>


struct shared_data {
    int multiple;
    int counter;
};

union semaphore_union {
    int value;
    struct semid_ds *buffer;
    unsigned short *array;
};


void semaphore_operation(int sem_id, short operation) {
    struct sembuf sem_buffer;
    sem_buffer.sem_num = 0;
    sem_buffer.sem_op = operation; 
    sem_buffer.sem_flg = 0;
    
    if (semop(sem_id, &sem_buffer, 1) == -1) {
        printf("Semaphore, operation failed\n");
        exit(1);
    }
}

int main() {
    key_t memory_key = 0x1234;
    int shared_mem_id;
    int semaphore_id;
    struct shared_data *shared_mem;
    
    printf("Parent startingn\n");
    

    shared_mem_id = shmget(memory_key, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shared_mem_id < 0) {
        printf("failed to create shared memory\n");
        return 1;
    }
    
   
    shared_mem = (struct shared_data*) shmat(shared_mem_id, NULL, 0);
    if (shared_mem == (void*)-1) {
        printf("failed the shared memory\n");
        return 1;
    }
    
 
    semaphore_id = semget(memory_key, 1, IPC_CREAT | 0666);
    if (semaphore_id == -1) {
        printf("failed to make semaphore\n");
        return 1;
    }
    

    union semaphore_union sem_arg;
    sem_arg.value = 1;
    if (semctl(semaphore_id, 0, SETVAL, sem_arg) == -1) {
        printf("Failed to initialize\n");
        return 1;
    }
    
    
    semaphore_operation(semaphore_id, -1);  
    shared_mem->multiple = 3;
    shared_mem->counter = 0;
    semaphore_operation(semaphore_id, 1);   
    
    printf("everything Initialized\n");
    
  
    pid_t child_pid = fork();
    
    if (child_pid < 0) {
        printf("failed to create child process\n");
        return 1;
    }
    
    if (child_pid == 0) {

        printf("Loading program\n");
        execl("./part5_child", "part5_child", NULL);
        printf("Failed to load  program\n");
        exit(1);
    } else {
    
        printf("Parent %d: Created child, with PID %d\n", getpid(), child_pid);

        

        while (1) {
       
            semaphore_operation(semaphore_id, -1);  
            shared_mem->counter++;
            int current_count = shared_mem->counter;
            int current_multiple = shared_mem->multiple;
            semaphore_operation(semaphore_id, 1); 
            

            if (current_count % current_multiple == 0) {
                printf("Parent: Count = %d - %d is multiple of %d\n", 
                       current_count, current_count, current_multiple);
            } else {
                printf("Parent: Count = %d\n", current_count);
            }
            
            usleep(150000); 
            

            if (current_count > 500) {
                printf("Parent reached %d, now will stop\n", current_count);
                break;
            }
        }
        

        printf("Parent waiting\n");
        wait(NULL);
        
        
        shmdt(shared_mem);
        shmctl(shared_mem_id, IPC_RMID, NULL);
        semctl(semaphore_id, 0, IPC_RMID);
        
        printf("Parent done!\n");
    }
    
    return 0;
}
