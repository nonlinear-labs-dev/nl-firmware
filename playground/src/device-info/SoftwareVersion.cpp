#include <device-info/SoftwareVersion.h>
#include <xml/Writer.h>
#include <xml/Attribute.h>
#include <device-settings/DebugLevel.h>

SoftwareVersion::SoftwareVersion(DeviceInformation *parent)
    : DeviceInformationItem(parent)
    , m_versionString("2019-10-23 12:10")
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
