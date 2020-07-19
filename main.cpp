#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <WinUser.h>
#include <tlhelp32.h>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <thread>
#include <TlHelp32.h>
#include <string>
#include <cstdarg>
#include <cmath>
#include <chrono>
#include <thread>
#include "Template.h"
#include "Vector.h"

uint64_t game_manager, profile_manager;

bool glow = false;
bool hands = false;
bool norecoil = false;
bool nospread = false;
bool noclip = false;
bool speed = false;
bool rapidfire = false;
bool cavesp = false;
bool infinitetime = false;
bool glowStyle = false;
bool hpmod = false;
bool bmod = false;
bool unlockall = false;

DWORD GetPID(const char* ProcessName) {
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);


    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if (!strcmp(processInfo.szExeFile, ProcessName))
    {
        CloseHandle(processesSnapshot);
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!strcmp(processInfo.szExeFile, ProcessName))
        {
            CloseHandle(processesSnapshot);
        }
    }
    return processInfo.th32ProcessID;
}



uintptr_t GetModule(HANDLE Handle)
{
    UINT64 LocalPlayer = RPM<UINT64>(RPM<UINT64>(profile_manager + 0x68));
    HMODULE hMods[1024];
    DWORD cbNeeded;
    unsigned int i;

    if (EnumProcessModules(Handle, hMods, sizeof(hMods), &cbNeeded))
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(Handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                std::string wstrModName = szModName;
                std::string wstrModContain = "RainbowSix.exe";
                if (wstrModName.find(wstrModContain) != std::string::npos)
                {
                    base_address = (uintptr_t)hMods[i];
                    return base_address;
                }
            }
        }
    }

    return false;
}

void EnablePatchLessUnlockAll()
{
    DWORD pid = GetPID("RainbowSix.exe");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    unsigned long long GameBaseAddress = GetModule(hProcess);
    char MovShell[] = { 0x41, 0xC6, 0x46, 0x51, 0x00, 0x90 };
    WriteProcessMemory(hProcess, (LPVOID)(GameBaseAddress + 0x1D2AE1B), MovShell, sizeof(MovShell), NULL);
}

void EnableRunAndShot()
{
    DWORD pid = GetPID("RainbowSix.exe");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    unsigned long long GameBaseAddress = GetModule(hProcess);
    char MovShell[7]= { 0x80,0xB9,0x80,0x00 ,0x00 ,0x00, 0x01 };
    WriteProcessMemory(hProcess, (LPVOID)(GameBaseAddress + 0x2DA5D35), MovShell, sizeof(MovShell), NULL);
    WriteProcessMemory(hProcess, (LPVOID)(GameBaseAddress + 0x14765E1), MovShell, sizeof(MovShell), NULL);
}



float OutlineRainbow1()
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::sin(freq * cnt + 0) * 0.5f + 0.5f;
}
float OutlineRainbow2()
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::sin(freq * cnt + 2) * 0.5f + 0.5f;
}
float OutlineRainbow3()
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::sin(freq * cnt + 4) * 0.5f + 0.5f;
}

auto Rainbow()
{
    static uint32_t cnt = 0;
    float freq = 0.06f; 

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::make_tuple(std::sin(freq * cnt + 0) * 0.5f + 0.5f,
        std::sin(freq * cnt + 2) * 0.5f + 0.5f,
        std::sin(freq * cnt + 4) * 0.5f + 0.5f);
}



bool InfiniteTime()
{
        uint64_t iTime = RPM<uint64_t>(base_address + 0x605D1C0);
        if (infinitetime)
        {
            WPM<BYTE>(iTime + 0x4c1, 0);
        }
        else
        {
            WPM<BYTE>(iTime + 0x4c1, 1);
        }
    return true;
}

