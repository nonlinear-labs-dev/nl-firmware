#include "SoundConditions.h"

DescriptiveLayouts::SoundConditions::EditBufferCondition::EditBufferCondition()
{
  m_editbufferChangedConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &EditBufferCondition::onEditBufferChanged));
}

void DescriptiveLayouts::SoundConditions::EditBufferCondition::onEditBufferChanged()
{
  update();
}

DescriptiveLayouts::SoundConditions::EditBufferCondition::~EditBufferCondition()
{
  m_editbufferChangedConnection.disconnect();
}

bool DescriptiveLayouts::SoundConditions::IsSingleSound::check() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->getType() == SoundType::Single;
}

bool DescriptiveLayouts::SoundConditions::IsLayerSound::check() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->getType() == SoundType::Layer;
}

bool DescriptiveLayouts::SoundConditions::IsSplitSound::check() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->getType() == SoundType::Split;
}
