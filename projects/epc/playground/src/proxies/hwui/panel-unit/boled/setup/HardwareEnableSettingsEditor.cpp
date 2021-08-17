#include "HardwareEnableSettingsEditor.h"
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <proxies/hwui/controls/CenterAlignedLabel.h>
#include <proxies/hwui/controls/Button.h>
#include <cmath>
#include <use-cases/SettingsUseCases.h>
#include <proxies/hwui/Oleds.h>
#include "HardwareEnableSettingsHelper.h"

inline auto getPreviewNameForAspect(RoutingSettings::tAspectIndex a)
{
  typedef RoutingSettings::tAspectIndex ASP;
  switch(a)
  {
    case ASP::SEND_PRIMARY:
      return "Send Prim.";
    case ASP::RECEIVE_PRIMARY:
      return "Rec. Prim.";
    case ASP::SEND_SPLIT:
      return "Send Split";
    case ASP::RECEIVE_SPLIT:
      return "Rec. Split";
    case ASP::LOCAL:
      return "Local";
    default:
    case ASP::LENGTH:
      return "";
  }
};

class AspectList : public Control
{
 public:
  AspectList(RoutingSettings::tRoutingIndex e, RoutingSettings::tAspectIndex a, const Rect& r);
  void update(RoutingSettings::tRoutingIndex entry, RoutingSettings::tAspectIndex aspect);

 public:
  bool redraw(FrameBuffer& fb) override;

 private:
  bool getState() const
  {
    return getSetting()->getState(entry, aspect);
  }

  bool getState(RoutingSettings::tAspectIndex a) const
  {
    return getSetting()->getState(entry, a);
  }

  RoutingSettings::tRoutingIndex entry;
  RoutingSettings::tAspectIndex aspect;
};

void AspectList::update(RoutingSettings::tRoutingIndex e, RoutingSettings::tAspectIndex a)
{
  entry = e;
  aspect = a;
  setDirty();
}

bool AspectList::redraw(FrameBuffer& fb)
{
  constexpr int paddingX = 1;
  constexpr int paddingY = 1;

  using ASP = RoutingSettings::tAspectIndex;
  const auto& pos = getPosition();
  auto y = pos.getTop();
  const auto x = pos.getLeft();
  const auto w = pos.getWidth();
  const auto totalHeight = pos.getHeight();
  const auto aspectH = totalHeight / 5;

  Label l(getPosition());
  l.setFontColor(FrameBufferColors::C43);

  for(auto a : getAspectsForIndex(entry))
  {
    auto aspPos = Rect(x + paddingX, y + paddingY, w - paddingX, aspectH - paddingY);

    auto isEnabled = getState(a);
    if(isEnabled)
      fb.setColor(FrameBufferColors::C128);
    else
      fb.setColor(FrameBufferColors::C77);

    fb.fillRect(aspPos);

    l.setPosition(aspPos);
    l.setText(getPreviewNameForAspect(a));
    l.redraw(fb);

    auto isSelected = a == aspect;
    if(isSelected)
    {
      fb.setColor(FrameBufferColors::C204);
      fb.drawRect(aspPos);
    }

    y += aspectH;
  }

  return true;
}

AspectList::AspectList(RoutingSettings::tRoutingIndex e, RoutingSettings::tAspectIndex a, const Rect& r)
    : Control(r)
    , entry { e }
    , aspect { a }
{
}

class VariableCenterAlignedLabel : public CenterAlignedLabel
{
 public:
  using CenterAlignedLabel::CenterAlignedLabel;

  enum class FontFamily
  {
    Regular8,
    Regular9,
    Bold8,
    Bold9
  };

  void setFamily(FontFamily fam)
  {
    if(m_family != fam)
    {
      m_family = fam;
      setDirty();
    }
  }

  std::shared_ptr<Font> getFont() const override
  {
    auto& oled = Oleds::get();
    switch(m_family)
    {
      case FontFamily::Regular8:
        return oled.getFont("Emphase-8-Regular", 8);
      case FontFamily::Regular9:
        return oled.getFont("Emphase-9-Regular", 9);
      case FontFamily::Bold8:
        return oled.getFont("Emphase-8-Bold", 8);
      case FontFamily::Bold9:
        return oled.getFont("Emphase-9-Bold", 9);
    }

    return Oleds::get().getFont("Emphase-8-Regular", getFontHeight());
  }

 private:
  FontFamily m_family;
};

class SetupModuleHeader : public Label
{
 public:
  using Label::Label;

  bool redraw(FrameBuffer& fb) override
  {
    fb.setColor(FrameBufferColors::C128);
    fb.fillRect(getPosition());
    return Label::redraw(fb);
  }

 protected:
  void setFontColor(FrameBuffer& fb) const override
  {
    fb.setColor(FrameBufferColors::C43);
  }
};

