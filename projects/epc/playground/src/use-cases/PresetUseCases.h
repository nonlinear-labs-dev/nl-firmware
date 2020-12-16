#pragma once
#include <string>
#include <glibmm/ustring.h>

class Preset;

class PresetUseCases
{
 public:
  explicit PresetUseCases(Preset* p);

  void rename(const std::string& newName);
  void setComment(const Glib::ustring& comment);

 private:
  Preset* m_preset;
};
