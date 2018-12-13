#pragma once

#include <serialization/Serializer.h>

class Preset;

class PresetSerializer : public Serializer
{
 public:
  PresetSerializer(Preset *preset, bool ignoreUUIDs = false);
  virtual ~PresetSerializer();

  static Glib::ustring getTagName();

 private:
  void writeTagContent(Writer &writer) const;
  void readTagContent(Reader &reader) const;

  Preset *m_preset = nullptr;
  bool m_ignoreUUIDs = false;
};
