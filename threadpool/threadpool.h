
typedef struct Threadpool Threadpool;
//创建线程池

Threadpool* threadPoolCreate(int min, int max, int queueSize);

//销毁线程池
int threadPoolDestory(Threadpool* pool);

//给线程池添加任务
void threadpoolAdd(Threadpool* pool, void (*func)(void*), void* arg);

//当前干活线程数
int workNum(Threadpool* pool);

//活着的线程个数
int liveNum(Threadpool* pool);

void* worker(void* arg);
void* manager(void* arg);
void threadExit(Threadpool* pool);