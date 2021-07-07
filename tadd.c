#include "tadd.h"

void * add_function(void * data)
{
    long result = 0;
    long i;
    struct Calc* ptr = (struct Calc*)data;

    for (i = ptr->startVal; i <= ptr->endVal; i++)
        result += i;

    ptr->result = result;
    
    return (void*)data;
}

int main()
{
    struct Calc *cptr1, *cptr2;
    pthread_t p_thread[2];
    int err;
    int status;


    // 구조체 생성
    cptr1 = malloc(sizeof(struct Calc));
    cptr2 = malloc(sizeof(struct Calc));
    
    // 구조체 값 초기화
    cptr1->startVal = 1;
    cptr1->endVal = END_VALUE/2;

    cptr1->result = 0;

    cptr2->startVal = END_VALUE/2+1;
    cptr2->endVal = END_VALUE;

    cptr2->result = 0;


    err = pthread_create(&p_thread[0], NULL, add_function, cptr1);
    if (err != 0)
    {
        perror("pthread_create(0) failed\n");
        exit(1);
    }

    err = pthread_create(&p_thread[1], NULL, add_function, cptr2);
    if (err != 0)
    {
        perror("pthread_create(1) failed\n");
        exit(1);
    }

    pthread_join(p_thread[0], (void**)&status);
    pthread_join(p_thread[1], (void**)&status);
 

    printf("1~중간값 sum 결과: %ld\n", cptr1->result);
    printf("중간값+1~끝값 sum 결과: %ld\n", cptr2->result);

    printf("Total sum : %ld\n", cptr1->result + cptr2->result);

    return 0;
}