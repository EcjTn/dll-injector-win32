#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
using namespace std;

DWORD GetProcessId(const wchar_t* processName);
void InjectDll(DWORD pid, const char* dll_path);

int main() {

    const wchar_t* processName = L"notepad.exe";
    const char* dll_path = "Path\\To\\Your\\DLL.dll";

    DWORD pid = GetProcessId(processName);

    if (pid == 0) {
        cout << "Process not found.";
        return 0;
    }

    cout << "Found PID: " << pid << endl;

    
    InjectDll(pid, dll_path);
    return 0;
}


void InjectDll(DWORD pid, const char* dll_path) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) {
		cout << "Failed to retrieve handle to remote process: " << GetLastError() << endl;
		return;
	}

	LPVOID allocated_mem = VirtualAllocEx(hProcess, NULL, strlen(dll_path) + 1, (MEM_COMMIT | MEM_RESERVE), PAGE_READWRITE);
	if (allocated_mem == NULL) {
		cout << "Failed to allocate memory in remote process: " << GetLastError() << endl;
		return;
	}

	cout << "memory allocated at: " << allocated_mem << endl;
	WriteProcessMemory(hProcess, allocated_mem, dll_path, strlen(dll_path) + 1, NULL);
	HMODULE kernel32Base = GetModuleHandleW(L"kernel32.dll");
	if (kernel32Base == NULL) {
		cout << "Failed to retrieve handle to kernel32.dll: " << GetLastError() << endl;
		return;
	}

	FARPROC load_library_address = GetProcAddress(kernel32Base, "LoadLibraryA");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)load_library_address, allocated_mem, 0, NULL);
	if (hThread == NULL) {
		cout << "Failed to create thread in remote process" << endl;
		return;
	}

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hProcess);

    system("pause");
	return;
}







DWORD GetProcessId(const wchar_t* processName) {
    wcout << "Searching for process: " << processName << endl;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        wcout << "Invalid process handle." << endl;
        return 0;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(hProcessSnap, &pe32)) {
        wcout << "Invalid PE32 First";
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
