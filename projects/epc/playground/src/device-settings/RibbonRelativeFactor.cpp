#include <Application.h>
#include <device-settings/DebugLevel.h>
#include <device-settings/RibbonRelativeFactor.h>
#include <device-settings/Settings.h>
#include <parameters/scale-converters/dimension/UnitlessDimension.h>
#include <parameters/scale-converters/LinearScaleConverter.h>
#include <parameters/scale-converters/ScaleConverter.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <xml/Writer.h>

class RibbonRelFactorScaleConverter : public LinearScaleConverter
{
 public:
  RibbonRelFactorScaleConverter()
      : LinearScaleConverter(tDisplayRange(1, 10), UnitlessDimension::get())
  {
  }
};

RibbonRelativeFactor::RibbonRelativeFactor(Settings &parent)
    : super(parent)
    , m_factor(nullptr, ScaleConverter::get<RibbonRelFactorScaleConverter>(), 1, 10, 100)
{
}

RibbonRelativeFactor::~RibbonRelativeFactor()
{
}

void RibbonRelativeFactor::loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val)
{
  auto f = std::get<float>(val);
  set(static_cast<double>(f));
}

void RibbonRelativeFactor::load(const Glib::ustring &text, Initiator initiator)
{
  try
  {
    set(std::stod(text));
  }
  catch(...)
  {
    set(0);
    nltools::Log::error("Could not read settings for ribbon relative factor:", text);
  }
}

Glib::ustring RibbonRelativeFactor::save() const
{
  return to_string(get());
}

void RibbonRelativeFactor::set(tControlPositionValue amount)
{
  amount = m_factor.getScaleConverter()->getControlPositionRange().clip(amount);

  m_factor.setRawValue(Initiator::INDIRECT, amount);
  notify();
}

void RibbonRelativeFactor::syncExternals(SendReason reason) const
{
  if(Application::exists())
  {
    auto proxy = Application::get().getPlaycontrollerProxy();
    auto tcdValue = proxy->ribbonRelativeFactorToTCDValue(m_factor.getQuantizedClipped());
    proxy->sendSetting(PLAYCONTROLLER_SETTING_ID_RIBBON_REL_FACTOR, tcdValue);
  }
}

void RibbonRelativeFactor::setDefault()
{
  set(m_factor.getDefaultValue());
}

tControlPositionValue RibbonRelativeFactor::get() const
{
  return m_factor.getQuantizedClipped();
}

void RibbonRelativeFactor::incDec(int incs, ButtonModifiers modifiers)
{
  while(incs > 0)
  {
    m_factor.inc(Initiator::INDIRECT, modifiers);
    incs--;
  }

  while(incs < 0)
  {
    m_factor.dec(Initiator::INDIRECT, modifiers);
    incs++;
  }

  notify();
}

Glib::ustring RibbonRelativeFactor::getDisplayString() const
{
  return m_factor.getDisplayString();
}

void RibbonRelativeFactor::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  super::writeDocument(writer, knownRevision);

  if(knownRevision == 0)
  {
    writer.writeTextElement("default", to_string(m_factor.getDefaultValue()));
    writer.writeTextElement("scaling", m_factor.getScaleConverter()->controlPositionToDisplayJS());
    writer.writeTextElement("bipolar", to_string(m_factor.isBiPolar()));
    writer.writeTextElement("coarse-denominator", to_string(m_factor.getCoarseDenominator()));
    writer.writeTextElement("fine-denominator", to_string(m_factor.getFineDenominator()));
  }
}
