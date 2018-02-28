#include <Application.h>
#include <presets/Preset.h>
#include <presets/PresetBank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankInfoContent.h>
#include <iomanip>
#include <sstream>
#include <device-info/DateTimeInfo.h>
#include <tools/TimeTools.h>

namespace DETAIL
{
  class BankComment : public MultiLineLabel
  {
    public:
      BankComment(Rect pos) :
          MultiLineLabel ("---")
      {
        setPosition(pos);
      }

      Oleds::tFont getFont ()
      {
        return Oleds::get ().getFont ("Emphase_8_TXT_Regular", 8);
      }
  };
}

static const int divider = 64;

BankInfoContent::BankInfoContent()
{
  addInfoField("name", new LeftAlignedLabel("Name", Rect(0, 0, divider, 16)), new DETAIL::BankComment(Rect(divider, 0, 256 - divider, 0)));
  addInfoField("size", new LeftAlignedLabel("Size", Rect(0, 16, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 16, 256 - divider, 16)));
  addInfoField("comment", new LeftAlignedLabel("Comment", Rect(0, 32, divider, 16)),
      new DETAIL::BankComment(Rect(divider, 32, 256 - divider, 0)));
  addInfoField("state", new LeftAlignedLabel("State", Rect(0, 48, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 48, 256 - divider, 16)));
  addInfoField("dateofchange", new LeftAlignedLabel("Last Change", Rect(0, 64, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 64, 256 - divider, 16)));
  addInfoField("importdate", new LeftAlignedLabel("Import Date", Rect(0, 96, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 96, 256 - divider, 16)));
  addInfoField("importfile", new LeftAlignedLabel("Import File", Rect(0, 112, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 112, 256 - divider, 16)));
  addInfoField("exportdate", new LeftAlignedLabel("Export Date", Rect(0, 124, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 124, 256 - divider, 16)));
  addInfoField("exportfile", new LeftAlignedLabel("Export File", Rect(0, 136, divider, 16)),
      new LeftAlignedLabel("---", Rect(divider, 136, 256 - divider, 16)));


  Application::get ().getPresetManager ()->onBankSelection (mem_fun (this, &BankInfoContent::onBankSelectionChanged));

  fillFromBank(Application::get().getPresetManager()->getSelectedBank().get());
}

BankInfoContent::~BankInfoContent ()
{
}

void BankInfoContent::onBankSelectionChanged (shared_ptr<PresetBank> bank)
{
  m_bankConnection.disconnect ();

  if (bank)
    m_bankConnection = bank->onBankChanged (sigc::bind (mem_fun (this, &BankInfoContent::onBankChanged), bank));
}

void BankInfoContent::onBankChanged (shared_ptr<PresetBank> bank)
{
  if (bank)
  {
    if(fillFromBank (bank.get ()))
    {
      fixLayout ();
    }
  }
  else if (fillDefaults ())
  {
    fixLayout ();
  }
}

bool BankInfoContent::fillFromBank(PresetBank *bank)
{
  infoFields["name"]->setInfo(bank->getName(true), FrameBuffer::Colors::C128);
  infoFields["size"]->setInfo(to_string(bank->getNumPresets()));
  infoFields["comment"]->setInfo(bank->getAttribute("Comment", "---"), FrameBuffer::Colors::C128);
  infoFields["state"]->setInfo(bank->calcStateString());
  infoFields["dateofchange"]->setInfo(TimeTools::getDisplayStringFromStamp(bank->getLastChangedTimestamp()));
  infoFields["importdate"]->setInfo(TimeTools::getDisplayStringFromIso(bank->getAttribute("Date of Import File", "---")));
  infoFields["importfile"]->setInfo(bank->getAttribute("Name of Import File", "---"));
  infoFields["exportdate"]->setInfo(TimeTools::getDisplayStringFromIso(bank->getAttribute("Date of Export File", "---")));
  infoFields["exportfile"]->setInfo(bank->getAttribute("Name of Export File", "---"));
  return true;
}

bool BankInfoContent::fillDefaults ()
{
  infoFields["name"]->setInfo("---", FrameBuffer::Colors::C128);
  infoFields["comment"]->setInfo("---", FrameBuffer::Colors::C128);
  infoFields["size"]->setInfo("---");
  infoFields["state"]->setInfo("Not saved by Export!");
  infoFields["dateofchange"]->setInfo("---");
  infoFields["exportdate"]->setInfo("---");
  infoFields["importdate"]->setInfo("---");
  infoFields["importfile"]->setInfo("---");
  infoFields["exportfile"]->setInfo("---");
  return true;
}

void BankInfoContent::setPosition (const Rect &rect)
{
  super::setPosition (rect);
  fixLayout ();
}

const Rect &BankInfoContent::getPosition () const
{
  return super::getPosition ();
}

void BankInfoContent::setDirty ()
{
  super::setDirty ();
  notifyDirty (true);
}

void BankInfoContent::fixLayout ()
{

  int y = 0;

  for(auto info :
  { infoFields["name"], infoFields["comment"], infoFields["size"], infoFields["state"], infoFields["dateofchange"],
      infoFields["importdate"], infoFields["importfile"], infoFields["exportdate"], infoFields["exportfile"] })
  {
    auto height = info->m_content->getHeight();
    info->m_label->setPosition(Rect(0, y, divider, 16));
    info->m_content->setPosition(Rect(divider, y, 256 - divider, height));
    y = std::max(info->m_content->getPosition().getBottom(), info->m_label->getPosition().getBottom());
  }

  Rect r = getPosition ();
  r.setHeight (y);
  super::setPosition (r);
}
