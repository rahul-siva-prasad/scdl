#ifndef __SCDL_H__
#define __SCDL_H__


    #ifdef SCDL_EXPORT
        #define __SCDL_API__ __declspec(dllexport)
    #else
        #define __SCDL_API__ __declspec(dllimport)
    #endif

    typedef enum E_DEBUG_LEVEL
    {
        NOTSET,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL,
        MAX_LEVEL
    }DEBUG_LEVEL;

    typedef struct LoggerContext
    {
        FILE* filePtr;
        const char* debugLogFile;
        DEBUG_LEVEL debugLevel;
    }LoggerContext;
    
    __SCDL_API__ void __f_logMessage(LoggerContext* ctx,DEBUG_LEVEL debugLevel,int debugLineNumber, const char* debugFunctionName,const char* format,...);
    __SCDL_API__ LoggerContext scdl_createLogger(const char* const debugFileName,const DEBUG_LEVEL debugLevel);
    __SCDL_API__ void scdl_closeLogger(LoggerContext* ctx);

    #define LOG(ctx_p,severityLevel,fmt,...)    __f_logMessage(ctx_p, severityLevel, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);

#endif