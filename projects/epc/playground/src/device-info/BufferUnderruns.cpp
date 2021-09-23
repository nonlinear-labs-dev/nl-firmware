#include <device-info/BufferUnderruns.h>
#include <xml/Writer.h>
#include <nltools/messaging/Message.h>

BufferUnderruns::BufferUnderruns(DeviceInformation *parent)
    : DeviceInformationItem(parent)
{
  m_msgConnection = nltools::msg::receive<nltools::msg::BufferUnderrunsChangedMessage>(
      nltools::msg::EndPoint::Playground, [this](const auto &msg) {
        if(std::exchange(m_numUnderruns, msg.numUnderruns) != msg.numUnderruns)
          onChange();
      });
}

BufferUnderruns::~BufferUnderruns()
{
  m_msgConnection.disconnect();
}

void BufferUnderruns::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("buffer-underruns", get());
}

Glib::ustring BufferUnderruns::get() const
{
  return std::to_string(m_numUnderruns);
}
