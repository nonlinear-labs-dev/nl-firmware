#include <parameters/scale-converters/LinearHerzScaleConverter.h>
#include "TuneReference.h"

TuneReference::TuneReference(Settings& parent)
    : Setting(parent)
    , m_value(nullptr, ScaleConverter::get<LinearHerzScaleConverter>(), 0, 100, 1000)
{
}

void TuneReference::load(const Glib::ustring& text)
{
  m_value.setRawValue(Initiator::EXPLICIT_OTHER, std::stof(text));
}

ustring TuneReference::save() const
{
  return std::to_string(m_value.getRawValue());
}

ustring TuneReference::getDisplayString() const
{
  return std::to_string(m_value.getRawValue()) + " Hz";
}

void TuneReference::incDec(int inc, ButtonModifiers mod)
{
  if(inc > 0)
  {
    while(inc-- > 0)
      m_value.inc(Initiator::EXPLICIT_OTHER, mod);
  }
  else
  {
    while(inc++ < 0)
      m_value.dec(Initiator::EXPLICIT_OTHER, mod);
  }

  notify();
}
