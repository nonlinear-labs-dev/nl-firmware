#include "CommandLineInterface.h"
#include <iostream>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int8_t getch()
{
  int ch;
  struct termios old_t, tmp_t;

  if(tcgetattr(STDIN_FILENO, &old_t))
    return -1;

  memcpy(&tmp_t, &old_t, sizeof(old_t));
  tmp_t.c_lflag &= ~ICANON & ~ECHO;

  if(tcsetattr(STDIN_FILENO, TCSANOW, (const struct termios *) &tmp_t))
    return -1;

  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, (const struct termios *) &old_t);
  return static_cast<int8_t>(ch);
}

CommandLineInterface::CommandLineInterface()
{
  if(isatty(fileno(stdin)))
    Glib::signal_timeout().connect(sigc::mem_fun(this, &CommandLineInterface::onIO), 100);
}

CommandLineInterface::~CommandLineInterface()
{
}

bool CommandLineInterface::onIO()
{
  auto c = getch();

  if(c >= 0)
    processByte(c);

  return true;
}
