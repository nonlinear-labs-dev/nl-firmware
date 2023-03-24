#pragma once

#include <parameters/value/QuantizedValue.h>
#include "Setting.h"

class TuneReference : public Setting
{
 public:
  explicit TuneReference(UpdateDocumentContributor& parent);

 public:
  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;
  void incDec(int inc, ButtonModifiers mod);
  void syncExternals(SendReason reason) const override;

  Glib::ustring getName() const;

 private:
  void loadDefaultValue(C15::Settings::SettingDescriptor::ValueType val) override;
  void writeDocument(Writer& writer, tUpdateID knownRevision) const override;

  QuantizedValue m_value;
};
