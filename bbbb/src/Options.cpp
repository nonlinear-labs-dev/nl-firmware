#include "Options.h"
#include <glibmm/optiongroup.h>

Options::Options(int &argc, char **&argv)
{
  Glib::OptionGroup mainGroup("common", "common options");
  Glib::OptionContext ctx;

  Glib::OptionEntry lpcSendFileName;
  lpcSendFileName.set_flags(Glib::OptionEntry::FLAG_FILENAME);
  lpcSendFileName.set_long_name("lpc-send-filename");
  lpcSendFileName.set_short_name('s');
  lpcSendFileName.set_description("name of the file used to talk to the LPC");
  mainGroup.add_entry_filename(lpcSendFileName,
      sigc::hide<0>(sigc::hide<1>(sigc::mem_fun(this, &Options::setLPCSendFileName))));

  Glib::OptionEntry lpcReceiveFileName;
  lpcReceiveFileName.set_flags(Glib::OptionEntry::FLAG_FILENAME);
  lpcReceiveFileName.set_long_name("lpc-receive-filename");
  lpcReceiveFileName.set_short_name('r');
  lpcReceiveFileName.set_description("name of the file used to receive messages from the LPC");
  mainGroup.add_entry_filename(lpcReceiveFileName,
      sigc::hide<0>(sigc::hide<1>(sigc::mem_fun(this, &Options::setLPCReceiveFileName))));

  ctx.set_main_group(mainGroup);
  ctx.set_help_enabled(true);

  ctx.parse(argc, argv);
}

Options::~Options()
{
}

const char *Options::getEncoderFileName() const
{
  return m_encoderFileName.c_str();
}

const char *Options::getButtonsFileName() const
{
  return m_buttonsFileName.c_str();
}

const char *Options::getLPCSendFileName() const
{
  return m_LPCSendFileName.c_str();
}

bool Options::setLPCSendFileName(const Glib::ustring &name)
{
  m_LPCSendFileName = name;
  return true;
}

const char *Options::getLPCReceiveFileName() const
{
  return m_LPCReceiveFileName.c_str();
}

bool Options::setLPCReceiveFileName(const Glib::ustring &name)
{
  m_LPCReceiveFileName = name;
  return true;
}

const char *Options::forwardLPCMessagesTo() const
{
  return "192.168.8.3:5001";
}

const char *Options::forwardEncoderMessagesTo() const
{
  return "192.168.8.3:5002";
}

const char *Options::forwardButtonMessagesTo() const
{
  return "192.168.8.3:5002";
}

int Options::receiveMessagesForLpcOnPort() const
{
  return 6000;
}

int Options::receiveMessagesForOledsOnPort() const
{
  return 6001;
}

const char *Options::getFrameBufferFileName() const
{
  return "/dev/fb0";
}

