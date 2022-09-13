#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <psapi.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <string>
using namespace std;
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    PROCESSENTRY32 pEntry;
    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);
    
    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.

    _tprintf(TEXT("%s  %u\n"), szProcessName, processID);

    // Release the handle to the process.

    CloseHandle(hProcess);
}
void killProcessByName(const wstring& filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);

    while (hRes)
    {
        if (!filename.compare(pEntry.szExeFile))
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                (DWORD)pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}
void listtask()
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);
     for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessNameAndID(aProcesses[i]);
        }
    }
}
void killProcessByPID(const DWORD pid)
{
    const auto explorer = OpenProcess(PROCESS_TERMINATE, false, pid);
    TerminateProcess(explorer, 1);
    CloseHandle(explorer);
            

}



int main(void)
{
    
    int choice;
    while (true)
    {
        cout << "\tMune:" << endl << "1. List all task\n" << "2.End process by name\n" << "3.End process by PID" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1: listtask(); break;

        case 2:
        {
            string name;
            cout << "Type process name: ";
            cin >> name;
            wstring wsTmp(name.begin(),name.end());
            wstring ws;
            ws = wsTmp;
            killProcessByName(ws);
            break;
        }
        case 3:
        {
            DWORD id;
            cout << "Type PID:";
            cin >> id;
           
            killProcessByPID(id);
            break;
        }
        }
        system("pause");
    }
    return 0;
}