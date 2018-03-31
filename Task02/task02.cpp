    #include <stdio.h>
    #include <conio.h>
    #include <windows.h>

    #define THREAD_NUM          3
    #define BASE_THREAD_NUM     1
    #define COUNT               10

    CRITICAL_SECTION critsection;

    typedef struct DATA_ {
        INT threadNum;
        INT data[COUNT];
    } DATA, *PDATA;

    DWORD WINAPI PrintValsThread(LPVOID lpParam)
    {
        PDATA pVal = (DATA*)lpParam;
        INT counter = 0;
        
        for (USHORT i = 0; i < COUNT; ++i) {
            if ((counter % 5) == 0)
                EnterCriticalSection(&critsection);

            pVal->data[i] = i;
            printf("Thread %d, i = %d\n", pVal->threadNum, pVal->data[i]);
            counter++;
            
            if ((counter % 5) == 0 && counter > 0)
                LeaveCriticalSection(&critsection);
        }
        return 0;
    }

    INT main(INT argc, WCHAR **argv)
    {
        PDATA pVal[THREAD_NUM + 1];
        DWORD dwthreadNumArray[THREAD_NUM];
        HANDLE hThreadArray[THREAD_NUM];
    
        InitializeCriticalSection(&critsection);
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
        USHORT counter = 0;
        
        for (USHORT i = 0; i < COUNT; ++i) {
            if ((counter % 5) == 0) {
                EnterCriticalSection(&critsection);
            }
            mainThreadData.data[i] = i;
            printf("Thread %d, i = %d\n", mainThreadData.threadNum, mainThreadData.data[i]);
            counter++;

            if ((counter % 5) == 0 && counter > 0) {
                LeaveCriticalSection(&critsection);
            }
        }

        INT wait = WaitForMultipleObjects(THREAD_NUM, hThreadArray, TRUE, INFINITE);
        if (wait == WAIT_FAILED)
            printf("The function WaitForMultipleObjects() has been failed\n");

        for (INT i = 0; i < THREAD_NUM; ++i) {
            if (pVal[i] != NULL)
                free(pVal[i]);
            CloseHandle(hThreadArray[i]);
        }
    
        DeleteCriticalSection(&critsection);
        printf("It's over\n");

        _getch();
        return 0;
    }
