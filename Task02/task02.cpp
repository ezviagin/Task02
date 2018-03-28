#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <windows.h>

#define THREAD_NUM          3
#define BASE_THREAD_NUM     1
#define COUNT               10

HANDLE hMutex;

typedef struct DATA_ {
    INT threadNum;
    INT data[COUNT];
} DATA, *PDATA;

DWORD WINAPI PrintValsThread(LPVOID lpParam)
{
    DATA* pVal = (DATA*)lpParam;
    INT counter = 0;
    
    INT wait = WaitForSingleObject(hMutex, INFINITE);
    assert(wait != WAIT_FAILED);
    
    for (USHORT i = 0; i < COUNT; ++i) {
        if (wait == WAIT_OBJECT_0)
        {
            pVal->data[i] = i;
            printf("Thread %d, i = %d\n", pVal->threadNum, pVal->data[i]);
            counter++;
            
            if ((counter % 4) == 0 && counter > 0) {
                ReleaseMutex(hMutex);
                //break;
            }
        }
    }

    return 0;
}

INT main(INT argc, WCHAR **argv)
{
    DATA* pVal[THREAD_NUM + 1];
    DWORD dwthreadNumArray[THREAD_NUM];
    HANDLE hThreadArray[THREAD_NUM];

    hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("CreateMutex failed, code %d\n", GetLastError());
        return -1;
    }

    for (INT i = 0; i < THREAD_NUM; ++i) {
        pVal[i] = (DATA*)malloc(sizeof(DATA));
        if (pVal[i] == NULL) {
            printf("pVal[%d] == NULL\n", i);
            break;
        }
        pVal[i]->threadNum = BASE_THREAD_NUM + i;

        hThreadArray[i] = CreateThread(NULL, 0, PrintValsThread, pVal[i], 0, &dwthreadNumArray[i]);
        if (hThreadArray[i] == NULL) {
            printf("hThreadArray[%d] failed, error %d\n", i, GetLastError());
        break;
        }

    }

    DATA mainThreadData;
    mainThreadData.threadNum = 0;
    for (INT i = 0; i < COUNT; ++i) {
        mainThreadData.data[i] = i;
        //printf("Thread %d, i = %d\n", mainThreadData.threadNum, mainThreadData.data[i]);
    }

    INT wait = WaitForMultipleObjects(THREAD_NUM, hThreadArray, TRUE, INFINITE);
    if (wait == WAIT_FAILED)
        printf("The function WaitForMultipleObjects() has been failed\n");

    for (INT i = 0; i < THREAD_NUM; ++i) {
        if (pVal[i] != NULL)
            free(pVal[i]);
        CloseHandle(hThreadArray[i]);
    }
    CloseHandle(hMutex);

    printf("It's over\n");
    _getch();
    return 0;
}
