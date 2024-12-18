/**
@file
@brief Pthread simple win32 compatible with MSVC and stdatomic
if you have code that use pthread & stdatomic,
and try to use pthreads4w-code-v3.0.0 on MSVC,
errors. MSVC stdatomic is compatible with
pthreads4w also known as pthreads-win32

~~~
pthread_t output_thread;
pthread_create(&output_thread,NULL,write_output_thread,argv[arg_num + 1]);
pthread_join(output_thread,NULL);
//void *write_output_thread(void *arg);
~~~
*/
#ifndef _WIN32
#include <pthread.h>
#else
#ifndef win32_pthread_h
#define win32_pthread_h

#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#ifndef WIN32_LEAND_AND_MEAN
#define WIN32_LEAND_AND_MEAN 1
#endif
#ifndef NOSERVICE
#define NOSERVICE 1
#endif
#ifndef NOMCX
#define NOMCX 1
#endif
#ifndef NOIME
#define NOIME 1
#endif
#include <Windows.h>//CreateThread

//typedef HANDLE pthread_mutex_t;
typedef struct { HANDLE mux; } pthread_mutex_t;
typedef void pthread_mutexattr_t;
//#define PTHREAD_MUTEX_INITIALIZER INVALID_HANDLE_VALUE;
static const pthread_mutex_t PTHREAD_MUTEX_INITIALIZER = { INVALID_HANDLE_VALUE };
/*int*/static void pthread_mutex_destroy(pthread_mutex_t* mutex);
/*int*/static void pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);
/*int*/static void pthread_mutex_lock(pthread_mutex_t* mutex);
/*int*/static int pthread_mutex_trylock(pthread_mutex_t* mutex);
/*int*/static void pthread_mutex_unlock(pthread_mutex_t* mutex);

typedef HANDLE pthread_t;
#define PTHREAD_NULL NULL
/** basic clone of
~~~
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
void *(*start_routine) (void *), void *arg);
~~~ */
static void pthread_create(pthread_t*output_thread,void* giveNULL,void*(*write_output_thread)(void*),void *argv);
/** basic clone of 
~~~
int pthread_join(pthread_t thread, void **retval);
~~~ */
static void pthread_join(pthread_t output_thread,void*giveNULL);
/*------------------implementation------------------*/
struct stackstore {
    void*(*func2call)(void*);
    void*funcarg;
    HANDLE gangsta_semaphore;
};
//#include <stdatomic.h>//why not, other code uses it anyway
static DWORD WINAPI ThreadProc(LPVOID lpParameter){
    struct stackstore*gangsta_struct=(struct stackstore*)lpParameter;
    void*(*gangsta_func)(void*)=gangsta_struct->func2call;
    void*gangsta_data=gangsta_struct->funcarg;
    void*gangstas_not_listening;
    ReleaseSemaphore(gangsta_struct->gangsta_semaphore,1,NULL);
    gangstas_not_listening=gangsta_func(gangsta_data);
    return 0;
}
/*
HANDLE CreateThread(
  [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
  [in]            SIZE_T                  dwStackSize,
  [in]            LPTHREAD_START_ROUTINE  lpStartAddress,
  [in, optional]  __drv_aliasesMem LPVOID lpParameter,
  [in]            DWORD                   dwCreationFlags,
  [out, optional] LPDWORD                 lpThreadId
);
*/
static void pthread_create(pthread_t*output_thread,void* giveNULL,void*(*write_output_thread)(void*),void *argv){
    struct stackstore param;
    HANDLE h;
    param.gangsta_semaphore=CreateSemaphore( 
        NULL,           // default security attributes
        0,  // initial count
        1,  // maximum count
        NULL);          // unnamed semaphore
    if (param.gangsta_semaphore == NULL) return;
    param.func2call = write_output_thread;
    param.funcarg = argv;
    h = CreateThread(NULL,0/*use default stack*/,&ThreadProc,&param,0/*use defaults run immidiately*/,NULL);
    if(h!=NULL&&h!=INVALID_HANDLE_VALUE) {
        /*--keep it on the stack until okay--*/
        WaitForSingleObject(param.gangsta_semaphore,INFINITE);
    }
    CloseHandle(param.gangsta_semaphore);
    *output_thread = h;
}
/*The function waits for the specified thread to terminate.
If the thread has already terminated, pthread_join returns
immediately*/
static void pthread_join(pthread_t output_thread,void*giveNULL){
    HANDLE h = (HANDLE)output_thread;
    if(h==NULL) return;
    if(h==INVALID_HANDLE_VALUE) return;
    WaitForSingleObject(h, INFINITE);
}


