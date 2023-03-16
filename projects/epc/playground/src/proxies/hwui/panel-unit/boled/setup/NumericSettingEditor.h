#pragma once

#include "SetupLabel.h"
#include "SetupEditor.h"
#include "use-cases/SettingsUseCases.h"
#include <Application.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/HWUI.h>

template <typename tSetting> class NumericSettingEditor : public SetupLabel, public SetupEditor
{
 public:
  NumericSettingEditor()
      : SetupLabel({ 0, 0, 0, 0 })
  {
    auto setting = Application::get().getSettings()->getSetting<tSetting>();
    setting->onChange(mem_fun(this, &NumericSettingEditor<tSetting>::onSettingChanged));
    auto hwui = Application::get().getHWUI();
    hwui->onModifiersChanged(mem_fun(this, &NumericSettingEditor<tSetting>::onModifiersChanged));
  }

  void onModifiersChanged(ButtonModifiers mod)
  {
    m_modifiers = mod;
    updateLabelText();
  }

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override
  {
    if(down && i == Buttons::BUTTON_FINE)
    {
      Application::get().getHWUI()->toggleFine();
      return true;
    }
    if(down && i == Buttons::BUTTON_DEFAULT)
    {
      auto setting = Application::get().getSettings()->getSetting<tSetting>();
      SettingsUseCases settingsUseCases(*Application::get().getSettings());
      settingsUseCases.factoryDefaultSetting(setting);
      return true;
    }

    return SetupEditor::onButton(i, down, modifiers);
  }

  bool onRotary(int inc, ButtonModifiers modifiers) override
  {
    Application::get().getSettings()->getSetting<tSetting>()->incDec(inc, modifiers);
    return true;
  }

  void onSettingChanged(const Setting *s)
  {
    if(auto setting = dynamic_cast<const tSetting *>(s))
    {
      m_lastDisplayValue = setting->getDisplayString();
      updateLabelText();
    }
  }

 protected:
  void updateLabelText()
  {
    if(m_modifiers[ButtonModifier::FINE])
    {
      setText({ m_lastDisplayValue + " F", 1 });
    }
    else
    {
      setText({ m_lastDisplayValue, 0 });
    }
  }

  void setSuffixFontColor(FrameBuffer &fb) const override
  {
    fb.setColor(FrameBufferColors::C179);
  }

 private:
  void setBackgroundColor(FrameBuffer &fb) const override
  {
    fb.setColor(FrameBufferColors::C103);
  }

  Font::Justification getJustification() const override
  {
    return Font::Justification::Left;
  }

  bool redraw(FrameBuffer &fb) override
  {
    SetupLabel::redraw(fb);

    fb.setColor(FrameBufferColors::C179);
    fb.drawRect(getPosition());
    return true;
  }

  void setFontColor(FrameBuffer &fb) const override
  {
    fb.setColor(FrameBufferColors::C255);
  }

  ButtonModifiers m_modifiers;
  Glib::ustring m_lastDisplayValue;
};

template <typename tSetting> class BooleanSettingEditor : public SetupLabel, public SetupEditor
{
 public:
  BooleanSettingEditor()
      : SetupLabel({ 0, 0, 0, 0 })
  {
    auto setting = Application::get().getSettings()->getSetting<tSetting>();
    setting->onChange(mem_fun(this, &BooleanSettingEditor<tSetting>::onSettingChanged));
  }

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override
  {
    if(down && i == Buttons::BUTTON_DEFAULT)
    {
      auto setting = Application::get().getSettings()->getSetting<tSetting>();
      SettingsUseCases settingsUseCases(*Application::get().getSettings());
      settingsUseCases.factoryDefaultSetting(setting);
      return true;
    }

    return SetupEditor::onButton(i, down, modifiers);
  }

  bool onRotary(int inc, ButtonModifiers modifiers) override
  {
    Application::get().getSettings()->getSetting<tSetting>()->toggle();
    return true;
  }

  void onSettingChanged(const Setting *s)
  {
    auto setting = dynamic_cast<const tSetting *>(s);
    setText({ setting->getDisplayString(), 0 });
  }

 private:
  void setBackgroundColor(FrameBuffer &fb) const override
  {
    fb.setColor(FrameBufferColors::C103);
  }

  Font::Justification getJustification() const override
  {
    return Font::Justification::Left;
  }

  bool redraw(FrameBuffer &fb) override
  {
    SetupLabel::redraw(fb);

    fb.setColor(FrameBufferColors::C179);
    fb.drawRect(getPosition());
    return true;
  }

  void setFontColor(FrameBuffer &fb) const override
  {
    fb.setColor(FrameBufferColors::C255);
  }
};