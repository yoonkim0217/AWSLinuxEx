#include "share.h"

int main()
{
    pid_t pid; 
    int shmid;
    void* shared_memory = NULL;
    long* shared_address = NULL;
    long i;
    long result = 0;
    int status;
    // step1. shmget
    shmid = shmget((key_t)KEY_VALUE, sizeof(long)*SHMSIZE, 0777|IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // step2. shmat
    // (void*)0 == NULL -> 특정 주소값을 원하지 않는다...
    shared_memory = shmat(shmid, (void*)0, 0);
    if (shared_memory == (void*)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }


    shared_address = (long*)shared_memory;

    pid = fork();
    if(pid == -1) { //-1 이면 fork생성 에러
        fprintf(stderr, "can't fork, erro\n");
		exit(0);
	}
    else if(pid == 0) { //0이면 자식 프로세스
        *(shared_address) = 0;
        *(shared_address+1) = 0;
        for (i = 1; i <= 1000000000; i++)
        {
              *(shared_address) += i;
        }
        *(shared_address+1) = 1;
        printf("child result = %ld\n", *(shared_address));

            // step4. shmdt
        if(shmdt(shared_memory)==-1)
        {
            fprintf(stderr, "child shmdt failed\n");
            exit(EXIT_FAILURE);
        }

	} else { //부모프로세스
		// step3. memory access
        
        while(*(shared_address+1) != 1)
        {
            for (i = 1000000001 ; i <= 2000000000; i++)
                result += i;
        }

        printf("parent result = %ld\n", result);

        result += *(shared_address);
        printf("result = %ld\n", result);

        // step4. shmdt
        if(shmdt(shared_memory)==-1)
        {
            fprintf(stderr, "parent shmdt failed\n");
            exit(EXIT_FAILURE);
        }
        wait(&status);

        // step5. shmctl
        if (shmctl(shmid, IPC_RMID, 0) == -1)
        {
            fprintf(stderr, "shmctl failed\n");
            perror("shmctl failed:");
            exit(EXIT_FAILURE);
        }
        
	}
   

    exit(EXIT_SUCCESS);
    
}