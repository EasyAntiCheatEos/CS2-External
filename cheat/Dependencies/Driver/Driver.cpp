#include "Driver.h"
#include <cwchar> // Include this header for wcscmp
#include <TlHelp32.h>

bool CDriver::Write(const std::uint64_t& Address, void* Buffer, const std::size_t& Size)
{
    return WriteProcessMemory(this->Handle, (void*)(Address), Buffer, Size, nullptr);
}

bool CDriver::Read(const std::uint64_t& Address, void* Buffer, const std::size_t& Size)
{
    return ReadProcessMemory(this->Handle, (void*)(Address), Buffer, Size, nullptr);
}

std::uint64_t CDriver::GetModuleBase(const wchar_t* Name)
{
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->Id);

    if (Snapshot == INVALID_HANDLE_VALUE)
    {
        return 0x00;
    }

    MODULEENTRY32W Entry; // Note the 'W' suffix indicating the wide-character version
    Entry.dwSize = sizeof(Entry);

    if (Module32FirstW(Snapshot, &Entry)) // Use Module32FirstW for wide-character support
    {
        do
        {
            if (!wcscmp(Entry.szModule, Name))
            {
                CloseHandle(Snapshot);
                return reinterpret_cast<std::uint64_t>(Entry.modBaseAddr);
            }
        } while (Module32NextW(Snapshot, &Entry)); // Use Module32NextW for wide-character support
    }

    CloseHandle(Snapshot);
    return 0x00;
}

bool CDriver::Attach(const wchar_t* Name)
{
    PROCESSENTRY32W Entry; // Note the 'W' suffix indicating the wide-character version
    Entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Snapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (Process32FirstW(Snapshot, &Entry)) // Use Process32FirstW for wide-character support
    {
        do
        {
            if (!wcscmp(Entry.szExeFile, Name))
            {
                HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, Entry.th32ProcessID);
                if (ProcessHandle)
                {
                    this->Handle = ProcessHandle;
                    this->Id = Entry.th32ProcessID;
                    CloseHandle(Snapshot);
                    return true;
                }
                else
                {
                    CloseHandle(Snapshot);
                    return false;
                }
            }
        } while (Process32NextW(Snapshot, &Entry)); // Use Process32NextW for wide-character support
    }

    CloseHandle(Snapshot);
    return false;
}
