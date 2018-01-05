#include <Application.h>
#include <presets/Preset.h>
#include <presets/PresetBank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetInfoContent.h>
#include <iomanip>
#include <sstream>

namespace DETAIL
{
  class PresetComment : public MultiLineLabel
  {
    public:
      PresetComment () :
          MultiLineLabel ("---")
      {
      }

      Oleds::tFont getFont ()
      {
        return Oleds::get ().getFont ("Emphase_8_TXT_Regular", 8);
      }
  };
}

static const int divider = 64;

PresetInfoContent::PresetInfoContent () :
    super (Rect (0, 0, 0, 64))
{
  addControl(m_nameLabel = new LeftAlignedLabel("Name", Rect(2, 0, divider, 16)));
  addControl (m_commentLabel = new LeftAlignedLabel ("Comment", Rect (2, 16, divider, 16)));
  addControl(m_storeTimeLabel = new LeftAlignedLabel("Last Change", Rect(2, 32, divider, 16)));
  addControl(m_deviceNameLabel = new LeftAlignedLabel("Device Name", Rect(2, 48, divider, 16)));
  addControl(m_softwareVersionLabel = new LeftAlignedLabel("UI Version", Rect(2, 64, divider, 16)));


  m_comment = addControl (new DETAIL::PresetComment ());
  m_comment->setPosition (Rect (divider, 0, 256 - divider, 0));

  m_name = addControl (new DETAIL::PresetComment ());
  m_name->setPosition (Rect (divider, 0, 256 - divider, 0));


  m_deviceName = addControl(new LeftAlignedLabel("---", Rect(divider, 32, 256 - divider, 16)));
  m_softwareVersion = addControl(new LeftAlignedLabel("---", Rect(divider, 48, 256 - divider, 16)));
  m_storeTime = addControl(new LeftAlignedLabel("---", Rect(divider, 64, 256 - divider, 16)));
  Application::get ().getPresetManager ()->onBankSelection (mem_fun (this, &PresetInfoContent::onBankChanged));
}

PresetInfoContent::~PresetInfoContent ()
{
}

void PresetInfoContent::onBankChanged (shared_ptr<PresetBank> bank)
{
  m_bankConnection.disconnect ();

  if(bank)
    m_bankConnection = bank->onBankChanged (mem_fun (this, &PresetInfoContent::onPresetSelectionChanged));
}

void PresetInfoContent::onPresetSelectionChanged ()
{
  if (auto preset = getCurrentPreset ())
  {
    connectToPreset (preset);
  }
  else if (fillDefaults ())
  {
    fixLayout ();
  }
}

void PresetInfoContent::connectToPreset (Preset* preset)
{
  m_presetConnection.disconnect ();
  m_presetConnection = preset->onPresetChanged (mem_fun (this, &PresetInfoContent::onPresetChanged));
}

void PresetInfoContent::onPresetChanged ()
{
  if (auto preset = getCurrentPreset ())
  {
    if (fillFromPreset (preset))
    {
      fixLayout ();
    }
  }
}

Preset *PresetInfoContent::getCurrentPreset ()
{
  if (auto bank = Application::get ().getPresetManager ()->getSelectedBank ())
    return bank->getPreset (bank->getSelectedPreset ()).get ();

  return nullptr;
}

bool PresetInfoContent::fillFromPreset (const Preset *preset)
{
  bool ret = m_name->setText (preset->getName (), FrameBuffer::Colors::C128);
  ret |= m_comment->setText (preset->getAttribute ("Comment", "---"), FrameBuffer::Colors::C128);
  ret |= m_storeTime->setText(localizeIsoTime(preset->getAttribute("StoreTime", "---")));
  ret |= m_deviceName->setText (preset->getAttribute ("DeviceName", "---"));
  ret |= m_softwareVersion->setText (preset->getAttribute ("SoftwareVersion", "---"));
  return ret;
}

bool PresetInfoContent::fillDefaults ()
{
  bool ret = m_name->setText ("---", FrameBuffer::Colors::C128);
  ret |= m_comment->setText ("---", FrameBuffer::Colors::C128);
  ret |= m_storeTime->setText ("---");
  ret |= m_deviceName->setText ("---");
  ret |= m_softwareVersion->setText ("---");
  return ret;
}

void PresetInfoContent::setPosition (const Rect &rect)
{
  super::setPosition (rect);
  fixLayout ();
}

const Rect &PresetInfoContent::getPosition () const
{
  return super::getPosition ();
}

void PresetInfoContent::setDirty ()
{
  super::setDirty ();
  notifyDirty (true);
}

void PresetInfoContent::fixLayout ()
{
  int y = 0;
  m_nameLabel->setPosition(Rect(0, y, divider, 16));
  m_name->setPosition(Rect (divider, y + 2, 256 - divider, m_name->getPosition().getHeight()));
  y = std::max (m_name->getPosition ().getBottom (), m_nameLabel->getPosition ().getBottom ());

  m_commentLabel->setPosition (Rect (0, y, divider, 16));
  m_comment->setPosition(Rect(divider, y + 2, 256 - divider, m_comment->getPosition().getHeight()));
  y = std::max (m_commentLabel->getPosition ().getBottom (), m_comment->getPosition ().getBottom ());

  m_storeTimeLabel->setPosition(Rect(0, y, divider, 16));
  m_storeTime->setPosition(Rect(divider, y, 256 - divider, 16));
  y += 16;

  m_deviceNameLabel->setPosition (Rect (0, y, divider, 16));
  m_deviceName->setPosition (Rect (divider, y, 256 - divider, 16));
  y += 16;

  m_softwareVersionLabel->setPosition (Rect (0, y, divider, 16));
  m_softwareVersion->setPosition (Rect (divider, y, 256 - divider, 16));
  y += 16;

  Rect r = getPosition ();
  r.setHeight (y);
  super::setPosition (r);
}

Glib::ustring PresetInfoContent::localizeIsoTime(Glib::ustring timestamp)
{
  struct tm t;

  if(!strptime(timestamp.raw().c_str(), "%Y-%m-%dT%H:%M:%S%z", &t))
    return "---";

  std::stringstream ss;
  ss << std::put_time(&t, "%x %X");
  return ss.str();
}