HardwareEnableSettingsEditor::HardwareEnableSettingsEditor(RoutingSettings::tRoutingIndex id)
    : MenuEditor()
    , m_id { id }
{
  clear();

  if(Application::exists())
  {
    auto setting = Application::get().getSettings()->getSetting<RoutingSettings>();
    setting->onChange(sigc::mem_fun(this, &HardwareEnableSettingsEditor::onSettingChanged));
  }

  addControl(new SetupModuleHeader({ "Routings" }, { 0, 0, 64, 16 }));

  m_entryLabel = addControl(new VariableCenterAlignedLabel("Entry", { 64, 0, 128, 16 }));
  m_entryLabel->setFamily(VariableCenterAlignedLabel::FontFamily::Bold9);
  m_entryLabel->setHighlight(true);
  m_aspectLabel = addControl(new VariableCenterAlignedLabel("Aspect", { 64, 16, 128, 16 }));
  m_aspectLabel->setFamily(VariableCenterAlignedLabel::FontFamily::Regular9);
  m_valueLabel = addControl(new VariableCenterAlignedLabel("Value", { 64, 32, 128, 16 }));
  m_valueLabel->setFamily(VariableCenterAlignedLabel::FontFamily::Regular8);

  m_aspectList = addControl(new AspectList(m_id, m_aspect, { 192, 0, 64, 64 }));
  addControl(new Button("Back", Buttons::BUTTON_A));
  addControl(new Button("<", Buttons::BUTTON_B));
  addControl(new Button(">", Buttons::BUTTON_C));
  update();
}

void HardwareEnableSettingsEditor::setPosition(const Rect& r)
{
  static const Rect menuEditorPosition(0, 0, 256, 96);
  ControlWithChildren::setPosition(menuEditorPosition);
}

void HardwareEnableSettingsEditor::drawBackground(FrameBuffer& fb)
{
  fb.setColor(FrameBufferColors::C128);
  fb.fillRect(getPosition());
}

void HardwareEnableSettingsEditor::incSetting(int inc)
{
  if(Application::exists())
  {
    static auto settings = Application::get().getSettings();
    SettingsUseCases useCases(settings);
    auto idx = static_cast<int>(m_id);
    auto asp = static_cast<int>(m_aspect);
    useCases.updateRoutingAspect(idx, asp, inc > 0);
    update();
  }
}

const std::vector<Glib::ustring>& HardwareEnableSettingsEditor::getDisplayStrings() const
{
  static std::vector<Glib::ustring> unused = { "" };
  return unused;
}

int HardwareEnableSettingsEditor::getSelectedIndex() const
{
  constexpr auto unused = 0;
  return unused;
}

bool HardwareEnableSettingsEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  auto doIfDownAndSwallowAll = [isDown = down, pressed = i](Buttons b, auto cb)
  {
    if(pressed == b)
    {
      if(isDown)
        cb();
      return true;
    }
    return false;
  };

  bool isShiftDown = modifiers[ButtonModifier::SHIFT];

  if(doIfDownAndSwallowAll(Buttons::BUTTON_B, [this] { stepEntry(-1); }))
    return true;
  if(doIfDownAndSwallowAll(Buttons::BUTTON_C, [this] { stepEntry(1); }))
    return true;
  if(doIfDownAndSwallowAll(Buttons::BUTTON_D, [this, isShiftDown] { stepAspect(isShiftDown ? -1 : 1); }))
    return true;

  return MenuEditor::onButton(i, down, modifiers);
}

void HardwareEnableSettingsEditor::stepEntry(int inc)
{
  const auto length = static_cast<int>(tID::LENGTH);
  auto currentIdx = static_cast<int>(m_id);
  currentIdx += inc;

  if(currentIdx >= length)
    currentIdx = 0;
  if(currentIdx < 0)
    currentIdx = length - 1;

  m_id = static_cast<tID>(currentIdx);

  if(m_id == tID::ProgramChange)
  {
    if(m_aspect == tAspect::LOCAL)
    {
      m_aspect = tAspect::RECEIVE_SPLIT;
    }
  }

  update();
}

void HardwareEnableSettingsEditor::stepAspect(int inc)
{
  const auto isProgramChange = m_id == tID::ProgramChange;

  const auto totalLength = static_cast<int>(tAspect::LENGTH);
  const auto trimmedLength = totalLength - 1;
  const auto length = isProgramChange ? trimmedLength : totalLength;

  auto currentIdx = static_cast<int>(m_aspect);
  currentIdx += inc;

  if(currentIdx >= length)
    currentIdx = 0;
  if(currentIdx < 0)
    currentIdx = length - 1;

  m_aspect = static_cast<tAspect>(currentIdx);
  update();
}

void HardwareEnableSettingsEditor::update()
{
  m_aspectList->update(m_id, m_aspect);
  m_entryLabel->setText(getTextFor(m_id));
  m_aspectLabel->setText(getTextFor(m_aspect));
  m_valueLabel->setText(getValueText());
  ControlWithChildren::setDirty();
}

const Glib::ustring& HardwareEnableSettingsEditor::getTextFor(tID index)
{
  static const std::vector<Glib::ustring> sRet
      = { "Pedal 1",  "Pedal 2",  "Pedal 3",   "Pedal 4", "Bender", "Aftertouch",
          "Ribbon 1", "Ribbon 2", "Prog. Ch.", "Notes",   "LENGTH" };
  return sRet.at(static_cast<int>(index));
}

const Glib::ustring& HardwareEnableSettingsEditor::getTextFor(tAspect aspect)
{
  static const std::vector<Glib::ustring> sRet
      = { "Send Primary", "Receive Primary", "Send Split", "Receive Split", "Local", "LENGTH" };
  return sRet.at(static_cast<int>(aspect));
}

const Glib::ustring& HardwareEnableSettingsEditor::getValueText() const
{
  static std::vector<Glib::ustring> ret = { "On", "Off" };
  auto active = getSetting()->getState(m_id, m_aspect);
  return active ? ret[0] : ret[1];
}