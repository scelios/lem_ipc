#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/sem.h>
#define SHM_KEY 65
#define SHM_SIZE 1024
#define MAX_PROCESSES 20
#define MSGSZ 128
typedef struct sharedMemory{
	sem_t *sem;
    unsigned short int counter;
	unsigned short int wichToPlay;
	unsigned short int nTeams;
    char message[SHM_SIZE - sizeof(int) - sizeof(int) * MAX_PROCESSES];
} sharedMemory;

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

#define SHM_KEY 65
#define SHM_SIZE 1024
#define MAX_PROCESSES 20
#define MSGSZ 128

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define SHM_KEY_PATH "/tmp/lemipc_key"
#define SHM_KEY_ID 65
void remove_all_ipc() {
    struct shmid_ds shm_info;
    struct semid_ds sem_info;
    struct msqid_ds msg_info;
    int shm_id, sem_id, msg_id;
    key_t key;

    // Remove all shared memory segments
    for (shm_id = shmctl(0, SHM_STAT, &shm_info); shm_id >= 0; shm_id = shmctl(shm_id + 1, SHM_STAT, &shm_info)) {
        if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
            perror("shmctl");
        }
		printf("shmctl");
    }

    // Remove all semaphores
    for (sem_id = semctl(0, 0, SEM_STAT, &sem_info); sem_id >= 0; sem_id = semctl(sem_id + 1, 0, SEM_STAT, &sem_info)) {
        if (semctl(sem_id, 0, IPC_RMID) == -1) {
            perror("semctl");
        }
		printf("semctl");
    }

    // Remove all message queues
    for (msg_id = msgctl(0, MSG_STAT, &msg_info); msg_id >= 0; msg_id = msgctl(msg_id + 1, MSG_STAT, &msg_info)) {
        if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
            perror("msgctl");
        }
		printf("msgctl");
    }
}

key_t keygen()
{
	key_t key;
    if ((key = ftok(SHM_KEY_PATH, SHM_KEY_ID)) == -1) {
        perror("ftok");
        return -1;
    }
    return key;
}
bool getSharedRessources(bool *isFirst, int *shmid, sharedMemory **shmaddr)
{
	// key_t key = keygen();
	// if (key == -1)
	// 	return false;
	*shmid = shmget(65, sizeof(sharedMemory), IPC_CREAT | IPC_EXCL | 0666);
    if (*shmid == -1) {
        // If the segment already exists, get the existing one
        *shmid = shmget(65, sizeof(sharedMemory), 0666);
        if (*shmid == -1) {
            perror("shmget");
            return false;
        }
        printf("Attached to existing shared memory segment\n");
    } else {
        printf("Created new shared memory segment\n");
        *isFirst = 1;
    }
	*shmaddr = (sharedMemory*) shmat(*shmid, NULL, 0);
    if (*shmaddr == (void *) -1) {
        perror("shmat");
        return false;
    }
	return true;
}
int main(int argc, char *argv[])
{
    int shmid;
    sharedMemory *shmaddr;
    int my_order;
    bool is_first_process = false;
	int msqid;
    key_t key;
    // message_buf sbuf;
    size_t buf_length;
	message_buf rbuf;
	sem_t *sem;
    key = 1234;

	// remove_all_ipc();

    // if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
    // 	perror("msgget");
    //     exit(1);
    // }

	sem = sem_open("/shmaddr_sem", O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
    // sbuf.mtype = 1;
    // strcpy(sbuf.mtext, "Message");
    // buf_length = strlen(sbuf.mtext) + 1;

    
    // Create or get the shared memory segment
    /*shmid = shmget(SHM_KEY, sizeof(sharedMemory), IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
        // If the segment already exists, get the existing one
        shmid = shmget(SHM_KEY, sizeof(sharedMemory), 0666);
        if (shmid == -1) {
            perror("shmget");
            exit(1);
        }
        printf("Attached to existing shared memory segment\n");
    } else {
        printf("Created new shared memory segment\n");
        is_first_process = 1;
    }

    // Attach to the shared memory segment
    shmaddr = (sharedMemory *)shmat(shmid, NULL, 0);
    if (shmaddr == (void *) -1) {
        perror("shmat");
        exit(1);
    }*/
   	if (getSharedRessources(&is_first_process, &shmid, &shmaddr) == false)
	{
		perror("getSharedResources");
        exit(EXIT_FAILURE);
    }
	printf("Shared memory segment attached at address %p\n", shmaddr);
	
	sem_wait(sem);
    // If this is the first process, initialize the shared memory
    if (is_first_process) {
		printf("first_process\n");
        shmaddr->counter = 1;
        // shmaddr->order[0] = getpid();
        strcpy(shmaddr->message, "memory");
		// if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
		// 	perror("msgsnd");
		// 	exit(1);
		// } else {
		// 	printf("Message sent: %s\n", sbuf.mtext);
		// }
        my_order = 1;
    } else {
        // Increment the counter and record the order of arrival
		printf("not first process\n");
        my_order = ++shmaddr->counter;
        // shmaddr->order[my_order - 1] = getpid();
		// if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
		// 	perror("msgrcv");
		// 	exit(1);
		// } else {
		// 	printf("Message received: %s\n", rbuf.mtext);
		// }
        // printf("Shared memory content: %s\n", shmaddr->message);
    }
	printf("counter: %d Order: %d\n", shmaddr->counter, my_order);
	sem_post(sem);
    // printf("Process %d arrived as number %d %d\n", getpid(), my_order, shmaddr->counter);

    // Wait until at least 2 instances have attached
	// printf("Waiting for other processes to attach\n");
    while (shmaddr->counter < 2) {
		printf("Waiting for other processes to attach\n");
        sleep(1);
    }

    // Detach from the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Remove the shared memory segment if this is the last process
    if (my_order == 1) {
        printf("Removing shared memory segment\n");
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
		remove_all_ipc();
		sem_close(sem);
		sem_unlink("/shmaddr_sem");
    }
	printf("End of process %d\n", getpid());
    return 0;
}