#pragma once

#include <parameters/mono-mode-parameters/MonoParameter.h>
#include "proxies/hwui/controls/Label.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <groups/MasterGroup.h>
#include <groups/ScaleGroup.h>
#include <groups/UnisonGroup.h>

class Application;
class Parameter;

class ModuleCaption : public Label
{
 private:
  typedef Label super;

 public:
  ModuleCaption(const Rect &pos);
  virtual ~ModuleCaption();

  bool redraw(FrameBuffer &fb) override;

  static bool enableVoiceGroupSuffix()
  {
    auto eb = Application::get().getPresetManager()->getEditBuffer();

    if(auto selected = dynamic_cast<MonoParameter *>(eb->getSelected()))
      return eb->getType() == SoundType::Split;
    if(auto selected = dynamic_cast<UnisonGroup *>(eb->getSelected()->getParent()))
      return eb->getType() == SoundType::Split;

    if(auto selected = dynamic_cast<MasterGroup *>(eb->getSelected()->getParent()))
      return false;
    if(auto selected = dynamic_cast<ScaleGroup *>(eb->getSelected()->getParent()))
      return false;

    return true;
  };

 private:
  virtual void setFontColor(FrameBuffer &fb) const override;
  virtual std::shared_ptr<Font> getFont() const override;
  virtual int getFontHeight() const override;

 protected:
  virtual void updateText(Parameter *newOne);

 private:
  void onParameterSelected(Parameter *newOne);
  void onSelectionChanged();
};