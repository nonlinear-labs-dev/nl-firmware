#include <Application.h>
#include <device-settings/DateTimeAdjustment.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/setup/DateTimeEditor.h>
#include <cstdio>
#include <ctime>
#include "use-cases/SettingsUseCases.h"

DateTimeEditor::DateTimeEditor()
    : ControlWithChildren(Rect(0, 0, 0, 0))
    , m_originalTime(std::time(nullptr))
{
  const auto margin = 5;
  const auto yStart = 12;
  auto y = yStart;
  auto w = 20;

  const auto labelAndControlHeight = 10;

  m_labels[Selection::Year] = addControl<Label>("yyyy", Rect(margin, y, 2 * w, labelAndControlHeight));
  m_labels[Selection::Month] = addControl<Label>("mm", Rect(margin + 2 * w, y, w, labelAndControlHeight));
  m_labels[Selection::Day] = addControl<Label>("dd", Rect(margin + 3 * w, y, w, labelAndControlHeight));

  m_labels[Selection::Hour] = addControl<Label>("HH", Rect(margin + 4 * w, y, w, labelAndControlHeight));
  m_labels[Selection::Minute] = addControl<Label>("MM", Rect(margin + 5 * w, y, w, labelAndControlHeight));

  y = yStart * 2;

  m_controls[Selection::Year] = addControl<Label>("", Rect(margin, y, 2 * w, labelAndControlHeight));
  m_controls[Selection::Month] = addControl<Label>("", Rect(margin + 2 * w, y, w, labelAndControlHeight));
  m_controls[Selection::Day] = addControl<Label>("", Rect(margin + 3 * w, y, w, labelAndControlHeight));

  m_controls[Selection::Hour] = addControl<Label>("", Rect(margin + 4 * w, y, w, labelAndControlHeight));
  m_controls[Selection::Minute] = addControl<Label>("", Rect(margin + 5 * w, y, w, labelAndControlHeight));

  const auto buttonHeight = 11;
  const auto buttonWidth = 58;
  const auto buttonMargin = 3;

  addControl<Button>("<",
                     Rect(buttonMargin, c_fullRightSidePosition.getHeight() - buttonHeight, buttonWidth, buttonHeight));
  addControl<Button>(">",
                     Rect(buttonMargin + buttonWidth + 6, c_fullRightSidePosition.getHeight() - buttonHeight,
                          buttonWidth, buttonHeight));

  m_labels.at(m_selection)->setHighlight(true);
  m_controls.at(m_selection)->setHighlight(true);

  Application::get().getSettings()->getSetting<DateTimeAdjustment>()->onChange(
      sigc::mem_fun(this, &DateTimeEditor::onAdjustmentChanged));

  setTimeValues();
}

DateTimeEditor::~DateTimeEditor() = default;

void DateTimeEditor::setPosition(const Rect &)
{
  ControlWithChildren::setPosition(c_fullRightSidePosition);
}

void DateTimeEditor::onAdjustmentChanged(const Setting *offset)
{
  if(auto dateTimeAdjustment = dynamic_cast<const DateTimeAdjustment *>(offset))
  {
    m_diff = dateTimeAdjustment->get();
    setTimeValues();
  }
}

bool DateTimeEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_DEFAULT)
  {
    SettingsUseCases settingsUseCases(*Application::get().getSettings());
    settingsUseCases.factoryDefaultSetting(Application::get().getSettings()->getSetting<DateTimeAdjustment>());
    setTimeValues();
    return true;
  }

  if(down && (i == Buttons::BUTTON_B || i == Buttons::BUTTON_ENTER))
  {
    diveUp();
    return true;
  }

  if(down)
  {
    m_labels.at(m_selection)->setHighlight(false);
    m_controls.at(m_selection)->setHighlight(false);

    if(i == Buttons::BUTTON_C)
    {
      m_selection = step(m_selection, -1);
    }

    if(i == Buttons::BUTTON_D)
    {
      m_selection = step(m_selection, 1);
    }

    m_labels.at(m_selection)->setHighlight(true);
    m_controls.at(m_selection)->setHighlight(true);
    return true;
  }
  return false;
}

bool DateTimeEditor::onRotary(int inc, ButtonModifiers modifiers)
{
  std::time_t t = m_originalTime + static_cast<std::time_t>(m_diff);
  std::tm *tm = std::localtime(&t);

  switch(m_selection)
  {
    case Month:
      tm->tm_mon += inc;
      break;

    case Day:
      tm->tm_mday += inc;
      break;

    case Year:
      tm->tm_year += inc;
      break;

    case Hour:
      tm->tm_hour += inc;
      break;

    case Minute:
      tm->tm_min += inc;
      break;

    default:
      break;
  }

  auto modifiedTime = timelocal(tm);
  Application::get().getSettings()->getSetting<DateTimeAdjustment>()->adjust(modifiedTime);
  m_diff = modifiedTime - m_originalTime;
  setTimeValues();
  return true;
}

Glib::ustring format(int v, int numDigits)
{
  std::array<char, 64> format {};
  std::array<char, 64> txt {};
  sprintf(format.data(), "%%0%dd", numDigits);
  sprintf(txt.data(), format.data(), v);
  return { txt.data() };
}

void DateTimeEditor::setTimeValues()
{
  constexpr auto yearOffset = 1900;
  std::time_t t = std::time(nullptr) + static_cast<std::time_t>(m_diff);
  std::tm *tm = std::localtime(&t);

  m_controls[Selection::Month]->setText(format(tm->tm_mon + 1, 2));
  m_controls[Selection::Day]->setText(format(tm->tm_mday, 2));
  m_controls[Selection::Year]->setText(format(tm->tm_year + yearOffset, 4));
  m_controls[Selection::Hour]->setText(format(tm->tm_hour, 2));
  m_controls[Selection::Minute]->setText(format(tm->tm_min, 2));
  ControlWithChildren::setDirty();
}

const DateTimeEditor::Selection DateTimeEditor::step(Selection s, int inc) const
{
  auto currentRaw = static_cast<int>(s);

  if(inc > 0)
    currentRaw += inc;
  else
    currentRaw += static_cast<int>(Selection::NumFields + inc);

  return static_cast<Selection>(currentRaw % Selection::NumFields);
}
