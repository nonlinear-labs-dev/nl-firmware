#include "Options.h"
#include <glibmm/optiongroup.h>

Options::Options(int &argc, char **&argv)
{
  OptionGroup mainGroup("common", "common options");
  OptionEntry scriptFile;

  scriptFile.set_flags(OptionEntry::FLAG_FILENAME);
  scriptFile.set_long_name("script");
  scriptFile.set_short_name('s');
  scriptFile.set_description("name of the file that describes the test");

  mainGroup.add_entry_filename(scriptFile, sigc::mem_fun(this, &Options::setScriptFile));
  m_ctx.set_main_group(mainGroup);
  m_ctx.set_help_enabled(true);

  if(!m_ctx.parse(argc, argv))
  {
    showHelp();
  }
}

Options::~Options()
{
}

void Options::showHelp()
{
  WARN(m_ctx.get_help());
}

string Options::getScript() const
{
  return m_script;
}

bool Options::setScriptFile(const string &optionName, const string &file, bool hasValue)
{
  if(hasValue)
  {
    m_script = file;
    return true;
  }
  return false;
}
