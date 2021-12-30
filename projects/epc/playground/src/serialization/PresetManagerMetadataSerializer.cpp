#include "PresetManagerMetadataSerializer.h"
#include "PresetBankOrderSerializer.h"
#include "EditBufferSerializer.h"
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>

PresetManagerMetadataSerializer::PresetManagerMetadataSerializer(PresetManager *pm, Progress progress)
    : Serializer(getTagName(), progress)
    , m_pm(pm)
{
}

Glib::ustring PresetManagerMetadataSerializer::getTagName()
{
  return "preset-manager";
}

void PresetManagerMetadataSerializer::writeTagContent(Writer &writer) const
{
  EditBufferSerializer eb(m_pm->getEditBuffer(), getProgressCB());
  eb.write(writer);

  PresetBankOrderSerializer bankOrder(m_pm);
  bankOrder.write(writer);
}

void PresetManagerMetadataSerializer::readTagContent(Reader &reader) const
{
  reader.onTag(EditBufferSerializer::getTagName(),
               [&](const auto &) mutable { return new EditBufferSerializer(m_pm->getEditBuffer(), getProgressCB()); });

  reader.onTag(PresetBankOrderSerializer::getTagName(),
               [&](const auto &) mutable { return new PresetBankOrderSerializer(m_pm); });
}
