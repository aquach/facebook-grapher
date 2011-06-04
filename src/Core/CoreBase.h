#pragma once

#include <cassert>

#if defined(_MSC_VER)
    #define COMPILER_VCC 1
    #define COMPILER_GCC 0
#elif defined(__GNUC__)
    #define COMPILER_VCC 0
    #define COMPILER_GCC 1
#endif

#if defined(_DEBUG)
    #define DEBUG 1
    #define RELEASE 0
#else
    #define DEBUG 0
    #define RELEASE 1
#endif

// Disable deprecation warning
#if COMPILER_VCC
    #pragma warning(disable: 4996)
#endif

//------------------------------------------------------------------
// General type typedefs
//------------------------------------------------------------------
typedef char int8;
typedef unsigned char uint8;

typedef short int16;
typedef unsigned short uint16;

typedef long int int32;
typedef unsigned long int uint32;

typedef long long int64;
typedef unsigned long long uint64;

typedef unsigned int uint;

typedef float real32;
typedef double real64;

//------------------------------------------------------------------
// Helper macros
//------------------------------------------------------------------

#define Assert_(cond) assert(cond);

#define arrsize(arr) (sizeof(arr) / sizeof((arr)[0]));

#define DelClearIfUsed_(ptr)       \
            if (ptr != NULL) {     \
                delete ptr;        \
                ptr = NULL;        \
            }

#define ref_(arg) (void)((void*)&(arg));

#define NullStmt_() ;

// Platform stuff
#if COMPILER_VCC
    #include <windows.h>
#endif

// Debugging
#if COMPILER_VCC && DEBUG
    #define _CRTDBG_MAP_ALLOC 1
    #include <stdlib.h>
    #include <crtdbg.h>
#endif

// Standardized buffer size
#define BUFFER_SIZE 1024

// Class helpers for singleton
#define StubSingleton_(className)                           \
        className () { }                                    \
        className (const className&);                       \
        className& operator= (const className& other);

#define StubSingletonNoConstructor_(className)              \
        className (const className&);                       \
        className& operator= (const className& other);

// Debug logging
#if COMPILER_VCC
    #if DEBUG
        #define LOG_(msg)                                                   \
            {                                                               \
                int len  = lstrlenA(msg);                                   \
                BSTR unicodestr = SysAllocStringLen(NULL, len);             \
                ::MultiByteToWideChar(CP_ACP, 0, msg, len, unicodestr, len);\
                OutputDebugString(unicodestr);                              \
                ::SysFreeString(unicodestr);                                \
            }
    #else
        #define LOG_(stuff) NullStmt_();
    #endif
#endif

// Iterate helper
#define Iterate_(type, container) \
    for (type##::iterator iter = container##.begin(); iter != container##.end(); ++iter) {

#define EndIterate_() }