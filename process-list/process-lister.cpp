#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
using namespace std;

//GUIDE: learn.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes

BOOL GetProcessList();

int main() {

    cout << "Main.";
    GetProcessList();

    return 0;
}


BOOL GetProcessList() {

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "Invalid process handle.";
        return FALSE;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap, &pe32)) {
        cout << "Invalid PE32 First";
        return FALSE;
    }

    do
    {
        wcout << "Process Name: " << pe32.szExeFile << " - " << "PID: " << pe32.th32ProcessID << endl;
    } while (Process32Next(hProcessSnap, &pe32));
    


    return TRUE;
}