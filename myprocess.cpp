#include "myprocess.h"
#include "winapi_shared.h"
#include <iostream>
#include <windows.h>
#include <winbase.h>
#include <tchar.h>
#include <psapi.h>

#include <tlhelp32.h>
#include <processthreadsapi.h>
#include <sddl.h>
#include <wow64apiset.h>
#pragma comment(lib, "advapi32.lib")

myProcess::myProcess()
{    
    PID = 0;
    PIDPArent = 0;
}

myProcess& myProcess::operator= (const myProcess& other){
    this->PID = other.PID;
    this->PIDPArent = other.PID;
    this->nameParent = other.nameParent;
    this->name = other.name;
    this->nameOwner = other.nameOwner;
    this->infDLL = other.infDLL;
    this->Description = other.Description;
    this->PATH = other.PATH;
    this->SID = other.SID;
    this->x = other.x;
    this->y = other.y;
    this->env = other.env;


    return *this;
}



myProcess::myProcess(int newPID)
{
    PID = newPID;
    PIDPArent = 0;
}

std::map<std::wstring, DWORD> myProcess::allPrivelegies()
{
   std::map<std::wstring, DWORD> res;

   HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, this->PID);

   if (NULL == hProc)
   {
      return res;
   }
   HANDLE hToken;

   if (!OpenProcessToken(hProc, TOKEN_QUERY, &hToken))
   {
      return res;
   }

   DWORD bufLength = 0;
   DWORD retVal = NO_ERROR;
   PTOKEN_PRIVILEGES pPrivileges = NULL;

   retVal = GetTokenInformation(hToken, TokenPrivileges,
                                   (PVOID&)pPrivileges, bufLength);


   if (retVal == NO_ERROR) {

       //allocate buffers
       TCHAR privilegeName[64];
       DWORD bufSize = sizeof(privilegeName);

       for (DWORD i = 0; i < pPrivileges->PrivilegeCount; i++) {
           LUID luid = pPrivileges->Privileges[i].Luid;

           //This needs to be reset because it is an in - out parameter and
           //otherwise the LookupPrivilegeName thinks the buffer is too small
           bufSize = sizeof(privilegeName);
           if (!LookupPrivilegeName(NULL, &luid, privilegeName, &bufSize)) {
               retVal = GetLastError();
               break;
           }

           std::wstring ws(&privilegeName[0]);
           DWORD privileges = pPrivileges->Privileges[i].Attributes;
           res[ws] = privileges;
       }
   }

   if(pPrivileges)
       HeapFree(GetProcessHeap(), 0, pPrivileges);

   return res;
}

void myProcess::setPID(DWORD PID){
    this->PID = PID;
}

void myProcess::setPATH(){
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, this->PID);
    wchar_t szFilePath[MAX_PATH] = {0};
    GetModuleFileNameEx(hProc, NULL, szFilePath, MAX_PATH);
    this->PATH.append(szFilePath);
    CloseHandle(hProc);
}

void myProcess::setOName(){

    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, this->PID);

    HANDLE TokenHandle;
    OpenProcessToken(hProc, TOKEN_QUERY, &TokenHandle);

    DWORD dwSize = 0;
    //PTOKEN_OWNER pOwnerInfo = (PTOKEN_OWNER) GlobalAlloc( GPTR, dwSize );
    if (GetTokenInformation(TokenHandle, TokenOwner , NULL, NULL, &dwSize)){
        CloseHandle(TokenHandle);
        CloseHandle(hProc);
        return;
    }
    PTOKEN_OWNER to = (PTOKEN_OWNER) LocalAlloc(LPTR, dwSize);
    if (!to)
    {
        CloseHandle(TokenHandle);
        CloseHandle(hProc);
        return;
    }

    if (!GetTokenInformation(TokenHandle, TokenOwner, to, dwSize, &dwSize))
       {
           //cerr << "GetTokenInformation failed, error " << dwError;
           LocalFree(to);
           CloseHandle(TokenHandle);
           CloseHandle(hProc);
           return;
       }

    char nameUser[256] = {0};
    char domainName[256] = {0};
    DWORD nameUserLen = 256;
    DWORD domainNameLen = 256;
    SID_NAME_USE snu;

    if (!LookupAccountSidA(NULL, to->Owner, nameUser, &nameUserLen, domainName, &domainNameLen, &snu))
    {
        LocalFree(to);
        CloseHandle(TokenHandle);
        CloseHandle(hProc);
        return;
    }
    nameUser[nameUserLen] = '\0';

    wchar_t Wdomain[domainNameLen + 1];
    mbstowcs(Wdomain, domainName, domainNameLen);
    Wdomain[domainNameLen] = '\0';
    Wdomain[domainNameLen + 1] = '\0';
    this->nameOwner.append(Wdomain);
    this->nameOwner.append(L"\\");

    wchar_t WnameUser[nameUserLen + 1];
    mbstowcs(WnameUser, nameUser, nameUserLen);
    WnameUser[nameUserLen] = '\0';
    WnameUser[nameUserLen + 1] = '\0';
    this->nameOwner.append(WnameUser);

    PSTR tempstr;
    if(!ConvertSidToStringSidA(to->Owner, &tempstr)){
        CloseHandle(TokenHandle);
        CloseHandle(hProc);
        return;
    }

    std::string ts(tempstr);
    wchar_t WSIDUser[ts.capacity()];
    mbstowcs(WSIDUser, ts.c_str(), ts.capacity());
    WSIDUser[ts.size()] = '\0';
    WSIDUser[ts.size() + 1] = '\0';
    this->SID.append(WSIDUser);

    CloseHandle(TokenHandle);
    CloseHandle(hProc);
}

