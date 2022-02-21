//
// Created by Gokalp on 12/25/2021.
//

#include "ErrorChecker.h"

namespace Era
{

    void ErrorChecker::Check(HRESULT hr)
    {
        LPTSTR errorText = NULL;
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|
        FORMAT_MESSAGE_ALLOCATE_BUFFER|
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
        (LPTSTR)&errorText,
        0,
        NULL);
        if(!SUCCEEDED(hr))
        {
            ERA_ERROR(errorText);
        }
        if(errorText != NULL)
        {
            free(errorText);
        }
    }

    void ErrorChecker::CheckLine_File(HRESULT hr, int line, const char *file) {
        LPTSTR errorText = NULL;
        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|
                      FORMAT_MESSAGE_ALLOCATE_BUFFER|
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      nullptr,
                      hr,
                      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                      (LPTSTR)&errorText,
                      0,
                      NULL);
        if(!SUCCEEDED(hr))
        {
            ERA_ERROR("Error in file {0}, line:{1} \n {2}",file,line,errorText);
        }
        if(errorText != NULL)
        {
            ::LocalFree(errorText);
        }
    }
}