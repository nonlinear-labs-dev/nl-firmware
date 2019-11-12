#include "SoundInfoContent.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <presets/Preset.h>

SoundInfoContent::SoundInfoContent()
{
  addInfoField("presetlabel", "Sound Label");
  addInfoField("type", "Sound Type");
  addInfoField("comment", "Comment", new MultiLineInfoContent());

  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(eb->getType() != SoundType::Single)
  {
    addInfoField("I", "Label I");
    if(eb->getType() == SoundType::Split)
    {
      addInfoField("split", "Split Point");
    }
    addInfoField("II", "Label II");
  }

  eb->onChange(sigc::mem_fun(this, &SoundInfoContent::updateContent));
}

void SoundInfoContent::fillContents()
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  infoFields["presetlabel"]->setInfo(editBuffer->getName());

  if(auto origin = editBuffer->getOrigin())
    infoFields["comment"]->setInfo(origin->getAttribute("Comment", "---"));
  else
    infoFields["comment"]->setInfo("---");
  infoFields["type"]->setInfo(toString(editBuffer->getType()));

  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(eb->getType() == SoundType::Split)
  {
    infoFields["split"]->setInfo(eb->getSplitPoint()->getDisplayValue(VoiceGroup::II) + " - "
                                 + eb->getSplitPoint()->getDisplayValue(VoiceGroup::I));
  }

  if(eb->getType() != SoundType::Single)
  {
    infoFields["type"]->setInfo(toString(eb->getType()));
    infoFields["I"]->setInfo(eb->getVoiceGroupName(VoiceGroup::I));
    infoFields["II"]->setInfo(eb->getVoiceGroupName(VoiceGroup::II));
  }
}
