#ifndef WHISPER_PATH_H
#define WHISPER_PATH_H

#include "whisper_inc.h"
#include "whisper_errorn.h"
#include "whisper_macro.h"


#define WHISPER_MAX_PATH    1024

namespace whisper 
{
    using namespace std;


    std::tstring GetFileName(tstring szFile);
    std::tstring GetFileSuffix(tstring szFile);
    std::tstring GetFileDir(tstring szFile);

    tstring GetThisModulePath(HMODULE hModule = NULL);
    tstring GetThisModuleDir(HMODULE hModule = NULL);
    tstring GetThisModuleName(HMODULE hModule = NULL);
    tstring GenThisDirFilePath(tstring szFileName, HMODULE hModule = NULL);

    

}

#endif // WHISPER_PATH_H
