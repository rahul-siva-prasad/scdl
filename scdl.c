#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "scdl.h"
#include "scdl_time.h"

typedef struct 
{
    volatile int locked;
}SimpleMutex;

static SimpleMutex logMutex={0};

/**
 * @brief Locks mutex object, Spinlock
 * @param[in] mutex SimpleMutex object
 */
void __lockMutex(SimpleMutex* mutex) 
{
    while (__sync_lock_test_and_set(&mutex->locked, 1));
}

/**
 *  @brief Unlocks mutex object
 *  @param[in] mutex SimpleMutex object
 */
void __unlockMutex(SimpleMutex* mutex) 
{
    __sync_lock_release(&mutex->locked);
}

/**
 * @brief Opens Log File if not done already (Lazy Open)
 * @param[in] ctx Pointer to LoggerContext
 */
static void __openLogFile(LoggerContext* ctx) 
{
    if (!ctx->filePtr) 
    {
        ctx->filePtr = fopen(ctx->debugLogFile, "a+");
        if (!ctx->filePtr) 
        {
            printf("Failed to open debug log file.\n");
        }
        else
        {
            fprintf(ctx->filePtr, "========================================================DEBUG========================================================\n");
        }
    }
}

/**
 * @brief Prints debug information (prints on console if log file doesn't open)
 * @param[in] e_debugLevel Debug Level/Severity
 * @param[in] debugLineNumber Line where the debug call is triggered
 * @param[in] debugFunctionName Function wherein the debug call is triggered
 * @param[in] format Message to log
 */
void __f_logMessage(LoggerContext* ctx,const char* const ctx_string,DEBUG_LEVEL debugLevel,int debugLineNumber, const char* debugFunctionName,const char* format,...)
{
    __lockMutex(&logMutex);

    const char* const s_debugLevel[MAX_LEVEL] = {   "NOTSET  ",
                                                    "DEBUG   ",
                                                    "INFO    ",
                                                    "WARN    ",
                                                    "ERROR   ",
                                                    "CRITICAL"  };
    
    if(debugLevel < ctx->debugLevel)
    {
        __unlockMutex(&logMutex);
        return;
    }
    __openLogFile(ctx);
    if (ctx->filePtr)
    {
        fprintf(ctx->filePtr,"%s : %s : Line == %u : LoggerName == %s : Function Name == %s : Message == ", __scdl_getTime(),
                                                                                                            s_debugLevel[debugLevel],
                                                                                                            debugLineNumber,
                                                                                                            ctx_string,
                                                                                                            debugFunctionName);
    }
    printf("%s : %s : Line == %u : LoggerName == %s : Function Name == %s : Message == ",	__scdl_getTime(),
                                                                                            s_debugLevel[debugLevel],
                                                                                            debugLineNumber,
                                                                                            ctx_string,
                                                                                            debugFunctionName);
    va_list args;
    va_start(args, format);

    if (ctx->filePtr)
    {
        vfprintf(ctx->filePtr, format, args);
        fprintf(ctx->filePtr, "\n");
        fflush(ctx->filePtr);
    }
    
    vprintf(format, args);
    printf("\n");

    va_end(args);
    __unlockMutex(&logMutex);
}

/**
 * @brief API called at end to close debug context
 * @param[in] ctx Pointer to LoggerContext
 */
void scdl_closeLogger(LoggerContext* ctx)
{
    if (ctx->filePtr) 
    {
        fclose(ctx->filePtr);
        ctx->filePtr = NULL;
    }
}


/**
 * @brief API called to create a debug context
 * @param[in] debugFileName Filename for debug log
 * @param[in] debugLevel Debug level above which log is active
 * @param[out] LoggerContext Returns LoggerContext *shallow copy*
 */
LoggerContext scdl_createLogger(const char* debugFileName,const DEBUG_LEVEL debugLevel)
{
    if(debugFileName == NULL)
    {
        debugFileName = "debug.log"; //string literal stored in read-only section, not bound to function scope
    }
    LoggerContext ctx = {.filePtr = NULL, .debugLogFile = debugFileName, .debugLevel = debugLevel};
    return ctx;
}