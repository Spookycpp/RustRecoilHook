#include <Windows.h>
#include <iostream>
#include <MinHook.h>

struct Vec3 {
	float x, y, z;
};

uintptr_t ModuleBase = (uintptr_t)GetModuleHandleA(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

uintptr_t HeldEntity__AddPunch = 0x8F2540;

void(__fastcall* HeldEntity__AddPunch_o)(DWORD*, Vec3, float, DWORD*);
void __stdcall HeldEntity__AddPunch_h(DWORD* __this, Vec3 amount, float duration, DWORD* method) {
    std::cout << "{" << amount.y << "," << amount.x << "},";

	return HeldEntity__AddPunch_o(__this, amount, duration, method);
}

DWORD WINAPI MainThread(HMODULE hModule) {
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
	
    MH_Initialize();
	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HeldEntity__AddPunch), &HeldEntity__AddPunch_h, (LPVOID*)&HeldEntity__AddPunch_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HeldEntity__AddPunch));
	
    while (true) {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            system("cls");
        }
		
        if (GetAsyncKeyState(VK_END) & 1) {
			MH_Uninitialize();
            if (file) { fclose(file); }
            FreeConsole();
            FreeLibraryAndExitThread(hModule, 0);
            return 0;
        }
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

