#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define THREAD_NUM          3
#define BASE_THREAD_NUM     1
#define COUNT               10

typedef struct DATA_ {
    INT threadNum;
    INT threadIndex;
    INT value[COUNT];
} DATA, *PDATA, **PPDATA;

typedef struct THREAD_DATA_
{
    CRITICAL_SECTION cs;
    DATA* data[THREAD_NUM];
} THREAD_DATA, *PTHREAD_DATA;

DWORD WINAPI PrintValsThread(LPVOID lpParam)
{
    THREAD_DATA* thData = (THREAD_DATA*)lpParam;
    USHORT counter = 0;

    for (USHORT i = 0; i < COUNT; ++i) {
        if ((counter % 5) == 0)
            EnterCriticalSection(&thData->cs);

        thData->data[0]->value[i] = i;
        printf("Thread %d, i = %d\n", thData->data[0]->threadNum, thData->data[0]->value[i]);
        counter++;
            
        if ((counter % 5) == 0)
            LeaveCriticalSection(&thData->cs);
    }
    return 0;
}

INT main(INT argc, WCHAR **argv)
{
    HANDLE hThreadArray[THREAD_NUM];
    DWORD dwthreadNumArray[THREAD_NUM];
    
    THREAD_DATA* thData = (THREAD_DATA*)malloc(sizeof(THREAD_DATA));
    if (thData == NULL) {
        printf("Malloc failed\n");
        return -1;
    }

    InitializeCriticalSection(&thData->cs);
    
    for (INT i = 0; i < THREAD_NUM; ++i) {
        thData->data[i] = (DATA*)malloc(sizeof(DATA));
        if (thData->data == NULL) {
            printf("Malloc failed\n");
            return-1;
        }

        thData->data[i]->threadNum = BASE_THREAD_NUM + i;
        
        hThreadArray[i] = CreateThread(NULL, 0, PrintValsThread, thData, 0, &dwthreadNumArray[i]);
        if (hThreadArray[i] == NULL) {
            printf("hThreadArray[%d] failed, error %d\n", i, GetLastError());
            break;
        }
    }

    INT wait = WaitForMultipleObjects(THREAD_NUM, hThreadArray, TRUE, INFINITE);
    if (wait == WAIT_FAILED)
        printf("The function WaitForMultipleObjects() has been failed\n");

    DeleteCriticalSection(&thData->cs);
    
    if (thData != NULL) {
        for (USHORT i = 0; i < THREAD_NUM; ++i) {
            if (thData->data[i] != NULL) {
                free(thData->data[i]);
            }
        }
        free(thData);
    }
    printf("It's over\n");

    _getch();
    return 0;
}
