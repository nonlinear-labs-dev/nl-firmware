#include "Options.h"
#include "CompileTimeOptions.h"
#include <glibmm/optiongroup.h>
#include "device-settings/DebugLevel.h"
#include "Application.h"
#include <giomm.h>
#include <nltools/ErrorCodes.h>
#include <filesystem>

bool Options::s_acceptanceTests = false;

Options::Options()
{
  if(s_acceptanceTests)
    setDefaultsForTests();
  else
    setDefaults();
}

Options::Options(int &argc, char **&argv)
{
  if(s_acceptanceTests)
    setDefaultsForTests();
  else
    setDefaults();

  Glib::OptionGroup mainGroup("common", "common options");
  Glib::OptionContext ctx;

  Glib::OptionEntry pmPath;
  pmPath.set_flags(Glib::OptionEntry::FLAG_FILENAME);
  pmPath.set_long_name("pm-path");
  pmPath.set_short_name('p');
  pmPath.set_description("Name of the folder that stores preset-managers banks as XML files");
  mainGroup.add_entry_filename(pmPath, sigc::mem_fun(this, &Options::setPMPathName));

  Glib::OptionEntry layoutPath;
  layoutPath.set_flags(Glib::OptionEntry::FLAG_FILENAME);
  layoutPath.set_long_name("layouts");
  layoutPath.set_short_name('l');
  layoutPath.set_description("name of the folder containing the styles, controls and layouts");
  mainGroup.add_entry_filename(layoutPath, sigc::mem_fun(this, &Options::setLayoutFolder));

  Glib::OptionEntry bbbb;
  bbbb.set_long_name("bbbb-host");
  bbbb.set_short_name('b');
  bbbb.set_description("Where to find the bbbb");
  mainGroup.add_entry(bbbb, m_bbbb);

  Glib::OptionEntry midiBridge;
  midiBridge.set_long_name("midi-bridge");
  midiBridge.set_short_name('m');
  midiBridge.set_description("Where to find the Midi Bridge");
  mainGroup.add_entry(midiBridge, m_midiBridge);

  Glib::OptionEntry ae;
  ae.set_long_name("audio-engine-host");
  ae.set_short_name('a');
  ae.set_description("Where to find the audio-engine");
  mainGroup.add_entry(ae, m_audioEngineHost);

  Glib::OptionEntry turn;
  turn.set_long_name("bbbb-turnaround-times");
  turn.set_short_name('t');
  turn.set_arg_description("measure turn around times for knob to BOLED messages");
  mainGroup.add_entry(turn, m_sendBBBTurnaroundTimes);

  ctx.set_main_group(mainGroup);
  ctx.set_help_enabled(true);

  try
  {
    ctx.parse(argc, argv);
  }
  catch(const Glib::OptionError &optErr)
  {
    nltools::Log::error(optErr.what());
    std::exit(static_cast<int>(nltools::ErrorCode::UnknownOptionKeyError));
  }
  catch(const Glib::ConvertError &convErr)
  {
    nltools::Log::error(convErr.what());
    std::exit(static_cast<int>(nltools::ErrorCode::OptionConvertError));
  }
  catch(...)
  {
    std::stringstream ss;
    for(auto i = 0; i < argc; i++)
      ss << argv[i] << " ";
    nltools::Log::error(__FILE__, __FUNCTION__, __LINE__, "Could not parse args:", ss.str());
    std::exit(static_cast<int>(nltools::ErrorCode::UnknownOptionKeyError));
  }
}

void Options::setDefaultsForTests()
{
  auto persistencePath = std::filesystem::path("/tmp");

  auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
  auto fine = std::chrono::high_resolution_clock::now().time_since_epoch().count();

  auto settingsName = std::string("/tmp/settings") + std::to_string(fine) + std::string(".xml");
  auto settingsFile = std::filesystem::path(settingsName);
  auto pmName = std::string("/tmp/preset-manager-") + std::to_string(timestamp) + "-" + std::to_string(fine) + std::string("/");
  auto pmDirectory = std::filesystem::path(pmName);

  if(is_directory(persistencePath))
  {
    if(not is_directory(pmDirectory))
      std::filesystem::create_directories(pmDirectory);
    m_pmPath = pmDirectory.string();
    m_settingsFile = settingsFile.string();
  }

  m_layoutFolder = getResourcesDir() + std::string("/templates/");
}

void Options::setDefaults()
{
  auto persistencePath = Gio::File::create_for_path("/persistent");
  auto settingsFile = Gio::File::create_for_path("/persistent/settings.xml");
  auto pmDirectory = Gio::File::create_for_path("/persistent/preset-manager/");

  if(persistencePath->query_exists())
  {
    if(pmDirectory->query_exists() || makePresetManagerDirectory(pmDirectory))
      m_pmPath = pmDirectory->get_path();

    m_settingsFile = settingsFile->get_path();
  }
  else
  {
    m_pmPath = "./preset-manager/";
    m_settingsFile = ".settings.xml";
  }

  m_layoutFolder = getResourcesDir() + std::string("/templates/");
}

bool Options::makePresetManagerDirectory(const Glib::RefPtr<Gio::File> &file)
{
  try
  {
    return file->make_directory();
  }
  catch(...)
  {
  }
  return false;
}

bool Options::setPMPathName(const Glib::ustring &, const Glib::ustring &path, bool hasValue)
{
  if(hasValue)
    m_pmPath = path;

  return true;
}

bool Options::setLayoutFolder(const Glib::ustring &, const Glib::ustring &path, bool hasValue)
{
  if(hasValue)
    m_layoutFolder = path;

  return true;
}

const Glib::ustring &Options::getPresetManagerPath() const
{
  return m_pmPath;
}

const Glib::ustring &Options::getBBBB() const
{
  return m_bbbb;
}

const Glib::ustring &Options::getAudioEngineHost() const
{
  return m_audioEngineHost;
}

const Glib::ustring &Options::getMidiBridge() const
{
  return m_midiBridge;
}

const Glib::ustring &Options::getSettingsFile() const
{
  return m_settingsFile;
}

const Glib::ustring &Options::getLayoutFolder() const
{
  return m_layoutFolder;
}

bool Options::sendBBBBTurnaroundTimestamps() const
{
  return m_sendBBBTurnaroundTimes;
}

void Options::setPresetManagerPath(const std::string& path)
{
  m_pmPath = path;
}
