#include <windows.h>

//g++ dllmain.cpp -shared -o custom.dll
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            MessageBoxA(NULL, "Injected by E.", "Title", MB_OK);
            break;
    }
    return TRUE;
}
