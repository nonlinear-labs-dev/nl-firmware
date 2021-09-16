#include "RoutingsEditor.h"
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>
#include <proxies/hwui/controls/CenterAlignedLabel.h>
#include <proxies/hwui/controls/Button.h>
#include <cmath>
#include <use-cases/SettingsUseCases.h>
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/controls/labels/LabelStyleable.h>
#include <proxies/hwui/controls/PNGControl.h>
#include "RoutingsSettingsHelper.h"

inline auto getPreviewNameForAspect(RoutingSettings::tAspectIndex a)
{
  typedef RoutingSettings::tAspectIndex ASP;
  switch(a)
  {
    case ASP::SEND_PRIMARY:
      return "Send Prim.";
    case ASP::RECEIVE_PRIMARY:
      return "Rcv. Prim.";
    case ASP::SEND_SPLIT:
      return "Send Split";
    case ASP::RECEIVE_SPLIT:
      return "Rcv. Split";
    case ASP::LOCAL:
      return "Local";
    default:
    case ASP::LENGTH:
      return "";
  }
};

class TickMark : public PNGControl
{
 public:
  explicit TickMark(const Rect& pos);
};

TickMark::TickMark(const Rect& pos)
    : PNGControl(pos)
{
  loadImage("tickmark.png");
}

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

  const auto& pos = getPosition();
  auto y = pos.getTop();
  const auto x = pos.getLeft();
  const auto w = pos.getWidth();
  const auto totalHeight = pos.getHeight();
  const auto aspectH = totalHeight / 5;

  LabelStyleable l(getPosition());
  LabelStyle style{.size = FontSize::Size8, .decoration = FontDecoration::Regular, .justification = Font::Justification::Left};
  l.setLabelStyle(style);

  TickMark t({ 0, 0, 10, 10 });
  t.setColor(FrameBufferColors::C255);

  for(auto a : getAspectsForIndex(entry))
  {
    auto aspPos = Rect(x + paddingX, y + paddingY, w - paddingX, aspectH - paddingY);

    auto isSelected = a == aspect;
    auto isEnabled = getState(a);

    if(isSelected)
    {
      auto backGroundRect = aspPos;
      backGroundRect.setTop(aspPos.getTop() + 1);
      backGroundRect.setHeight(aspPos.getHeight() - 2);
      backGroundRect.setRight(aspPos.getRight() - 1);
      fb.setColor(FrameBufferColors::C103);
      fb.fillRect(backGroundRect);
    }

    if(isEnabled)
    {
      auto tickPos = aspPos;
      tickPos.setTop(aspPos.getTop() + 2);
      tickPos.setWidth(10);
      tickPos.setLeft(aspPos.getRight() - 10);
      t.setPosition(tickPos);
      t.redraw(fb);
    }

    {
      auto labelPos = aspPos;
      labelPos.setLeft(aspPos.getLeft() + 1);
      l.setPosition(labelPos);
      l.setText(getPreviewNameForAspect(a));
      l.setFontColor(isSelected ? FrameBufferColors::C255 : FrameBufferColors::C128);
      l.redraw(fb);
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

  std::shared_ptr<Font> getFont() const override
  {
    return Oleds::get().getFont("Emphase-8-Regular", getFontHeight());
  }

  int getFontHeight() const override
  {
    return 8;
  }
};

RoutingsEditor::RoutingsEditor(RoutingSettings::tRoutingIndex id)
    : MenuEditor()
    , m_id { id }
{
  clear();

  if(Application::exists())
  {
    auto setting = Application::get().getSettings()->getSetting<RoutingSettings>();
    setting->onChange(sigc::mem_fun(this, &RoutingsEditor::onSettingChanged));
  }

  addControl(new SetupModuleHeader({ "MIDI Routing" }, { 0, 0, 64, 13 }));

  LabelStyle entryStyle = { FontSize::Size9, FontDecoration::Bold, Font::Justification::Center };
  m_entryLabel = addControl(new LabelStyleable("Entry", { 64, 0, 128, 16 }, entryStyle));
  m_entryLabel->setHighlight(true);

  LabelStyle aspectStyle = { FontSize::Size9, FontDecoration::Regular, Font::Justification::Center };
  m_aspectLabel = addControl(new LabelStyleable("Aspect", { 64, 16, 128, 16 }, aspectStyle));

  LabelStyle valueStyle = { FontSize::Size9, FontDecoration::Regular, Font::Justification::Center };
  m_valueLabel = addControl(new LabelStyleable("Value", { 64, 32, 128, 16 }, valueStyle));
  m_valueLabel->setHighlight(true);

  m_aspectList = addControl(new AspectList(m_id, m_aspect, { 192, 0, 64, 64 }));
  addControl(new Button("Back", Buttons::BUTTON_A));
  addControl(new Button("<", Buttons::BUTTON_B));
  addControl(new Button(">", Buttons::BUTTON_C));
  update();
}

void RoutingsEditor::setPosition(const Rect& r)
{
  static const Rect menuEditorPosition(0, 0, 256, 96);
  ControlWithChildren::setPosition(menuEditorPosition);
}

void RoutingsEditor::drawBackground(FrameBuffer& fb)
{
  fb.setColor(FrameBufferColors::C128);
  fb.fillRect(getPosition());
}

void RoutingsEditor::incSetting(int inc)
{
  if(Application::exists())
  {
    static auto settings = Application::get().getSettings();
    SettingsUseCases useCases(*settings);
    auto idx = static_cast<int>(m_id);
    auto asp = static_cast<int>(m_aspect);
    useCases.updateRoutingAspect(idx, asp, inc > 0);
    update();
  }
}

const std::vector<Glib::ustring>& RoutingsEditor::getDisplayStrings() const
{
  static std::vector<Glib::ustring> unused = { "" };
  return unused;
}

int RoutingsEditor::getSelectedIndex() const
{
  constexpr auto unused = 0;
  return unused;
}

bool RoutingsEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
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

void RoutingsEditor::stepEntry(int inc)
{
  constexpr auto len = static_cast<int>(tID::LENGTH);
  static std::array<tID, len> customOrder {
    tID::Notes,  tID::ProgramChange, tID::Pedal1,     tID::Pedal2,  tID::Pedal3,
    tID::Pedal4, tID::Bender,        tID::Aftertouch, tID::Ribbon1, tID::Ribbon2
  };

  auto it = std::find(customOrder.begin(), customOrder.end(), m_id);
  auto currentIdx = std::distance(customOrder.begin(), it) + inc;

  if(currentIdx >= len)
    currentIdx = 0;
  if(currentIdx < 0)
    currentIdx = len - 1;

  m_id = customOrder[currentIdx];

  if(m_id == tID::ProgramChange)
  {
    if(m_aspect == tAspect::LOCAL)
    {
      m_aspect = tAspect::RECEIVE_SPLIT;
    }
  }

  update();
}

void RoutingsEditor::stepAspect(int inc)
{
  constexpr auto totalLength = static_cast<int>(tAspect::LENGTH);
  constexpr auto trimmedLength = totalLength - 1;

  const auto isProgramChange = m_id == tID::ProgramChange;
  const auto length = isProgramChange ? trimmedLength : totalLength;

  auto currentIdx = static_cast<int>(m_aspect) + inc;

  if(currentIdx >= length)
    currentIdx = 0;
  if(currentIdx < 0)
    currentIdx = length - 1;

  m_aspect = static_cast<tAspect>(currentIdx);
  update();
}

void RoutingsEditor::update()
{
  m_aspectList->update(m_id, m_aspect);
  m_entryLabel->setText(getTextFor(m_id));
  m_aspectLabel->setText(getTextFor(m_aspect));
  m_valueLabel->setText(getValueText());
  ControlWithChildren::setDirty();
}

const Glib::ustring& RoutingsEditor::getTextFor(tID index)
{
  static const std::vector<Glib::ustring> sRet
      = { "Pedal 1",  "Pedal 2",  "Pedal 3",   "Pedal 4", "Bender", "Aftertouch",
          "Ribbon 1", "Ribbon 2", "Prog. Ch.", "Notes",   "LENGTH" };
  return sRet.at(static_cast<int>(index));
}

const Glib::ustring& RoutingsEditor::getTextFor(tAspect aspect)
{
  static const std::vector<Glib::ustring> sRet
      = { "Send Primary", "Receive Primary", "Send Split", "Receive Split", "Local", "LENGTH" };
  return sRet.at(static_cast<int>(aspect));
}

const Glib::ustring& RoutingsEditor::getValueText() const
{
  static std::vector<Glib::ustring> ret = { "On", "Off" };
  auto active = getSetting()->getState(m_id, m_aspect);
  return active ? ret[0] : ret[1];
}