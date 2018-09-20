#include "Options.h"
#include <glibmm/optiongroup.h>

Options::Options(int &argc, char **&argv)
{
  Glib::OptionGroup mainGroup("common", "common options");
  Glib::OptionContext ctx;

  Glib::OptionEntry doTimestamps;
  doTimestamps.set_long_name("timestamps");
  doTimestamps.set_short_name('t');
  doTimestamps.set_description("measure turn around time encoder -> playground -> oled");
  mainGroup.add_entry(doTimestamps, m_doTimeStamps);

  ctx.set_main_group(mainGroup);
  ctx.set_help_enabled(true);

  ctx.parse(argc, argv);
}

Options::~Options()
{
}

bool Options::doTimeStamps() const
{
  return m_doTimeStamps;
}
