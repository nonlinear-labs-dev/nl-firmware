#include "bbbb.h"
#include <stdlib.h>
#include <execinfo.h>
#include <iostream>
#include <chrono>
#include "Application.h"
#include <ui/Window.h>


void printStackTrace (int i)
{
  TRACE("Crash signal caught!");

  const size_t max_frames = 64;
  void* addrlist[max_frames + 1];

  // retrieve current stack addresses
  guint32 addrlen = backtrace (addrlist, sizeof(addrlist) / sizeof(void*));

  if (addrlen == 0)
  {
    TRACE("");
    return;
  }

  TRACE ("\n\nThe stack trace:");

  // create readable strings to each frame. __attribute__((no_instrument_function))
  char** symbollist = backtrace_symbols (addrlist, addrlen);

  // print the stack trace.
  for (guint32 i = 0; i < addrlen; i++)
    TRACE (symbollist[i]);

  free (symbollist);
  exit (EXIT_FAILURE);
}

int main (int numArgs, char **argv)
{
  Gio::init ();

  ::signal (SIGSEGV, printStackTrace);
  ::signal (SIGILL, printStackTrace);
  ::signal (SIGBUS, printStackTrace);

  {
    Application app (numArgs, argv);
    Application::get ().runWithWindow ();
  }

  return EXIT_SUCCESS;
}
