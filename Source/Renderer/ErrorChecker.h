//
// Created by Gokalp on 12/25/2021.
//

#pragma once
#include "../MinWindows.h"

#include "../Log.h"
#include <iostream>
#define DX_RESULT(HR) Era::ErrorChecker::CheckLine_File(HR,__LINE__,__FILE__)

namespace Era
{
    class ErrorChecker {
    public:
        ErrorChecker() = default;

        static void Check(HRESULT hr);
        static void CheckLine_File(HRESULT hr,int line,const char* file);
    };
}



