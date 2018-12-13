#include "logging.h"

#include <stdio.h>

DataFlowException::DataFlowException(const char *type, const char *error)
{
    sprintf(msg, "Throwing exception: (%s): %s", type, error);
    Logger::LogEvent(msg);
}

FILE* Logger::logger;

void Logger::LogEvent(const char *event)
{
    if (logger == NULL)
    {
        if (!(logger = fopen("logger", "w")))
            return;
    }

    fprintf(logger, "%s\n", event);
}

void Logger::Finalize()
{
    if (!logger)
        return;

    fflush(logger);
    fclose(logger);
    logger = NULL;

}
