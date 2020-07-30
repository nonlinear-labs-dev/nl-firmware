#include "RTSoftwareVersion.h"
#include <xml/Writer.h>
#include <xml/Attribute.h>
#include <device-settings/DebugLevel.h>
#include <Application.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>

RTSoftwareVersion::RTSoftwareVersion(DeviceInformation *parent)
    : DeviceInformationItem(parent)
{
  m_connection = Application::get().getPlaycontrollerProxy()->onPlaycontrollerSoftwareVersionChanged(
      sigc::mem_fun(this, &RTSoftwareVersion::onVersionChanged));
}

void RTSoftwareVersion::onVersionChanged(int v)
{
  onChange();
}

void RTSoftwareVersion::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("rt-software-version", get());
}

int RTSoftwareVersion::get() const
{
  return Application::get().getPlaycontrollerProxy()->getPlaycontrollerSoftwareVersion();
}

