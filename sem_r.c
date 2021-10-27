#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>



#define N 100

struct shareData
{
    sem_t sem1;
    sem_t sem2; //２个信号量实现同步操作
    int arr[N];
};
int main()
{
    int pid;
    sem_t *psem1, *psem2;

    /*IPC_CREAT表示在key标识的共享内存不存在时，创建共享内存*/
    int shmid = shmget((key_t)1234, sizeof(struct shareData), 0666 | IPC_CREAT);
    /*调用成功时返回一个指向共享内存第一个字节的指针*/
    struct shareData *shareData = (struct shareData *)shmat(shmid, 0, 0);
    /*初始化信号量，第一个１表示信号量可以在进程之间共享，第二个１表示只能同时执行一个进程*/
    psem1 = sem_open("test1",O_CREAT,0644,0);
    psem2 = sem_open("test2",O_CREAT,0644,1);
    if(psem1 == SEM_FAILED)
       {
         printf("unable to create semaphore");
         //sem_unlink("yangchao");
         //exit(-1);
     }
    //创建进程
    //sem_init(&(shareData->sem1), 1, 0); 
    //sem_init(&(shareData->sem2), 1, 0);
    
    
    for (int i = 0; i < N; i++)
    {
        sem_wait(psem1);
        printf("Output:%d\n", shareData->arr[i]); //子进程读出数据
        sem_post(psem2);
    }
    

    sem_close(psem1);
    sem_close(psem2);
    sem_unlink("test1");
    sem_unlink("test2");
    //sem_destroy(&(shareData->sem1));
    //sem_destroy(&(shareData->sem2)); //删除信号量
    shmdt(shareData);
    shmctl(shmid, IPC_RMID, 0);      //删除共享内存
    return 0;
}


