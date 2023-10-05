#include "stdafx.h"
int main()
{
    HANDLE hProcess = 0;

    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0, on_floorAddr = 0;
    bool bHealth = false,bIncreasedValue = false,bFastBullets = false,bOn_floor = false, bGrenade = false,bAutoBhop = false;


    const int newValue = 1337;
    int on_floor_value = 0;
    //Get ProcId of the target process
    DWORD procId = GetProcId(L"ac_client.exe");

    if (procId)
    {
        //Get Handle to Process
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

        //Getmodulebaseaddress
        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

        //Resolve address
        localPlayerPtr = moduleBase + 0x109B74;

        //Resolve base address of the pointer chain
        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xF8 });
        on_floorAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0x69 });


    }
    else
    {
        std::cout << "Process not found, press enter to exit\n";
        getchar();
        return 0;
    }

    DWORD dwExit = 0;
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        system("cls");
        std::cout << R"(                     
       ,---.  ,--.,--.  ,--. ,---.      
      /  O  \ |  ||  ,'.|  |'   .-'     
     |  .-.  ||  ||  |' '  |`.  `-.     
     |  | |  ||  ||  | `   |.-'    |    
     `--' `--'`--'`--'  `--'`-----' )";

        std::cout << R"(                                                                                                
 _____ _____ _____ _____ _____ _____ _____ 
|_____|_____|_____|_____|_____|_____|_____|
)";
        std::cout << "\n";

        if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        {
            bHealth = !bHealth;
        }

        //Continuous write
        if (bHealth)
        {
            std::cout << "\nNUMPAD 0 --> GOD MODE [ON] OFF \n";
            mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }
        else {
            std::cout << "\nNUMPAD 0 --> GOD MODE ON [OFF] \n";                                        
        }


        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bIncreasedValue = !bIncreasedValue;
        }
            
        if (bIncreasedValue)
        {
            //FF 06 = inc [esi]
            mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x06", 2, hProcess);
            std::cout << "NUMPAD 1 --> ADD BULLETS ON SHOT [ON] OFF \n";
        }
        else
        {
           //FF 0E = dec [esi]
           mem::PatchEx((BYTE*)(moduleBase + 0x637E9), (BYTE*)"\xFF\x0E", 2, hProcess);
           std::cout << "NUMPAD 1 --> ADD BULLETS ON SHOT ON [OFF] \n";
        }


        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bFastBullets = !bFastBullets;
        }

        if (bFastBullets)
        {
            mem::NopEx((BYTE*)(moduleBase + 0x6366C), 10, hProcess);
            std::cout << "NUMPAD 2 --> FastBullets [ON] OFF \n";
        }
        else {
            mem::PatchEx((BYTE*)(moduleBase + 0x6366C), (BYTE*)"\x8B\x3D\xBC\x9E\x50\x00\x3B\xC7\x7C\x02\x8B\xC7\x2B\xC8\x8B\xF9\x52\x8B\xDF\xE8\x1C\xBF\xFF\xFF\x8B\x4E\x14\x33\xDB\x83\xC4\x04\x39\x19\x75\x71\x6A\x18\x68\x2C\x19\x4E\x00\x6A\x0B\xE8\x12\xCE\xFB\xFF\xD9\xEE\x83\xC4\x0C\x53\x51\xD9\x1C\x24\x6A\x01\x8D\x44\x24\x1C\x8D\x4B\x18\xBF\x68\xCE\x51\x00\x89\x5C\x24\x20\xC7\x44\x24\x1C\xD0\xDF\x4E\x00\xE8\xB9\x19\x03\x00\x8B\x46\x18\x81\x00\xFA\x00\x00\x00\x8B\x56\x08\x89\x9A\x84\x03\x00\x00\x89\x5E\x1C\x39\x1D\xD0\x01\x51\x00\x0F\x84\x79\xFF\xFF\xFF\x8B\x46\x08\x3B\x05\xF4\xF4\x50\x00\x0F\x85\x6A\xFF\xFF\xFF\x8B\x4E\x14\x39\x19\xE9\x53\xFF\xFF\xFF\x8B\x4E\x08\x89\xB1\x84\x03\x00\x00\x8B\x56\x0C\xFF\x46\x1C\x38\x9A\x28\x01\x00\x00\x75\x09\x8B\x46\x08\x88\x98\x24\x02\x00\x00\x8B\x4E\x04\x8B\x04\x8D\x38\xA2\x50\x00\x3B\xC3\x7E\x0E\x39\x46\x1C\x7C\x09\x8B\x56\x08\x88\x9A\x24\x02\x00\x00\x8B\x46\x08\x8B\x48\x04\x83\xC0\x04\x89\x4C\x24\x18\x8B\x50\x04\x89\x54\x24\x1C\x8B\x40\x08\x89\x44\x24\x20\xD9\x44\x24\x20\x8B\x45\x08\xD8\x25\xC0\x12\x4E\x00\x8B\x08\x8B\x50\x04\x8B\x40\x08\xD9\x5C\x24\x20\x89\x44\x24\x2C\x89\x4C\x24\x24\x8D\x44\x24\x24\x89\x54\x24\x28\x8B\x16\x8B\x52\x14\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2\x8B\x46\x08\x3B\x05\xF4\xF4\x50\x00\x75\x0C\x8B\x4E\x04\x51\xE8\x9C\xD0\xFF\xFF\x83\xC4\x04\x89\x1D\xD4\x09\x51\x00\x8B\x56\x08\x52\x8D\x44\x24\x28\x50\x8D\x4C\x24\x20\x51\xE8\xF0\xDB\xFF\xFF\x8B\x16\x8B\x52\x10\x83\xC4\x0C\x53\x8D\x44\x24\x28\x50\x8D\x4C\x24\x20\x51\x8B\xCE\xFF\xD2\x8B\x46\x0C\x0F\xBF\x88\x0A\x01\x00\x00\x8B\x56\x18\x89\x0A\x8B\x76\x14\xFF\x0E\x57\x8B\x7C\x24\x14\x8D\x74\x24\x28\xE8\x87\xE3\xFF\xFF\x5F\x5E\xB0\x01\x5B\x8B\xE5\x5D\xC2\x04\x00", 10, hProcess);
            std::cout << "NUMPAD 2 --> FastBullets ON [OFF] \n";
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bOn_floor = !bOn_floor;
        }

        if (bOn_floor)
        {
            on_floor_value = 1;
            mem::PatchEx((BYTE*)on_floorAddr, (BYTE*)&on_floor_value, sizeof(on_floor_value), hProcess);
            std::cout << "NUMPAD 3 --> Infinite Jump [ON] OFF \n";
        }
        else {
            std::cout << "NUMPAD 3 --> Infinite Jump ON [OFF] \n";
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            bGrenade = !bGrenade;
        }

        if (bGrenade)
        {
            //mem::PatchEx((BYTE*)(moduleBase + 0x63378), (BYTE*)"\xFF\x00", 2, hProcess);
            mem::NopEx((BYTE*)(moduleBase + 0x63378), 2, hProcess);

            std::cout << "NUMPAD 4 --> INFINITE GRENADE [ON] OFF \n";
        }
        else
        {
            //FF 0E = dec [esi]
            mem::PatchEx((BYTE*)(moduleBase + 0x63378), (BYTE*)"\xFF\x08", 2, hProcess);
            std::cout << "NUMPAD 4 --> INFINITE GRENADE ON [OFF] \n";
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            bAutoBhop = !bAutoBhop;
        }

        if (bAutoBhop) {
            std::cout << "NUMPAD 5 --> AUTO BHOP [ON] OFF";
            int in_floor_value = 0;
            ReadProcessMemory(hProcess, (BYTE*)on_floorAddr, &in_floor_value, sizeof(in_floor_value), nullptr);
            if (static_cast<int>(in_floor_value) == 1) 
            {
                keybd_event(0x20, 0, 0, 0);
                Sleep(5);
                keybd_event(0x20, 0, KEYEVENTF_KEYUP, 0);
            }

        }
        else
        {
            std::cout << "NUMPAD 5 --> AUTO BHOP ON [OFF]";
        }

        std::cout << R"(                                                                                                
 _____ _____ _____ _____ _____ _____ _____ 
|_____|_____|_____|_____|_____|_____|_____|
)";

        Sleep(10);

    }

    std::cout << "Process not found, press enter to exit\n";
    getchar();
    return 0;
}