#include "SoundInfoContent.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <presets/Preset.h>

SoundInfoContent::SoundInfoContent()
{
  addInfoField("presetlabel", "Sound Label", new MultiLineContent());
  addInfoField("comment", "Comment", new MultiLineContent());
  addInfoField("type", "Sound Type");

  Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &SoundInfoContent::updateContent));
}

void SoundInfoContent::fillContents()
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
  infoFields["presetlabel"]->setInfo(editBuffer->getName());

  if(auto origin = editBuffer->getOrigin())
    infoFields["comment"]->setInfo(origin->getAttribute("Comment", "---"), FrameBuffer::Colors::C128);
  else
    infoFields["comment"]->setInfo("---");
  infoFields["type"]->setInfo(toString(editBuffer->getType()));
}
