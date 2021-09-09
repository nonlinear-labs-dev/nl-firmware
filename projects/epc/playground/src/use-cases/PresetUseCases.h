#pragma once
#include <string>
#include <glibmm/ustring.h>
#include <presets/EditBuffer.h>

class Preset;

class PresetUseCases
{
 public:
  explicit PresetUseCases(Preset* p);

  void rename(const std::string& newName);
  void setComment(const Glib::ustring& comment);
  void setAttribute(const Glib::ustring& key, const Glib::ustring& value);

  void overwriteWithEditBuffer(EditBuffer& editBuffer);

 private:
  Preset* m_preset;
};
