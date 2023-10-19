#ifndef WINAPI_SHARED_H
#define WINAPI_SHARED_H

#include <windows.h>
#include <tchar.h>

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    );


DWORD GetTokenInformation(
    HANDLE hToken,
    TOKEN_INFORMATION_CLASS InfoType,
    PVOID &info,
    DWORD &bufSize);

#endif // WINAPI_SHARED_H
