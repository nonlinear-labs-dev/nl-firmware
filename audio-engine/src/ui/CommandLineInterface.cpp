#include "CommandLineInterface.h"

CommandLineInterface::CommandLineInterface()
    : m_readersCancel(Gio::Cancellable::create())
{
  if(isatty(fileno(stdin)))
  {
    m_keyboardInput = Gio::DataInputStream::create(Gio::UnixInputStream::create(0, true));
    m_keyboardInput->read_line_async(mem_fun(this, &CommandLineInterface::onKeyboardLineRead), m_readersCancel);
  }
}

CommandLineInterface::~CommandLineInterface()
{
  m_readersCancel->cancel();
}

void CommandLineInterface::onKeyboardLineRead(Glib::RefPtr<Gio::AsyncResult> &res)
{
  std::string line;

  if(m_keyboardInput->read_line_finish(res, line))
    if(!line.empty())
      processLine(line);

  m_keyboardInput->read_line_async(mem_fun(this, &CommandLineInterface::onKeyboardLineRead), m_readersCancel);
}
