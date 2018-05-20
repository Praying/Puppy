#include <base/common/Errors.hpp>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <cstdarg>

namespace Flow
{
    namespace Error
    {
        void Assert(const char *file, int line, const char *function, const char *message)
        {
            fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n %s\n", file, line, function, message);
            *((volatile int *) NULL) = 0;
            exit(1);
        }

        void Assert(const char *file, int line, const char *function, const char *message, const char *format, ...)
        {
            va_list args;
            va_start(args, format);
            fprintf(stderr, "\n%s:%i in %s ASSERTION FAILED:\n %s ", file, line, function, message);
            vfprintf(stderr, format, args);
            fprintf(stderr, "\n");
            fflush(stderr);
            va_end(args);
            *((volatile int *) NULL) = 0;
            exit(1);
        }


        void Fatal(const char *file, int line, const char *function, const char *message, ...)
        {
            va_list args;
            va_start(args, message);
            fprintf(stderr, "\n%s:%i in %s FATAL ERROR:\n ", file, line, function);
            vfprintf(stderr, message, args);
            fprintf(stderr, "\n");
            fflush(stderr);
            va_end(args);

            std::this_thread::sleep_for(std::chrono::seconds(10));
            *((volatile int *) NULL) = 0;
            exit(1);
        }

        void Warning(const char *file, int line, const char *function, const char *message)
        {
            fprintf(stderr, "\n%s:%i in %s WARNING:\n %s\n", file, line, function, message);
        }

        void Abort(const char *file, int line, const char *function)
        {
            fprintf(stderr, "\n%s:%i in %s ABORTED.\n", file, line, function);
            *((volatile int *) NULL);
        }

        void AbortHandler(int /*sigval*/)
        {
            *((volatile int *) NULL) = 0;
            exit(1);
        }
    }
}
