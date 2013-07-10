#include "Arduino.h"

// Definitions (could go somewhere else...)
#if is_computer()

  #include <sys/time.h>

  unsigned long millis(void) {
    return micros() / 1000;
  }

  // TODO: Reimplement for Windows
  unsigned long micros(void) {
    static struct timeval start = { 0, 0 };
    if (start.tv_sec == 0 && start.tv_usec == 0)
      gettimeofday(&start, NULL);

    struct timeval end;
      gettimeofday(&end, NULL);

    unsigned long long seconds  = end.tv_sec  - start.tv_sec;
    unsigned long long useconds = end.tv_usec - start.tv_usec;

    return (seconds * 1000000UL + useconds) /* + 0.5 */;
  }

  #ifdef WIN32

    #warning "millis(), micros(), delay() and delayMicroseconds() have not been tested under Windows"

    #include <windows.h>

    void delay(unsigned long milliseconds)
    {
      Sleep(milliseconds);
    }

    // Source: http://stackoverflow.com/questions/5801813/c-usleep-is-obsolete-workarounds-for-windows-mingw/11470617
    // XXX: Untested
    void delayMicroseconds(__int64 usec)
    {
        HANDLE timer;
        LARGE_INTEGER ft;

        ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time

        timer = CreateWaitableTimer(NULL, TRUE, NULL);
        SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);
    }

  #else // Unix / OSX

    #include <unistd.h>

    void delay(unsigned long ms)
    {
      usleep(ms*1000);
    }

    void delayMicroseconds(unsigned int us) {
      usleep(us);
    }

  #endif

#endif
