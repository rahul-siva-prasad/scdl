#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "scdl.h"
#include "scdl_time.h"

static const char* const s_debugLevel[MAX_LEVEL] = 	{
                                                        "NOTSET  ",
                                                        "DEBUG   ",
                                                        "INFO    ",
                                                        "WARN    ",
                                                        "ERROR   ",
                                                        "CRITICAL"
                                                    };

typedef struct 
{
    volatile int locked;
}SimpleMutex;

static SimpleMutex logMutex={0};

void lockMutex(SimpleMutex* mutex) 
{
    while (__sync_lock_test_and_set(&mutex->locked, 1));
}

void unlockMutex(SimpleMutex* mutex) 
{
    __sync_lock_release(&mutex->locked);
}

/**
 * @brief Opens Log File if not done already (Lazy Open)
 */
static void __openLogFile(LoggerContext* ctx) 
{
    if (!ctx->filePtr) 
    {
        fopen_s(&ctx->filePtr, ctx->debugLogFile, "a+");
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
 * @param e_debugLevel Debug Level/Severity
 * @param debugLineNumber Line where the debug call is triggered
 * @param debugFunctionName Function wherein the debug call is triggered
 * @param format Message to log
 */
void __f_logMessage(LoggerContext* ctx,DEBUG_LEVEL debugLevel,int debugLineNumber, const char* debugFunctionName,const char* format,...)
{	
    lockMutex(&logMutex);
    __openLogFile(ctx);
    if (ctx->filePtr)
    {
        fprintf(ctx->filePtr,"%s : %s : Line == %u : Function Name == %s : Message == ",getTime(),
                                                                                s_debugLevel[debugLevel],
                                                                                debugLineNumber,
                                                                                debugFunctionName);
    }
    printf("%s : %s : Line == %u : Function Name == %s : Message == ",	getTime(),
                                                                        s_debugLevel[debugLevel],
                                                                        debugLineNumber,
                                                                        debugFunctionName);
    va_list args;
    va_start(args, format);

    if (ctx->filePtr) {
        vfprintf(ctx->filePtr, format, args);
        fprintf(ctx->filePtr, "\n");
        fflush(ctx->filePtr);
    }
    
    vprintf(format, args);
    printf("\n");

    va_end(args);
    unlockMutex(&logMutex);
}

void scdl_closeLogger(LoggerContext* ctx)
{
    if (ctx->filePtr) 
    {
        fclose(ctx->filePtr);
        ctx->filePtr = NULL;
    }
}

int main()
{	
    LoggerContext ctx = {NULL,"debug.log",NOTSET};
    LoggerContext ctx2 = {NULL,"de2.log",NOTSET};

    LOG_MESSAGE(&ctx,CRITICAL, "Test %d", 10);
    LOG_MESSAGE(&ctx2,INFO, "Test NOOO");

    scdl_closeLogger(&ctx);
    scdl_closeLogger(&ctx2);
    return 0;
}