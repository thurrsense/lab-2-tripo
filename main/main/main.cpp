#define _CRT_SECURE_NO_WARNINGS
#define _UNICODE
#include <iostream>
#include <fstream>
#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <comdef.h>
#include <winbase.h>
#include <sddl.h>
#include <string>
#include <array>
#include <cstddef>
#include <sstream>
#include <cstring>

using namespace std;

char* doubleSym(const char* str, char sym)
{
    unsigned char count = 0;
    for (unsigned short i = 0; str[i] != '\0'; i++)
        if (str[i] == sym)
            count++;

    char* new_str = new char[strlen(str) + 1 + count];
    memset(new_str, '\0', strlen(str) + 1 + count);
    unsigned short i = 0;
    for (unsigned short j = 0, k = 0; k < count; i++, j++)
    {
        new_str[i] = str[j];
        if (new_str[i] == sym)
        {
            new_str[++i] = sym;
            k++;
        }
    }
    if (strrchr(str, sym) != NULL)
    {
        _memccpy(new_str + i, strrchr(str, sym) + 1, '\0', strlen(strrchr(str, sym) + 1));
    }
    else
    {
        _memccpy(new_str, str, '\0', strlen(str) + 1);
    }

    return new_str;
}

std::string utf8_encode(const std::wstring& wstr) {

    if (wstr.empty()) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);

    std::string strTo(size_needed, 0);

    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);

    return strTo;

}
// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

DWORD getppid(DWORD childProcessID)
{
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD ppid = 0, pid = childProcessID;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    __try {
        if (hSnapshot == INVALID_HANDLE_VALUE) __leave;

        ZeroMemory(&pe32, sizeof(pe32));
        pe32.dwSize = sizeof(pe32);
        if (!Process32First(hSnapshot, &pe32)) __leave;

        do {
            if (pe32.th32ProcessID == pid) {
                ppid = pe32.th32ParentProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));

    }
    __finally {
        if (hSnapshot != INVALID_HANDLE_VALUE) CloseHandle(hSnapshot);
    }
    return ppid;
}

BOOL GetLogonFromToken(HANDLE hToken, ofstream& fout)
{
    DWORD dwSize = MAX_PATH;
    BOOL bSuccess = FALSE;
    DWORD dwLength = 0;
    PTOKEN_USER ptu = NULL;

    SID_NAME_USE SidType;
    char lpName[2 * MAX_PATH];
    char lpDomain[2 * MAX_PATH];
    //Verify the parameter passed in is not NULL.
    if (NULL == hToken)
        goto Cleanup;

    if (!GetTokenInformation(
        hToken,         // handle to the access token
        TokenUser,    // get information about the token's groups 
        (LPVOID)ptu,   // pointer to PTOKEN_USER buffer
        0,              // size of buffer
        &dwLength       // receives required buffer size
    ))
    {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            goto Cleanup;

        ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(),
            HEAP_ZERO_MEMORY, dwLength);

        if (ptu == NULL)
            goto Cleanup;
    }

    if (!GetTokenInformation(
        hToken,         // handle to the access token
        TokenUser,    // get information about the token's groups 
        (LPVOID)ptu,   // pointer to PTOKEN_USER buffer
        dwLength,       // size of buffer
        &dwLength       // receives required buffer size
    ))
    {
        goto Cleanup;
    }


    if (!LookupAccountSid(NULL, ptu->User.Sid, (LPWSTR)lpName, &dwSize, (LPWSTR)lpDomain, &dwSize, &SidType))
    {
        DWORD dwResult = GetLastError();
        if (dwResult == ERROR_NONE_MAPPED)
            _tcscpy((LPWSTR)lpName, L"NONE_MAPPED");
        else
        {
            printf("LookupAccountSid Error %u\n", GetLastError());
        }
    }
    else
    {
        _tprintf(TEXT("\tuser: %ls\t%ls\t"), (LPWSTR)lpDomain, (LPWSTR)lpName);
        wstring s1((LPWSTR)lpDomain);
        string s2 = utf8_encode(s1);
        fout << "\t\t\"domain\" : \"" << s2 << "\"," << endl;
        wstring s3((LPWSTR)lpName);
        s2 = utf8_encode(s3);
        fout << "\t\t\"owner_name\" : \"" << s2 << "\"," << endl;
        FILE* f = fopen("try.txt", "a");
        fwprintf(f, TEXT("%s\n"), (LPWSTR)lpName);
        LPSTR str;
        bool result = ConvertSidToStringSidA(ptu->User.Sid, &str);
        cout << str << endl;
        fout << "\t\t\"sid\" : \"" << str << "\"," << endl;
        //cout << (LPWSTR)lpName << endl;
        bSuccess = TRUE;
    }

