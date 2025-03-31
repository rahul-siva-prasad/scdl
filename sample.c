#include <stdio.h>
#include <limits.h>

#include "scdl.h"

int main()
{   
    //Create LoggerContext object
    LoggerContext ctx = scdl_createLogger("debug.log",DEBUG);

    //Sample log messages
    for(unsigned int cnt=0; cnt< 10000; cnt++)
    {
        LOG(ctx,INFO,"CNT :: %u",cnt);
    }

    //Close LoggerContext object
    scdl_closeLogger(&ctx);
    return 0;
}