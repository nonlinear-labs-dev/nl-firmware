#include <device-info/FreeDiscSpaceInformation.h>
#include <glibmm/main.h>
#include <glibmm/ustring.h>
#include <sigc++/functors/mem_fun.h>
#include <nltools/system/SpawnCommandLine.h>
#include <xml/Writer.h>
#include <chrono>
#include <algorithm>
#include "nltools/system/AsyncCommandLine.h"
#include "nltools/system/SpawnAsyncCommandLine.h"
#include "Application.h"

FreeDiscSpaceInformation::FreeDiscSpaceInformation(DeviceInformation* parent)
    : DeviceInformationItem(parent)
    , m_value("N/A")
{
  refresh();

  using namespace std::chrono;
  constexpr minutes timeout(5);
  m_signalRefresh = Application::get().getMainContext()->signal_timeout().connect(
      sigc::mem_fun(this, &FreeDiscSpaceInformation::refresh), duration_cast<milliseconds>(timeout).count());
}

bool FreeDiscSpaceInformation::refresh()
{
  SpawnAsyncCommandLine::spawn(
      Application::get().getMainContext(),
      { "sh", "-c", "\"df", "-h", "|", "grep", "'persistent'", "|", "awk", "'{print $4}'\"" },
      [&](const std::string& success) {
        if(success.empty())
          m_value = "N/A";
        else
        {
          std::string trimmed {};
          std::copy_if(success.begin(), success.end(), std::back_inserter(trimmed),
                       [](const char o) { return std::isalnum(o); });
          if(m_value != trimmed)
          {
            m_value = trimmed;
            onChange();
          }
        }
      },
      [](auto err) {});
  return true;
}

void FreeDiscSpaceInformation::writeDocument(Writer& writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("free-disc-space", m_value);
}

Glib::ustring FreeDiscSpaceInformation::get() const
{
  return m_value;
}
