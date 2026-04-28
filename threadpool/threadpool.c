#include "threadpool.h"
#include <pthread.h>


const int NUMBER = 2;
//任务结构体
typedef struct Task
{
    void (*function) (void* arg); //函数指针格式  返回类型 (*指针变量名)(参数列表);
    void* arg;
    
}Task;

//线程池结构体

struct Threadpool
{   
    Task* taskQ;
    int queueCapacity; //容量
    int queueSize; //当前任务个数
    int queueFront;
    int queueRear;

    pthread_t mangerID; //管理的线程ID
    pthread_t* threadIDs; //工作的线程ID

    int minNum;     //最小线程数量
    int maxNum;     //最大线程数量
    int busyNum;    //忙的线程个数
    int liveNum;    //存活的线程个数
    int exitNum;    //要销毁的线程个数

    pthread_mutex_t mutexPool; //锁整个线程池
    pthread_mutex_t mutexBusy; //锁busyNum这个变量

    int shutdown;  //销毁线程池变量,不销毁为0，销毁为1

    pthread_cond_t notFUll;
    pthread_cond_t notEmpty;
};


Threadpool* threadPoolCreate(int min, int max, int queueSize)
{

    Threadpool* Pool = (Threadpool*)malloc(sizeof(Threadpool));
    do
    {
    if (Pool == NULL)
    {
        printf("malloc fail\n");
        break;
    }
    Pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t)*max);
    if (Pool->threadIDs == NULL)
    {
        //这里没有释放前面申请的内存,内存泄露了
        printf("malloc fail\n");
        break;
    }
    memset(Pool->threadIDs,0,sizeof(pthread_t)*max);

    Pool->minNum = min;
    Pool->maxNum = max;
    Pool->busyNum = 0;
    Pool->liveNum = min;
    Pool->exitNum = 0;
    if( pthread_mutex_init(&Pool->mutexPool,NULL) != 0 || //初始化锁与条件变量
        pthread_mutex_init(&Pool->mutexBusy,NULL) != 0 ||
        pthread_cond_init(&Pool->notEmpty,NULL) != 0 ||
        pthread_cond_init(&Pool->notFUll,NULL) != 0 
    )
    {
        printf("fail init mutex and conditon\n");
        break;
    }

    //给任务分配了内存
    Pool->taskQ = (Task*)malloc(sizeof(Task)*queueSize);
    Pool->queueCapacity = queueSize;
    Pool->queueSize = 0;
    Pool->queueFront = 0;
    Pool->queueRear = 0;

    Pool->shutdown = 0;

    //创建线程
    pthread_create(&Pool->mangerID,NULL,manager,Pool);
    for (int i = 0; i < Pool->maxNum; i++)
    {
        pthread_create(Pool->threadIDs+i,NULL,worker,Pool);
    }
    return Pool;
    }
    while(0);

    //释放资源
    if(Pool->threadIDs) free(Pool->threadIDs);
    if(Pool->taskQ) free(Pool->taskQ);
    if(Pool) free(Pool);

    return NULL;
}

void* worker(void* arg)
{
    Threadpool* pool = (Threadpool*)arg;
    while (1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        //没有多余的任务了
        while (pool->queueSize == 0 && !pool->shutdown)
        {
            //阻塞工作线程
            pthread_cond_wait(&pool->notEmpty,&pool->mutexPool);

            //判断是否需要销毁线程
            if (pool->exitNum > 0)
            {
                pool->exitNum--;
                pool->liveNum--;
                pthread_mutex_unlock(&pool->mutexPool);
                threadExit(pool);
            }
            
        }
        if (pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
                threadExit(pool);
        }
        //从任务队列中取出一个任务
        Task task;
        task.function = (pool->taskQ+(pool->queueFront))->function; //从任务队列的头部取数据
        task.arg = pool->taskQ[pool->queueFront].arg;
        //移动queueFront
        pool->queueFront = (pool->queueFront+1) % pool->queueCapacity;

        //取出了一个任务,数量-1
        pool->queueSize--;
        pthread_cond_signal(&pool->notFUll);
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum++;
        pthread_mutex_unlock(&pool->mutexBusy);
        task.function(task.arg);
        free(task.arg);
        task.arg = NULL;

        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum--;
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    
    return NULL;
}

void* manager(void* arg)
{
    Threadpool* pool = (Threadpool*)arg;
    while (!pool->shutdown)
    {
        //每隔3s检测一次
        sleep(3);
        //取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;
        int liveNum = pool->liveNum;
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexPool);

        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

        //添加线程
        if (queueSize > liveNum && liveNum < pool->maxNum)
        {
            int counter = 0;
            for (int i = 0; i < pool->maxNum && counter < NUMBER && pool->liveNum < pool->maxNum; i++)
            {
                if (pool->threadIDs[i] == 0)
                {
                     pthread_create(&pool->threadIDs[i],NULL,worker,pool);
                     counter++;
                     pool->liveNum++;
                }
            }
        }

        //销毁线程
        if (busyNum*2 < liveNum && liveNum > pool->minNum)
        {
            pool->exitNum = NUMBER;
            //让工作的线程自杀
            for (int i = 0; i < NUMBER; i++)
            {
                //唤醒堵塞的线程让其自杀
                pthread_cond_signal(&pool->notEmpty);
            }
            
        }   
    }
    return NULL;
}

void threadExit(Threadpool* pool)
{
    pthread_t tid = pthread_self();
    pthread_mutex_lock(&pool->mutexPool);
    for (int i = 0; i < pool->maxNum; i++)
    {
        if (tid == pool->threadIDs[i])
        {
            pool->threadIDs[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&pool->mutexPool);
    pthread_exit(NULL);
    return NULL;
}

void threadpoolAdd(Threadpool* pool, void (*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    while (pool->queueSize == pool->queueCapacity && !pool->shutdown)
    {
        pthread_cond_wait(&pool->notFUll, &pool->mutexPool);
    }
    if (pool->shutdown)
    {
        pthread_mutex_unlock(&pool->mutexPool);
        return NULL;
    }
    pool->taskQ[pool->queueRear].function = func;
    pool->taskQ[pool->queueRear].arg =arg;
    pool->queueRear = (pool->queueRear+1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_cond_signal(&pool->notEmpty);
}

int workNum(Threadpool* pool)
{
    int worknum = pool->busyNum;

    return workNum;
}

int liveNum(Threadpool* pool)
{
    int livenum = pool->liveNum;

    return liveNum;
}

int threadPoolDestory(Threadpool* pool)
{
    if (pool == NULL)
    {
        return -1;
    }
    //关闭线程池
    pool->shutdown = 1;
    pthread_join(pool->mangerID,NULL);
    for (int i = 0; i < pool->liveNum; i++)
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    //释放内存
    free(pool->taskQ);
    free(pool->threadIDs);
    free(pool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_comd_destroy(&pool->notEmpty);
    pthread_comd_destroy(&pool->notFUll);


    return 0;
}