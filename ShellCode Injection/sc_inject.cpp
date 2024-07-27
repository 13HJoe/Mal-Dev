#include <windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* e = "[-]";
const char* i = "[*]";

int main(int argc, char* argv[]){

    /*VARS for later use*/
    PVOID rBuffer = NULL;
    DWORD dwPID = NULL, dwTid = NULL;
    HANDLE hProcess = NULL, hThread = NULL;

    if (argc < 2){
        printf("%s usage: %s <PID>", e, argv[0]);
        return EXIT_FAILURE;
    }

    dwPID = atoi(argv[1]);

    printf("%s trying to get a handle to the process(%ld)\n", i, dwPID);

    return EXIT_SUCCESS;
}
