#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
using namespace std;

DWORD GetProgramPid(const wchar_t* processName);
//void InjectDll(DWORD pid);

int main() {

    DWORD pid = GetProgramPid(L"notepad.exe");

    if (pid != 0) {
        cout << "PID: " << pid << endl;
    }
    else {
        cout << "Process not found." << endl;
    }

    return 0;
}

DWORD GetProgramPid(const wchar_t* processName) {

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "Invalid process handle." << endl;
        return 0;
    }

    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return 0;
    }

    do {
        if (_wcsicmp(pe32.szExeFile, processName) == 0) {
            DWORD foundPid = pe32.th32ProcessID;

            CloseHandle(hProcessSnap);
            return foundPid;
        }

    } while (Process32NextW(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return 0;
}