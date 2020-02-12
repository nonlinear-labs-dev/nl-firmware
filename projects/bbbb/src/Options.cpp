#include "Options.h"
#include <glibmm/optiongroup.h>
#include <glibmm/optioncontext.h>
#include <nltools/logging/Log.h>

namespace OptionDetail
{
  template <typename T>
  inline Glib::OptionEntry createOption(Glib::OptionGroup& ctx, const char* l, char s, const char* desc, T& dest)
  {
    Glib::OptionEntry entry;
    entry.set_long_name(l);
    entry.set_short_name(s);
    entry.set_description(desc);
    ctx.add_entry(entry, dest);
 
    return entry;
  }
}

Options::Options(int &argc, char **&argv)
{
  Glib::OptionGroup mainGroup("common", "common options");
  Glib::OptionContext ctx;

  auto timeStamps = OptionDetail::createOption(
      mainGroup, "timestamps", 't', "measure turn around time encoder -> playground -> oled", m_doTimeStamps);

  auto pgHost
      = OptionDetail::createOption(mainGroup, "playground-host", 'p', "where to find the playground", m_playgroundHost);

  auto aeHost = OptionDetail::createOption(mainGroup, "audioengine-host", 'a', "where to find the audio engine",
                                           m_audioengineHost);

  Glib::OptionEntry pg;
  pg.set_long_name("playground-host");
  pg.set_short_name('p');
  pg.set_description("Where to find the playground");
  mainGroup.add_entry(pg, m_playgroundHost);

  Glib::OptionEntry ae;
  pg.set_long_name("audioengine-host");
  pg.set_short_name('p');
  pg.set_description("Where to find the audioengine");
  mainGroup.add_entry(ae, m_audioengineHost);

  ctx.set_main_group(mainGroup);
  ctx.set_help_enabled(true);

  try {
    ctx.parse(argc, argv);
  } catch(...) {
    std::stringstream ss;
    for(auto i = 0; i < argc; i++)
      ss << argv[i] << " ";
    nltools::Log::error(__FILE__, __FUNCTION__, __LINE__, "Could not parse args:", ss.str());
  }
}

Glib::ustring Options::getPlaygroundHost() const
{
  return m_playgroundHost;
}

Glib::ustring Options::getAudioEngineHost() const
{
  return m_audioengineHost;
}

bool Options::doTimeStamps() const
{
  return m_doTimeStamps;
}
