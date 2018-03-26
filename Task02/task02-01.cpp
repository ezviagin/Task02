#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define THREAD_NUM			2
#define THREAD_NUM_TOTAL	3
#define COUNT				10
	
typedef struct DATA_ {
    INT threadNum;
    INT data[COUNT];
} DATA, *PDATA;

DWORD WINAPI PrintValsThread(LPVOID lpParam)
{
	DATA* pVal = (DATA*)lpParam;
    for (BYTE i = 0; i <= COUNT; ++i) {
        pVal->data[i] = i;
        printf("Thread %d, i = %d\n", pVal->threadNum, pVal->data[i]);
    }
    return 0;
}

INT main(INT argc, WCHAR **argv)
{
    DATA* pVal[THREAD_NUM_TOTAL];
    DWORD dwthreadNumArray[THREAD_NUM];
    HANDLE hThreadArray[THREAD_NUM];

	pVal[0] = (DATA*)malloc(sizeof(DATA));
	if (pVal[0] == NULL) {
		printf("pVal[%d] == NULL\n", 0);
		return -1;
	}

	pVal[0]->threadNum = 0;
	for (INT i = 0; i <= COUNT; ++i) {
		pVal[0]->data[i] = i;
		printf("Thread %d, i = %d\n", pVal[0]->threadNum, pVal[0]->data[i]);
	}

    for (INT i = 1; i < THREAD_NUM_TOTAL; ++i) {
        pVal[i] = (DATA*)malloc(sizeof(DATA));
        if (pVal[i] == NULL) {
            printf("pVal[%d] == NULL\n", i);
			break;
        }
		pVal[i]->threadNum = i;

        hThreadArray[i] = CreateThread(NULL, 0, PrintValsThread, pVal[i], 0, &dwthreadNumArray[i]);
        if (hThreadArray[i] == NULL) {
            printf("hThreadArray[%d] is NULL\n", i);
			break;
        }
    }

    INT wait = WaitForMultipleObjects(THREAD_NUM, hThreadArray, TRUE, INFINITE);
    if (wait == WAIT_FAILED)
        printf("The function WaitForMultipleObjects() has been failed\n");

    for (INT i = 1; i < THREAD_NUM_TOTAL; ++i) {
        CloseHandle(hThreadArray[i]);
            free(pVal[i]);
    }

	printf("It's over\n");
    _getch();
    return 0;
}
