#include "Arduino.h"

// Definitions (could go somewhere else...)
#if is_computer()

#include <time.h>

  unsigned long millis(void) {
    return clock() * 1000 / CLOCKS_PER_SEC;
  }
  unsigned long micros(void) {
    return clock() * 1000000UL / CLOCKS_PER_SEC;
  }

  #ifdef WIN32

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

    void delay(unsigned long milliseconds)
    {
      usleep(milliseconds * 1000); // takes microseconds
    }

    void delayMicroseconds(unsigned int us) {
      usleep(us);
    }

  #endif

#endif
