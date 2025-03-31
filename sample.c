#include <stdio.h>

#include "scdl.h"

int main()
{   
    //Create LoggerContext object
    LoggerContext ctx = scdl_createLogger(NULL,DEBUG);

    //Sample log messages
    LOG(&ctx,ERROR  ,"Uh Oh an error occured");
    LOG(&ctx,INFO   ,"CTX FilePtr = 0x%X",ctx.filePtr);

    //Close LoggerContext object
    scdl_closeLogger(&ctx);
    return 0;
}