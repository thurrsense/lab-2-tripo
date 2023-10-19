#include "winapi_shared.h"

#include <stdio.h>

#include <QDebug>

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    )
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if ( !LookupPrivilegeValue(
            NULL,            // lookup privilege on local system
            lpszPrivilege,   // privilege to lookup
            &luid ) )        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError() );
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
           hToken,
           FALSE,
           &tp,
           sizeof(TOKEN_PRIVILEGES),
           (PTOKEN_PRIVILEGES) NULL,
           (PDWORD) NULL) )
    {
          auto lastError = GetLastError();
          printf("AdjustTokenPrivileges error: %u\n",  lastError);
          qDebug() << "AdjustTokenPrivileges error: " << lastError;
          return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
          printf("The token does not have the specified privilege. \n");
          qDebug() << "The token does not have the specified privilege";
          return FALSE;
    }

    return TRUE;
}


DWORD GetTokenInformation(
    HANDLE hToken,
    TOKEN_INFORMATION_CLASS InfoType,
    PVOID &info,
    DWORD &bufSize)
{
    DWORD retVal = NO_ERROR;
    //get the required buffer size for the privilege struct
    if (!GetTokenInformation(
        hToken, InfoType, NULL, 0, &bufSize)) {
        retVal = GetLastError();
    }

    //allocate the required size.
    //the struct has a variable array size
    if (retVal == ERROR_INSUFFICIENT_BUFFER) {
        retVal = NO_ERROR;
        info = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufSize);
        if (!info)
            retVal = GetLastError();
    }

    //get the privilege information
    if (retVal == NO_ERROR && !GetTokenInformation(
        hToken, InfoType, info, bufSize, &bufSize)) {
        retVal = GetLastError();
    }

    //release the memory if there was an error
    if (retVal != NO_ERROR) {
        if (info)
            HeapFree(GetProcessHeap(), 0, info);
        info = NULL;
    }

    return retVal;
}
