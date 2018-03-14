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
  addInfoField("name", "Name", new MultiLineContent());
  addInfoField("size", "Size", new SingleLineContent());
  addInfoField("comment", "Comment", new MultiLineContent());
  addInfoField("state", "State", new SingleLineContent());
  addInfoField("dateofchange", "Last Change",  new SingleLineContent());
  addInfoField("importdate", "Import Date", new SingleLineContent());
  addInfoField("importfile", "Import File", new SingleLineContent());
  addInfoField("exportdate", "Export Date", new SingleLineContent());
  addInfoField("exportfile", "Export File", new SingleLineContent());


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
    y = info->format(y);
  }

  Rect r = getPosition ();
  r.setHeight (y);
  super::setPosition (r);
}
