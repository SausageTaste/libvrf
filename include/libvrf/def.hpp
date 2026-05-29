#pragma once


#if defined(_MSC_VER)
    //  Microsoft
    #define LIBVRF_DLLExport __declspec(dllexport)
#elif defined(__GNUC__)
    //  GCC
    #define LIBVRF_DLLExport __attribute__((visibility("default")))
#else
    //  do nothing and hope for the best?
    #define LIBVRF_DLLExport
    #pragma warning Unknown dynamic link import/export semantics.
#endif
