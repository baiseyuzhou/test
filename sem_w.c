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
    sem_t sem2; //２个信号量实现同步操作,需要link　pthread
    int arr[N];
};
int main()
{
    int pid;
    sem_t *psem1, *psem2;

    /*IPC_CREAT表示在key标识的共享内存不存在时，创建共享内存*/
    int shmid = shmget((key_t)1234, sizeof(struct shareData), 0666 | IPC_CREAT);
    /*调用成功时返回一个指向共享内存第一个字节的指针*/
    void *shm = shmat(shmid, 0, 0);
    struct shareData *shareData = (struct shareData *)shm;
    /*初始化信号量，第一个１表示信号量可以在进程之间共享，第二个１表示只能同时执行一个进程*/
    psem1 = sem_open("test1",0);
    psem2 = sem_open("test2",0);
    
   if(psem1 == SEM_FAILED)
   {
     printf("unable to create semaphore");
   }
    
    
    for (int i = 0; i < N; i++)
    {
        sem_wait(psem2);
        shareData->arr[i] = i + 1; //父进程写入数据
        printf("Parent Input:%d\n", shareData->arr[i]);
        sleep(1);
        sem_post(psem1); //释放
    }
    
    sleep(10);
   
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

