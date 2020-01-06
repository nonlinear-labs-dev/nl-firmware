#include "Options.h"
#include <glibmm/optiongroup.h>

Options::Options(int &argc, char **&argv)
{
  Glib::OptionGroup mainGroup("common", "common options");
  Glib::OptionContext ctx;

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

  ctx.parse(argc, argv);
}

Glib::ustring Options::getPlaygroundHost() const
{
  return m_playgroundHost;
}

Glib::ustring Options::getAudioEngineHost() const
{
  return m_audioengineHost;
}