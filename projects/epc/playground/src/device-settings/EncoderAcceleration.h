#pragma once

#include "Setting.h"
#include <parameters/value/QuantizedValue.h>

class EncoderAcceleration : public Setting
{
 private:
  typedef Setting super;

 public:
  explicit EncoderAcceleration(Settings &parent);
  ~EncoderAcceleration() override;

  void load(const Glib::ustring &text, Initiator initiator) override;
  Glib::ustring save() const override;

  void set(tControlPositionValue amount);
  tControlPositionValue get() const;
  void setDefault();
  void incDec(int incs, ButtonModifiers modifiers);
  Glib::ustring getDisplayString() const override;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;
  QuantizedValue m_acceleration;

  void syncExternals(SendReason reason) const override;
};
