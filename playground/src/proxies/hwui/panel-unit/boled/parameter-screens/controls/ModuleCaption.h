#pragma once

#include "proxies/hwui/controls/Label.h"

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

  virtual bool enableVoiceGroupSuffix() const {
    return true;
  };

 private:
  virtual void setFontColor(FrameBuffer &fb) const override;
  virtual std::shared_ptr<Font> getFont() const override;
  virtual int getFontHeight() const override;

  void onParameterSelected(Parameter *newOne);
  void onSelectionChanged();
};


class MonoModuleCaption : public ModuleCaption
{
public:
  explicit MonoModuleCaption(const Rect& r);
  bool enableVoiceGroupSuffix() const override;
};