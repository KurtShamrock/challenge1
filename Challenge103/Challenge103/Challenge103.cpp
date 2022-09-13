#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <tlhelp32.h>
#include <stack>
using namespace std;
namespace fs = std::filesystem;
int t1, t2, t3;
stack<string> prize;
//Cac ham su dung API CreateThread
DWORD WINAPI DoStuff(LPVOID lpParameter)
{

    // The new thread will start here
    auto startTime = std::chrono::high_resolution_clock::now();
    ifstream ifs("D://BC-training//1.txt");
    if (!ifs) {
        cout << "Error: file not opened." << endl;
    }
    char chr;
    int count = 0;
    while (ifs >> chr) {
        count++;
    }
    ifs.close();
    auto stopTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
    
    prize.pop();
    cout << prize.top() << " Thoi gian hoan thanh tien trinh 1 : " << elapsed.count() / (double)1000000 << "\n";
    cout << "\t---File 1.txt co " << count << " tu" << "\n";
    return 0;
}
DWORD WINAPI Read_File_txt(LPVOID lpParameter)
{

    auto startTime = std::chrono::high_resolution_clock::now();
    int count{ };
    std::filesystem::path p1{ "D://BC-training//" };
    for (auto& p : std::filesystem::directory_iterator(p1))
    {
        ++count;
    }

    auto stopTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
    t2 = elapsed.count() / (double)1000000;
    prize.pop();
    cout << prize.top() << " - Thoi gian hoan thanh tien trinh 2 : " << elapsed.count() / (double)1000000 << "\n";
    cout << "\t---So file trong folder " << p1 << "la : " << count << '\n';

    return 0;
}
DWORD FindProcessId(const std::wstring& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}
int GetCurrentThreadCount()
{
    // first determine the id of the current process
    DWORD const  id = FindProcessId(L"explorer.exe");
    // then get a process list snapshot.
    HANDLE const  snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    // initialize the process entry structure.
    PROCESSENTRY32 entry = { 0 };
    entry.dwSize = sizeof(entry);

    // get the current process info.
    BOOL  ret = true;
    ret = Process32First(snapshot, &entry);
    while (ret && entry.th32ProcessID != id) {
        ret = Process32Next(snapshot, &entry);
    }
    CloseHandle(snapshot);
    return ret
        ? entry.cntThreads
        : -1;
}
DWORD WINAPI Count_Thread(LPVOID lpParameter)
{

    auto startTime = std::chrono::high_resolution_clock::now();
    int a = GetCurrentThreadCount();
    auto stopTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
    t3 = elapsed.count() / (double)1000000;
    prize.pop();
    cout << prize.top() <<" Thoi gian hoan thanh tien trinh 3 : " << elapsed.count() / (double)1000000 << "\n";
    cout << "\t-----So thread trong process la : " << a << "\n";

    return 0;
}

int main() {
    prize.push("giai ba");
    prize.push("giai nhi");
    prize.push("giai nhat");
    prize.push("0");
    HANDLE hThread = CreateThread(
        NULL,    // Thread attributes
        0,       // Stack size (0 = use default)
        DoStuff, // Thread start address
        NULL,    // Parameter to pass to the thread
        0,       // Creation flags
        NULL);   // Thread id
    if (hThread == NULL)
    {
        // Thread creation failed.
        // More details can be retrieved by calling GetLastError()
        return 1;
    }

    //WaitForSingleObject(hThread, INFINITE);

    // Thread handle must be closed when no longer needed
    CloseHandle(hThread);


    HANDLE hThread1 = CreateThread(
        NULL,    // Thread attributes
        0,       // Stack size (0 = use default)
        Read_File_txt, // Thread start address
        NULL,    // Parameter to pass to the thread
        0,       // Creation flags
        NULL);   // Thread id
    if (hThread1 == NULL)
    {
        // Thread creation failed.
        // More details can be retrieved by calling GetLastError()
        return 1;
    }
    
    CloseHandle(hThread1);
    HANDLE hThread2 = CreateThread(
        NULL,    // Thread attributes
        0,       // Stack size (0 = use default)
        Count_Thread, // Thread start address
        NULL,    // Parameter to pass to the thread
        0,       // Creation flags
        NULL);   // Thread id
    if (hThread2 == NULL)
    {
        // Thread creation failed.
        // More details can be retrieved by calling GetLastError()
        return 1;
    }
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread2);
    system("pause");
    return 0;
}