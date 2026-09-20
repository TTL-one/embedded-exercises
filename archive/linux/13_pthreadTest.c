#include <pthread.h>
#include <unistd.h>
#include <stdio.h>  
#include <semaphore.h>
#include <string.h>

static char buffer[1000];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t sem;

void * thread_function (void * arg){
    while(1){
        sleep(1);
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        printf("recv:%s", buffer);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_t thread;
    if(pthread_create(&thread, &attr,thread_function, NULL)){
        return -1;
    }
    sem_init(&sem, 0, 1);
#if 0
    while(1){
        sleep(1);
        pthread_mutex_lock(&mutex);
        fgets(buffer ,1000, stdin);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem);

    }
#else
    char temp[1000];
    while(1){
        sleep(1);
        fgets(temp,1000, stdin);
        pthread_mutex_lock(&mutex);
        memcpy(buffer, temp, 1000);
        pthread_mutex_unlock(&mutex);
        sem_post(&sem);

    }
#endif
    return 0;
}

/*
**有区别，而且这个区别会导致死锁。**
---
## 核心问题：mutex 锁的时机
| | Version 1 (`#if 0`) | Version 2 (`#else`) |
|---|---|---|
| `fgets` 位置 | **在 mutex 锁里面**（阻塞式等待输入时仍持有锁） | 在 mutex 外面 |
| `memcpy` 位置 | — | **在 mutex 里面** |
---
## Version 1 为什么有问题
```c
pthread_mutex_lock(&mutex);
fgets(buffer, 1000, stdin);   // ← 这里阻塞等输入，但一直拿着锁！
pthread_mutex_unlock(&mutex);
sem_post(&sem);
```
当你调用 `fgets` 等待键盘输入时，**mutex 锁一直被 main 持有**。
与此同时，子线程的 `sem_wait` 被 `sem_post` 唤醒后，想要：
```c
pthread_mutex_lock(&mutex);   // ← 想拿锁，但 main 还没释放！
printf("recv:%s", buffer);
```
**子线程会被卡住**，等 main 释放锁。
这时候如果你在键盘输入，main 收到信号、unlock、post，**子线程才能打印**。功能上可能"看起来能用"，但存在**锁竞争导致的不确定性**，而且逻辑上是错误的——`fgets` 属于 I/O 操作，不应该放在 critical section 里。
---
## Version 2 为什么正确
```c
fgets(temp, 1000, stdin);     // ← 没有锁，可以自由等待输入
pthread_mutex_lock(&mutex);
memcpy(buffer, temp, 1000);   // ← 只需要对 buffer 赋值这一瞬间加锁
pthread_mutex_unlock(&mutex);
sem_post(&sem);
```
`fgets` 在锁外面，**不会阻塞其他线程**。mutex 只保护 `memcpy` 那几毫秒的赋值操作。
---
## 总结
- **Version 1**：把 `fgets`（慢速 I/O）包在锁里，**增加了死锁和等待的风险**，而且 `fgets` 本身不需要锁保护。
- **Version 2**：**正确做法**——锁只保护对共享缓冲区的写入，`fgets` 和 `sem_post` 都不需要持有锁。
**原则：critical section 要尽可能小，锁的范围只覆盖真正需要互斥的操作。**
*/