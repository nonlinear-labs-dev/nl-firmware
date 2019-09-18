#pragma once

#include <parameters/value/QuantizedValue.h>
#include "Setting.h"
class TuneReference : public Setting
{
 public:
  TuneReference(Settings& parent);

 protected:
 public:
  void load(const Glib::ustring& text) override;
  ustring save() const override;
  ustring getDisplayString() const override;
  void incDec(int inc, ButtonModifiers mod);

 protected:
  QuantizedValue m_value;
};
