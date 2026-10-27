#include <windows.h>
#include <iostream>

//g++ load-own-dll.cpp -o main.exe
int main() {
    HMODULE dll = LoadLibraryA("name.dll");

    if (dll == NULL) {
        std::cout << "failed to load DLL\n";
        return 1;
    }

    std::cout << "dll loaded successfully\n";

    FreeLibrary(dll);
    return 0;
}