static void pthread_mutex_destroy(pthread_mutex_t* mutex) {
    HANDLE h;
    if (mutex == NULL)return;
    h = mutex->mux;
    if (h == NULL) return;
    if (h == INVALID_HANDLE_VALUE) return;
    CloseHandle(h);
    *mutex = PTHREAD_MUTEX_INITIALIZER;
}
static void pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
    HANDLE h = CreateMutexA(NULL, FALSE, NULL);
    if (h == INVALID_HANDLE_VALUE) h = NULL;
//    CloseHandle(h);
    mutex->mux = h;
}
static void pthread_mutex_lock(pthread_mutex_t* mutex) {
    DWORD dwWaitResult;
    HANDLE ghMutex;
    int hung = 0;
    if (mutex == NULL)return;
    ghMutex = mutex->mux;
    while (1) {
        dwWaitResult = WaitForSingleObject(
            ghMutex,    // handle to mutex
            INFINITE);  // no time-out interval
        if (dwWaitResult == WAIT_OBJECT_0) return;
        Sleep(1);
        ++hung;
        if (hung > 2000) {
            HANDLE pseudo = GetCurrentThread();
            DWORD id = GetThreadId(pseudo);
#ifdef stderr
            fprintf(stderr, "thread %d is hung?\n", id);
#endif//stdio.h included
        }
    }
}
static int pthread_mutex_trylock(pthread_mutex_t* mutex) {
    DWORD dwWaitResult;
    HANDLE ghMutex;
    int punyresult;
    if (mutex == NULL)return-__LINE__;
    ghMutex = mutex->mux;
    if (ghMutex == NULL) return-__LINE__;
    if (ghMutex == INVALID_HANDLE_VALUE) return-__LINE__;
    dwWaitResult = WaitForSingleObject(
        ghMutex,    // handle to mutex
        INFINITE);  // no time-out interval
    if (dwWaitResult == WAIT_OBJECT_0) return 0;
    punyresult = (int)dwWaitResult;
    return punyresult ? punyresult : -1;
}
static void pthread_mutex_unlock(pthread_mutex_t* mutex) {
    HANDLE ghMutex;
    if (mutex == NULL)return;
    ghMutex = mutex->mux;
    if (ghMutex == NULL) return;
    if (ghMutex == INVALID_HANDLE_VALUE) return;
    ReleaseMutex(ghMutex);
}


#ifdef TEST_PTHREADS
volatile int ping=1;
pthread_mutex_t pisstdio;
void*perv(void*p){
    const char* s = (const char*)p;
    pthread_mutex_lock(&pisstdio); printf("%s\n",s); pthread_mutex_unlock(&pisstdio);
    while(ping<10) {
        if(ping&0x01) {
            pthread_mutex_lock(&pisstdio); printf("pow %d\n",ping); pthread_mutex_unlock(&pisstdio);
            ++ping;
        }
    }
    return NULL;
}
void*power(void*ppp_slip){
    while(ping<10){
        if(!(ping&0x01)){
            pthread_mutex_lock(&pisstdio); printf("pong %d\n",ping); pthread_mutex_unlock(&pisstdio);
            ++ping;
        }
    }
    return NULL;
}
int main(int argc, char**argv){
    pthread_t pitty;
    pthread_t plow;
    pthread_mutex_init(&pisstdio, NULL);
    pthread_mutex_lock(&pisstdio); printf("phyre one\n"); pthread_mutex_unlock(&pisstdio);
    pthread_create(&pitty,NULL,&perv,"DINGDONG");
    pthread_mutex_lock(&pisstdio); printf("phyre two\n"); pthread_mutex_unlock(&pisstdio);
    pthread_create(&plow,NULL,&power,NULL);
    pthread_join(pitty,NULL);
    pthread_mutex_lock(&pisstdio); printf("pitty has performed\n"); pthread_mutex_unlock(&pisstdio);
    pthread_join(plow,NULL);
    pthread_mutex_lock(&pisstdio); printf("power has percolated\n"); pthread_mutex_unlock(&pisstdio);
    pthread_mutex_destroy(&pisstdio);
    return 0;
}
#endif//TEST_PTHREADS
#endif//win32_pthread_h
#endif//_WIN32
