#include <device-info/SoftwareVersion.h>
#include <xml/Writer.h>
#include <xml/Attribute.h>
#include <device-settings/DebugLevel.h>
#include <version.h>

SoftwareVersion::SoftwareVersion(DeviceInformation *parent)
    : DeviceInformationItem(parent)
    , m_versionString(GetC15Version())
{
}

void SoftwareVersion::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("software-version", m_versionString);
}

Glib::ustring SoftwareVersion::get() const
{
  return m_versionString;
}