bool speedMod()
{
        uintptr_t zoomer = RPM<uintptr_t>(base_address + 0x53a02e8);
        zoomer = RPM<uintptr_t>(zoomer + 0xC8);
        zoomer = RPM<uintptr_t>(zoomer + 0x0);
        zoomer = RPM<uintptr_t>(zoomer + 0x30);
        zoomer = RPM<uintptr_t>(zoomer + 0x30);
        zoomer = RPM<uintptr_t>(zoomer + 0x38);
        if (speed)
        {
            WPM<int>(zoomer + 0x58, 200);
        }
        else
        {
            WPM<int>(zoomer + 0x58, 115);
        }
    return true;
}

bool nC()
{
        uint64_t NC = RPM<uint64_t>(base_address + 0x6999918);
        NC = RPM<uint64_t>(NC + 0x150);
        NC = RPM<uint64_t>(NC + 0x8);
        if (noclip)
        {
            WPM<float>(NC + 0x7F0, -1.0f);
        }
        else
        {
            WPM<float>(NC + 0x7F0, 0.0003051850945f);
            WPM<float>(NC + 0x7F4, 0.0003051850945f);
            WPM<float>(NC + 0x7F8, 0.0003051850945f);
            WPM<float>(NC + 0x7FC, 0.0003051850945f);
        }
    return true;
}

bool nS()
{
        uintptr_t noSpread = RPM<uintptr_t>(base_address + 0x1495e26d0);
        if (nospread)
        {
            WPM<float>(noSpread, 0);
        }
        else
        {
            WPM<float>(noSpread, 1);
        }
    return true;
}

bool nR() //the best way
{
        uintptr_t noRecoilM = RPM<uintptr_t>(base_address + 0x53a02e8);
        noRecoilM = RPM<uintptr_t>(noRecoilM + 0xc8);
        noRecoilM = RPM<uintptr_t>(noRecoilM + 0x0);
        noRecoilM = RPM<uintptr_t>(noRecoilM + 0x90);
        noRecoilM = RPM<uintptr_t>(noRecoilM + 0xc8);
        noRecoilM = RPM<uintptr_t>(noRecoilM + 0x278);
        uintptr_t noRecoilH = RPM<uintptr_t>(base_address + 0x53a02e8);
        noRecoilH = RPM<uintptr_t>(noRecoilH + 0xc8);
        noRecoilH = RPM<uintptr_t>(noRecoilH + 0x0);
        noRecoilH = RPM<uintptr_t>(noRecoilH + 0x90);
        noRecoilH = RPM<uintptr_t>(noRecoilH + 0xc8);
        noRecoilH = RPM<uintptr_t>(noRecoilH + 0x278);
        uintptr_t noRecoilV = RPM<uintptr_t>(base_address + 0x53a02e8);
        noRecoilV = RPM<uintptr_t>(noRecoilV + 0xc8);
        noRecoilV = RPM<uintptr_t>(noRecoilV + 0x0);
        noRecoilV = RPM<uintptr_t>(noRecoilV + 0x90);
        noRecoilV = RPM<uintptr_t>(noRecoilV + 0xc8);
        noRecoilV = RPM<uintptr_t>(noRecoilV + 0x278);
        if (norecoil)
        {
            WPM<byte>(noRecoilM + 0x168, 0);
            WPM<float>(noRecoilH + 0x14c, 0);
            WPM<float>(noRecoilV + 0x15c, 0);


        }
        else
        {
            WPM<byte>(noRecoilM + 0x168, 1);
            WPM<float>(noRecoilH + 0x14C, 1);
            WPM<float>(noRecoilV + 0x15c,1);
        }
    return true;
}
bool hpMod()
{
    uintptr_t hpMode = RPM<uintptr_t>(base_address + 0xe9bfdde8);
    if (hpmod)
    {
        WPM<byte>(hpMode , 999);
    }
    else
    {
        WPM<byte>(hpMode , 120);
    }
    return true;
}

