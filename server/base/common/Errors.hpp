#pragma once

namespace Flow
{
    namespace Error
    {
        void Assert(const char *file, int line, const char *function, const char *message);

        void Assert(const char *file, int line, const char *function, const char *message, const char *format, ...);

        void Fatal(const char *file, int line, const char *function, const char *message, ...);

        void Error(const char *file, int line, const char *function, const char *message);

        void Abort(const char *file, int line, const char *function);

        void Warning(const char *file, int line, const char *function, const char *message);

        void AbortHandler(int sigval);
    }

}

#define FLOW_ASSERT(cond, ...) do{if(!(cond)) Flow::Error::Assert(__FILE__, __LINE__, __FUNCTION__,#cond, ##__VA_ARGS__);} while(0)
#define FLOW_FATAL(cond, ...) do{if(!(cond)) Flow::Error::Fatal(__FILE__, __LINE__, __FUNCTION__,#cond, ##__VA_ARGS__);} while(0)
#define FLOW_ABORT() do{Flow::Error::Abort(__FILE__, __LINE__, __FUNCTION__);}while(0);