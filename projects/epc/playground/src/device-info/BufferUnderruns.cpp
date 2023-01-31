#include <device-info/BufferUnderruns.h>
#include <xml/Writer.h>
#include <nltools/messaging/Message.h>

BufferUnderruns::BufferUnderruns(DeviceInformation *parent)
    : DeviceInformationItem(parent)
{
  m_msgConnection = nltools::msg::receive<nltools::msg::BufferUnderrunsChangedMessage>(
      nltools::msg::EndPoint::Playground, [this](const auto &msg) {
        auto changed = std::exchange(m_numUnderruns, msg.numUnderruns) != msg.numUnderruns;
        changed |= std::exchange(m_framesPerPeriod, msg.framesPerPeriod) != msg.framesPerPeriod;
        changed |= std::exchange(m_numPeriods, msg.numPeriods) != msg.numPeriods;

        if(changed)
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
  return nltools::string::concat(m_numUnderruns, " @ ", m_numPeriods, "p x ", m_framesPerPeriod, "fpp");
}