void myProcess::setX(){

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, this->PID);
    if(hProc == 0)
        return;

    USHORT pProcessMachine, pNativeMachine;
    BOOL result = IsWow64Process2(hProc, &pProcessMachine, &pNativeMachine);
    IsWow64Process(hProc, &result);
    if(pNativeMachine == 34404){
        //x64 OS
        if (pProcessMachine != IMAGE_FILE_MACHINE_UNKNOWN && pProcessMachine == 332){
            this->x.append(L"x32");
        }
        else{
            this->x.append(L"x64");
        }
    }
    else{
        if(pNativeMachine == 332){
            //x32 OS
            if (result == false){
                this->x.append(L"x32");
            }
        }
    }


    CloseHandle(hProc);
}

void myProcess::setY(){
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, this->PID);
    if(hProc == 0)
        return;
    // HANDLE hProcess = GetCurrentProcess();
    DWORD flags = 0;
    BOOL permanent = FALSE;
    BOOL bRet = GetProcessDEPPolicy(hProc, &flags, &permanent);
    if (bRet) {
        if (flags == 0) {
            this->y.append(L"DEP is disabled; "); //DEP отключен для указанного процесса
        } else if (flags & PROCESS_DEP_ENABLE) {
            this->y.append(L"DEP is enabled; "); //DEP включен для указанного процесса
        } else if (flags & PROCESS_DEP_DISABLE_ATL_THUNK_EMULATION) {
            this->y.append(L"DEP; "); //Эмуляция преобразователя DEP-ATL отключена для указанного процесса
        } else {
            this->y.append(L"Unknown DEP flag; "); //cout << "Unknown DEP flag" << endl;
        }
    }
    else {
       this->y.append(L"Trouble with DEP, last error is: "
                      + std::to_wstring(GetLastError()) + L"; ");
    }


    CloseHandle(hProc);
}

void myProcess::setY2(){
    HMODULE hModule = GetModuleHandle(NULL);
    PROCESS_MITIGATION_ASLR_POLICY buffer;
    BOOL bRet2 = GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, NULL, &hModule);
    if (bRet2) {
        if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)hModule, &hModule)) {
            if (GetProcessMitigationPolicy(GetCurrentProcess(), ProcessASLRPolicy, &buffer, sizeof(DWORD))) {
                if (buffer.EnableBottomUpRandomization == 1) {
                    this->y.append(L"ASLR BottomUpRandomization is enabled; ");
                } else if (buffer.EnableForceRelocateImages == 1){
                    this->y.append(L"ASLR ForceRelocateImages is enabled; ");
                } else if (buffer.EnableHighEntropy == 1){
                   this->y.append(L"ASLR HighEntropy is enabled; ");
                } else if (buffer.DisallowStrippedImages == 1){
                   this->y.append(L"ASLR StrippedImages is disallowed; ");
                }
                else {
                   this->y.append(L"ASLR HighEntropy is disabled; ");
                }
            } else {
                this->y.append(L"Failed to get ASLR policy, last error is: "
                               + std::to_wstring(GetLastError()) + L"; ");
            }
        }
    }
}

void myProcess::setEnv(){
    HMODULE hMod = LoadLibraryA("mscoree.dll");
    if (!hMod) {
        this->env.append(L"Native");
    } else {
        typedef HRESULT(__stdcall *GetCORSystemDirectoryPtr)(LPWSTR, DWORD, DWORD*);
        GetCORSystemDirectoryPtr pGetCORSystemDirectory = (GetCORSystemDirectoryPtr)GetProcAddress(hMod, "GetCORSystemDirectory");
        if (pGetCORSystemDirectory) {
            WCHAR szBuffer[MAX_PATH];
            DWORD dwLength = 0;
            HRESULT hResult = pGetCORSystemDirectory(szBuffer, MAX_PATH, &dwLength);
            if (hResult == S_OK) {
                this->env.append(L"CLR .NET");
            } else {
                this->env.append(L"Failed to get CLR .NET version");
            }
        }
    }
}

void myProcess::setInfDLL(){
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, this->PID);
    if (hProc) {
        HMODULE hMods[1024];
        DWORD cbNeeded;
        if (EnumProcessModulesEx(hProc, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL)) {
            //Извлекает дескриптор для каждого модуля в указанном процессе
            for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
                TCHAR szModName[MAX_PATH];
                if (GetModuleFileNameEx(hProc, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
                    if (i == 0)
                        continue;
                    this->infDLL.append(szModName);
                    this->infDLL.append(L"\n");
                } else {
                    this->infDLL.append(L"Failed to get name");
                }
            }
        }
        CloseHandle(hProc);
    }
}

void myProcess::setProcessInfo(){

    setlocale(LC_ALL, "Russian");
    this->setPATH();
    this->setOName();
    this->setX();
    this->setY();
    this->setY2();
    this->setEnv();
    this->setInfDLL();

}

myProcess::~myProcess()
{

}