bool bulletMod()
{
    uintptr_t BulletMod = RPM<uintptr_t>(base_address + 0x53a02e8);
    BulletMod = RPM<uintptr_t>(BulletMod + 0xc8);
    BulletMod = RPM<uintptr_t>(BulletMod + 0x0);
    BulletMod = RPM<uintptr_t>(BulletMod + 0x90);
    BulletMod = RPM<uintptr_t>(BulletMod + 0xc8);

    if (bmod)
    {
        WPM<int>(BulletMod + 0x10c, 999);
    }
    else
    {
        int a = 1;
    }
    return true;
}


bool chams() 
{
        float strength = -1.5f;
        float strength2 = 3.5f;
        uint64_t Glow = RPM<uint64_t>(base_address + 0x54E9CF8);
        Glow = RPM<uint64_t>(Glow + 0xB8);
        if (glow)
        {
            if (glowStyle)
            {
                WPM(Glow + 0x11c, strength2);
                WPM<Vector3>(Glow + 0xD0, { OutlineRainbow1() * 255, OutlineRainbow2() * 255, OutlineRainbow3() * 255 });
            }
            else
            {
                WPM(Glow + 0x11c, strength);
                WPM(Glow + 0xD0, Rainbow());
            }
            if (hands)
            {
                WPM<Vector2>(Glow + 0x110, { 0, 0 });
            }
            else
            {
                WPM<Vector2>(Glow + 0x110, { 1, 1 });
            }
        }
        else
        {
            WPM<Vector3>(Glow + 0xD0, { 0, 0, 0 });
            WPM<Vector2>(Glow + 0x110, { 0, 0 });
            WPM(Glow + 0x11c, strength);
        }
    return true;
}

void Features()
{
    if (true)
    {
        chams();
        nR();
        nS();
        nC();
        speedMod();
        InfiniteTime();
        hpMod();
        bulletMod();
    }
}
int main()
{
    SetConsoleTitle("Offline Trainers by Dine");
    pid = GetPID("RainbowSix.exe");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    base_address = GetModule(hProcess);
    std::cout << " [+] Rainbow Six:Siege Cheat Features Show Case"<< "\n";
    std::cout << " [+] Trainers Coded by DineMX based on Oly474's Offline Trainers\n";
    std::cout << " [!!] ATTENTION : Using '/belaunch' to diable BattleEye Service\n\n\n";
    std::cout << " [+]R6S pid=" << GetPID("RainbowSix.exe") << " Version = 1.1.1c\n\n\n";
    std::cout << " [F1] Cham\n";
    std::cout << " [F2] Cham On Hands\n";
    std::cout << " [F3] Cham Type\n";
    std::cout << " [F4] No Spread\n";
    std::cout << " [F5] No Recoil\n";
    std::cout << " [F6] No Clips\n";
    std::cout << " [F7] Speed Hack\n";
    std::cout << " [F8] Infinite Time\n";
    std::cout << " [F9] Infinite Health\n";
    std::cout << " [F11] Infinite Ammo\n\n\n";
    std::cout << "[+] Unlock All Have Loaded Successfully\n";
    std::cout << " ";
    EnablePatchLessUnlockAll();
    EnableRunAndShot();
    while (1)
    {
        game_manager = RPM<uint64_t>(base_address + 0x6713ED8);
        profile_manager = RPM<uint64_t>(base_address + 0x6759418);
        Features();
  
        
            if (GetAsyncKeyState(VK_F1))
            {
                glow = !glow;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F2))
            {
                hands = !hands;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F3))
            {
                glowStyle = !glowStyle;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F4))
            {
                norecoil = !norecoil;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F5))
            {
                nospread = !nospread;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F6))
            {
                noclip = !noclip;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F7))
            {
                speed = !speed;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F8))
            {
                infinitetime = !infinitetime;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F9))
            {
                hpmod = !hpmod;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F11))
            {
                bmod = !bmod;
                Sleep(180);
            }
           
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}
