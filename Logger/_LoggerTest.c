#include "Logger.h"

int
main(void)
{
    
    char*   format = "[Set '%c'] Log level is '%s'";    
    char    level;
    
    
    level = '?';
    printf("----------\n");
    Logger_err("ERROR");
    Logger_wrn("WARNING");
    Logger_inf("INFORMATION");
    Logger_trc("TRACE");
    Logger_dbg("DEBUG");
    Logger_cor("CORE");
    
    
    Logger_set(LOG_LEVEL_ERROR, NULL);
    level = 'E';
    printf("----------\n");
    Logger_err(format, level, "ERROR");
    Logger_wrn(format, level, "WARNING");
    Logger_inf(format, level, "INFORMATION");
    Logger_trc(format, level, "TRACE");
    Logger_dbg(format, level, "DEBUG");
    Logger_cor(format, level, "CORE");
    
    
    Logger_set(LOG_LEVEL_WARNING, NULL);
    level = 'W';
    printf("----------\n");
    Logger_err(format, level, "ERROR");
    Logger_wrn(format, level, "WARNING");
    Logger_inf(format, level, "INFORMATION");
    Logger_trc(format, level, "TRACE");
    Logger_dbg(format, level, "DEBUG");
    Logger_cor(format, level, "CORE");
    
    
    Logger_set(LOG_LEVEL_INFORMATION, NULL);
    level = 'I';
    printf("----------\n");
    Logger_err(format, level, "ERROR");
    Logger_wrn(format, level, "WARNING");
    Logger_inf(format, level, "INFORMATION");
    Logger_trc(format, level, "TRACE");
    Logger_dbg(format, level, "DEBUG");
    Logger_cor(format, level, "CORE");
    
    
    Logger_set(LOG_LEVEL_TRACE, NULL);
    level = 'T';
    printf("----------\n");
    Logger_err(format, level, "ERROR");
    Logger_wrn(format, level, "WARNING");
    Logger_inf(format, level, "INFORMATION");
    Logger_trc(format, level, "TRACE");
    Logger_dbg(format, level, "DEBUG");
    Logger_cor(format, level, "CORE");
    
    
    Logger_set(LOG_LEVEL_DEBUG, NULL);
    level = 'D';
    printf("----------\n");
    Logger_err(format, level, "ERROR");
    Logger_wrn(format, level, "WARNING");
    Logger_inf(format, level, "INFORMATION");
    Logger_trc(format, level, "TRACE");
    Logger_dbg(format, level, "DEBUG");
    Logger_cor(format, level, "CORE");
    
    
    Logger_set(LOG_LEVEL_CORE, NULL);
    level = 'C';
    printf("----------\n");
    Logger_err(format, level, "ERROR");
    Logger_wrn(format, level, "WARNING");
    Logger_inf(format, level, "INFORMATION");
    Logger_trc(format, level, "TRACE");
    Logger_dbg(format, level, "DEBUG");
    Logger_cor(format, level, "CORE");
    
    
    return 0;
}