Cleanup:

    if (ptu != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);
    return bSuccess;
}

void InformationAboutProcess(DWORD processID, ofstream& fout, bool* isStarted)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    TCHAR szProcessPath[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    // Get the process name.

    if (NULL != hProcess)
    {
        if (*isStarted)
            fout << "\t," << endl;
        else
            *isStarted = true;

        fout << "\t{" << endl;


        HMODULE hMod;
        DWORD cbNeeded;
        DWORD pProcessID;

        if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
            GetModuleFileNameEx(hProcess, NULL, szProcessPath, sizeof(szProcessPath) / sizeof(TCHAR));

            pProcessID = getppid(processID);

        }

        wstring s1(szProcessName);
        string s2 = utf8_encode(s1);
        fout << "\t\t\"name\" : \"" << s2 << "\"," << endl;
        s1 = szProcessPath;
        s2 = utf8_encode(s1);
        char* print = doubleSym(s2.c_str(), '\\');
        fout << "\t\t\"path\" : \"" << print << "\"," << endl;
        fout << "\t\t\"pid\" : \"" << processID << "\"," << endl;
        _tprintf(TEXT("%s  %s (PID: %u)\n"), szProcessName, szProcessPath, processID);
        delete[] print;

        HANDLE pProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pProcessID);
        TCHAR pProcessName[MAX_PATH] = TEXT("<unknown>");
        TCHAR pProcessPath[MAX_PATH] = TEXT("<unknown>");
        if (pProcess != NULL)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModulesEx(pProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_ALL))
            {
                GetModuleBaseName(pProcess, hMod, pProcessName, sizeof(pProcessName) / sizeof(TCHAR));
                GetModuleFileNameEx(pProcess, NULL, pProcessPath, sizeof(pProcessPath) / sizeof(TCHAR));
            }
        }

        s1 = pProcessName;
        string s4(s1.begin(), s1.end());
        fout << "\t\t\"p_name\" : \"" << s4 << "\"," << endl;
        s1 = pProcessPath;
        string s5(s1.begin(), s1.end());
        char* p2 = doubleSym(s5.c_str(), '\\');
        fout << "\t\t\"p_path\" : \"" << p2 << "\"," << endl;
        fout << "\t\t\"p_pid\" : \"" << pProcessID << "\"," << endl;
        delete[] p2;

        _tprintf(TEXT("\t\tparent: %s  %s (PID: %u)\n"), pProcessName, pProcessPath, pProcessID);
        //PrintProcessNameAndID(pProcessID);


        HANDLE hToken = NULL;

        if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
        {
            CloseHandle(hProcess);
            return;
        }
        BOOL bres = GetLogonFromToken(hToken, fout);
        BOOL resBool = false;
        DWORD resDword = 0;
        BOOL lPermanent = TRUE;
        IsWow64Process(hProcess, &resBool);
        GetProcessDEPPolicy(hProcess, &resDword, &lPermanent);

        PROCESS_MITIGATION_ASLR_POLICY pAslrPolicy;
        BOOL res = GetProcessMitigationPolicy(hProcess, ProcessASLRPolicy, &pAslrPolicy, sizeof(PROCESS_MITIGATION_ASLR_POLICY));


        fout << "\t\t\"bit64\" : \"" << (!resBool ? "yes" : "no") << "\"," << endl;
        fout << "\t\t\"DEP\" : \"" << (resDword ? "yes" : "no") << "\"," << endl;
        fout << "\t\t\"ASLR\" : \"" << (pAslrPolicy.Flags ? "yes" : "no") << "\"," << endl;
        _tprintf(TEXT("\t\tbit64: %s\n"), !resBool ? (LPWSTR)L"yes" : (LPWSTR)L"no");
        _tprintf(TEXT("\t\tDEP: %s\n"), resDword ? (LPWSTR)L"yes" : (LPWSTR)L"no");
        _tprintf(TEXT("\t\tASLR: %s\n"), pAslrPolicy.Flags ? (LPWSTR)L"yes" : (LPWSTR)L"no");

        DWORD dwLengthNeeded;
        if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwLengthNeeded))
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_INSUFFICIENT_BUFFER)
            {
                PTOKEN_MANDATORY_LABEL pTIL = (PTOKEN_MANDATORY_LABEL)LocalAlloc(0, dwLengthNeeded);
                if (pTIL != NULL)
                {
                    if (GetTokenInformation(hToken, TokenIntegrityLevel, pTIL, dwLengthNeeded, &dwLengthNeeded))
                    {
                        DWORD dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid, (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1));
                        if (dwIntegrityLevel == SECURITY_MANDATORY_UNTRUSTED_RID)
                        {
                            fout << "\t\t\"integrity\" : \"" << "untrusted" << "\"," << endl;
                            _tprintf(TEXT("\t\tintegrity: Untrusted Integrity Process\n"));
                        }
                        else if (dwIntegrityLevel == SECURITY_MANDATORY_LOW_RID)
                        {
                            fout << "\t\t\"integrity\" : \"" << "low" << "\"," << endl;
                            _tprintf(TEXT("\t\tintegrity: Low Integrity Process\n"));
                        }
                        else if (dwIntegrityLevel == SECURITY_MANDATORY_MEDIUM_RID)
                        {
                            fout << "\t\t\"integrity\" : \"" << "medium" << "\"," << endl;
                            _tprintf(TEXT("\t\tintegrity: Medium Integrity Process\n"));
                        }
                        else if (dwIntegrityLevel == SECURITY_MANDATORY_HIGH_RID)
                        {
                            fout << "\t\t\"integrity\" : \"" << "high" << "\"," << endl;
                            _tprintf(TEXT("\t\tintegrity: High Integrity Process\n"));
                        }
                        else if (dwIntegrityLevel == SECURITY_MANDATORY_SYSTEM_RID)
                        {
                            fout << "\t\t\"integrity\" : \"" << "system" << "\"," << endl;
                            _tprintf(TEXT("\t\tintegrity: System Integrity Process\n"));
                        }
                    }
                    LocalFree(pTIL);
                }
            }
        }
        /*
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
        {
            if (Elevation.TokenIsElevated)
            {
                fout << "\t\"privileges\" : \"" << "elevated" << "\"," << endl;
                _tprintf(TEXT("\privileges: elevated\n"));
            }
            else
            {
                fout << "\t\"privileges\" : \"" << "non-elevated" << "\"," << endl;
                _tprintf(TEXT("\privileges: non-elevated\n"));
            }
        }
        */


        UCHAR InfoBuffer[1000];
        PTOKEN_PRIVILEGES ptgPrivileges = (PTOKEN_PRIVILEGES)InfoBuffer;
        WCHAR ucPrivilegeName[500];
        WCHAR ucDisplayName[500];
        DWORD dwInfoBufferSize;
        DWORD dwPrivilegeNameSize;
        DWORD dwDisplayNameSize;

        GetTokenInformation(hToken, TokenPrivileges, InfoBuffer,
            sizeof InfoBuffer, &dwInfoBufferSize);

        if (ptgPrivileges->PrivilegeCount)
        {
            fout << "\t\t\"privileges\" : [" << endl;
            bool isDone = false;
            for (unsigned long i = 0; i < ptgPrivileges->PrivilegeCount; i++)
            {

                dwPrivilegeNameSize = sizeof ucPrivilegeName;
                dwDisplayNameSize = sizeof ucDisplayName;
                LookupPrivilegeNameA(NULL, &ptgPrivileges->Privileges[i].Luid,
                    (char*)ucPrivilegeName, &dwPrivilegeNameSize);
                //if (ptgPrivileges->Privileges[i].Attributes)
                //{
                if (isDone)
                    fout << "\t\t\t," << endl;
                else
                    isDone = true;
                fout << "\t\t\t{" << endl;
                fout << "\t\t\t\t\"name\" : \"" << (char*)ucPrivilegeName << "\"," << endl << "\t\t\t\t\"attribute\" : \"";

                if ((ptgPrivileges->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED_BY_DEFAULT) == SE_PRIVILEGE_ENABLED_BY_DEFAULT)
                {
                    fout << "SE_PRIVILEGE_ENABLED_BY_DEFAULT\"" << endl;
                }
                else if ((ptgPrivileges->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED) == SE_PRIVILEGE_ENABLED)
                {
                    fout << "SE_PRIVILEGE_ENABLED\"" << endl;
                }
                else if ((ptgPrivileges->Privileges[i].Attributes & SE_PRIVILEGE_REMOVED) == SE_PRIVILEGE_REMOVED)
                {
                    fout << "SE_PRIVILEGE_REMOVED\"" << endl;
                }
                else if ((ptgPrivileges->Privileges[i].Attributes & SE_PRIVILEGE_USED_FOR_ACCESS) == SE_PRIVILEGE_USED_FOR_ACCESS)
                {
                    fout << "SE_PRIVILEGE_USED_FOR_ACCESS\"" << endl;
                }
                else if ((ptgPrivileges->Privileges[i].Attributes & SE_PRIVILEGE_VALID_ATTRIBUTES) == SE_PRIVILEGE_VALID_ATTRIBUTES)
                {
                    fout << "SE_PRIVILEGE_VALID_ATTRIBUTES\"" << endl;
                }
                else
                {
                    fout << "SE_PRIVILEGE_DISABLED\"" << endl;
                }
                fout << "\t\t\t}" << endl;
                //}
            }
            fout << "\t\t]," << endl;
        }


        HMODULE hMods[1024];
        DWORD Needed = cbNeeded;
        if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
        {
            fout << "\t\t\"dlls\" : [" << endl;
            bool isDone = false;
            for (DWORD i = 0; i < (Needed / sizeof(HMODULE)); i++)
            {
                if (hMods[i] != NULL)
                {
                    TCHAR szModName[MAX_PATH];

                    // Get the full path to the module's file.



                    if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
                        sizeof(szModName) / sizeof(TCHAR)))
                    {

                        if (isDone)
                            fout << "\t\t\t," << endl;
                        else
                            isDone = true;
                        fout << "\t\t\t{" << endl;
                        // Print the module name and handle value.
                        wstring s1(szModName);
                        string s2 = utf8_encode(s1);
                        char* p2 = doubleSym(s2.c_str(), '\\');
                        fout << "\t\t\t\t\"dll_path\" : \"" << (p2) << "\"," << endl;
                        fout << "\t\t\t\t\"dll_addr\" : \"" << hMods[i] << "\"" << endl;
                        delete[] p2;
                        //_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
                        fout << "\t\t\t}" << endl;
                    }


                }
            }
            fout << "\t\t]" << endl;
        }
        else
        {
            fout << "\t\t\"dlls\" : [" << endl;
            fout << "\t\t\t{" << endl;
            fout << "\t\t\t\t\"dll_path\" : \"" << "<unknown>" << "\"," << endl;
            fout << "\t\t\t\t\"dll_addr\" : \"" << "<unknown>" << "\"" << endl;
            fout << "\t\t\t}" << endl;
            fout << "\t\t]" << endl;
        }


        // Print the process name and identifier.

        // Release the handle to the process.

        fout << "\t}" << endl;

        CloseHandle(hProcess);
    }
}

int main(void)
{
    setlocale(LC_ALL, "Russian");
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return 1;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    ofstream fout("data.json");
    fout << '[' << endl;

    bool isStarted = false;

    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            InformationAboutProcess(aProcesses[i], fout, &isStarted);
        }
    }

    fout << ']' << endl;
    fout.close();
    return 0;
}
